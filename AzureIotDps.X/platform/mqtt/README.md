# Microchip MQTT Library APIs

## Introduction

  MQTT (Message Queuing Telemetry and Transport) is a publish/subscribe messaging transport protocol. It is widely used in many situations, such as communications in Machine to Machine (M2M) or Internet of Things (IoT) applications, especially in bandwidth and/or memory constrained environments. This protocol runs over TCP/IP, or can over other network protocols that provide ordered, lossless, bi-directional connections. In this implementation of MQTT is implemented over TCP. 
  This MQTT implementation supports all the required features and message types of this protocol along with some optional features. 
  Supported message types:
 - CONNECT
 - CONNACK
 - PUBLISH (QoS level = 0)
 - PINGREQ
 - PINGRESP
 - SUBSCRIBE (QoS level = 0, beta)
 - DISCONNECT 
 
 This document describes the structure and the APIs implemented as part of the MQTT library. The public APIs are used by user application for performing all the MQTT specific tasks such as establishing a MQTT connection, publishing data, sending ping requests, etc. The private APIs are used internally by the MQTT library for maintaining its modularity. 

## Public APIs
1.	INITIALISE
    - MQTT_ClientInitialise

		i.	Description
		void MQTT_ClientInitialise(void)
		MQTT_ClientInitialise API initializes all the parameters in the send and receive data buffer used by the MQTT library. The size of these buffers is dictated by the application requirements. The buffers are used by MQTT library to handler packet exchange with MQTT broker.

		ii.	Parameters
		None.

		iii. Return Values
		None.
2.	CONNECT
    - MQTT_CreateConnectPacket

		i.	Description
		bool MQTT_CreateConnectPacket(mqttConnectPacket *newConnectPacket)
		MQTT_CreateConnectPacket API creates a CONNECT packet structure, which follows MQTT standard.

		ii.	Parameters
		A pointer that points to a MQTT CONNECT packet structure mqttConnectPacket.

		iii. Return Values
		A bool type value indicating whether a connect data packet structure is initialized successfully. A return value of 'true' implies that CONNECT packet has been successfully created.
3.	PUBLISH
    - MQTT_CreatePublishPacket

		i.	Description
		bool MQTT_CreatePublishPacket(mqttPublishPacket *newPublishPacket)
		MQTT_CreatePublishPacket API creates a MQTT publish data packet structure, which follows MQTT standard.

		ii.	Parameters
		A pointer that points to a MQTT PUBLISH packet structure mqttPublishPacket.

		iii. Return Values
		A bool value indicating whether a publish data packet structure is created successfully. A return value of 'true' means that the PUBLISH packet has been created correctly as per the parameters passed by the user application.
4.	SUBSCRIBE
    - MQTT_CreateSubscribePacket

		i.	Description
		bool MQTT_CreateSubscribePacket(mqttSubscribePacket *newSubscribePacket)
		MQTT_CreateSubscribePacket API creates a MQTT subscribe data packet structure, which follows MQTT standard.

		ii.	Parameters
		A pointer that points to a MQTT SUBSCRIBE packet structure mqttSubscribePacket.

		iii. Return Values
		A bool type value indicating whether a subscribe data packet structure is created successfully. A return value of 'true' means that the SUBSCRIBE packet has been created correctly as per the parameters passed by the user application.
5.	DISCONNECT
    - MQTT_Disconnect

		i.	Description
		void MQTT_Disconnect(void)
		This API sends a MQTT DISCONNECT packet. This disconnects the client cleanly from the MQTT server.

		ii.	Parameters
		None.

		iii. Return Values
		None.
6.	GET CLIENT CONNECTION INFO
    - MQTT_GetClientConnectionInfo

		i.	Description
		mqttTxRxInformation* MQTT_GetClientConnectionInfo(void) 
		This API returns a pointer to the current MQTT connection's information, which is essentially a structure with both transmit and receive buffer information.  

		ii.	Parameters
		None.

		iii.Return Values
		A pointer points to current MQTT connection's buffer information, which is a structure containing MQTT transmit and receive buffer information.

7.	GET RECEIVED DATA
    - MQTT_GetReceivedData

		i.	Description
		void MQTT_GetReceivedData(uint8_t *pData, uint8_t len) 
		MQTT_GetReceivedData API is responsible for receiving packets from the MQTT server and copying the received packets in the reception specific exchange buffer.  

		ii.	Parameters
		uint8_t *pData: the received data buffer pointer.
		uint8_t len: the received data length.

		iii.Return Values
		None.
8.	GET CONNECTION STATE
    - MQTT_GetConnectionState

		i.	Description
		mqttCurrentState MQTT_GetConnectionState(void) 
		MQTT_GetConnectionState API returns the current MQTT connection state. Possible values include DISCONNECTED, CONNECTING, WAITFORCONNACK, CONNECTED.   

		ii.	Parameters
		None.

		iii. Return Values
		mqttCurrentState: An enum indicating the current MQTT connection state. Possible valid values: DISCONNECTED, CONNECTING, WAITFORCONNACK, CONNECTED.
9.	RECEPTION HANDLER
    - MQTT_ReceptionHandler

		i.	Description
		mqttCurrentState MQTT_ReceptionHandler(mqttTxRxInformation *mqttConnectionPtr) 
		MQTT_ReceptionHandler API handles the received MQTT packet based on the MQTT state and then sets the state to a proper value based on the data received.   

		ii.	Parameters
		A pointer that points to the current MQTT returns a pointer to the current MQTT connection's information, which is essentially a structure relevant buffer information.

		iii. Return Values
		mqttCurrentState: An enum indicating the current MQTT connection state.  Possible valid values: DISCONNECTED, CONNECTING, WAITFORCONNACK, CONNECTED.
10.	TRANSMISSION HANDLER
    - MQTT_TransmissionHandler

		i.	Description
		mqttCurrentState MQTT_TransmissionHandler(mqttTxRxInformation *mqttConnectionPtr) 
		MQTT_TransmissionHandler API sends out an MQTT packet based on the settings of MQTT packet transmission flags and the current MQTT state, then set the current MQTT state to a proper state.   

		ii.	Parameters
		A pointer that points to the current MQTT returns a pointer to the current MQTT connection's information, which is essentially a structure relevant buffer information.

		iii. Return Values
		mqttCurrentState: An enum indicating the current MQTT connection state. Possible valid values: DISCONNECTED, CONNECTING, WAITFORCONNACK, CONNECTED.
11.	SET PUBLISH RECEPTION HANDLER TABLE
    - MQTT_SetPublishReceptionHandlerTable

		i.	Description
		void MQTT_SetPublishReceptionHandlerTable(publishReceptionHandler_t *appPublishReceptionInfo) 
		MQTT_SetPublishReceptionHandlerTable is called by the user application to inform the MQTT core of the call back table defined to handle the PUBLISH messages received from the MQTT server.   

		ii.	Parameters
		A publishReceptionHandler_t table information defined in the user application, which involves a call back function pointer of a corresponding MQTT topic.

		iii. Return Values
	None.
12.	GET PUBLISH RECEPTION HANDLER TABLE
    - MQTT_GetPublishReceptionHandlerTable
		i.	Description
		publishReceptionHandler_t *MQTT_GetPublishReceptionHandlerTable(); 
		MQTT_GetPublishReceptionHandlerTable API returns a publishReceptionHandler_t table information defined in the user application, which involves a call back function pointer of a corresponding MQTT topic.   

		ii.	Parameters
		None.

		iii. Return Values
		A publishReceptionHandler_t table information defined in the user application, which involves a call back function pointer of a corresponding MQTT topic.
13.	GET CONNECTION AGE
    - MQTT_getConnectionAge

		i.	Description
		int32_t MQTT_getConnectionAge(void); 
		MQTT_getConnectionAge API calculates the time elapsed since MQTT connection setup.   

		ii.	Parameters
		None.

		iii. Return Values
		The value indicating the time elapsed since MQTT connection setup.

## Private APIs
1. SEND CONNECT
    - mqttSendConnect

		i.	Description
		static bool mqttSendConnect(mqttTxRxInformation *mqttConnectionPtr); 
		mqttSendConnect API sends the MQTT CONNECT packet using the underlying TCP layer.   

		ii.	Parameters
		Pointer to the MQTT connection structure *mqttConnectionPtr.

		iii. Return Values
		Boolean value indicating whether the packet has been successfully sent. The value 'true' implies that the packet has been sent successfully to the server.
2.	SEND PUBLISH
    - mqttSendPublish

		i.	Description
		static bool mqttSendPublish(mqttTxRxInformation *mqttConnectionPtr);
	 
		mqttSendPublish API sends the MQTT PUBLISH packet using the underlying TCP layer.   
		ii.	Parameters
		Pointer to the MQTT connection structure *mqttConnectionPtr.

		iii. Return Values
		Boolean value indicating whether the packet has been successfully sent. The value 'true' implies that the packet has been sent successfully to the server.

3.	SEND PINGREQ
    - mqttSendPingreq

		i.	Description
		static bool mqttSendPingreq(mqttTxRxInformation *mqttConnectionPtr); 
		mqttSendPingreq API sends the MQTT PINGREQ packet using the underlying TCP layer.  
	 
		ii.	Parameters
		Pointer to the MQTT connection structure *mqttConnectionPtr.

		iii. Return Values
		Boolean value indicating whether the packet has been successfully sent. The value 'true' implies that the packet has been sent to the server
4.	SEND DISCONNECT
    - mqttSendDisconnect

		i.	Description
		static bool mqttSendDisconnect(mqttTxRxInformation *mqttConnectionPtr); 
		mqttSendDisconnect API sends the MQTT DISCONNECT packet using the underlying TCP layer.  
	 
		ii.	Parameters
		Pointer to the MQTT connection structure *mqttConnectionPtr.

		iii. Return Values
		Boolean value indicating whether the packet has been successfully sent. The value 'true' implies that the packet has been sent to the server

5.	PROCESS CONNACK
    - mqttProcessConnack

		i.	Description
		static mqttCurrentState mqttProcessConnack(mqttTxRxInformation *mqttConnectionPtr); 
		Processes the CONNACK packet received from the broker.   

		ii.	Parameters
		Pointer to the MQTT connection structure *mqttConnectionPtr.

		iii. Return Values
		Current state of the MQTT (CONNECTED if correct CONNACK packet is received and DISCONNECTED if certain parameters in the CONNACK packet indicate that the server has not granted a connection).

6.	PROCESS PINGRESP
    - mqttProcessPingresp

		i.	Description
		static mqttCurrentState mqttProcessPingresp(mqttTxRxInformation *mqttConnectionPtr); 
		Processes the PINGRESP packet received from the broker.  
	 
		ii.	Parameters
		Pointer to the MQTT connection structure *mqttConnectionPtr.

		iii. Return Values
		None.

7.	PROCESS SUBACK
    - mqttProcessSuback

		i.	Description
		static mqttCurrentState mqttProcessSuback(mqttTxRxInformation *mqttConnectionPtr); 
		Processes the PINGRESP packet received from the broker.   

		ii.	Parameters
		Pointer to the MQTT connection structure *mqttConnectionPtr.

		iii. Return Values
		Current state of the MQTT (CONNECTED if correct SUBACK packet is received correctly and DISCONNECTED if certain parameters in the SUBACK packet indicate that the server has acknowledged the SUBSCRIBE message completely).

8.	CHECK CONNACK TIMEOUT STATE
    - checkConnackTimeoutState

		i.	Description
		static absolutetime_t checkConnackTimeoutState (); 
		checkConnackTimeoutState is a call back function that will be called when a timeout (30s) has occurred after sending the CONNECT packet, since a CONNACK packet is expected from the broker within 30s.   

		ii.	Parameters
		None.

		iii. Return Values
		Number of ticks until the connackTimer expires. In the current implementation it is 0, indicating that the timer function will be executed only once.

9.	CHECK PINGREQ TIMEOUT STATE
    - checkPingreqTimeoutState

		i.	Description
		static absolutetime_t checkPingreqTimeoutState (); 
		checkPingreqTimeoutState is a call back function that will be called when a "keep-alive-timeout" defined in user application is near after a MQTT connection has been set up to make sure the connection keeps alive. In the current implementation it is 1 second before "keep-alive-timeout".   

		ii.	Parameters
		None.

		iii. Return Values
		Number of ticks until the pingreqTimer expires.

10.	CHECK PINGRESP TIMEOUT STATE
    - checkPingrespTimeoutState

		i.	Description
		static absolutetime_t checkPingrespTimeoutState (); 
		checkPingrespTimeoutState is a call back function that will be called when a timeout (30s) has occurred after sending a PINGREQ packet.  In the current MQTT client implementation, the client waits for 30s after transmission of PINGREQ packet to receive a PINGRESP packet.   

		ii.	Parameters
		None.

		iii. Return Values
		Number of ticks until the pingrespTimer expires.

11.	ENCODING REMAINING LENGTH
    - mqttEncodeLength

		i.	Description
		static uint8_t mqttEncodeLength(uint16_t length, uint8_t *output); 
		The function encodes the text fields in MQTT packets as UTF-8 strings 
	  
		ii.	Parameters
		uint16_t length: the number of bytes remaining within the current packet, including data in the variable header and the payload. 
		uint8_t *output: a pointer points to the encoded bytes.

		iii. Return Values
		The number of bytes encoded.

12.	DECODING REMAINING LENGTH
    - mqttDecodeLength

		i.	Description
		static absolutetime_t mqttEncodeLength(uint8_t *encodedData); 
		The function decodes UTF-8 encoded string to text fields as per the requirement of MQTT standard.   

		ii.	Parameters
		uint8_t *encodedData: a pointer points to the encoded value of remaining length of a MQTT control packet.

		iii. Return Values
		The decoded value of the remaining length of an MQTT control packet header.

## Dependent APIs

1.	EXCHANGE BUFFER INIT
    - ExchangeBufferInit

		i.	Description
		void MQTT_ExchangeBufferInit(exchangeBuffer *buffer); 
		Sets the current location pointer to the beginning of the buffer and sets the length to zero.   

		ii.	Parameters
		Pointer to Exchange Buffer structure.

		iii.Return Values
		None.
2.	EXCHANGE BUFFER WRITE
    - ExchangeBufferWrite

		i.	Description
		uint16_t MQTT_ExchangeBufferWrite(exchangeBuffer *buffer, uint8_t *data, uint16_t length); 
		Sets the current location pointer to the beginning of the buffer and sets the length to zero.  
	 
		ii.	Parameters
		Copies a data buffer to the Exchange Buffer.

		iii. Return Values
		Length.
3.	EXCHANGE BUFFER READ
    - ExchangeBufferRead

		i.	Description
		uint16_t MQTT_ExchangeBufferRead(exchangeBuffer *buffer, uint8_t *data, uint16_t length); 
		Copies the Exchange buffer to the data buffer.  Exchange buffer is reset in the process. 
	  
		ii.	Parameters
		Copies a data buffer to the Exchange Buffer.

		iii. Return Values
		Number of bytes copied.
4.	EXCHANGE BUFFER PEEK
    - ExchangeBufferPeek

		i.	Description
		uint16_t MQTT_ExchangeBufferPeek(exchangeBuffer *buffer, uint8_t *data, uint16_t length); 
		Copies data out of the Exchange buffer without modifying the data length or current pointer.   

		ii.	Parameters
		Pointer to Exchange Buffer structure, Pointer to Data Buffer, dataLength.

		iii. Return Values
		Number of bytes copied.

## References
[MQTT Standard](http://mqtt.org/documentation)