# Connecting the PIC-IoT Wx Development Board to Azure IoT Central

## Introduction

[Azure IoT Central](https://docs.microsoft.com/en-us/azure/iot-central/core/overview-iot-central) is an IoT application platform that reduces the burden and cost of developing, managing, and maintaining enterprise-grade IoT solutions. Choosing to build with IoT Central gives you the opportunity to focus time, money, and energy on transforming your business with IoT data, rather than just maintaining and updating a complex and continually evolving IoT infrastructure.

The web UI lets you quickly connect devices, monitor device conditions, create rules, and manage millions of devices and their data throughout their life cycle. Furthermore, it enables you to act on device insights by extending IoT intelligence into line-of-business applications.

[IoT Plug and Play](https://docs.microsoft.com/en-us/azure/iot-develop/overview-iot-plug-and-play) enables solution builders to integrate IoT devices with their solutions without any manual configuration. At the core of IoT Plug and Play, is a device model that a device uses to advertise its capabilities to an IoT Plug and Play-enabled application. This model is structured as a set of elements that define:

- `Properties` that represent the read-only or writable state of a device or other entity. For example, a device serial number may be a read-only property and a target temperature on a thermostat may be a writable property

- `Telemetry` that's the data emitted by a device, whether the data is a regular stream of sensor readings, an occasional error, or an information message

- `Commands` that describe a function or operation that can be done on a device. For example, a command could reboot a gateway or take a picture using a remote camera

As a solution builder, you can use IoT Central to develop a cloud-hosted IoT solution that uses IoT Plug and Play devices. IoT Plug and Play devices connect directly to an IoT Central application where you can use customizable dashboards to monitor and control your devices. You can also use device templates in the IoT Central web UI to create and edit [Device Twins Definition Language (DTDL)](https://github.com/Azure/opendigitaltwins-dtdl) models.

**NOTE**: If you run into issues, please see the below [FAQ](#faq) section for possible solutions.
## Program the Plug and Play Demo

1. Clone/download the MPLAB X demo project by issuing the following commands in a `Command Prompt` or `PowerShell` window

    ```bash
    git clone https://github.com/Azure-Samples/Microchip-PIC-IoT-Wx.git
    cd Microchip-PIC-IoT-Wx
    git submodule update --init
    ```

2. Connect the board to PC, then make sure `CURIOSITY` device shows up as a disk drive on the `Desktop` or in a `File Explorer` window. Drag and drop (i.e. copy) the pre-built `*.hex` file (located in the folder at `Microchip-PIC-IoT-Wx` > `AzurePnPDps.X` > `dist` > `default` > `production`) to the `CURIOSITY` drive 

    <img src=".//media/image115.png">

    NOTE: If this file copy operation fails for any reason, [Make and Program Device](./AzurePnP_MPLABX.md) by building the MPLAB X source code project that was used to generate the `*.hex` file

3. Set up a Command Line Interface (CLI) to the board

    - Open a serial terminal window (e.g. PuTTY, TeraTerm, etc.) and connect to the COM port corresponding to your board at `9600 baud` (e.g. open PuTTY Configuration window &gt; choose `session` &gt; choose `Serial`&gt; Enter the right COMx port). You can find the COM info by opening your PC’s `Device Manager` &gt; expand `Ports(COM & LPT)` &gt; take note of the `Curiosity Virtual COM Port`

    - In your `Terminal Options`, enable `Local Echo` to allow your keystrokes to be displayed in the terminal window

        <img src=".//media/image27.png">

4. In the terminal emulator window, hit `[RETURN]` to get the list of available commands for the CLI.  The Command Line Interface allows you to send simple ASCII-string commands to set or get the user-configurable operating parameters of the application while it is running.  The CLI prompt is simply the `.` (period) character

    <img src=".//media/image44.png">

5. In the terminal emulator window, set the debug messaging level to 0 to temporarily disable the output messages. The debug level can be set anywhere from 0 to 4.  Use the `debug <level>` command by manually typing it into the CLI.  The complete command must be followed by hitting `[RETURN]`
    ```bash
    >debug 0
    ```

6. Set the Wi-Fi credentials used by your board whenever it tries to connect to an Access Point.  Execute the following `wifi` command on the CLI

    ```bash
    wifi <NETWORK_SSID>,<PASSWORD>,<SECURITY_OPTION[1=Open|2=WPA|3=WEP]>
    ```
    For example, if the SSID of the router is "MyWirelessRouter" and the WPA/WPA2 key is "MyRoutersPassword", the exact command to type into the CLI (followed by `[RETURN]`) would be
    ```bash
    wifi MyWirelessRouter,MyRoutersPassword,2
    ```

    Once these credentials have been used to successfully connect to Wi-Fi once, they will be stored in the board and will be used in all subsequent Wi-Fi connection attempts.  In other words, the `wifi` command only needs to be executed successfully one time for your Access Point.

7. At the CLI prompt, type in the `reset` command and hit `[RETURN]` to restart the host application.  The application will try to connect to Wi-Fi using the credentials that were specified in the last accepted `wifi` command.  The Blue LED should eventually stay solidly ON to signify that the SAM-IoT board has successfully connected to the wireless router

## Create an IoT Central Application

IoT Central allows you to create an application dashboard to monitor the telemetry and take appropriate actions based on customized rules.  To access all of your custom applications, you must be signed into the [Azure IoT Central Portal](https://apps.azureiotcentral.com) (it is recommended to bookmark this link for later use).

1. Create a custom IoT Central application by starting with an existing [Microchip IoT Development Board Template](https://apps.azureiotcentral.com/build/new/32c74ec5-cf39-4a90-acda-5cb1909c2c28) (if there is a problem with loading the template after clicking the link, refer to the [Create an application](https://docs.microsoft.com/en-us/azure/iot-central/core/quick-deploy-iot-central) section to create your IoT Central application from scratch). If you are not currently logged into your [Microsoft account](https://account.microsoft.com/account), you will be prompted to sign in with your credentials to proceed. If you do not have an existing Microsoft account, go ahead and create one now by clicking on the `Create one!` link

2. Azure IoT Builder will guide you through the process of creating your application. Review and select the settings for your IoT Central application (if needed, refer to [Create an application](https://docs.microsoft.com/en-us/azure/iot-central/core/quick-deploy-iot-central) for additional guidance on selecting the settings for your application). Do not click the `Create` button just yet - only after reviewing and taking into consideration the following recommendations:
  
    - Selecting a name for your application: Azure IoT Builder will generate a suggested randomly-generated unique name for your application.  You are free to make modifications to the name, but it must be a **unique** `Application name` **without** using any spaces (which will then automatically result in a corresponding unique `URL` for accessing your application from a web browser).  Take note of the custom `Application name` and `Application URL` assigned to your IoT Central application as they will be needed when it becomes time to register the device with the Device Provisioning Service (DPS) associated with your custom IoT Central application

        NOTE: Make sure the `Application name` does **not** contain any spaces

        <img src=".//media/image80a.png">

    - `Pricing plan` selection: If you select the **Free** plan, you can connect up to 5 devices.  However, the free trial period will expire after 7 days which means a [paid pricing plan](https://azure.microsoft.com/en-us/pricing/details/iot-central/) will need to be selected to continue using the application.  Of course, there is nothing to stop you from creating a new free trial application but the device will need to be configured for the new app from scratch.  Since the **Standard** plans each allow 2 free devices with no time-restricted trial period, if you only plan on evaluating 1 or 2 devices for connecting to the IoT Central app, then it's best to choose the **Standard 2** plan to get the highest total allowable number of messages (30K per month)

        <img src=".//media/image80b.png" style="width:6.5.in;height:3.63506in" alt="A screenshot of a cell phone Description automatically generated" />

    - `Billing info` section: If there is an issue with selecting an existing subscription in the drop-down list (or no subscriptions appear in the list at all), click on the `Create subscription` link to create a new subscription to use for the creation of this application (you will be asked to create an Azure account and provide credit card information but don't worry - you will not be charged since you are only connecting one device to the application)
    
        <img src=".//media/image80c.png" style="width:6.5.in;height:2.53506in" alt="A screenshot of a cell phone Description automatically generated" />

3. Register the device certificate with your custom IoT Central application by running the [pyazureutils](https://pypi.org/project/pyazureutils/) utility supplied by Microchip (which should already be installed).

    - Refer to the [Dev Tools Installation](./Dev_Tools_Install.md) procedure and confirm that `Azure CLI`, `Python`, and `pyazureutils` have all been installed. The device certificate should reside in the `.microchip_iot` folder which was generated by the IoT Provisioning Tool, so `pyazureutils` will look in that folder to grab the correct certificate used during the attestation process required for connecting a device to your IoT Central application
    
    - Launch a `Command Prompt` or `PowerShell` window and then execute the following command line (where <YOUR_CUSTOM_APP_NAME> is the unique `Application name` which was set in the previous step). If this command fails to execute, try specifying the `Application URL` for <YOUR_CUSTOM_APP_NAME>. You may be prompted to log into your Microsoft Azure account if you are not currently signed in

        ```bash
        pyazureutils iotcentral register-device --application-name <YOUR_CUSTOM_APP_NAME>
        ```

    - The final output messages should look something like the following:

        ```bash
        Using kit 'MCHP3261021800002492'
        Using certificate file '/Users/c14166/.microchip-iot/MCHP3261021800002492/device.crt'
        Certificate loaded
        Extracting common name from certificate (to use as device ID)
        Device ID will be: 'sn01234CB4D6759E60FE'
        Retrieving device templates
        Using device template: PIC-IoT WM (dtmi:modelDefinition:dftia5bwj:un5msohpx)
        Creating device 'sn01234CB4D6759E60FE' from template 'dtmi:modelDefinition:dftia5bwj:un5msohpx'
        Checking device
        Creating device attestation using certificate
        Checking device attestation
        Registration complete!
        ```

    NOTE: This command will fail if the custom application name contains one or more spaces. To resolve it, go back to the `Your application` page (accessed by clicking `Administration` under `App settings` in the navigation pane of your [IoT Central application](https://apps.azureiotcentral.com)) and rename your application (removing all spaces).  If the `pyazureutils` command continues to fail, follow the procedure for [Creating an X.509 Enrollment Group](./IoT_Central_Enrollment_Group.md) to get your device connected using the group enrollment method

    **Future Consideration**: An enrollment group is an entry for a group of devices that share a common attestation mechanism. Using an enrollment group is recommended for a large number of devices that share an initial configuration, or for devices that go to the same tenant. Devices that use either symmetric key or X.509 certificates attestation are supported. [Create an X.509 Enrollment Group](./IoT_Central_Enrollment_Group.md) for your IoT Central application should the need ever arise in the future, when tens, hundreds, thousands, or even millions of devices (that are all derived from the same root certificate) need to connect to an application...

4. Launch a terminal emulator window and connect to the COM port corresponding to the PIC-IoT board at `9600 baud`.  In the terminal window, hit `[RETURN]` to get the list of available commands for the CLI

    <img src=".//media/image83.png">

5.	Look up the `ID Scope` for the DPS created/used by your IoT Central application (using the left-hand navigation pane, select `Administration` > `Device connection`).  The `ID Scope` will be programmed/saved into the [ATECC608A](https://www.microchip.com/wwwproducts/en/atecc608a) secure element on the board in the next step

    <img src=".//media/image84.png">

6. In the terminal emulator window, confirm that `local echo` is enabled in the terminal settings.  At the CLI prompt, type in the `idscope <ID-scope>` command to set it (which gets saved in the [ATECC608A](https://www.microchip.com/wwwproducts/en/atecc608a) secure element on the board) and then hit `[RETURN]`.  The ID Scope can be read out from the board by issuing the `idscope` command without specifying any parameter on the command line

    <img src=".//media/image85.png">

7. Using the CLI prompt, type in the `reset` command and hit `[RETURN]`

8. Wait for the PIC-IoT board to connect to your IoT Central’s DPS (it can take a few minutes for the LED's to stop flashing); eventually the Blue and Green LEDs should both stay constantly ON

    NOTE: If the Red LED comes on, then something was incorrectly programmed (e.g. ID scope is invalid or was entered/saved incorrectly)

9. Go back to your web browser to access the Azure IoT Central application.  Use the left-hand side pane and select `Devices` > `All Devices`.  Confirm that your device is listed – the device name & ID should both be equal to the Common Name of the device certificate (which should be `sn + {17-digit device ID}`)

    <img src=".//media/image86.png">

    NOTE: The device ID can be read out from the board at any time using the CLI `device` command.  This value is basically the serial number which has been pre-programmed into the [ATECC608A](https://www.microchip.com/wwwproducts/en/atecc608a) secure element by Microchip

10. If desired, change the Device name by clicking on `Manage device` > `Rename`

    <img src=".//media/image87.png">

11. Click on the `Command` tab; type `PT5S` in the `Reboot delay` field and then click on `Run` to send the command to the device to reboot in 5 seconds

    <img src=".//media/image88.png" style="width:5.in;height:1.58982in" alt="A screenshot of a cell phone Description automatically generated" />

12. Within 5 seconds of sending the Reboot command, the PIC-IoT development board should reset itself.  Once the Blue and Green LED's stay constantly ON, press the SW0 and SW1 buttons

    <img src=".//media/image89.png" style="width:5.in;height:1.28982in" alt="A screenshot of a cell phone Description automatically generated" />

13. Click on the `Raw data` tab and confirm that the button press telemetry messages were received

    <img src=".//media/image90.png" style="width:5.in;height:1.98982in" alt="A screenshot of a cell phone Description automatically generated" />

14. Click on the `Refresh` icon to display all messages received since the previous page refresh operation.  Confirm that periodic telemetry messages are being continuously received approximately every 10 seconds (the default interval value for the `telemetryInterval` property)

    <img src=".//media/image91.png" style="width:5.in;height:1.58982in" alt="A screenshot of a cell phone Description automatically generated" />

    <img src=".//media/image92.png" style="width:5.in;height:2.12982in" alt="A screenshot of a cell phone Description automatically generated" />

15. Increase the ambient light source shining on top of the board. Wait approximately 30 seconds.  Click on the `Refresh` icon to confirm that the light sensor value has increased

    <img src=".//media/image93.png">

## Connect Device to the Dashboard for Data Visualization

1. Navigate to the left-hand vertical toolbar and click on the `Dashboards` icon

    <img src=".//media/image100.png" style="width:5.in;height:0.98982in" alt="A screenshot of a cell phone Description automatically generated" />

2. Towards the top of the web page, click on the `Edit` icon

    <img src=".//media/image101.png" style="width:5.in;height:0.48982in" alt="A screenshot of a cell phone Description automatically generated" />

3. For **all** of the existing tiles named `Light` or `Temperature`, click on the upper right-hand corner of the tile to select `Configure`

    <img src=".//media/image102a.png" style="width:5.in;height:2.18982in" alt="A screenshot of a cell phone Description automatically generated" />
    <img src=".//media/image102b.png" style="width:5.in;height:2.18982in" alt="A screenshot of a cell phone Description automatically generated" />

4. Select `Device Group` > `PIC-IoT WM - All devices` and then check the box for your specific device name for `Devices`

    <img src=".//media/image103.png" style="width:5.in;height:2.08982in" alt="A screenshot of a cell phone Description automatically generated" />

5. Under the `Telemetry` category, click on `+ Capability` and select the parameter pertaining to the title of the tile (e.g. `Brightness from light sensor` for each of the `Light` tiles or `Temperature` for each of the `Temperature` tiles)

    <img src=".//media/image104a.png" style="width:5.in;height:0.89082in" alt="A screenshot of a cell phone Description automatically generated" />
    <img src=".//media/image104b.png" style="width:5.in;height:2.18982in" alt="A screenshot of a cell phone Description automatically generated" />
    <img src=".//media/image104c.png" style="width:5.in;height:1.18982in" alt="A screenshot of a cell phone Description automatically generated" />

6. Click on `Update` and repeat the process for the remainder of the existing tiles

    <img src=".//media/image105.png" style="width:5.in;height:0.48982in" alt="A screenshot of a cell phone Description automatically generated" />

7. After every tile has been configured to visualize your device's sensor data, click on the `Save` icon to save the latest changes to the dashboard

    <img src=".//media/image106.png" style="width:5.in;height:0.38982in" alt="A screenshot of a cell phone Description automatically generated" />

8. Confirm that the dashboard is being continuously updated with the expected telemetry data received from the device.  For example, adjust the ambient light source directed at the board and observe that the light sensor values are changing accordingly

    <img src=".//media/image107.png" style="width:5.in;height:2.58982in" alt="A screenshot of a cell phone Description automatically generated" />

9. To access your IoT Central application(s) in the future, go to [Azure IoT Central](https://apps.azureiotcentral.com) and click on `My apps`

    <img src=".//media/image108.png" style="width:5.in;height:1.98982in" alt="A screenshot of a cell phone Description automatically generated" />

## Expand the Dashboard with Additional Tiles

To create additional tiles for your IoT Central dashboard, refer to [Configure the IoT Central application dashboard](https://docs.microsoft.com/en-us/azure/iot-central/core/howto-add-tiles-to-your-dashboard). The below screen captures show additional possibilities of dashboard components that can highlight the telemetry data and properties facilitated by the `Plug and Play` interface.  Note that multiple devices can be selected for each tile to allow groups of devices to be visualized within a single tile. 

<img src=".//media/image95.png" style="width:5.in;height:3.34982in" alt="A screenshot of a cell phone Description automatically generated" />

<img src=".//media/image96.png" style="width:5.in;height:4.4182in" alt="A screenshot of a cell phone Description automatically generated" />

<img src=".//media/image97.png" style="width:5.in;height:3.34982in" alt="A screenshot of a cell phone Description automatically generated" />

<img src=".//media/image98.png" style="width:5.in;height:3.34982in" alt="A screenshot of a cell phone Description automatically generated" />

## FAQ

- The board isn't showing up on my computer as a drive?

> Try using a different cable. Some micro USB cables are power only and you need a power and data cable.

- My board won't connect to the WiFi?

> Make sure your WiFi doesn't have spaces in the SSID or password. Also, this board can only connect to 2.4GHz bands. Make sure the network you are connected to isn't 5GHz.

- How do I know that my board is connected to WiFi?

> The blue LED should be constantly on. A possible tip is to use your phone as a hotspot. Generally phones will let you know with a notification that a device connected to it. Note that if you are using an iPhone, make sure "Maximize Compatibility" is turned on. You can also change the name of your WiFi hotspot on an iPhone by going to `Settings -> General -> About` and the top option `Name` can be update to a name without spaces.

- I can't see my commands being typed on the terminal?

> Try using [TeraTerm](https://osdn.net/projects/ttssh2/releases/). The defaults settings seem to work the best and it has additional features like session resumption and auto-finding serial connections.

- The provisioning tool doesn't seem to be working?

> Make sure MPLAB X is **NOT** open while running the provisioning tool.

- The `pyazureutils` utility can't be found when I invoke it?

> Make sure the path to the util is added to your `PATH` environment variable. [Link here for Windows instructions](https://docs.microsoft.com/previous-versions/office/developer/sharepoint-2010).

- I am getting an error similar to `sn0123342F748ACFD3FE ERROR ERROR BSD: BSD_send() wincSendReturn (-14)`?

> Try issuing the `reset` CLI command. A networking configuration might need to be initialized via a reboot.
