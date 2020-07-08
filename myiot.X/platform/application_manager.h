/*
 * application_manager.h
 *
 * Created: 10/4/2018 1:36:20 PM
 *  Author: I17643
 */


#ifndef APPLICATION_MANAGER_H_
#define APPLICATION_MANAGER_H_

#include <stdint.h>
#include "cloud/mqtt_packetPopulation/mqtt_packetPopulate.h"

typedef union
{
	uint8_t allBits;
	struct
	{
		unsigned amDisconnecting : 1;
		unsigned haveAPConnection : 1;
		unsigned haveERROR : 1;
		unsigned : 5;
	};
} shared_networking_params_t;
extern shared_networking_params_t shared_networking_params;

void application_init(void);
void application_cloud_mqtt_connect(char* host, pf_MQTT_CLIENT* pf_table, void (*fpSendToCloud)());
void application_post_provisioning(void);
void runScheduler(void);

#endif /* APPLICATION_MANAGER_H_ */