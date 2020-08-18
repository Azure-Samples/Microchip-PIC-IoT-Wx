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

#define DEFAULT_START_TEMP_CELSIUS 22

extern az_iot_hub_client hub_client;
extern pf_MQTT_CLIENT pf_mqtt_iotprovisioning_client;
extern pf_MQTT_CLIENT pf_mqtt_iothub_client;

// * PnP Values *
// The model id is the JSON document (also called the Digital Twins Model Identifier or DTMI)
// which defines the capability of your device. The functionality of the device should match what
// is described in the corresponding DTMI. Should you choose to program your own PnP capable device,
// the functionality would need to match the DTMI and you would need to update the below 'model_id'.
// Please see the sample README for more information on this DTMI.
const az_span device_model_id = AZ_SPAN_LITERAL_FROM_STR("dtmi:com:example:Thermostat;1");
// ISO8601 Time Format
static const char iso_spec_time_format[] = "%Y-%m-%dT%H:%M:%S%z";

// IoT Hub Connection Values
static int32_t request_id_int;
static char request_id_buf[8];

// IoT Hub Telemetry Values
char telemetry_topic[128];
static const az_span telemetry_name = AZ_SPAN_LITERAL_FROM_STR("temperature");
static char telemetry_payload[32];

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

// IoT Hub Twin Values
//static char twin_get_topic[128];
static char reported_property_topic[128];
static const az_span desired_property_name = AZ_SPAN_LITERAL_FROM_STR("desired");
static const az_span desired_property_version_name = AZ_SPAN_LITERAL_FROM_STR("$version");
static const az_span desired_temp_property_name = AZ_SPAN_LITERAL_FROM_STR("targetTemperature");
static const az_span desired_temp_response_value_name = AZ_SPAN_LITERAL_FROM_STR("value");
static const az_span desired_temp_ack_code_name = AZ_SPAN_LITERAL_FROM_STR("ac");
static const az_span desired_temp_ack_version_name = AZ_SPAN_LITERAL_FROM_STR("av");
static const az_span desired_temp_ack_description_name = AZ_SPAN_LITERAL_FROM_STR("ad");
static const az_span max_temp_reported_property_name
    = AZ_SPAN_LITERAL_FROM_STR("maxTempSinceLastReboot");
static char reported_property_payload[128];

// PnP Device Values
static bool max_temp_changed = false;
static int32_t current_device_temp;
static int32_t device_temperature_avg_total;
static uint32_t device_temperature_avg_count = 0;
static int32_t device_max_temp;
static int32_t device_min_temp;
static int32_t device_avg_temp;

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
  cloudPublishPacket.publishHeaderFlags.duplicate = AZ_HUB_CLIENT_DEFAULT_MQTT_TELEMETRY_DUPLICATE;
  cloudPublishPacket.publishHeaderFlags.qos = qos;
  cloudPublishPacket.publishHeaderFlags.retain = AZ_HUB_CLIENT_DEFAULT_MQTT_TELEMETRY_RETAIN;
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
  int rc;
  // Get the response topic to publish the command response
  if (az_failed(
          rc = az_iot_hub_client_methods_response_get_publish_topic(
              &hub_client,
              request->request_id,
              status,
              commands_response_topic,
              sizeof(commands_response_topic),
              NULL)))
  {
    debug_printError("Unable to get twin document publish topic");
    return rc;
  }

  debug_printInfo("Status: %u\tPayload:", status);
  char* payload_char = (char*)az_span_ptr(response);
  if (payload_char != NULL)
  {
    for (int32_t i = 0; i < az_span_size(response); i++)
    {
      putchar(*(payload_char + i));
    }
  }

  // Send the commands response
  if ((rc = mqtt_publish_message(commands_response_topic, response, 0)) == 0)
  {
    debug_printInfo("Sent response");
  }

  return rc;
}
static az_result build_command_response_payload(
    az_json_writer* json_builder,
    az_span start_time_span,
    az_span end_time_span,
    az_span* response_payload)
{
  // Build the command response payload
  AZ_RETURN_IF_FAILED(az_json_writer_append_begin_object(json_builder));
  AZ_RETURN_IF_FAILED(az_json_writer_append_property_name(json_builder, report_max_temp_name_span));
  AZ_RETURN_IF_FAILED(
      az_json_writer_append_int32(json_builder, device_max_temp));
  AZ_RETURN_IF_FAILED(az_json_writer_append_property_name(json_builder, report_min_temp_name_span));
  AZ_RETURN_IF_FAILED(
      az_json_writer_append_int32(json_builder, device_min_temp));
  AZ_RETURN_IF_FAILED(az_json_writer_append_property_name(json_builder, report_avg_temp_name_span));
  AZ_RETURN_IF_FAILED(
      az_json_writer_append_int32(json_builder, device_avg_temp));
  AZ_RETURN_IF_FAILED(
      az_json_writer_append_property_name(json_builder, report_start_time_name_span));
  AZ_RETURN_IF_FAILED(az_json_writer_append_string(json_builder, start_time_span));
  AZ_RETURN_IF_FAILED(az_json_writer_append_property_name(json_builder, report_end_time_name_span));
  AZ_RETURN_IF_FAILED(az_json_writer_append_string(json_builder, end_time_span));
  AZ_RETURN_IF_FAILED(az_json_writer_append_end_object(json_builder));

  *response_payload = az_json_writer_get_bytes_used_in_destination(json_builder);

  return AZ_OK;
}


static az_result invoke_getMaxMinReport(az_span payload, az_span response, az_span* out_response)
{
  // Parse the "since" field in the payload.
  az_span start_time_span = AZ_SPAN_NULL;
  az_json_reader jp;
  AZ_RETURN_IF_FAILED(az_json_reader_init(&jp, payload, NULL));
  AZ_RETURN_IF_FAILED(az_json_reader_next_token(&jp));
  int32_t incoming_since_value_len;
  AZ_RETURN_IF_FAILED(az_json_token_get_string(
      &jp.token, incoming_since_value, sizeof(incoming_since_value), &incoming_since_value_len));
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
  az_span end_time_span = az_span_create((uint8_t*)end_time_buffer, (int32_t)len);

  az_json_writer json_builder;
  AZ_RETURN_IF_FAILED(az_json_writer_init(&json_builder, response, NULL));
  AZ_RETURN_IF_FAILED(
      build_command_response_payload(&json_builder, start_time_span, end_time_span, out_response));

  return AZ_OK;
}

// Invoke the requested command if supported and return status | Return error otherwise
static void handle_command_message(
    az_span payload,
    az_iot_hub_client_method_request* command_request)
{

  if (az_span_is_content_equal(report_command_name_span, command_request->name))
  {
    az_span command_response_span = AZ_SPAN_FROM_BUFFER(commands_response_payload);

    // Invoke command
    uint16_t return_code;
    az_result response = invoke_getMaxMinReport(
        payload, command_response_span, &command_response_span);
    if (response != AZ_OK)
    {
      return_code = 400;
    }
    else
    {
      return_code = 200;
    }

    // Send command response with report as JSON payload
    int rc;
    if ((rc = send_command_response(command_request, return_code, command_response_span)) != 0)
    {
      debug_printError("Unable to send %u response, status %d", return_code, rc);
    }
  }
  else
  {
    // Unsupported command
    debug_printError(
        "Unsupported command received: %.*s.",
        az_span_size(command_request->name),
        az_span_ptr(command_request->name));

    int rc;
    if ((rc = send_command_response(command_request, 404, report_error_payload)) != 0)
    {
      debug_printError("Unable to send %d response, status %d", 404, rc);
    }
  }
}

void receivedFromCloud_methods(uint8_t* topic, uint8_t* payload)
{
  debug_printInfo("Methods");
	az_iot_hub_client_method_request method_request;
	az_result result = az_iot_hub_client_methods_parse_received_topic(&hub_client, az_span_create_from_str((char*)topic), &method_request);
	if (az_failed(result))
	{
		debug_printError("az_iot_hub_client_methods_parse_received_topic failed");
		return;
	}

  handle_command_message(az_span_create_from_str((char*)payload), &method_request);
}

// Parse the desired temperature property from the incoming JSON
static az_result parse_twin_desired_temperature_property(
    az_span twin_payload_span,
    bool is_twin_get,
    int32_t* parsed_value,
    int32_t* version_number)
{
  az_json_reader jp;
  bool desired_found = false;
  AZ_RETURN_IF_FAILED(az_json_reader_init(&jp, twin_payload_span, NULL));
  AZ_RETURN_IF_FAILED(az_json_reader_next_token(&jp));
  if (jp.token.kind != AZ_JSON_TOKEN_BEGIN_OBJECT)
  {
    return AZ_ERROR_UNEXPECTED_CHAR;
  }

  if (is_twin_get)
  {
    // If is twin get payload, we have to parse one level deeper for "desired" wrapper
    AZ_RETURN_IF_FAILED(az_json_reader_next_token(&jp));
    while (jp.token.kind != AZ_JSON_TOKEN_END_OBJECT)
    {
      if (az_json_token_is_text_equal(&jp.token, desired_property_name))
      {
        desired_found = true;
        AZ_RETURN_IF_FAILED(az_json_reader_next_token(&jp));
        break;
      }
      else
      {
        // else ignore token.
        AZ_RETURN_IF_FAILED(az_json_reader_skip_children(&jp));
      }

      AZ_RETURN_IF_FAILED(az_json_reader_next_token(&jp));
    }
  }
  else
  {
    desired_found = true;
  }

  if (!desired_found)
  {
    debug_printError("Desired property object not found in twin");
    return AZ_ERROR_ITEM_NOT_FOUND;
  }

  if (jp.token.kind != AZ_JSON_TOKEN_BEGIN_OBJECT)
  {
    return AZ_ERROR_UNEXPECTED_CHAR;
  }
  AZ_RETURN_IF_FAILED(az_json_reader_next_token(&jp));

  bool temp_found = false;
  bool version_found = false;
  while (!(temp_found && version_found) || (jp.token.kind != AZ_JSON_TOKEN_END_OBJECT))
  {
    if (az_json_token_is_text_equal(&jp.token, desired_temp_property_name))
    {
      AZ_RETURN_IF_FAILED(az_json_reader_next_token(&jp));
      AZ_RETURN_IF_FAILED(az_json_token_get_int32(&jp.token, parsed_value));
      temp_found = true;
    }
    else if (az_json_token_is_text_equal(&jp.token, desired_property_version_name))
    {
      AZ_RETURN_IF_FAILED(az_json_reader_next_token(&jp));
      AZ_RETURN_IF_FAILED(az_json_token_get_int32(&jp.token, version_number));
      version_found = true;
    }
    else
    {
      // else ignore token.
      AZ_RETURN_IF_FAILED(az_json_reader_skip_children(&jp));
    }
    AZ_RETURN_IF_FAILED(az_json_reader_next_token(&jp));
  }

  if (temp_found && version_found)
  {
    debug_printInfo("Desired temperature: %d\tVersion number: %d", (int)*parsed_value, (int)*version_number);
    return AZ_OK;
  }

  return AZ_ERROR_ITEM_NOT_FOUND;
}

// Build the JSON payload for the reported property
static az_result build_confirmed_reported_property(
    az_json_writer* json_builder,
    az_span property_name,
    int32_t property_val,
    int32_t ack_code_value,
    int32_t ack_version_value,
    az_span ack_description_value)
{
  AZ_RETURN_IF_FAILED(
      az_json_writer_init(json_builder, AZ_SPAN_FROM_BUFFER(reported_property_payload), NULL));
  AZ_RETURN_IF_FAILED(az_json_writer_append_begin_object(json_builder));
  AZ_RETURN_IF_FAILED(az_json_writer_append_property_name(json_builder, property_name));
  AZ_RETURN_IF_FAILED(az_json_writer_append_begin_object(json_builder));
  AZ_RETURN_IF_FAILED(
      az_json_writer_append_property_name(json_builder, desired_temp_response_value_name));
  AZ_RETURN_IF_FAILED(
      az_json_writer_append_int32(json_builder, property_val));
  AZ_RETURN_IF_FAILED(
      az_json_writer_append_property_name(json_builder, desired_temp_ack_code_name));
  AZ_RETURN_IF_FAILED(az_json_writer_append_int32(json_builder, ack_code_value));
  AZ_RETURN_IF_FAILED(
      az_json_writer_append_property_name(json_builder, desired_temp_ack_version_name));
  AZ_RETURN_IF_FAILED(az_json_writer_append_int32(json_builder, ack_version_value));
  AZ_RETURN_IF_FAILED(
      az_json_writer_append_property_name(json_builder, desired_temp_ack_description_name));
  AZ_RETURN_IF_FAILED(az_json_writer_append_string(json_builder, ack_description_value));
  AZ_RETURN_IF_FAILED(az_json_writer_append_end_object(json_builder));
  AZ_RETURN_IF_FAILED(az_json_writer_append_end_object(json_builder));

  return AZ_OK;
}

static az_result build_reported_property(
    az_json_writer* json_builder,
    az_span property_name,
    int32_t property_val)
{
  AZ_RETURN_IF_FAILED(
      az_json_writer_init(json_builder, AZ_SPAN_FROM_BUFFER(reported_property_payload), NULL));
  AZ_RETURN_IF_FAILED(az_json_writer_append_begin_object(json_builder));
  AZ_RETURN_IF_FAILED(az_json_writer_append_property_name(json_builder, property_name));
  AZ_RETURN_IF_FAILED(
      az_json_writer_append_int32(json_builder, property_val));
  AZ_RETURN_IF_FAILED(az_json_writer_append_end_object(json_builder));

  return AZ_OK;
}

static int send_reported_temperature_property(
    int32_t temp_value,
    int32_t version,
    bool is_max_reported_prop)
{
  int rc;
  debug_printInfo("Sending reported property");

  // Get the topic used to send a reported property update
  az_span request_id_span = get_request_id();
  if (az_failed(
          rc = az_iot_hub_client_twin_patch_get_publish_topic(
              &hub_client,
              request_id_span,
              reported_property_topic,
              sizeof(reported_property_topic),
              NULL)))
  {
    debug_printError("Unable to get twin document publish topic, return code %d", rc);
    return rc;
  }

  // Twin reported properties must be in JSON format. The payload is constructed here.
  az_json_writer json_builder;
  if (is_max_reported_prop)
  {
    if (az_failed(
            rc
            = build_reported_property(&json_builder, max_temp_reported_property_name, temp_value)))
    {
      return rc;
    }
  }
  else
  {
    if (az_failed(
            rc = build_confirmed_reported_property(
                &json_builder,
                desired_temp_property_name,
                temp_value,
                200,
                version,
                AZ_SPAN_FROM_STR("success"))))
    {
      return rc;
    }
  }
  az_span json_payload = az_json_writer_get_bytes_used_in_destination(&json_builder);

  // Publish the reported property payload to IoT Hub
  rc = mqtt_publish_message(reported_property_topic, json_payload, 0);

  max_temp_changed = false;

  return rc;
}

static void update_device_temp(void)
{
  int16_t temp = SENSORS_getTempValue();
  current_device_temp = (int)(temp / 100);

  bool ret = false;
  if (current_device_temp > device_max_temp)
  {
    device_max_temp = current_device_temp;
    ret = true;
  }
  if (current_device_temp < device_min_temp)
  {
    device_min_temp = current_device_temp;
  }

  // Increment the avg count, add the new temp to the total, and calculate the new avg
  device_temperature_avg_count++;
  device_temperature_avg_total += current_device_temp;
  device_avg_temp = device_temperature_avg_total / device_temperature_avg_count;

  max_temp_changed = ret;
}

// Switch on the type of twin message and handle accordingly | On desired prop, respond with max
// temp reported prop.
static void handle_twin_message(
    az_span payload,
    az_iot_hub_client_twin_response* twin_response)
{
  az_result result;

  int32_t desired_temp;
  int32_t version_num;
  // Determine what type of incoming twin message this is. Print relevant data for the message.
  switch (twin_response->response_type)
  {
    // A response from a twin GET publish message with the twin document as a payload.
    case AZ_IOT_CLIENT_TWIN_RESPONSE_TYPE_GET:
      debug_printInfo("A twin GET response was received");
      if (az_failed(
              result = parse_twin_desired_temperature_property(
                  payload, true, &desired_temp, &version_num)))
      {
        // If the item can't be found, the desired temp might not be set so take no action
        break;
      }
      else
      {
        send_reported_temperature_property(desired_temp, version_num, false);
      }
      break;
    // An update to the desired properties with the properties as a JSON payload.
    case AZ_IOT_CLIENT_TWIN_RESPONSE_TYPE_DESIRED_PROPERTIES:
      debug_printInfo("A twin desired properties message was received");

      // Get the new temperature
      if (az_failed(
              result = parse_twin_desired_temperature_property(
                  payload, false, &desired_temp, &version_num)))
      {
        debug_printError("Could not parse desired temperature property, az_result %04x", result);
        break;
      }
      send_reported_temperature_property(desired_temp, version_num, false);

      break;

    // A response from a twin reported properties publish message. With a successfull update of
    // the reported properties, the payload will be empty and the status will be 204.
    case AZ_IOT_CLIENT_TWIN_RESPONSE_TYPE_REPORTED_PROPERTIES:
      debug_printInfo("A twin reported properties response message was received");
      break;
  }
}

void receivedFromCloud_twin(uint8_t* topic, uint8_t* payload)
{
  debug_printInfo("Twin");
	if (topic == NULL)
	{
		return;
	}

	az_iot_hub_client_twin_response twin_response;
	az_span twin_topic = az_span_create_from_str((char*)topic);
	az_result result = az_iot_hub_client_twin_parse_received_topic(&hub_client, twin_topic, &twin_response);
	if (az_failed(result))
	{
		debug_printError("az_iot_hub_client_twin_parse_received_topic failed");
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
		return; // no payload, nothing to process
	}

  handle_twin_message(az_span_create_from_str((char*)payload), &twin_response);
}

void receivedFromCloud_patch(uint8_t* topic, uint8_t* payload)
{
  debug_printInfo("Patch");
	if (topic == NULL)
	{
		return;
	}

	az_iot_hub_client_twin_response twin_response;
	az_span twin_topic = az_span_create_from_str((char*)topic);
	az_result result = az_iot_hub_client_twin_parse_received_topic(&hub_client, twin_topic, &twin_response);
	if (az_failed(result))
	{
		debug_printError("az_iot_hub_client_twin_parse_received_topic failed");
		return;
	}

  if(payload)
  {
    debug_printGOOD("Payload: %s", payload);
  }

  handle_twin_message(az_span_create_from_str((char*)payload), &twin_response);

}

static az_result build_telemetry_message(az_span* out_payload)
{
  az_json_writer json_builder;
  AZ_RETURN_IF_FAILED(
      az_json_writer_init(&json_builder, AZ_SPAN_FROM_BUFFER(telemetry_payload), NULL));
  AZ_RETURN_IF_FAILED(az_json_writer_append_begin_object(&json_builder));
  AZ_RETURN_IF_FAILED(az_json_writer_append_property_name(&json_builder, telemetry_name));
  AZ_RETURN_IF_FAILED(az_json_writer_append_int32(
      &json_builder, current_device_temp));
  AZ_RETURN_IF_FAILED(az_json_writer_append_end_object(&json_builder));
  *out_payload = az_json_writer_get_bytes_used_in_destination(&json_builder);

  return AZ_OK;
}

static int send_telemetry_message(void)
{
  int rc;

  if (az_failed(
          rc = az_iot_hub_client_telemetry_get_publish_topic(
              &hub_client, NULL, telemetry_topic, sizeof(telemetry_topic), NULL)))
  {
    return rc;
  }

  update_device_temp();

  az_span telemetry_payload_span;
  if (az_failed(rc = build_telemetry_message(&telemetry_payload_span)))
  {
    debug_printError("Could not build telemetry payload, az_result %d", rc);
    return rc;
  }

  debug_printInfo("Sending Telemetry Message: temp %d", (int)current_device_temp);
  rc = mqtt_publish_message(telemetry_topic, telemetry_payload_span, 0);

  return rc;
}

// This will get called every 1 second only while we have a valid Cloud connection
void sendToCloud(void)
{
  send_telemetry_message();
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

