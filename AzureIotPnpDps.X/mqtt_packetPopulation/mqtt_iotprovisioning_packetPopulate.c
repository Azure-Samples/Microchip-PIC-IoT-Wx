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
#define HALF_SECOND 1000L

pf_MQTT_CLIENT pf_mqtt_iotprovisioning_client = {
  MQTT_CLIENT_iotprovisioning_publish,
  MQTT_CLIENT_iotprovisioning_receive,
  MQTT_CLIENT_iotprovisioning_connect,
  MQTT_CLIENT_iotprovisioning_subscribe,
  MQTT_CLIENT_iotprovisioning_connected,  
  NULL
};

extern const az_span device_model_id;
extern uint8_t device_id_buf[100];
extern az_span device_id;
char hub_device_key_buf[64];
char hub_hostname_buf[128];

az_iot_provisioning_client provisioning_client;
az_iot_provisioning_client_register_response dps_register_response;
char mqtt_dsp_topic_buf[200];
char register_payload_buf[512];
char register_topic_buf[64];

static uint16_t dps_retryTimer;
static uint32_t dps_retry_task(void* payload);
static timerStruct_t dps_retry_timer = { dps_retry_task };
static uint32_t dps_assigning_task(void* payload);
static timerStruct_t dps_assigning_timer = { dps_assigning_task };

void dps_client_register(uint8_t* topic, uint8_t* payload)
{
    int rc;
    strncpy(register_topic_buf, (char*)topic, sizeof(register_topic_buf));
    strncpy(register_payload_buf, (char*)payload, sizeof(register_payload_buf));
    register_topic_buf[sizeof(register_topic_buf)-1] = '\0';
    register_payload_buf[sizeof(register_payload_buf)-1] = '\0';
    int topic_len = strlen(register_topic_buf);
    int payload_len = strlen(register_payload_buf);
    
    if (az_failed(
            rc = az_iot_provisioning_client_parse_received_topic_and_payload(
                &provisioning_client,
                az_span_create(topic, topic_len),
                az_span_create(payload, payload_len),
                &dps_register_response)))
    {
      debug_printError("az_iot_provisioning_client_parse_received_topic_and_payload failed, return code %d\n", rc);
      return;
    }

    az_iot_provisioning_client_operation_status operation_status;
    if (az_failed(
            rc = az_iot_provisioning_client_parse_operation_status(&dps_register_response, &operation_status)))
    {   
        debug_printError("az_iot_provisioning_client_parse_operation_status failed, return code %d\n", rc);
        return;
    }
    
    switch (operation_status)
    {       
        case AZ_IOT_PROVISIONING_STATUS_ASSIGNING:
            timeout_create(&dps_assigning_timer, HALF_SECOND * 2 * dps_register_response.retry_after_seconds);            
            break;

        case AZ_IOT_PROVISIONING_STATUS_ASSIGNED:
            LED_holdRedOn(LED_OFF);
            timeout_delete(&dps_retry_timer); 
            az_span_to_str(hub_hostname_buf, sizeof(hub_hostname_buf), dps_register_response.registration_result.assigned_hub_hostname);
            hub_hostname = hub_hostname_buf;
            pf_mqtt_iotprovisioning_client.MQTT_CLIENT_task_completed();
            break;
            
        case AZ_IOT_PROVISIONING_STATUS_FAILED:
        case AZ_IOT_PROVISIONING_STATUS_DISABLED:            
        default:
            LED_holdRedOn(LED_ON);
            break;
    }
}

static uint32_t dps_assigning_task(void* payload)
{
    int rc; 
    if (az_failed(
            rc = az_iot_provisioning_client_query_status_get_publish_topic(&provisioning_client, &dps_register_response, mqtt_dsp_topic_buf, sizeof(mqtt_dsp_topic_buf), NULL)))
    {   
        debug_printError("az_iot_provisioning_client_query_status_get_publish_topic failed");
        return 0L;
    }
    
    mqttPublishPacket cloudPublishPacket;
    // Fixed header
    cloudPublishPacket.publishHeaderFlags.duplicate = AZ_HUB_CLIENT_DEFAULT_MQTT_TELEMETRY_DUPLICATE;
    cloudPublishPacket.publishHeaderFlags.qos = AZ_HUB_CLIENT_DEFAULT_MQTT_TELEMETRY_QOS;
    cloudPublishPacket.publishHeaderFlags.retain = AZ_HUB_CLIENT_DEFAULT_MQTT_TELEMETRY_RETAIN;
    // Variable header
    cloudPublishPacket.topic = (uint8_t*)mqtt_dsp_topic_buf;

    // Payload
    cloudPublishPacket.payload = NULL;
    cloudPublishPacket.payloadLength = 0;

    if (MQTT_CreatePublishPacket(&cloudPublishPacket) != true)
    {
        debug_printError("MQTT: Connection lost PUBLISH failed");
    }
    return 0L;
}

static uint32_t dps_retry_task(void* payload)
{
    if (++dps_retryTimer % 240 > 0)  // retry every 2 mins
        return HALF_SECOND;
    
    LED_holdRedOn(LED_ON);
    MQTT_CLIENT_iotprovisioning_connect((char* )device_id_buf);
    return 0L;
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
    const az_span deviceID_parm = az_span_create_from_str(deviceID);
    az_span device_id = AZ_SPAN_FROM_BUFFER(device_id_buf);
    az_span_copy(device_id, deviceID_parm);
    device_id = az_span_slice(device_id, 0, az_span_size(deviceID_parm));
   
    const az_span global_device_endpoint = AZ_SPAN_LITERAL_FROM_STR(CFG_MQTT_PROVISIONING_HOST);
    const az_span id_scope = AZ_SPAN_LITERAL_FROM_STR(PROVISIONING_ID_SCOPE);
    az_result result = az_iot_provisioning_client_init(&provisioning_client, global_device_endpoint, id_scope, device_id, NULL);
    if (az_failed(result))
    {
        debug_printError("az_iot_provisioning_client_init failed");
        return;
    }
    
    size_t mqtt_username_buf_len;
    result = az_iot_provisioning_client_get_user_name(&provisioning_client, mqtt_username_buf, sizeof(mqtt_username_buf), &mqtt_username_buf_len);
    if (az_failed(result))
    {
        debug_printError("az_iot_provisioning_client_get_user_name failed");
        return;
    }
    
    mqttConnectPacket cloudConnectPacket;
    memset(&cloudConnectPacket, 0, sizeof(mqttConnectPacket));
    cloudConnectPacket.connectVariableHeader.connectFlagsByte.cleanSession = 1; 
    cloudConnectPacket.connectVariableHeader.keepAliveTimer = AZ_IOT_DEFAULT_MQTT_CONNECT_KEEPALIVE_SECONDS;    
    cloudConnectPacket.clientID = (uint8_t*) az_span_ptr(device_id);
    cloudConnectPacket.password = NULL;
    cloudConnectPacket.passwordLength = 0;
    cloudConnectPacket.username = (uint8_t*) mqtt_username_buf;
    cloudConnectPacket.usernameLength = (uint16_t) mqtt_username_buf_len;

    MQTT_CreateConnectPacket(&cloudConnectPacket);
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

    bool ret = MQTT_CreateSubscribePacket(&cloudSubscribePacket);
    if (ret == true)
    {
        debug_printInfo("MQTT: SUBSCRIBE packet created");
    }

    return ret;
}

void MQTT_CLIENT_iotprovisioning_connected()
{
    az_result result = az_iot_provisioning_client_register_get_publish_topic(&provisioning_client, mqtt_dsp_topic_buf, sizeof(mqtt_dsp_topic_buf), NULL);
    if (az_failed(result))
    {
        debug_printError("az_iot_provisioning_client_register_get_publish_topic failed");
        return;
    }

    mqttPublishPacket cloudPublishPacket;
    // Fixed header
    //cloudPublishPacket.publishHeaderFlags.duplicate = AZ_HUB_CLIENT_DEFAULT_MQTT_TELEMETRY_DUPLICATE;
    cloudPublishPacket.publishHeaderFlags.qos = 1;
    cloudPublishPacket.publishHeaderFlags.retain = 0;
    // Variable header
    cloudPublishPacket.topic = (uint8_t*)mqtt_dsp_topic_buf;

    // Payload
    az_span register_payload = az_span_create((uint8_t*)register_payload_buf, sizeof(register_payload_buf));
    az_span remainder = az_span_copy(register_payload, az_span_create_from_str("{\"payload\":{\"modelId\":\""));
    remainder = az_span_copy(remainder, device_model_id);
    remainder = az_span_copy(remainder, az_span_create_from_str("\"}}"));
    az_span_copy_u8(remainder, '\0');
    
    cloudPublishPacket.payload = register_payload_buf;
    cloudPublishPacket.payloadLength = strlen(register_payload_buf);

    if (MQTT_CreatePublishPacket(&cloudPublishPacket) != true)
    {
        debug_printError("MQTT: Connection lost PUBLISH failed");
    }
    
    // keep retrying connecting to DPS
    dps_retryTimer = 0; 
    timeout_create(&dps_retry_timer, HALF_SECOND);
}

#endif // CFG_MQTT_PROVISIONING_HOST
