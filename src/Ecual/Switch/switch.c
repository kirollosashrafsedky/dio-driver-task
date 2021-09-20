#include "../../Mcal/lib/core.h"
#include "../../Infrastructure/types.h"
#include "../../Mcal/Dio/dio.h"
#include "../../Mcal/Timer/timer.h"
#include "switch.h"
#include "switch_config.h"


// switch init
void SWITCH_init(uint8_t pin, uint8_t isInternalPulledUp)
{
	DIO_pin_init(pin, INPUT);
	if(isInternalPulledUp)
	{
		DIO_enable_pin_pullup(pin);
	}
}

// read switch
uint8_t SWITCH_read(uint8_t pin, uint8_t logic)
{
	uint8_t value;
	DIO_read_pin(pin, &value);
#if CHECK_DEBOUNCE == 1
	if(value ^ logic)
	{
		timer_delay(30);
		DIO_read_pin(pin, &value);
		if (value ^ logic)
		{
			return SWITCH_PRESSED;
		}
	}
	return SWITCH_RELEASED;
#else
	return value ^ logic;
#endif

}
