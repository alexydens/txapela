/* Implements arch/x86_64/port.h */
#include <arch/x86_64/port.h>

/* Specific to x86_64 */
#if defined(__arch_x86_64__)

/* Read a byte from a port */
u8 port_inb(u16 port) {
  u8 value;
  __asm__ __volatile__ ("inb %1, %0" : "=a" (value) : "dN" (port));
  return value;
}
/* Read a 16-bit word from a port */
u16 port_inw(u16 port) {
  u16 value;
  __asm__ __volatile__ ("inw %1, %0" : "=a" (value) : "dN" (port));
  return value;
}

/* Write a byte to a port */
void port_outb(u16 port, u8 value) {
  __asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (value));
}
/* Write a 16-bit word to a port */
void port_outw(u16 port, u16 value) {
  __asm__ __volatile__ ("outw %1, %0" : : "dN" (port), "a" (value));
}

#endif
