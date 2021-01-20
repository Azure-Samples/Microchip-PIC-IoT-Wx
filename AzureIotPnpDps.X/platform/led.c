/*
    \file   led.c

    \brief  Manage board LED's

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
#include <xc.h>
#include <stdbool.h>
#include "led.h"
#include "drivers/timeout.h"
#include "delay.h"
#include "debug_print.h"
#include "../main.h"

led_status_t led_status;

#define LOW   0
#define HIGH  1

#define LEDS_TEST_INTERVAL  50L
#define LED_100ms_INTERVAL  timeout_mSecToTicks(100L)
#define LED_400ms_INTERVAL  timeout_mSecToTicks(400L)

static uint32_t blink_task(void* payload);
static timerStruct_t blinkTimer_blue = { blink_task, (void*)LED_BLUE };
static timerStruct_t blinkTimer_green = { blink_task, (void*)LED_GREEN};
static timerStruct_t blinkTimer_yellow = { blink_task, (void*)LED_YELLOW};
static timerStruct_t blinkTimer_red = { blink_task, (void*)LED_RED};

const char* debug_led_state[] =
{
    "Off",
    "On",
    "Blink(Fast)",
    "N/A",
    "Blink(Slow)"
};

static void testSequence(uint8_t ledState)
{
    if (ledState == LED_ON)
    {
        LATCbits.LATC5 = 0; /* set LED_BLUE output low */
        DELAY_milliseconds(LEDS_TEST_INTERVAL);
        LATCbits.LATC4 = 0; /* set LED_GREEN output low */
        DELAY_milliseconds(LEDS_TEST_INTERVAL);
        LATCbits.LATC3 = 0; /* set LED_YELLOW output low */
        DELAY_milliseconds(LEDS_TEST_INTERVAL);
        LATBbits.LATB4 = 0; /* set LED_RED output low */
        DELAY_milliseconds(LEDS_TEST_INTERVAL);
    }
    else
    {
        LATCbits.LATC5 = 1; /* set LED_BLUE output high */
        DELAY_milliseconds(LEDS_TEST_INTERVAL);
        LATCbits.LATC4 = 1; /* set LED_GREEN output high */
        DELAY_milliseconds(LEDS_TEST_INTERVAL);
        LATCbits.LATC3 = 1; /* set LED_YELLOW output high */
        DELAY_milliseconds(LEDS_TEST_INTERVAL);
        LATBbits.LATB4 = 1; /* set LED_RED output high */
        DELAY_milliseconds(LEDS_TEST_INTERVAL);
    }
}

void LED_test(void)
{
    testSequence(LED_ON);
    testSequence(LED_OFF);
}

void LED_init(void)
{
    led_status.change_flag.AsUSHORT = 0;
    led_status.state_flag.AsUSHORT = 0;

    led_status.state_flag.blue = LED_STATE_OFF;
    LATCbits.LATC5 = HIGH; /* set LED_BLUE output high */
    led_status.state_flag.green = LED_STATE_OFF;
    LATCbits.LATC4 = HIGH; /* set LED_GREEN output high */
    led_status.state_flag.yellow = LED_STATE_OFF;
    LATCbits.LATC3 = HIGH; /* set LED_YELLOW output high */
    led_status.state_flag.red = LED_STATE_OFF;
    LATBbits.LATB4 = HIGH; /* set LED_RED output high */
}

static uint32_t blink_task(void* payload)
{
    led_number_t led = (led_number_t)payload;
    uint32_t timeout = LED_100ms_INTERVAL;

    switch (led)
    {
        case LED_BLUE:
            LATCbits.LATC5 ^= 1; /* toggle LED_BLUE output */
            timeout = led_status.state_flag.blue == LED_STATE_BLINK_FAST ? LED_100ms_INTERVAL : LED_400ms_INTERVAL;
            break;
        case LED_GREEN:
            LATCbits.LATC4 ^= 1; /* toggle LED_GREEN output */
            timeout = led_status.state_flag.green == LED_STATE_BLINK_FAST ? LED_100ms_INTERVAL : LED_400ms_INTERVAL;
            break;
        case LED_YELLOW:
            LATCbits.LATC3 ^= 1; /* toggle LED_YELLOW output */
            timeout = led_status.state_flag.yellow == LED_STATE_BLINK_FAST ? LED_100ms_INTERVAL : LED_400ms_INTERVAL;
            break;
        case LED_RED:
            LATBbits.LATB4 ^= 1; /* toggle LED_RED output */
            timeout = led_status.state_flag.red == LED_STATE_BLINK_FAST ? LED_100ms_INTERVAL : LED_400ms_INTERVAL;
            break;
    }

    return timeout;
}

/*************************************************
*
* Blue LED
*
*************************************************/

void LED_SetBlue(led_set_state_t newState)
{
    timerStruct_t* timer = &blinkTimer_blue;

    debug_printInfo("LED(B): %s => %s", debug_led_state[led_status.state_flag.blue], debug_led_state[newState]);

    if (led_status.state_flag.blue == newState)
    {
        return;
    }

    switch ((int32_t)led_status.state_flag.blue)
    {
        case LED_STATE_OFF:
        case LED_STATE_HOLD:
            if ((newState & (LED_STATE_BLINK_FAST | LED_STATE_BLINK_SLOW)) != 0)
            {
                timeout_create(timer, newState == LED_STATE_BLINK_FAST? LED_100ms_INTERVAL : LED_400ms_INTERVAL);
            }

            break;

        case LED_STATE_BLINK_FAST:

            if (newState == LED_STATE_HOLD || newState == LED_STATE_OFF)
            {
                timeout_delete(timer);
            }
            break;

        case LED_STATE_BLINK_SLOW:

            if (newState == LED_STATE_HOLD || newState == LED_STATE_OFF)
            {
                timeout_delete(timer);
            }

            break;

        default:
            break;
    }

    if (newState == LED_STATE_HOLD)
    {
        LATCbits.LATC5 = LOW;
    }
    else if (newState == LED_STATE_OFF)
    {
        LATCbits.LATC5 = HIGH;
    }

    led_status.state_flag.blue = newState;
    led_status.change_flag.blue = 1;
}

/*************************************************
*
* Green LED
*
*************************************************/

void LED_SetGreen(led_set_state_t newState)
{
    timerStruct_t* timer = &blinkTimer_green;

    debug_printInfo("LED(G): %s => %s", debug_led_state[led_status.state_flag.green], debug_led_state[newState]);

    if (led_status.state_flag.green == newState)
    {
        return;
    }

    switch ((int32_t)led_status.state_flag.green)
    {
        case LED_STATE_OFF:
        case LED_STATE_HOLD:
            if ((newState & (LED_STATE_BLINK_FAST | LED_STATE_BLINK_SLOW)) != 0)
            {
                timeout_create(timer, newState == LED_STATE_BLINK_FAST? LED_100ms_INTERVAL : LED_400ms_INTERVAL);
            }

            break;

        case LED_STATE_BLINK_FAST:

            if (newState == LED_STATE_HOLD || newState == LED_STATE_OFF)
            {
                timeout_delete(timer);
            }
            break;

        case LED_STATE_BLINK_SLOW:

            if (newState == LED_STATE_HOLD || newState == LED_STATE_OFF)
            {
                timeout_delete(timer);
            }

            break;

        default:
            break;
    }

    if (newState == LED_STATE_HOLD)
    {
        LATCbits.LATC4 = LOW;
    }
    else if (newState == LED_STATE_OFF)
    {
        LATCbits.LATC4 = HIGH;
    }

    led_status.state_flag.green = newState;
    led_status.change_flag.green = 1;
}

/*************************************************
*
* Yellow LED
*
*************************************************/

void LED_SetYellow(led_set_state_t newState)
{
    timerStruct_t* timer = &blinkTimer_yellow;

    debug_printInfo("LED(Y): %s => %s", debug_led_state[led_status.state_flag.yellow], debug_led_state[newState]);

    if (led_status.state_flag.yellow == newState)
    {
        return;
    }

    switch ((int32_t)led_status.state_flag.yellow)
    {
        case LED_STATE_OFF:
        case LED_STATE_HOLD:
            if ((newState & (LED_STATE_BLINK_FAST | LED_STATE_BLINK_SLOW)) != 0)
            {
                timeout_create(timer, newState == LED_STATE_BLINK_FAST? LED_100ms_INTERVAL : LED_400ms_INTERVAL);
            }

            break;

        case LED_STATE_BLINK_FAST:

            if (newState == LED_STATE_HOLD || newState == LED_STATE_OFF)
            {
                timeout_delete(timer);
            }
            break;

        case LED_STATE_BLINK_SLOW:

            if (newState == LED_STATE_HOLD || newState == LED_STATE_OFF)
            {
                timeout_delete(timer);
            }

            break;

        default:
            break;
    }

    if (newState == LED_STATE_HOLD)
    {
        LATCbits.LATC3 = LOW;
    }
    else if (newState == LED_STATE_OFF)
    {
        LATCbits.LATC3 = HIGH;
    }

    led_status.state_flag.yellow = newState;
    led_status.change_flag.yellow = 1;
}

/*************************************************
*
* Red LED
*
*************************************************/

void LED_SetRed(led_set_state_t newState)
{
    timerStruct_t* timer = &blinkTimer_red;

    debug_printInfo("LED(R): %s => %s", debug_led_state[led_status.state_flag.red], debug_led_state[newState]);

    if (led_status.state_flag.red == newState)
    {
        return;
    }

    switch ((int32_t)led_status.state_flag.red)
    {
        case LED_STATE_OFF:
        case LED_STATE_HOLD:
            if ((newState & (LED_STATE_BLINK_FAST | LED_STATE_BLINK_SLOW)) != 0)
            {
                timeout_create(timer, newState == LED_STATE_BLINK_FAST? LED_100ms_INTERVAL : LED_400ms_INTERVAL);
            }

            break;

        case LED_STATE_BLINK_FAST:

            if (newState == LED_STATE_HOLD || newState == LED_STATE_OFF)
            {
                timeout_delete(timer);
            }
            break;

        case LED_STATE_BLINK_SLOW:

            if (newState == LED_STATE_HOLD || newState == LED_STATE_OFF)
            {
                timeout_delete(timer);
            }

            break;

        default:
            break;
    }

    if (newState == LED_STATE_HOLD)
    {
        LATBbits.LATB4 = LOW;
    }
    else if (newState == LED_STATE_OFF)
    {
        LATBbits.LATB4 = HIGH;
    }

    led_status.state_flag.red = newState;
    led_status.change_flag.red = 1;
}
