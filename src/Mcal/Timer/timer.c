#include "../../Infrastructure/types.h"
#include "../lib/core.h"
#include "../lib/registers.h"
#include "../../Ecual/Led/led.h"
#include "../Dio/dio.h"
#include "timer.h"
#include "timer_config.h"

uint32_t gu32_timers_interrupt_count[TIMERS_COUNT_SUPPORTED] = { 0 };
uint32_t gu32_timers_interrupt_actual_count[TIMERS_COUNT_SUPPORTED] = { 0 };

static uint16_t ticksPerIteration(uint8_t timer, uint32_t ticks, uint16_t maxCounts);

void (*Timer0_OVF_Interrupt)(void);
void (*Timer0_CTC_Interrupt)(void);

void (*Timer1_OVF_Interrupt)(void);
void (*Timer1_CTCA_Interrupt)(void);

void (*Timer2_OVF_Interrupt)(void);
void (*Timer2_CTC_Interrupt)(void);

uint8_t timer0_ovf_start, timer2_ovf_start \
	  , timer0_init_prescaler = 0, timer1_init_prescaler = 0, timer2_init_prescaler = 0;
uint16_t timer1_ovf_start = 0;

uint8_t timer_init(ST_TIMER_config *timerConfig) {
	switch (timerConfig->timer) {
	case TIMER_0:
		if (timerConfig->ticks1 == 0 && timerConfig->mode != TIMER_0_FAST_PWM) {
			return TIMER_TICKS_ERROR;
		}
		switch (timerConfig->mode) {
		case TIMER_0_OVF:
			timer0_ovf_start = TIMER_0_MAX_COUNT - ticksPerIteration(timerConfig->timer, timerConfig->ticks1,
			TIMER_0_MAX_COUNT);
			TCNT0_R = timer0_ovf_start;
			Timer0_OVF_Interrupt = timerConfig->ptrToHandler;
			TCCR0_R &= SELECT_INV_BIT(WGM00) & SELECT_INV_BIT(WGM01) & SELECT_INV_BIT(COM00) & SELECT_INV_BIT(COM01);
			SET_BIT(TCCR0_R, FOC0);
			SET_BIT(TIMSK_R, TOIE0);
			break;
		case TIMER_0_CTC:
		case TIMER_0_CTC_TOGGLE_OC0:
			TCNT0_R = 0;
			OCR0_R = ticksPerIteration(timerConfig->timer, timerConfig->ticks1,
			TIMER_0_MAX_COUNT);
			Timer0_CTC_Interrupt = timerConfig->ptrToHandler;
			SET_BIT(TIMSK_R, OCIE0);
			TCCR0_R &= SELECT_INV_BIT(
					WGM00) & SELECT_INV_BIT(COM00) & SELECT_INV_BIT(COM01);
			TCCR0_R |= SELECT_BIT(FOC0) | SELECT_BIT(WGM01);
			if (timerConfig->mode == TIMER_0_CTC_TOGGLE_OC0) {
				DIO_pin_init(OC0, OUTPUT);
				SET_BIT(TCCR0_R, COM00);
			}
			break;
		case TIMER_0_FAST_PWM:
			if(timerConfig->ticks1 > TIMER_0_MAX_COUNT)
			{
				return TIMER_PWM_ERROR;
			}
			TCNT0_R = 0;
			OCR0_R = timerConfig->ticks1;
			TCCR0_R &= SELECT_INV_BIT(FOC0) & SELECT_INV_BIT(COM00);
			TCCR0_R |= SELECT_BIT(WGM00) | SELECT_BIT(WGM01) | SELECT_BIT(COM01);
			DIO_pin_init(OC0, OUTPUT);
			break;
		default:
			return TIMER_MOOD_NOT_DEFINED;
		}
		timer0_init_prescaler = timerConfig->prescaler;
		break;
	case TIMER_1:
		switch (timerConfig->mode) {

		case TIMER_1_OVF:
			if (timerConfig->ticks1 == 0) {
				return TIMER_TICKS_ERROR;
			}
			timer1_ovf_start = TIMER_1_MAX_COUNT - ticksPerIteration(timerConfig->timer, timerConfig->ticks1,
			TIMER_1_MAX_COUNT);
			TCNT1_R = timer1_ovf_start;
			Timer1_OVF_Interrupt = timerConfig->ptrToHandler;
			TCCR1A_R &= SELECT_INV_BIT(COM1A1) & SELECT_INV_BIT(COM1A0) & SELECT_INV_BIT(COM1B1) & SELECT_INV_BIT(COM1B0) \
					& SELECT_INV_BIT(WGM10) & SELECT_INV_BIT(WGM11);
			TCCR1A_R |= SELECT_BIT(FOC1A) | SELECT_BIT(FOC1B);
			TCCR1B_R &= SELECT_INV_BIT(WGM12) & SELECT_INV_BIT(WGM13) & SELECT_INV_BIT(ICNC1);
			SET_BIT(TIMSK_R, TOIE1);
			break;
		case TIMER_1_CTC:
		case TIMER_1_CTC_TOGGLE_OC1A:
		case TIMER_1_CTC_TOGGLE_OC1B:
			if (timerConfig->ticks1 == 0) {
				return TIMER_TICKS_ERROR;
			}
			TCNT1_R = 0;
			OCR1A_R = ticksPerIteration(timerConfig->timer, timerConfig->ticks1, TIMER_1_MAX_COUNT);
			Timer1_CTCA_Interrupt = timerConfig->ptrToHandler;
			TCCR1A_R &= SELECT_INV_BIT(COM1A1) & SELECT_INV_BIT(COM1A0) & SELECT_INV_BIT(COM1B1) & SELECT_INV_BIT(COM1B0) \
							& SELECT_INV_BIT(WGM10) & SELECT_INV_BIT(WGM11);
			TCCR1A_R |= SELECT_BIT(FOC1A) | SELECT_BIT(FOC1B);
			TCCR1B_R |= SELECT_BIT(WGM12);
			TCCR1B_R &=  SELECT_INV_BIT(WGM13) & SELECT_INV_BIT(ICNC1);
			SET_BIT(TIMSK_R, OCIE1A);
			if (timerConfig->mode == TIMER_1_CTC_TOGGLE_OC1A) {
				DIO_pin_init(OC1A, OUTPUT);
				SET_BIT(TCCR1A_R, COM1A0);
			}
			else if(timerConfig->mode == TIMER_1_CTC_TOGGLE_OC1B) {
				DIO_pin_init(OC1B, OUTPUT);
				SET_BIT(TCCR1A_R, COM1B0);
			}
			break;
		case TIMER_1_FAST_PWM_A:
		case TIMER_1_FAST_PWM_B:
		case TIMER_1_FAST_PWM_A_AND_B:
			ICR1_R = timerConfig->pwmTop;
			TCCR1A_R &= SELECT_INV_BIT(FOC1A) & SELECT_INV_BIT(FOC1B) \
					& SELECT_INV_BIT(COM1A0) & SELECT_INV_BIT(COM1B0) \
					& SELECT_INV_BIT(WGM10);
			TCCR1A_R |= SELECT_BIT(WGM11);
			TCCR1B_R |= SELECT_BIT(WGM12) | SELECT_BIT(WGM13);
			TCCR1B_R &= SELECT_INV_BIT(ICNC1);
			switch(timerConfig->mode)
			{
			case TIMER_1_FAST_PWM_A:
				OCR1A_R = timerConfig->ticks1;
				TCCR1A_R |= SELECT_BIT(COM1A1);
				TCCR1A_R &= SELECT_INV_BIT(COM1B1);
				DIO_pin_init(OC1A, OUTPUT);
				break;
			case TIMER_1_FAST_PWM_B:
				OCR1B_R = timerConfig->ticks2;
				TCCR1A_R |= SELECT_BIT(COM1B1);
				TCCR1A_R &= SELECT_INV_BIT(COM1A1);
				DIO_pin_init(OC1B, OUTPUT);
				break;
			default:
				OCR1A_R = timerConfig->ticks1;
				OCR1B_R = timerConfig->ticks2;
				TCCR1A_R |= SELECT_BIT(COM1A1) | SELECT_BIT(COM1B1);
				DIO_pin_init(OC1A, OUTPUT);
				DIO_pin_init(OC1B, OUTPUT);
				break;
			}
			break;
		default:
			return TIMER_MOOD_NOT_DEFINED;
		}
		timer1_init_prescaler = timerConfig->prescaler;
		break;
	case TIMER_2:
		if (timerConfig->ticks1 == 0 && timerConfig->mode != TIMER_2_FAST_PWM) {
			return TIMER_TICKS_ERROR;
		}
		switch (timerConfig->mode) {
		case TIMER_2_OVF:
			timer2_ovf_start = TIMER_2_MAX_COUNT - ticksPerIteration(timerConfig->timer, timerConfig->ticks1,
			TIMER_2_MAX_COUNT);
			TCNT2_R = timer2_ovf_start;
			Timer2_OVF_Interrupt = timerConfig->ptrToHandler;
			TCCR2_R &= SELECT_INV_BIT(WGM20) & SELECT_INV_BIT(WGM21) & SELECT_INV_BIT(COM20) & SELECT_INV_BIT(COM21);
			SET_BIT(TCCR2_R, FOC2);
			SET_BIT(TIMSK_R, TOIE2);
			break;
		case TIMER_2_CTC:
		case TIMER_2_CTC_TOGGLE_OC2:
			TCNT2_R = 0;
			OCR2_R = ticksPerIteration(timerConfig->timer, timerConfig->ticks1, TIMER_2_MAX_COUNT);
			Timer2_CTC_Interrupt = timerConfig->ptrToHandler;
			SET_BIT(TIMSK_R, OCIE2);
			TCCR2_R &= SELECT_INV_BIT(WGM20) & SELECT_INV_BIT(COM20) & SELECT_INV_BIT(COM21);
			TCCR2_R |= SELECT_BIT(FOC2) | SELECT_BIT(WGM21);
			if (timerConfig->mode == TIMER_2_CTC_TOGGLE_OC2) {
				DIO_pin_init(OC2, OUTPUT);
				SET_BIT(TCCR2_R, COM20);
			}
			break;
		case TIMER_2_FAST_PWM:
			if(timerConfig->ticks1 > TIMER_2_MAX_COUNT)
			{
				return TIMER_PWM_ERROR;
			}
			TCNT2_R = 0;
			OCR2_R = timerConfig->ticks1;
			TCCR2_R &= SELECT_INV_BIT(FOC2) & SELECT_INV_BIT(COM20);
			TCCR2_R |= SELECT_BIT(WGM20) | SELECT_BIT(WGM21) | SELECT_BIT(COM21);
			DIO_pin_init(OC2, OUTPUT);
			break;
		default:
			return TIMER_MOOD_NOT_DEFINED;
		}
		timer2_init_prescaler = timerConfig->prescaler;
		break;
	default:
		return TIMER_NOT_DEFINED;
	}
	return TIMER_INIT_SUCCESS;
}

void timer_start(uint8_t timer) {
	switch(timer)
	{
	case TIMER_0:
		COPY_BITS(TCCR0_R, 0b00000111, timer0_init_prescaler, 0);
		break;
	case TIMER_1:
		COPY_BITS(TCCR1B_R, 0b00000111, timer1_init_prescaler, 0);
		break;
	case TIMER_2:
		COPY_BITS(TCCR2_R, 0b00000111, timer2_init_prescaler, 0);
		break;
	default:
		break;
	}
}

void timer_stop(uint8_t timer) {
	switch(timer)
	{
	case TIMER_0:
		COPY_BITS(TCCR0_R, 0b00000111, 0, 0);
		break;
	case TIMER_1:
		COPY_BITS(TCCR1B_R, 0b00000111, 0, 0);
		break;
	case TIMER_2:
		COPY_BITS(TCCR2_R, 0b00000111, 0, 0);
		break;
	default:
		break;
	}
}

void timer_delay(float64_t time_ms) {
#if DELAY_TIMER == TIMER_0
	uint32_t u32_counts = 1, u32_loopCounter, totalTicks;
	totalTicks = TIME_MS_TO_TICKS(1, time_ms);
	if (totalTicks <= TIMER_0_MAX_COUNT)
	{
		OCR0_R = (uint8_t) (totalTicks);
	}
	else
	{
		u32_counts = (totalTicks / (TIMER_0_MAX_COUNT + 1)) + 1;
		OCR0_R = (uint8_t) ((totalTicks / u32_counts) - 1);
	}
	TCNT0_R = 0;
	TCCR0_R &= SELECT_INV_BIT(WGM00) & SELECT_INV_BIT(COM00) & SELECT_INV_BIT(COM01);
	TCCR0_R |= SELECT_BIT(FOC0) | SELECT_BIT(WGM01);
	timer0_init_prescaler = TIMER_0_PRESCALER_1;
	timer_start(TIMER_0);
	for(u32_loopCounter = 0; u32_loopCounter < u32_counts; u32_loopCounter ++)
	{
		while(BIT_IS_CLEAR(TIFR_R, OCF0));
		SET_BIT(TIFR_R, OCF0);
	}
	timer_stop(TIMER_0);
#elif DELAY_TIMER == TIMER_1
	uint32_t u32_counts = 1, u32_loopCounter, totalTicks;
	totalTicks = TIME_MS_TO_TICKS(1, time_ms);
	if (totalTicks <= TIMER_1_MAX_COUNT)
	{
		OCR1A_R = (uint16_t) (totalTicks);
	}
	else
	{
		u32_counts = (totalTicks / (TIMER_1_MAX_COUNT + 1)) + 1;
		OCR1A_R = (uint16_t) ((totalTicks / u32_counts) - 1);
	}
	TCNT1_R = 0;

	TCCR1A_R &= SELECT_INV_BIT(COM1A1) & SELECT_INV_BIT(COM1A0) & SELECT_INV_BIT(COM1B1) & SELECT_INV_BIT(COM1B0) \
			& SELECT_INV_BIT(WGM10) & SELECT_INV_BIT(WGM11);
	TCCR1A_R |= SELECT_BIT(FOC1A) | SELECT_BIT(FOC1B);
	TCCR1B_R |= SELECT_BIT(WGM12);
	TCCR1B_R &=  SELECT_INV_BIT(WGM13) & SELECT_INV_BIT(ICNC1);

	timer1_init_prescaler = TIMER_1_PRESCALER_1;
	timer_start(TIMER_1);
	for(u32_loopCounter = 0; u32_loopCounter < u32_counts; u32_loopCounter ++)
	{
		while(BIT_IS_CLEAR(TIFR_R, OCF1A));
		SET_BIT(TIFR_R, OCF1A);
	}
	timer_stop(TIMER_1);
#elif DELAY_TIMER == TIMER_2
	uint32_t u32_counts = 1, u32_loopCounter, totalTicks;
	totalTicks = TIME_MS_TO_TICKS(1, time_ms);
	if (totalTicks <= TIMER_2_MAX_COUNT)
	{
		OCR2_R = (uint8_t) (totalTicks);
	}
	else
	{
		u32_counts = (totalTicks / (TIMER_2_MAX_COUNT + 1)) + 1;
		OCR2_R = (uint8_t) ((totalTicks / u32_counts) - 1);
	}
	TCNT2_R = 0;
	TCCR2_R &= SELECT_INV_BIT(WGM20) & SELECT_INV_BIT(COM20) & SELECT_INV_BIT(COM21);
	TCCR2_R |= SELECT_BIT(FOC2) | SELECT_BIT(WGM21);
	timer2_init_prescaler = TIMER_2_PRESCALER_1;
	timer_start(TIMER_2);
	for(u32_loopCounter = 0; u32_loopCounter < u32_counts; u32_loopCounter ++)
	{
		while(BIT_IS_CLEAR(TIFR_R, OCF2));
		SET_BIT(TIFR_R, OCF2);
	}
	timer_stop(TIMER_2);
#endif
}

static uint16_t ticksPerIteration(uint8_t timer, uint32_t ticks, uint16_t maxCounts) {
	uint32_t u32_counts;
	float64_t f64_counts_temp;
	if (ticks <= maxCounts) {
		gu32_timers_interrupt_count[timer] = 1;
		gu32_timers_interrupt_actual_count[timer] = 1;
		return (uint16_t) (ticks - 1);
	}
	f64_counts_temp = (float64_t) ticks / (maxCounts + 1);
	u32_counts = (uint32_t) f64_counts_temp;
	if (u32_counts != f64_counts_temp) {
		u32_counts++;
	}
	gu32_timers_interrupt_count[timer] = u32_counts;
	return (uint16_t) ((ticks / u32_counts) - 1);
}

ISR(TIMER0_COMP_vect) {
	if (gu32_timers_interrupt_actual_count[TIMER_0] == gu32_timers_interrupt_count[TIMER_0]) {
		(*Timer0_CTC_Interrupt)();
		gu32_timers_interrupt_actual_count[TIMER_0] = 1;
	} else {
		gu32_timers_interrupt_actual_count[TIMER_0]++;
	}
}

ISR(TIMER0_OVF_vect) {
	TCNT0_R = timer0_ovf_start;
	if (gu32_timers_interrupt_actual_count[TIMER_0] == gu32_timers_interrupt_count[TIMER_0]) {
		(*Timer0_OVF_Interrupt)();
		gu32_timers_interrupt_actual_count[TIMER_0] = 1;
	} else {
		gu32_timers_interrupt_actual_count[TIMER_0]++;
	}
}

ISR(TIMER2_COMP_vect) {
	if (gu32_timers_interrupt_actual_count[TIMER_2] == gu32_timers_interrupt_count[TIMER_2]) {
		(*Timer2_CTC_Interrupt)();
		gu32_timers_interrupt_actual_count[TIMER_2] = 1;
	} else {
		gu32_timers_interrupt_actual_count[TIMER_2]++;
	}
}

ISR(TIMER2_OVF_vect) {
	TCNT0_R = timer2_ovf_start;
	if (gu32_timers_interrupt_actual_count[TIMER_2] == gu32_timers_interrupt_count[TIMER_2]) {
		(*Timer2_OVF_Interrupt)();
		gu32_timers_interrupt_actual_count[TIMER_2] = 1;
	} else {
		gu32_timers_interrupt_actual_count[TIMER_2]++;
	}
}

ISR(TIMER1_OVF_vect) {
	TCNT1_R = timer1_ovf_start;
	LED_toggle(PA2);
	if (gu32_timers_interrupt_actual_count[TIMER_1] == gu32_timers_interrupt_count[TIMER_1]) {
		(*Timer1_OVF_Interrupt)();
		gu32_timers_interrupt_actual_count[TIMER_1] = 1;
	} else {
		gu32_timers_interrupt_actual_count[TIMER_1]++;
	}
}

ISR(TIMER1_COMPA_vect) {
	if (gu32_timers_interrupt_actual_count[TIMER_1] == gu32_timers_interrupt_count[TIMER_1]) {
		(*Timer1_CTCA_Interrupt)();
		gu32_timers_interrupt_actual_count[TIMER_1] = 1;
	} else {
		gu32_timers_interrupt_actual_count[TIMER_1]++;
	}
}

