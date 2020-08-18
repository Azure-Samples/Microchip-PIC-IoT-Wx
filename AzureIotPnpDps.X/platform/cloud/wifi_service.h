/*
 * wifi_service.h
 *
 * Created: 10/4/2018 2:22:04 PM
 *  Author: I17643
 */

#ifndef WIFI_SERVICE_H_
#define WIFI_SERVICE_H_

#include <stdint.h>
#include <stdbool.h>

#define MAX_WIFI_CRED_LENGTH 31
#define DEFAULT_CREDENTIALS 0
#define NEW_CREDENTIALS     1
#define WIFI_SOFT_AP  0
#define WIFI_DEFAULT  1

struct wifi_params {
	char wifi_ssid[MAX_WIFI_CRED_LENGTH];
	char wifi_password[MAX_WIFI_CRED_LENGTH];
	char wifi_authType[2];
};
extern struct wifi_params wifi_params;

// If you pass a callback function in here it will be called when the AP state changes. Pass NULL if you do not want that.
void wifi_init(void (*funcPtr)(uint8_t), uint8_t  mode);
void wifi_reinit();
bool wifi_connectToAp(uint8_t passed_wifi_creds);
bool wifi_disconnectFromAp(void);
bool wifi_getIpAddressByHostName(uint8_t* host_name);
#endif /* WIFI_SERVICE_H_ */

