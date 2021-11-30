# Development Tools Installation

1. Install the Microchip `MPLAB X` tool chain for embedded code development

- [MPLAB X IDE](https://www.microchip.com/mplab/mplab-x-ide)

- [MPLAB XC16 Compiler](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers#tabs)

- `MPLAB Code Configurator` (launch MPLAB X IDE &gt; click on `Tools` &gt; `Plugins Download` &gt; `Go to MPLAB X Plugin Manager` &gt; `Available Plugins` &gt; search for `mcc` and install it)

    <img src=".//media/image10.png"/>

2. Install [Git](https://git-scm.com) (a free and open source distributed version control system)

- Download/install the latest version of [Git for Windows, macOS, or Linux](https://git-scm.com/downloads)

- Verify working operation of the `Git Bash` prompt (e.g. for Windows: click `Start` > type `Git Bash`)

    <img src=".//media/image15.png"/>

3. Download and run Microchip's [IoT Provisioning Tool](https://github.com/microchip-pic-avr-tools/iotprovision-bin/releases/latest) which is used to (1) generate the security certificates used for authentication and (2) store them into the [ATWINC1510](https://www.microchip.com/en-us/product/ATWINC1510) Wi-Fi controller module by executing the following steps:

   - Connect the Microchip PIC-IoT Development Board to the Host PC using a standard micro-USB cable
   - Confirm that a device named `CURIOSITY` shows up as a disk drive on the `Desktop` or in a `File Explorer` window
   - Launch a `Command Prompt` or `PowerShell` window (click on `Start` > type `PowerShell` in the Search field > `Open`)
   - Using the command line, navigate to the directory where the `iotprovision-bin` executable file resides (`<my_path>\iotprovision-bin-2\windows64\`)
   - Execute the following command line

        ```shell
        .\iotprovision-bin.exe -c azure
        ```

4. Install the Microsoft [Azure Command Line Interface (CLI)](https://docs.microsoft.com/en-us/cli/azure/?view=azure-cli-latest). The Azure CLI is a set of commands used to create and manage Azure resources. The Azure CLI is available across Azure services and is designed to get you working quickly with Azure, with an emphasis on automation

5. Install [Python](https://www.python.org)

6. Install [pyazureutils](https://pypi.org/project/pyazureutils/) (a Microchip utility for interacting with Microsoft Azure web services via the Azure CLI) by executing the following steps:

   - Launch a `Command Prompt` or `PowerShell` window (e.g. for Windows: click on `Start` > type `PowerShell` in the Search field > `Open`)
   - Execute the following command line

        ```shell
        pip install pyazureutils
        ```
   - Search for the location of the `pyazureutils.exe` program and add the absolute path to the Windows `PATH` environment variable

