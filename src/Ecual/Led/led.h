#ifndef __LED_H__
#define __LED_H__

#include "../../Infrastructure/types.h"

#define LED_POSITIVE_LOGIC 1
#define LED_NEGATIVE_LOGIC 0

#define LED_NOT_FOUND -1

typedef enum ST_LED_state{
	LED_OFF = 0,
	LED_ON
}ST_LED_state;

typedef struct ST_LED{
	uint8_t pin;
	ST_LED_state state;
	uint8_t logic;
}ST_LED;

// init the led
void LED_init(uint8_t pin, uint8_t initialState, uint8_t logic);

// led on
void LED_on(uint8_t pin);

// led off
void LED_off(uint8_t pin);

// led toggle
void LED_toggle(uint8_t pin);

// get led state toggle
void LED_state(uint8_t pin, uint8_t *state);


#endif /* __LED_H__ */
