/**
  Generated main.c file from MPLAB Code Configurator

  @Company
	Microchip Technology Inc.

  @File Name
	main.c

  @Summary
	This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
	This source file provides main entry point for system initialization and application code development.
	Generation Information :
		Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.166.0
		Device            :  PIC24FJ128GA705
	The generated drivers are tested against the following:
		Compiler          :  XC16 v1.41
		MPLAB 	          :  MPLAB X v5.30
*/

/*
	(c) 2019 Microchip Technology Inc. and its subsidiaries. You may use this
	software and any derivatives exclusively with Microchip products.

	THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
	EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
	WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
	PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
	WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

	IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
	INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
	WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
	BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
	FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
	ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
	THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

	MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
	TERMS.
*/

/**
  Section: Included Files
*/
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "platform/application_manager.h"
#include "platform/system.h"
#include "platform/led.h"
#include "platform/sensors_handling.h"
#include "platform/cloud/cloud_service.h"
#include "platform/debug_print.h"
#include "platform/mqtt/mqtt_core/mqtt_core.h"
#include "platform/mqtt/mqtt_packetTransfer_interface.h"
#include "azure/core/az_span.h"
#include "azure/core/az_json.h"
#include "azure/iot/az_iot_hub_client.h"

extern az_iot_hub_client hub_client;
extern pf_MQTT_CLIENT pf_mqtt_iotprovisioning_client;
extern pf_MQTT_CLIENT pf_mqtt_iothub_client;

char mqtt_method_topic_buf[256];
char c2d_key[128];
char c2d_value[256];
uint8_t methods_response_payload[] = "{\"status\":\"succeed\"}";

const az_span changeOilReminder = AZ_SPAN_LITERAL_FROM_STR("changeOilReminder");
const az_span maxSpeed = AZ_SPAN_LITERAL_FROM_STR("maxSpeed");
const az_span latitude = AZ_SPAN_LITERAL_FROM_STR("latitude");
const az_span longitude = AZ_SPAN_LITERAL_FROM_STR("longitude");
bool   is_desired_properties_valid = false;
char   desired_changeOilReminder[32];
double desired_maxSpeed;
double desired_longitude;
double desired_latitude;

//This handles messages published from the MQTT server when subscribed

void receivedFromCloud_c2d(uint8_t* topic, uint8_t* payload)
{
	az_iot_hub_client_c2d_request c2d_topic;
	az_result result = az_iot_hub_client_c2d_parse_received_topic(&hub_client, az_span_from_str((char*)topic), &c2d_topic);
	if (az_failed(result))
	{
		debug_printError("az_iot_hub_client_c2d_parse_received_topic failed");
		return;
	}

	az_pair property_pair;
	while (az_iot_hub_client_properties_next(&c2d_topic.properties, &property_pair) == AZ_OK)
	{
		az_span_to_str(c2d_key, sizeof(c2d_key), property_pair.key);
		az_span_to_str(c2d_value, sizeof(c2d_value), property_pair.value);
		debug_printer(SEVERITY_NONE, LEVEL_NORMAL, "topic property: %.*s, value: %.*s", c2d_key, c2d_value);
	}

	const char* const toggleToken = "\"toggle\":";
	const char* subString;
	if ((subString = strstr((char*)payload, toggleToken)))
	{
		LED_holdYellowOn(subString[strlen(toggleToken)] == '1');
	}

	debug_printer(SEVERITY_NONE, LEVEL_NORMAL, "topic: %s", topic);
	debug_printer(SEVERITY_NONE, LEVEL_NORMAL, "payload: %s", payload);
}

void receivedFromCloud_message(uint8_t* topic, uint8_t* payload)
{
	az_iot_hub_client_method_request method_topic;
	az_result result = az_iot_hub_client_methods_parse_received_topic(&hub_client, az_span_from_str((char*)topic), &method_topic);
	if (az_failed(result))
	{
		debug_printError("az_iot_hub_client_methods_parse_received_topic failed");
		return;
	}

	az_span blink = AZ_SPAN_LITERAL_FROM_STR("blink");
	if (az_span_is_content_equal_ignoring_case(method_topic.name, blink))
	{
		az_json_parser json_parser;
		az_span json = az_span_from_str((char*)payload);
		result = az_json_parser_init(&json_parser, json);
		if (az_failed(result))
		{
			debug_printError("az_json_parser_init failed");
			return;
		}

		az_json_token token;
		result = az_json_parser_parse_token(&json_parser, &token);
		if (az_failed(result))
		{
			debug_printError("az_json_parser_parse_token failed");
			return;
		}

		while (result == AZ_OK)
		{
			az_json_token_member token_member;
			result = az_json_parser_parse_token_member(&json_parser, &token_member);
			if (az_failed(result))
			{
				break;
			}

			az_span duration = AZ_SPAN_LITERAL_FROM_STR("duration");
			if (token_member.token.kind == AZ_JSON_TOKEN_NUMBER &&
				az_span_is_content_equal_ignoring_case(token_member.name, duration))
			{
				double duration;
				result = az_json_token_get_number(&token_member.token, &duration);
				LED_flashRed(duration);
				break;
			}
		}

		result = az_iot_hub_client_methods_response_get_publish_topic(&hub_client, method_topic.request_id, 200, mqtt_method_topic_buf, sizeof(mqtt_method_topic_buf), NULL);
		if (az_failed(result))
		{
			debug_printError("az_iot_hub_client_methods_response_get_publish_topic failed");
			return;
		}

		mqttPublishPacket cloudPublishPacket;
		// Fixed header
		cloudPublishPacket.publishHeaderFlags.duplicate = AZ_HUB_CLIENT_DEFAULT_MQTT_TELEMETRY_DUPLICATE;
		cloudPublishPacket.publishHeaderFlags.qos = AZ_HUB_CLIENT_DEFAULT_MQTT_TELEMETRY_QOS;
		cloudPublishPacket.publishHeaderFlags.retain = AZ_HUB_CLIENT_DEFAULT_MQTT_TELEMETRY_RETAIN;
		// Variable header
		cloudPublishPacket.topic = (uint8_t*)mqtt_method_topic_buf;

		// Payload
		cloudPublishPacket.payload = methods_response_payload;
		cloudPublishPacket.payloadLength = sizeof(methods_response_payload);

		if (MQTT_CreatePublishPacket(&cloudPublishPacket) != true)
		{
			debug_printError("MQTT: Connection lost PUBLISH failed");
		}
	}
}

void parse_desired_properties(az_json_parser* json_parser)
{
	az_result result = AZ_OK;
	az_json_token_member token_member;
	while (az_succeeded(result) && az_succeeded(az_json_parser_parse_token_member(json_parser, &token_member)))
	{
		if (az_span_is_content_equal_ignoring_case(token_member.name, changeOilReminder) && token_member.token.kind == AZ_JSON_TOKEN_STRING)
		{
			az_span changeOilReminderSrc;
			result = az_json_token_get_string(&token_member.token, &changeOilReminderSrc);
			if (az_succeeded(result))
			{
				az_span_to_str(desired_changeOilReminder, sizeof(desired_changeOilReminder), changeOilReminderSrc);
			}
		}
		else if (az_span_is_content_equal_ignoring_case(token_member.name, maxSpeed) && token_member.token.kind == AZ_JSON_TOKEN_NUMBER)
		{
			result = az_json_token_get_number(&token_member.token, &desired_maxSpeed);
		}
		else if (az_span_is_content_equal_ignoring_case(token_member.name, latitude) && token_member.token.kind == AZ_JSON_TOKEN_NUMBER)
		{
			result = az_json_token_get_number(&token_member.token, &desired_latitude);
		}
		else if (az_span_is_content_equal_ignoring_case(token_member.name, longitude) && token_member.token.kind == AZ_JSON_TOKEN_NUMBER)
		{
			result = az_json_token_get_number(&token_member.token, &desired_longitude);
		}
	}
}

void receivedFromCloud_twin(uint8_t* topic, uint8_t* payload)
{
	if (topic == NULL)
	{
		return;
	}

	az_iot_hub_client_twin_response twin_response;
	az_span twin_topic = az_span_from_str((char*)topic);
	az_result result = az_iot_hub_client_twin_parse_received_topic(&hub_client, twin_topic, &twin_response);
	if (az_failed(result))
	{
		debug_printError("az_iot_hub_client_twin_parse_received_topic failed");
		return;
	}

    if (IOT_DEBUG_PRINT)
    {
        if (az_span_size(twin_response.request_id) != 0)
        {
            char request_id_buf[50];
            az_span_to_str(request_id_buf, sizeof(request_id_buf), twin_response.request_id);
            debug_printInfo("Twin request, request_id:%s, status: %d", request_id_buf, twin_response.status);
        }
    }

	if (payload == NULL)
	{
		return; // no payload, nothing to process
	}

	is_desired_properties_valid = true;

	az_json_parser json_parser;
	az_span json = az_span_from_str((char*)payload);
	result = az_json_parser_init(&json_parser, json);
	if (az_failed(result))
	{
		debug_printError("az_json_parser_init failed");
		return;
	}

	az_json_token token;
	result = az_json_parser_parse_token(&json_parser, &token);
	if (az_failed(result))
	{
		debug_printError("az_json_parser_parse_token failed");
		return;
	}

	while (token.kind == AZ_JSON_TOKEN_BEGIN_OBJECT && result == AZ_OK)
	{
		az_json_token_member token_member;
		result = az_json_parser_parse_token_member(&json_parser, &token_member);
		if (az_succeeded(result))
		{
			az_span desired = AZ_SPAN_LITERAL_FROM_STR("desired");
			if (token_member.token.kind == AZ_JSON_TOKEN_BEGIN_OBJECT)
			{
				if (az_span_is_content_equal_ignoring_case(token_member.name, desired))
				{
					parse_desired_properties(&json_parser);
				}
				else
				{
					while (az_succeeded(az_json_parser_parse_token_member(&json_parser, &token_member)));
				}
			}
		}
	}
}

void receivedFromCloud_patch(uint8_t* topic, uint8_t* payload)
{
	if (topic == NULL)
	{
		return;
	}

	az_iot_hub_client_twin_response twin_response;
	az_span twin_topic = az_span_from_str((char*)topic);
	az_result result = az_iot_hub_client_twin_parse_received_topic(&hub_client, twin_topic, &twin_response);
	if (az_failed(result))
	{
		debug_printError("az_iot_hub_client_twin_parse_received_topic failed");
		return;
	}
    
    if (IOT_DEBUG_PRINT)
    {
        if (az_span_size(twin_response.version) != 0)
        {
            char version_buf[50];
            az_span_to_str(version_buf, sizeof(version_buf), twin_response.version);
            debug_printInfo("Twin update, version:%s, status: %d", version_buf, twin_response.status);
        }
    }

	if (topic == NULL || payload == NULL)
	{
		return;
	}

	az_json_parser json_parser;
	az_span json = az_span_from_str((char*)payload);
	result = az_json_parser_init(&json_parser, json);
	if (az_failed(result))
	{
		debug_printError("az_json_parser_init failed");
		return;
	}

	az_json_token token;
	result = az_json_parser_parse_token(&json_parser, &token);
	if (az_failed(result))
	{
		debug_printError("az_json_parser_parse_token failed");
		return;
	}

	while (token.kind == AZ_JSON_TOKEN_BEGIN_OBJECT && result == AZ_OK)
	{
		az_json_token_member token_member;
		result = az_json_parser_parse_token_member(&json_parser, &token_member);
		if (az_succeeded(result))
		{
			parse_desired_properties(&json_parser);
		}
	}
}

// This will get called every 1 second only while we have a valid Cloud connection
void sendToCloud(void)
{
	static char json[70];

	// This part runs every CFG_SEND_INTERVAL seconds
	int rawTemperature = SENSORS_getTempValue();
	int light = SENSORS_getLightValue();
	int len = sprintf(json, "{\"Light\":%d,\"Temp\":\"%d.%02d\"}", light, rawTemperature / 100, abs(rawTemperature) % 100);

	if (len > 0) {
		CLOUD_publishData((uint8_t*)json, len);
		LED_flashYellow();
	}
}

#ifdef CFG_MQTT_PROVISIONING_HOST
void iot_provisioning_completed(void)
{
    debug_printGOOD("IoT Provisioning Completed");
    application_cloud_mqtt_connect(hub_hostname, &pf_mqtt_iothub_client, sendToCloud);
}
#endif //CFG_MQTT_PROVISIONING_HOST 


/*
	Main application
 */

int main(void)
{
	// initialize the device
	SYSTEM_Initialize();
	application_init();

#ifdef CFG_MQTT_PROVISIONING_HOST
    pf_mqtt_iotprovisioning_client.MQTT_CLIENT_task_completed = iot_provisioning_completed;
	application_cloud_mqtt_connect(CFG_MQTT_PROVISIONING_HOST, &pf_mqtt_iotprovisioning_client, NULL);
#else
	application_cloud_mqtt_connect(hub_hostname, &pf_mtqt_iothub_client, sendToCloud);
#endif //CFG_MQTT_PROVISIONING_HOST 

	while (true)
	{
		// Add your application code
		runScheduler();
	}

	return true;
}
/**
 End of File
*/

