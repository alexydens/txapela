/* Implements dev/uart.h */
#include <dev/uart.h>

/* Includes */
#include <dev/port.h>

/* Port offsets */
#define DATA_PORT           0
#define DIVISOR_LOW_PORT    0
#define DIVISOR_HIGH_PORT   1
#define FIFO_CONTROL_PORT   2
#define LINE_CONTROL_PORT   3
#define MODEM_CONTROL_PORT  4
#define LINE_STATUS_PORT    5
#define MODEM_STATUS_PORT   6
#define SCRATCH_PORT        7

/* Set the divisor of a COM port */
bool uart_com_set_divisor(u16 port, u16 divisor) {
  port_outb(port + LINE_CONTROL_PORT, 0x80);
  port_outb(port + DIVISOR_LOW_PORT, divisor & 0xff);
  port_outb(port + DIVISOR_HIGH_PORT, (divisor >> 8) & 0xff);
  return true;
}
/* Setup a COM port */
bool uart_com_init(u16 port) {
  uart_com_set_divisor(port, 1);
  port_outb(port + LINE_CONTROL_PORT, 0x03);
  port_outb(port + FIFO_CONTROL_PORT, 0xC7);
  port_outb(port + MODEM_CONTROL_PORT, 0x0B);
  return true;
}
/* Print one character to a COM port */
void uart_com_putc(u16 port, char c) {
  while (!(port_inb(port + LINE_STATUS_PORT) & 0x20));
  port_outb(port + DATA_PORT, c);
}
/* Print a NUL-terminated string to a COM port */
void uart_com_puts(u16 port, const char *c) {
  while (*c) uart_com_putc(port, *c++);
}

/* Print one character to COM1 (the defualt) */
void uart_com1_putc(char c) { uart_com_putc(UART_COM1, c); }
