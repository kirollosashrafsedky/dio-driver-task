#ifndef __TIMER_H__
#define __TIMER_H__

#include "../../Infrastructure/types.h"

#define TIMERS_COUNT_SUPPORTED			3

#define TIMER_0 						0
#define TIMER_0_BITS 					8
#define TIMER_0_MAX_COUNT 				255

#define TIMER_1 						1
#define TIMER_1_BITS 					16
#define TIMER_1_MAX_COUNT 				65535

#define TIMER_2 						2
#define TIMER_2_BITS 					8
#define TIMER_2_MAX_COUNT 				255

#define TIMER_INIT_SUCCESS				0
#define TIMER_NOT_DEFINED 				1
#define TIMER_MOOD_NOT_DEFINED			2
#define TIMER_TICKS_ERROR 				3
#define TIMER_PWM_ERROR 				4

#define TIME_MS_TO_TICKS(prescaler,time) ((uint32_t)(((time) / 1000.0) / ((float64_t)(prescaler) / F_CPU)))

typedef enum EN_TIMER_0_modes{
	TIMER_0_OVF = 0x00,
	TIMER_0_CTC,
	TIMER_0_CTC_TOGGLE_OC0,
	TIMER_0_FAST_PWM
}EN_TIMER_0_modes;

typedef enum EN_TIMER_1_modes{
	TIMER_1_OVF = 0x10,
	TIMER_1_CTC,
	TIMER_1_CTC_TOGGLE_OC1A,
	TIMER_1_CTC_TOGGLE_OC1B,
	TIMER_1_FAST_PWM_A,
	TIMER_1_FAST_PWM_B,
	TIMER_1_FAST_PWM_A_AND_B,
}EN_TIMER_1_modes;

typedef enum EN_TIMER_2_modes{
	TIMER_2_OVF = 0x20,
	TIMER_2_CTC,
	TIMER_2_CTC_TOGGLE_OC2,
	TIMER_2_FAST_PWM
}EN_TIMER_2_modes;

typedef enum EN_TIMER_0_prescalers{
	TIMER_0_PRESCALER_1 = 1,
	TIMER_0_PRESCALER_8,
	TIMER_0_PRESCALER_64,
	TIMER_0_PRESCALER_256,
	TIMER_0_PRESCALER_1024,
	TIMER_0_PRESCALER_EXT_FALLIING,
	TIMER_0_PRESCALER_EXT_RISING,
}EN_TIMER_0_prescalers;

typedef enum EN_TIMER_1_prescalers{
	TIMER_1_PRESCALER_1 = 1,
	TIMER_1_PRESCALER_8,
	TIMER_1_PRESCALER_64,
	TIMER_1_PRESCALER_256,
	TIMER_1_PRESCALER_1024,
	TIMER_1_PRESCALER_EXT_FALLIING,
	TIMER_1_PRESCALER_EXT_RISING,
}EN_TIMER_1_prescalers;

typedef enum EN_TIMER_2_prescalers{
	TIMER_2_PRESCALER_1 = 1,
	TIMER_2_PRESCALER_8,
	TIMER_2_PRESCALER_32,
	TIMER_2_PRESCALER_64,
	TIMER_2_PRESCALER_128,
	TIMER_2_PRESCALER_256,
	TIMER_2_PRESCALER_1024,
}EN_TIMER_2_prescalers;

typedef struct ST_TIMER_config{
	uint8_t timer;
	uint8_t mode;
	uint8_t prescaler;
	uint32_t ticks1;
	uint32_t ticks2;
	uint16_t pwmTop;
	void (* ptrToHandler)(void);
}ST_TIMER_config;

uint8_t timer_init(ST_TIMER_config* timerConfig);

void timer_start(uint8_t timer);

void timer_stop(uint8_t timer);

void timer_delay(float64_t time_ms);

#endif /* __TIMER_H__ */
