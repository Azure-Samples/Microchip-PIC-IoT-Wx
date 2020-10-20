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
#include <stddef.h>
#include "led.h"
#include "drivers/timeout.h"
#include "delay.h"
#include "debug_print.h"
#include "../main.h"

led_change_t led_change;

#define LEDS_TEST_INTERVAL  50L
#define LED_100ms_INTERVAL	timeout_mSecToTicks(100L)
#define LED_200ms_INTERVAL	timeout_mSecToTicks(200L)
#define LED_500ms_INTERVAL	timeout_mSecToTicks(500L)

static bool isSoftAp = false;
static bool isLedBlinking_blue = false;
static uint32_t blink_task_blue(void* payload);
static timerStruct_t blinkTimer_blue = { blink_task_blue };

static bool isProvisioning = false;
static bool isLedBlinking_green = false;
static uint32_t blink_task_green(void* payload);
static timerStruct_t blinkTimer_green = { blink_task_green };

static bool isLedBlinking_red = false;
static uint32_t blink_task_red(void* payload);
static timerStruct_t blinkTimer_red = { blink_task_red };

static bool isLedBlinking_yellow = false;
static uint32_t blink_task_yellow(void* payload);
static timerStruct_t blinkTimer_yellow = { blink_task_yellow };

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
	led_change.AsUSHORT = 0;
	testSequence(LED_ON);
	testSequence(LED_OFF);
}

/*************************************************
* Blue LED
*************************************************/
void LED_holdBlue(bool setLed)
{
	// if blinking, WiFi is in AP mode
	if (isSoftAp != true)
	{
		debug_printInfo("LED(B): Turn %s", setLed == LED_ON?"On":"Off");
		if (setLed == LED_ON)
		{
			LATCbits.LATC5 = 0; /* set LED_GREEN output low */
		}
		else
		{
			LATCbits.LATC5 = 1; /* set LED_GREEN output high */
		}
		led_change.blue = 1;
	}
	else
	{
		debug_printInfo("LED(B): IsBlinking");
	}

}

static uint32_t blink_task_blue(void* payload)
{
	LATCbits.LATC5 ^= 1; /* toggle LED_GREEN output */
	return isSoftAp?LED_500ms_INTERVAL:LED_200ms_INTERVAL;
}

bool LED_isBlinkingBlue(void)
{
	return (isLedBlinking_blue || isSoftAp);
}

void LED_startBlinkingBlue(bool softAp)
{
	debug_printInfo("LED(B): Start Blink : WiFi SoftAP? %s", softAp == true? "True":"False");

	if (isLedBlinking_blue)
	{
		timeout_delete(&blinkTimer_blue);
	}

	isLedBlinking_blue = true;
	isSoftAp = softAp;

	if (isSoftAp)
	{
		timeout_create(&blinkTimer_blue, LED_500ms_INTERVAL);
	}
	else
	{
		timeout_create(&blinkTimer_blue, LED_200ms_INTERVAL);
	}
	led_change.blue = 1;
}

void LED_stopBlinkingAndSetBlue(bool setLed)
{
	debug_printInfo("LED(B): Stop Blink isBlinking? %s", isLedBlinking_blue == true ? "True":"False");
	if (isLedBlinking_blue && !isSoftAp)
	{
		// don't stop blink if the device is in Soft AP mode
		timeout_delete(&blinkTimer_blue);
		isLedBlinking_blue = false;
	}
	LED_holdBlue(setLed);
}

/*************************************************
* Green LED (DPS and IoTHub)
*************************************************/
void LED_holdGreen(bool setLed)
{
	debug_printInfo("LED(G): Turn %s", setLed == LED_ON?"On":"Off");

	if (setLed == LED_ON)
	{
		LATCbits.LATC4 = 0; /* set LED_GREEN output low */
	}
	else
	{
		LATCbits.LATC4 = 1; /* set LED_GREEN output high */
	}
	led_change.green = 1;
}

static uint32_t blink_task_green(void* payload)
{
	LATCbits.LATC4 ^= 1; /* toggle LED_GREEN output */
	return isProvisioning ? LED_200ms_INTERVAL : LED_500ms_INTERVAL;
}

bool LED_isBlinkingGreen(void)
{
	return isLedBlinking_green;
}

void LED_startBlinkingGreen(bool provisioning)
{
	debug_printInfo("LED(G): Start Blink Provisioning? %s", provisioning == true?"True":"False");

	if (isLedBlinking_green)
	{
		timeout_delete(&blinkTimer_green);
	}

	isLedBlinking_green = true;
	isProvisioning = provisioning;
	if (isProvisioning)
	{
		timeout_create(&blinkTimer_green, LED_200ms_INTERVAL);
	}
	else
	{
		timeout_create(&blinkTimer_green, LED_500ms_INTERVAL);
	}

	led_change.green = 1;
}

void LED_stopBlinkingAndSetGreen(bool setLed)
{
	debug_printInfo("LED(G): Stop Blink isBlinking? %s", isLedBlinking_green == true ? "True":"False");
	if (isLedBlinking_green == true)
	{
		timeout_delete(&blinkTimer_green);
		isLedBlinking_green = false;
	}
	LED_holdGreen(setLed);
}

/*************************************************
* Red LED (Error)
*************************************************/
void LED_holdRed(bool setLed)
{
	debug_printInfo("LED(R): Turn %s", setLed == LED_ON?"On":"Off");

	if (setLed == LED_ON)
	{
		LATBbits.LATB4 = 0; /* set LED_RED output low */
	}
	else
	{
		LATBbits.LATB4 = 1; /* set LED_RED output high */
	}
	led_change.red = 1;
}

static uint32_t blink_task_red(void* payload)
{
	LATBbits.LATB4 ^= 1; /* toggle LED_RED output */
	return isLedBlinking_red ? LED_100ms_INTERVAL : 0;
}

bool LED_isBlinkingRed(void)
{
	return isLedBlinking_red;
}

void LED_startBlinkingRed(void)
{
	debug_printInfo("LED(R): Start Blink");
	timeout_create(&blinkTimer_red, LED_100ms_INTERVAL);
	isLedBlinking_red = true;
	led_change.red = 1;
}

void LED_stopBlinkingAndSetRed(bool setLed)
{
	debug_printInfo("LED(R): Stop Blink isBlinking? %s", isLedBlinking_blue== true ? "True":"False");

	timeout_delete(&blinkTimer_red);
	isLedBlinking_red = false;
	LED_holdRed(setLed);
}

/*************************************************
* Yellow LED (User LED)
*************************************************/
void LED_holdYellow(bool setLed)
{
	debug_printInfo("LED(Y): Turn %s", setLed == LED_ON?"On":"Off");
	if (setLed == LED_ON)
	{
		LATCbits.LATC3 = 0; /* set LED_RED output low */
	}
	else
	{
		LATCbits.LATC3 = 1; /* set LED_RED output high */
	}
	led_change.yellow = 1;
}

static uint32_t blink_task_yellow(void* payload)
{
	LATCbits.LATC3 ^= 1; /* toggle LED_RED output */
	return isLedBlinking_yellow ? LED_200ms_INTERVAL : 0;
}

bool LED_isBlinkingYellow(void)
{
	return isLedBlinking_yellow;
}

void LED_startBlinkingYellow(void)
{
	debug_printInfo("LED(Y): Start Blink");
	timeout_create(&blinkTimer_yellow, LED_200ms_INTERVAL);
	isLedBlinking_yellow = true;
	led_change.yellow = 1;
}

void LED_stopBlinkingAndSetYellow(bool setLed)
{
	debug_printInfo("LED(Y): Stop Blink isBlinking? %s", isLedBlinking_yellow == true ? "True":"False");

	timeout_delete(&blinkTimer_yellow);
	isLedBlinking_yellow = false;
	LED_holdYellow(setLed);
}
