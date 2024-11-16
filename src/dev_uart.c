/* Implements dev/uart.h */
#include <dev/uart.h>

/* Consts */
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

/* Globals */
static enum uart_com_port uart_def_com_port = UART_COM1;

/* Specific to x86_64 */
#if defined(__arch_x86_64__)
/* Includes */
#include <arch/x86_64/port.h>

/* Consts */
const u16 UART_COM_PORTS[] = {
  0x3f8,
  0x2f8,
  0x3e8,
  0x2e8,
  0x5f8,
  0x4f8,
  0x5e8,
  0x4e8
};
#endif
/* Specific to riscv64 */
#if defined(__arch_riscv64__)
/* Consts */
volatile u8 * const UART_COM_ADDRS[] = {
  (volatile u8 *)0x10000000,
  (volatile u8 *)0x10000008,
  (volatile u8 *)0x10000010,
  (volatile u8 *)0x10000018,
  (volatile u8 *)0x10000020,
  (volatile u8 *)0x10000028,
  (volatile u8 *)0x10000030,
  (volatile u8 *)0x10000038
};
#if defined(__boot_limine__)
/* Includes */
#include <ext/limine.h>

/* Request */
extern struct limine_hhdm_request limine_hhdm_request;
#else
#error "This feature isn't available without the limine bootloader"
#endif

#endif
/* Specific to aarch64 */
#if defined(__arch_aarch64__)
/* Consts */
volatile u8 * const UART_COM_ADDRS[] = {
  (volatile u8 *)0x09000000,
  (volatile u8 *)0x09000008,
  (volatile u8 *)0x09000010,
  (volatile u8 *)0x09000018,
  (volatile u8 *)0x09000020,
  (volatile u8 *)0x09000028,
  (volatile u8 *)0x09000030,
  (volatile u8 *)0x09000038
};

#if defined(__boot_limine__)
/* Includes */
#include <ext/limine.h>

/* Request */
extern struct limine_hhdm_request limine_hhdm_request;
#else
#error "This feature isn't available without the limine bootloader"
#endif

#endif

/* Setup a COM port */
bool uart_com_init(enum uart_com_port port) {
#if defined(__arch_x86_64__)
  u16 com = UART_COM_PORTS[port];
  u16 divisor = 0x0001;
  port_outb(com + LINE_CONTROL_PORT, 0x80);
  port_outb(com + DIVISOR_LOW_PORT, divisor & 0xff);
  port_outb(com + DIVISOR_HIGH_PORT, (divisor >> 8) & 0xff);
  port_outb(com + LINE_CONTROL_PORT, 0x03);
  port_outb(com + FIFO_CONTROL_PORT, 0xC7);
  port_outb(com + MODEM_CONTROL_PORT, 0x0B);
  return true;
#endif
#if defined(__arch_riscv64__)
  volatile u8 *addr = UART_COM_ADDRS[port];
  addr += limine_hhdm_request.response->offset;
  u16 divisor = 0x0001;
  addr[LINE_CONTROL_PORT] = 0x80;
  addr[DIVISOR_LOW_PORT] = divisor & 0xff;
  addr[DIVISOR_HIGH_PORT] = (divisor >> 8) & 0xff;
  addr[LINE_CONTROL_PORT] = 0x03;
  addr[FIFO_CONTROL_PORT] = 0xC7;
  addr[MODEM_CONTROL_PORT] = 0x0B;
  return true;
#endif
#if defined(__arch_aarch64__)
  volatile u8 *addr = UART_COM_ADDRS[port];
  addr += limine_hhdm_request.response->offset;
  u16 divisor = 0x0001;
  addr[LINE_CONTROL_PORT] = 0x80;
  addr[DIVISOR_LOW_PORT] = divisor & 0xff;
  addr[DIVISOR_HIGH_PORT] = (divisor >> 8) & 0xff;
  addr[LINE_CONTROL_PORT] = 0x03;
  addr[FIFO_CONTROL_PORT] = 0xC7;
  addr[MODEM_CONTROL_PORT] = 0x0B;
  return true;
  return true;
#endif
}

/* Send a character to a COM port */
bool uart_com_putc(enum uart_com_port port, char c) {
#if defined(__arch_x86_64__)
  u16 com = UART_COM_PORTS[port];
  while (!(port_inb(com + LINE_STATUS_PORT) & 0x20));
  port_outb(com + DATA_PORT, c);
  return true;
#endif
#if defined(__arch_riscv64__)
  volatile u8 *addr = UART_COM_ADDRS[port];
  addr += limine_hhdm_request.response->offset;
  while (!(addr[LINE_STATUS_PORT] & 0x20));
  addr[DATA_PORT] = c;
  return true;
#endif
#if defined(__arch_aarch64__)
  volatile u8 *addr = UART_COM_ADDRS[port];
  addr += limine_hhdm_request.response->offset;
  //while (!(addr[LINE_STATUS_PORT] & 0x20));
  addr[DATA_PORT] = c;
  return true;
#endif
}
/* Send a NUL-terminated string to a COM port */
bool uart_com_puts(enum uart_com_port port, const char *s) {
  while (*s) {
    if (!uart_com_putc(port, *s)) return false;
    s++;
  }
  return true;
}

/* Set the default COM port (COM1 on startup) */
bool uart_com_set_def(enum uart_com_port port) {
  uart_def_com_port = port;
  return true;
}
/* Send a character to the default COM port */
bool uart_def_com_putc(char c) {
  return uart_com_putc(uart_def_com_port, c);
}
/* Send a NUL-terminated string to the default COM port */
bool uart_def_com_puts(const char *s) {
  return uart_com_puts(uart_def_com_port, s);
}
