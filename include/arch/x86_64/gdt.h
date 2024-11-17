/* Include guard */
#if !defined(_ARCH_X86_64_GDT_H)
#define _ARCH_X86_64_GDT_H

/* Includes */
#include <core/base.h>

/* Consts */
/* Segment selectors */
/* The kernel code segment */
#define GDT_SEG_KERNEL_CODE   0x10
/* The kernel data segment */
#define GDT_SEG_KERNEL_DATA   0x20
/* The user code segment */
#define GDT_SEG_USER_CODE     0x33
/* The user data segment */
#define GDT_SEG_USER_DATA     0x43
/* The task state segment */
#define GDT_SEG_TSS           0x50

/* Initialize the Global Descriptor Table */
extern bool gdt_init(void);

#endif /* _ARCH_X86_64_GDT_H */
