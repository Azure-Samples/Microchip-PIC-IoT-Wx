#ifndef CONF_WIFI_M2M_H_INCLUDED
#define CONF_WIFI_M2M_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/** BUS Type Settings */
#define CONF_WIFI_M2M_SPI (1)
#define CONF_WINC_USE_SPI (1)

/** Debug Options */
#define CONF_WIFI_M2M_DEBUG (0)
#define CONF_WIFI_M2M_debug_print printf

// #define CONF_WINC_DEBUG					(1)
// #define CONF_WINC_PRINTF				printf

// <h> WLAN Configuration

// <s> SSID
// <i> Target WLAN SSID
// <id> main_wlan_ssid
//#define CFG_MAIN_WLAN_SSID "GUESTNETWORK"
#define CFG_MAIN_WLAN_SSID "SSID"
    
// <y> Authentication
// <i> Target WLAN Authentication
// <M2M_WIFI_SEC_INVALID"> Invalid security type
// <M2M_WIFI_SEC_OPEN"> Wi-Fi network is not secured
// <M2M_WIFI_SEC_WPA_PSK"> Wi-Fi network is secured with WPA/WPA2 personal(PSK)
// <M2M_WIFI_SEC_WEP"> Security type WEP (40 or 104) OPEN OR SHARED
// <M2M_WIFI_SEC_802_1X"> Wi-Fi network is secured with WPA/WPA2 Enterprise.IEEE802.1x user-name/password authentication
// <id> main_wlan_auth
//#define CFG_MAIN_WLAN_AUTH M2M_WIFI_SEC_OPEN
#define CFG_MAIN_WLAN_AUTH M2M_WIFI_SEC_WPA_PSK
    
// <s> Password
// <i> Target WLAN password
// <id> main_wlan_psk
//#define CFG_MAIN_WLAN_PSK ""
#define CFG_MAIN_WLAN_PSK "wifipassword"
    
// </h>

// <h> AP Mode Configuration

// <s> Name
// <i> AP name
// <id> wlan_ap_name
#define CFG_WLAN_AP_NAME "PIC.IOT.ACCESSPOINT"

// <s> IP Address
// <i> AP IP Address
// <id> wlan_ap_ip_address
#define CFG_WLAN_AP_IP_ADDRESS                                                                                         \
	{                                                                                                                  \
		192, 168, 1, 1                                                                                                 \
	}
// </h>

#ifdef __cplusplus
}
#endif

#endif /* CONF_WIFI_M2M_H_INCLUDED */
