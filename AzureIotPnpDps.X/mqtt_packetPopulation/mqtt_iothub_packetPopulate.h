// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT


#ifndef MQTT_IOTHUB_PACKET_POPULATE_H
#define MQTT_IOTHUB_PACKET_POPULATE_H

#include <stdbool.h>
#include <stdint.h>
#include "../platform/config/IoT_Sensor_Node_config.h"

void MQTT_CLIENT_pnp_publish(uint8_t *data, uint16_t len);
void MQTT_CLIENT_pnp_receive(uint8_t *data, uint16_t len);
void MQTT_CLIENT_pnp_connect(char* deviceID);
bool MQTT_CLIENT_pnp_subscribe();
void MQTT_CLIENT_pnp_connected();


#endif /* MQTT_IOTHUB_PACKET_POPULATE_H */
