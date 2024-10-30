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

struct isr_args {
  u64 rax, rbx, rcx, rdx, rsi, rdi, rbp;
  u64 r8, r9, r10, r11, r12, r13, r14, r15;
  u64 interrupt_number;
  u64 error_code;
  u64 rip, cs, rflags, rsp, ss;
} __packed;
/* An interrupt handler */
typedef void (*interrupt_handler_t)(struct buffer data, struct isr_args *args);
/*
 * Add an interrupt handler:
 * - The name will be cut short if over 32 characters.
 */
extern void interrupt_add_handler(
    u32 interrupt_number,
    const char *name,
    interrupt_handler_t handler,
    struct buffer data
);

#endif /* TX_SYS_INTERRUPTS_H */
