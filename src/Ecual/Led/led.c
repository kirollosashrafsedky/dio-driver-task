#include "../../Mcal/lib/core.h"
#include "../../Infrastructure/types.h"
#include "../../Mcal/Dio/dio.h"
#include "led.h"
#include "led_config.h"

// return led structure index in array
static uint8_t LED_get(uint8_t pin);

ST_LED garr_leds[LEDS_COUNT];
uint8_t gu8_ledsArrCount = 0;

// led init
void LED_init(uint8_t pin, uint8_t initialState, uint8_t logic)
{
	garr_leds[gu8_ledsArrCount].pin = pin;
	garr_leds[gu8_ledsArrCount].state = initialState;
	garr_leds[gu8_ledsArrCount].logic = logic;
	gu8_ledsArrCount ++;
	DIO_pin_init(pin, OUTPUT);
	if(initialState == LED_ON)
	{
		LED_on(pin);
	}
	else
	{
		LED_off(pin);
	}
}

// led on
void LED_on(uint8_t pin)
{
	uint8_t ledIndex = LED_get(pin);
	if(ledIndex != LED_NOT_FOUND)
	{
		if(garr_leds[ledIndex].logic == LED_NEGATIVE_LOGIC)
		{
			DIO_write_pin(pin, LOW);
		}
		else
		{
			DIO_write_pin(pin, HIGH);
		}
		garr_leds[ledIndex].state = LED_ON;
	}
}

// led off
void LED_off(uint8_t pin)
{
	uint8_t ledIndex = LED_get(pin);
	if(ledIndex != LED_NOT_FOUND)
	{
		if(garr_leds[ledIndex].logic == LED_NEGATIVE_LOGIC)
		{
			DIO_write_pin(pin, HIGH);
		}
		else
		{
			DIO_write_pin(pin, LOW);
		}
		garr_leds[ledIndex].state = LED_OFF;
	}
}

// led toggle
void LED_toggle(uint8_t pin)
{
	uint8_t ledIndex = LED_get(pin);
	if(ledIndex != LED_NOT_FOUND)
	{
		DIO_toggle_pin(pin);
		if(garr_leds[ledIndex].state == LED_ON)
		{
			garr_leds[ledIndex].state = LED_OFF;
		}
		else
		{
			garr_leds[ledIndex].state = LED_ON;
		}
	}
}

// get led state toggle
void LED_state(uint8_t pin, uint8_t *state)
{
	uint8_t ledIndex = LED_get(pin);
	if(ledIndex != LED_NOT_FOUND)
	{
		*state = garr_leds[ledIndex].state;
	}
}

// return led struct index in array
static uint8_t LED_get(uint8_t pin)
{
	for(uint8_t counter = 0; counter < gu8_ledsArrCount; counter++)
	{
		if(garr_leds[counter].pin == pin)
		{
			return counter;
		}
	}
	return LED_NOT_FOUND;
}

