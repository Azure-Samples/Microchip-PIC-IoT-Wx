# Provisioning the Microchip PIC-IoT Wx Development Board for Azure IoT Services

## Introduction

 This document describes how to connect the PIC-IoT Wx Development Board (featuring a 16-bit PIC24F MCU, ATECC608A secure element, and ATWINC1510 Wi-Fi module) to Azure IoT Central and/or Hub which leverages Microsoft’s Azure IoT Embedded C SDK. The PIC-IoT Wx Development Board will be provisioned for use with Azure IoT services using self-signed X.509 certificate-based authentication.

<img src=".//media/image1.png" />

## Table of Contents

- [Introduction](#introduction)
- [Background Knowledge](#background-knowledge)
  - [PIC-IoT Wx Development Board Overview & Features](#pic-iot-wx-development-board-overview--features-smart--connected--secure)
  - [Microchip “Provisioning” vs. Microsoft “Provisioning”](#microchip-provisioning-vs-microsoft-provisioning)
  - [High Level Architecture between the Client (PIC-IoT) and the Cloud (Azure)](#high-level-architecture-between-the-client-pic-iot-and-the-cloud-azure)
  - [Azure IoT Embedded C SDK](#azure-iot-embedded-c-sdk)
  - [TLS Connection](#tls-connection)
  - [MQTT Connection](#mqtt-connection)
- [Programming the PIC-IoT Wx Development Board](#programming-the-pic-iot-wx-development-board)
  - [1. Install the Development Tools](#1-install-the-development-tools)
  - [2. Connect to Azure IoT Services](#2-connect-device-to-azure-iot-services)
      - a. IoT Central (`Preferred Method`)
      - b. IoT Hub
- [References](#references)
- [Conclusion](#conclusion)

## Background Knowledge

### PIC-IoT Wx Development Board Overview & Features (SMART \| CONNECTED \| SECURE)

<img src=".//media/image2.png"/>

 Download the [PIC-IoT Wx HW User Guide](http://ww1.microchip.com/downloads/en/DeviceDoc/PIC-IoT-Wx-Hardware-User-Guide-DS50002964A.pdf) for more details

### Microchip “Provisioning” vs. Microsoft “Provisioning”

The term “provisioning” will be use throughout this document (e.g. IoT Provisioning Tool, provisioning key, provisioning device, device provisioning service, etc.). On the Microchip side, the provisioning process is to securely inject certificates into the hardware. From the
context of Microsoft, provisioning is defined in the relationship between the hardware and the cloud, Azure. [Azure IoT Hub Device Provisioning Service (DPS)](https://docs.microsoft.com/azure/iot-dps/#:~:text=The%20IoT%20Hub%20Device%20Provisioning%20Service%20%28DPS%29%20is,of%20devices%20in%20a%20secure%20and%20scalable%20manner.)
allows the hardware to be provisioned securely to the right IoT Hub.

<img src=".//media/image3.png"/>

### High Level Architecture between the Client (PIC-IoT) and the Cloud (Azure)

This high-level architecture description summarizes the interactions between the PIC-IoT board and Azure. These are the major puzzle pieces that make up this enablement work of connecting PIC-IoT Wx Developoment Board to Azure through DPS using X.509-based authentication:

- [ATECC608A](https://www.microchip.com/wwwproducts/en/ATECC608A): a secure element from the Microchip CryptoAuthentication portfolio. It securely stores a private key that is used to authenticate the hardware with cloud providers to uniquely identify every board

- [ATWINC1510](https://www.microchip.com/wwwproducts/en/ATWINC1510): a low-power consumption Wi-Fi module that has access to the device certificate, signer CA certificate, and public key for mutual TLS handshaking between the board and the cloud

- [IoT Provisioning Tool](https://www.microchip.com/design-centers/internet-of-things/iot-dev-kits/iot-provision-tool): Microchip-provided tool for provisioning self-signed certificate utilizing the unique serial number and private key stored in the ATECC608A secure element

- [Azure IoT Embedded C SDK](https://azure.microsoft.com/en-us/updates/embedded-c-sdk-general-availability/): Microsoft-provided API designed to allow small, low-cost embedded IoT devices to communicate with Azure services, serving as translation logic between the application code and transport client

- [Azure IoT Central](https://docs.microsoft.com/en-us/azure/iot-central/core/overview-iot-central): IoT Central is an IoT application platform that reduces the burden and cost of developing, managing, and maintaining enterprise-grade IoT solutions

- [Azure IoT Hub](https://docs.microsoft.com/en-us/azure/iot-hub/about-iot-hub): IoT Hub is a managed service, hosted in the Cloud, that acts as a central message hub for bi-directional communication between your IoT application and the devices it manages

- [Device Provisioning Service (DPS)](https://docs.microsoft.com/en-us/azure/iot-dps/): a helper service for IoT Hub that enables zero-touch, just-in-time provisioning to the right IoT hub without requiring human intervention, allowing customers to automatically provision millions of devices in a secure and scalable manner

 <img src=".//media/image4.png"/>

In a nutshell, we will use Microchip’s IoT Provisioning Tool to send a Certificate Signing Request (CSR) to the ATECC608A to generate a self-signed certificate chain which is then obtained by the ATWINC1510 Wi-Fi module to perform a TLS mutual handshake between the client (PIC-IoT board) and the server (Azure), specifically using DPS.

On successful authentication, the PIC-IoT board will be provisioned to the correct IoT Hub that is pre-linked to DPS during the setup process. We can then leverage the Azure IoT Central or IoT Explorer (web and graphical tools used for interacting with and testing your IoT devices). Note that the ATECC608A only contains the private key. The self-signed certificate chain including root CA, signer CA (or intermediate CA), and device is stored in the ATWINC1510 Wi-Fi module used for the TLS handshake.

### Azure IoT Embedded C SDK

This is the high-level view of the Embedded C SDK which translates the application code into an Azure-friendly logic that can be easily understood by Azure IoT Hub. Note that Microsoft is only responsible for the logic in the green box; it is up to the IoT Developer to provide the remaining layers of application code, Transport Client, TLS, and Socket. In the provided demo project, Microchip provides the layers in blue. Please reference the [Azure SDK for Embedded C](https://github.com/Azure/azure-sdk-for-c/tree/78a280b7160201cf10a106e8499e03eec88ea582) document for more details.

<img src=".//media/image7.png" style="width:4in;height:4in"/>

### TLS connection

The TLS connection performs both authentication and encryption.
Authentication consists of two parts:

- Server authentication; the board authenticates the server
- Client authentication; the server authenticates the board

Server authentication happens transparently to the user since the ATWINC1510 on the PIC-IoT  board comes preloaded with the required CA certificate. During client authentication the client private key must be used, but since this is stored inside the ATECC608A chip and cannot be extracted, all calculations must be done inside the ATECC608A. The ATWINC1510 library offers an API to delegate the TLS calculations to the main application. The main application will in turn call the ATECC608A library APIs to perform the calculations. Before the TLS connection is complete, a shared secret key must be negotiated between the server and the client. This key is used to encrypt all future communications during the connection.

### MQTT Connection

After successfully connecting on the TLS level, the board starts establishing the MQTT connection. Since the TLS handles authentication and security, MQTT does not have to provide a username or password.

## Programming the PIC-IoT Wx Development Board

### 1. Install the Development Tools

Embedded software development tools from Microchip need to be pre-installed in order to properly program the PIC-IoT Wx Development Board and provision it for use with Microsoft Azure IoT services.

Click this link for the setup procedure: [Development Tools Installation](./Dev_Tools_Install.md)

### 2. Connect Device to Azure IoT Services

- a. IoT Central (`Preferred Method`)

    [Azure IoT Central](https://docs.microsoft.com/en-us/azure/iot-central/core/overview-iot-central) is an IoT application platform that reduces the burden and cost of developing, managing, and maintaining enterprise-grade IoT solutions. Choosing to build with IoT Central gives you the opportunity to focus time, money, and energy on transforming your business with IoT data, rather than just maintaining and updating a complex and continually evolving IoT infrastructure.

    The web UI lets you quickly connect devices, monitor device conditions, create rules, and manage millions of devices and their data throughout their life cycle. Furthermore, it enables you to act on device insights by extending IoT intelligence into line-of-business applications.

    Click this link for the programming procedure: [Connecting the PIC-IoT Wx Development Board to Azure IoT Central](./PIC_IoT_Central.md)

- b. IoT Hub

    [Azure IoT Hub](https://docs.microsoft.com/en-us/azure/iot-hub/about-iot-hub) is a managed service, hosted in the cloud, that acts as a central message hub for bi-directional communication between your IoT application and the devices it manages. You can use Azure IoT Hub to build IoT solutions with reliable and secure communications between millions of IoT devices and a cloud-hosted solution backend. You can connect virtually any device to IoT Hub.

    IoT Hub supports communications both from the device to the cloud and from the cloud to the device. IoT Hub supports multiple messaging patterns such as device-to-cloud telemetry, file upload from devices, and request-reply methods to control your devices from the cloud. IoT Hub monitoring helps you maintain the health of your solution by tracking events such as device creation, device failures, and device connections.

    IoT Hub's capabilities help you build scalable, full-featured IoT solutions such as managing industrial equipment used in manufacturing, tracking valuable assets in healthcare, and monitoring office building usage.

    Click this link for the programming procedure: [Connecting the Microchip PIC-IoT Wx Development Board to Azure IoT Hub](./PIC_IoT_Hub.md)

## References

Refer to the following links for additional information for IoT Explorer, IoT Hub, DPS, Plug and Play model, and IoT Central

•	[Manage cloud device messaging with Azure-IoT-Explorer](https://github.com/Azure/azure-iot-explorer/releases)

•	[Import the Plug and Play model](https://docs.microsoft.com/en-us/azure/iot-pnp/concepts-model-repository)

•	[Configure to connect to IoT Hub](https://docs.microsoft.com/en-us/azure/iot-pnp/quickstart-connect-device-c)

•	[How to use IoT Explorer to interact with the device](https://docs.microsoft.com/en-us/azure/iot-pnp/howto-use-iot-explorer#install-azure-iot-explorer)

•	[Create an Azure IoT Central application](https://docs.microsoft.com/en-us/azure/iot-central/core/quick-deploy-iot-central)

•	[Manage devices in your Azure IoT Central application](https://docs.microsoft.com/en-us/azure/iot-central/core/howto-manage-devices)

•	[How to connect devices with X.509 certificates for IoT Central](https://docs.microsoft.com/en-us/azure/iot-central/core/how-to-connect-devices-x509)

•	[Configure the IoT Central application dashboard](https://docs.microsoft.com/en-us/azure/iot-central/core/howto-add-tiles-to-your-dashboard)

•	[Customize the IoT Central UI](https://docs.microsoft.com/en-us/azure/iot-central/core/howto-customize-ui)

## Conclusion

You are now able to connect PIC-IoT to Azure IoT services using self-signed X.509 certificate-based authentication and have deeper knowledge of how all the pieces of the puzzle fit together from the ATECC608A secure element, ATWINC1510 Wi-Fi, Azure Embedded C SDK, and Azure IoT Hub/Central/DPS. Let’s start thinking out of the box and see how you can apply this project to provision securely and quickly a massive number of Microchip devices to Azure and safely manage them through the entire product life cycle.
