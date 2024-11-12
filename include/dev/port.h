/* Include guard */
#if !defined(_DEV_PORT_H)
#define _DEV_PORT_H

/*
 * Ported I/O for x86_64 CPUs (inline asm wrapper).
 */

/* Includes */
#include <core/base.h>

/* Get a byte from a port */
extern u8 port_inb(u16 port);
/* Get a word from a port */
extern u16 port_inw(u16 port);

/* Send a byte to a port */
extern void port_outb(u16 port, u8 val);
/* Send a word to a port */
extern void port_outw(u16 port, u16 val);

#endif /* _DEV_PORT_H */
