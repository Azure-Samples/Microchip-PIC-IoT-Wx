// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include "../platform/mqtt/mqtt_core/mqtt_core.h"
#include "../platform/cloud/crypto_client/hmac-sha256.h"
#include "../platform/mqtt/mqtt_packetTransfer_interface.h"
#include "mqtt_packetPopulate.h"
#include "mqtt_iotprovisioning_packetPopulate.h"
#include "../platform/config/IoT_Sensor_Node_config.h"
#include "../platform/debug_print.h"
#include "../platform/cryptoauthlib/lib/basic/atca_basic.h"
#include "../platform/drivers/timeout.h"
#include "../platform/led.h"
#include "azure/iot/az_iot_provisioning_client.h"
#include "azure/iot/az_iot_hub_client.h"

#ifdef CFG_MQTT_PROVISIONING_HOST
#define HALF_SECOND_MS timeout_mSecToTicks(500L)

/**
* @brief Provisioning polling interval.
*
* @details This is used for cases where the service does not supply a retry-after hint during the 
*          register and query operations.
*/
#ifndef PROVISIONING_POLLING_INTERVAL_S
    #define PROVISIONING_POLLING_INTERVAL_S (3U)
#endif

extern void iot_provisioning_completed(void);
pf_MQTT_CLIENT pf_mqtt_iotprovisioning_client = {
  MQTT_CLIENT_iotprovisioning_publish,
  MQTT_CLIENT_iotprovisioning_receive,
  MQTT_CLIENT_iotprovisioning_connect,
  MQTT_CLIENT_iotprovisioning_subscribe,
  MQTT_CLIENT_iotprovisioning_connected,
  NULL
};

extern const az_span device_model_id_span;
extern uint8_t device_id_buf[100];
extern az_span device_id;
uint8_t atca_dps_id_scope[12];
char hub_device_key_buf[64];
char hub_hostname_buf[128];

az_iot_provisioning_client provisioning_client;
az_iot_provisioning_client_register_response dps_register_response;
char mqtt_dsp_topic_buf[255];
az_span register_payload;
char register_payload_buf[1024];
az_span span_remainder;

static uint16_t dps_retryTimer;
static uint32_t dps_retry_task(void* payload);
static timerStruct_t dps_retry_timer = { dps_retry_task };
static uint32_t dps_assigning_task(void* payload);
static timerStruct_t dps_assigning_timer = { dps_assigning_task };

void dps_client_register(uint8_t* topic, uint8_t* payload)
{
    int rc;
    int topic_len = strlen((const char*)topic);
    int payload_len = strlen((const char*)payload);

    debug_printInfo("   DPS: %s()", __func__);

    if (az_result_failed(
            rc = az_iot_provisioning_client_parse_received_topic_and_payload(
                    &provisioning_client,
                    az_span_create(topic, topic_len),
                    az_span_create(payload, payload_len),
                    &dps_register_response)))
    {
        if ( rc != AZ_ERROR_IOT_TOPIC_NO_MATCH )
        {
            debug_printError("   DPS: az_iot_provisioning_client_parse_received_topic_and_payload fail:%d\n", rc);
        }
        else
        {
            debug_printWarn("   DPS: ignoring unknown topic.\n");
        }
    }
    else if (az_iot_provisioning_client_operation_complete (dps_register_response.operation_status))
    {
        switch (dps_register_response.operation_status)
        {
            case AZ_IOT_PROVISIONING_STATUS_ASSIGNED:
                debug_printGood("   DPS: ASSIGNED");
                timeout_delete(&dps_retry_timer);
                az_span_to_str(hub_hostname_buf, sizeof(hub_hostname_buf), dps_register_response.registration_state.assigned_hub_hostname);
                hub_hostname = hub_hostname_buf;
                // Slow Blink to indicate IoT Hub connection in progress
                LED_SetGreen(LED_STATE_BLINK_SLOW);
                LED_SetRed(LED_STATE_OFF);
                pf_mqtt_iotprovisioning_client.MQTT_CLIENT_task_completed();
                break;

            case AZ_IOT_PROVISIONING_STATUS_FAILED:
                debug_printError("   DPS: FAILED with error %lu: TrackingID: [%.*s] \"%.*s\"",
                    (unsigned long)dps_register_response.registration_state.extended_error_code,
                    (size_t)az_span_size(dps_register_response.registration_state.error_tracking_id),
                    (char *)az_span_ptr(dps_register_response.registration_state.error_tracking_id),
                    (size_t)az_span_size(dps_register_response.registration_state.error_message),
                    (char *)az_span_ptr(dps_register_response.registration_state.error_message));

                LED_SetRed(LED_STATE_BLINK_FAST);
                break;

            case AZ_IOT_PROVISIONING_STATUS_DISABLED:
                debug_printError("   DPS: device is DISABLED");
                LED_SetRed(LED_STATE_BLINK_FAST);
                break;

            default:
                debug_printError("   DPS: unexpected status: %d\n", (int)dps_register_response.operation_status);
                LED_SetRed(LED_STATE_BLINK_FAST);
                break;
        }
    }
    else // Operation is not complete.
    {
        if (dps_register_response.retry_after_seconds == 0)
        {
            dps_register_response.retry_after_seconds = PROVISIONING_POLLING_INTERVAL_S;
        }

        debug_printInfo("   DPS: ASSIGNING");
        timeout_create(&dps_assigning_timer, HALF_SECOND_MS * 2 * dps_register_response.retry_after_seconds);
    }
}

static uint32_t dps_assigning_task(void* payload)
{
    bool bRet = false; // assume failure

    if (az_result_failed(az_iot_provisioning_client_query_status_get_publish_topic(
                            &provisioning_client,
                            dps_register_response.operation_id,
                            mqtt_dsp_topic_buf,
                            sizeof(mqtt_dsp_topic_buf),
                            NULL)))
    {
        debug_printError("   DPS: az_iot_provisioning_client_query_status_get_publish_topic failed");
    }
    else
    {
        mqttPublishPacket cloudPublishPacket;
        // Fixed header
        cloudPublishPacket.publishHeaderFlags.duplicate = 0;
        cloudPublishPacket.publishHeaderFlags.qos = 0;
        cloudPublishPacket.publishHeaderFlags.retain = 1;
        // Variable header
        cloudPublishPacket.topic = (uint8_t*)mqtt_dsp_topic_buf;

        // Payload
        cloudPublishPacket.payload = NULL;
        cloudPublishPacket.payloadLength = 0;

        if (MQTT_CreatePublishPacket(&cloudPublishPacket))
        {
            bRet = true;
        }
        else
        {
            debug_printError("   DPS: Connection lost PUBLISH failed");
        }
    }

    if (!bRet)
    {
        LED_SetRed(LED_STATE_BLINK_FAST);
    }

    return 0L;
}

static uint32_t dps_retry_task(void* payload)
{
    if (++dps_retryTimer % 120 == 0)  // retry every 1 min
    {
        MQTT_CLIENT_iotprovisioning_connect((char* )device_id_buf);
        dps_retryTimer = 0;
    }
    return HALF_SECOND_MS;
}

/** \brief MQTT publish handler call back table.
 *
 * This callback table lists the callback function for to be called on reception
 * of a PUBLISH message for each topic which the application has subscribed to.
 * For each new topic which is subscribed to by the application, there needs to
 * be a corresponding publish handler.
 * E.g.: For a particular topic
 *       mchp/mySubscribedTopic/myDetailedPath
 *       Sample publish handler function  = void handlePublishMessage(uint8_t *topic, uint8_t *payload)
 */
extern publishReceptionHandler_t imqtt_publishReceiveCallBackTable[MAX_NUM_TOPICS_SUBSCRIBE];

void MQTT_CLIENT_iotprovisioning_publish(uint8_t* data, uint16_t len)
{
}

void MQTT_CLIENT_iotprovisioning_receive(uint8_t* data, uint16_t len)
{
    MQTT_GetReceivedData(data, len);
}

void MQTT_CLIENT_iotprovisioning_connect(char* deviceID)
{
    size_t mqtt_username_buf_len;
    bool bRet = false; // assume failure

    debug_printGood("   DPS: Start prov DeviceId %s", deviceID);

    LED_SetGreen(LED_STATE_BLINK_FAST);

    const az_span deviceID_parm = az_span_create_from_str(deviceID);
    az_span device_id = AZ_SPAN_FROM_BUFFER(device_id_buf);
    az_span_copy(device_id, deviceID_parm);
    device_id = az_span_slice(device_id, 0, az_span_size(deviceID_parm));

    const az_span global_device_endpoint = AZ_SPAN_LITERAL_FROM_STR(CFG_MQTT_PROVISIONING_HOST);

    // Read the ID Scope from the secure element (e.g. ATECC608A)
    atcab_read_bytes_zone(ATCA_ZONE_DATA, ATCA_SLOT_DPS_IDSCOPE, 0, atca_dps_id_scope, sizeof(atca_dps_id_scope));
    const az_span scopeID_parm = az_span_create_from_str((char*)atca_dps_id_scope);

    debug_printGood("   DPS: ID Scope=%s from secure element", atca_dps_id_scope);

    if (strlen((char*)atca_dps_id_scope) < 11)
    {
        debug_printError("   DPS: invalid IDScope");
    }
    else if (az_result_failed(az_iot_provisioning_client_init(&provisioning_client, global_device_endpoint, scopeID_parm, device_id, NULL)))
    {
        debug_printError("   DPS: az_iot_provisioning_client_init failed");
    }
    else if (az_result_failed(az_iot_provisioning_client_get_user_name(&provisioning_client, mqtt_username_buf, sizeof(mqtt_username_buf), &mqtt_username_buf_len)))
    {
        debug_printError("   DPS: az_iot_provisioning_client_get_user_name failed");
    }
    else
    {
        mqttConnectPacket cloudConnectPacket = { 0 };
        cloudConnectPacket.connectVariableHeader.connectFlagsByte.cleanSession = 1;
        cloudConnectPacket.connectVariableHeader.keepAliveTimer = AZ_IOT_DEFAULT_MQTT_CONNECT_KEEPALIVE_SECONDS;
        cloudConnectPacket.clientID = (uint8_t*) az_span_ptr(device_id);
        cloudConnectPacket.password = NULL;
        cloudConnectPacket.passwordLength = 0;
        cloudConnectPacket.username = (uint8_t*) mqtt_username_buf;
        cloudConnectPacket.usernameLength = (uint16_t) mqtt_username_buf_len;

        debug_printInfo("   DPS: ConnectPacket username(%d): %s", mqtt_username_buf_len, mqtt_username_buf);

        if (MQTT_CreateConnectPacket(&cloudConnectPacket))
        {
            bRet = true;
        }
    }

    if (!bRet)
    {
        LED_SetRed(LED_STATE_BLINK_FAST);
    }
}

bool MQTT_CLIENT_iotprovisioning_subscribe()
{

    mqttSubscribePacket cloudSubscribePacket = { 0 };
    // Variable header
    cloudSubscribePacket.packetIdentifierLSB = 1;
    cloudSubscribePacket.packetIdentifierMSB = 0;

    cloudSubscribePacket.subscribePayload[0].topic = (uint8_t*) AZ_IOT_PROVISIONING_CLIENT_REGISTER_SUBSCRIBE_TOPIC;
    cloudSubscribePacket.subscribePayload[0].topicLength = sizeof(AZ_IOT_PROVISIONING_CLIENT_REGISTER_SUBSCRIBE_TOPIC) - 1;
    cloudSubscribePacket.subscribePayload[0].requestedQoS = 0;

    memset(imqtt_publishReceiveCallBackTable, 0, sizeof(imqtt_publishReceiveCallBackTable));
    imqtt_publishReceiveCallBackTable[0].topic = (uint8_t*) AZ_IOT_PROVISIONING_CLIENT_REGISTER_SUBSCRIBE_TOPIC;
    imqtt_publishReceiveCallBackTable[0].mqttHandlePublishDataCallBack = dps_client_register;
    MQTT_SetPublishReceptionHandlerTable(imqtt_publishReceiveCallBackTable);

    if (MQTT_CreateSubscribePacket(&cloudSubscribePacket) != true)
    {
        debug_printError("   DPS: failed to create subscribe packet");
        LED_SetRed(LED_STATE_BLINK_FAST);
        return false;
    }
    return true;
}

void MQTT_CLIENT_iotprovisioning_connected()
{
    bool bRet = false; // assume failure

    debug_printGood("   DPS: Connected.  Announcing DTMI '%s'", az_span_ptr(device_model_id_span));

    if (az_result_failed(az_iot_provisioning_client_register_get_publish_topic(&provisioning_client, mqtt_dsp_topic_buf, sizeof(mqtt_dsp_topic_buf), NULL)))
    {
        debug_printError("az_iot_provisioning_client_register_get_publish_topic failed");
    }
    else
    {
        mqttPublishPacket cloudPublishPacket = { 0 };
        // Fixed header
        cloudPublishPacket.publishHeaderFlags.qos = 0;
        cloudPublishPacket.publishHeaderFlags.retain = 0;
        // Variable header
        cloudPublishPacket.topic = (uint8_t*)mqtt_dsp_topic_buf;

        // Payload
        register_payload = az_span_create((uint8_t*)register_payload_buf, sizeof(register_payload_buf));
        span_remainder = az_span_copy(register_payload, az_span_create_from_str("{\"payload\":{\"modelId\":\""));
        span_remainder = az_span_copy(span_remainder, device_model_id_span);
        span_remainder = az_span_copy(span_remainder, az_span_create_from_str("\"}}"));
        az_span_copy_u8(span_remainder, '\0');

        cloudPublishPacket.payload = (uint8_t*)register_payload_buf;
        cloudPublishPacket.payloadLength = strlen(register_payload_buf);

        if (MQTT_CreatePublishPacket(&cloudPublishPacket) != true)
        {
            debug_printError("   DPS: Connection lost PUBLISH failed");
        }
        else
        {
            bRet = true;
        }

        // keep retrying connecting to DPS
        dps_retryTimer = 0;
        timeout_create(&dps_retry_timer, HALF_SECOND_MS);
    }

    if (!bRet)
    {
        LED_SetRed(LED_STATE_BLINK_FAST);
    }
}

#endif // CFG_MQTT_PROVISIONING_HOST
