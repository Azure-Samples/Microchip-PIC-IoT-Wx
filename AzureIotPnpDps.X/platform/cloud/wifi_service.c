/*
 * wifi_service.c
 *
 * Created: 10/4/2018 2:21:45 PM
 *  Author: I17643
 */
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "../clock.h"
#include <libpic30.h>
#include "wifi_service.h"
#include "../winc/driver/include/m2m_wifi.h"
#include "../drivers/timeout.h"
#include "../application_manager.h"
#include "../debug_print.h"
#include "../config/IoT_Sensor_Node_config.h"
#include "../config/conf_winc.h"
#include "../config/mqtt_config.h"
#include "../winc/socket/include/socket.h"
#include "../credentials_storage/credentials_storage.h"
#include "../tmr2.h"
#include "../led.h"
#include "../winc/driver/include/m2m_ssl.h"

#define CLOUD_WIFI_TASK_INTERVAL        timeout_mSecToTicks(50L)
#define CLOUD_NTP_TASK_INTERVAL         timeout_mSecToTicks(500L)
#define SOFT_AP_CONNECT_RETRY_INTERVAL  timeout_mSecToTicks(1000L)

 // Scheduler
uint32_t ntpTimeFetchTask(void* payload);
uint32_t wifiHandlerTask(void* param);
uint32_t softApConnectTask(void* param);

timerStruct_t softApConnectTimer = { softApConnectTask };
timerStruct_t ntpTimeFetchTimer = { ntpTimeFetchTask };
timerStruct_t wifiHandlerTimer = { wifiHandlerTask };

uint32_t checkBackTask(void* param);
timerStruct_t checkBackTimer = { checkBackTask };

static bool responseFromProvisionConnect = false;
static bool isSoftAP = false;

void (*callback_funcPtr)(uint8_t);

void enable_provision_ap(void);

// Callback function pointer for indicating status updates upwards
void  (*wifiConnectionStateChangedCallback)(uint8_t  status) = NULL;

// Function to be called by WifiModule on status updates from below
static void wifiCallback(uint8_t msgType, void* pMsg);

// This is a workaround to wifi_deinit being broken in the winc, so we can de-init without hanging up
int8_t hif_deinit(void* arg);

void wifi_reinit()
{
	tstrWifiInitParam param;
	/* Initialize Wi-Fi parameters structure. */
	memset((uint8_t*)&param, 0, sizeof(tstrWifiInitParam));

	param.pfAppWifiCb = wifiCallback;
	socketDeinit();
	hif_deinit(NULL);
	nm_bsp_deinit();

	wifiConnectionStateChangedCallback = callback_funcPtr;

	nm_bsp_init();
	m2m_wifi_init(&param);

	socketInit();
}

// funcPtr passed in here will be called indicating AP state changes with the following values
// Wi-Fi state is disconnected   == 0
// Wi-Fi state is connected      == 1
// Wi-Fi state is undefined      == 0xff
void wifi_init(void (*funcPtr)(uint8_t), uint8_t mode)
{
	callback_funcPtr = funcPtr;
	isSoftAP = (mode == WIFI_SOFT_AP);

	// This uses the global ptr set above
	wifi_reinit();
	LED_SetRed(LED_STATE_OFF);
	// Mode == 0 means AP configuration mode
	if (isSoftAP)
	{
		debug_printInfo("  WiFi: Access Point Mode for provisioning");
		LED_SetBlue(LED_STATE_BLINK_SLOW);
		enable_provision_ap();
	}
	else
	{
		LED_SetBlue(LED_STATE_BLINK_FAST);
		timeout_create(&ntpTimeFetchTimer, CLOUD_NTP_TASK_INTERVAL);
	}

	timeout_create(&wifiHandlerTimer, CLOUD_WIFI_TASK_INTERVAL);
}

bool wifi_connectToAp(uint8_t passed_wifi_creds)
{
	int8_t e = 0;

	LED_SetBlue(LED_STATE_BLINK_FAST);

	m2m_wifi_configure_sntp((char*)"time.nist.gov", 18, SNTP_ENABLE_DHCP);
	//e = m2m_wifi_set_device_name((uint8*)"01233EAD58E86797FE", strlen("01233EAD58E86797FE"));

	if (strlen(CFG_MAIN_WLAN_SSID) > 0)
	{
		passed_wifi_creds = NEW_CREDENTIALS;
		sprintf(authType, "%d", CFG_MAIN_WLAN_AUTH);
		strcpy(ssid, CFG_MAIN_WLAN_SSID);
		strcpy(pass, CFG_MAIN_WLAN_PSK);
	}

	if (passed_wifi_creds == NEW_CREDENTIALS)
	{
		e = m2m_wifi_connect((char*)ssid, sizeof(ssid), atoi((char*)authType), (char*)pass, M2M_WIFI_CH_ALL);
	}
	else
	{
		e = m2m_wifi_default_connect();
	}

	if (M2M_SUCCESS != e)
	{
		debug_printError("  WiFi: WiFi error = %d", e);
		shared_networking_params.haveERROR = 1;
		LED_SetRed(LED_STATE_HOLD);
		return false;
	}

	return true;
}

uint32_t softApConnectTask(void* param)
{
	if (!wifi_connectToAp((uint8_t)NEW_CREDENTIALS))
	{
		debug_printError("  WiFi: Soft AP Connect Failure");
	}
	else
	{
		debug_printInfo("  WiFi: SoftAP: New Connect credentials sent WINC");
	}
	return SOFT_AP_CONNECT_RETRY_INTERVAL;
}

bool wifi_disconnectFromAp(void)
{
	int8_t m2mDisconnectError;
	if (shared_networking_params.haveAPConnection == 1)
	{
		if (M2M_SUCCESS != (m2mDisconnectError = m2m_wifi_disconnect()))
		{
			debug_printError("  WiFi: Disconnect from AP error = %d", m2mDisconnectError);
			return false;
		}
	}
	return true;
}

// Update the system time every CLOUD_NTP_TASK_INTERVAL milliseconds
uint32_t ntpTimeFetchTask(void* payload)
{
	m2m_wifi_get_sytem_time();
	return CLOUD_NTP_TASK_INTERVAL;
}


uint32_t wifiHandlerTask(void* param)
{
	m2m_wifi_handle_events(NULL);
	return CLOUD_WIFI_TASK_INTERVAL;
}

uint32_t checkBackTask(void* param)
{
	shared_networking_params.haveAPConnection = 0;
	shared_networking_params.haveERROR = 1;
	shared_networking_params.amDisconnecting = 0;
	return 0;
}

static void wifiCallback(uint8_t msgType, void* pMsg)
{
	switch (msgType)
	{
		case M2M_WIFI_RESP_CON_STATE_CHANGED:
		{
			tstrM2mWifiStateChanged* pstrWifiState = (tstrM2mWifiStateChanged*)pMsg;
			if (pstrWifiState->u8CurrState == M2M_WIFI_CONNECTED)
			{
				if (responseFromProvisionConnect)
				{
					timeout_delete(&softApConnectTimer);
					responseFromProvisionConnect = false;
					timeout_create(&ntpTimeFetchTimer, CLOUD_NTP_TASK_INTERVAL);
					application_post_provisioning();
				}
				shared_networking_params.haveAPConnection = 1;
				debug_printGood("  WiFi: CONNECTED");
				CREDENTIALS_STORAGE_clearWifiCredentials();

				if (!isSoftAP)
				{
					LED_SetBlue(LED_STATE_HOLD);
				}
				LED_SetRed(LED_STATE_OFF);
				// We need more than AP to have an APConnection, we also need a DHCP IP address!
			}
			else if (pstrWifiState->u8CurrState == M2M_WIFI_DISCONNECTED)
			{
				debug_printWarn("  WiFi: DISCONNECTED: %d", pstrWifiState->u8ErrCode);
				
				if (!isSoftAP)
				{
					LED_SetBlue(LED_STATE_OFF);
				}
				LED_SetRed(LED_STATE_HOLD);
				
				timeout_create(&checkBackTimer, CLOUD_WIFI_TASK_INTERVAL);
				shared_networking_params.amDisconnecting = 1;
			}

			if ((wifiConnectionStateChangedCallback != NULL) && (shared_networking_params.amDisconnecting == 0))
			{
				wifiConnectionStateChangedCallback(pstrWifiState->u8CurrState);
			}
			break;
		}

		case M2M_WIFI_RESP_GET_SYS_TIME:
		{
			tstrSystemTime* WINCTime = (tstrSystemTime*)pMsg;
			struct tm theTime;

			// Convert to UNIX_EPOCH, this mktime uses years since 1900 and months are 0 based so we
			// are doing a couple of adjustments here.
			if (WINCTime->u16Year > 0)
			{
				theTime.tm_hour = WINCTime->u8Hour;
				theTime.tm_min = WINCTime->u8Minute;
				theTime.tm_sec = WINCTime->u8Second;
				theTime.tm_year = WINCTime->u16Year - 1900;
				theTime.tm_mon = WINCTime->u8Month - 1;
				theTime.tm_mday = WINCTime->u8Day;
				theTime.tm_isdst = 0;

				TMR2_Counter32BitSet(mktime(&theTime));
			}
			break;
		}

		case M2M_WIFI_RESP_PROVISION_INFO:
		{
			tstrM2MProvisionInfo* pstrProvInfo = (tstrM2MProvisionInfo*)pMsg;
			if (pstrProvInfo->u8Status == M2M_SUCCESS)
			{
				sprintf((char*)authType, "%d", pstrProvInfo->u8SecType);
				strcpy(ssid, (char*)pstrProvInfo->au8SSID);
				strcpy(pass, (char*)pstrProvInfo->au8Password);
				debug_printInfo("  WiFi: SsftAP : Connect Credentials sent to WINC");
				responseFromProvisionConnect = true;
				timeout_create(&softApConnectTimer, 0);
			}
			break;
		}

		default:
		{
			break;
		}
	}
}

void enable_provision_ap(void)
{
	debug_printGood("  WiFi: Enableing SoftAP : SSID %s", CFG_WLAN_AP_NAME);
	tstrM2MAPConfig apConfig = {
		CFG_WLAN_AP_NAME, // Access Point Name.
		1, // Channel to use.
		0, // Wep key index.
		WEP_40_KEY_STRING_SIZE, // Wep key size.
		"1234567890", // Wep key.
		M2M_WIFI_SEC_OPEN, // Security mode.
		SSID_MODE_VISIBLE, // SSID visible.
		CFG_WLAN_AP_IP_ADDRESS
	};
	static char gacHttpProvDomainName[] = CFG_WLAN_AP_NAME;
	m2m_wifi_start_provision_mode(&apConfig, gacHttpProvDomainName, 1);
}

bool wifi_getIpAddressByHostName(uint8_t* host_name)
{
	return gethostbyname(host_name) == M2M_SUCCESS;
}

