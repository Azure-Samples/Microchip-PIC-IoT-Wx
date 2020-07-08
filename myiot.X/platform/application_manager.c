/*
 * application_manager.c
 *
 * Created: 10/4/2018 1:37:19 PM
 *  Author: I17643
 */

#include <string.h>
#include <time.h>
#include <stdio.h>
#include "application_manager.h"
#include "pin_manager.h"
#include "mcc.h"
#include "config/IoT_Sensor_Node_config.h"
#include "config/conf_winc.h"
#include "cloud/cloud_service.h"
#include "cloud/crypto_client/cryptoauthlib_main.h"
#include "cloud/crypto_client/crypto_client.h"
#include "cloud/wifi_service.h"
#if CFG_ENABLE_CLI
#include "cli/cli.h"
#endif
#include "credentials_storage/credentials_storage.h"
#include "winc/driver/include/m2m_types.h"
#include "led.h"
#include "debug_print.h"
#include "drivers/timeout.h"

#define MAIN_DATATASK_INTERVAL timeout_mSecToTicks(100L)
#define SW0_Value   (PORTAbits.RA7 /* get SW0 value */)
#define SW1_Value   (PORTAbits.RA10 /* get SW1 value */)
#define SN_STRING "sn"

 // This will contain the device ID, before we have it this dummy value is the init value which is non-0
char* attDeviceID;
char attDeviceID_buf[20] = "BAAAAADD1DBAAADD1D";
char buf[25];
shared_networking_params_t shared_networking_params;
ATCA_STATUS retValCryptoClientSerialNumber;

uint32_t MAIN_dataTask(void* payload);
timerStruct_t MAIN_dataTasksTimer = { MAIN_dataTask };
typedef void (*cloud_cb_t)(void);
cloud_cb_t fpSendToCloudCallback;

void  wifiConnectionStateChanged(uint8_t status);

void application_init() {
	uint8_t mode = WIFI_DEFAULT;

	LED_test();
#if CFG_ENABLE_CLI     
	CLI_init();
	CLI_setdeviceId(attDeviceID);
#endif   
	debug_init(attDeviceID);

	// Initialization of modules where the init needs interrupts to be enabled
	cryptoauthlib_init();
	if (cryptoDeviceInitialized == false)
	{
		debug_printError("APP: CryptoAuthInit failed");
	}
    
#ifdef HUB_DEVICE_ID
	attDeviceID = HUB_DEVICE_ID;
#else    
	// Get serial number from the ECC608 chip 
	retValCryptoClientSerialNumber = CRYPTO_CLIENT_printSerialNumber(attDeviceID_buf);
	if (retValCryptoClientSerialNumber != ATCA_SUCCESS)
	{
		switch (retValCryptoClientSerialNumber)
		{
		case ATCA_GEN_FAIL:
			debug_printError("APP: DeviceID generation failed, unspecified error");
			break;
		case ATCA_BAD_PARAM:
			debug_printError("APP: DeviceID generation failed, bad argument");
		default:
			debug_printError("APP: DeviceID generation failed");
			break;
		}
	}
    else
    {
        // To use Azure provisioning service, attDeviceID should match with the device cert CN,
        // which is the serial number of ECC608 prefixed with "sn" if you are using the 
        // the microchip provisioning tool for PIC24.
        strcat(buf, SN_STRING);
        strcat(buf, attDeviceID_buf);
        attDeviceID = buf;
        debug_print("CRYPTO_CLIENT_printSerialNumber %s", attDeviceID);
    }
#endif    
    
#if CFG_ENABLE_CLI   
	CLI_setdeviceId(attDeviceID);
#endif   
	debug_setPrefix(attDeviceID);

	if (!SW0_Value && SW1_Value)
	{
		//SW0 only
		mode = WIFI_SOFT_AP;
	}
	else if (SW0_Value && !SW1_Value)
	{
		//SW1 only
	}
	else if (!SW0_Value && !SW1_Value)
	{
		//SW0 and SW1
		strcpy(ssid, CFG_MAIN_WLAN_SSID);
		strcpy(pass, CFG_MAIN_WLAN_PSK);
		sprintf((char*)authType, "%d", CFG_MAIN_WLAN_AUTH);
		LED_startBlinkingGreen();
	}
	strcpy(ssid, CFG_MAIN_WLAN_SSID);
	strcpy(pass, CFG_MAIN_WLAN_PSK);
	LED_startBlinkingGreen();
	wifi_init(wifiConnectionStateChanged, mode);

	if (mode == WIFI_DEFAULT) {
		application_post_provisioning();
	}

	LED_test();
}

void application_post_provisioning(void)
{
	CLOUD_init(attDeviceID);
}

void application_cloud_mqtt_connect(char* host, pf_MQTT_CLIENT* pf_table, cloud_cb_t fpSendToCloud)
{
	CLOUD_init_host(host, attDeviceID, pf_table);
    fpSendToCloudCallback = fpSendToCloud;
    timeout_create(&MAIN_dataTasksTimer, MAIN_DATATASK_INTERVAL);    
}

// React to the WIFI state change here. Status of 1 means connected, Status of 0 means disconnected
void  wifiConnectionStateChanged(uint8_t status)
{
	// If we have no AP access we want to retry
	if (status != true)
	{
		// Restart the WIFI module if we get disconnected from the WiFi Access Point (AP)
		CLOUD_reset();
	}
}


// This scheduler will check all tasks and timers that are due and service them
void runScheduler(void)
{
	timeout_callNextCallback();
}


// This could be better done with a function pointer (DI) but in the interest of simplicity 
//     we avoided that. This is being called from MAIN_dataTask below  
void sendToCloud(void);

// This gets called by the scheduler approximately every 100ms
uint32_t MAIN_dataTask(void* payload)
{
	static time_t previousTransmissionTime = 0;

	// Get the current time. This uses the C standard library time functions
	time_t timeNow = time(NULL);

	// Example of how to send data when MQTT is connected every 1 second based on the system clock
	if (fpSendToCloudCallback && CLOUD_isConnected())
	{
		// How many seconds since the last time this loop ran?
		int32_t delta = difftime(timeNow, previousTransmissionTime);
		if (delta >= CFG_SEND_INTERVAL)
		{
			previousTransmissionTime = timeNow;

			// Call the data task in main.c
			fpSendToCloudCallback();
		}
	}

	if (shared_networking_params.haveAPConnection)
	{
		LED_BLUE_SetLow();
	}
	else
	{
		LED_BLUE_SetHigh();
	}

	if (LED_isBlinkingGreen() == false)
	{
		if (CLOUD_isConnected())
		{
			LED_GREEN_SetLow();
		}
		else
		{
			LED_GREEN_SetHigh();
		}
	}

	// This is milliseconds managed by the RTC and the scheduler, this return makes the
	//      timer run another time, returning 0 will make it stop
	return MAIN_DATATASK_INTERVAL;
}
