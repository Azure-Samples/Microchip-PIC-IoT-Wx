// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

#ifndef MQTT_PACKET_POPULATE_H
#define MQTT_PACKET_POPULATE_H

#include <stdbool.h>
#include <stdint.h>
#include "../platform/config/IoT_Sensor_Node_config.h"

extern char cid[];
extern char mqtt_topic_buf[];
extern char mqtt_password_buf[512];
extern char mqtt_username_buf[200];
static const uint8_t az_iot_b64rules[4] = { '+', '/', '=', 0 };

typedef struct
{
  void (*MQTT_CLIENT_publish)(uint8_t *data, uint16_t len);
  void (*MQTT_CLIENT_receive)(uint8_t *data, uint16_t len);
  void (*MQTT_CLIENT_connect)(char* deviceID);
  bool (*MQTT_CLIENT_subscribe)();
  void (*MQTT_CLIENT_connected)();  
  void (*MQTT_CLIENT_task_completed)();  
}  pf_MQTT_CLIENT;

char* url_encode_rfc3986(char* s, char* dest, size_t dest_len);

extern char* hub_device_key;
extern char* hub_hostname;

#endif /* MQTT_PACKET_POPULATE_H */
