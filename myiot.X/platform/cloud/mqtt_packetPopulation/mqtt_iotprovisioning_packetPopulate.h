// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT


#ifndef MQTT_IOTPROVISIONING_PACKET_POPULATE_H
#define MQTT_IOTPROVISIONING_PACKET_POPULATE_H

#include <stdbool.h>
#include <stdint.h>
#include "../../config/IoT_Sensor_Node_config.h"

void MQTT_CLIENT_iotprovisioning_publish(uint8_t *data, uint16_t len);
void MQTT_CLIENT_iotprovisioning_receive(uint8_t *data, uint16_t len);
void MQTT_CLIENT_iotprovisioning_connect(char* deviceID);
bool MQTT_CLIENT_iotprovisioning_subscribe();
void MQTT_CLIENT_iotprovisioning_connected();

static const char* const provisioning_enrollment_key = PROVISIONING_ENROLLMENT_KEY;

#endif /* MQTT_IOTPROVISIONING_PACKET_POPULATE_H */
