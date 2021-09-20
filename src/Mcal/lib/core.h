#ifndef __CORE_H__
#define __CORE_H__

#define NULL (void *) (0x0)

#define TRUE	1
#define FALSE	0

#define OUTPUT 	1
#define INPUT 	0

#define ALL_OUTPUT 0xFF
#define ALL_INPUT 0x00

#define HIGH 1
#define LOW 0

#define ALL_HIGH 0xFF
#define ALL_LOW 0x00

/** PORTS **/

#define PORTS_NO 4

#define PORTA 0
#define PORTB 1
#define PORTC 2
#define PORTD 3

/** PINS **/
/* PORTA PINS */
#define PA0 (0x00)
#define PA1 (0x01)
#define PA2 (0x02)
#define PA3 (0x03)
#define PA4 (0x04)
#define PA5 (0x05)
#define PA6 (0x06)
#define PA7 (0x07)

/* PORTB PINS */
#define PB0 (0x10)
#define PB1 (0x11)
#define PB2 (0x12)
#define PB3 (0x13)
#define PB4 (0x14)
#define PB5 (0x15)
#define PB6 (0x16)
#define PB7 (0x17)

/* PORTC PINS */
#define PC0 (0x20)
#define PC1 (0x21)
#define PC2 (0x22)
#define PC3 (0x23)
#define PC4 (0x24)
#define PC5 (0x25)
#define PC6 (0x26)
#define PC7 (0x27)

/* PORTD PINS */
#define PD0 (0x30)
#define PD1 (0x31)
#define PD2 (0x32)
#define PD3 (0x33)
#define PD4 (0x34)
#define PD5 (0x35)
#define PD6 (0x36)
#define PD7 (0x37)

/* Bits operations */
#define SET_BIT(reg,bit) ((reg) |= (1 << (bit)))

#define CLEAR_BIT(reg,bit) ((reg) &= ~(1 << (bit)))

#define TOGGLE_BIT(reg,bit) ((reg) ^= (1 << (bit)))

#define COPY_BITS(reg,mask,src,shift) ((reg) = ((reg) & ((0xFF ^ (mask)) \
<< (shift))) | (((src) & (mask)) << (shift)))

#define BIT_IS_SET(reg,bit) ((reg) & (1 << (bit)))

#define BIT_IS_CLEAR(reg,bit) (!((reg) & (1 << (bit))))

#define GET_BIT(reg,bit) (((reg) & (1 << (bit))) >> (bit))

#define GET_PIN_NO(pin) ((pin) & 0x0F)

#define GET_PORT_NO(pin) ((pin) >> 4)

#define SELECT_BIT(bit) (1 << (bit))

#define SELECT_INV_BIT(bit) (~(1 << (bit)))

/* Interrupts */
#define ENABLE_GLOBAL_INTERRUPT()  __asm__ __volatile__ ("sei" ::)
#define DISABLE_GLOBAL_INTERRUPT()  __asm__ __volatile__ ("cli" ::)

#define INT0_vect				_VECTOR(1)
#define INT1_vect				_VECTOR(2)
#define INT2_vect				_VECTOR(3)
#define TIMER2_COMP_vect		_VECTOR(4)
#define TIMER2_OVF_vect			_VECTOR(5)
#define TIMER1_CAPT_vect		_VECTOR(6)
#define TIMER1_COMPA_vect		_VECTOR(7)
#define TIMER1_COMPB_vect		_VECTOR(8)
#define TIMER1_OVF_vect			_VECTOR(9)
#define TIMER0_COMP_vect		_VECTOR(10)
#define TIMER0_OVF_vect			_VECTOR(11)
#define SPI_STC_vect			_VECTOR(12)
#define USART_RXC_vect			_VECTOR(13)
#define USART_UDRE_vect			_VECTOR(14)
#define USART_TXC_vect			_VECTOR(15)
#define ADC_vect				_VECTOR(16)
#define EE_RDY_vect				_VECTOR(17)
#define ANA_COMP_vect			_VECTOR(18)
#define TWI_vect				_VECTOR(19)
#define SPM_RDY_vect			_VECTOR(20)

/* Interrupts - copied from <avr/interrupt.h> */

#ifndef _VECTOR
#define _VECTOR(N) __vector_ ## N
#endif

#if (__GNUC__ == 4 && __GNUC_MINOR__ >= 1) || (__GNUC__ > 4)
#  define __INTR_ATTRS used, externally_visible
#else /* GCC < 4.1 */
#  define __INTR_ATTRS used
#endif

#ifdef __cplusplus
#  define ISR(vector, ...)            \
    extern "C" void vector (void) __attribute__ ((signal,__INTR_ATTRS)) __VA_ARGS__; \
    void vector (void)
#else
#  define ISR(vector, ...)            \
    void vector (void) __attribute__ ((signal,__INTR_ATTRS)) __VA_ARGS__; \
    void vector (void)
#endif

#ifdef __cplusplus
#  define SIGNAL(vector)					\
    extern "C" void vector(void) __attribute__ ((signal, __INTR_ATTRS));	\
    void vector (void)
#else
#  define SIGNAL(vector)					\
    void vector (void) __attribute__ ((signal, __INTR_ATTRS));		\
    void vector (void)
#endif

#ifdef __cplusplus
#  define EMPTY_INTERRUPT(vector)                \
    extern "C" void vector(void) __attribute__ ((signal,naked,__INTR_ATTRS));    \
    void vector (void) {  __asm__ __volatile__ ("reti" ::); }
#else
#  define EMPTY_INTERRUPT(vector)                \
    void vector (void) __attribute__ ((signal,naked,__INTR_ATTRS));    \
    void vector (void) { __asm__ __volatile__ ("reti" ::); }
#endif

#ifdef __cplusplus
#  if defined(__AVR_MEGA__) && __AVR_MEGA__
#    define ISR_ALIAS(vector, tgt) extern "C" void vector (void) \
	__attribute__((signal, naked, __INTR_ATTRS)); \
	void vector (void) { asm volatile ("jmp " __STRINGIFY(tgt) ::); }
#  else /* !__AVR_MEGA */
#    define ISR_ALIAS(vector, tgt) extern "C" void vector (void) \
	__attribute__((signal, naked, __INTR_ATTRS)); \
	void vector (void) { asm volatile ("rjmp " __STRINGIFY(tgt) ::); }
#  endif  /* __AVR_MEGA__ */
#else	  /* !__cplusplus */
#  if defined(__AVR_MEGA__) && __AVR_MEGA__
#  define ISR_ALIAS(vector, tgt) void vector (void) \
	__attribute__((signal, naked, __INTR_ATTRS)); \
	void vector (void) { asm volatile ("jmp " __STRINGIFY(tgt) ::); }
#  else /* !__AVR_MEGA */
#  define ISR_ALIAS(vector, tgt) void vector (void) \
	__attribute__((signal, naked, __INTR_ATTRS)); \
	void vector (void) { asm volatile ("rjmp " __STRINGIFY(tgt) ::); }
#  endif  /* __AVR_MEGA__ */
#endif	/* __cplusplus */

#  define reti()  __asm__ __volatile__ ("reti" ::)

#  define BADISR_vect __vector_default

#  define ISR_BLOCK
#  define ISR_NOBLOCK    __attribute__((interrupt))
#  define ISR_NAKED      __attribute__((naked))
#  define ISR_ALIASOF(v) __attribute__((alias(__STRINGIFY(v))))

/** Register bits **/

/* SFIOR */
#define PUD 2

/* External Interrupts */
#define ISC00			0
#define ISC01			1
#define ISC10			2
#define ISC11			3
#define ISC2			6

#define INT0			6
#define INT1			7
#define INT2			5

#define INTF0			6
#define INTF1			7
#define INTF2			5

#define INT0_PIN		PD2
#define INT1_PIN		PD3
#define INT2_PIN		PB2

/** Timers **/

/* TCCR0 */
#define CS00			0
#define CS01			1
#define CS02			2
#define WGM01			3
#define COM00			4
#define COM01			5
#define WGM00			6
#define FOC0			7

/* TIMSK */
#define TOIE0			0
#define OCIE0			1
#define TOIE1			2
#define OCIE1B			3
#define OCIE1A			4
#define TICIE1			5
#define TOIE2			6
#define OCIE2			7

/* TIFR */
#define TOV0			0
#define OCF0			1
#define TOV1			2
#define OCF1B			3
#define OCF1A			4
#define ICF1			5
#define TOV2			6
#define OCF2			7

/* TCCR1A */
#define WGM10			0
#define WGM11			1
#define FOC1B			2
#define FOC1A			3
#define COM1B0			4
#define COM1B1			5
#define COM1A0			6
#define COM1A1			7

/* TCCR1B */
#define CS10			0
#define CS11			1
#define CS12			2
#define WGM12			3
#define WGM13			4

#define ICES1			6
#define ICNC1			7

/* TCCR2 */
#define CS20			0
#define CS21			1
#define CS22			2
#define WGM21			3
#define COM20			4
#define COM21			5
#define WGM20			6
#define FOC2			7

/* ASSR */
#define TCR2UB			0
#define OCR2UB			1
#define TCN2UB			2
#define AS2				3

/* TIMER PINS */
#define OC0				PB3
#define OC1A			PD5
#define OC1B			PD4
#define OC2				PD7

#endif /* __CORE_H__*/
