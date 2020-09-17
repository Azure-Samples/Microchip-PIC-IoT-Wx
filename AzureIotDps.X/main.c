// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

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
uint8_t methods_response_payload[] = "{\"status\":\"succeed\"}";

az_span const changeOilReminder = AZ_SPAN_LITERAL_FROM_STR("changeOilReminder");
az_span const maxSpeed = AZ_SPAN_LITERAL_FROM_STR("maxSpeed");
az_span const latitude = AZ_SPAN_LITERAL_FROM_STR("latitude");
az_span const longitude = AZ_SPAN_LITERAL_FROM_STR("longitude");
bool   is_desired_properties_valid = false;
char   desired_changeOilReminder[32];
double desired_maxSpeed;
double desired_longitude;
double desired_latitude;

//This handles messages published from the MQTT server when subscribed

void receivedFromCloud_c2d(uint8_t* topic, uint8_t* payload)
{
    az_iot_hub_client_c2d_request c2d_topic;
    az_result result = az_iot_hub_client_c2d_parse_received_topic(&hub_client, az_span_create_from_str((char*)topic), &c2d_topic);
    if (az_result_failed(result))
    {
        debug_printError("az_iot_hub_client_c2d_parse_received_topic failed");
        return;
    }

    az_span property_name;
    az_span property_value;
    while (az_iot_message_properties_next(&c2d_topic.properties, &property_name, &property_value) == AZ_OK)
    {
        debug_printInfo("topic property: %.*s, value: %.*s", (int)az_span_size(property_name), az_span_ptr(property_name), (int)az_span_size(property_value), az_span_ptr(property_value));
    }

    const char* const toggleToken = "\"toggle\":";
    const char* subString;
    if ((subString = strstr((char*)payload, toggleToken)))
    {
        LED_holdYellowOn(subString[strlen(toggleToken)] == '1');
    }

    debug_printInfo("topic: %s", topic);
    debug_printInfo("payload: %s", payload);
}

void receivedFromCloud_message(uint8_t* topic, uint8_t* payload)
{
    az_iot_hub_client_method_request method_topic;
    az_result result = az_iot_hub_client_methods_parse_received_topic(&hub_client, az_span_create_from_str((char*)topic), &method_topic);
    if (az_result_failed(result))
    {
        debug_printError("az_iot_hub_client_methods_parse_received_topic failed");
        return;
    }

    az_span const blink = AZ_SPAN_LITERAL_FROM_STR("blink");
    if (az_span_is_content_equal_ignoring_case(method_topic.name, blink))
    {
        az_json_reader json_parser;
        result = az_json_reader_init(&json_parser, az_span_create_from_str((char*)payload), NULL);
        if (az_result_failed(result))
        {
            debug_printError("az_json_reader_init failed");
            return;
        }

        result = az_json_reader_next_token(&json_parser);
        if (az_result_failed(result) || json_parser.token.kind != AZ_JSON_TOKEN_BEGIN_OBJECT)
        {
            debug_printError("az_json_reader_next_token failed");
            return;
        }

        while (result == AZ_OK)
        {
            result = az_json_reader_next_token(&json_parser);
            if (az_result_failed(result) || json_parser.token.kind != AZ_JSON_TOKEN_PROPERTY_NAME)
            {
                break;
            }

            az_span const duration = AZ_SPAN_LITERAL_FROM_STR("duration");
            if (az_span_is_content_equal_ignoring_case(json_parser.token.slice, duration))
            {
                result = az_json_reader_next_token(&json_parser);
                if (az_result_succeeded(result) && json_parser.token.kind == AZ_JSON_TOKEN_NUMBER)
                {
                    double duration;
                    result = az_span_atod(json_parser.token.slice, &duration);
                    if (az_result_succeeded(result))
                    {
                        LED_flashRed(duration);
                        break;
                    }
                }
            }
        }

        result = az_iot_hub_client_methods_response_get_publish_topic(&hub_client, method_topic.request_id, 200, mqtt_method_topic_buf, sizeof(mqtt_method_topic_buf), NULL);
        if (az_result_failed(result))
        {
            debug_printError("az_iot_hub_client_methods_response_get_publish_topic failed");
            return;
        }

        mqttPublishPacket cloudPublishPacket;
        // Fixed header
        cloudPublishPacket.publishHeaderFlags.duplicate = 0;
        cloudPublishPacket.publishHeaderFlags.qos = 0;
        cloudPublishPacket.publishHeaderFlags.retain = 0;
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

void parse_desired_properties(az_json_reader* json_parser)
{
    if (json_parser->token.kind != AZ_JSON_TOKEN_BEGIN_OBJECT)
    {
        return;
    }

    az_result result = AZ_OK;
    while (az_result_succeeded(result) && az_result_succeeded(az_json_reader_next_token(json_parser)))
    {
        if (json_parser->token.kind != AZ_JSON_TOKEN_PROPERTY_NAME)
        {
            break;
        }

        az_span property_name = json_parser->token.slice;
        result = az_json_reader_next_token(json_parser);
        if (az_result_succeeded(result))
        {
            if (az_span_is_content_equal_ignoring_case(property_name, changeOilReminder) && json_parser->token.kind == AZ_JSON_TOKEN_STRING)
            {
                az_span changeOilReminderSrc = json_parser->token.slice;
                az_span_to_str(desired_changeOilReminder, sizeof(desired_changeOilReminder), changeOilReminderSrc);
            }
            else if (az_span_is_content_equal_ignoring_case(property_name, maxSpeed) && json_parser->token.kind == AZ_JSON_TOKEN_NUMBER)
            {
                result = az_span_atod(json_parser->token.slice, &desired_maxSpeed);
            }
            else if (az_span_is_content_equal_ignoring_case(property_name, latitude) && json_parser->token.kind == AZ_JSON_TOKEN_NUMBER)
            {
                result = az_span_atod(json_parser->token.slice, &desired_latitude);
            }
            else if (az_span_is_content_equal_ignoring_case(property_name, longitude) && json_parser->token.kind == AZ_JSON_TOKEN_NUMBER)
            {
                result = az_span_atod(json_parser->token.slice, &desired_longitude);
            }     
            else
            {
                result = az_json_reader_skip_children(json_parser);
            }
        }
    }
}

void receivedFromCloud_twin(uint8_t* topic, uint8_t* payload)
{
    debug_printInfo("Device Twin GET");

    if (topic == NULL || payload == NULL)
    {
        debug_printError("receivedFromCloud_twin: NULL topic or payload");
        return;
    }

    az_iot_hub_client_twin_response twin_response;
    az_span twin_topic = az_span_create_from_str((char*)topic);
    az_result result = az_iot_hub_client_twin_parse_received_topic(&hub_client, twin_topic, &twin_response);
    if (az_result_failed(result))
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

    is_desired_properties_valid = true;

    az_json_reader json_parser;
    az_span json = az_span_create_from_str((char*)payload);
    result = az_json_reader_init(&json_parser, json, NULL);
    if (az_result_failed(result))
    {
        debug_printError("az_json_reader_init failed");
        return;
    }

    result = az_json_reader_next_token(&json_parser);
    if (az_result_failed(result))
    {
        debug_printError("az_json_reader_next_token failed");
        return;
    }

    while (json_parser.token.kind == AZ_JSON_TOKEN_BEGIN_OBJECT && result == AZ_OK)
    {
        result = az_json_reader_next_token(&json_parser);
        if (az_result_succeeded(result) && json_parser.token.kind == AZ_JSON_TOKEN_PROPERTY_NAME)
        {
            az_span const desired = AZ_SPAN_LITERAL_FROM_STR("desired");
            if (az_span_is_content_equal_ignoring_case(json_parser.token.slice, desired))
            {
                result = az_json_reader_next_token(&json_parser);                
                if (az_result_succeeded(result) && json_parser.token.kind == AZ_JSON_TOKEN_BEGIN_OBJECT)
                {
                    parse_desired_properties(&json_parser);
                }                
            }
            else
            {
                result = az_json_reader_skip_children(&json_parser);                
            }
        }
        else
        {
            result = az_json_reader_skip_children(&json_parser);
        }
    }
}

void receivedFromCloud_patch(uint8_t* topic, uint8_t* payload)
{
    debug_printInfo("Device Twin PATCH");
    if (topic == NULL || payload == NULL)
    {
        debug_printError("receivedFromCloud_patch: NULL topic or payload");
        return;
    }

    az_iot_hub_client_twin_response twin_response;
    az_span twin_topic = az_span_create_from_str((char*)topic);
    az_result result = az_iot_hub_client_twin_parse_received_topic(&hub_client, twin_topic, &twin_response);
    if (az_result_failed(result))
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

    az_json_reader json_parser;
    az_span json = az_span_create_from_str((char*)payload);
    result = az_json_reader_init(&json_parser, json, NULL);
    if (az_result_failed(result))
    {
        debug_printError("az_json_reader_init failed");
        return;
    }

    result = az_json_reader_next_token(&json_parser);
    if (az_result_failed(result))
    {
        debug_printError("az_json_reader_next_token failed");
        return;
    }

    while (json_parser.token.kind == AZ_JSON_TOKEN_BEGIN_OBJECT && result == AZ_OK)
    {
        result = az_json_reader_next_token(&json_parser);
        if (az_result_succeeded(result) && json_parser.token.kind == AZ_JSON_TOKEN_BEGIN_OBJECT)
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

