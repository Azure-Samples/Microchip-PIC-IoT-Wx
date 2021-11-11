# Programming the Plug and Play Demo using Microchip's MPLAB X IDE

1. Connect the board to PC, then make sure `CURIOSITY` device shows up as a disk drive on the `Desktop` or in a `File Explorer` window

2. Launch the MPLAB X IDE and navigate to the main toolbar's `File` > `Open Project` operation to load the demo project folder (\*.X) located at:

    ```bash
    [path]\Microchip-PIC-IoT-Wx\AzureIotPnpDps
    ```

    <img src=".//media/image40.png"/>

   If the `load error` message in red appears, click on the `Resolve DFP for configuration: default` link: 

    <img src=".//media/image21.png"/>

3. Set the `AzureIotPnpDps` project as the main (currently focused/active) project by right-clicking on it and selecting `Set as Main Project`

    <img src=".//media/image41.png">

4. Verify the project properties are set correctly before building the project.  Right-click the project `AzureIotPnPDps` and select `Properties`. Verify that `Packs` and `Compiler Toolchain` each have the highest versions selected. Select your PIC-IoT board for the `Connected Hardware Tool`. If any changes were made in the project properties window, make sure to hit the `Apply` button before hitting `OK`

5. Right-click the `AzureIotPnPDps` project and select `Make and Program Device`.  This operation will automatically clean and build the project before attempting to program the target device. After the `BUILD SUCCESSFUL` message appears in the Output window, the application HEX file will be programmed onto the PIC-IoT board. Once programming has finished, the board will automatically reset and start running its application code.
