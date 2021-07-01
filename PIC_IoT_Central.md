# Connecting the PIC-IoT Wx Development Board to Azure IoT Central

## Introduction

[Azure IoT Central](https://docs.microsoft.com/en-us/azure/iot-central/core/overview-iot-central) is an IoT application platform that reduces the burden and cost of developing, managing, and maintaining enterprise-grade IoT solutions. Choosing to build with IoT Central gives you the opportunity to focus time, money, and energy on transforming your business with IoT data, rather than just maintaining and updating a complex and continually evolving IoT infrastructure.

The web UI lets you quickly connect devices, monitor device conditions, create rules, and manage millions of devices and their data throughout their life cycle. Furthermore, it enables you to act on device insights by extending IoT intelligence into line-of-business applications.

## Program the Plug and Play Demo

1. Clone/download the MPLAB X demo project by issuing the following commands in a `Command Prompt` or `PowerShell` window

    ```bash
    git clone https://github.com/Azure-Samples/Microchip-PIC-IoT-Wx.git
    cd Microchip-PIC-IoT-Wx
    git submodule update --init
    ```

2. Launch the MPLAB X IDE and navigate to the main toolbar's `File` > `Open Project` operation to load the demo project folder (\*.X) located at \Microchip-PIC-IoT-Wx\AzureIotPnpDps

    <img src=".//media/image40.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

   If the `load error` message in red appears, click on the `Resolve DFP for configuration: default` link

    <img src=".//media/image21.png" style="width:6.5in;height:1.00833in" alt="A screenshot of a cell phone Description automatically generated" />

3. Set the `AzureIotPnpDps` project as the main (currently focused/active) project by right-clicking on it and selecting `Set as Main Project`

    <img src=".//media/image41.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

4. Modify `AzureIotPnPDps/Header Files/platform/config/conf_winc.h` with your
    wireless router’s SSID and password (keeping the surrounding quotation marks for each); for example

    ```c
    #define CFG_MAIN_WLAN_SSID "MY_WIFI_AP_SSID"
    #define CFG_MAIN_WLAN_PSK  "MY_WIFI_AP_PSWD"
    ```

5. Verify the project properties are set correctly before building the
   project

    - Connect the board to PC, then make sure `CURIOSITY` device shows up as a disk drive in a `File Explorer` window

    - Right-click the project `AzureIotPnPDps` &gt; select `Properties` &gt; Verify
    that all Configuration settings are at least the minimum versions as
    shown in the below screenshot (and that your PIC-IoT board is
    selected as the Connected Hardware Tool). If any changes were made in the project properties window, make sure to hit the `Apply` button before hitting `OK`

        <img src=".//media/image26.png" style="width:5.88652in;height:2.68981in" alt="A screenshot of a social media post Description automatically generated" />

6. Build the project and set up a Command Line Interface (CLI) to the board

    - Open a serial terminal (e.g. PuTTY, TeraTerm, etc.) and connect to the COM port corresponding to your board at 9600 baud (e.g. open PuTTY Configuration window &gt; choose `session` &gt; choose `Serial`&gt; Enter the right COMx port). You can find the COM info by opening your PC’s `Device Manager` &gt; expand `Ports(COM & LPT)` &gt; take note of `Curiosity Virtual COM Port` 

        <img src=".//media/image27.png" style="width:3in;height:3in" alt="A screenshot of a cell phone Description automatically generated" />

    - Right-click the `AzureIotPnPDps` project and select `Make and Program Device`.  This operation will automatically clean and build the project before attempting to program the target device. After the `BUILD SUCCESSFUL` message appears in the Output window, the application HEX file will be programmed onto the PIC-IoT board. Once programming has finished, the board will automatically reset and start running its application code.

7. In the terminal emulator window, hit `[RETURN]` to get the list of available commands for the Command Line Interface (CLI).  The Command Line Interface allows you to send simple ASCII-string commands to set or get the user-configurable operating parameters of the application while it is running.  The CLI prompt is simply the `.` (period) character

    <img src=".//media/image44.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

## Create an IoT Central Application for your Device

IoT Central allows you to create an application dashboard to monitor the telemetry and take appropriate actions based on customized rules.

1. Create an IoT Central application by using the existing application template provided for the PIC-IoT Wx Development Board [PIC-IoT Application Template](https://apps.azureiotcentral.com/build/new/1f71e467-74da-4554-8444-c650de86e4df)

2. Review the settings for your IoT Central application (refer to [Create an Azure IoT Central application](https://docs.microsoft.com/en-us/azure/iot-central/core/quick-deploy-iot-central) for additional details).  Click the `Create` button to save your application

3. Create an X.509 enrollment group for your IoT Central application.  Open your IoT Central application and navigate to `Administration` in the left pane and select `Device connection`

4. Select `+ Create enrollment group`, and create a new enrollment group using any name (Group type = `IoT devices`, attestation type = `Certificates (X.509)`).  Hit `Save` when finished

    <img src=".//media/image81.png" style="width:6.5.in;height:4.43506in" alt="A screenshot of a cell phone Description automatically generated" />

5. Now that the new enrollment group has been created, select `+ Manage Primary`.

6. Select the file/folder icon associated with the `Primary` field and upload the root certificate file `root-ca.crt` (located in the `.microchip_iot` folder that was created when you ran the `IoT Provisioning Tool`).  The message `(!) Needs verification` should appear.  The `Subject` and `Thumbprint` fields will automatically populate themselves.

7.	Click `Generate verification code` (this code will be copied to the clipboard which will be needed in a future step)

8. Open a Git Bash window (Start menu &gt; type `Git Bash`)

    <img src=".//media/image15.png" style="width:3.21739in;height:0.94745in" alt="A picture containing ball, clock Description automatically generated" />

9. Using the Git Bash command line, navigate to your certificates folder (which was generated by the `IoT Provisioning Tool`)

    ```bash
    cd <path>\.microchip-iot
    ```

10. Execute the below command in the Git Bash window (copy and paste for best results)

    **Note**: Once you enter the below command, you will then be asked to enter information for various fields that will be incorporated into your certificate request. Enter the verification code (which was just generated previously) when prompted for the `Common Name`. It's recommended to just copy the Verification code to the clipboard and paste it when it's time to enter the `Common Name`.  For the rest of the fields, you can enter anything you want (or just hit `[RETURN]` to keep them blank which is fine for basic demonstration purposes).  If you accidentally hit `[RETURN]` when asked for the `Common Name`, you will need to run the command again...

    ```bash
    openssl req -new -key root-ca.key -out azure_root_ca_verification.csr
    ```
    <img src=".//media/image16.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

11. Generate the verification certificate by executing the following command (copy and paste for best results)

    ```bash
    openssl x509 -req -in azure_root_ca_verification.csr -CA root-ca.crt -CAkey root-ca.key -CAcreateserial -out azure_signer_verification.cer -days 365 -sha256
    ```

12. Click `Verify` and select the `azure_signer_verification.cer` file to upload.  Confirm that the `Primary` certificate has been verified and that a `Thumbprint` has been generated for your certificate.  Click on `Close` to exit the window.  The X.509 enrollment group should be ready to go!

    <img src=".//media/image82.png" style="width:5.in;height:2.18982in" alt="A screenshot of a cell phone Description automatically generated" />

13. Launch a terminal emulator window and connect to the COM port corresponding to the PIC-IoT board at `9600` baud.  In the terminal window, hit `[RETURN]` to get the list of available commands for the Command Line Interface (CLI)

    <img src=".//media/image83.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

14.	Look up the `ID Scope` for the DPS created/used by your IoT Central application (using the left-hand navigation pane, select `Administration` > `Device connection`).  The `ID Scope` will be programmed/saved into the [ATECC608A](https://www.microchip.com/wwwproducts/en/atecc608a) secure element on the board in the next step

    <img src=".//media/image84.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

15. In the terminal emulator window, confirm that `local echo` is enabled in the terminal settings.  At the CLI prompt, type in the `idscope <ID-scope>` command to set it (which gets saved in the [ATECC608A](https://www.microchip.com/wwwproducts/en/atecc608a) secure element on the board) and then hit `[RETURN]`.  The ID Scope can be read out from the board by issuing the `idscope` command without specifying any parameter on the command line

    <img src=".//media/image85.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

16. Using the CLI prompt, type in the command `reset` and hit `[RETURN]`

17. Wait for the PIC-IoT board to connect to your IoT Central’s DPS (allow up to 2 minutes); eventually the Blue and Green LEDs should both stay constantly on.  If the Red LED comes on, then something was incorrectly programmed (e.g. ID scope was entered incorrectly)

18. Go back to your web browser to access the Azure IoT Central application.  Use the left-hand side pane and select Devices -> All Devices.  Confirm that your device is listed – the device name & ID is the Common Name of the device certificate (which should be `sn + {17-digit device ID}`)

    <img src=".//media/image86.png" style="width:5.in;height:1.58982in" alt="A screenshot of a cell phone Description automatically generated" />

19. Change the Device name to something friendlier by clicking on it and then editing the box at the top of the page

    <img src=".//media/image87.png" style="width:5.in;height:1.18982in" alt="A screenshot of a cell phone Description automatically generated" />

20. Confirm that the Blue & Green LED states are both set to `Turn On`

    <img src=".//media/image88.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

21. Click on `Overview` under your device name to see the telemetry displays being updated every few seconds

    <img src=".//media/image89.png" style="width:5.in;height:3.18982in" alt="A screenshot of a cell phone Description automatically generated" />

22. Click on `Commands` under your device name.  Go to the `Get Max-Min report` box and click on the calendar icon

    <img src=".//media/image90.png" style="width:5.in;height:0.58982in" alt="A screenshot of a cell phone Description automatically generated" />

23. Select today’s date, then click `Run`

    <img src=".//media/image91.png" style="width:5.in;height:1.78982in" alt="A screenshot of a cell phone Description automatically generated" />

24. Click on `command history`, then `View Payload`

    <img src=".//media/image92.png" style="width:5.in;height:1.18982in" alt="A screenshot of a cell phone Description automatically generated" />

25. Confirm that valid data shows up in the `Response payload`

    <img src=".//media/image93.png" style="width:5.in;height:1.84982in" alt="A screenshot of a cell phone Description automatically generated" />

26. Click on `Raw data` under your device name to see the raw telemetry messages being received every few seconds

    <img src=".//media/image94.png" style="width:5.in;height:1.84982in" alt="A screenshot of a cell phone Description automatically generated" />

27. Follow the procedure [Configure the IoT Central application dashboard](https://docs.microsoft.com/en-us/azure/iot-central/core/howto-add-tiles-to-your-dashboard) to create additional tiles for your IoT Central application.  The below screen captures show various examples of dashboard components that highlight the telemetry data and properties facilitated by the `Plug and Play` interface

    <img src=".//media/image95.png" style="width:5.in;height:3.34982in" alt="A screenshot of a cell phone Description automatically generated" />

    <img src=".//media/image96.png" style="width:5.in;height:4.4182in" alt="A screenshot of a cell phone Description automatically generated" />

    <img src=".//media/image97.png" style="width:5.in;height:3.34982in" alt="A screenshot of a cell phone Description automatically generated" />

    <img src=".//media/image98.png" style="width:5.in;height:3.34982in" alt="A screenshot of a cell phone Description automatically generated" />
