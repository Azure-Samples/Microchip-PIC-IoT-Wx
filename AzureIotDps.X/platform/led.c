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

#define LEDS_TEST_INTERVAL        50L
#define LED_ON_INTERVAL           timeout_mSecToTicks(200L)
#define LED_ON_INTERVAL_FAST      timeout_mSecToTicks(100L)
#define LEDS_HOLD_INTERVAL	      timeout_mSecToTicks(2000L)

static bool ledForHubConnection = false;
static bool ledYellowHeld = false;
static bool isRedLedFlashing = false;

static uint32_t redblink_task(void* payload);
static timerStruct_t redBlink_timer = { redblink_task };

static uint32_t yellow_task(void* payload);
static timerStruct_t yellow_timer = { yellow_task };

static uint32_t red_task(void* payload);
static timerStruct_t red_timer = { red_task };

static uint32_t hubConnection_task(void* payload);
static timerStruct_t hubConnection_timer = { hubConnection_task };

static uint32_t softAp_task(void* payload);
static timerStruct_t softAP_timer = { softAp_task };

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

static uint32_t yellow_task(void* payload)
{
	LATCbits.LATC3 = 1; /* set LED_YELLOW output high */
	ledYellowHeld = false;
	return 0;
}

static uint32_t red_task(void* payload)
{
	isRedLedFlashing = false;
	LATBbits.LATB4 = 1; /* set LED_RED output high */
	return 0;
}

static uint32_t redblink_task(void* payload)
{
	LATBbits.LATB4 ^= 1; /* toggle LED_RED output */
	return isRedLedFlashing ? LED_ON_INTERVAL_FAST : 0;
}

static uint32_t softAp_task(void* payload)
{
	LATCbits.LATC5 ^= 1; /* toggle LED_BLUE output */
	return LED_ON_INTERVAL;
}

static uint32_t hubConnection_task(void* payload)
{
	LATCbits.LATC4 ^= 1; /* toggle LED_GREEN output */
	return LED_ON_INTERVAL;
}

void LED_flashYellow(void)
{
	if (ledYellowHeld == false)
	{
		LATCbits.LATC3 = 0; /* set LED_YELLOW output low */
		timeout_create(&yellow_timer, LED_ON_INTERVAL);
	}

}

void LED_holdYellowOn(bool holdHigh)
{
	if (holdHigh == true)
	{
		LATCbits.LATC3 = 0; /* set LED_YELLOW output low */
	}
	else
	{
		LATCbits.LATC3 = 1; /* set LED_YELLOW output high */
	}
	// Re-Use yellow_timer task
	ledYellowHeld = true;
	timeout_create(&yellow_timer, LEDS_HOLD_INTERVAL);
}

void LED_holdGreenOn(bool holdHigh)
{
	if (holdHigh == LED_ON)
	{
		LATCbits.LATC4 = 0; /* set LED_GREEN output low */
	}
	else
	{
		LATCbits.LATC4 = 1; /* set LED_GREEN output high */
	}
}

void LED_flashRed(double interval_seconds)
{
	isRedLedFlashing = true;
	long interval_ms = LED_ON_INTERVAL;
	if (interval_seconds > 0)
	{
		interval_ms = timeout_mSecToTicks(interval_seconds * 1000L);
	}
	LATBbits.LATB4 = 0; /* set LED_RED output low */
	timeout_create(&red_timer, interval_ms);
}

bool LED_isBlinkingRed(void)
{
	return isRedLedFlashing;
}

void LED_stopBlinkingRed(void)
{
	isRedLedFlashing = false;
}

void LED_startBlinkingRed(void)
{
	timeout_create(&redBlink_timer, LED_ON_INTERVAL_FAST);
	isRedLedFlashing = true;
}

void LED_holdRedOn(bool holdHigh)
{
	if (holdHigh == LED_ON)
	{
		LATBbits.LATB4 = 0; /* set LED_RED output low */
	}
	else
	{
		LATBbits.LATB4 = 1; /* set LED_RED output high */
	}
}

void LED_blinkingBlue(bool amBlinking)
{
	if (amBlinking == true)
	{
		timeout_create(&softAP_timer, LED_ON_INTERVAL);
	}
	else
	{
		timeout_delete(&softAP_timer);
	}
}

void LED_startBlinkingGreen(void)
{
	timeout_create(&hubConnection_timer, LED_ON_INTERVAL);
	ledForHubConnection = true;
}

void LED_stopBlinkingGreen(void)
{
	if (ledForHubConnection == true)
	{
		timeout_delete(&hubConnection_timer);
		ledForHubConnection = false;
	}
}

bool LED_isBlinkingGreen(void)
{
	return ledForHubConnection;
}