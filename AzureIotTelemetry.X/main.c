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
#include "azure/iot/az_iot_hub_client.h"

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

extern az_iot_hub_client hub_client;
extern pf_MQTT_CLIENT pf_mqtt_pnp_client;

// Telemetry Values
char telemetry_topic[128];
static const az_span telemetry_name = AZ_SPAN_LITERAL_FROM_STR("temperature");
static char telemetry_payload[16];

// Device Values
static int32_t current_device_temp;

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

static void update_device_temp(void)
{
  int16_t temp = SENSORS_getTempValue();
  current_device_temp = (int)(temp / 100);
}

static int send_telemetry_message(void)
{
  int rc = az_iot_hub_client_telemetry_get_publish_topic(
      &hub_client, NULL, telemetry_topic, sizeof(telemetry_topic), NULL);
  RETURN_ERR_WITH_MESSAGE_IF_FAILED(rc, "Could not get the telemetry topic");

  update_device_temp();

  az_span telemetry_payload_span = az_span_create((uint8_t*)telemetry_payload, (int32_t)sizeof(telemetry_payload));
  rc = az_span_i32toa(telemetry_payload_span, current_device_temp, &telemetry_payload_span);

  debug_printInfo("Sending Telemetry Message: temp %d", (int)current_device_temp);
  rc = mqtt_publish_message(telemetry_topic, telemetry_payload_span, 0);

  return rc;
}

// This will get called every 1 second only while we have a valid Cloud connection
void sendToCloud(void)
{
  send_telemetry_message();
}


/*
    Main application
 */

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    application_init();

    application_cloud_mqtt_connect(hub_hostname, &pf_mqtt_pnp_client, sendToCloud);

    while (true)
    {
      runScheduler();
    }

    return true;
}

