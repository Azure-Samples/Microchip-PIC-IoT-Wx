// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include "../platform/mqtt/mqtt_core/mqtt_core.h"
#include "../platform/mqtt/mqtt_packetTransfer_interface.h"
#include "mqtt_packetPopulate.h"
#include "mqtt_iothub_packetPopulate.h"
#include "../platform/config/IoT_Sensor_Node_config.h"
#include "../platform/debug_print.h"
#include "../platform/cryptoauthlib/lib/basic/atca_basic.h"
#include <azure/iot/az_iot_hub_client.h>

pf_MQTT_CLIENT pf_mqtt_iothub_client = {
  MQTT_CLIENT_iothub_publish,
  MQTT_CLIENT_iothub_receive,
  MQTT_CLIENT_iothub_connect,
  MQTT_CLIENT_iothub_subscribe,
  MQTT_CLIENT_iothub_connected,  
  NULL
};

extern const az_span device_model_id;
extern void receivedFromCloud_methods(uint8_t* topic, uint8_t* payload);
extern void receivedFromCloud_twin(uint8_t* topic, uint8_t* payload);
extern void receivedFromCloud_patch(uint8_t* topic, uint8_t* payload);
static const az_span twin_request_id = AZ_SPAN_LITERAL_FROM_STR("initial_get");

char mqtt_telemetry_topic_buf[64];
char mqtt_get_topic_twin_buf[64];
uint8_t device_id_buf[100];
az_span device_id;
az_iot_hub_client hub_client;

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
publishReceptionHandler_t imqtt_publishReceiveCallBackTable[MAX_NUM_TOPICS_SUBSCRIBE];

void MQTT_CLIENT_iothub_publish(uint8_t* data, uint16_t len)
{
    uint8_t properties_buf[256];
    az_span properties = AZ_SPAN_FROM_BUFFER(properties_buf);
    az_iot_message_properties properties_topic;
    az_result result = az_iot_message_properties_init(&properties_topic, properties, 0);
    if (az_result_failed(result))
    {
        debug_printError("az_iot_hub_client_properties_init failed");
        return;
    }

    result = az_iot_message_properties_append(&properties_topic, AZ_SPAN_FROM_STR("foo"), AZ_SPAN_FROM_STR("bar"));
    if (az_result_failed(result))
    {
        debug_printError("az_iot_hub_client_properties_append failed");
        return;
    }

    result = az_iot_hub_client_telemetry_get_publish_topic(
        &hub_client, &properties_topic, mqtt_telemetry_topic_buf, sizeof(mqtt_telemetry_topic_buf), NULL);

    if (az_result_failed(result))
    {
        debug_printError("az_iot_hub_client_telemetry_get_publish_topic failed");
        return;
    }

    mqttPublishPacket cloudPublishPacket;
    // Fixed header
    cloudPublishPacket.publishHeaderFlags.duplicate = 0;
    cloudPublishPacket.publishHeaderFlags.qos = 1;
    cloudPublishPacket.publishHeaderFlags.retain = 0;
    // Variable header
    cloudPublishPacket.topic = (uint8_t*)mqtt_telemetry_topic_buf;

    // Payload
    cloudPublishPacket.payload = data;
    cloudPublishPacket.payloadLength = len;

    if (MQTT_CreatePublishPacket(&cloudPublishPacket) != true)
    {
        debug_printError("MQTT: Connection lost PUBLISH failed");
    }
}

void MQTT_CLIENT_iothub_receive(uint8_t* data, uint16_t len)
{
    MQTT_GetReceivedData(data, len);
}

void MQTT_CLIENT_iothub_connect(char* deviceID)
{
    const az_span iothub_hostname = az_span_create_from_str(hub_hostname);
    const az_span deviceID_parm = az_span_create_from_str(deviceID);
        az_span device_id = AZ_SPAN_FROM_BUFFER(device_id_buf);
    az_span_copy(device_id, deviceID_parm);
    device_id = az_span_slice(device_id, 0, az_span_size(deviceID_parm));

    az_iot_hub_client_options options = az_iot_hub_client_options_default();
    options.model_id = device_model_id;
    az_result result = az_iot_hub_client_init(&hub_client, iothub_hostname, device_id, &options);
    if (az_result_failed(result))
    {
        debug_printError("az_iot_hub_client_init failed");
        return;
    }

    size_t mqtt_username_buf_len;
    result = az_iot_hub_client_get_user_name(&hub_client, mqtt_username_buf, sizeof(mqtt_username_buf), &mqtt_username_buf_len);
    if (az_result_failed(result))
    {
        debug_printError("az_iot_hub_client_get_user_name failed");
        return;
    }

#ifdef SAS
    time_t expire_time = time(NULL) + 60 * 60; // token expires in 1 hour
    uint8_t signature_buf[256];
    az_span signature = AZ_SPAN_FROM_BUFFER(signature_buf);
    result = az_iot_hub_client_sas_get_signature(&hub_client, expire_time, signature, &signature); 
    if (az_failed(result))
    {
        debug_printError("az_iot_hub_client_sas_get_signature failed");
        return;
    }

    uint8_t key[32];
    size_t key_size = sizeof(key);
    atcab_base64decode_(hub_device_key, strlen(hub_device_key), key, &key_size, az_iot_b64rules);
    atcab_nonce(key);
    uint8_t hash[32];
    ATCA_STATUS atca_status = atcab_sha_hmac(signature_buf, az_span_size(signature), ATCA_TEMPKEY_KEYID, hash, SHA_MODE_TARGET_OUT_ONLY);
    if (atca_status != ATCA_SUCCESS)
    {
        debug_printError("atcab_sha_hmac failed");
        return;
    }

    char signature_hash_buf[64];
    key_size = sizeof(signature_hash_buf);
    atcab_base64encode_(hash, sizeof(hash), signature_hash_buf, &key_size, az_iot_b64rules);

    size_t mqtt_password_buf_len;
    az_span signature_hash = az_span_create_from_str(signature_hash_buf);
    result = az_iot_hub_client_sas_get_password(&hub_client, signature_hash, expire_time, AZ_SPAN_NULL, mqtt_password_buf, sizeof(mqtt_password_buf), &mqtt_password_buf_len);
    if (az_failed(result))
    {
        debug_printError("az_iot_hub_client_sas_get_password failed");
        return;
    }   
#endif

    mqttConnectPacket cloudConnectPacket;
    memset(&cloudConnectPacket, 0, sizeof(mqttConnectPacket));
    cloudConnectPacket.connectVariableHeader.connectFlagsByte.cleanSession = 0; 
    cloudConnectPacket.connectVariableHeader.keepAliveTimer = AZ_IOT_DEFAULT_MQTT_CONNECT_KEEPALIVE_SECONDS;

    cloudConnectPacket.clientID = az_span_ptr(device_id);
    
#ifdef SAS
    cloudConnectPacket.password = (uint8_t*)mqtt_password_buf;
    cloudConnectPacket.passwordLength = mqtt_password_buf_len;
#else
    cloudConnectPacket.password = NULL;
    cloudConnectPacket.passwordLength = 0;
#endif
    
    cloudConnectPacket.username = (uint8_t*)mqtt_username_buf;
    cloudConnectPacket.usernameLength = (uint16_t)mqtt_username_buf_len;

    MQTT_CreateConnectPacket(&cloudConnectPacket);
}

bool MQTT_CLIENT_iothub_subscribe()
{
    mqttSubscribePacket cloudSubscribePacket = { 0 };
    // Variable header   
    cloudSubscribePacket.packetIdentifierLSB = 1;
    cloudSubscribePacket.packetIdentifierMSB = 0;

    cloudSubscribePacket.subscribePayload[0].topic = (uint8_t*) AZ_IOT_HUB_CLIENT_METHODS_SUBSCRIBE_TOPIC;
    cloudSubscribePacket.subscribePayload[0].topicLength = sizeof(AZ_IOT_HUB_CLIENT_METHODS_SUBSCRIBE_TOPIC) - 1;
    cloudSubscribePacket.subscribePayload[0].requestedQoS = 0;
    cloudSubscribePacket.subscribePayload[1].topic = (uint8_t*) AZ_IOT_HUB_CLIENT_TWIN_PATCH_SUBSCRIBE_TOPIC;
    cloudSubscribePacket.subscribePayload[1].topicLength = sizeof(AZ_IOT_HUB_CLIENT_TWIN_PATCH_SUBSCRIBE_TOPIC) - 1;
    cloudSubscribePacket.subscribePayload[1].requestedQoS = 0;
    cloudSubscribePacket.subscribePayload[2].topic = (uint8_t*) AZ_IOT_HUB_CLIENT_TWIN_RESPONSE_SUBSCRIBE_TOPIC;
    cloudSubscribePacket.subscribePayload[2].topicLength = sizeof(AZ_IOT_HUB_CLIENT_TWIN_RESPONSE_SUBSCRIBE_TOPIC) - 1;
    cloudSubscribePacket.subscribePayload[2].requestedQoS = 0;

    memset(imqtt_publishReceiveCallBackTable, 0, sizeof(imqtt_publishReceiveCallBackTable));
    imqtt_publishReceiveCallBackTable[0].topic = (uint8_t*) AZ_IOT_HUB_CLIENT_METHODS_SUBSCRIBE_TOPIC;
    imqtt_publishReceiveCallBackTable[0].mqttHandlePublishDataCallBack = receivedFromCloud_methods;
    imqtt_publishReceiveCallBackTable[1].topic = (uint8_t*) AZ_IOT_HUB_CLIENT_TWIN_PATCH_SUBSCRIBE_TOPIC;
    imqtt_publishReceiveCallBackTable[1].mqttHandlePublishDataCallBack = receivedFromCloud_patch;
    imqtt_publishReceiveCallBackTable[2].topic = (uint8_t*) AZ_IOT_HUB_CLIENT_TWIN_RESPONSE_SUBSCRIBE_TOPIC;
    imqtt_publishReceiveCallBackTable[2].mqttHandlePublishDataCallBack = receivedFromCloud_twin;
    MQTT_SetPublishReceptionHandlerTable(imqtt_publishReceiveCallBackTable);

    bool ret = MQTT_CreateSubscribePacket(&cloudSubscribePacket);
    if (ret == true)
    {
        debug_printInfo("MQTT: SUBSCRIBE packet created");
    }

    return ret;
}

void MQTT_CLIENT_iothub_connected()
{
    // get the current state of the device twin

    az_result result = az_iot_hub_client_twin_document_get_publish_topic(&hub_client, twin_request_id, mqtt_get_topic_twin_buf, sizeof(mqtt_get_topic_twin_buf), NULL);
    if (az_result_failed(result))
    {
        debug_printError("az_iot_hub_client_twin_document_get_publish_topic failed");
        return;
    }

    mqttPublishPacket cloudPublishPacket;
    // Fixed header
    cloudPublishPacket.publishHeaderFlags.duplicate = 0;
    cloudPublishPacket.publishHeaderFlags.qos = 0;
    cloudPublishPacket.publishHeaderFlags.retain = 0;
    // Variable header
    cloudPublishPacket.topic = (uint8_t*)mqtt_get_topic_twin_buf;

    // Payload
    cloudPublishPacket.payload = NULL;
    cloudPublishPacket.payloadLength = 0;

    if (MQTT_CreatePublishPacket(&cloudPublishPacket) != true)
    {
        debug_printError("MQTT: Connection lost PUBLISH failed");
    }
}
