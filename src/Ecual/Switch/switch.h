#ifndef __SWITCH_H__
#define __SWITCH_H__

#include "../../Infrastructure/types.h"
#include "../../Mcal/Timer/timer.h"

#define SWITCH_POSITIVE_LOGIC 			0
#define SWITCH_NEGATIVE_LOGIC 			1

typedef enum ST_SWITCH_state{
	SWITCH_RELEASED = 0,
	SWITCH_PRESSED = 1,
}ST_SWITCH_state;


// init the switch
void SWITCH_init(uint8_t pin, uint8_t isInternalPulledUp);

// read switch
uint8_t SWITCH_read(uint8_t pin, uint8_t logic);

#endif /* __SWITCH_H__ */
