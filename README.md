# Repurpose PIC-IoT Wx Development Board to Connect to Azure through IoT Hub Device Provisioning Service (DPS) with "Plug and Play" Model Interface

## Introduction

 This document describes how to connect the PIC-IoT Wx Development Board (featuring a 16-bit PIC24F MCU, ATECC608A secure element, and ATWINC1510 Wi-Fi module) to an Azure IoT Hub via Device Provisioning Service (DPS) while leveraging Microsoft’s Azure IoT Embedded C SDK. The PIC-IoT is provisioned for use with Azure using self-signed X.509 certificate-based authentication.

<img src=".//media/image1.png" />

## Table of Contents

- [Repurpose PIC-IoT Wx Development Board to Connect to Azure through IoT Hub Device Provisioning Service (DPS)](#repurpose-pic-iot-wx-development-board-to-connect-to-azure-through-iot-hub-device-provisioning-service-dps)
  - [Introduction](#introduction)
  - [Table of Contents](#table-of-contents)
  - [Background Knowledge](#background-knowledge)
    - [PIC-IoT Wx Development Board Overview & Features (SMART \| CONNECTED \| SECURE)](#pic-iot-wx-development-board-overview--features-smart--connected--secure)
    - [Microchip “Provisioning” vs. Microsoft “Provisioning”](#microchip-provisioning-vs-microsoft-provisioning)
    - [High Level Architecture between the Client (PIC-IoT) and the Cloud (Azure)](#high-level-architecture-between-the-client-pic-iot-and-the-cloud-azure)
    - [Azure IoT Embedded C SDK](#azure-iot-embedded-c-sdk)
    - [TLS connection](#tls-connection)
    - [MQTT Connection](#mqtt-connection)
  - [Checklist](#checklist)
  - [Procedure](#instruction)
    - [Section 1: Prepare your development environment](#step-1-prepare-your-development-environment)
      - [1. Set up Microchip’s MPLAB X IDE Tool Chain](#1-set-up-microchips-mplab-x-ide-tool-chain)
      - [2. Set up Azure cloud resources](#2-set-up-azure-cloud-resources)
      - [3. Set up Git](#3-set-up-git)
    - [Section 2: Prepare your PIC-IoT board to connect to Azure](#step-2-prepare-your-pic-iot-board-to-connect-to-azure)
    - [Section 3: Enroll device into DPS](#step-3-enroll-device-into-dps)
      - [1. Preparing your environment for the certification verifying process:](#1-preparing-your-environment-for-the-certification-verifying-process)
      - [2. In Azure portal, upload the root CA cert “root-ca.pem” in DPS and do proof-of-possession for X.509 CA certificates with your Device Provisioning Service](#2-in-azure-portal-upload-the-root-ca-cert-root-capem-in-dps-and-do-proof-of-possession-for-x509-ca-certificates-with-your-device-provisioning-service)
        - [a. Register the public part of an X.509 certificate and get a verification code](#a-register-the-public-part-of-an-x509-certificate-and-get-a-verification-code)
        - [b. Digitally sign the verification code to create a verification certificate](#b-digitally-sign-the-verification-code-to-create-a-verification-certificate)
        - [c. Upload the signed verification certificate to DPS](#c-upload-the-signed-verification-certificate-to-dps)
        - [STOP! Quick summary of this step](#stop-quick-summary-of-this-step)
      - [3. Add a new enrollment group using the signer-ca.pem file](#3-add-a-new-enrollment-group-using-the-signer-capem-file)
      - [STOP! Sanity checks:](#stop-sanity-checks)
    - [Section 4: Connect the PIC-IoT device to Azure](#step-4-connect-the-pic-iot-device-to-azure)
    - [Section 5: Verify the connection between PIC-IoT and Azure](#step-5-verify-the-connection-between-pic-iot-and-azure)
    - [Section 6: View PIC-IoT board telemetry on Azure IoT Explorer](#step-6-view-pic-iot-board-telemetry-on-azure-iot-explorer)
  - [Future Considerations](#further-consideration)
  - [Conclusion](#conclusion)

## Background Knowledge

### PIC-IoT Wx Development Board Overview & Features (SMART \| CONNECTED \| SECURE)

<img src=".//media/image2.png"/>

 Download the [PIC-IoT Wx HW User Guide](http://ww1.microchip.com/downloads/en/DeviceDoc/PIC-IoT-Wx-Hardware-User-Guide-DS50002964A.pdf) for more details

### Microchip “Provisioning” vs. Microsoft “Provisioning”

The term “provisioning” will be use throughout this document (e.g. IoT
Provisioning Tool, provisioning key, provisioning device, device
provisioning service, etc.). On the Microchip side, the provisioning
process is to securely inject certificates into the hardware. From the
context of Microsoft, provisioning is defined in the relationship
between the hardware and the cloud, Azure. [Azure IoT Hub Device
Provisioning Service
(DPS)](https://docs.microsoft.com/azure/iot-dps/#:~:text=The%20IoT%20Hub%20Device%20Provisioning%20Service%20%28DPS%29%20is,of%20devices%20in%20a%20secure%20and%20scalable%20manner.)
allows the hardware to be provisioned securely to the right IoT Hub.

<img src=".//media/image3.png"/>

### High Level Architecture between the Client (PIC-IoT) and the Cloud (Azure)

This high-level architecture description summarizes the interactions
between the PIC-IoT board and Azure. These are the top six major puzzle
pieces that make up this enablement work of connecting PIC-IoT to Azure
through DPS using X.509-based authentication:

- ATECC608A: a secure element from the Microchip CryptoAuthentication
    portfolio. It securely stores a private key that is used to
    authenticate the hardware with cloud providers to uniquely identify
    every board <https://www.microchip.com/wwwproducts/en/ATECC608A>

- ATWINC1510: a low-power consumption Wi-Fi module that has access to
    the device certificate, signer CA certificate, and public key for
    mutual TLS handshaking between the board and the cloud
    <https://www.microchip.com/wwwproducts/en/ATWINC1510>

- IoT Provisioning Tool: Microchip-provided tool for provisioning
    self-signed certificate utilizing the unique serial number and
    private key stored in the ATECC608A secure element.

- Azure IoT Embedded C SDK: Microsoft-provided API designed to
    allow small, low-cost embedded IoT devices to communicate with Azure
    services, serving as translation logic between the application code
    and transport client

- Azure IoT Hub: IoT Hub is a managed service, hosted in the cloud,
    that acts as a central message hub for bi-directional communication
    between your IoT application and the devices it manages

- Device Provisioning Service (DPS): a helper service for IoT Hub that
    enables zero-touch, just-in-time provisioning to the right IoT hub
    without requiring human intervention, allowing customers to
    provision millions of devices in a secure and scalable manner

 <img src=".//media/image4.png"/>

In a nutshell, we will use Microchip’s IoT Provisioning Tool to send a
Certificate Signing Request (CSR) to the ATECC608A to generate a
self-signed certificate chain which is then obtained by the ATWINC1510
Wi-Fi module to perform a TLS mutual handshake between the client
(PIC-IoT board) and the server (Azure), specifically using DPS.

Once successful, the PIC-IoT board will be provisioned to the correct
IoT Hub that is pre-linked to DPS during the setup process. We can then
leverage the Azure IoT Explorer which is a graphical tool for
interacting with and testing your IoT devices. Note that the ATECC608A
only contains the private key. The self-signed certificate chain
including root CA, signer CA (or intermediate CA), and device CA is
stored in the ATWINC1510 Wi-Fi module used for the TLS handshake.

### Azure IoT Embedded C SDK

This is the high-level view of the Embedded C SDK which translates the application code into an Azure-friendly logic that can be easily understood by Azure IoT Hub. Note that Microsoft is only responsible for the logic in the green box; it is up to the IoT Developer to provide the remaining layers of application code, Transport Client, TLS, and Socket. In the provided demo project, Microchip provides the layers in blue. Please the [Azure SDK for Embedded C](https://github.com/Azure/azure-sdk-for-c/tree/78a280b7160201cf10a106e8499e03eec88ea582) document for more details.

<img src=".//media/image7.png" style="width:2in;height:2in"/>

### TLS connection

The TLS connection performs both authentication and encryption.
Authentication consists of two parts:

- Server authentication; the board authenticates the server
- Client authentication; the server authenticates the board

Server authentication happens transparently to the user since the
ATWINC1510 on the PIC-IoT board comes preloaded with the required CA
certificate. During client authentication the client private key must be
used, but since this is stored inside the ATECC608A chip and cannot be extracted, all calculations must be done inside the ATECC608A. The ATWINC1510 library offers an API to delegate the TLS calculations to the main application. The main
application will in turn call the ATECC608A library API’s to perform the
calculations. Before the TLS connection is complete, a shared secret key
must be negotiated between the server and the client. This key is used
to encrypt all future communications during the connection.

### MQTT Connection

After successfully connecting on the TLS level, the board starts
establishing the MQTT connection. Since the TLS handles authentication
and security, MQTT does not have to provide a username or password.

## Checklist

Here are major steps of this project. Track your progress using this
list as you complete each stage:

<img src=".//media/image5.png"/>

## Procedure

### Section 1: Prepare your development environment

#### 1. Set up Microchip’s MPLAB X IDE Tool Chain

- [MPLAB X IDE V5.30 or
    later](https://www.microchip.com/mplab/mplab-x-ide)

- [XC16 Compiler v1.50 or
    later](https://www.microchip.com/mplab/compilers)

- MPLAB Code Configurator 3.95 or later (once you finish the
    installation of the previous items, launch MPLAB X IDE &gt; click on
    Tools &gt; Plugins Download &gt; search for MPLAB Code Configurator
    and install it)

    <img src=".//media/image10.png" style="width:4.17917in;height:2.98149in"/>

#### 2. Set up Azure Cloud resources

- [Create an Azure free account for 30 day
    trial](https://azure.microsoft.com/en-us/free/)    
    As a result, you should be able to access to Azure Portal by the end of this section.

- [Set up Azure IoT Hub and Device Provisioning Service (DPS) in Azure
    Portal](https://docs.microsoft.com/azure/iot-dps/quick-setup-auto-provision)    
    As a result, you should be able to 1) create an IoT Hub, 2) create a Device Provisioning Service, and 3) link your DPS to your IoT Hub. 

-  [Install Azure IoT Explorer](https://docs.microsoft.com/azure/iot-pnp/howto-install-iot-explorer#install-azure-iot-explorer) (make sure to download \*.msi file of the release 0.12.1 or later). The Azure IoT explorer is a graphical tool for interacting with and testing your IoT device on Azure. View [this GitHub](https://github.com/Azure/azure-iot-explorer) for more details.

#### 3. Set up Git

- Install latest version of [Git](https://git-scm.com/download/win)
- Verify that you have accessed to Git Bash by going to Start menu > type "Git Bash", a window like this will pop-out: 

    <img src=".//media/image15.png"/>

### Section 2: Prepare your PIC-IoT board to Connect to Azure

This section serves two purposes:

1. Update the internal firmware of the ATWINC1510 Wi-Fi module on the PIC-IoT to enable mutual TLS handshake between client’s ECC and server’s RSA. 

2. Provision the PIC-IoT board by securely inject certificates into the hardware. Perform this section to create a self-signed certificate chain which acts as a device unique ID to enroll into DPS. The final result is the ATWINC1510 obtains the device certificate, CA certificate, and CA public key. The device certificate is based on a key pair that has previously been generated and is already pre-programmed in the ATECC608A. The device certificate is the client certificate used by the TLS layer for the client authentication (“device” and “client” are used interchangeably in this document). The end goals of the provisioning process include the following:
    - The PC running the Python script requests and receives a Certificate Signing Request (CSR) based on the key pair stored in the ATECC608A
    - The signer CA generates the certificate and returns it alongside with its own certificate and public key to the MCU; the two (signer and device) certificates are stored in the ATWINC1510

<img src=".//media/image11.png"/>

Procedure:

1. Download the [Microchip IoT Provisioning Tool v1.4.0](http://www.microchip.com/mymicrochip/filehandler.aspx?ddocname=en1001525)

2. Unzip the downloaded folder and follow the instructions in the "README.txt" file to perform WINC FW upgrade and board provisioning. 

   - Launch PowerShell: click on Start > type “PowerShell” in the Search field > Open
   - Go to the directory where the “iotprovision-azure” executable file resides
   - Type the following command (this should generate and write all the certs to your disk):

        ```shell
        iotprovision-bin.exe -c azure -m custom
        ```

3. Take note of the generated cert location mentioned in the output message starting with “Saving to your …[your path]\.microchip-iot\MCHP3261021800001185.”. These certs will be used in IoT Hub DPS enrollment in the next step:
    - root-ca.crt (or .key): self-signed root CA cert
    - signer-ca.crt (or .key) (aka. intermediate CA) is a uniquely generated by the root cert, which is then used to generate device cert in [your-path]\.microchip-iot\MCHP<xxxxxxxxxxxxxxxx>\device.crt


**STOP! Knowledge backfill:** to understand the steps approaching
in the upcoming session, please take some time review the document
[Conceptual understanding of X.509 CA certificates in the IoT
industry](https://docs.microsoft.com/azure/iot-hub/iot-hub-x509ca-concept)
which describes the value of using X.509 certificate authority (CA)
certificates in IoT device manufacturing and authentication to IoT
Hub.

<img src=".//media/image14.png" />

### Section 3: Enroll Device into DPS

#### 1. Set up certificates for the verification process:

- Go to generated cert location in the previous step at `\[your
    path]\.microchip-iot`

- Copy `*.crt` files and rename them to `*.pem`. (Note: DPS only accepts
    \*.pem or \*.cer file formats. If choosing \*.cer file, only base-64
    encoded certificate)

#### 2. In Azure portal, upload the root CA cert “root-ca.pem” in DPS and do proof-of-possession for X.509 CA certificates with your Device Provisioning Service

##### a. Register the public part of an X.509 certificate and get a verification code

Follow the “Register the public part of an X.509 certificate and get a verification code” section in [this document](https://docs.microsoft.com/azure/iot-dps/how-to-verify-certificates).
Again, the verification code is generated by encrypting the public key
portion of your X.509 certification. It will be used to validate the
uploaded certificate ownership. So make sure to copy the generated
verification code to notepad for next step.

##### b. Digitally sign the verification code to create a verification certificate

 Now that you've registered your root CA with Azure IoT Hub, you'll
need to prove that you actually own it by:

1. Generating the Certificate Signing Request (CSR) using the
    verification code.
2. Generating a verification certificate using CSR above

Once this done, you can upload your verification certificate to DPS to finish the [proof of possession](https://tools.ietf.org/html/rfc5280#section-3.1).

Procedure:

1. Open the Git Bash: Start menu &gt; type “Git Bash”, a window like this will pop out:

    <img src=".//media/image15.png" style="width:3.21739in;height:0.94745in" alt="A picture containing ball, clock Description automatically generated" />

2. Change to your generated certification folder:

    ```bash
    cd drive\[your path\.microchip-iot

    Example: cd /C/Users/john5/Azure/.microchip-iot
    ```

3. **Generate a certification signing request (CSR)** by entering the below command. CSR is generated by encrypting these two key inputs:

    - The `root-ca.key` generated in previous step by Microchip IoT Provisioning tool. This will be used in the openssl command line below.
    - The `Verification Code` generated from DPS in previous step. You will be asked to provide this during the process of creating the CSR.

    **Note**: Once you enter to command below, you will then be asked to enter information that will be used to will be incorporated into your certificate request. Enter verification code (generated from Azure portal) when prompt for CommonName. For the rest, you can enter anything you want.

    ```bash
    openssl req -new -key root-ca.key -out azure_root_ca_verification.csr
    ```

    As the result, you should see the file `azure_root_ca_verification.csr` appear in the \\.microchip-iot folder: 

    <img src=".//media/image16.png" style="width:5.12286in;height:2.47003in" alt="A screenshot of a cell phone Description automatically generated" />

4. Generate a verification certificate by executing the following command:

    ```bash
    openssl x509 -req -in azure_root_ca_verification.csr -CA root-ca.crt -CAkey root-ca.key -CAcreateserial -out azure_signer_verification.cer -days 365 -sha256
    ```

    As the result, you should see the file `azure_signer_verification.cer` appear in the \\.microchip-iot folder:

    <img src=".//media/image17.png" style="width:5.14375in;height:0.784in" alt="A screenshot of a computer screen Description automatically generated" />

##### c. Upload the signed verification certificate to DPS

Follow the “Upload the signed verification certificate” section in
[this document](https://docs.microsoft.com/azure/iot-dps/how-to-verify-certificates).  As a result, the status of your uploaded certification should be “Verified” as shown below (make sure to refresh the page to see the updated change in status). 

<img src=".//media/image18.png" style="width:5.256in;height:1.40665in" alt="A screenshot of a social media post Description automatically generated" />

##### STOP! Quick summary of this step

At this point, you have verified your X.509 CA certificate in DPS. To link all the above sub-steps and understand why this step is significant, it is recommended to download and read [this document](https://docs.microsoft.com/azure/iot-dps/how-to-verify-certificates).

#### 3. Add a new enrollment group using the "signer-ca.pem" file

a. In the Azure portal, navigate to your DPS &gt; Manage enrollments &gt;
Select “Enrollment Groups” tab:

<img src=".//media/image19.png" style="width:4.86246in;height:3.09722in" alt="A screenshot of a cell phone Description automatically generated" />

b. Add enrollment group &gt; Enter “Group name” &gt; Choose Certificate
as “Attestation Type” &gt; Choose “False” for IoT Edge Device &gt;
Choose “Intermediate Certificate” as Certificate Type &gt; Upload
\[path]\.microchip-iot\signer-ca.pem to Primary Certificate &gt;
select “Evenly weighted distribution” for how you want to assign
devices to hub &gt; select your IoT Hub that this new enrollment group
can assign to &gt; leave the rest as their existing defaults &gt; hit
“Save”.

Once this has been done, your enrollment group name should show up in the Enrollment Groups tab:

<img src=".//media/image20.png" style="width:2in;height:4in" alt="A screenshot of a cell phone Description automatically generated" />

#### STOP! Sanity checks:

At this point, you have successfully created an enrollment group in DPS. However, as we have not yet programmed the PIC-IoT board with
the demo firmware, the device should not show up anywhere in Azure.
This can be verified in both DPS and in IoT Hub.

1. In your DPS, Enrollment Group’s Registration Records should be empty.
This can be verified by clicking on your newly enrolled group &gt;
Registration Records &gt; observe that no device shows up.

2. In your IoT Hub, your device ID should not show up in the IoT Devices.
This can be verified by clicking on your IoT Hub that links to your
DPS &gt; click “IoT devices” (on the left-hand side under “Explorers”
&gt; observe that your PIC-IoT device ID (e.g. “sn01237F696BEB9C89FE”) does
not show up.

### Section 4: Connect the PIC-IoT Device to Azure IoT Hub

In this section, we will flash the PIC-IoT board and connect it to Azure.

1. Clone/download the MPLAB X demo project
    by issuing the following commands in a Command Prompt or PowerShell
    window:

    ```bash
    git clone https://github.com/Azure-Samples/Microchip-PIC-IoT-Wx.git
    cd Microchip-PIC-IoT-Wx
    git submodule update --init
    ```

2. Launch the MPLAB X IDE and navigate to the main toolbar's "File -> Open Project" operation to load the demo project folder (\*.X) located
    at:

    ```bash
    [path]\Microchip-PIC-IoT-Wx\AzureIotPnpDps
    ```

    <img src=".//media/image40.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

   If the "load error" message in red appears, click on the “Resolve DFP for configuration: default” link: 

    <img src=".//media/image21.png" style="width:6.5in;height:1.00833in" alt="A screenshot of a cell phone Description automatically generated" />

3. Set the AzureIotPnpDps project as the main (currently focused/active) project by right-clicking on it and selecting "Set as Main Project"

    <img src=".//media/image41.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

4. Modify `AzureIotPnPDps/Header Files/platform/config/conf_winc.h` with your
    wireless router’s SSID and password (keeping the surrounding quotation marks for each); for example

    ```c
    #define CFG_MAIN_WLAN_SSID "MY_WIFI_AP_SSID"
    #define CFG_MAIN_WLAN_PSK  "MY_WIFI_AP_PSWD"
    ```

    > [!TIP]  
    > You may also configure WiFi SSID and WiFI Password through CLI

5. Verify the project properties are set correctly before building the
   project:

    - Connect the board to PC, then make sure “CURIOSITY” device shows up as a disk drive in a File Explorer window

    - Right-click the project AzureIotPnPDps &gt; select “Properties” &gt; Verify
    that all Configuration settings are at least the minimum versions as
    shown in the below screenshot (and that your PIC-IoT board is
    selected as the Connected Hardware Tool). If any changes were made in the project properties window,
    make sure to hit the “Apply” button before hitting “OK”.

    <img src=".//media/image26.png" style="width:5.88652in;height:2.68981in" alt="A screenshot of a social media post Description automatically generated" />

6. Build the project and set up a Command Line Interface (CLI) to the board:

    - Open a serial terminal (e.g. PuTTY, TeraTerm, etc.) and connect to the COM port corresponding to your board at 9600 baud (e.g. open PuTTY Configuration window &gt; choose “session” &gt; choose “Serial”&gt; Enter the right COMx port). You can find the COM info by opening your PC’s Device Manager &gt; expand Ports(COM & LPT) &gt; take note of Curiosity Virtual COM Port. 

        <img src=".//media/image27.png" style="width:2in;height:2in" alt="A screenshot of a cell phone Description automatically generated" />

    - Right-click the AzureIotPnPDps project and select “Make and Program Device”.  This operation will automatically clean and build the project before attempting to program the target device. After the “BUILD SUCCESSFUL” message appears in the Output window, the application HEX file will be programmed onto the PIC-IoT board. Once programming has finished, the board will automatically reset and start running its application code.

7. In the terminal emulator window, hit [RETURN] to get the list of available commands for the Command Line Interface (CLI).  The Command Line Interface allows you to send simple ASCII-string commands to set or get the user-configurable operating parameters of the application while it is running.  The CLI prompt is simply the '.' character

    <img src=".//media/image44.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

8. Look up the ID Scope corresponding to your DPS in the Microsoft Azure Portal.  This value is displayed in a web browser when clicking on "Overview" on the DPS resource page (the DPS was created earlier using a web page on the Azure Portal).  The ID Scope is programmed/saved into the PIC-IoT board in the next step using a CLI command (allowing you to change the ID Scope for the board without having to reprogram the MCU's application firmware)

    <img src=".//media/image23.png" style="width:6.5.in;height:1.43506in" alt="A screenshot of a cell phone Description automatically generated" />

9. In the terminal emulator window, confirm that “local echo” is enabled in the terminal settings.  At the CLI prompt, type in the command "idscope <MY_ID_SCOPE>" to set the ID Scope (which gets saved in the ATECC608A secure element on the PIC-IoT board) and then hit [ENTER].  To confirm it was set correctly, the ID Scope can be read out from the board by issuing the "idscope" command (i.e. without specifying an ID Scope value as the parameter on the command line)

    <img src=".//media/image46.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

10. At the CLI prompt, type in the command "reset" and hit [ENTER] to restart the application using the updated ID Scope to establish a connection to your DPS.

11.	Wait for the PIC-IoT board to connect to your DPS and stabilize (allow up to 2 minutes); eventually the Blue and Green LEDs should both stay constantly on (which signifies a successful & stable DPS connection).  If the Red LED comes on and stays lit, then something was incorrectly programmed (e.g. application firmware, Wi-Fi credentials, ID Scope).  If the Blue LED is not constantly on, then there is an issue with connecting to your wireless access point.

12. To enable the “full” debug messaging output to the terminal emulator window, execute the command "debug 4" on the Command Line Interface (CLI).  To disable the debug messages at any time, execute the command "debug 0" (debug levels range from 0 to 4).  The CLI is always active, even while debug messages are being continuously displayed on the terminal window.

### Section 5: Verify the Connection between PIC-IoT and Azure

A successful PIC-IoT to Azure DPS connection can be verified two ways:

1) Correct device ID shows up in the DPS enrollment
2) Correct device ID shows up in the IoT Hub

Procedure:

1. Verify in the Azure Device Provisioning Service

    - In the Azure Portal, go to your DPS &gt; click “Manage enrollments” &gt; under Enrollment Group, click “your group name” &gt; click “Registration Records” &gt; device should show up with the IoT Hub info that it got assigned to

    <img src=".//media/image28.png" style="width:5.64758in;height:2.34954in" alt="A screenshot of a social media post Description automatically generated" />

2. In the Azure Portal, go to your IoT Hub &gt; click “IoT
    Devices” &gt; click “Refresh” &gt; device should show up with the
    Status “Enabled” and Authentication Type of “SelfSigned”

    <img src=".//media/image29.png" style="width:4.91723in;height:2.58102in" alt="A screenshot of a cell phone Description automatically generated" />

### Section 6: PIC-IoT Board Interaction with Azure IoT Explorer

 Once the PIC-IoT connection to Azure has been verified in the previous section, the device can be monitored & controlled using Microsoft's Azure IoT Explorer. The Azure IoT Explorer is a graphical tool for interacting with and testing your IoT device on Azure. View [this document](https://docs.microsoft.com/azure/iot-pnp/howto-install-iot-explorer#install-azure-iot-explorer) for more details.

1. Connect Azure IoT Explorer to IoT Hub by providing your IoT Hub’s connection string.  

    From the Azure Portal: click on your IoT Hub &gt;Shared access polices &gt; iothubowner &gt; connection string-primary key &gt; Copy to clipboard

    <img src=".//media/image30.png" style="width:2in;height:4in" alt="A screenshot of a cell phone Description automatically generated" />

2. Launch Azure IoT Explorer: Click on “Add connection” &gt; paste the
    Connection String &gt; Save

    <img src=".//media/image31.png" style="width:5.56482in;height:2.24436in" alt="A screenshot of a cell phone Description automatically generated" />

    <img src=".//media/image37.png" style="width:4.49781in;height:3.72222in" alt="A screenshot of a cell phone Description automatically generated" />

3. In the Azure IoT Explorer window, click on the "Home" link near the top of the window

4. On the left-hand side of the Azure IoT Explorer window, click on "IoT Plug and Play Settings"

    <img src=".//media/image48.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

5. Please make sure "Public repository" is in the list

    If "Public repository" is not listed, add it from "Add" menu

    <img src=".//media/image49.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

6. Click on "Save"

    <img src=".//media/image50.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

7. On the left-hand side of the IoT Explorer window, click on "IoT hubs"

    <img src=".//media/image51.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

8. Verify that the name of your IoT hub is displayed, then click on "View devices in this hub"

    <img src=".//media/image52.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

9. Verify that your device ID is displayed (and status is enabled), then click on it

    <img src=".//media/image53.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />
  
10. On the left-hand side of the IoT Explorer window, click on "IoT Plug and Play components"

    <img src=".//media/image54.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />	

11.	Click on "Default component" near the bottom of the IoT Explorer window

    <img src=".//media/image55.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />
 
12.	Click on "Properties (read-only)" near the top of the IoT Explorer window

13.	Confirm that the “Enum” value of each LED matches the state observed on the PIC-IoT board (1 = On, 2 = Off, 3 = Blinking) and note the maximum temperature reading since device reset (maxTempSinceLastReboot)

    <img src=".//media/image57.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

14.	Click on "Properties (writable)" near the top of the IoT Explorer window

15. Click on the input field labeled "led_yellow" and select "Blink"

    <img src=".//media/image59.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

16. Click on "Update desired value"

    <img src=".//media/image60.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

17. Observe the notification that the request to write the property was accepted by your device, and that the Yellow LED on the PIC-IoT board is blinking/toggling/flashing

    <img src=".//media/image61.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

18. Click on "Commands" near the top of the IoT Explorer window

19. To set up the getMaxMinReport command, hover the mouse near the right corners of the since input field until the little triangle shows up and then click on it

20. When the calendar window pops up, select today’s date

    <img src=".//media/image64.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

21. Type in the current time

    <img src=".//media/image65.png" style="width:4.in;height:2.18982in" alt="A screenshot of a cell phone Description automatically generated" />

22. Click on "Send command"

    <img src=".//media/image66.png" style="width:4.in;height:2.18982in" alt="A screenshot of a cell phone Description automatically generated" />

23. Confirm that the command was successfully invoked and that both the max & min temperatures are reported in the message that appears in the pop-up window (once the message window disappears, it can be read again by clicking on "Notifications")

    <img src=".//media/image67.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

24. Click on "Telemetry" near the top of the IoT Explorer window and then click on "Start"

25. Observe the telemetry data (for the temperature and light sensors) is updating every few seconds

    <img src=".//media/image69.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

26. Increase the ambient light shining on top of the board and observe that the value of the light sensor increases within a few seconds

    <img src=".//media/image70.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

27. On the PIC-IoT Wx development board, press and release user buttons SW0 and/or SW1 

    <img src=".//media/image71.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

28. Observe the button event message (telemetry) that is generated each time a user button has been pressed/released

    <img src=".//media/image72.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

29. Click on "Commands" near the top of the IoT Explorer window

30. Click on the input field for "delay" and type "PT10S", then click "Send command".  Confirm that the command was successfully invoked via a notification message, and then the board resets itself in approximately 10 seconds (the LEDs on the board will cycle and re-initialize)

    <img src=".//media/image74.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

31. Experiment with the CLI commands to control the LED’s from the terminal emulator window by issuing the led command (usage: "led [led],[state]")
[led] : {1 = blue, 2 = green, 3 = yellow, 4 = red}
[state] : {1 = on, 2 = off, 3 = start blinking, 4 = stop blinking}

    <img src=".//media/image75.png" style="width:8.in;height:6.18982in" alt="A screenshot of a cell phone Description automatically generated" />	

## Future Considerations

Instead of connecting to IoT Hub and viewing the telemetry on Azure IoT
Explorer, the PIC-IoT board can be provisioned to connect to Azure IoT Central instead, which has a built-in dashboard to monitor the telemetry. Using IoT Central should be strongly considered for future projects.

## Conclusion

You are now able to connect PIC-IoT to Azure using self-signed cert base
authentication and have deeper knowledge of how all the pieces of the puzzle
fit together from the ATECC608A secure element, ATWINC1510 Wi-Fi, Azure
Embedded C SDK, and Azure IoT Hub/DPS. Let’s start thinking out of the
box and see how you can apply this project to provision securely and
quickly a massive number of Microchip devices to Azure and safely manage
them through the whole device life cycle.