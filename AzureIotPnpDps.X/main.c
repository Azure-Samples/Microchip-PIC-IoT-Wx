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
#include "platform/cryptoauthlib/lib/atca_command.h"
#include "platform/cryptoauthlib/lib/basic/atca_basic.h"
#include "platform/drivers/timeout.h"
#include "platform/delay.h"
#include "platform/debug_print.h"
#include "platform/mqtt/mqtt_core/mqtt_core.h"
#include "platform/mqtt/mqtt_packetTransfer_interface.h"
#include "azure/core/az_span.h"
#include "azure/core/az_json.h"
#include "azure/iot/az_iot_hub_client.h"
#include "main.h"

#define DEFAULT_START_TEMP_CELSIUS 22
#define DEBOUNCE_DLY_MSEC 50 // Delay to prevent switch debouncing on release

extern az_iot_hub_client hub_client;
extern pf_MQTT_CLIENT pf_mqtt_iotprovisioning_client;
extern pf_MQTT_CLIENT pf_mqtt_iothub_client;
extern led_change_t led_change;

static int rebootDelay = 5;
static uint32_t reboot_task(void* payload);
static timerStruct_t reboot_timer = { reboot_task };

static bool isGetReceived = false;
extern uint32_t telemetry_interval;
/*
* LEDs
*			   | On			              | Off			                 | Blink Slow 500ms	     | Blink Fast (200ms)
* Blue	 | AP Connected         | AP Disconnected          | WiFi Soft AP Mode	   | WiFi connection in progress
* Green	 | Connected to IoT Hub | Not Connected to IoT Hub | Connecting to IoT Hub | Provisioning |
* Yellow | User LED.  Control from Cloud (On, Off, Blink)
* Red    | Error                
*/

#define LED_INIT (-1)

// * PnP Values *
// The model id is the JSON document (also called the Digital Twins Model Identifier or DTMI)
// which defines the capability of your device. The functionality of the device should match what
// is described in the corresponding DTMI. Should you choose to program your own PnP capable device,
// the functionality would need to match the DTMI and you would need to update the below 'model_id'.
// Please see the sample README for more information on this DTMI.
const az_span device_model_id = AZ_SPAN_LITERAL_FROM_STR("dtmi:com:Microchip:PIC_IoT_WM;1");
// ISO8601 Time Format
static const char iso_spec_time_format[] = "%Y-%m-%dT%H:%M:%SZ";

// IoT Hub Connection Values
static int32_t request_id_int;
static char request_id_buf[8];

// IoT Hub Telemetry Values
char telemetry_topic[128];
static const az_span telemetry_name_temperature = AZ_SPAN_LITERAL_FROM_STR("temperature");
static const az_span telemetry_name_light = AZ_SPAN_LITERAL_FROM_STR("light");
static const az_span telemetry_name_sw0 = AZ_SPAN_LITERAL_FROM_STR("button_presses_sw0");
static const az_span telemetry_name_sw1 = AZ_SPAN_LITERAL_FROM_STR("button_presses_sw1");
static char telemetry_payload[256];

// IoT Hub Commands Values
static char commands_response_topic[128];

static const az_span report_command_name_span = AZ_SPAN_LITERAL_FROM_STR("getMaxMinReport");
static const az_span report_max_temp_name_span = AZ_SPAN_LITERAL_FROM_STR("maxTemp");
static const az_span report_min_temp_name_span = AZ_SPAN_LITERAL_FROM_STR("minTemp");
static const az_span report_avg_temp_name_span = AZ_SPAN_LITERAL_FROM_STR("avgTemp");
static const az_span report_start_time_name_span = AZ_SPAN_LITERAL_FROM_STR("startTime");
static const az_span report_end_time_name_span = AZ_SPAN_LITERAL_FROM_STR("endTime");
static const az_span report_error_payload = AZ_SPAN_LITERAL_FROM_STR("{}");
static char end_time_buffer[32];
static char commands_response_payload[256];
static char incoming_since_value[32];

static const az_span reboot_command_name_span = AZ_SPAN_LITERAL_FROM_STR("reboot");
static const az_span reboot_command_response_span = AZ_SPAN_LITERAL_FROM_STR("RebootResponse");
static const char reboot_command_response_msg_format[] = "Rebooting in %d seconds";

// IoT Hub Twin Values
//static char twin_get_topic[128];
static char reported_property_topic[128];
static const az_span desired_property_name = AZ_SPAN_LITERAL_FROM_STR("desired");
static const az_span desired_property_version_name = AZ_SPAN_LITERAL_FROM_STR("$version");
static const az_span desired_response_value_name = AZ_SPAN_LITERAL_FROM_STR("value");
static const az_span desired_ack_code_name = AZ_SPAN_LITERAL_FROM_STR("ac");
static const az_span desired_ack_version_name = AZ_SPAN_LITERAL_FROM_STR("av");
static const az_span desired_ack_description_name = AZ_SPAN_LITERAL_FROM_STR("ad");
static const az_span max_temp_reported_property_name = AZ_SPAN_LITERAL_FROM_STR("maxTempSinceLastReboot");
static const az_span led_blue_property_name = AZ_SPAN_LITERAL_FROM_STR("led_blue");
static const az_span led_green_property_name = AZ_SPAN_LITERAL_FROM_STR("led_green");
static const az_span led_yellow_property_name = AZ_SPAN_LITERAL_FROM_STR("led_yellow");
static const az_span led_red_property_name = AZ_SPAN_LITERAL_FROM_STR("led_red");
static char reported_property_payload[512];
static const az_span telemetry_interval_property_name = AZ_SPAN_LITERAL_FROM_STR("telemetryInterval");
static const az_span device_id_property_name = AZ_SPAN_LITERAL_FROM_STR("deviceId");

static const az_span led_on_string	= AZ_SPAN_LITERAL_FROM_STR("On");
static const az_span led_off_string   = AZ_SPAN_LITERAL_FROM_STR("Off");
static const az_span led_blink_string = AZ_SPAN_LITERAL_FROM_STR("Blink");

// PnP Device Values
static int32_t device_current_temp;
static int32_t device_current_light;
static int32_t device_temperature_avg_total;
static uint32_t device_temperature_avg_count = 0;
static int32_t device_max_temp;
static int32_t device_min_temp;
static int32_t device_avg_temp;

#define RETURN_IF_AZ_RESULT_FAILED(exp) \
  do									\
  {									 \
    az_result const _az_result = (exp); \
    if (az_result_failed(_az_result))   \
    {								   \
      return _az_result;				\
    }								   \
  } while (0)


static uint32_t reboot_task(void* payload)
{
  debug_printGOOD("Reboot");
	asm("RESET");
  return 0;
}

/**********************************************
* Start JSON_BUILDER for JSON Document
* This creates a json with "{" 
**********************************************/
static az_result start_json_document(
  az_json_writer* jw,
  az_span az_span_buffer)
{
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_init(jw, az_span_buffer, NULL));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_begin_object(jw));
  return AZ_OK;
}

/**********************************************
* Start JSON_BUILDER for JSON Document
* This adds "}" to JSON 
**********************************************/
static az_result end_json_document(
	az_json_writer* jw)
{
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_end_object(jw));
  return AZ_OK;
}

/**********************************************
*  Add Reported Property int32 data
**********************************************/
static az_result append_jason_data_int32(
	az_json_writer* jw,
	az_span property_name,
	int32_t property_val)
{
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_property_name(jw, property_name));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_int32(jw, property_val));
  return AZ_OK;
}

/**********************************************
*  Add Reported Property string data
**********************************************/
static az_result append_jason_data_string(
	az_json_writer* jw,
	az_span property_name,
	az_span property_val)
{
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_property_name(jw, property_name));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_string(jw, property_val));
  return AZ_OK;
}

/**********************************************
*  Add Reported Property bool data
**********************************************/
// static az_result add_reported_property_bool(
//	 az_json_writer* jw,
//	 az_span property_name,
//	 bool property_val)
// {
//   RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_property_name(jw, property_name));
//   RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_bool(jw, property_val));
//   return AZ_OK;
// }

/**********************************************
*  Add Reported Property int32 element for desired property response
**********************************************/
static az_result append_reported_property_response_int32(
	az_json_writer* jw,
	az_span property_name,
	int32_t property_val,
	int32_t ack_code_value,
	int32_t ack_version_value,
	az_span ack_description_value)
{
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_property_name(jw, property_name));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_begin_object(jw));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_property_name(jw, desired_response_value_name));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_int32(jw, property_val));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_property_name(jw, desired_ack_code_name));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_int32(jw, ack_code_value));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_property_name(jw, desired_ack_version_name));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_int32(jw, ack_version_value));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_property_name(jw, desired_ack_description_name));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_string(jw, ack_description_value));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_end_object(jw));

  return AZ_OK;
}

void init_property_data(twin_properties_t* twin_properties)
{

  twin_properties->flag.AsUSHORT = 0;
  twin_properties->version_num = 0;
  twin_properties->desired_led_yellow = LED_INIT;
  twin_properties->reported_led_red = LED_INIT;
  twin_properties->reported_led_blue = LED_INIT;
  twin_properties->reported_led_green = LED_INIT;
}

// Create request id span which increments source int each call. Capable of holding 8 digit number.
static az_span get_request_id(void)
{
  az_span remainder;
  az_span out_span = az_span_create((uint8_t*)request_id_buf, sizeof(request_id_buf));
  az_result result = az_span_i32toa(out_span, request_id_int++, &remainder);
  (void)remainder;
  (void)result;
  return out_span;
}

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
  	debug_printError("MQTT: Connection lost PUBLISH failed");
  }

  return 0;
}

// Send the response of the command invocation
static int send_command_response(
	az_iot_hub_client_method_request* request,
	uint16_t status,
	az_span response)
{
  int result;
  // Get the response topic to publish the command response
  if (az_result_failed(
		  result = az_iot_hub_client_methods_response_get_publish_topic(
			  &hub_client,
			  request->request_id,
			  status,
			  commands_response_topic,
			  sizeof(commands_response_topic),
			  NULL)))
  {
    debug_printError("Unable to get twin document publish topic");
    return result;
  }

  debug_printInfo("Command Status: %u", status);

  // Send the commands response
  if ((result = mqtt_publish_message(commands_response_topic, response, 0)) == 0)
  {
	  debug_printInfo("Sent command response");
  }

  return result;
}

static az_result build_getMaxMinReport_response_payload(
	az_json_writer* jw,
	az_span start_time_span,
	az_span end_time_span,
	az_span* response_payload)
{
  // Build the command response payload
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_begin_object(jw));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_property_name(jw, report_max_temp_name_span));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_int32(jw, device_max_temp));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_property_name(jw, report_min_temp_name_span));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_int32(jw, device_min_temp));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_property_name(jw, report_avg_temp_name_span));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_int32(jw, device_avg_temp));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_property_name(jw, report_start_time_name_span));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_string(jw, start_time_span));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_property_name(jw, report_end_time_name_span));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_string(jw, end_time_span));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_end_object(jw));

  *response_payload = az_json_writer_get_bytes_used_in_destination(jw);

  return AZ_OK;
}

static az_result invoke_getMaxMinReport(az_span payload, az_span response, az_span* out_response)
{
	if (az_span_size(payload) == 0)
	{
		return AZ_ERROR_ITEM_NOT_FOUND;
	}

  debug_printInfo("invoke_getMaxMinReport payload : %s", az_span_ptr(payload));

  // Parse the "since" field in the payload.
  az_span start_time_span = AZ_SPAN_EMPTY;
  az_json_reader jr;
  RETURN_IF_AZ_RESULT_FAILED(az_json_reader_init(&jr, payload, NULL)); 
  RETURN_IF_AZ_RESULT_FAILED(az_json_reader_next_token(&jr));
  int32_t incoming_since_value_len;
  RETURN_IF_AZ_RESULT_FAILED(az_json_token_get_string(
	  &jr.token, incoming_since_value, sizeof(incoming_since_value), &incoming_since_value_len));
  start_time_span = az_span_create((uint8_t*)incoming_since_value, incoming_since_value_len);

  // Set the response payload to error if the "since" field was not sent
  if (az_span_ptr(start_time_span) == NULL)
  {
    response = report_error_payload;
    return AZ_ERROR_ITEM_NOT_FOUND;
  }

  // Get the current time as a string
  time_t rawtime;
  struct tm* timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  size_t len = strftime(end_time_buffer, sizeof(end_time_buffer), iso_spec_time_format, timeinfo);
  debug_printGOOD("end_time_buffer %s", end_time_buffer);
  az_span end_time_span = az_span_create((uint8_t*)end_time_buffer, (int32_t)len);

  az_json_writer jw;
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_init(&jw, response, NULL));
  RETURN_IF_AZ_RESULT_FAILED(
	  build_getMaxMinReport_response_payload(&jw, start_time_span, end_time_span, out_response));

  return AZ_OK;
}

static az_result invoke_reboot(az_span payload, az_span response, az_span* out_response)
{
  char reboot_delay[32];
  char buffer[32] = {0};
  char responseMsg[32];

  az_json_reader jr;

	if (az_span_size(payload) == 0)
	{
    debug_printError("Empty Payload");
    response = report_error_payload;
		return AZ_ERROR_ITEM_NOT_FOUND;
	}
	
  RETURN_IF_AZ_RESULT_FAILED(az_json_reader_init(&jr, payload, NULL)); 
  RETURN_IF_AZ_RESULT_FAILED(az_json_reader_next_token(&jr));
  RETURN_IF_AZ_RESULT_FAILED(az_json_token_get_string(&jr.token, reboot_delay, sizeof(reboot_delay), NULL));

  if (reboot_delay[0] != 'P' || reboot_delay[1] != 'T' || reboot_delay[strlen(reboot_delay) - 1] != 'S')
  {
    debug_printError("Reboot Delay wrong format");
    return AZ_ERROR_ARG;
  }

  memcpy(buffer, &reboot_delay[2], strlen(reboot_delay) - 3);
  buffer[strlen(reboot_delay) - 3] = '\0';
  rebootDelay = atoi((const char *)buffer);

  sprintf(responseMsg, reboot_command_response_msg_format, rebootDelay);

  return AZ_OK;
}

// Invoke the requested command if supported and return status | Return error otherwise
static void handle_command_message(
	az_span payload,
	az_iot_hub_client_method_request* command_request)
{
  uint16_t return_code;
  int result;

  debug_printInfo("handle_command_message %s", command_request->name);

  if (az_span_is_content_equal(report_command_name_span, command_request->name))
  {
    az_span command_response_span = AZ_SPAN_FROM_BUFFER(commands_response_payload);

    // Invoke command
    az_result response = invoke_getMaxMinReport(payload, command_response_span, &command_response_span);
    if (response != AZ_OK)
    {
      return_code = 400;
    }
    else
    {
      return_code = 200;
    }

    // Send command response with report as JSON payload
    if ((result = send_command_response(command_request, return_code, command_response_span)) != 0)
    {
      debug_printError("Unable to send %u response, status %d", return_code, result);
    }
  }
  else if (az_span_is_content_equal(reboot_command_name_span, command_request->name))
  {
    az_span command_response_span = AZ_SPAN_FROM_BUFFER(commands_response_payload);

    az_result response = invoke_reboot(payload, command_response_span, &command_response_span);

    debug_printInfo("<< invoke_reboot");

    if (response != AZ_OK)
    {
      return_code = 400;
    }
    else
    {
      return_code = 200;
    }

    debug_printGOOD("Reboot Response %s", az_span_ptr(command_response_span));
    // Send command response with report as JSON payload
    if ((result = send_command_response(command_request, return_code, command_response_span)) != 0)
    {
      debug_printError("Unable to send %u response, status %d", return_code, result);
    }

 	  if (result == 0)
	  {
      debug_printInfo("Reboot Time Set");
      timeout_create(&reboot_timer, timeout_mSecToTicks(rebootDelay * 1000));
	  }
  }
  else
  {
    // Unsupported command
    debug_printError(
      "Unsupported command received: %.*s.",
      az_span_size(command_request->name),
      az_span_ptr(command_request->name));

    if ((result = send_command_response(command_request, 404, report_error_payload)) != 0)
    {
      debug_printError("Unable to send %d response, status %d", 404, result);
    }
  }
}

void receivedFromCloud_methods(uint8_t* topic, uint8_t* payload)
{
  debug_printInfo("Methods");
	az_iot_hub_client_method_request method_request;
	az_result result = az_iot_hub_client_methods_parse_received_topic(&hub_client, az_span_create_from_str((char*)topic), &method_request);
	if (az_result_failed(result))
	{
		debug_printError("az_iot_hub_client_methods_parse_received_topic failed");
		return;
	}

  handle_command_message(az_span_create_from_str((char*)payload), &method_request);
}

/*****************************************************************************************

Properties (Device Twin)

*****************************************************************************************/
/**********************************************
*  Build single reported property JSON for int32 data type
**********************************************/
// static az_result build_reported_property(
// 	az_json_writer* jw,
// 	az_span property_name,
// 	int32_t property_val)
// {
//   RETURN_IF_AZ_RESULT_FAILED(
// 	  az_json_writer_init(jw, AZ_SPAN_FROM_BUFFER(reported_property_payload), NULL));
//   RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_begin_object(jw));
//   RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_property_name(jw, property_name));
//   RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_int32(jw, property_val));
//   RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_end_object(jw));

//   return AZ_OK;
// }

/**********************************************
*  Send Reported Property
**********************************************/
static int send_reported_property(
	twin_properties_t* twin_properties)
{
  az_result result;
 
  debug_printInfo("%s() : %d", __func__, __LINE__);

  // Get the topic used to send a reported property update
  az_span request_id_span = get_request_id();
  if (az_result_failed(
		  result = az_iot_hub_client_twin_patch_get_publish_topic(
			  &hub_client,
			  request_id_span,
			  reported_property_topic,
			  sizeof(reported_property_topic),
			  NULL)))
  {
    debug_printError("Unable to get twin document publish topic, return code %d", result);
    return result;
  }

  // Clear buffer and initialize JSON Payload.  This creates "{"
  az_json_writer jw;
  memset(reported_property_payload, 0, sizeof(reported_property_payload));
  az_span az_span_buffer = AZ_SPAN_FROM_BUFFER(reported_property_payload);

  if (az_result_failed(result = start_json_document(&jw, az_span_buffer)))
  {
    debug_printError("Unable to initialize json_builder, return code %d", result);
    return result;
  }

  debug_printInfo("%s() : %d", __func__, __LINE__);

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
      debug_printError("Unable to add property for telemetry interval, return code %d", result);
      return result;
    }
  }

  // Add Yellow LED
  // Example with integer Enum
  if (twin_properties->desired_led_yellow != LED_INIT)
  {
    int32_t yellow_led;

    if (LED_isBlinkingYellow() == true)
    {
      yellow_led = 3;
    }
    else
    {
      yellow_led = LED_YELLOW_GetValue() + 1;
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
      debug_printError("Unable to add property for Yellow LED, return code %d", result);
      return result;
    }
  }

  // Add Red LED
  // Example with String Enum
  if (twin_properties->reported_led_red != LED_INIT)
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
        result = append_jason_data_string(
          &jw,
          led_red_property_name,
          red_led_value)))
    {
      debug_printError("Unable to add property for Red LED, return code %d", result);
      return result;
    }
  }

  // Add Blue LED
  if (twin_properties->reported_led_blue != LED_INIT)
  {
    if (az_result_failed(
        result = append_jason_data_int32(
          &jw,
          led_blue_property_name,
          twin_properties->reported_led_blue)))
    {
      debug_printError("Unable to add property for Blue LED, return code %d", result);
      return result;
    }
  }

  // Add Green LED
  if (twin_properties->reported_led_green != LED_INIT)
  {
    if (az_result_failed(
        result = append_jason_data_int32(
          &jw,
          led_green_property_name,
          twin_properties->reported_led_green)))
    {
      debug_printError("Unable to add property for Green LED, return code %d", result);
      return result;
    }
  }

  if (twin_properties->flag.isGet == 1)
  {
    // add other reported properties
  }

  if (twin_properties->flag.max_temp_updated || twin_properties->flag.isGet == 1)
  {
    if (az_result_failed(
        result = append_jason_data_int32(
          &jw,
          max_temp_reported_property_name,
          device_max_temp)))
    {
      debug_printError("Unable to add property for Green LED, return code %d", result);
      return result;
    }
  }

  // Close JSON Payload (appends "}")
  if (az_result_failed(result = end_json_document(&jw)))
  {
    debug_printError("Unable to append end object, return code %d", result);
    return result;
  }

  az_span json_payload = az_json_writer_get_bytes_used_in_destination(&jw);

  // Publish the reported property payload to IoT Hub
  debug_printInfo("Sending twin reported property : %s", az_span_ptr(json_payload));
  result = mqtt_publish_message(reported_property_topic, json_payload, 0);


  return result;
}

/**********************************************
*  Send Reported Property
**********************************************/
void check_led_status(twin_properties_t* twin_properties)
{
  twin_properties_t* twin_properties_ptr;
  bool bForceSync = false; // force LED status synchronization if this request is coming from Twin Get

  twin_properties_t twin_properties_local;
  init_property_data(&twin_properties_local);

  if (twin_properties == NULL)
  {
    twin_properties_ptr = &twin_properties_local;
  }
  else {
    twin_properties_ptr = twin_properties;
  }

  if (led_change.AsUSHORT == 0 && twin_properties_ptr->flag.isGet == 0)
  {
    return;
  }

  debug_printInfo("%s() led_change %x", __func__, led_change.AsUSHORT);

  bForceSync = twin_properties_ptr->flag.isGet == 1 ? true : false;

  if (led_change.AsUSHORT != 0 || bForceSync)
  {
    if (led_change.blue == 1 || bForceSync)
    {
      if (LED_isBlinkingBlue() == true)
      {
        twin_properties_ptr->reported_led_blue = 3;
      }
      else {
        twin_properties_ptr->reported_led_blue = LED_BLUE_GetValue() + 1;
      }
    }
    else {
      twin_properties_ptr->reported_led_blue = LED_INIT;
    }

    if (led_change.green == 1 || bForceSync)
    {
      if (LED_isBlinkingGreen() == true)
      {
        twin_properties_ptr->reported_led_green = 3;
      }
      else {
        twin_properties_ptr->reported_led_green = LED_GREEN_GetValue() + 1;
      }
    } else
    {
      twin_properties_ptr->reported_led_green = LED_INIT;
    }

    if (led_change.red == 1 || bForceSync)
    {
      if (LED_isBlinkingRed() == true)
      {
        twin_properties_ptr->reported_led_red = 3;
      }
      else {
        twin_properties_ptr->reported_led_red = LED_RED_GetValue() + 1;
      }
    } else
    {
      twin_properties_ptr->reported_led_red = LED_INIT;
    }

    // clear flags
    led_change.AsUSHORT = 0;

    // if this is from Get Twin, Device Twin code pass will update reported properties 
    if (!bForceSync)
    {
      send_reported_property(twin_properties_ptr);
    }
  }
}

/**********************************************
*  Update and track temperature data
**********************************************/
static void update_sensor_data(twin_properties_t* twin_properties)
{
  int16_t temp = SENSORS_getTempValue();
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

  device_current_light = SENSORS_getLightValue();

  return;
}

/**********************************************
*  Process LED Update/Patch
**********************************************/
static void update_leds(
	twin_properties_t* twin_properties)
{
  // If desired properties are not set, send current LED states.
  // Otherwise, set LED state based on Desired property
  if (twin_properties->flag.yellow_led_found == 1 && twin_properties->desired_led_yellow != LED_INIT)
  {
    if ((LED_isBlinkingYellow() == true) && (twin_properties->desired_led_yellow != 3))
    {
      LED_stopBlinkingAndSetYellow(twin_properties->desired_led_yellow == 2);
    }

    if (twin_properties->desired_led_yellow == 1)
    {
      LED_holdYellow(LED_ON);
    }
    else if (twin_properties->desired_led_yellow == 2)
    {
      LED_holdYellow(LED_OFF);
    }
    else if (twin_properties->desired_led_yellow == 3)
    {
      LED_startBlinkingYellow();
    }
  }

  // If this is Twin Get, populate LED states for Red, Blue, Green LEDs
  if (twin_properties->flag.isGet == 1)
  {
    check_led_status(twin_properties);
  }
}

/**********************************************
* Parse Desired Property
**********************************************/
static az_result parse_twin_desired_property(
	az_span twin_payload_span,
	twin_properties_t* twin_properties)
{
  az_json_reader jr;
  bool desired_found = false;

  debug_printInfo("%s()", __func__);

  RETURN_IF_AZ_RESULT_FAILED(az_json_reader_init(&jr, twin_payload_span, NULL));
  RETURN_IF_AZ_RESULT_FAILED(az_json_reader_next_token(&jr));

  if (jr.token.kind != AZ_JSON_TOKEN_BEGIN_OBJECT)
  {
	  return AZ_ERROR_UNEXPECTED_CHAR;
  }

  debug_printInfo("Parsing Device Twin : \t%s", az_span_ptr(twin_payload_span));

  if (twin_properties->flag.isGet == 1)
  {
    // If is twin get payload, we have to parse one level deeper for "desired" wrapper
    RETURN_IF_AZ_RESULT_FAILED(az_json_reader_next_token(&jr));
    while (jr.token.kind != AZ_JSON_TOKEN_END_OBJECT)
    {
      if (jr.token.kind != AZ_JSON_TOKEN_PROPERTY_NAME)
      {
        debug_print("Token Kind %d", jr.token.kind);
        RETURN_IF_AZ_RESULT_FAILED(az_json_reader_skip_children(&jr));
      }
      else {
        if (az_json_token_is_text_equal(&jr.token, desired_property_name))
        {
          desired_found = true;
          RETURN_IF_AZ_RESULT_FAILED(az_json_reader_next_token(&jr));
          break;
        }
        else {
          RETURN_IF_AZ_RESULT_FAILED(az_json_reader_skip_children(&jr));
        }
      }
      RETURN_IF_AZ_RESULT_FAILED(az_json_reader_next_token(&jr));
    }
  }
  else {
	  desired_found = true;
  }

  if (!desired_found)
  {
    debug_printError("Desired property object not found in twin : %s", az_span_ptr(twin_payload_span));
    return AZ_ERROR_ITEM_NOT_FOUND;
  }

  if (jr.token.kind != AZ_JSON_TOKEN_BEGIN_OBJECT)
  {
    debug_printError("Start of a JSON object not found in twin : %s", az_span_ptr(twin_payload_span));
	  return AZ_ERROR_UNEXPECTED_CHAR;
  }

  RETURN_IF_AZ_RESULT_FAILED(az_json_reader_next_token(&jr));

  //
  // Loop through Desired Twin
  //
  while (jr.token.kind != AZ_JSON_TOKEN_END_OBJECT)
  {
    if (az_json_token_is_text_equal(&jr.token, desired_property_version_name))
    {
      // found $version
      RETURN_IF_AZ_RESULT_FAILED(az_json_reader_next_token(&jr));
      RETURN_IF_AZ_RESULT_FAILED(az_json_token_get_int32(&jr.token, &twin_properties->version_num));
      twin_properties->flag.version_found = 1;
    }
    else if (az_json_token_is_text_equal(&jr.token, led_yellow_property_name)) {
      // found writable property to control Yellow LED
      RETURN_IF_AZ_RESULT_FAILED(az_json_reader_next_token(&jr));
      RETURN_IF_AZ_RESULT_FAILED(az_json_token_get_int32(&jr.token, &twin_properties->desired_led_yellow));
      twin_properties->flag.yellow_led_found = 1;
    }
    else if (az_json_token_is_text_equal(&jr.token, telemetry_interval_property_name)) {
      // found writable property to adjust telemetry interval
      RETURN_IF_AZ_RESULT_FAILED(az_json_reader_next_token(&jr));
      RETURN_IF_AZ_RESULT_FAILED(az_json_token_get_uint32(&jr.token, &telemetry_interval));
      twin_properties->flag.telemetry_interval_found = 1;
    }
    else {
      // else ignore token.
      RETURN_IF_AZ_RESULT_FAILED(az_json_reader_skip_children(&jr));
    }
    RETURN_IF_AZ_RESULT_FAILED(az_json_reader_next_token(&jr));
  }

  // make sure $version and at least one of writable properties are found
  if (twin_properties->flag.version_found == 1 && (twin_properties->flag.AsUSHORT & (~1)) != 0)
  {
    return AZ_OK;
  }

  return AZ_ERROR_ITEM_NOT_FOUND;
}

/**********************************************
* Process Twin Update/Patch
**********************************************/
static void handle_twin_update(
	az_span payload,
	az_iot_hub_client_twin_response* twin_response)
{
  az_result result;
  twin_properties_t twin_properties = {0};

  init_property_data(&twin_properties);

  // Determine what type of incoming twin message this is. Print relevant data for the message.
  switch (twin_response->response_type)
  {
	// A response from a twin GET publish message with the twin document as a payload.
	case AZ_IOT_HUB_CLIENT_TWIN_RESPONSE_TYPE_GET:

	  debug_printInfo("A twin GET response received");
    isGetReceived = true;
    twin_properties.flag.isGet = 1;
	  break;

	// An update to the desired properties with the properties as a JSON payload.
	case AZ_IOT_HUB_CLIENT_TWIN_RESPONSE_TYPE_DESIRED_PROPERTIES:

	  debug_printInfo("A twin desired properties patch message received");
    twin_properties.flag.isGet = 0;
	  break;

	// A response from a twin reported properties publish message. With a successful update of
	// the reported properties, the payload will be empty and the status will be 204.
	case AZ_IOT_HUB_CLIENT_TWIN_RESPONSE_TYPE_REPORTED_PROPERTIES:

	  debug_printInfo("A twin reported properties response message was received : %s ", az_span_ptr(payload));
    return;
  }

  if (az_result_failed(result = parse_twin_desired_property(payload, &twin_properties)))
  {
    // If the item can't be found, the desired temp might not be set so take no action
    debug_printError("Could not parse desired property, return code %d\n", result);
  }
  else {
    update_leds(&twin_properties);
    send_reported_property(&twin_properties);
  }
}

//
// Callback for Twin Get
//
void receivedFromCloud_twin(uint8_t* topic, uint8_t* payload)
{

  debug_printGOOD("%s()", __func__);

	if (topic == NULL)
	{
		debug_printError("NULL topic");
		return;
	}

	az_iot_hub_client_twin_response twin_response;
	az_span twin_topic = az_span_create_from_str((char*)topic);
	az_result result = az_iot_hub_client_twin_parse_received_topic(&hub_client, twin_topic, &twin_response);
	if (az_result_failed(result))
	{
		debug_printError("az_iot_hub_client_twin_parse_received_topic failed, return code %d\n", result);
		return;
	}

	if (az_span_size(twin_response.request_id) != 0 && IOT_DEBUG_PRINT)
	{
		char request_id_buf[50];
		az_span_to_str(request_id_buf, sizeof(request_id_buf), twin_response.request_id);
		debug_printInfo("Twin request, request_id:%s, status: %d", request_id_buf, twin_response.status);
	}

	if (payload == NULL)
	{
		debug_printError("NULL payload");
		return; // no payload, nothing to process
	}

  handle_twin_update(az_span_create_from_str((char*)payload), &twin_response);
}

//
// Callback for Twin Patch
//
void receivedFromCloud_patch(uint8_t* topic, uint8_t* payload)
{
  debug_printInfo("Device Twin Patch");
	if (topic == NULL)
	{
	  debug_printError("NULL topic");
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

  if (payload)
  {
	  debug_printGOOD("Payload: %s", payload);
  }
  else
  {
	  debug_printError("NULL payload");
  }

  handle_twin_update(az_span_create_from_str((char*)payload), &twin_response);
}

static az_result build_sensor_telemetry_message(az_span* out_payload)
{
  az_json_writer jw;
  memset(&telemetry_payload, 0, sizeof(telemetry_payload));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_init(&jw, AZ_SPAN_FROM_BUFFER(telemetry_payload), NULL));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_begin_object(&jw));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_property_name(&jw, telemetry_name_temperature));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_int32(&jw, device_current_temp));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_property_name(&jw, telemetry_name_light));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_int32(&jw, device_current_light));
  RETURN_IF_AZ_RESULT_FAILED(az_json_writer_append_end_object(&jw));
  *out_payload = az_json_writer_get_bytes_used_in_destination(&jw);

  return AZ_OK;
}

static int send_telemetry_message(void)
{
  az_result result;
  twin_properties_t twin_properties;
  init_property_data(&twin_properties);

  if (az_result_failed(
		  result = az_iot_hub_client_telemetry_get_publish_topic(
			  &hub_client, NULL, telemetry_topic, sizeof(telemetry_topic), NULL)))
  {
	  return result;
  }

  update_sensor_data(&twin_properties);
  
  az_span telemetry_payload_span;
  if (az_result_failed(result = build_sensor_telemetry_message(&telemetry_payload_span)))
  {
    debug_printError("Could not build telemetry payload, az_result %d", result);
    return result;
  }

  // debug_printInfo("Sending Telemetry Message: temp=%d light=%d", 
  //		 (int)device_current_temp, (int)device_current_light);
  result = mqtt_publish_message(telemetry_topic, telemetry_payload_span, 0);

  if (twin_properties.flag.max_temp_updated == 1 && isGetReceived)
  {
    send_reported_property(&twin_properties);
  }

  return result;
}

// This will get called every 1 second only while we have a valid Cloud connection
void sendToCloud(void)
{
  az_result result;
  if (az_result_failed(result = send_telemetry_message()))
  {
    debug_printError("Could not send telemetry, az_result %d", result);
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
	// Initialize the device
	SYSTEM_Initialize();
	application_init();

	// Can execute this once to write a default ID Scope to the secure element
	// atcab_write_bytes_zone(ATCA_ZONE_DATA, ATCA_SLOT_IDSCOPE, 0, atca_id_scope, sizeof(atca_id_scope));

#ifdef CFG_MQTT_PROVISIONING_HOST
	pf_mqtt_iotprovisioning_client.MQTT_CLIENT_task_completed = iot_provisioning_completed;
	application_cloud_mqtt_connect(CFG_MQTT_PROVISIONING_HOST, &pf_mqtt_iotprovisioning_client, NULL);
#else
	application_cloud_mqtt_connect(hub_hostname, &pf_mtqt_iothub_client, sendToCloud);
#endif //CFG_MQTT_PROVISIONING_HOST 

	while (true) {
	  runScheduler();
	  /*** Add your APPlication code below this line ***/
	  if (BUTTON_SW0_wasPushed == BUTTON_ENUM_TRUE) {// Was SW0 button pressed?
      BUTTON_SW0_numPresses++; // Increment button presses counter for SW0
      DELAY_milliseconds(DEBOUNCE_DLY_MSEC); // Switch debounce delay
      BUTTON_SW0_wasPushed = BUTTON_ENUM_FALSE; // End while (SW0 released)
	  } // End (SW0 button was released)
	  if (BUTTON_SW1_wasPushed == BUTTON_ENUM_TRUE) {// Was SW1 button pressed?
      BUTTON_SW1_numPresses++; // Increment button presses counter for SW1
      DELAY_milliseconds(DEBOUNCE_DLY_MSEC); // Switch debounce delay
      BUTTON_SW1_wasPushed = BUTTON_ENUM_FALSE; // End while (SW1 released)
	  } // End (SW1 button was released)
	}

	return true;
}