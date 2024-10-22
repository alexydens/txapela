/* Include guard */
#ifndef TX_IO_SERIAL_H
#define TX_IO_SERIAL_H

/* Includes */
#include <core/base.h>
//#include <core/printf.h>
#include <io/port.h>

/* Serial ports (COM1-COM4) */
/* COM1 port */
#define SERIAL_PORT_COM1       0x3F8
/* COM2 port */
#define SERIAL_PORT_COM2       0x2F8
/* COM3 port */
#define SERIAL_PORT_COM3       0x3E8
/* COM4 port */
#define SERIAL_PORT_COM4       0x2E8

/* Baud rate */
/* The default baud rate */
#define SERIAL_DEFAULT_BAUD    115200
/* The default baud divisor */
#define SERIAL_DEFAULT_DIVISOR 2

/* Initialize all serial ports (return false on error) */
extern bool serial_init(u16 divisor);

/* Send a character to a serial port */
extern void serial_putc(io_port_t port, char c);
/* Send a string to a serial port */
extern void serial_puts(io_port_t port, const char *str);
/* Change the divisor for a serial port */
extern void serial_set_divisor(io_port_t port, u16 divisor);

/* Put a character to com1 (internal, just for macro) */
//static inline void _putc_com1(char c) { serial_putc(c, SERIAL_PORT_COM1); }
/* Printf to com1 */
//#define serial_printf(...) _printf(_putc_com1, __VA_ARGS__)

#endif /* TX_IO_SERIAL_H */
