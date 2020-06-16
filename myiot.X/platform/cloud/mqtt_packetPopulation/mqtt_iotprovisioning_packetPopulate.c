// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include "../../mqtt/mqtt_core/mqtt_core.h"
#include "../crypto_client/hmac-sha256.h"
#include "../../mqtt/mqtt_packetTransfer_interface.h"
#include "mqtt_packetPopulate.h"
#include "mqtt_iotprovisioning_packetPopulate.h"
#include "../../config/IoT_Sensor_Node_config.h"
#include "../../debug_print.h"
#include "../../../platform/cryptoauthlib/lib/basic/atca_basic.h"
#include "../../../platform/drivers/timeout.h"
#include "../../../platform/led.h"
#include "azure/iot/az_iot_provisioning_client.h"
#include "azure/iot/az_iot_hub_client.h"

#ifdef CFG_MQTT_PROVISIONING_HOST
#define HALF_SECOND 1000L

pf_MQTT_CLIENT pf_mqqt_iotprovisioning_client = {
  MQTT_CLIENT_iotprovisioning_publish,
  MQTT_CLIENT_iotprovisioning_receive,
  MQTT_CLIENT_iotprovisioning_connect,
  MQTT_CLIENT_iotprovisioning_subscribe,
  MQTT_CLIENT_iotprovisioning_connected,  
  NULL
};

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
                az_span_init(topic, topic_len),
                az_span_init(payload, payload_len),
                &dps_register_response)))
    {
      debug_printError("az_iot_provisioning_client_parse_received_topic_and_payload failed");
      return;
    }

    az_iot_provisioning_client_operation_status operation_status;
    if (az_failed(
            rc = az_iot_provisioning_client_parse_operation_status(&dps_register_response, &operation_status)))
    {   
        debug_printError("az_iot_provisioning_client_parse_operation_status failed");
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
            pf_mqqt_iotprovisioning_client.MQTT_CLIENT_task_completed();
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
    debug_printError("az_iot_provisioning_client_parse_operation_status failed");
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
    //TODO: revised logic for x509 flow.  currently just get it to compile
	const az_span deviceID_parm = az_span_from_str(deviceID);
	az_span device_id = AZ_SPAN_FROM_BUFFER(device_id_buf);
	az_span_copy(device_id, deviceID_parm);
	device_id = az_span_slice(device_id, 0, az_span_size(deviceID_parm));

    uint8_t enrollments_group_key[64];
	size_t enrollments_group_key_size = sizeof(enrollments_group_key);
    atcab_base64decode_(provisioning_enrollment_key, strlen(provisioning_enrollment_key), enrollments_group_key, &enrollments_group_key_size, az_iot_b64rules);
    
//    uint8_t device_key[32];
//    hmac_sha256(device_key, device_id_buf, strlen((char*)device_id_buf), enrollments_group_key, enrollments_group_key_size);

//    memset(hub_device_key_buf, 0, sizeof(hub_device_key_buf));
//	size_t hub_device_key_buf_len = sizeof(hub_device_key_buf);
//	atcab_base64encode_(device_key, sizeof(device_key), (char*)hub_device_key_buf, &hub_device_key_buf_len, az_iot_b64rules);
//    hub_device_key = hub_device_key_buf;
    
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

    char mqtt_username_encoded_buf[256];
	url_encode_rfc3986(mqtt_username_buf, mqtt_username_encoded_buf, _az_COUNTOF(mqtt_username_encoded_buf));
    
	time_t expire_time = time(NULL) + 60 * 60; // token expires in 1 hour  
	uint8_t signature_buf[128];
	az_span signature = AZ_SPAN_FROM_BUFFER(signature_buf);
	result = az_iot_provisioning_client_sas_get_signature(&provisioning_client, expire_time, signature, &signature);  
	if (az_failed(result))
	{
		debug_printError("az_iot_provisioning_client_sas_get_signature failed");
		return;
	}

//    uint8_t sas_hash[32];
//	atcab_nonce(device_key);
//	ATCA_STATUS atca_status = atcab_sha_hmac(signature_buf, az_span_size(signature), ATCA_TEMPKEY_KEYID, sas_hash, SHA_MODE_TARGET_OUT_ONLY);
//	if (atca_status != ATCA_SUCCESS)
//	{
//		debug_printError("atcab_sha_hmac failed");
//		return;
//	}
    
//	char signature_hash_buf[64];
//	size_t key_size = _az_COUNTOF(signature_hash_buf);
//	atcab_base64encode_(sas_hash, sizeof(sas_hash), signature_hash_buf, &key_size, az_iot_b64rules);

//	char signature_hash_encoded_buf[256];
//	url_encode_rfc3986(signature_hash_buf, signature_hash_encoded_buf, _az_COUNTOF(signature_hash_encoded_buf));

	size_t mqtt_password_buf_len;
//	az_span signature_hash_encoded = az_span_from_str(signature_hash_encoded_buf);
//	result = az_iot_provisioning_client_sas_get_password(&provisioning_client, signature_hash_encoded, expire_time, az_span_from_str("registration"), mqtt_password_buf, sizeof(mqtt_password_buf), &mqtt_password_buf_len);
//	if (az_failed(result))
//	{
//		debug_printError("az_iot_provisioning_client_sas_get_password failed");
//		return;
//	}
    
	mqttConnectPacket cloudConnectPacket;
	memset(&cloudConnectPacket, 0, sizeof(mqttConnectPacket));
	cloudConnectPacket.connectVariableHeader.connectFlagsByte.cleanSession = 1; 
	cloudConnectPacket.connectVariableHeader.keepAliveTimer = AZ_IOT_DEFAULT_MQTT_CONNECT_KEEPALIVE_SECONDS;    
    cloudConnectPacket.clientID = (uint8_t*) az_span_ptr(device_id);
	cloudConnectPacket.password = (uint8_t*) mqtt_password_buf;
	cloudConnectPacket.passwordLength = mqtt_password_buf_len;
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
	cloudPublishPacket.publishHeaderFlags.duplicate = AZ_HUB_CLIENT_DEFAULT_MQTT_TELEMETRY_DUPLICATE;
	cloudPublishPacket.publishHeaderFlags.qos = AZ_HUB_CLIENT_DEFAULT_MQTT_TELEMETRY_QOS;
	cloudPublishPacket.publishHeaderFlags.retain = AZ_HUB_CLIENT_DEFAULT_MQTT_TELEMETRY_RETAIN;
	// Variable header
	cloudPublishPacket.topic = (uint8_t*)mqtt_dsp_topic_buf;

	// Payload
    strcpy(register_payload_buf, "{\"registrationId\":\"");
    strcat(register_payload_buf, (char*)device_id_buf);
    strcat(register_payload_buf, "\"}");
	cloudPublishPacket.payload = (uint8_t*) register_payload_buf;
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
