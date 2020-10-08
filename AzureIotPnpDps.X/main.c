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
#include "azure/iot/az_iot_pnp_client.h"

#define DEFAULT_START_TEMP_CELSIUS 22

#define RETURN_ERR_WITH_MESSAGE_IF_FAILED(ret, msg)   \
  do {                                                \
    if(az_result_failed(ret)) {                       \
        debug_printError(msg);                        \
        return rc;                                    \
    }                                                 \
  } while (0)

#define RETURN_WITH_MESSAGE_IF_FAILED(ret, msg)     \
  do {                                              \
    if(az_result_failed(ret)) {                     \
        debug_printError(msg);                      \
        return;                                     \
    }                                               \
  } while (0)

extern az_iot_pnp_client pnp_client;
extern pf_MQTT_CLIENT pf_mqtt_iotprovisioning_client;
extern pf_MQTT_CLIENT pf_mqtt_pnp_client;

// * Plug and Play Values *
// The model id is the JSON document (also called the Digital Twins Model Identifier or DTMI)
// which defines the capability of your device. The functionality of the device should match what
// is described in the corresponding DTMI. Should you choose to program your own PnP capable device,
// the functionality would need to match the DTMI and you would need to update the below 'model_id'.
// Please see the sample README for more information on this DTMI.
const az_span device_model_id = AZ_SPAN_LITERAL_FROM_STR("dtmi:com:example:Thermostat;1");
// ISO8601 Time Format
static const char iso_spec_time_format[] = "%Y-%m-%dT%H:%M:%S.000%zZ";

// Plug and Play Connection Values
static int32_t request_id_int;
static char request_id_buf[8];

// Plug and Play Telemetry Values
char telemetry_topic[128];
static const az_span telemetry_name = AZ_SPAN_LITERAL_FROM_STR("temperature");
static char telemetry_payload[256];

// Plug and Play Commands Values
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

// Plug and Play Values
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

// Plug and Play Device Values
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
    az_iot_pnp_client_command_request *request,
    uint16_t status,
    az_span response)
{
  // Get the response topic to publish the command response
  int rc = az_iot_pnp_client_commands_response_get_publish_topic(
      &pnp_client, request->request_id, status, commands_response_topic,
      sizeof(commands_response_topic), NULL);
  RETURN_ERR_WITH_MESSAGE_IF_FAILED(rc, "Unable to get commands document publish topic");

  debug_printInfo("Command Status: %u", status);

  // Send the commands response
  if ((rc = mqtt_publish_message(commands_response_topic, response, 0)) == 0)
  {
    debug_printInfo("Sent command response");
  }

  return rc;
}

static az_result build_command_response_payload(
    az_json_writer* json_writer,
    az_span start_time_span,
    az_span end_time_span,
    az_span* response_payload)
{
  az_result rc;
  // Build the command response payload
  if (az_result_failed(rc = az_json_writer_append_begin_object(json_writer)) ||
      az_result_failed(rc = az_json_writer_append_property_name(json_writer, report_max_temp_name_span)) ||
      az_result_failed(
          rc = az_json_writer_append_int32(json_writer, device_max_temp)) ||
      az_result_failed(rc = az_json_writer_append_property_name(json_writer, report_min_temp_name_span)) ||
      az_result_failed(
          rc = az_json_writer_append_int32(json_writer, device_min_temp)) ||
      az_result_failed(rc = az_json_writer_append_property_name(json_writer, report_avg_temp_name_span)) ||
      az_result_failed(
          rc = az_json_writer_append_int32(json_writer, device_avg_temp)) ||
      az_result_failed(
          rc = az_json_writer_append_property_name(json_writer, report_start_time_name_span)) ||
      az_result_failed(rc = az_json_writer_append_string(json_writer, start_time_span)) ||
      az_result_failed(rc = az_json_writer_append_property_name(json_writer, report_end_time_name_span)) ||
      az_result_failed(rc = az_json_writer_append_string(json_writer, end_time_span)) ||
      az_result_failed(rc = az_json_writer_append_end_object(json_writer)))
  {
    debug_printError("Could not build command response");
    return rc;
  }

  *response_payload = az_json_writer_get_bytes_used_in_destination(json_writer);

  return AZ_OK;
}

static az_result invoke_getMaxMinReport(az_span payload, az_span response, az_span* out_response)
{
  if (az_span_size(payload) == 0)
  {
    return AZ_ERROR_ITEM_NOT_FOUND;
  }
  // Parse the "since" field in the payload.
  az_span start_time_span = AZ_SPAN_EMPTY;
  az_json_reader jp;
  az_result rc = az_json_reader_init(&jp, payload, NULL);
  RETURN_ERR_WITH_MESSAGE_IF_FAILED(rc, "Could not initialize json reader");

  rc = az_json_reader_next_token(&jp);
  RETURN_ERR_WITH_MESSAGE_IF_FAILED(rc, "Could not get next token");

  int32_t incoming_since_value_len;
  rc = az_json_token_get_string(&jp.token, incoming_since_value,
              sizeof(incoming_since_value), &incoming_since_value_len);
  RETURN_ERR_WITH_MESSAGE_IF_FAILED(rc, "Could not get string");

  start_time_span = az_span_create((uint8_t *)incoming_since_value, incoming_since_value_len);

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

  az_json_writer json_writer;
  rc = az_json_writer_init(&json_writer, response, NULL);
  RETURN_ERR_WITH_MESSAGE_IF_FAILED(rc, "Could not initialize the json writer");

  rc = build_command_response_payload(&json_writer, start_time_span, end_time_span, out_response);
  RETURN_ERR_WITH_MESSAGE_IF_FAILED(rc, "Could not build command response");

  return AZ_OK;
}

// Invoke the requested command if supported and return status | Return error otherwise
static void handle_command_message(
    az_span payload,
    az_iot_pnp_client_command_request* command_request)
{

  if (az_span_is_content_equal(report_command_name_span, command_request->command_name))
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
        az_span_size(command_request->command_name),
        az_span_ptr(command_request->command_name));

    int rc;
    if ((rc = send_command_response(command_request, 404, report_error_payload)) != 0)
    {
      debug_printError("Unable to send %d response, status %d", 404, rc);
    }
  }
}

void receivedFromCloud_commands(uint8_t* topic, uint8_t* payload)
{
  debug_printInfo("Commands");
  az_iot_pnp_client_command_request command_request;
  az_result rc = az_iot_pnp_client_commands_parse_received_topic(&pnp_client, az_span_create_from_str((char*)topic), &command_request);
  RETURN_WITH_MESSAGE_IF_FAILED(rc, "az_iot_pnp_client_commands_parse_received_topic failed");

  handle_command_message(az_span_create_from_str((char*)payload), &command_request);
}

static az_result build_reported_property(
    az_json_writer* json_writer,
    az_span property_name,
    int32_t property_val)
{
  az_result rc;
  if (az_result_failed(
          rc = az_json_writer_init(json_writer, AZ_SPAN_FROM_BUFFER(reported_property_payload), NULL)) ||
      az_result_failed(rc = az_json_writer_append_begin_object(json_writer)) ||
      az_result_failed(rc = az_json_writer_append_property_name(json_writer, property_name)) ||
      az_result_failed(
          rc = az_json_writer_append_int32(json_writer, property_val)) ||
      az_result_failed(rc = az_json_writer_append_end_object(json_writer)))
  {
    debug_printError("Could not build reported property");
    return rc;
  }

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
  rc = az_iot_pnp_client_property_patch_get_publish_topic(
      &pnp_client,
      request_id_span,
      reported_property_topic,
      sizeof(reported_property_topic),
      NULL);
  RETURN_ERR_WITH_MESSAGE_IF_FAILED(rc, "Unable to get property document publish topic");

  // Reported properties must be in JSON format. The payload is constructed here.
  az_json_writer json_writer;
  rc = az_json_writer_init(&json_writer, AZ_SPAN_FROM_BUFFER(reported_property_payload), NULL);
  RETURN_ERR_WITH_MESSAGE_IF_FAILED(rc, "Could not initialize json writer");

  rc = az_json_writer_append_begin_object(&json_writer);
  RETURN_ERR_WITH_MESSAGE_IF_FAILED(rc, "Could not append begin object");

  if (is_max_reported_prop)
  {
    if (az_result_failed(
            rc
            = build_reported_property(&json_writer, max_temp_reported_property_name, temp_value)))
    {
      return rc;
    }
  }
  else
  {
    if (az_result_failed(rc = az_iot_pnp_client_property_builder_begin_reported_status(&pnp_client, &json_writer, desired_temp_property_name, 200, version, AZ_SPAN_FROM_STR("success"))) ||
        az_result_failed(rc = az_json_writer_append_int32(&json_writer, temp_value)) ||
        az_result_failed(rc = az_iot_pnp_client_property_builder_end_reported_status(&pnp_client, &json_writer)))
    {
      debug_printError("Could not create the reported property with status");
      return rc;
    }
  }

  rc = az_json_writer_append_end_object(&json_writer);
  RETURN_ERR_WITH_MESSAGE_IF_FAILED(rc, "Could not append end object");

  az_span json_payload = az_json_writer_get_bytes_used_in_destination(&json_writer);

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

static az_result parse_desired_temperature_property(
  az_span payload, az_iot_pnp_client_property_response_type response_type,
  int32_t* desired_temp, int32_t* version_num)
{
  az_json_reader jr;
  az_result rc = az_json_reader_init(&jr, payload, NULL);
  RETURN_ERR_WITH_MESSAGE_IF_FAILED(rc, "Could not get initialize the json reader");

  rc = az_iot_pnp_client_property_get_property_version(
      &pnp_client, jr, response_type, version_num);
  RETURN_ERR_WITH_MESSAGE_IF_FAILED(rc, "Could not get the property version");

  az_span component_name;
  az_json_reader property_name_and_value;
  while (az_result_succeeded(az_iot_pnp_client_property_get_next_component_property(
      &pnp_client, &jr, response_type, &component_name, &property_name_and_value)))
  {
    if (az_json_token_is_text_equal(&property_name_and_value.token, desired_temp_property_name))
    {
      rc = az_result_failed(az_json_reader_next_token(&property_name_and_value));
      RETURN_ERR_WITH_MESSAGE_IF_FAILED(rc, "Could not advance to the property value");

      rc = az_result_failed(az_json_token_get_int32(&property_name_and_value.token, desired_temp));
      RETURN_ERR_WITH_MESSAGE_IF_FAILED(rc, "Could not get the property value");

      rc = send_reported_temperature_property(*desired_temp, *version_num, false);
      RETURN_ERR_WITH_MESSAGE_IF_FAILED(rc, "Could not send the reported property");
    }
  }

  return rc;
}

// Switch on the type of property message and handle accordingly | On desired prop, respond with max
// temp reported prop.
static void handle_property_message(
    az_span payload,
    az_iot_pnp_client_property_response* property_response)
{
  az_result rc;

  int32_t desired_temp;
  int32_t version_num;
  // Determine what type of incoming property message this is. Print relevant data for the message.
  switch (property_response->response_type)
  {
    // A response from a property GET publish message with the property document as a payload.
    case AZ_IOT_PNP_CLIENT_PROPERTY_RESPONSE_TYPE_GET:
      debug_printInfo("A property GET response was received");

      rc = parse_desired_temperature_property(payload, property_response->response_type, &desired_temp, &version_num);
      RETURN_WITH_MESSAGE_IF_FAILED(rc, "Could not parse desired temperature property");

      break;
    // An update to the desired properties with the properties as a JSON payload.
    case AZ_IOT_PNP_CLIENT_PROPERTY_RESPONSE_TYPE_DESIRED_PROPERTIES:
      debug_printInfo("A property desired properties message was received");

      rc = parse_desired_temperature_property(payload, property_response->response_type, &desired_temp, &version_num);
      RETURN_WITH_MESSAGE_IF_FAILED(rc, "Could not parse desired temperature property");

      break;

    // A response from a reported properties publish message. With a successful update of
    // the reported properties, the payload will be empty and the status will be 204.
    case AZ_IOT_PNP_CLIENT_PROPERTY_RESPONSE_TYPE_REPORTED_PROPERTIES:
      debug_printInfo("A property reported properties response message was received");
      break;
  }
}

void receivedFromCloud_property(uint8_t* topic, uint8_t* payload)
{
  debug_printInfo("Device Property Get");
  if (topic == NULL)
  {
    debug_printError("NULL topic");
    return;
  }

  az_iot_pnp_client_property_response property_response;
  az_span property_topic = az_span_create_from_str((char*)topic);
  az_result rc = az_iot_pnp_client_property_parse_received_topic(&pnp_client, property_topic, &property_response);
  RETURN_WITH_MESSAGE_IF_FAILED(rc, "az_iot_pnp_client_property_parse_received_topic failed");

  if (az_span_size(property_response.request_id) != 0 && IOT_DEBUG_PRINT)
  {
    char request_id_buf[50];
    az_span_to_str(request_id_buf, sizeof(request_id_buf), property_response.request_id);
    debug_printInfo("Property request, request_id:%s, status: %d", request_id_buf, property_response.status);
  }

  if (payload == NULL)
  {
    debug_printError("NULL payload");
    return; // no payload, nothing to process
  }

  handle_property_message(az_span_create_from_str((char*)payload), &property_response);
}

void receivedFromCloud_patch(uint8_t* topic, uint8_t* payload)
{
  debug_printInfo("Device Property Patch");
  if (topic == NULL)
  {
    debug_printError("NULL topic");
    return;
  }

  az_iot_pnp_client_property_response property_response;
  az_span property_topic = az_span_create_from_str((char *)topic);
  az_result rc = az_iot_pnp_client_property_parse_received_topic(&pnp_client, property_topic, &property_response);
  RETURN_WITH_MESSAGE_IF_FAILED(rc, "az_iot_pnp_client_property_parse_received_topic failed");

  if (payload)
  {
    debug_printGOOD("Payload: %s", payload);
  }
  else
  {
    debug_printError("NULL payload");
  }

  handle_property_message(az_span_create_from_str((char *)payload), &property_response);
}

static az_result build_telemetry_message(az_span* out_payload)
{
  az_result rc;
  az_json_writer json_writer;
  if (az_result_failed(rc =
                           az_json_writer_init(&json_writer, AZ_SPAN_FROM_BUFFER(telemetry_payload), NULL)) ||
      az_result_failed(rc = az_json_writer_append_begin_object(&json_writer)) ||
      az_result_failed(rc = az_json_writer_append_property_name(&json_writer, telemetry_name)) ||
      az_result_failed(rc = az_json_writer_append_int32(
                           &json_writer, current_device_temp)) ||
      az_result_failed(rc = az_json_writer_append_end_object(&json_writer)))
  {
    return rc;
  }
  *out_payload = az_json_writer_get_bytes_used_in_destination(&json_writer);

  return AZ_OK;
}

static int send_telemetry_message(void)
{
  int rc = az_iot_pnp_client_telemetry_get_publish_topic(
              &pnp_client, AZ_SPAN_EMPTY, NULL, telemetry_topic, sizeof(telemetry_topic), NULL);
  RETURN_ERR_WITH_MESSAGE_IF_FAILED(rc, "Could not get the telemetry topic");

  update_device_temp();

  az_span telemetry_payload_span;
  rc = build_telemetry_message(&telemetry_payload_span);
  RETURN_ERR_WITH_MESSAGE_IF_FAILED(rc, "Could not build telemetry message");

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
    application_cloud_mqtt_connect(hub_hostname, &pf_mqtt_pnp_client, sendToCloud);
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

