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

extern uint32_t telemetry_interval_seconds;

static bool is_get_received = false;
static int reboot_delay_seconds = 0;
static uint32_t reboot_task(void* payload);
static timerStruct_t reboot_timer = { reboot_task };

#define LED_NO_CHANGE (-1)
//
// * LEDs *
//        | On                   | Off                      | Fast Blink (100ms)           | Slow Blink (400ms)
// Blue	  | AP Connected         | AP Disconnected          | WiFi Connection in progress  |
// Green  | Connected to IoT Hub | Not Connected to IoT Hub | DPS provisioning in progress | IoT Hub connection in progress
// Yellow | User LED Control from Cloud (On, Off, Blink)
// Red    | Error                | No error                 | Error with DPS connection    | Error with IoT Hub connection
//

// * IoT Plug and Play Values *
// The model id is the unique identifier for device model (also called the Digital Twins Model Identifier or DTMI)
// The device model defines the capability of your device. The functionality of the device should match what
// is described in the corresponding device model while following IoT Plug and Play convention.
// Should you choose to program your own IoT Plug and Play device model,
// the functionality would need to match the device model and you would need to update the below 'device_model_id_span'.
// Please see the sample README for more information on IoT Plug and Play.
const az_span device_model_id_span = AZ_SPAN_LITERAL_FROM_STR("dtmi:com:Microchip:PIC_IoT_WM;1");

// ISO8601 Time Format
static const char iso_spec_time_format[] = "%Y-%m-%dT%H:%M:%SZ";

// IoT Plug and Play Connection Values
static int32_t request_id_int;
static char request_id_buf[8];

// IoT Plug and Play temperature and light sensor telemetry
// Frequency of telemetry is determined by the writable property telemetryInterval.
// JSON document example :
// {"temperature" : <temperature sensor value in celsius>, "light":<light sensor value>}
char telemetry_topic[128];
static char telemetry_payload[256];
static const az_span telemetry_interval_property_name_span = AZ_SPAN_LITERAL_FROM_STR("telemetryInterval");
static const az_span telemetry_name_temperature_span       = AZ_SPAN_LITERAL_FROM_STR("temperature");
static const az_span telemetry_name_light_span             = AZ_SPAN_LITERAL_FROM_STR("light");

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
static const az_span event_name_button_event_span      = AZ_SPAN_LITERAL_FROM_STR("button_event");
static const az_span event_name_button_name_span       = AZ_SPAN_LITERAL_FROM_STR("button_name");
static const az_span event_name_button_sw0_span        = AZ_SPAN_LITERAL_FROM_STR("SW0");
static const az_span event_name_button_sw1_span        = AZ_SPAN_LITERAL_FROM_STR("SW1");
static const az_span event_name_press_count_span       = AZ_SPAN_LITERAL_FROM_STR("press_count");

// IoT Plug and Play commands
static char commands_response_topic[128];
static char end_time_buffer[32];
static char commands_response_payload[256];
static char incoming_since_value[32];

// getMaxMinReport command
static const az_span command_getmaxmin_span            = AZ_SPAN_LITERAL_FROM_STR("getMaxMinReport");
static const az_span command_getmaxmin_since_span      = AZ_SPAN_LITERAL_FROM_STR("since");
static const az_span command_getmaxmin_max_temp_span   = AZ_SPAN_LITERAL_FROM_STR("maxTemp");
static const az_span command_getmaxmin_min_temp_span   = AZ_SPAN_LITERAL_FROM_STR("minTemp");
static const az_span command_getmaxmin_avg_temp_span   = AZ_SPAN_LITERAL_FROM_STR("avgTemp");
static const az_span command_getmaxmin_start_time_span = AZ_SPAN_LITERAL_FROM_STR("startTime");
static const az_span command_getmaxmin_end_time_span   = AZ_SPAN_LITERAL_FROM_STR("endTime");

// reboot command
static const az_span command_reboot_span               = AZ_SPAN_LITERAL_FROM_STR("reboot");
static const az_span command_reboot_status_span        = AZ_SPAN_LITERAL_FROM_STR("status");
static const az_span command_reboot_success_span       = AZ_SPAN_LITERAL_FROM_STR("success");
static const az_span command_reboot_delay_span         = AZ_SPAN_LITERAL_FROM_STR("delay");

// IoT Plug and Play properties
static char reported_property_topic[128];
static char reported_property_payload[256];

static const az_span desired_property_span             = AZ_SPAN_LITERAL_FROM_STR("desired");
static const az_span desired_property_version_span     = AZ_SPAN_LITERAL_FROM_STR("$version");
static const az_span desired_ack_value_span            = AZ_SPAN_LITERAL_FROM_STR("value");
static const az_span desired_ack_code_span             = AZ_SPAN_LITERAL_FROM_STR("ac");
static const az_span desired_ack_version_span          = AZ_SPAN_LITERAL_FROM_STR("av");
static const az_span desired_ack_description_span      = AZ_SPAN_LITERAL_FROM_STR("ad");

// Telemetry Interval writable property
static const az_span property_telemetry_interval_span  = AZ_SPAN_LITERAL_FROM_STR("telemetryInterval");

// maxTempSinceLastReboot
static const az_span property_max_temp_span            = AZ_SPAN_LITERAL_FROM_STR("maxTempSinceLastReboot");

// LEDs
static const az_span led_blue_property_name_span       = AZ_SPAN_LITERAL_FROM_STR("led_blue");
static const az_span led_green_property_name_span      = AZ_SPAN_LITERAL_FROM_STR("led_green");
static const az_span led_yellow_property_name_span     = AZ_SPAN_LITERAL_FROM_STR("led_yellow");
static const az_span led_red_property_name_span        = AZ_SPAN_LITERAL_FROM_STR("led_red");

static const az_span led_on_string_span                = AZ_SPAN_LITERAL_FROM_STR("On");
static const az_span led_off_string_span               = AZ_SPAN_LITERAL_FROM_STR("Off");
static const az_span led_blink_string_span             = AZ_SPAN_LITERAL_FROM_STR("Blink");

// IoT Plug and Play error responses
static const az_span error_property_name_span          = AZ_SPAN_LITERAL_FROM_STR("Error");
static const az_span empty_payload_span                = AZ_SPAN_LITERAL_FROM_STR("\"\"");
static const az_span error_no_payload_getmaxmin_span   = AZ_SPAN_LITERAL_FROM_STR("Start Time not found");
static const az_span error_command_not_supported_span  = AZ_SPAN_LITERAL_FROM_STR("{\"Status\":\"Unsupported Command\"}");
static const az_span error_no_payload_reboot_span      = AZ_SPAN_LITERAL_FROM_STR("Delay time not found. Specify 'delay' in period format (PT5S for 5 sec)");
static const az_span command_empty_payload_maxmin_span = AZ_SPAN_LITERAL_FROM_STR("Payload Empty. Specify parameter 'since'");

// IoT Plug and Play Device Values for telemetry

static int32_t device_current_temp;
static int32_t device_current_light;
static int32_t device_temperature_avg_total;
static uint32_t device_temperature_avg_count = 0;
static int32_t device_max_temp = 0;
static int32_t device_min_temp = INT32_MAX;
static int32_t device_avg_temp = 0;

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
static az_result append_json_property_int32(
	az_json_writer* jw,
	az_span property_name_span,
	int32_t property_val)
{
	RETURN_ERR_IF_FAILED(az_json_writer_append_property_name(jw, property_name_span));
	RETURN_ERR_IF_FAILED(az_json_writer_append_int32(jw, property_val));
	return AZ_OK;
}

/**********************************************
* Add a JSON key-value pair with string data
* e.g. "property_name" : "property_val (string)"
**********************************************/
static az_result append_jason_property_string(
	az_json_writer* jw,
	az_span property_name_span,
	az_span property_val_span)
{
	RETURN_ERR_IF_FAILED(az_json_writer_append_property_name(jw, property_name_span));
	RETURN_ERR_IF_FAILED(az_json_writer_append_string(jw, property_val_span));
	return AZ_OK;
}

/**********************************************
* Add JSON for writable property response with int32 data
* e.g. "property_name" : property_val_int32
**********************************************/
static az_result append_reported_property_response_int32(
	az_json_writer* jw,
	az_span property_name_span,
	int32_t property_val,
	int32_t ack_code,
	int32_t ack_version,
	az_span ack_description_span)
{
//	RETURN_ERR_IF_FAILED(az_json_writer_append_begin_object(jw));
	RETURN_ERR_IF_FAILED(az_iot_pnp_client_property_builder_begin_reported_status(
                           &pnp_client, jw, property_name_span, ack_code, ack_version, ack_description_span));
	RETURN_ERR_IF_FAILED(az_json_writer_append_int32(jw, property_val));
	RETURN_ERR_IF_FAILED(az_iot_pnp_client_property_builder_end_reported_status(&pnp_client, jw));
//	RETURN_ERR_IF_FAILED(az_json_writer_append_end_object(jw));
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

	RETURN_ERR_IF_FAILED(append_json_property_int32(&jw, command_getmaxmin_max_temp_span, device_max_temp));
	RETURN_ERR_IF_FAILED(append_json_property_int32(&jw, command_getmaxmin_min_temp_span, device_min_temp));
	RETURN_ERR_IF_FAILED(append_json_property_int32(&jw, command_getmaxmin_avg_temp_span, device_avg_temp));
	RETURN_ERR_IF_FAILED(append_jason_property_string(&jw, command_getmaxmin_start_time_span, start_time_span));
	RETURN_ERR_IF_FAILED(append_jason_property_string(&jw, command_getmaxmin_end_time_span, end_time_span));
	RETURN_ERR_IF_FAILED(end_json_object(&jw));
	*response_payload_span = az_json_writer_get_bytes_used_in_destination(&jw);
	return AZ_OK;
}

/**********************************************
* Create JSON document for error response
**********************************************/
static az_result build_error_response_payload(
	az_span response_span,
	az_span error_string_span,
	az_span* response_payload_span)
{
	az_json_writer jw;

	// Build the command response payload
	RETURN_ERR_IF_FAILED(start_json_object(&jw, response_span));
	RETURN_ERR_IF_FAILED(append_jason_property_string(&jw, error_property_name_span, error_string_span));
	RETURN_ERR_IF_FAILED(end_json_object(&jw));
	*response_payload_span = az_json_writer_get_bytes_used_in_destination(&jw);
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
	RETURN_ERR_IF_FAILED(az_json_writer_append_property_name(jw, event_name_button_event_span));
	RETURN_ERR_IF_FAILED(az_json_writer_append_begin_object(jw));
	RETURN_ERR_IF_FAILED(append_jason_property_string(jw, event_name_button_name_span, button_name_span));
	RETURN_ERR_IF_FAILED(append_json_property_int32(jw, event_name_press_count_span, press_count));
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
	az_span remainder_span;
	az_span out_span = az_span_create((uint8_t*)request_id_buf, sizeof(request_id_buf));
	az_result az_ret = az_span_i32toa(out_span, request_id_int++, &remainder_span);
	(void)remainder_span;
	(void)az_ret;
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
static az_result build_sensor_telemetry_message(az_span* out_payload_span)
{
	az_json_writer jw;
	memset(&telemetry_payload, 0, sizeof(telemetry_payload));
	RETURN_ERR_IF_FAILED(start_json_object(&jw, AZ_SPAN_FROM_BUFFER(telemetry_payload)));
	RETURN_ERR_IF_FAILED(append_json_property_int32(&jw, telemetry_name_temperature_span, device_current_temp));
	RETURN_ERR_IF_FAILED(append_json_property_int32(&jw, telemetry_name_light_span, device_current_light));
	RETURN_ERR_IF_FAILED(end_json_object(&jw));
	*out_payload_span = az_json_writer_get_bytes_used_in_destination(&jw);
	return AZ_OK;
}

/**********************************************
* Send telemetry with data from temperature and light sensors
**********************************************/
static az_result send_telemetry_message(void)
{
	az_result az_ret;
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

	az_ret = mqtt_publish_message(telemetry_topic, telemetry_payload_span, 0);

	if (twin_properties.flag.max_temp_updated == 1 && is_get_received)
	{
		send_reported_property(&twin_properties);
	}

	return az_ret;
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
	debug_printGood("  MAIN: Provisioning Complete");
	application_cloud_mqtt_connect(hub_hostname, &pf_mqtt_pnp_client, sendToCloud);
}
#endif //CFG_MQTT_PROVISIONING_HOST

/**********************************************
* Send the response of the command invocation
**********************************************/
static int send_command_response(
				az_iot_pnp_client_command_request* request,
				uint16_t status,
				az_span response_span)
{
	// Get the response topic to publish the command response
	int rc = az_iot_pnp_client_commands_response_get_publish_topic(
		&pnp_client, request->request_id, status, commands_response_topic,
		sizeof(commands_response_topic), NULL);

	RETURN_ERR_WITH_MESSAGE_IF_FAILED(rc, "Unable to get command response publish topic");

	debug_printInfo("  MAIN: Command Status: %u", status);

	// Send the commands response
	if ((rc = mqtt_publish_message(commands_response_topic, response_span, 0)) == 0)
	{
		debug_printInfo("  MAIN: Successfully sent command response");
	}

	return rc;
}

/**********************************************
* Handle getMaxMinReport command
**********************************************/
static az_result process_getMaxMinReport(az_span payload_span, az_span response_span, az_span* out_response_span)
{
	debug_printInfo("  MAIN: %s() : Enter", __func__);

	if (az_span_size(payload_span) == 0 || (az_span_size(payload_span) == 2 && az_span_is_content_equal(empty_payload_span, payload_span)))
	{
		debug_printError("  MAIN: Empty command Payload");
		RETURN_ERR_IF_FAILED(build_error_response_payload(response_span, command_empty_payload_maxmin_span, out_response_span));
		return AZ_ERROR_ITEM_NOT_FOUND;
	}

	// Parse the "since" field in the payload.
	az_span start_time_span = AZ_SPAN_EMPTY;
	az_json_reader jr;
	int32_t incoming_since_value_len;

	// we don't actually respect this sinceStr to keep the sample simple,
	// but want to demonstrate how to parse out in any case.

	RETURN_ERR_IF_FAILED(az_json_reader_init(&jr, payload_span, NULL));
	while (jr.token.kind != AZ_JSON_TOKEN_END_OBJECT)
	{
		if (jr.token.kind == AZ_JSON_TOKEN_PROPERTY_NAME)
		{
			if (az_json_token_is_text_equal(&jr.token, command_getmaxmin_since_span))
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
		RETURN_ERR_IF_FAILED(build_error_response_payload(response_span, error_no_payload_getmaxmin_span, out_response_span));
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
								response_span,
								start_time_span,
								end_time_span,
								out_response_span));

	return AZ_OK;

}

/**********************************************
*	Handle reboot command
**********************************************/
static az_result process_reboot(az_span payload_span, az_span response_span, az_span* out_response_span)
{
	char reboot_delay[32];
	az_json_writer jw;
	az_json_reader jr;

	debug_printInfo("  MAIN: %s() : Payload %s", __func__, az_span_ptr(payload_span));

	if (az_span_size(payload_span) == 0 || (az_span_size(payload_span) == 2 && az_span_is_content_equal(empty_payload_span, payload_span)))
	{
		RETURN_ERR_IF_FAILED(build_error_response_payload(response_span, error_no_payload_reboot_span, out_response_span));
		return AZ_ERROR_ITEM_NOT_FOUND;
	}

	RETURN_ERR_IF_FAILED(az_json_reader_init(&jr, payload_span, NULL));

	while (jr.token.kind != AZ_JSON_TOKEN_END_OBJECT)
	{
		if (jr.token.kind == AZ_JSON_TOKEN_PROPERTY_NAME)
		{
			if (az_json_token_is_text_equal(&jr.token, command_reboot_delay_span))
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
		RETURN_ERR_IF_FAILED(build_error_response_payload(response_span, error_no_payload_reboot_span, out_response_span));
		return AZ_ERROR_ARG;
	}

	reboot_delay_seconds = atoi((const char *)&reboot_delay[2]);

	start_json_object(&jw, response_span);
	append_jason_property_string(&jw, command_reboot_status_span, command_reboot_success_span);
	append_json_property_int32(&jw, command_reboot_delay_span, reboot_delay_seconds);
	end_json_object(&jw);

	*out_response_span = az_json_writer_get_bytes_used_in_destination(&jw);

	return AZ_OK;
}

/**********************************************
* Handle commands
**********************************************/
static void handle_command_message(
				az_span payload_span,
				az_iot_pnp_client_command_request* command_request)
{
	az_span command_response_span = AZ_SPAN_FROM_BUFFER(commands_response_payload);
	uint16_t return_code;
	az_result az_ret;
	int ret;

	if (az_span_is_content_equal(command_getmaxmin_span, command_request->command_name))
	{
		az_ret = process_getMaxMinReport(payload_span, command_response_span, &command_response_span);
		if (az_ret != AZ_OK)
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
			debug_printError("  MAIN: Fail send command response, az_ret 0x%08x status %d", az_ret, ret);
		}
	}
	else if (az_span_is_content_equal(command_reboot_span, command_request->command_name))
	{
		az_ret = process_reboot(payload_span, command_response_span, &command_response_span);

		if (az_ret != AZ_OK)
		{
			debug_printError("  MAIN: Fail process_reboot, az_ret 0x%08x span size %d", az_ret, az_span_size(command_response_span));
			if(az_span_size(command_response_span) == 0)
			{
				// if response is empty, payload was not in the right format.
				if (az_result_failed(az_ret = build_error_response_payload(command_response_span, error_no_payload_reboot_span, &command_response_span)))
				{
					debug_printError("  MAIN: Fail build error response. (0x%08x)", az_ret);
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
			debug_printError("  MAIN: Unable to send command response, az_result 0x%08x status %d", az_ret, ret);
		}

		if (ret == 0 && return_code == 200)
		{
			debug_printGood("  MAIN: Reboot Time Set");
			timeout_create(&reboot_timer, timeout_mSecToTicks(reboot_delay_seconds * 1000));
		}
	}
	else
	{
		// Unsupported command
		debug_printError("  MAIN: Unsupported command received: %s.",
						az_span_ptr(command_request->command_name));

		if ((ret = send_command_response(command_request, 404, error_command_not_supported_span)) != 0)
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
	debug_printInfo("  MAIN: Command Received");

	az_iot_pnp_client_command_request command_request;
	az_span command_topic_span = az_span_create_from_str((char*)topic);
	az_result rc = az_iot_pnp_client_commands_parse_received_topic(&pnp_client, command_topic_span, &command_request);
	RETURN_WITH_MESSAGE_IF_FAILED(rc, "az_iot_pnp_client_commands_parse_received_topic failed");

	handle_command_message(az_span_create_from_str((char*)payload), &command_request);
}

/**********************************************
* Send Reported Property
**********************************************/
static int send_reported_property(
	twin_properties_t* twin_properties)
{
	az_result az_ret;

	debug_printInfo("  MAIN: send property");

	// Get the topic used to send a reported property update
	az_span request_id_span = get_request_id();
	if (az_result_failed(
		az_ret = az_iot_pnp_client_property_patch_get_publish_topic(
					&pnp_client,
					request_id_span,
					reported_property_topic,
					sizeof(reported_property_topic),
					NULL)))
	{
		debug_printError("  MAIN: Fail get twin document publish topic:0x%08x", az_ret);
		return az_ret;
	}

	// Clear buffer and initialize JSON Payload.	This creates "{"
	az_json_writer jw;
	memset(reported_property_payload, 0, sizeof(reported_property_payload));
	az_span az_span_buffer = AZ_SPAN_FROM_BUFFER(reported_property_payload);

	if (az_result_failed(az_ret = start_json_object(&jw, az_span_buffer)))
	{
		debug_printError("  MAIN: Fail initialize json_builder (0x%08x)", az_ret);
		return az_ret;
	}

	if (twin_properties->flag.telemetry_interval_found)
	{
		if (az_result_failed(
			az_ret = append_reported_property_response_int32(
						&jw,
						telemetry_interval_property_name_span,
						telemetry_interval_seconds,
						200,
						twin_properties->version_num,
						AZ_SPAN_FROM_STR("Success"))))
		{
			debug_printError("  MAIN: Fail add property for telemetry interval:0x%08x", az_ret);
			return az_ret;
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
			az_ret = append_reported_property_response_int32(
						&jw,
						led_yellow_property_name_span,
						yellow_led,
						200,
						twin_properties->version_num,
						AZ_SPAN_FROM_STR("Success"))))
		{
			debug_printError("  MAIN: Unable to add property LED(y):0x%08x", az_ret);
			return az_ret;
		}
	}

	// Add Red LED
	// Example with String Enum
	if (twin_properties->reported_led_red != LED_NO_CHANGE)
	{
		az_span red_led_value_span;

		switch (twin_properties->reported_led_red)
		{
			case 1:
			red_led_value_span = led_on_string_span;
			break;

			case 2:
			red_led_value_span = led_off_string_span;
			break;

			case 3:
			red_led_value_span = led_blink_string_span;
			break;
		}

		if (az_result_failed(
			az_ret = append_jason_property_string(
				&jw,
				led_red_property_name_span,
				red_led_value_span)))
		{
			debug_printError("  MAIN: Unable to add property LED(r):0x%08x", az_ret);
			return az_ret;
		}
	}

	// Add Blue LED
	if (twin_properties->reported_led_blue != LED_NO_CHANGE)
	{
		if (az_result_failed(
			az_ret = append_json_property_int32(
				&jw,
				led_blue_property_name_span,
				twin_properties->reported_led_blue)))
		{
			debug_printError("  MAIN: Unable to add property LED(b):0x%08x", az_ret);
			return az_ret;
		}
	}

	// Add Green LED
	if (twin_properties->reported_led_green != LED_NO_CHANGE)
	{
		if (az_result_failed(
			az_ret = append_json_property_int32(
				&jw,
				led_green_property_name_span,
				twin_properties->reported_led_green)))
		{
			debug_printError("  MAIN: Unable to add property LED(g):0x%08x", az_ret);
			return az_ret;
		}
	}

	if (twin_properties->flag.max_temp_updated || twin_properties->flag.isGet == 1)
	{
		if (az_result_failed(
			az_ret = append_json_property_int32(
				&jw,
				property_max_temp_span,
				device_max_temp)))
		{
			debug_printError("  MAIN: Unable to add property Max Temp:0x%08x)", az_ret);
			return az_ret;
		}
	}

	// Close JSON Payload (appends "}")
	if (az_result_failed(az_ret = end_json_object(&jw)))
	{
		debug_printError("  MAIN: Unable to append end object:0x%08x", az_ret);
		return az_ret;
	}

	az_span json_payload_span = az_json_writer_get_bytes_used_in_destination(&jw);

	// Publish the reported property payload to IoT Hub
	debug_printInfo("IOTHUB: Sending twin reported property:%s", az_span_ptr(json_payload_span));

	// Send the reported property
	if ((az_ret = mqtt_publish_message(reported_property_topic, json_payload_span, 0)) == 0)
	{
		debug_printInfo("  MAIN: PUBLISH : Reported property");
	}

	return az_ret;
}

/**********************************************
* Parse Desired Property (Writable Property)
**********************************************/
static az_result parse_twin_property(
	az_span twin_payload_span,
	twin_properties_t* twin_properties,
	az_iot_pnp_client_property_response_type response_type)
{
	az_json_reader jr;
	az_span component_name_span;
	az_json_reader jr_property_name_and_value;

	RETURN_ERR_IF_FAILED(az_json_reader_init(&jr, twin_payload_span, NULL));
	RETURN_ERR_IF_FAILED(az_iot_pnp_client_property_get_property_version(
		&pnp_client, &jr, response_type, &twin_properties->version_num));

	twin_properties->flag.version_found = 1;

	RETURN_ERR_IF_FAILED(az_json_reader_init(&jr, twin_payload_span, NULL));

	while (az_result_succeeded(az_iot_pnp_client_property_get_next_component_property(
		&pnp_client, &jr, response_type, &component_name_span, &jr_property_name_and_value)))
	{
		if (az_json_token_is_text_equal(&jr_property_name_and_value.token, led_yellow_property_name_span)) {
			// found writable property to control Yellow LED
			RETURN_ERR_IF_FAILED(az_json_reader_next_token(&jr_property_name_and_value));
			RETURN_ERR_IF_FAILED(az_json_token_get_int32(&jr_property_name_and_value.token, &twin_properties->desired_led_yellow));
			twin_properties->flag.yellow_led_found = 1;
		}
		else if (az_json_token_is_text_equal(&jr_property_name_and_value.token, property_telemetry_interval_span)) {
 			// found writable property to adjust telemetry interval
 			RETURN_ERR_IF_FAILED(az_json_reader_next_token(&jr_property_name_and_value));
 			RETURN_ERR_IF_FAILED(az_json_token_get_uint32(&jr_property_name_and_value.token, &telemetry_interval_seconds));
 			twin_properties->flag.telemetry_interval_found = 1;
 		}
	}

	return AZ_OK;
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

	az_result az_ret;
	twin_properties_t twin_properties;

	init_twin_data(&twin_properties);

	// Determine what type of incoming twin message this is. Print relevant data for the message.
	switch (property_response->response_type)
	{
		// A response from a twin GET publish message with the twin document as a payload.
		case AZ_IOT_PNP_CLIENT_PROPERTY_RESPONSE_TYPE_GET:

			debug_printGood("  MAIN: twin GET response received");
			is_get_received = true;
			twin_properties.flag.isGet = 1;
			break;

		// An update to the desired properties with the properties as a JSON payload.
		case AZ_IOT_PNP_CLIENT_PROPERTY_RESPONSE_TYPE_DESIRED_PROPERTIES:

			debug_printInfo("  MAIN: twin desired property patch message received");
			twin_properties.flag.isGet = 0;
			break;

		// A response from a twin reported properties publish message. With a successful update of
		// the reported properties, the payload will be empty and the status will be 204.
		case AZ_IOT_PNP_CLIENT_PROPERTY_RESPONSE_TYPE_REPORTED_PROPERTIES:

			debug_printInfo("  MAIN: twin reported property response message received");
			return;
	}

	if (az_result_failed(az_ret = parse_twin_property(payload_span, &twin_properties, property_response->response_type)))
	{
		// If the item can't be found, the desired temp might not be set so take no action
		debug_printError("  MAIN: Couldn't parse desired property (0x%08x)\n", az_ret);
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
		debug_printWarn("  MAIN: NULL topic");
		return;
	}

	az_iot_pnp_client_property_response property_response;
	az_span property_topic_span = az_span_create_from_str((char *)topic);
	az_result rc = az_iot_pnp_client_property_parse_received_topic(&pnp_client, property_topic_span, &property_response);
	RETURN_WITH_MESSAGE_IF_FAILED(rc, "az_iot_pnp_client_property_parse_received_topic failed");

	if (az_span_size(property_response.request_id) != 0 && IOT_DEBUG_PRINT)
	{
		char request_id_buf[50];
		az_span_to_str(request_id_buf, sizeof(request_id_buf), property_response.request_id);
		debug_printInfo("  MAIN: Property request, id:%s, status:%d", request_id_buf, property_response.status);
	}

	if (payload == NULL)
	{
		debug_printWarn("  MAIN: NULL payload");
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
		debug_printWarn("  MAIN: NULL topic");
		return;
	}

	az_iot_pnp_client_property_response property_response;
	az_span property_topic_span = az_span_create_from_str((char *)topic);
	az_result rc = az_iot_pnp_client_property_parse_received_topic(&pnp_client, property_topic_span, &property_response);
	RETURN_WITH_MESSAGE_IF_FAILED(rc, "az_iot_pnp_client_property_parse_received_topic failed");

	if (payload == NULL)
	{
		debug_printWarn("  MAIN: NULL payload");
	}
	else
	{
		handle_property_message(az_span_create_from_str((char*)payload), &property_response);
	}
}

/**********************************************
* Check if button(s) was pressed.
**********************************************/
void check_button_status(void)
{
	az_span button_event_span = AZ_SPAN_FROM_BUFFER(button_event_payload);
	az_json_writer jw;

	// save flags in case user pressed buttons very fast, and for error case.
	bool sw0_pressed = button_press_data.flag.sw0 == 1 ? true : false;
	bool sw1_pressed = button_press_data.flag.sw1 == 1 ? true : false;

	// clear the flags
	button_press_data.flag.AsUSHORT = 0;

	if (sw0_pressed != true && sw1_pressed != true)
	{
		return;
	}

	RETURN_IF_FAILED(start_json_object(&jw, button_event_span));

	if (sw0_pressed == true)
	{
		debug_printGood("  MAIN: Button SW0 Count %lu", button_press_data.sw0_press_count);
		RETURN_IF_FAILED(append_button_press_telemetry(&jw, event_name_button_sw0_span, button_press_data.sw0_press_count));
	}

	if (sw1_pressed == true)
	{
		debug_printGood("  MAIN: Button SW1 Count %lu", button_press_data.sw1_press_count);
		RETURN_IF_FAILED(append_button_press_telemetry(&jw, event_name_button_sw1_span, button_press_data.sw1_press_count));
	}

	RETURN_IF_FAILED(end_json_object(&jw));

	button_event_span = az_json_writer_get_bytes_used_in_destination(&jw);
	if (mqtt_publish_message(telemetry_topic, button_event_span, 0) != 0)
	{
		debug_printError("  MAIN: Failed to publish Button Press event telemetry");
	}
}

/**********************************************
* Check if LED status has changed or not.
**********************************************/
void check_led_status(twin_properties_t* twin_properties)
{
	twin_properties_t* twin_properties_ptr;
	twin_properties_t twin_properties_local;

	bool b_force_sync = false; // force LED status synchronization if this request is coming from Twin Get

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
	b_force_sync = twin_properties_ptr->flag.isGet == 1 ? true : false;

	if (led_status.change_flag.AsUSHORT != 0 || b_force_sync)
	{
		if (led_status.change_flag.blue == 1 || b_force_sync)
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

		if (led_status.change_flag.green == 1 || b_force_sync)
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

		if (led_status.change_flag.red == 1 || b_force_sync)
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

		// if this is from Get Twin, Device Twin code path will update reported properties
		if (!b_force_sync)
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
