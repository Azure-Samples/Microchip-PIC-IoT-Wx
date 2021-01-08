/*
	\file   cloud_service.c

	\brief  Cloud Service Abstraction Layer

	(c) 2018 Microchip Technology Inc. and its subsidiaries.

	Subject to your compliance with these terms, you may use Microchip software and any
	derivatives exclusively with Microchip products. It is your responsibility to comply with third party
	license terms applicable to your use of third party software (including open source software) that
	may accompany Microchip software.

	THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
	EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
	IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
	FOR A PARTICULAR PURPOSE.

	IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
	INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
	WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
	HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
	THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
	CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
	OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
	SOFTWARE.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../clock.h"
#include "cloud_service.h"
#include "bsd_adapter/bsdWINC.h"
#include "../winc/driver/include/m2m_wifi.h"
#include "../winc/socket/include/socket.h"
#include "../config/IoT_Sensor_Node_config.h"
#include "crypto_client/crypto_client.h"
#include "crypto_client/cryptoauthlib_main.h"
#include "../debug_print.h"
#include "../drivers/timeout.h"
#include "../mqtt/mqtt_core/mqtt_core.h"
#include "wifi_service.h" 
#include "../led.h"

#include "../application_manager.h"
#include "../credentials_storage/credentials_storage.h"
#include "../mqtt/mqtt_packetTransfer_interface.h"
#include "azure/iot/az_iot_hub_client.h"

static bool cloudInitialized = false;
static bool waitingForMQTT = false;
pf_MQTT_CLIENT* pf_mqtt_client;

char* mqtt_host;
char* deviceId;
char mqttSubscribeTopic[TOPIC_SIZE];

// Scheduler Callback functions
uint32_t CLOUD_task(void* param);
uint32_t mqttTimeoutTask(void* payload);
uint32_t cloudResetTask(void* payload);

static void dnsHandler(uint8_t* domainName, uint32_t serverIP);
static int8_t connectMQTTSocket(void);
static void connectMQTT();
static uint8_t reInit(void);

bool isResetting = false;
bool cloudResetTimerFlag = false;
bool sendSubscribe = true;

#define CLOUD_TASK_INTERVAL            timeout_mSecToTicks(500L)
#define CLOUD_MQTT_TIMEOUT_COUNT       timeout_mSecToTicks(10000L)  // 10 seconds max allowed to establish a connection
#define MQTT_CONN_AGE_TIMEOUT          3600L                        // 3600 seconds = 60minutes
#define CLOUD_RESET_TIMEOUT            timeout_mSecToTicks(2000L)   // 2 seconds


// Create the timers for scheduler_timeout which runs these tasks
timerStruct_t CLOUD_taskTimer = { CLOUD_task };
timerStruct_t mqttTimeoutTaskTimer = { mqttTimeoutTask };
timerStruct_t cloudResetTaskTimer = { cloudResetTask };

uint32_t mqttHostIP;
uint32_t dnsRetryDelay = 0;

packetReceptionHandler_t cloud_packetReceiveCallBackTable[CLOUD_PACKET_RECV_TABLE_SIZE];

void NETWORK_wifiSslCallback(uint8_t u8MsgType, void *pvMsg)
{
	switch (u8MsgType)
	{
		case M2M_SSL_REQ_ECC:
		{
			tstrEccReqInfo *ecc_request = (tstrEccReqInfo*)pvMsg;
			CRYPTO_CLIENT_processEccRequest(ecc_request);
			
			break;
		}

		case M2M_SSL_RESP_SET_CS_LIST:
		{
			tstrSslSetActiveCsList *pstrCsList = (tstrSslSetActiveCsList *)pvMsg;
			debug_printInfo(" CLOUD: ActiveCS bitmap:%04x", pstrCsList->u32CsBMP);
			
			break;
		}

		default:
			break;
	}
}

void CLOUD_reset(void)
{
	debug_printInfo(" CLOUD: Cloud_reset()");
	cloudInitialized = false;
	CLOUD_disconnect();
}

uint32_t mqttTimeoutTask(void* payload) {
	debug_printError(" CLOUD: MQTT Connection Timeout");
	LED_SetRed(LED_STATE_HOLD);
	CLOUD_reset();
	waitingForMQTT = false;
	return 0;
}

uint32_t cloudResetTask(void* payload) {
	debug_printInfo(" CLOUD: cloudResetTask()");
	cloudInitialized = reInit();
	return 0;
}

void CLOUD_init(char* attDeviceID)
{
	debug_printInfo(" CLOUD: CLOUD_init() %s", attDeviceID);
	// Create timers for the application scheduler
	timeout_create(&CLOUD_taskTimer, CLOUD_TASK_INTERVAL);
	deviceId = attDeviceID;
	CLOUD_reset();
}

void CLOUD_init_host(char* host, char* attDeviceID, pf_MQTT_CLIENT* pf_table)
{
	mqtt_host = host;
	mqttHostIP = 0;
	pf_mqtt_client = pf_table;
	CLOUD_init(attDeviceID);
}

static void connectMQTT()
{
	debug_printInfo(" CLOUD: MQTT Connect %s", mqtt_host);

	uint32_t currentTime = time(NULL);
	if (currentTime > 0)
	{
		pf_mqtt_client->MQTT_CLIENT_connect(deviceId);
	}

	// MQTT SUBSCRIBE packet will be sent after the MQTT connection is established.
	sendSubscribe = true;
}

void CLOUD_subscribe(void)
{
	if (pf_mqtt_client->MQTT_CLIENT_subscribe() == true)
	{
		sendSubscribe = false;
	}
}

// This forces a disconnect, which forces a reconnect...
void CLOUD_disconnect(void) {
	debug_printWarn(" CLOUD: Disconnect");
	if (MQTT_GetConnectionState() == CONNECTED)
	{
		MQTT_Disconnect(MQTT_GetClientConnectionInfo());
	}
	else
	{
		LED_SetGreen(LED_STATE_OFF);
	}
}

packetReceptionHandler_t* getSocketInfo(uint8_t sock);
static int8_t connectMQTTSocket(void)
{
	int8_t ret = false;

	// Abstract the SSL section into a separate function
	int8_t sslInit;

	sslInit = m2m_ssl_init(NETWORK_wifiSslCallback);
	if(sslInit != M2M_SUCCESS)
	{
		debug_printError(" CLOUD: WiFi SSL Initialization failed. M2M Status %d", sslInit);
	}
		
	if (mqttHostIP > 0)
	{
		struct bsd_sockaddr_in addr;

		addr.sin_family = PF_INET;
		addr.sin_port = BSD_htons(CFG_MQTT_PORT);
		addr.sin_addr.s_addr = mqttHostIP;

		mqttContext* context = MQTT_GetClientConnectionInfo();
		socketState_t  socketState = BSD_GetSocketState(*context->tcpClientSocket);

		// Todo: Check - Are we supposed to call close on the socket here to ensure we do not leak ?
		if (socketState == NOT_A_SOCKET)
		{
			*context->tcpClientSocket = BSD_socket(PF_INET, BSD_SOCK_STREAM, 1);
			if (*context->tcpClientSocket >= 0)
			{
				packetReceptionHandler_t* sockInfo = getSocketInfo(*context->tcpClientSocket);
				if (sockInfo != NULL)
				{
					sockInfo->socketState = SOCKET_CLOSED;
				}
			}
		}

		socketState = BSD_GetSocketState(*context->tcpClientSocket);
		if (socketState == SOCKET_CLOSED) {
			debug_printWarn(" CLOUD: Configuring SNI to connect to %s", mqtt_host);
			ret = BSD_setsockopt(*context->tcpClientSocket, SOL_SSL_SOCKET, SO_SSL_SNI, mqtt_host, strlen(mqtt_host));
			if (ret == BSD_SUCCESS) {
				int optVal = 1;
				ret = BSD_setsockopt(*context->tcpClientSocket, SOL_SSL_SOCKET, SO_SSL_ENABLE_SNI_VALIDATION, &optVal, sizeof(optVal));
			}

			if (ret == BSD_SUCCESS) {
				debug_printGood(" CLOUD: Connect socket");
				ret = BSD_connect(*context->tcpClientSocket, (struct bsd_sockaddr*) & addr, sizeof (struct bsd_sockaddr_in));
			}
			else {
				debug_printError(" CLOUD: connect failed.  BSD Status = %d", ret);
				shared_networking_params.haveERROR = 1;
				LED_SetRed(LED_STATE_HOLD);
				BSD_close(*context->tcpClientSocket);
			}
		}
	}
	return ret;
}

uint32_t CLOUD_task(void* param)
{
	mqttContext* mqttConnnectionInfo = MQTT_GetClientConnectionInfo();

	if (!cloudInitialized)
	{
		if (!isResetting)
		{
			isResetting = true;
			debug_printInfo(" CLOUD: Cloud reset timer is created");
			timeout_delete(&mqttTimeoutTaskTimer);
			timeout_create(&cloudResetTaskTimer, CLOUD_RESET_TIMEOUT);
			cloudResetTimerFlag = true;
		}
	}
	else 
	{
		if (!waitingForMQTT)
		{
			if ((MQTT_GetConnectionState() != CONNECTED) && (cloudResetTimerFlag == false))
			{
				// Start the MQTT connection timeout
				debug_printWarn("  MQTT: MQTT reset timer is created");
				timeout_create(&mqttTimeoutTaskTimer, CLOUD_MQTT_TIMEOUT_COUNT);
				waitingForMQTT = true;
			}
		}
	}

	// If we have lost the AP we need to get the mqttState to disconnected
	if (shared_networking_params.haveAPConnection == false)
	{
		//Cleared on Access Point Connection
		shared_networking_params.haveERROR = 1;
		if (MQTT_GetConnectionState() == CONNECTED)
		{
			MQTT_initialiseState();
		}
	}
	else
	{
		static int32_t lastAge = -1;
		socketState_t socketState = BSD_GetSocketState(*mqttConnnectionInfo->tcpClientSocket);

		int32_t thisAge = MQTT_getConnectionAge();
		time_t theTime = time(NULL);
		if (theTime <= 0)
		{
			debug_printWarn(" CLOUD: time not ready");
		}
		else
		{
			if (MQTT_GetConnectionState() == CONNECTED)
			{
				if (lastAge != thisAge)
				{
					debug_printInfo(" CLOUD: Uptime %lus SocketState (%d) MQTT (%d)", thisAge, socketState, MQTT_GetConnectionState());
					lastAge = thisAge;
				}
			}
		}

		switch (socketState)
		{
		case NOT_A_SOCKET:
		case SOCKET_CLOSED:
			if (dnsRetryDelay)
			{
				dnsRetryDelay--;
				// still waiting for DNS look up
			}
			else if (mqttHostIP == 0)
			{
				dnsRetryDelay = 30;
				wifi_getIpAddressByHostName((uint8_t*)mqtt_host);
			}
			else
			{
				// Reinitialize MQTT
				MQTT_ClientInitialise();
				connectMQTTSocket();
			}
			break;

		case SOCKET_CONNECTED:
			// If MQTT was disconnected but the socket is up we retry the MQTT connection
			if (MQTT_GetConnectionState() == DISCONNECTED)
			{
				connectMQTT();
			}
			else
			{
				MQTT_ReceptionHandler(mqttConnnectionInfo);
				MQTT_TransmissionHandler(mqttConnnectionInfo);

				// Todo: We already processed the data in place using PEEK, this just flushes the buffer
				BSD_recv(*MQTT_GetClientConnectionInfo()->tcpClientSocket, MQTT_GetClientConnectionInfo()->mqttDataExchangeBuffers.rxbuff.start, MQTT_GetClientConnectionInfo()->mqttDataExchangeBuffers.rxbuff.bufferLength, 0);

				if (MQTT_GetConnectionState() == CONNECTED)
				{
					shared_networking_params.haveERROR = 0;
					timeout_delete(&mqttTimeoutTaskTimer);
					timeout_delete(&cloudResetTaskTimer);
					isResetting = false;
					waitingForMQTT = false;

					if (sendSubscribe == true)
					{
						CLOUD_subscribe();
					}

					// The Authorization timeout is set to 3600, so we need to re-connect that often
					// ericwol: todo remove this
					if (MQTT_getConnectionAge() > MQTT_CONN_AGE_TIMEOUT) {
						debug_printWarn("  MQTT: Connection aged, Uptime %lus SocketState (%d) MQTT (%d)", thisAge, socketState, MQTT_GetConnectionState());
						MQTT_Disconnect(mqttConnnectionInfo);
						BSD_close(*mqttConnnectionInfo->tcpClientSocket);
					}
				}
			}
			break;

		case SOCKET_IN_PROGRESS:
			break;

		default:
			shared_networking_params.haveERROR = 1;
			break;
		}
	}
	return CLOUD_TASK_INTERVAL;
}

bool CLOUD_isConnected(void)
{
	return MQTT_GetConnectionState() == CONNECTED;
}

void CLOUD_publishData(uint8_t* data, unsigned int len)
{
	pf_mqtt_client->MQTT_CLIENT_publish(data, len);
}

static void dnsHandler(uint8_t* domainName, uint32_t serverIP)
{
	if (serverIP != 0)
	{
		dnsRetryDelay = 0;
		mqttHostIP = serverIP;
		debug_printGood(" CLOUD: mqttHostIP %s = (%lu.%lu.%lu.%lu)", domainName, (0x0FF & (serverIP)), (0x0FF & (serverIP >> 8)), (0x0FF & (serverIP >> 16)), (0x0FF & (serverIP >> 24)));
	}
}

static uint8_t reInit(void)
{
	debug_printInfo(" CLOUD: reinit()");

	shared_networking_params.haveAPConnection = 0;
	waitingForMQTT = false;
	isResetting = false;
	uint8_t wifi_creds;

	// Re-init the WiFi
	wifi_reinit();

	registerSocketCallback(BSD_SocketHandler, dnsHandler);

	MQTT_ClientInitialise();
	memset(&cloud_packetReceiveCallBackTable, 0, sizeof(cloud_packetReceiveCallBackTable));
	BSD_SetRecvHandlerTable(cloud_packetReceiveCallBackTable);

	cloud_packetReceiveCallBackTable[0].socket = MQTT_GetClientConnectionInfo()->tcpClientSocket;
	cloud_packetReceiveCallBackTable[0].recvCallBack = pf_mqtt_client->MQTT_CLIENT_receive;

	// When the input comes through cli/.cfg
	if ((strcmp(ssid, "") != 0) && (strcmp(authType, "") != 0))
	{
		wifi_creds = NEW_CREDENTIALS;
		debug_printInfo("  WiFi: Connecting to AP with new credentials");
	}
	// This works provided the board had connected to the AP successfully	
	else
	{
		wifi_creds = DEFAULT_CREDENTIALS;
		debug_printInfo("  WiFi: Connecting to AP with the last used credentials");
	}

	if (!wifi_connectToAp(wifi_creds))
	{
		return false;
	}

	timeout_delete(&cloudResetTaskTimer);
	debug_printInfo(" CLOUD: Cloud reset timer is deleted");
	timeout_create(&mqttTimeoutTaskTimer, CLOUD_MQTT_TIMEOUT_COUNT);
	cloudResetTimerFlag = false;
	waitingForMQTT = true;

	return true;
}
