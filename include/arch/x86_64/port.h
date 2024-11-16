/* Include guard */
#if !defined(_ARCH_X86_64_PORT_H)
#define _ARCH_X86_64_PORT_H

/* Include */
#include <core/base.h>

/* Read a byte from a port */
extern u8 port_inb(u16 port);
/* Read a 16-bit word from a port */
extern u16 port_inw(u16 port);

/* Write a byte to a port */
extern void port_outb(u16 port, u8 value);
/* Write a 16-bit word to a port */
extern void port_outw(u16 port, u16 value);

#endif /* _ARCH_X86_64_PORT_H */
