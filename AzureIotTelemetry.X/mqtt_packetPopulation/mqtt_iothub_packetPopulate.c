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
#include <azure/iot/az_iot_common.h>
#include <azure/iot/az_iot_hub_client.h>

pf_MQTT_CLIENT pf_mqtt_pnp_client = {
  MQTT_CLIENT_pnp_publish,
  MQTT_CLIENT_pnp_receive,
  MQTT_CLIENT_pnp_connect,
  MQTT_CLIENT_pnp_subscribe,
  MQTT_CLIENT_pnp_connected,
  NULL
};

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

void MQTT_CLIENT_pnp_publish(uint8_t* data, uint16_t len)
{
    az_result result = az_iot_hub_client_telemetry_get_publish_topic(
        &hub_client, NULL, mqtt_telemetry_topic_buf, sizeof(mqtt_telemetry_topic_buf), NULL);

    if (az_result_failed(result))
    {
        debug_printError("az_iot_hub_client_telemetry_get_publish_topic failed");
        return;
    }

    static uint16_t packetIdentifier;
    packetIdentifier++;
    mqttPublishPacket cloudPublishPacket;
    // Fixed header
    cloudPublishPacket.publishHeaderFlags.duplicate = 0;
    cloudPublishPacket.publishHeaderFlags.qos = 1;
    cloudPublishPacket.publishHeaderFlags.retain = 0;
    cloudPublishPacket.packetIdentifierLSB = packetIdentifier & 0xff;
    cloudPublishPacket.packetIdentifierMSB = packetIdentifier >> 8;
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

void MQTT_CLIENT_pnp_receive(uint8_t* data, uint16_t len)
{
    MQTT_GetReceivedData(data, len);
}

void MQTT_CLIENT_pnp_connect(char* deviceID)
{
    const az_span iothub_hostname = az_span_create_from_str(hub_hostname);
    const az_span deviceID_parm = az_span_create_from_str(deviceID);
        az_span device_id = AZ_SPAN_FROM_BUFFER(device_id_buf);
    az_span_copy(device_id, deviceID_parm);
    device_id = az_span_slice(device_id, 0, az_span_size(deviceID_parm));

    az_result result = az_iot_hub_client_init(&hub_client, iothub_hostname, device_id, NULL);
    if (az_result_failed(result))
    {
        debug_printError("az_iot_hub_client_init failed");
        return;
    }

    size_t mqtt_username_buf_len;
    result = az_iot_hub_client_get_user_name(&hub_client, mqtt_username_buf, sizeof(mqtt_username_buf), &mqtt_username_buf_len);
    if (az_result_failed(result))
    {
      debug_printError("az_iot_hub_client_sas_get_signature failed");
      return;
    }
#ifdef SAS
    time_t expire_time = time(NULL) + 60 * 60; // token expires in 1 hour
    uint8_t signature_buf[256];
    az_span signature = AZ_SPAN_FROM_BUFFER(signature_buf);
    result = az_iot_hub_client_sas_get_signature(&pnp_client, expire_time, signature, &signature); 
    if (az_result_failed(result))
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
    result = az_iot_hub_client_sas_get_password(&pnp_client, signature_hash, expire_time, AZ_SPAN_NULL, mqtt_password_buf, sizeof(mqtt_password_buf), &mqtt_password_buf_len);
    if (az_result_failed(result))
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

bool MQTT_CLIENT_pnp_subscribe()
{
    return true;
}

void MQTT_CLIENT_pnp_connected()
{
    
}
