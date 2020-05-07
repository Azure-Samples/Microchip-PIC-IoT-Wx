/*
	\file   mqtt_iotprovisioning_packetParameters.c

	\brief  MQTT Packet Parameters source file.

	(c) 2018 Microchip Technology Inc. and its subsidiaries.

	Subject to your compliance with these terms, you may use Microchip software and any
	derivatives exclusively with Microchip products. It is your responsibility to comply with third party
	license terms applicable to your use of third party software (including open source software) that
	may accompany Microchip software.

	THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
	EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
	IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
	FOR A PARTICULAR PURPOSE.

	IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
	INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
	WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
	HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
	THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
	CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
	OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
	SOFTWARE.
*/

// ToDo This file needs to be renamed as app_mqttClient.c
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include "../../mqtt/mqtt_core/mqtt_core.h"
#include "../../mqtt/mqtt_packetTransfer_interface.h"
#include "mqtt_packetPopulate.h"
#include "mqtt_iotprovisioning_packetPopulate.h"
#include "../../config/IoT_Sensor_Node_config.h"
#include "../../debug_print.h"
#include "../../../platform/cryptoauthlib/lib/basic/atca_basic.h"
#include "az_iot_provisioning_client.h"

#ifdef CFG_MQTT_PROVISIONING_HOST

pf_MQTT_CLIENT pf_mqqt_iotprovisioning_client = {
  MQTT_CLIENT_iotprovisioning_publish,
  MQTT_CLIENT_iotprovisioning_receive,
  MQTT_CLIENT_iotprovisioning_connect,
  MQTT_CLIENT_iotprovisioning_subscribe,
  MQTT_CLIENT_iotprovisioning_connected,  
};

        
//extern void receivedFromCloud_c2d(uint8_t* topic, uint8_t* payload);
//extern void receivedFromCloud_message(uint8_t* topic, uint8_t* payload);
//extern void receivedFromCloud_twin(uint8_t* topic, uint8_t* payload);
//extern void receivedFromCloud_patch(uint8_t* topic, uint8_t* payload);
//static const az_span twin_request_id = AZ_SPAN_LITERAL_FROM_STR("initial_get world");

//char mqtt_sub_c2d_topic_buf[128];
//char mqtt_telemetry_topic_buf[64];
//char mqtt_sub_topic_methods_buf[64];
//char mqtt_sub_topic_twin_patch_buf[64];
//char mqtt_sub_topic_twin_document_buf[64];
//char mqtt_get_topic_twin_buf[64];
extern char username_buf[200];
extern char sas_token_buf[512];
//uint8_t device_id_buf[100];
//az_span device_id;
az_iot_provisioning_client provisioning_client;
static const char* sas_token_buf2 = "SharedAccessSignature sr=0ne000CDA9E%2fregistrations%2f01233EAD58E86797FE&sig=eQj1TKIJ%2banv098IiGXmAJcm8mlXxp1HTkcUDPgKX44%3d&se=1590923226&skn=registration";
static const char* username_buf2 = "0ne000CDA9E/registrations/01233EAD58E86797FE/api-version=2019-03-31";
    
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
publishReceptionHandler_t imqtt_iotprovisioning_publishReceiveCallBackTable[MAX_NUM_TOPICS_SUBSCRIBE] = { 0 };

void MQTT_CLIENT_iotprovisioning_publish(uint8_t* data, uint16_t len)
{

}

void MQTT_CLIENT_iotprovisioning_receive(uint8_t* data, uint8_t len)
{
	MQTT_GetReceivedData(data, len);
}

void MQTT_CLIENT_iotprovisioning_connect(char* deviceID)
{
    const az_span global_device_endpoint = AZ_SPAN_LITERAL_FROM_STR(CFG_MQTT_PROVISIONING_HOST);
    const az_span id_scope = AZ_SPAN_LITERAL_FROM_STR(PROVISIONING_ID_SCOPE);
    const az_span registration_id = az_span_from_str(deviceID);    
    az_result result = az_iot_provisioning_client_init(&provisioning_client, global_device_endpoint, id_scope, registration_id, NULL);
	if (az_failed(result))
	{
		debug_printError("az_iot_provisioning_client_init failed");
		return;
	}
        
    size_t username_buf_len;
    result = az_iot_provisioning_client_get_user_name(&provisioning_client, username_buf, sizeof(username_buf), &username_buf_len);
	if (az_failed(result))
	{
		debug_printError("az_iot_provisioning_client_get_user_name failed");
		return;
	}

	time_t expire_time = time(NULL) + 60 * 60; // token expires in 1 hour
	uint8_t signature_buf[128];
	az_span signature = AZ_SPAN_FROM_BUFFER(signature_buf);
	result = az_iot_provisioning_client_sas_get_signature(&provisioning_client, expire_time, signature, &signature);  
	if (az_failed(result))
	{
		debug_printError("az_iot_provisioning_client_sas_get_signature failed");
		return;
	}

	uint8_t key[32];
	size_t key_size = _az_COUNTOF(key);
	atcab_base64decode_(provisioning_device_key, strlen(provisioning_device_key), key, &key_size, az_iot_b64rules);
    atcab_nonce(key);
    //	atcab_nonce_load(NONCE_MODE_TARGET_TEMPKEY, key, 64);
	uint8_t hash[32];
	ATCA_STATUS atca_status = atcab_sha_hmac(signature_buf, az_span_size(signature), ATCA_TEMPKEY_KEYID, hash, SHA_MODE_TARGET_OUT_ONLY);
	if (atca_status != ATCA_SUCCESS)
	{
		debug_printError("atcab_sha_hmac failed");
		return;
	}
    
    
	char signature_hash_buf[64];
	key_size = _az_COUNTOF(signature_hash_buf);
	atcab_base64encode_(hash, sizeof(hash), signature_hash_buf, &key_size, az_iot_b64rules);

	char signature_hash_encoded_buf[256];
	url_encode_rfc3986(signature_hash_buf, signature_hash_encoded_buf, _az_COUNTOF(signature_hash_encoded_buf));

	size_t sas_token_buf_len;
	az_span signature_hash_encoded = az_span_from_str(signature_hash_encoded_buf);
	result = az_iot_provisioning_client_sas_get_password(&provisioning_client, signature_hash_encoded, expire_time, az_span_from_str("registration"), sas_token_buf, sizeof(sas_token_buf), &sas_token_buf_len);
	if (az_failed(result))
	{
		debug_printError("az_iot_provisioning_client_sas_get_password failed");
		return;
	}
    
    
	mqttConnectPacket cloudConnectPacket;
	memset(&cloudConnectPacket, 0, sizeof(mqttConnectPacket));
	cloudConnectPacket.connectVariableHeader.connectFlagsByte.All = 0x20; // AZ_CLIENT_DEFAULT_MQTT_CONNECT_CLEAN_SESSION
	cloudConnectPacket.connectVariableHeader.keepAliveTimer = 0;

	int sas_token_buf_len2 = strlen(sas_token_buf2);
    int username_buf2_len2  = strlen(username_buf2);
    
    cloudConnectPacket.clientID = (uint8_t*) az_span_ptr(registration_id);
	cloudConnectPacket.password = (uint8_t*) sas_token_buf2;
	cloudConnectPacket.passwordLength = sas_token_buf_len2;
	cloudConnectPacket.username = (uint8_t*) username_buf2;
	cloudConnectPacket.usernameLength = (uint16_t) username_buf2_len2;

	MQTT_CreateConnectPacket(&cloudConnectPacket);
}

bool MQTT_CLIENT_iotprovisioning_subscribe()
{

	mqttSubscribePacket cloudSubscribePacket;
	// Variable header   
	cloudSubscribePacket.packetIdentifierLSB = 1;
	cloudSubscribePacket.packetIdentifierMSB = 0;

//	cloudSubscribePacket.subscribePayload[0].topic = (uint8_t*)mqtt_sub_c2d_topic_buf;
//	cloudSubscribePacket.subscribePayload[0].topicLength = mqtt_sub_c2d_topic_buf_len;
	cloudSubscribePacket.subscribePayload[0].requestedQoS = 0;
//	cloudSubscribePacket.subscribePayload[1].topic = (uint8_t*)mqtt_sub_topic_methods_buf;
//	cloudSubscribePacket.subscribePayload[1].topicLength = mqtt_sub_topic_methods_buf_len;
	cloudSubscribePacket.subscribePayload[1].requestedQoS = 0;
//	cloudSubscribePacket.subscribePayload[2].topic = (uint8_t*)mqtt_sub_topic_twin_patch_buf;
//	cloudSubscribePacket.subscribePayload[2].topicLength = mqtt_sub_topic_twin_patch_buf_len;
	cloudSubscribePacket.subscribePayload[2].requestedQoS = 0;
//	cloudSubscribePacket.subscribePayload[3].topic = (uint8_t*)mqtt_sub_topic_twin_document_buf;
//	cloudSubscribePacket.subscribePayload[3].topicLength = mqtt_sub_topic_twin_document_buf_len;
	cloudSubscribePacket.subscribePayload[3].requestedQoS = 0;

//	imqtt_iotprovisioning_publishReceiveCallBackTable[0].topic = (uint8_t*)mqtt_sub_c2d_topic_buf;
//	imqtt_iotprovisioning_publishReceiveCallBackTable[0].mqttHandlePublishDataCallBack = receivedFromCloud_c2d;
//	imqtt_iotprovisioning_publishReceiveCallBackTable[1].topic = (uint8_t*)mqtt_sub_topic_methods_buf;
//	imqtt_iotprovisioning_publishReceiveCallBackTable[1].mqttHandlePublishDataCallBack = receivedFromCloud_message;
//	imqtt_iotprovisioning_publishReceiveCallBackTable[2].topic = (uint8_t*)mqtt_sub_topic_twin_patch_buf;
//	imqtt_iotprovisioning_publishReceiveCallBackTable[2].mqttHandlePublishDataCallBack = receivedFromCloud_patch;
//	imqtt_iotprovisioning_publishReceiveCallBackTable[3].topic = (uint8_t*)mqtt_sub_topic_twin_document_buf;
//	imqtt_iotprovisioning_publishReceiveCallBackTable[3].mqttHandlePublishDataCallBack = receivedFromCloud_twin;
	MQTT_SetPublishReceptionHandlerTable(imqtt_iotprovisioning_publishReceiveCallBackTable);

	bool ret = MQTT_CreateSubscribePacket(&cloudSubscribePacket);
	if (ret == true)
	{
		debug_printInfo("MQTT: SUBSCRIBE packet created");
	}

	return ret;
}

void MQTT_CLIENT_iotprovisioning_connected()
{

}

#endif // CFG_MQTT_PROVISIONING_HOST