/* Include guard */
#ifndef TX_IO_PORT_H
#define TX_IO_PORT_H

/* Includes */
#include <core/base.h>

/* An I/O port */
typedef u16 io_port_t;

/* Read in a byte from a port */
static inline u8 port_inb(io_port_t port) {
  u8 value;
  __asm__ __volatile__ ("inb %1, %0" : "=a" (value) : "Nd" (port));
  return value;
}
//extern u8 port_inb(io_port_t port);
/* Read in a word from a port */
static inline u16 port_inw(io_port_t port) {
  u16 value;
  __asm__ __volatile__ ("inw %%dx, %%ax" : "=a" (value) : "d" (port));
  return value;
}
//extern u8 port_inw(io_port_t port);

/* Write a byte to a port */
static inline void port_outb(io_port_t port, u8 value) {
  __asm__ __volatile__ ("outb %0, %1" : : "a" (value), "Nd" (port));
}
//extern void port_outb(io_port_t port, u8 val);
/* Write a word to a port */
static inline void port_outw(io_port_t port, u16 value) {
  __asm__ __volatile__ ("outw %0, %1" : : "a" (value), "Nd" (port));
}
//extern void port_outw(io_port_t port, u16 val);

#endif /* TX_IO_PORT_H */
