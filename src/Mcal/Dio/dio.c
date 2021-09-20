#include "../lib/core.h"
#include "../lib/registers.h"
#include "dio.h"
#include "../../Infrastructure/types.h"

volatile uint8_t * gu8_DDRx_ptrs[PORTS_NO] = {&DDRA_R, &DDRB_R, &DDRC_R, &DDRD_R};
volatile uint8_t * gu8_PORTx_ptrs[PORTS_NO] = {&PORTA_R, &PORTB_R, &PORTC_R, &PORTD_R};
volatile uint8_t * gu8_PINx_ptrs[PORTS_NO] = {&PINA_R, &PINB_R, &PINC_R, &PIND_R};

// init DIO port
void DIO_port_init(uint8_t port, uint8_t direction)
{
	if(direction == INPUT)
	{
		*gu8_DDRx_ptrs[port] = ALL_INPUT;
	}
	else
	{
		*gu8_DDRx_ptrs[port] = ALL_OUTPUT;
	}
}

// init DIO pin
void DIO_pin_init(uint8_t pin, uint8_t direction)
{
	uint8_t port = GET_PORT_NO(pin), pinNo = GET_PIN_NO(pin);
	if(direction == INPUT)
	{
		CLEAR_BIT(*gu8_DDRx_ptrs[port], pinNo);
	}
	else
	{
		SET_BIT(*gu8_DDRx_ptrs[port], pinNo);
	}
}

// enable port pull up
void DIO_enable_port_pullup(uint8_t port)
{
	if(*gu8_DDRx_ptrs[port] == ALL_INPUT)
	{
		*gu8_PORTx_ptrs[port] = ALL_HIGH;
	}
}

// enable pin pull up
void DIO_enable_pin_pullup(uint8_t pin)
{
	uint8_t port = GET_PORT_NO(pin), pinNo = GET_PIN_NO(pin);
	if(!GET_BIT(*gu8_DDRx_ptrs[port], pinNo))
	{
		SET_BIT(*gu8_PORTx_ptrs[port], pinNo);
	}
}

// disable port pull up
void DIO_disable_port_pullup(uint8_t port)
{
	if(*gu8_DDRx_ptrs[port] == ALL_INPUT)
	{
		*gu8_PORTx_ptrs[port] = ALL_LOW;
	}
}

// disable pin pull up
void DIO_disable_pin_pullup(uint8_t pin)
{
	uint8_t port = GET_PORT_NO(pin), pinNo = GET_PIN_NO(pin);
	if(!GET_BIT(*gu8_DDRx_ptrs[port], pinNo))
	{
		CLEAR_BIT(*gu8_PORTx_ptrs[port], pinNo);
	}
}

// enable global pull up
void DIO_enable_global_pullup(void)
{
	CLEAR_BIT(SFIOR_R, PUD);
}

// disable global pull up
void DIO_disable_global_pullup(void)
{
	SET_BIT(SFIOR_R, PUD);
}

// Write value to DIO port
void DIO_write_port(uint8_t port, uint8_t data)
{
	if(*gu8_DDRx_ptrs[port] == ALL_OUTPUT)
	{
		if(data)
		{
			*gu8_PORTx_ptrs[port] = ALL_HIGH;
		}
		else
		{
			*gu8_PORTx_ptrs[port] = ALL_LOW;
		}
	}
}

// Write value to DIO pin
void DIO_write_pin(uint8_t pin, uint8_t data)
{
	uint8_t port = GET_PORT_NO(pin), pinNo = GET_PIN_NO(pin);
	if(GET_BIT(*gu8_DDRx_ptrs[port], pinNo))
	{
		if(data)
		{
			SET_BIT(*gu8_PORTx_ptrs[port], pinNo);
		}
		else
		{
			CLEAR_BIT(*gu8_PORTx_ptrs[port], pinNo);
		}
	}
}

// Read value from DIO port
void DIO_read_port(uint8_t port, uint8_t* data)
{
	*data = *gu8_PINx_ptrs[port];
}

// Read value from DIO pin
void DIO_read_pin(uint8_t pin, uint8_t* data)
{
	uint8_t port = GET_PORT_NO(pin), pinNo = GET_PIN_NO(pin);
	if(GET_BIT(*gu8_PINx_ptrs[port], pinNo))
	{
		*data = HIGH;
	}
	else
	{
		*data = LOW;
	}
}

// Toggle DIO port
void DIO_toggle_port(uint8_t port)
{
	if(*gu8_DDRx_ptrs[port] == ALL_OUTPUT)
	{
		*gu8_PORTx_ptrs[port] ^= ALL_HIGH;
	}
}

// Toggle DIO pin
void DIO_toggle_pin(uint8_t pin)
{
	uint8_t port = GET_PORT_NO(pin), pinNo = GET_PIN_NO(pin);
	if(GET_BIT(*gu8_DDRx_ptrs[port], pinNo))
	{

		TOGGLE_BIT(*gu8_PORTx_ptrs[port], pinNo);

	}
}
