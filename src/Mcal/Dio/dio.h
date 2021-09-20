#ifndef __DIO_H__
#define __DIO_H__

#include "../../Infrastructure/types.h"


/*
 * initialize all port pins
 * @param {port} => port to be intialized
 * @param {direction} => INPUT or OUTPUT
 */
void DIO_port_init(uint8_t port, uint8_t direction);

/*
 * initialize specific pin
 * @param {pin} => pin to be intialized
 * @param {direction} => INPUT or OUTPUT
 */
void DIO_pin_init(uint8_t pin, uint8_t direction);

/*
 * enable internal pull up for all port pins
 * @param {port} => port to enable pullup
 */
void DIO_enable_port_pullup(uint8_t port);

/*
 * enable internal pull up for specific pin
 * @param {port} => pin to enable pullup
 */
void DIO_enable_pin_pullup(uint8_t pin);

/*
 * disable internal pull up for all port pins
 * @param {port} => port to disable pullup
 */
void DIO_disable_port_pullup(uint8_t port);

/*
 * disable internal pull up for specific pin
 * @param {port} => pin to disable pullup
 */
void DIO_disable_pin_pullup(uint8_t pin);

/*
 * enable global pull up
 */
void DIO_enable_global_pullup(void);

/*
 * disable global pull up
 */
void DIO_disable_global_pullup(void);

/*
 * write value to all the port
 * @param {port} port to write on
 * @param {data} data to write from 0 to 255
 */
void DIO_write_port(uint8_t port, uint8_t data);

/*
 * write value to a pin
 * @param {port} pin to write on
 * @param {data} data to write, HIGH or LOW
 */
void DIO_write_pin(uint8_t pin, uint8_t data);

/*
 * read value from a port
 * @param {port} port to read from
 * @param {data} pointer to uint8 which holds the data read
 */
void DIO_read_port(uint8_t port, uint8_t* data);

/*
 * read value from a pin
 * @param {pin} pin to read from
 * @param {data} pointer to uint8 which holds either HIGH or LOW
 */
void DIO_read_pin(uint8_t pin, uint8_t* data);

/*
 * toggle all port pins
 * @param {port} port to toggle
 */
void DIO_toggle_port(uint8_t port);

/*
 * toggle pin value
 * @param {pin} pin to toggle
 */
void DIO_toggle_pin(uint8_t pin);

#endif /* __DIO_H__ */
