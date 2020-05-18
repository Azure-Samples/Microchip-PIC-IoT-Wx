/*
    \file   mqtt_iotprovisioning_packetPopulate.h

    \brief  MQTT Packet Populate header file.

    (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip software and any
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party
    license terms applicable to your use of third party software (including open source software) that
    may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
    FOR A PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
    SOFTWARE.
*/


#ifndef MQTT_IOTPROVISIONING_PACKET_POPULATE_H
#define MQTT_IOTPROVISIONING_PACKET_POPULATE_H

#include <stdbool.h>
#include <stdint.h>
#include "../../config/IoT_Sensor_Node_config.h"

void MQTT_CLIENT_iotprovisioning_publish(uint8_t *data, uint16_t len);
void MQTT_CLIENT_iotprovisioning_receive(uint8_t *data, uint8_t len);
void MQTT_CLIENT_iotprovisioning_connect(char* deviceID);
bool MQTT_CLIENT_iotprovisioning_subscribe();
void MQTT_CLIENT_iotprovisioning_connected();

static const char* const provisioning_enrollment_key = PROVISIONING_ENROLLMENT_KEY;

#endif /* MQTT_IOTPROVISIONING_PACKET_POPULATE_H */