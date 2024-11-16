/* Include guard */
#if !defined(_DEV_UART_H)
#define _DEV_UART_H

/* Simple UART-16550A driver */

/* Includes */
#include <core/base.h>
#include <core/printf.h>

/* The possible COM ports */
enum uart_com_port {
  UART_COM1 = 0,
  UART_COM2 = 1,
  UART_COM3 = 2,
  UART_COM4 = 3,
  UART_COM5 = 4,
  UART_COM6 = 5,
  UART_COM7 = 6,
  UART_COM8 = 7,
};

/* Setup a COM port */
extern bool uart_com_init(enum uart_com_port port);

/* Send a character to a COM port */
extern bool uart_com_putc(enum uart_com_port port, char c);
/* Send a NUL-terminated string to a COM port */
extern bool uart_com_puts(enum uart_com_port port, const char *s);

/* Set the default COM port (COM1 on startup) */
extern bool uart_com_set_def(enum uart_com_port port);
/* Send a character to the default COM port */
extern bool uart_def_com_putc(char c);
/* Send a NUL-terminated string to the default COM port */
extern bool uart_def_com_puts(const char *s);

/* Internal */
static inline void __uart_def_com_putc_nobool(char c) { uart_def_com_putc(c); }
/* Print formatted output to the default COM port */
#define uart_def_com_printf(...)  _printf(__uart_def_com_putc_nobool, __VA_ARGS__)

#endif /* _DEV_UART_H */
