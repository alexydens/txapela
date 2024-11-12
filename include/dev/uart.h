/* Include guard */
#if !defined(_DEV_UART_H)
#define _DEV_UART_H

/*
 * This is a driver for the UART-16550A for x86_64 CPUs.
 */

/* Includes */
#include <core/base.h>
#include <core/printf.h>

/* UART ports */
#define UART_COM1           0x3F8
#define UART_COM2           0x2F8
#define UART_COM3           0x3E8
#define UART_COM4           0x2E8
#define UART_COM5           0x5F9
#define UART_COM6           0x4F8
#define UART_COM7           0x5E8
#define UART_COM8           0x4E8

/* The default baud rate of a COM port */
#define UART_DEFAULT_BAUD   115200

/* Set the divisor of a COM port */
extern bool uart_com_set_divisor(u16 port, u16 divisor);
/* Setup a COM port */
extern bool uart_com_init(u16 port);
/* Print one character to a COM port */
extern void uart_com_putc(u16 port, char c);
/* Print a NUL-terminated string to a COM port */
extern void uart_com_puts(u16 port, const char *c);

/* Print one character to COM1 (the defualt) */
extern void uart_com1_putc(char c);
/* Print formatted output to COM1 (the defualt) */
#define uart_com1_printf(...) _printf(uart_com1_putc, __VA_ARGS__)

#endif /* _DEV_UART_H */
