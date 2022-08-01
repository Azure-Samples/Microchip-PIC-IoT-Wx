## Frequently Asked Questions (FAQ)

- The board isn't showing up on my computer as a drive?

    > Try using a different cable. Some micro USB cables are power only and you need a power and data cable.

- My board won't connect to the WiFi?

    > Make sure your WiFi doesn't have spaces in the SSID or password. Also, this board can only connect to 2.4GHz bands. Make sure the network you are connected to isn't 5GHz. Check the serial output messages on the CLI for feedback on why connection to WiFi may be failing...

- How do I know that my board is connected to WiFi?

    > The blue LED should be constantly on. A possible tip is to use your phone as a hotspot. Generally phones will let you know with a notification that a device connected to it. Note that if you are using an iPhone, make sure "Maximize Compatibility" is turned on. You can also change the name of your WiFi hotspot on an iPhone by going to `Settings -> General -> About` and the top option `Name` can be updated to a name without spaces.

- I can't see my commands being typed on the terminal?

    > Try using [TeraTerm](https://osdn.net/projects/ttssh2/releases/). The defaults settings seem to work the best and it has additional features like session resumption and auto-finding serial connections.

- The provisioning tool doesn't seem to be working?

    > Make sure MPLAB X is **NOT** open while running the IoT Provisioning Tool.

- The `pyazureutils` utility can't be found when I invoke it?

    > Make sure the path to the util is added to your `PATH` environment variable.
    [Link here for Windows instructions](https://docs.microsoft.com/en-us/previous-versions/office/developer/sharepoint-2010/ee537574(v=office.14))

- I am getting an error similar to `sn0123342F748ACFD3FE ERROR ERROR BSD: BSD_send() wincSendReturn (-14)`?

    > Try issuing the `reset` CLI command. A networking configuration might need to be initialized via a reboot.
