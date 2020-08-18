/*
 * cloud_service.h
 *
 * Created: 9/27/2018 2:25:12 PM
 *  Author: C14674
 */

#ifndef CLOUD_SERVICE_H_
#define CLOUD_SERVICE_H_

#include <stdint.h>
#include <stdbool.h>
#include "../../mqtt_packetPopulation/mqtt_packetPopulate.h"

#define CLOUD_PACKET_RECV_TABLE_SIZE	2
#define CLOUD_MAX_DEVICEID_LENGTH 30
#define PASSWORD_SPACE 456

void CLOUD_reset(void);
void CLOUD_init(char* deviceId);
void CLOUD_init_host(char* host, char* deviceId, pf_MQTT_CLIENT* pf_table);
void CLOUD_subscribe(void);
void CLOUD_disconnect(void);
void CLOUD_publishData(uint8_t* data, unsigned int len);
bool CLOUD_isConnected(void);
#endif /* CLOUD_SERVICE_H_ */