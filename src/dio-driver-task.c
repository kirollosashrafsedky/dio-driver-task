/* Basic */
#include "dio-driver-task.h"
#include "Mcal/lib/core.h"
#include "Mcal/lib/registers.h"
#include "Infrastructure/types.h"

/* MCAL */
#include "Mcal/Dio/dio.h"

/* ECUAL */
#include "Ecual/Led/led.h"
#include "Ecual/Switch/switch.h"

int main(void) {
	LED_init(PA0, LED_OFF, LED_POSITIVE_LOGIC);
	SWITCH_init(PC0, FALSE);

	uint8_t ledOn = FALSE;
	while(1)
	{
		if(SWITCH_read(PC0, SWITCH_POSITIVE_LOGIC))
		{
			if(ledOn == FALSE)
			{
				LED_toggle(PA0);
				ledOn = TRUE;
			}
		}
		else
		{
			ledOn = FALSE;
		}

	}
}

