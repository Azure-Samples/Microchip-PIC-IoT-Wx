#ifndef IOT_SENSOR_NODE_CONFIG_H
#define IOT_SENSOR_NODE_CONFIG_H

// <h> Application Configuration

// <o> Send Interval <0-100000>
// <i> Send interval in seconds
// <id> application_send_interval
#define CFG_SEND_INTERVAL 10

// <o> Timeout <0-100000>
// <i> Timeout
// <id> application_timeout
#define CFG_TIMEOUT 5000

// </h>

// <h> Cloud Configuration

// <s> mqtt port
// <i> mqtt port value
// <id> mqtt_port
#define CFG_MQTT_PORT 8883

// <s> mqtt connect timeout
// <i> mqtt connect timeout length in seconds
// <id> mqtt_conn_timeout
#define CFG_MQTT_CONN_TIMEOUT 10

// <s> mqtt hub host
// <i> mqtt hub host address
// <id> mqtt_hub_host
#define CFG_MQTT_HUB_HOST "<INSERT HUB>"

// <s> hub device ID
// <i> hub device ID in the Azure IoT hub
// <id> id
// >>comment this out to use embedded ID device from the HSM
// #define HUB_DEVICE_ID "01233EAD58E86797FE"

// <s> hub device key
// <i> hub device key of the device in the Azure IoT hub
// <id> key
#define HUB_DEVICE_KEY "XXXXXXXXXXHASH"

// <s> mqtt provisioning host
// <i> mqtt provisioning host address
// <id> provisioning host
#define CFG_MQTT_PROVISIONING_HOST "global.azure-devices-provisioning.net"

// <s> provisioning id scope
// <i> provisioning id scope of the DPS service
// <id> id
#define PROVISIONING_ID_SCOPE "<INSERT PROVISIONING SCOPE>"

// </h>

// <h> Debug Configuration

// <q> Enable debug messages:
// <i> Check to enable debug messages
// <id> debug_msg
#define CFG_DEBUG_MSG 1

// </h>

// <h> CLI Support

// <q> Enable CLI:
// <i> Check to enable cli
// <id> enable_cli
#define CFG_ENABLE_CLI 1

// </h>


#endif // IOT_SENSOR_NODE_CONFIG_H
