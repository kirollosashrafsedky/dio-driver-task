#ifndef __REGISTERS_H__
#define __REGISTERS_H__

#include "../../Infrastructure/types.h"


/** General **/
#define SFIOR_R 	(*(volatile uint8_t*)(0x50))

/** DIO **/
/* DDRx Registers */
#define DDRA_R 		(*(volatile uint8_t*)(0x3A))
#define DDRB_R 		(*(volatile uint8_t*)(0x37))
#define DDRC_R 		(*(volatile uint8_t*)(0x34))
#define DDRD_R 		(*(volatile uint8_t*)(0x31))

/* PORTx Registers */
#define PORTA_R		(*(volatile uint8_t*)(0x3B))
#define PORTB_R		(*(volatile uint8_t*)(0x38))
#define PORTC_R 	(*(volatile uint8_t*)(0x35))
#define PORTD_R 	(*(volatile uint8_t*)(0x32))

/* PINx Registers */
#define PINA_R 		(*(volatile uint8_t*)(0x39))
#define PINB_R 		(*(volatile uint8_t*)(0x36))
#define PINC_R 		(*(volatile uint8_t*)(0x33))
#define PIND_R 		(*(volatile uint8_t*)(0x30))

/** External Interrupts **/
#define MCUCR_R 	(*(volatile uint8_t*)(0x55))
#define MCUCSR_R 	(*(volatile uint8_t*)(0x54))
#define GICR_R 		(*(volatile uint8_t*)(0x5B))
#define GIFR_R 		(*(volatile uint8_t*)(0x5A))

/** Timers **/
#define TCCR0_R 	(*(volatile uint8_t*)(0x53))
#define TCNT0_R 	(*(volatile uint8_t*)(0x52))
#define OCR0_R 		(*(volatile uint8_t*)(0x5C))
#define TIMSK_R 	(*(volatile uint8_t*)(0x59))
#define TIFR_R 		(*(volatile uint8_t*)(0x58))
#define TCCR1A_R 	(*(volatile uint8_t*)(0x4F))
#define TCCR1B_R 	(*(volatile uint8_t*)(0x4E))
#define TCNT1L_R 	(*(volatile uint8_t*)(0x4C))
#define TCNT1H_R 	(*(volatile uint8_t*)(0x4D))
#define TCNT1_R 	(*(volatile uint16_t*)(0x4C))	//
#define OCR1AL_R 	(*(volatile uint8_t*)(0x4A))
#define OCR1AH_R 	(*(volatile uint8_t*)(0x4B))
#define OCR1A_R 	(*(volatile uint16_t*)(0x4A))	//
#define OCR1BL_R 	(*(volatile uint8_t*)(0x48))
#define OCR1BH_R 	(*(volatile uint8_t*)(0x49))
#define OCR1B_R 	(*(volatile uint16_t*)(0x48))	//
#define ICR1L_R 	(*(volatile uint8_t*)(0x46))
#define ICR1H_R 	(*(volatile uint8_t*)(0x47))
#define ICR1_R 		(*(volatile uint16_t*)(0x46))	//
#define TCCR2_R 	(*(volatile uint8_t*)(0x45))
#define TCNT2_R 	(*(volatile uint8_t*)(0x44))
#define OCR2_R 		(*(volatile uint8_t*)(0x43))
#define ASSR_R 		(*(volatile uint8_t*)(0x42))


#endif /* __REGISTERS_H__ */
