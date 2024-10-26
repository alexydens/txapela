/* Include guard */
#ifndef TX_IO_TTY_H
#define TX_IO_TTY_H

/* Includes */
#include <core/base.h>
#include <core/printf.h>
#include <ext/font8x8.h>

/* Initialize basic TTY */
extern bool tty_init(struct framebuffer *fb, size_t scale_up);

/* Change TTY scale */
extern void tty_scale(size_t scale_up);

/* Print a character to the TTY */
extern void tty_putc(char c);
/* Print a string to the TTY */
extern void tty_puts(const char *str);

/* Clear the TTY screen */
extern void tty_clear(void);
/* Set the cursor position of the TTY */
extern void tty_set_cursor(size_t x, size_t y);

/* Put a character to the TTY buffer, but don't re-draw */
extern void _tty_putc_norefresh(char c);
/* Re-render the TTY */
extern void _tty_refresh(void);

/* Print formatted output to the TTY */
#define tty_printf(...) do{ \
  _printf(_tty_putc_norefresh, ##__VA_ARGS__); \
  _tty_refresh();\
  } while (0)\

#endif /* TX_IO_TTY_H */
