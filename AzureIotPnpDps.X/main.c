// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "platform/application_manager.h"
#include "platform/system.h"
#include "platform/led.h"
#include "platform/pin_manager.h"
#include "platform/sensors_handling.h"
#include "platform/cloud/cloud_service.h"
#include "platform/delay.h"
#include "platform/debug_print.h"
#include "platform/drivers/timeout.h"
#include "platform/mqtt/mqtt_core/mqtt_core.h"
#include "platform/mqtt/mqtt_packetTransfer_interface.h"
#include "azure/core/az_span.h"
#include "azure/core/az_json.h"
#include "azure/iot/az_iot_pnp_client.h"
#include "main.h"

#define RETURN_ERR_IF_FAILED(ret)                   \
  do {                                              \
    if (az_result_failed(ret)) {                    \
      return ret;                                   \
    }                                               \
  } while (0)

#define RETURN_IF_FAILED(ret)                       \
  do {                                              \
    if (az_result_failed(ret)) {                    \
      return;                                       \
    }                                               \
  } while (0)

#define RETURN_ERR_WITH_MESSAGE_IF_FAILED(ret, msg) \
  do {                                              \
    if (az_result_failed(ret)) {                    \
      debug_printError(msg);                        \
      return ret;                                   \
    }                                               \
  } while (0)

#define RETURN_WITH_MESSAGE_IF_FAILED(ret, msg)     \
  do {                                              \
    if (az_result_failed(ret)) {                    \
      debug_printError(msg);                        \
      return;                                       \
    }                                               \
  } while (0)

static int send_reported_property(twin_properties_t* twin_properties);

extern az_iot_pnp_client pnp_client;
extern pf_MQTT_CLIENT pf_mqtt_iotprovisioning_client;
extern pf_MQTT_CLIENT pf_mqtt_pnp_client;

// used by led.c to communicate LED states changes
extern led_status_t led_status;

// used by pin_manager.c
extern button_press_data_t button_press_data;

extern uint32_t telemetry_interval;

static bool isGetReceived = false;
static int rebootDelay = 0;
static uint32_t reboot_task(void* payload);
static timerStruct_t reboot_timer = { reboot_task };

#define LED_NO_CHANGE (-1)
//
// * LEDs *
//        | On                   | Off                      | Fast Blink (100ms)           | Slow Blink (400ms)
// Blue	  | AP Connected         | AP Disconnected          | WiFi Connectin in progress   | 
// Green  | Connected to IoT Hub | Not Connected to IoT Hub | DPS provisioning in progress | IoT Hub connection in progress
// Yellow | User LED Control from Cloud (On, Off, Blink)
// Red    | Error                | No error                 | Error with DPS connection    | Error with IoT Hub connection
//

// * IoT Plug and Play Values *
// The model id is the unique identifier for device model (also called the Digital Twins Model Identifier or DTMI)
// The device Model defines the capability of your device. The functionality of the device should match what
// is described in the corresponding device model while following IoT Plug and Play convension. 
// Should you choose to program your own IoT Plug and Play device model,
// the functionality would need to match the device model and you would need to update the below 'device_model_id'.
// Please see the sample README for more information on IoT Plug and Play.
const az_span device_model_id = AZ_SPAN_LITERAL_FROM_STR("dtmi:com:Microchip:PIC_IoT_WM;1");

// ISO8601 Time Format
static const char iso_spec_time_format[] = "%Y-%m-%dT%H:%M:%SZ";

// IoT Plug and Play Connection Values
static int32_t request_id_int;
static char request_id_buf[8];

// IoT Plug and Play temperature and light sensor telemetry
// Frequency of telemetry is determined by telemetryInterval desired property (or writable property)
// JSON document example :
// {"temperature" : <temperature sensor value in celsius>, "light":<light sensor value>}
char telemetry_topic[128];
static char telemetry_payload[256];
static const az_span telemetry_interval_property_name = AZ_SPAN_LITERAL_FROM_STR("telemetryInterval");
static const az_span span_telemetry_name_temperature  = AZ_SPAN_LITERAL_FROM_STR("temperature");
static const az_span span_telemetry_name_light        = AZ_SPAN_LITERAL_FROM_STR("light");

// IoT Plug and Play button press telemetry (event)
// JSON document example :
//
// {
//   "button_event" : 
//   {
//     "button_name" : "SW0",
//     "press_count" : 1
//   }
// }
static char button_event_payload[256];
static const az_span span_event_name_button_event      = AZ_SPAN_LITERAL_FROM_STR("button_event");
static const az_span span_event_name_button_name       = AZ_SPAN_LITERAL_FROM_STR("button_name");
static const az_span span_event_name_button_sw0        = AZ_SPAN_LITERAL_FROM_STR("SW0");
static const az_span span_event_name_button_sw1        = AZ_SPAN_LITERAL_FROM_STR("SW1");
static const az_span span_event_name_press_count       = AZ_SPAN_LITERAL_FROM_STR("press_count");

// IoT Plug and Play commands
static char commands_response_topic[128];
static char end_time_buffer[32];
static char commands_response_payload[256];
static char incoming_since_value[32];

// getMaxMinReport command
static const az_span span_command_getmaxmin            = AZ_SPAN_LITERAL_FROM_STR("getMaxMinReport");
static const az_span span_command_getmaxmin_since      = AZ_SPAN_LITERAL_FROM_STR("since");
static const az_span span_command_getmaxmin_max_temp   = AZ_SPAN_LITERAL_FROM_STR("maxTemp");
static const az_span span_command_getmaxmin_min_temp   = AZ_SPAN_LITERAL_FROM_STR("minTemp");
static const az_span span_command_getmaxmin_avg_temp   = AZ_SPAN_LITERAL_FROM_STR("avgTemp");
static const az_span span_command_getmaxmin_start_time = AZ_SPAN_LITERAL_FROM_STR("startTime");
static const az_span span_command_getmaxmin_end_time   = AZ_SPAN_LITERAL_FROM_STR("endTime");

// reboot command
static const az_span span_command_reboot               = AZ_SPAN_LITERAL_FROM_STR("reboot");
static const az_span span_command_reboot_status        = AZ_SPAN_LITERAL_FROM_STR("status");
static const az_span span_command_reboot_success       = AZ_SPAN_LITERAL_FROM_STR("success");
static const az_span span_command_reboot_delay         = AZ_SPAN_LITERAL_FROM_STR("delay");

// IoT Plug and Play properties
static char reported_property_topic[128];
static char reported_property_payload[256];

static const az_span span_desired_property             = AZ_SPAN_LITERAL_FROM_STR("desired");
static const az_span span_desired_property_version     = AZ_SPAN_LITERAL_FROM_STR("$version");
static const az_span span_desired_ack_value            = AZ_SPAN_LITERAL_FROM_STR("value");
static const az_span span_desired_ack_code             = AZ_SPAN_LITERAL_FROM_STR("ac");
static const az_span span_desired_ack_version          = AZ_SPAN_LITERAL_FROM_STR("av");
static const az_span span_desired_ack_description      = AZ_SPAN_LITERAL_FROM_STR("ad");

// Telemetry Interval writable property
static const az_span span_property_telemetry_interval  = AZ_SPAN_LITERAL_FROM_STR("telemetryInterval");

// maxTempSinceLastReboot
static const az_span span_property_max_temp            = AZ_SPAN_LITERAL_FROM_STR("maxTempSinceLastReboot");

// LEDs
static const az_span led_blue_property_name            = AZ_SPAN_LITERAL_FROM_STR("led_blue");
static const az_span led_green_property_name           = AZ_SPAN_LITERAL_FROM_STR("led_green");
static const az_span led_yellow_property_name          = AZ_SPAN_LITERAL_FROM_STR("led_yellow");
static const az_span led_red_property_name             = AZ_SPAN_LITERAL_FROM_STR("led_red");

static const az_span led_on_string                     = AZ_SPAN_LITERAL_FROM_STR("On");
static const az_span led_off_string                    = AZ_SPAN_LITERAL_FROM_STR("Off");
static const az_span led_blink_string                  = AZ_SPAN_LITERAL_FROM_STR("Blink");

// IoT Plug and Play error responses 
static const az_span span_error_property_name          = AZ_SPAN_LITERAL_FROM_STR("Error");
static const az_span empty_payload_span                = AZ_SPAN_LITERAL_FROM_STR("\"\"");
static const az_span span_error_no_payload_getmaxmin   = AZ_SPAN_LITERAL_FROM_STR("Start Time not found");
static const az_span span_error_command_not_supported  = AZ_SPAN_LITERAL_FROM_STR("{\"Status\":\"Unsupported Command\"}");
static const az_span span_error_no_payload_reboot      = AZ_SPAN_LITERAL_FROM_STR("Delay time not specified. Please specify parameter 'delay' in period format (e.g. PT5S for 5 sec)");
static const az_span command_empty_payload_maxmin_span = AZ_SPAN_LITERAL_FROM_STR("Payload Empty. Please specify parameter 'since'");

// IoT Plug and Play Device Values
static int32_t device_current_temp;
static int32_t device_current_light;
static int32_t device_temperature_avg_total;
static uint32_t device_temperature_avg_count = 0;
static int32_t device_max_temp;
static int32_t device_min_temp;
static int32_t device_avg_temp;

/**************************************
 A task to reboot (reset) the device
static uint32_t reboot_task(void* payload)
*************************************/
static uint32_t reboot_task(void* payload)
{
	asm("RESET"); // never returns
	return 0;
}

/**************************************
 Start JSON_BUILDER for JSON Document
 This creates a new JSON with "{"
*************************************/
static az_result start_json_object(
	az_json_writer* jw,
	az_span az_span_buffer)
{
	RETURN_ERR_IF_FAILED(az_json_writer_init(jw, az_span_buffer, NULL));
	RETURN_ERR_IF_FAILED(az_json_writer_append_begin_object(jw));
	return AZ_OK;
}

/**********************************************
* Start JSON_BUILDER for JSON Document
* This adds "}" to the JSON
**********************************************/
static az_result end_json_object(
	az_json_writer* jw)
{
	RETURN_ERR_IF_FAILED(az_json_writer_append_end_object(jw));
	return AZ_OK;
}

/**********************************************
*	Add a JSON key-value pair with int32 data
*	e.g. "property_name" : property_val (number)
**********************************************/
static az_result append_jason_property_int32(
	az_json_writer* jw,
	az_span property_name,
	int32_t property_val)
{
	RETURN_ERR_IF_FAILED(az_json_writer_append_property_name(jw, property_name));
	RETURN_ERR_IF_FAILED(az_json_writer_append_int32(jw, property_val));
	return AZ_OK;
}

/**********************************************
* Add a JSON key-value pair with string data
* e.g. "property_name" : "property_val (string)"
**********************************************/
static az_result append_jason_property_string(
	az_json_writer* jw,
	az_span property_name,
	az_span property_val)
{
	RETURN_ERR_IF_FAILED(az_json_writer_append_property_name(jw, property_name));
	RETURN_ERR_IF_FAILED(az_json_writer_append_string(jw, property_val));
	return AZ_OK;
}

/**********************************************
* Add JSON for writable property response with int32 data
* e.g. "property_name" : property_val_int32
**********************************************/
static az_result append_reported_property_response_int32(
	az_json_writer* jw,
	az_span property_name,
	int32_t property_val,
	int32_t ack_code,
	int32_t ack_version,
	az_span ack_description)
{
	RETURN_ERR_IF_FAILED(az_json_writer_append_property_name(jw, property_name));
	RETURN_ERR_IF_FAILED(az_json_writer_append_begin_object(jw));
	RETURN_ERR_IF_FAILED(az_json_writer_append_property_name(jw, span_desired_ack_value));
	RETURN_ERR_IF_FAILED(az_json_writer_append_int32(jw, property_val));
	RETURN_ERR_IF_FAILED(az_json_writer_append_property_name(jw, span_desired_ack_code));
	RETURN_ERR_IF_FAILED(az_json_writer_append_int32(jw, ack_code));
	RETURN_ERR_IF_FAILED(az_json_writer_append_property_name(jw, span_desired_ack_version));
	RETURN_ERR_IF_FAILED(az_json_writer_append_int32(jw, ack_version));
	RETURN_ERR_IF_FAILED(az_json_writer_append_property_name(jw, span_desired_ack_description));
	RETURN_ERR_IF_FAILED(az_json_writer_append_string(jw, ack_description));
	RETURN_ERR_IF_FAILED(az_json_writer_append_end_object(jw));
	return AZ_OK;
}

/**********************************************
*	Add JSON key-value pairs for getMaxMinReport command
*	e.g.
*	{
*		"maxTemp" : 0,
*		"minTemp" : 2,
*		"minTemp" : 1,
*		"startTime" : 1,
*		"endTime" : 1,
*	}
**********************************************/
static az_result build_getMaxMinReport_response_payload(
	az_span response_span,
	az_span start_time_span,
	az_span end_time_span,
	az_span* response_payload_span)
{
	az_json_writer jw;

	// Build the command response payload
	RETURN_ERR_IF_FAILED(start_json_object(&jw, response_span));

	RETURN_ERR_IF_FAILED(append_jason_property_int32(&jw, span_command_getmaxmin_max_temp, device_max_temp));
	RETURN_ERR_IF_FAILED(append_jason_property_int32(&jw, span_command_getmaxmin_min_temp, device_min_temp));
	RETURN_ERR_IF_FAILED(append_jason_property_int32(&jw, span_command_getmaxmin_avg_temp, device_avg_temp));
	RETURN_ERR_IF_FAILED(append_jason_property_string(&jw, span_command_getmaxmin_start_time, start_time_span));
	RETURN_ERR_IF_FAILED(append_jason_property_string(&jw, span_command_getmaxmin_end_time, end_time_span));
	RETURN_ERR_IF_FAILED(end_json_object(&jw));
	*response_payload_span = az_json_writer_get_bytes_used_in_destination(&jw);
	return AZ_OK;
}

/**********************************************
* Create JSON document for error response
**********************************************/
static az_result build_error_response_payload(
	az_span response,
	az_span error_string,
	az_span* response_payload)
{
	az_json_writer jw;

	// Build the command response payload
	RETURN_ERR_IF_FAILED(start_json_object(&jw, response));
	RETURN_ERR_IF_FAILED(append_jason_property_string(&jw, span_error_property_name, error_string));
	RETURN_ERR_IF_FAILED(end_json_object(&jw));
	*response_payload = az_json_writer_get_bytes_used_in_destination(&jw);
	return AZ_OK;
}

/**********************************************
* Append JSON key-value pairs for button name and button press count
* e.g.
* {
*   "button_event" :
*   {
*     "button_name" : "SW0",
*     "press_count" : 1
*   }
* }
**********************************************/
static az_result append_button_press_telemetry(
	az_json_writer* jw,
	az_span button_name_span,
	int32_t press_count)
{
	RETURN_ERR_IF_FAILED(az_json_writer_append_property_name(jw, span_event_name_button_event));
	RETURN_ERR_IF_FAILED(az_json_writer_append_begin_object(jw));
	RETURN_ERR_IF_FAILED(append_jason_property_string(jw, span_event_name_button_name, button_name_span));
	RETURN_ERR_IF_FAILED(append_jason_property_int32(jw, span_event_name_press_count, press_count));
	RETURN_ERR_IF_FAILED(az_json_writer_append_end_object(jw));
	return AZ_OK;
}

/**********************************************
* Initialize twin property data structure
**********************************************/
void init_twin_data(twin_properties_t* twin_properties)
{
	twin_properties->flag.AsUSHORT      = 0;
	twin_properties->version_num        = 0;
	twin_properties->desired_led_yellow = LED_NO_CHANGE;
	twin_properties->reported_led_red   = LED_NO_CHANGE;
	twin_properties->reported_led_blue  = LED_NO_CHANGE;
	twin_properties->reported_led_green = LED_NO_CHANGE;
}

/**********************************************
* Create request id span which increments source int each call. 
* Capable of holding 8 digit number.
**********************************************/
static az_span get_request_id(void)
{
	az_span remainder;
	az_span out_span = az_span_create((uint8_t*)request_id_buf, sizeof(request_id_buf));
	az_result result = az_span_i32toa(out_span, request_id_int++, &remainder);
	(void)remainder;
	(void)result;
	return out_span;
}

/**********************************************
*	Publish MQTT message.
**********************************************/
static int mqtt_publish_message(char* topic, az_span payload, int qos)
{
	mqttPublishPacket cloudPublishPacket;
	// Fixed header
	cloudPublishPacket.publishHeaderFlags.duplicate = 0;
	cloudPublishPacket.publishHeaderFlags.qos = qos;
	cloudPublishPacket.publishHeaderFlags.retain = 0;
	// Variable header
	cloudPublishPacket.topic = (uint8_t*)topic;

	// Payload
	cloudPublishPacket.payload = az_span_ptr(payload);
	cloudPublishPacket.payloadLength = az_span_size(payload);

	if (MQTT_CreatePublishPacket(&cloudPublishPacket) != true)
	{
		debug_printError("  MQTT: Connection lost PUBLISH failed");
		return 1;
	}

	return 0;
}

/**********************************************
* Update temperature and light sensor data.
* Track temperature data for getMaxMinReport command and Max Temperature report.
**********************************************/
static void update_sensor_data(twin_properties_t* twin_properties)
{
	int16_t temp = SENSORS_getTempValue();

	device_current_light = SENSORS_getLightValue();
	device_current_temp = (int)(temp / 100);

	// Increment the avg count, add the new temp to the total, and calculate the new avg
	device_temperature_avg_count++;
	device_temperature_avg_total += device_current_temp;
	device_avg_temp = device_temperature_avg_total / device_temperature_avg_count;

	if (device_current_temp > device_max_temp)
	{
		device_max_temp = device_current_temp;
		twin_properties->flag.max_temp_updated = 1;
	}

	if (device_current_temp < device_min_temp)
	{
		device_min_temp = device_current_temp;
	}
	return;
}

/**********************************************
* Build sensor telemetry JSON
**********************************************/
static az_result build_sensor_telemetry_message(az_span* out_payload)
{
	az_json_writer jw;
	memset(&telemetry_payload, 0, sizeof(telemetry_payload));
	RETURN_ERR_IF_FAILED(start_json_object(&jw, AZ_SPAN_FROM_BUFFER(telemetry_payload)));
	RETURN_ERR_IF_FAILED(append_jason_property_int32(&jw, span_telemetry_name_temperature, device_current_temp));
	RETURN_ERR_IF_FAILED(append_jason_property_int32(&jw, span_telemetry_name_light, device_current_light));
	RETURN_ERR_IF_FAILED(end_json_object(&jw));
	*out_payload = az_json_writer_get_bytes_used_in_destination(&jw);
	return AZ_OK;
}

/**********************************************
* Send telemetry with data from temperature and light sensors
**********************************************/
static az_result send_telemetry_message(void)
{
	az_result result;
	twin_properties_t twin_properties; // for maxTempSinceLastReboot reported property
	az_span telemetry_payload_span;

	init_twin_data(&twin_properties);

	RETURN_ERR_WITH_MESSAGE_IF_FAILED(
		az_iot_pnp_client_telemetry_get_publish_topic(
			&pnp_client, AZ_SPAN_EMPTY, NULL, telemetry_topic, sizeof(telemetry_topic), NULL),
		"Failed to get MQTT topic for telemetry");

	update_sensor_data(&twin_properties);

	RETURN_ERR_WITH_MESSAGE_IF_FAILED(
		build_sensor_telemetry_message(&telemetry_payload_span),
		"Failed to build sensor telemetry JSON payload");

	result = mqtt_publish_message(telemetry_topic, telemetry_payload_span, 0);

	if (twin_properties.flag.max_temp_updated == 1 && isGetReceived)
	{
		send_reported_property(&twin_properties);
	}

	return result;
}

/**********************************************
* This will get called every 1 second only while we have a valid Cloud connection
**********************************************/
void sendToCloud(void)
{
	send_telemetry_message();
}

/**********************************************
* Callback for device provisioning
**********************************************/
#ifdef CFG_MQTT_PROVISIONING_HOST
void iot_provisioning_completed(void)
{
	debug_printGood("  MAIN: IoT Provisioning Completed");
	application_cloud_mqtt_connect(hub_hostname, &pf_mqtt_pnp_client, sendToCloud);
}
#endif //CFG_MQTT_PROVISIONING_HOST 

/**********************************************
* Send the response of the command invocation
**********************************************/
static int send_command_response(
				az_iot_pnp_client_command_request* request,
				uint16_t status,
				az_span response)
{
	// Get the response topic to publish the command response
	int rc = az_iot_pnp_client_commands_response_get_publish_topic(
		&pnp_client, request->request_id, status, commands_response_topic,
		sizeof(commands_response_topic), NULL);

	RETURN_ERR_WITH_MESSAGE_IF_FAILED(rc, "Unable to get command response publish topic");

	debug_printInfo("  MAIN: Command Status: %u", status);

	// Send the commands response
	if ((rc = mqtt_publish_message(commands_response_topic, response, 0)) == 0)
	{
		debug_printInfo("  MAIN: Successfully sent command response");
	}

	return rc;
}

/**********************************************
* Handle getMaxMinReport command
**********************************************/
static az_result process_getMaxMinReport(az_span payload, az_span response, az_span* out_response)
{
	debug_printInfo("  MAIN: %s() : Enter", __func__);

	if (az_span_size(payload) == 0 || (az_span_size(payload) == 2 && az_span_is_content_equal(empty_payload_span, payload)))
	{
		debug_printError("  MAIN: Empty Payload");
		RETURN_ERR_IF_FAILED(build_error_response_payload(response, command_empty_payload_maxmin_span, out_response));
		return AZ_ERROR_ITEM_NOT_FOUND;
	}

	// Parse the "since" field in the payload.
	az_span start_time_span = AZ_SPAN_EMPTY;
	az_json_reader jr;
	int32_t incoming_since_value_len;

	RETURN_ERR_IF_FAILED(az_json_reader_init(&jr, payload, NULL));
	while (jr.token.kind != AZ_JSON_TOKEN_END_OBJECT)
	{
		if (jr.token.kind == AZ_JSON_TOKEN_PROPERTY_NAME)
		{
			if (az_json_token_is_text_equal(&jr.token, span_command_getmaxmin_since))
			{
				RETURN_ERR_IF_FAILED(az_json_reader_next_token(&jr));
				RETURN_ERR_IF_FAILED(az_json_token_get_string(&jr.token, incoming_since_value, sizeof(incoming_since_value), &incoming_since_value_len));
				break;
			}
		}
		else if (jr.token.kind == AZ_JSON_TOKEN_STRING)
		{
			RETURN_ERR_IF_FAILED(az_json_token_get_string(&jr.token, incoming_since_value, sizeof(incoming_since_value), &incoming_since_value_len));
			break;
		}
		RETURN_ERR_IF_FAILED(az_json_reader_next_token(&jr));
	}

	start_time_span = az_span_create((uint8_t*)incoming_since_value, incoming_since_value_len);

	// Set the response payload to error if the "since" field was not sent
	if (az_span_ptr(start_time_span) == NULL)
	{
		RETURN_ERR_IF_FAILED(build_error_response_payload(response, span_error_no_payload_getmaxmin, out_response));
		return AZ_ERROR_ITEM_NOT_FOUND;
	}

	// Get the current time as a string
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	size_t len = strftime(end_time_buffer, sizeof(end_time_buffer), iso_spec_time_format, timeinfo);
	az_span end_time_span = az_span_create((uint8_t*)end_time_buffer, (int32_t)len);

	RETURN_ERR_IF_FAILED(build_getMaxMinReport_response_payload(
								response,
								start_time_span,
								end_time_span,
								out_response));

	return AZ_OK;

}

/**********************************************
*	Handle reboot command
**********************************************/
static az_result process_reboot(az_span payload, az_span response, az_span* out_response)
{
	char reboot_delay[32];
	char buffer[32] = {0};
	az_json_writer jw;
	az_json_reader jr;

	debug_printInfo("  MAIN: %s() : Payload %s", __func__, az_span_ptr(payload));

	if (az_span_size(payload) == 0 || (az_span_size(payload) == 2 && az_span_is_content_equal(empty_payload_span, payload)))
	{
		RETURN_ERR_IF_FAILED(build_error_response_payload(response, span_error_no_payload_reboot, out_response));
		return AZ_ERROR_ITEM_NOT_FOUND;
	}

	RETURN_ERR_IF_FAILED(az_json_reader_init(&jr, payload, NULL));

	while (jr.token.kind != AZ_JSON_TOKEN_END_OBJECT)
	{
		if (jr.token.kind == AZ_JSON_TOKEN_PROPERTY_NAME)
		{
			if (az_json_token_is_text_equal(&jr.token, span_command_reboot_delay))
			{
				RETURN_ERR_IF_FAILED(az_json_reader_next_token(&jr));
				RETURN_ERR_IF_FAILED(az_json_token_get_string(&jr.token, reboot_delay, sizeof(reboot_delay), NULL));
				break;
			}
		}
		else if (jr.token.kind == AZ_JSON_TOKEN_STRING)
		{
				RETURN_ERR_IF_FAILED(az_json_token_get_string(&jr.token, reboot_delay, sizeof(reboot_delay), NULL));
				break;
		}
		RETURN_ERR_IF_FAILED(az_json_reader_next_token(&jr));
	}

	if (reboot_delay[0] != 'P' || reboot_delay[1] != 'T' || reboot_delay[strlen(reboot_delay) - 1] != 'S')
	{
		debug_printError("  MAIN: Reboot Delay wrong format");
		RETURN_ERR_IF_FAILED(build_error_response_payload(response, span_error_no_payload_reboot, out_response));
		return AZ_ERROR_ARG;
	}

	memcpy(buffer, &reboot_delay[2], strlen(reboot_delay) - 3);
	buffer[strlen(reboot_delay) - 3] = '\0';
	rebootDelay = atoi((const char *)buffer);

	start_json_object(&jw, response);
	append_jason_property_string(&jw, span_command_reboot_status, span_command_reboot_success);
	append_jason_property_int32(&jw, span_command_reboot_delay, rebootDelay);
	end_json_object(&jw);

	*out_response = az_json_writer_get_bytes_used_in_destination(&jw);

	return AZ_OK;
}

/**********************************************
* Handle commands
**********************************************/
static void handle_command_message(
				az_span payload,
				az_iot_pnp_client_command_request* command_request)
{
	az_span command_response_span = AZ_SPAN_FROM_BUFFER(commands_response_payload);
	uint16_t return_code;
	az_result azRet;
	int ret;

	if (az_span_is_content_equal(span_command_getmaxmin, command_request->command_name))
	{
		azRet = process_getMaxMinReport(payload, command_response_span, &command_response_span);
		if (azRet != AZ_OK)
		{
			return_code = 404;
		}
		else
		{
			return_code = 200;
		}

		// Send command response with report as JSON payload
		if ((ret = send_command_response(command_request, return_code, command_response_span)) != 0)
		{
			debug_printError("  MAIN: Unable to send command response, az_result %x status %d", azRet, ret);
		}
	}
	else if (az_span_is_content_equal(span_command_reboot, command_request->command_name))
	{
		azRet = process_reboot(payload, command_response_span, &command_response_span);

		if (azRet != AZ_OK)
		{
			debug_printError("  MAIN: process_reboot() failed.  AZ Result %x %d", azRet, az_span_size(command_response_span));
			if(az_span_size(command_response_span) == 0)
			{
				// if response is empty, payload was not in the right format.
				if (az_result_failed(azRet = build_error_response_payload(command_response_span, span_error_no_payload_reboot, &command_response_span)))
				{
					debug_printError("  MAIN: Failed to build error response. AZ Result %d", azRet);
				}
				
			}
			return_code = 404;
		}
		else
		{
			return_code = 200;
		}

		// Send command response with report as JSON payload
		if ((ret = send_command_response(command_request, return_code, command_response_span)) != 0)
		{
			debug_printError("  MAIN: Unable to send command response, az_result %x status %d", azRet, ret);
		}

		if (ret == 0 && return_code == 200)
		{
			debug_printGood("  MAIN: Reboot Time Set");
			timeout_create(&reboot_timer, timeout_mSecToTicks(rebootDelay * 1000));
		}
	}
	else
	{
		// Unsupported command
		debug_printError("  MAIN: Unsupported command received: %s.",
						az_span_ptr(command_request->command_name));

		if ((ret = send_command_response(command_request, 404, span_error_command_not_supported)) != 0)
		{
			debug_printError("  MAIN: Unable to send %d response, status %d", 404, ret);
		}
	}
}

/**********************************************
* Callback for device method
**********************************************/
void receivedFromCloud_commands(uint8_t* topic, uint8_t* payload)
{
	debug_printInfo("  MAIN: Commands");

	az_iot_pnp_client_command_request command_request;
	az_span command_topic = az_span_create_from_str((char*)topic);
	az_result rc = az_iot_pnp_client_commands_parse_received_topic(&pnp_client, command_topic, &command_request);
	RETURN_WITH_MESSAGE_IF_FAILED(rc, "az_iot_pnp_client_commands_parse_received_topic failed");

	handle_command_message(az_span_create_from_str((char*)payload), &command_request);
}

/**********************************************
* Send Reported Property
**********************************************/
static int send_reported_property(
	twin_properties_t* twin_properties)
{
	az_result result;

	debug_printInfo("  MAIN: %s() : Enter", __func__);

	// Get the topic used to send a reported property update
	az_span request_id_span = get_request_id();
	if (az_result_failed(
		result = az_iot_pnp_client_property_patch_get_publish_topic(
					&pnp_client,
					request_id_span,
					reported_property_topic,
					sizeof(reported_property_topic),
					NULL)))
	{
		debug_printError("  MAIN: Unable to get twin document publish topic, return code %x", result);
		return result;
	}

	// Clear buffer and initialize JSON Payload.	This creates "{"
	az_json_writer jw;
	memset(reported_property_payload, 0, sizeof(reported_property_payload));
	az_span az_span_buffer = AZ_SPAN_FROM_BUFFER(reported_property_payload);

	if (az_result_failed(result = start_json_object(&jw, az_span_buffer)))
	{
		debug_printError("  MAIN: Unable to initialize json_builder, return code %x", result);
		return result;
	}

	if (twin_properties->flag.telemetry_interval_found)
	{
		if (az_result_failed(
			result = append_reported_property_response_int32(
						&jw,
						telemetry_interval_property_name,
						telemetry_interval,
						200,
						twin_properties->version_num,
						AZ_SPAN_FROM_STR("Success"))))
		{
			debug_printError("  MAIN: Unable to add property for telemetry interval, return code %x", result);
			return result;
		}
	}

	// Add Yellow LED to the reported property
	// Example with integer Enum
	if (twin_properties->desired_led_yellow != LED_NO_CHANGE)
	{
		int32_t yellow_led;

		if ((led_status.state_flag.yellow & (LED_STATE_BLINK_SLOW | LED_STATE_BLINK_FAST)) != 0)
		{
			yellow_led = 3; // blink
		}
		else if (led_status.state_flag.yellow == LED_STATE_HOLD)
		{
			yellow_led = 1; // on
		}
		else
		{
			yellow_led = 2; // off
		}

		if (az_result_failed(
			result = append_reported_property_response_int32(
						&jw,
						led_yellow_property_name,
						yellow_led,
						200,
						twin_properties->version_num,
						AZ_SPAN_FROM_STR("Success"))))
		{
			debug_printError("  MAIN: Unable to add property for Yellow LED, return code %x", result);
			return result;
		}
	}

	// Add Red LED
	// Example with String Enum
	if (twin_properties->reported_led_red != LED_NO_CHANGE)
	{
		az_span red_led_value;

		switch (twin_properties->reported_led_red)
		{
			case 1:
			red_led_value = led_on_string;
			break;

			case 2:
			red_led_value = led_off_string;
			break;

			case 3:
			red_led_value = led_blink_string;
			break;
		}

		if (az_result_failed(
			result = append_jason_property_string(
				&jw,
				led_red_property_name,
				red_led_value)))
		{
			debug_printError("  MAIN: Unable to add property for Red LED, return code %x", result);
			return result;
		}
	}

	// Add Blue LED
	if (twin_properties->reported_led_blue != LED_NO_CHANGE)
	{
		if (az_result_failed(
			result = append_jason_property_int32(
				&jw,
				led_blue_property_name,
				twin_properties->reported_led_blue)))
		{
			debug_printError("  MAIN: Unable to add property for Blue LED, return code %x", result);
			return result;
		}
	}

	// Add Green LED
	if (twin_properties->reported_led_green != LED_NO_CHANGE)
	{
		if (az_result_failed(
			result = append_jason_property_int32(
				&jw,
				led_green_property_name,
				twin_properties->reported_led_green)))
		{
			debug_printError("  MAIN: Unable to add property for Green LED, return code %x", result);
			return result;
		}
	}

	if (twin_properties->flag.max_temp_updated || twin_properties->flag.isGet == 1)
	{
		if (az_result_failed(
			result = append_jason_property_int32(
				&jw,
				span_property_max_temp,
				device_max_temp)))
		{
			debug_printError("  MAIN: Unable to add property for Max Temp, return code %x", result);
			return result;
		}
	}

	// Close JSON Payload (appends "}")
	if (az_result_failed(result = end_json_object(&jw)))
	{
		debug_printError("  MAIN: Unable to append end object, return code %x", result);
		return result;
	}

	az_span json_payload = az_json_writer_get_bytes_used_in_destination(&jw);

	// Publish the reported property payload to IoT Hub
	debug_printInfo("IOTHUB: Sending twin reported property : %s", az_span_ptr(json_payload));

	// Send the reported property
	if ((result = mqtt_publish_message(reported_property_topic, json_payload, 0)) == 0)
	{
		debug_printInfo("  MAIN: PUBLISH : Reported property");
	}

	return result;
}

/**********************************************
* Parse Desired Property (Writable Property)
**********************************************/
static az_result parse_twin_desired_property(
	az_span twin_payload_span,
	twin_properties_t* twin_properties)
{
	az_json_reader jr;
	bool desired_found = false;

	RETURN_ERR_IF_FAILED(az_json_reader_init(&jr, twin_payload_span, NULL));
	RETURN_ERR_IF_FAILED(az_json_reader_next_token(&jr));

	if (jr.token.kind != AZ_JSON_TOKEN_BEGIN_OBJECT)
	{
		debug_printError("  MAIN: Start of a JSON object not found in twin : %s", az_span_ptr(twin_payload_span));
		return AZ_ERROR_UNEXPECTED_CHAR;
	}

	if (twin_properties->flag.isGet == 1)
	{
		// If is twin get payload, we have to parse one level deeper for "desired" wrapper
		RETURN_ERR_IF_FAILED(az_json_reader_next_token(&jr));
		while (jr.token.kind != AZ_JSON_TOKEN_END_OBJECT)
		{
			if (jr.token.kind != AZ_JSON_TOKEN_PROPERTY_NAME)
			{
				RETURN_ERR_IF_FAILED(az_json_reader_skip_children(&jr));
			}
			else {
				if (az_json_token_is_text_equal(&jr.token, span_desired_property))
				{
					desired_found = true;
					RETURN_ERR_IF_FAILED(az_json_reader_next_token(&jr));
					break;
				}
				else {
					RETURN_ERR_IF_FAILED(az_json_reader_skip_children(&jr));
				}
			}
			RETURN_ERR_IF_FAILED(az_json_reader_next_token(&jr));
		}
	}
	else
	{
		desired_found = true;
	}

	if (!desired_found)
	{
		debug_printError("  MAIN: Desired property object not found in twin : %s", az_span_ptr(twin_payload_span));
		return AZ_ERROR_ITEM_NOT_FOUND;
	}

	if (jr.token.kind != AZ_JSON_TOKEN_BEGIN_OBJECT)
	{
		debug_printError("  MAIN: Start of a JSON object not found in twin : %s", az_span_ptr(twin_payload_span));
		return AZ_ERROR_UNEXPECTED_CHAR;
	}

	RETURN_ERR_IF_FAILED(az_json_reader_next_token(&jr));

	//
	// Loop through Desired Twin
	//
	while (jr.token.kind != AZ_JSON_TOKEN_END_OBJECT)
	{
		if (az_json_token_is_text_equal(&jr.token, span_desired_property_version))
		{
			// found $version
			RETURN_ERR_IF_FAILED(az_json_reader_next_token(&jr));
			RETURN_ERR_IF_FAILED(az_json_token_get_int32(&jr.token, &twin_properties->version_num));
			twin_properties->flag.version_found = 1;
		}
		else if (az_json_token_is_text_equal(&jr.token, led_yellow_property_name)) {
			// found writable property to control Yellow LED
			RETURN_ERR_IF_FAILED(az_json_reader_next_token(&jr));
			RETURN_ERR_IF_FAILED(az_json_token_get_int32(&jr.token, &twin_properties->desired_led_yellow));
			twin_properties->flag.yellow_led_found = 1;
		}
		else if (az_json_token_is_text_equal(&jr.token, span_property_telemetry_interval)) {
			// found writable property to adjust telemetry interval
			RETURN_ERR_IF_FAILED(az_json_reader_next_token(&jr));
			RETURN_ERR_IF_FAILED(az_json_token_get_uint32(&jr.token, &telemetry_interval));
			twin_properties->flag.telemetry_interval_found = 1;
		}
		else
		{
			// else ignore token.
			RETURN_ERR_IF_FAILED(az_json_reader_skip_children(&jr));
		}
		RETURN_ERR_IF_FAILED(az_json_reader_next_token(&jr));
	}

	// make sure $version and at least one of writable properties are found
	if (twin_properties->flag.version_found == 1 && (twin_properties->flag.AsUSHORT & (~1)) != 0)
	{
		return AZ_OK;
	}

	return AZ_ERROR_ITEM_NOT_FOUND;
}

/**********************************************
*	Process LED Update/Patch
**********************************************/
static void update_leds(
	twin_properties_t* twin_properties)
{
	// If desired properties are not set, send current LED states.
	// Otherwise, set LED state based on Desired property
	if (twin_properties->flag.yellow_led_found == 1 && twin_properties->desired_led_yellow != LED_NO_CHANGE)
	{
		if (twin_properties->desired_led_yellow == 1)
		{
			LED_SetYellow(LED_STATE_HOLD);
		}
		else if (twin_properties->desired_led_yellow == 2)
		{
			LED_SetYellow(LED_STATE_OFF);
		}
		else if (twin_properties->desired_led_yellow == 3)
		{
			LED_SetYellow(LED_STATE_BLINK_FAST);
		}
	}

	// If this is Twin Get, populate LED states for Red, Blue, Green LEDs
	if (twin_properties->flag.isGet == 1)
	{
		check_led_status(twin_properties);
	}
}

/**********************************************
* Process Twin Update/Patch
**********************************************/
static void handle_property_message(
				az_span payload_span,
				az_iot_pnp_client_property_response* property_response)
{
	debug_printGood("  MAIN: handle_property_message : Payload = %s", az_span_ptr(payload_span));

	az_result result;
	twin_properties_t twin_properties;

	init_twin_data(&twin_properties);

	// Determine what type of incoming twin message this is. Print relevant data for the message.
	switch (property_response->response_type)
	{
		// A response from a twin GET publish message with the twin document as a payload.
		case AZ_IOT_HUB_CLIENT_TWIN_RESPONSE_TYPE_GET:

			debug_printGood("  MAIN: A twin GET response received");
			isGetReceived = true;
			twin_properties.flag.isGet = 1;
			break;

		// An update to the desired properties with the properties as a JSON payload.
		case AZ_IOT_HUB_CLIENT_TWIN_RESPONSE_TYPE_DESIRED_PROPERTIES:

			debug_printInfo("  MAIN: A twin desired properties patch message received");
			twin_properties.flag.isGet = 0;
			break;

		// A response from a twin reported properties publish message. With a successful update of
		// the reported properties, the payload will be empty and the status will be 204.
		case AZ_IOT_HUB_CLIENT_TWIN_RESPONSE_TYPE_REPORTED_PROPERTIES:

			debug_printInfo("  MAIN: A twin reported properties response message was received");
			return;
	}

	if (az_result_failed(result = parse_twin_desired_property(payload_span, &twin_properties)))
	{
		// If the item can't be found, the desired temp might not be set so take no action
		debug_printError("  MAIN: Could not parse desired property, return code %d\n", result);
	}
	else
	{
		update_leds(&twin_properties);
		send_reported_property(&twin_properties);
	}
}

/**********************************************
* Callback for device twin
**********************************************/
void receivedFromCloud_property(uint8_t* topic, uint8_t* payload)
{
	debug_printInfo("  MAIN: Device Property Get");

	if (topic == NULL)
	{
		debug_printError("  MAIN: NULL topic");
		return;
	}

	az_iot_pnp_client_property_response property_response;
	az_span property_topic = az_span_create_from_str((char *)topic);
	az_result rc = az_iot_pnp_client_property_parse_received_topic(&pnp_client, property_topic, &property_response);
	RETURN_WITH_MESSAGE_IF_FAILED(rc, "az_iot_pnp_client_property_parse_received_topic failed");

	if (az_span_size(property_response.request_id) != 0 && IOT_DEBUG_PRINT)
	{
		char request_id_buf[50];
		az_span_to_str(request_id_buf, sizeof(request_id_buf), property_response.request_id);
		debug_printInfo("  MAIN: Property request, request_id:%s, status: %d", request_id_buf, property_response.status);
	}
	if (payload == NULL)
	{
		debug_printError("  MAIN: NULL payload");
	}
	else
	{
		handle_property_message(az_span_create_from_str((char*)payload), &property_response);
	}
}

/**********************************************
* Callback for device twin patch
**********************************************/
void receivedFromCloud_patch(uint8_t* topic, uint8_t* payload)
{
	debug_printInfo("  MAIN: Device Property Patch");

	if (topic == NULL)
	{
		debug_printError("  MAIN: NULL topic");
		return;
	}

	az_iot_pnp_client_property_response property_response;
	az_span property_topic = az_span_create_from_str((char *)topic);
	az_result rc = az_iot_pnp_client_property_parse_received_topic(&pnp_client, property_topic, &property_response);
	RETURN_WITH_MESSAGE_IF_FAILED(rc, "az_iot_pnp_client_property_parse_received_topic failed");

	if (payload == NULL)
	{
		debug_printError("  MAIN: NULL payload");
	}
	else
	{
		handle_property_message(az_span_create_from_str((char*)payload), &property_response);
	}
}

/**********************************************
* Check if button(s) interrupt happened or not.
**********************************************/
void check_button_status(void)
{
	az_span button_event_span = AZ_SPAN_FROM_BUFFER(button_event_payload);
	az_json_writer jw;

	// save flags in case user pressed buttons very fast, and for error case.
	bool sw0 = button_press_data.flag.sw0_button_press == 1 ? true : false;
	bool sw1 = button_press_data.flag.sw1_button_press == 1 ? true : false;

	// clear the flags
	button_press_data.flag.AsUSHORT = 0;

	if (sw0 != true && sw1 != true)
	{
		return;
	}

	RETURN_IF_FAILED(start_json_object(&jw, button_event_span));

	if (sw0 == true)
	{
		debug_printGood("  MAIN: Button SW0 Press Count %lu", button_press_data.sw0_press_count);
		RETURN_IF_FAILED(append_button_press_telemetry(&jw, span_event_name_button_sw0, button_press_data.sw0_press_count));
	}

	if (sw1 == true)
	{
		debug_printGood("  MAIN: Button SW1 Press Count %lu", button_press_data.sw1_press_count);
		RETURN_IF_FAILED(append_button_press_telemetry(&jw, span_event_name_button_sw1, button_press_data.sw1_press_count));
	}

	RETURN_IF_FAILED(end_json_object(&jw));

	button_event_span = az_json_writer_get_bytes_used_in_destination(&jw);
	if (mqtt_publish_message(telemetry_topic, button_event_span, 0) != 0)
	{
		debug_printInfo("  MAIN: Failed to publish Button Press event telemetry");
	}
}

/**********************************************
* Check if LED status has changed or not.
**********************************************/
void check_led_status(twin_properties_t* twin_properties)
{
	twin_properties_t* twin_properties_ptr;
	twin_properties_t twin_properties_local;

	bool bForceSync = false; // force LED status synchronization if this request is coming from Twin Get

	if (twin_properties == NULL)
	{
		// twin data is not provided by the caller
		init_twin_data(&twin_properties_local);
		twin_properties_ptr = &twin_properties_local;
	}
	else {
		// twin data is provided by the caller through Get Twin
		twin_properties_ptr = twin_properties;
	}

	if (led_status.change_flag.AsUSHORT == 0 && twin_properties_ptr->flag.isGet == 0)
	{
		// no changes, nothing to update
		return;
	}

	debug_printInfo("  MAIN: %s() led_change %x", __func__, led_status.change_flag.AsUSHORT);

  // if this is from Get Twin, update according to Desired Property
	bForceSync = twin_properties_ptr->flag.isGet == 1 ? true : false;

	if (led_status.change_flag.AsUSHORT != 0 || bForceSync)
	{
		if (led_status.change_flag.blue == 1 || bForceSync)
		{
			if ((led_status.state_flag.blue & (LED_STATE_BLINK_SLOW | LED_STATE_BLINK_FAST)) != 0)
			{
				twin_properties_ptr->reported_led_blue = 3;
			}
			else if (led_status.state_flag.blue == LED_STATE_HOLD)
			{
				twin_properties_ptr->reported_led_blue = 1;
			}
			else
			{
				twin_properties_ptr->reported_led_blue = 2;
			}
		}

		if (led_status.change_flag.green == 1 || bForceSync)
		{
			if ((led_status.state_flag.green & (LED_STATE_BLINK_SLOW | LED_STATE_BLINK_FAST)) != 0)
			{
				twin_properties_ptr->reported_led_green = 3;
			}
			else if (led_status.state_flag.green == LED_STATE_HOLD)
			{
				twin_properties_ptr->reported_led_green = 1;
			}
			else
			{
				twin_properties_ptr->reported_led_green = 2;
			}
		}

		if (led_status.change_flag.red == 1 || bForceSync)
		{
			if ((led_status.state_flag.red & (LED_STATE_BLINK_SLOW | LED_STATE_BLINK_FAST)) != 0)
			{
				twin_properties_ptr->reported_led_red = 3;
			}
			else if (led_status.state_flag.red == LED_STATE_HOLD)
			{
				twin_properties_ptr->reported_led_red = 1;
			}
			else
			{
				twin_properties_ptr->reported_led_red = 2;
			}
		}

		// clear flags
		led_status.change_flag.AsUSHORT = 0;

		// if this is from Get Twin, Device Twin code pass will update reported properties
		if (!bForceSync)
		{
			send_reported_property(twin_properties_ptr);
		}
	}
}

/**********************************************
* Main application
**********************************************/
int main(void)
{
	// initialize global variable
	memset(&button_press_data, 0, sizeof(button_press_data_t));

	// initialize the device
	SYSTEM_Initialize();
	application_init();

#ifdef CFG_MQTT_PROVISIONING_HOST
	pf_mqtt_iotprovisioning_client.MQTT_CLIENT_task_completed = iot_provisioning_completed;
	application_cloud_mqtt_connect(CFG_MQTT_PROVISIONING_HOST, &pf_mqtt_iotprovisioning_client, NULL);
#else
	application_cloud_mqtt_connect(hub_hostname, &pf_mqtt_pnp_client, sendToCloud);
#endif //CFG_MQTT_PROVISIONING_HOST

	while (true)
	{
		runScheduler();
	}

	return true;
}
