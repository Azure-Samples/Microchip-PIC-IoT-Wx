# Microchip BSD Adaption Layer APIs

## BSD Adapter Implementation

The adapter layer is intended to allow conformity with BSD APIs when using libraries, or drivers, which do not conform fully to BSD implementation.  

The purpose of the adapter is to provide a standard/expected BSD set of APIs, making portability easy, and to allow the adapter APIs to call required underlying APIs following the principle of polymorphism. Essentially making everything above this layer agnostic to the transport layer used such as WINC1500, TCPIP Lite or any other custom stack. 

The Red Hat Linux/i386 9 (Linux Programmers Manual) standard has been used as a reference for the BSD API implementations.

## BSD Adapter Protocol Support
* TCP (IPv4)
* UDP (IPv4)

## WINC1500 BSD Adapter Implementation
BSD layer makes use of the WINC library and ensures the conformity to the BSD standard. The APIs specific to BSD adapter layer begin with ?BSD_methodName? and the corresponding call would return either a ?BSD_SUCCESS? or a ?BSD_ERROR? in most cases with few exceptions wherein a pointer will be returned. To get the details of the specific error the ?BSD_GetErrNo? method could be called upon which in turn will return the ?bsdErrorNuber?.  All the BSD API?s functionality with respect to our implementation is described below: 

# BSD Standard Defined APIs

## 1. Socket
int BSD_socket(int domain, int type, int protocol) 
  
_**Description:**_
  
Called to establish a socket connection on a specific protocol using a provided transport type. Successful use of socket () will return a value 0, or greater, to correspond to its assigned BSD ?socket?. -1 value represent a failure to get the socket. 
  
_**Passed Parameters:**_
  
Name | Declaration Type | Supplied Types  
------------ | ------------- | -------------  
domain | int | bsdDomain_t  
type | int | bsdTypes_t  
protocol | int | 0/1  
  
_**Parameter Descriptions:**_
  
* domain ? Internet Protocol Type
* type ? Socket Transport Implementation of Protocol Type
* protocol - The supported protocols are NON-TLS(i.e 0) OR TLS(i.e 1)
  
_**Supplied Types:**_
  
### Domain Options
  
Enumerator Name | Usage | WINC  
------------ | ------------- | -------------  
PF_UNIX | Host Internal Protocol | [ ]  
PF_LOCAL | Host Internal Protocol | [ ]  
PF_INET | IPv4 | [X]  
PF_INET6 | IPv6 | [ ]  
  
### Type Options
  
Enumerator Name | Usage | WINC  
------------ | ------------- | -------------  
BSD_SOCK_STREAM | TCP/IP ? Sequenced, reliable, two-way, connection based by streams. Out-of-band data transmission mechanism may be supported | [X]  
BSD_SOCK_DGRAM | UDP ? Connectionless, unreliable messages of a fixed max length | [X]  
BSD_SOCK_SEQPACKET | Sequence, reliable, two-way connection-based data transmission path for datagrams of fixed maximum length. Consumers are required to read entire packets with each read system call | [X]  
BSD_SOCK_RAW | Raw network protocol access | [ ]  
BSD_SOCK_RDM | Reliable datagram layer that does not guarantee orderin | [ ]  
BSD_SOCK_PACKET | Obsolete, should not be used | [ ]  
  
_**Return Values:**_
  
* 0 - Socket ID would be returned on successful socket creation.
* -1 - failure, ErrNo is set with additional details when available
  
_**Error Number (ErrNo) Possible Values:**_
  
* EAFNOSUPPORT ? domain or type not supported.
* EINVAL ? protocol not supported
* EACCES ? Access Denied (WINC: Failure to get Socket)
  
## 2. Connect
int BSD_connect(int socket, const struct bsd_sockaddr *name, socklen_t namelen)
  
_**Description:**_
  
This will try to initiate a connection on a socket.  
  
_**Passed Parameters:**_
  
Name | Declaration Type  
------------ | -------------  
socket | int  
*name | const struct sockaddr  
namelen | socklen_t  
  
_**Parameter Descriptions:**_
  
* socket ? Socket value which the BSD API return for use.
* *name ? Structure which contains address family, port number and IP address.
* namelen - Size of the space pointed to by name
  
_**Return Values:**_
  
* 0 - Success.
* -1 - Failure, ErrNo is set with additional details when available
  
_**Error Number (ErrNo) Possible Values:**_
  
* ENOTSOCK ? When the passed argument s is not a socket i.e. s<0. In WINC it falls under WINC_SOCK_ERR_INVALID_ARG error.
* EADDRNOTAVAIL ? The specified address is not available i.e. name == NULL. In WINC it falls under WINC_SOCK_ERR_INVALID_ARG.
* EINVAL ? The namelen argument is not a valid length for the address family. In WINC it falls under WINC_SOCK_ERR_INVALID_ARG.
* EIO ? It?s an input output error and it falls under WINC_SOCK_ERR_INVALID.
  
## 3. Bind
int BSD_bind (int socket, const struct sockaddr *addr, socklen_t   addrlen) 
  
_**Description:**_
  
Assign a local protocol address to a socket. On socket creation with BSD API, an address family space exists, but no protocol address is assigned.  The bind () system call requests that the passed addr be assigned to the socket.
  
_**Passed Parameters:**_
  
Name | Declaration Type  
------------ | -------------  
socket | int  
*addr | const struct sockaddr  
addrlen | socklen_t  
  
_**Parameter Descriptions:**_
  
* socket ? Socket value which the BSD API return for use.
* *addr ? Structure which contains address family, port number and IP address.
* addrlen - Size of the space pointed to by name
  
_**Return Values:**_
  
* 0 - Success.
* -1 - Failure, ErrNo is set with additional details when available
  
_**Error Number (ErrNo) Possible Values:**_
  
* ENOTSOCK ? When the passed argument s is not a socket i.e. s<0. In WINC it falls under WINC_SOCK_ERR_INVALID_ARG error.
* EFAULT ? The addr argument is not in a valid part of the user address space. In WINC its falls under WINC_SOCK_ERR_INVALID_ARG error.
* EINVAL ? The namelen argument is not a valid length for the address family. In WINC it falls under WINC_SOCK_ERR_INVALID_ARG.
* EIO ? It?s an input output error and it falls under WINC_SOCK_ERR_INVALID.
  
## 4. Listen 
int BSD_listen (int socket, int backlog) 
  
_**Description:**_
  
To accept connections, a socket is first created with socket(), a willingness to accept incoming connections and a queue limit for incoming connections are specified with listen (). The listen () system call applies only to sockets of type SOCK_STREAM or SOCK_SEQPACKET. 
  
_**Passed Parameters:**_
  
Name | Declaration Type  
------------ | -------------  
socket | int  
backlog | socklen_t  
  
_**Parameter Descriptions:**_
  
* socket ? Socket value which the BSD API return for use.
* backlog ? Maximum length the queue of pending connections can grow.
  
_**Return Values:**_
  
* 0 - Success.
* -1 - Failure, ErrNo is set with additional details when available
  
_**Error Number (ErrNo) Possible Values:**_
  
* ENOTSOCK ? When the passed argument s is not a socket i.e. s<0. In WINC it falls under WINC_SOCK_ERR_INVALID_ARG error.
* EINVAL ? The namelen argument is not a valid length for the address family. In WINC it falls under WINC_SOCK_ERR_INVALID_ARG.
* EIO ? It?s an input output error and it falls under WINC_SOCK_ERR_INVALID.
  
## 5. Accept
int BSD_accept (int socket, struct sockaddr *addr, socklen_t *addrlen)
  
_**Description:**_
  
Accepts a connection on a socket. 
  
_**Passed Parameters:**_
  
Name | Declaration Type  
------------ | -------------  
socket | int  
backlog | socklen_t  
  
_**Parameter Descriptions:**_
  
* socket ? Socket value which the BSD API return for use.
* *addr ? Structure which contains address family, port number and IP address.
* *addrlen - Size of the space pointed to by name
  
_**Return Values:**_
  
* 0 - Success.
* -1 - Failure, ErrNo is set with additional details when available
  
_**Error Number (ErrNo) Possible Values:**_
  
* ENOTSOCK ? When the passed argument s is not a socket i.e. s<0. In WINC it falls under WINC_SOCK_ERR_INVALID_ARG error.
* EINVAL ? listen() has not been called on the socket descriptor. In WINC it falls under WINC_SOCK_ERR_INVALID_ARG.
  
## 6. Send
int BSD_send(int socket, const void *msg, size_t len, int flags)
  
_**Description:**_
  
The Send API is used to transmit a message only when the socket is in a connected state. If the message is too long to pass atomically through the underlying protocol, the error EMSGSIZE is returned, and the message is not transmitted. 
  
No indication of failure to deliver is implicit in a send. Locally detected errors are indicated by a return value of -1.  
  
The current implementation does not take the flags argument into consideration. The value passed here is ?0? otherwise -1 is returned with EINVAL error set in ?bsdErrorNumber?.
_**Passed Parameters:**_
  
Name | Declaration Type  
------------ | -------------  
socket | int  
*msg | Const void
len | size_t 
flags | int 
  
_**Parameter Descriptions:**_
  
* socket ? Socket value which the BSD API return for use.
* *msg ? Pointer to buffer holding data to be transmitted.
* len - Length of the message.
* flags - There is no flag feature implemented on the WINC.?The value passed here must be ?0?, otherwise -1 is returned with the bsdErrorNumber set to EINVAL error.
  
_**Return Values:**_
  
* The calls return the number of characters sent, or -1 if an error occurred. 
  
_**Error Number (ErrNo) Possible Values:**_
  
These are some standard errors generated by the socket layer. Additional errors may be generated and returned from the underlying protocol modules; see their respective manual pages.
  
* ENOTSOCK ? This error occurs when the argument s is not a socket, i.e. s is less than 0.
* EFAULT ? An invalid user space address is specified for a parameter.
* EMSGSIZE ? The size of the message exceeds the size of the buffer space available for sending the message.
* EAGAIN or EWOULDBLOCK ? This error message is not supported in this implementation.
* ENOBUFS ? This error occurs when the output queue for a network interface is full. 
* EINTR ? This error message is not supported in this implementation. 
* ENOMEM ? This error message is not supported in this implementation. 
* EINVAL ? This error message occurs when a parameter internal to the WINC1500 socket implementation being used by the Send API is invalid. In this case, this error occurs if (gastrSockets[sock].bIsUsed != 1). 
* EPIPE ? This error message is not supported in this implementation. 
  
The BSD_send() internally calls send() function implemented in socket.c file in the WINC1500 library.
  
## 7. Write
int BSD_write (int fd, const void *buf, size_t nbytes) 
  
Writes to a file descriptor. The write method is not supported and would return -1 when called.
  
## 8. Recv
int BSD_recv(int socket, void *msg, size_t len, int flags) 
  
_**Description:**_
  
Receive a message from a socket. The recv call is normally used only on a connected socket.
  
_**Passed Parameters:**_
  
Name | Declaration Type  
------------ | -------------  
socket | int  
*buf | void
len | size_t 
flags | int 
  
_**Parameter Descriptions:**_
  
* socket ? Socket value which the BSD API return for use.
* *buf ? Points to a buffer where the message should be stored.
* len - Specifies the length in bytes of the buffer pointed to by the buffer argument.
* flags - The current implementation does not take the flags argument into consideration. The value passed here is ?0? otherwise -1 is returned with EINVAL error set in bsdErrorNumber.
  
_**Return Values:**_
  
As per BSD standard upon successful completion, recv () shall return the length of the message in bytes. If no messages are available to be received and the peer has performed an orderly shutdown, recv () shall return 0. Otherwise, -1 shall be returned and errno set to indicate the error.  
  
However, we by using WINC?s implementation return a 0 if it's successful or -1 in case of errors. The errors are mapped to BSD standards which is given below. 
  
_**Error Number (ErrNo) Possible Values:**_
  
* ENOTSOCK ? The argument s does not refer to a socket.
* EFAULT ? The receive buffer pointer(s) point outside the process's address space.
* EINVAL ? Invalid argument passed.
* EMSGSIZE ? The socket requires the message to be sent atomically, and the size of the message to be sent made this impossible. In WINC implementation it falls under WINC_SOCK_ERR_INVALID.
* ENOBUFS ? The output queue for a network interface was full. This generally indicates that the interface has stopped sending but may be caused by transient congestion. In WINC?S implementation it falls under SOCK_ERR_BUFFER_FULL.
  
## 9. Recvfrom
int  BSD_recvfrom(int socket, void *buf, size_t len, int flags, struct bsd_sockaddr *from, socklen_t *fromlen)
  
_**Description:**_
  
The recvfrom call is used to receive messages from a socket whether or not it is connection-oriented. In the implementation here it?s used to retrieve data from a UDP socket. The socket must already be bound to a local port before a call to recvfrom function is made.
  
_**Passed Parameters:**_
  
Name | Declaration Type  
------------ | -------------  
socket | int  
*buf | void
len | size_t 
flags | int 
*from | struct bsd_sockaddr
*fromlen | socklen_t
  
_**Parameter Descriptions:**_
  
* socket ? Socket value which the BSD API return for use.
* *buf ? Points to a buffer where the message should be stored
* len - Specifies the length in bytes of the buffer pointed to by the buffer argument.
* flags - The current implementation does not take the flags argument into consideration. The value passed here is ?0? otherwise -1 is returned with EINVAL error set in bsdErrorNumber.
* *from ? If from is non-nil, and the socket is not connection-oriented, the source address of the message is filled in.
* *fromlen ? The fromlen argument is a value result argument, initialized to the size of the buffer associated with from.
  
_**Return Values:**_
  
The recvfrom behaves in the similar way considering the return values by returning either 0 for success or -1 for an error. As per BSD standard upon successful completion, recv () shall return the length of the message in bytes which our implementation doesn?t support now.
  
_**Error Number (ErrNo) Possible Values:**_
  
* ENOTSOCK ? The argument s does not refer to a socket.
* EFAULT ? The receive buffer pointer(s) point outside the process's address space.
* EINVAL ? Invalid argument passed.
* EMSGSIZE ? The socket requires the message to be sent atomically, and the size of the message to be sent made this impossible. In WINC implementation it falls under WINC_SOCK_ERR_INVALID.
* ENOBUFS ? The output queue for a network interface was full. This generally indicates that the interface has stopped sending but may be caused by transient congestion. In WINC?S implementation it falls under SOCK_ERR_BUFFER_FULL.
  
## 10. Read
int BSD_read(int fd, void *buf, size_t nbytes)
  
_**Description:**_
  
The read method reads from a file descriptor. The read method is currently not supported by WINC adapter, -1 will always be returned when called.
  
## 11. Close
int BSD_close(int socket)
  
_**Description:**_
  
The close API closes a socket descriptor, so that it no longer refers to any socket that may be reused.  The resources associated with the socket descriptor are freed.  
  
_**Passed Parameters:**_
  
Name | Declaration Type  
------------ | -------------  
socket | int  
  
_**Parameter Descriptions:**_
  
* socket ? Socket value which the BSD API return for use.
  
_**Return Values:**_
  
* 0 - Success.
* -1 - Failure, ErrNo is set with additional details when available
  
The recvfrom behaves in the similar way considering the return values by returning either 0 for success or -1 for an error. As per BSD standard upon successful completion, recv () shall return the length of the message in bytes which our implementation doesn?t support now.
  
_**Error Number (ErrNo) Possible Values:**_
  
* EBADF ? The argument s does not refer to a socket.
* EIO ? The receive buffer pointer(s) point outside the process's address space.

## 12. Poll
int BSD_poll(struct pollfd *ufds, nfds_t nfds, int timeout)
  
_**Description:**_
  
The poll () system call examines a set of file descriptors to see if some of them are ready for I/O. The poll method is not supported and would return -1 when called.
  
## 13. Setsockopt
int BSD_setsockopt(int socket, int level, int optname, const void *optval, socklen_t optlen)
  
_**Description:**_
  
The setsockopt() call manipulates the options  associated with a socket.  Options may exist at multiple protocol levels; they are always present at the uppermost ``socket'' level. 
  
When manipulating socket options, the level at which the option resides, and the name of the option must be specified.  To manipulate options at the socket level, level is specified as SOL_SOCKET.  To manipulate options at any other level the protocol number of the appropriate protocol controlling the option is supplied.  For example, to indicate that an option is to be interpreted by the TCP protocol, level should be set to the protocol number of TCP.

_**Passed Parameters:**_
  
Name | Declaration Type  
------------ | -------------  
socket | int  
level | int
optname | int
*optval | const void 
optlen | socklen_t
  
_**Parameter Descriptions:**_
  
* socket ? Socket value which the BSD API return for use.
* level ? Protocol level.
* optname ? Socket options.
* *optval ? To access option value
* optlen ? Size of buffer pointer by optval.
  
_**Return Values:**_
  
* 0 - Success.
* -1 - Failure.
  
_**Error Number (ErrNo) Possible Values:**_
  
* ENOTSOCK ? The argument socket does not refer to a socket. In WINC its falls under WINC_SOCK_ERR_INVALID_ARG.
* EFAULT ? The address pointed to by optval is not in a valid part of the process address space. For getsockopt, this error may also be returned if optlen is not in a valid part of the process address space. In WINC its falls under WINC_SOCK_ERR_INVALID_ARG.
* EINVAL ? Invalid argument passed. In WINC its falls under WINC_SOCK_ERR_INVALID_ARG.
* EIO ? It's an input output error and it falls under WINC_SOCK_ERR_INVALID.
  
NOTE: The select feature is currently not implemented.
  
## 14. Getsockopt
int BSD_getsockopt(int socket, int level, int optname, void *optval, socklen_t *optlen)
  
_**Description:**_
  
The getsockopt() call gets the options  associated with a socket. This feature is not supported now and would return -1 upon calling. Also, bsdErrorNumber would be set to ENOSYS.
  
## 15. htons
uint32_t BSD_htons(uint32_t hostshort)
  
_**Description:**_
  
The htons() function converts the unsigned short integer hostshort from host byte order to network byte order.
 
_**Passed Parameters:**_
Name | Declaration Type  
------------ | -------------  
hostshort | uint32_t 
   
_**Parameter Descriptions:**_
  
* hostshort ? The bytes in host byte order. In case of a big-endian system, the most significant value in the sequence is stored at the lowest storage address (i.e., first).
  
_**Return Values:**_
  
Bytes in network byte order.
  
## 16. htonl
uint32_t BSD_ htonl(uint32_t hostlong)
  
_**Description:**_
  
The htonl() function converts the unsigned integer hostlong from host byte order to network byte order.
 
_**Passed Parameters:**_
Name | Declaration Type  
------------ | -------------  
hostlong | uint32_t 
   
_**Parameter Descriptions:**_
  
* hostlong ? The bytes in host byte order. In case of a big-endian system, the most significant value in the sequence is stored at the lowest storage address (i.e., first).
  
_**Return Values:**_
  
Bytes in network byte order.
  
## 17. ntohs
uint32_t BSD_ntohs(uint32_t netshort)
  
_**Description:**_
  
The ntohs() function converts the unsigned short integer netshort from network byte order to host byte order.
 
_**Passed Parameters:**_
Name | Declaration Type  
------------ | -------------  
netshort | uint32_t 
   
_**Parameter Descriptions:**_
  
* netshort ? The bytes in network byte order. In case of a big-endian system, the most significant value in the sequence is stored at the lowest storage address (i.e., first). 
  
_**Return Values:**_
  
Bytes in network byte order.
  
## 18. ntohl
uint32_t BSD_ntohl(uint32_t netlong)
  
_**Description:**_
  
The ntohl() function converts the unsigned integer netlong from network byte order to host byte order.
  
_**Passed Parameters:**_
Name | Declaration Type  
------------ | -------------  
netlong | uint32_t 
   
_**Parameter Descriptions:**_
  
* netlong ? The host byte ordered bytes. In case of a big-endian system, the most significant value in the sequence is stored at the lowest storage address (i.e., first).
  
_**Return Values:**_
  
Bytes in network byte order.
  
## 19. sendto
int  BSD_sendto(int socket, const void *msg, size_t len, int flags, const struct bsd_sockaddr *to, socklen_t tolen)
  
_**Description:**_
  
The sendto function is used to transmit one or more messages to another socket. It can be used at any time regardless of whether the socket is connected state or not.   
  
_**Passed Parameters:**_
  
Name | Declaration Type  
------------ | -------------  
socket | int  
*msg | const void
len | size_t
flags | int
*to | const struct bsd_sockaddr
tolen | socklen_t
  
_**Parameter Descriptions:**_
  
* socket ? Socket value which the BSD API return for use.
* *msg ? Pointer to buffer holding data to be transmitted
* len ? Length of the message
* flags ? The flags feature is not implemented. The value passed here is ?0? otherwise -1 is returned with EINVAL error set in bsdErrorNumber.
* *to ? Address of the target
* tolen ? Length of the addressed target pointed by *to 
  
_**Return Values:**_
  
The calls return the number of characters sent, or -1 if an error occurred.
  
_**Error Number (ErrNo) Possible Values:**_
  
* ENOTSOCK ? When the passed argument socket (i.e. socket < 0) is not a socket. In WINC it falls under WINC_SOCK_ERR_INVALID_ARG error.
* EFAULT ? The addr argument is not in a valid part of the user address space.
* EINVAL ? Invalid argument is passed. In WINC its falls under WINC_SOCK_ERR_INVALID_ARG.
* EMSGSIZE ? The socket requires the message to be sent atomically, and the size of the message to be sent made this impossible. In WINC implementation it falls under WINC_SOCK_ERR_INVALID.
* ENOBUFS ? The output queue for a network interface was full. This generally indicates that the interface has stopped sending but may be caused by transient congestion. In WINC?S implementation it falls under SOCK_ERR_BUFFER_FULL.

# ADAPTER LAYER APIs

The below APIs are specific to the adapter layer on the WINC and are not part of the BSD standard. These APIs bind the adapter layer and WINC.

## 1. BSD_GetSocketState
int BSD_socket(int domain, int type, int protocol)

_**Description:**_

Returns the socket state of the corresponding passed socket.

_**Passed parameter:**_

Name | Declaration Type  
------------ | -------------  
sock  | int

_**Parameter Descriptions:**_
* sock	- Socket value which the BSD API return for use.

_**Return Value:**_
sockState ? holds the socket state

## 2. BSD_SocketHandler
void BSD_SocketHandler(int8_t sock, uint8_t msgType, void *pMsg)

_**Description:**_

Manages the socket. This method is registered in the WINC1500 driver?s callback ?registerSocketCallBack? to manage socket events. The ?BSD_SocketHandler? function needs to be passed as the first argument to the ?registerSocketCallBack? function. Currently, the supported states are: SOCKET_MSG_CONNECT, SOCKET_MSG_SEND, SOCKET_RECV and SOCKET_MSG_RECVFROM.

_**Passed Parameters:**_

Name | Declaration Type  
------------ | -------------  
sock  | int8_t
msgType  | uint8_t
*pMsg  | void

_**Parameter Descriptions:**_
* sock - Socket value which the BSD API return for use.
* msgType - Socket message type
* *pMsg - Pointer to the socket message 

## 3. BSD_SetRecvHandlerTable
void BSD_SetRecvHandlerTable(packetReceptionHandler_t *appRecvInfo)

_**Description:**_

An instance of ?packetReceptionHandler_t? should be created for sending the packets received over a socket to a reception handler defined for the corresponding socket by the user application. 

_**Passed Parameters:**_

Name | Declaration Type  
------------ | -------------  
*appRecvInfo | packetReceptionHandler_t
  
_**Parameter Description:**_
* *appRecvInfo	- Pointer to the receive handler table defined by the application for sending received packets over a socket

## 4. BSD_GetRecvHandlerTable
packetReceptionHandler_t* BSD_GetRecvHandlerTable(void)

_**Description:**_
  
This method returns the location of the packetReceptionHandler table(?packetReceptionHandler_t?) created in the user application. It?s the user?s responsibility to direct the data received over each socket to the corresponding socket reception handler.

_**Return Value:**_
Returns the pointer to the packetReceptionHandler table.

## 5. BSD_GetErrNo
bsdError_t BSD_GetErrNo(void)

_**Description:**_

Returns the BSD error number, which can be used to get more specific details upon a BSD_ERROR returned by a BSD Standard API function request.

_**Return Values:**_

Returns the BSD specific error ?bsdErrorNumber?.
