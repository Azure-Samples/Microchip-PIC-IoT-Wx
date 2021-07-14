# Development Tools Installation

1. Install the Microchip `MPLAB X` Tool Chain

- [MPLAB X IDE](https://www.microchip.com/mplab/mplab-x-ide)

- [MPLAB XC16 Compiler](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers#tabs)

- `MPLAB Code Configurator` (launch MPLAB X IDE &gt; click on `Tools` &gt; `Plugins Download` &gt; search for `MPLAB Code Configurator` and install it)

    <img src=".//media/image10.png" style="width:5.17917in;height:3.98149in"/>

2. Install [Git](https://git-scm.com)

- Download/install the latest version of [Git for Windows](https://git-scm.com/download/win)

- Verify working operation of the `Git Bash` prompt (click `Start` > type `Git Bash`)

    <img src=".//media/image15.png"/>

3. Download the Microchip [IoT Provisioning Tool](https://www.microchip.com/design-centers/internet-of-things/iot-dev-kits/iot-provision-tool)

   - Connect the Microchip IoT Development Board to the Host PC using a standard micro-USB cable
   - Launch `PowerShell` (click on `Start` > type `PowerShell` in the Search field > `Open`)
   - Using the command line, go to the directory where the `iotprovision-bin` executable file resides (`\iotprovision-bin-1\windows64\`)
   - Execute the command

        ```shell
        .\iotprovision-bin.exe -c azure -m custom
        ```
