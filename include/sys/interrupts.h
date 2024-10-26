/* Include guard */
#ifndef TX_SYS_INTERRUPTS_H
#define TX_SYS_INTERRUPTS_H

/* Includes */
#include <core/base.h>
#include <sys/segmentation.h>
#include <io/tty.h>
#include <io/port.h>

/* Initialize interrupts */
extern bool interrupts_init(void);

/* An interrupt handler */
typedef void (*interrupt_handler)(struct buffer data);
/*
 * Add an interrupt handler:
 * - The name will be cut short if over 32 characters.
 */
extern void interrupt_add_handler(
    u32 interrupt_number,
    const char *name,
    interrupt_handler handler,
    struct buffer data
);

#endif /* TX_SYS_INTERRUPTS_H */
