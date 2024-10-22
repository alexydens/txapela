/* Implements io/serial.h */
#include <io/serial.h>

/* The different offsets for the serial ports */
/* Serial data */
#define SERIAL_DATA_PORT(base)          (base)
/* Command first in first out */
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
/* Line command */
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
/* Modem command */
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
/* Line status */
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

/* Commands */
/* Enable DLAB - allow the divisor to come byte by byte */
#define SERIAL_LINE_ENABLE_DLAB         0x80

/* Setup a serial port */
static void setup_com(io_port_t port) {
  /* Configure line */
  port_outb(SERIAL_LINE_COMMAND_PORT(port), 0x03);
  /* Configure buffers */
  port_outb(SERIAL_FIFO_COMMAND_PORT(port), 0xC7);
  /* Configure modem */
  port_outb(SERIAL_MODEM_COMMAND_PORT(port), 0x0B);
}

/* Initialize all serial ports (return false on error) */
bool serial_init(u16 divisor) {
  serial_set_divisor(divisor, SERIAL_PORT_COM1);
  serial_set_divisor(divisor, SERIAL_PORT_COM2);
  serial_set_divisor(divisor, SERIAL_PORT_COM3);
  serial_set_divisor(divisor, SERIAL_PORT_COM4);
  setup_com(SERIAL_PORT_COM1);
  setup_com(SERIAL_PORT_COM2);
  setup_com(SERIAL_PORT_COM3);
  setup_com(SERIAL_PORT_COM4);
  return true;
}
/* Send a character to a serial port */
void serial_putc(io_port_t port, char c) {
  while ((port_inb(SERIAL_LINE_STATUS_PORT(port)) & 0x20) == 0);
  port_outb(SERIAL_DATA_PORT(port), c);
}
/* Send a string to a serial port */
void serial_puts(io_port_t port, const char *str) {
  while (*str) {
    serial_putc(*str, port);
    str++;
  }
}
/* Change the divisor for a serial port */
void serial_set_divisor(u16 divisor, io_port_t port) {
  port_outb(SERIAL_LINE_COMMAND_PORT(port), SERIAL_LINE_ENABLE_DLAB);
  port_outb(SERIAL_DATA_PORT(port), (divisor >> 8) & 0xff);
  port_outb(SERIAL_DATA_PORT(port), (divisor) & 0xff);
}
