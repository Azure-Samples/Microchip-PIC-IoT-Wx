/*
    \file   led.h

    \brief  led header file.

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

#ifndef LED_H_
#define LED_H_
#include <stdint.h>
#include <stdbool.h>

#define LED_ON   false
#define LED_OFF  true

typedef enum
{
    LED_BLUE,
    LED_GREEN,
    LED_YELLOW,
    LED_RED
} led_number_t;

typedef union
{
    struct {
        unsigned short red:1;
        unsigned short green:1;
        unsigned short blue:1;
        unsigned short yellow:1;
        unsigned short reserved:12;
    };
    unsigned short AsUSHORT;
} led_change_t;

typedef union
{
    struct {
        unsigned short red:3;
        unsigned short green:3;
        unsigned short blue:3;
        unsigned short yellow:3;
        unsigned short reserved:6;
    };
    unsigned short AsUSHORT;
} led_state_t;

typedef struct led
{
    led_change_t change_flag;
    led_state_t  state_flag;
} led_status_t;

typedef enum
{
    LED_STATE_OFF        = 0,
    LED_STATE_HOLD       = (1 << 0),
    LED_STATE_BLINK_FAST = (1 << 1),
    LED_STATE_BLINK_SLOW = (1 << 2),
    LED_STAT_MAX         = INT16_MAX
} led_set_state_t;

void LED_test(void);
void LED_init(void);

void LED_SetBlue(led_set_state_t newState);
void LED_SetGreen(led_set_state_t newState);
void LED_SetYellow(led_set_state_t newState);
void LED_SetRed(led_set_state_t newState);

#endif /* LED_H_ */
