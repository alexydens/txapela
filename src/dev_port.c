/* Implements dev/port.h */
#include <dev/port.h>

/* Get a byte from a port */
u8 port_inb(u16 port) {
  u8 val;
  __asm__ __volatile__ ("inb %1, %0" : "=a" (val) : "dN" (port));
  return val;
}
/* Get a word from a port */
u16 port_inw(u16 port) {
  u16 val;
  __asm__ __volatile__ ("inw %1, %0" : "=a" (val) : "dN" (port));
  return val;
}

/* Send a byte to a port */
void port_outb(u16 port, u8 val) {
  __asm__ __volatile__ ("outb %0, %1" : : "a" (val), "dN" (port));
}
/* Send a word to a port */
void port_outw(u16 port, u16 val) {
  __asm__ __volatile__ ("outw %0, %1" : : "a" (val), "dN" (port));
}
