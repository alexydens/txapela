/* Include guard */
#if !defined(_ARCH_RISCV64_MMIO_H)
#define _ARCH_RISCV64_MMIO_H

/* Includes */
#include <core/base.h>

/* Write an 8 bit value to a MMIO register */
extern void mmio_writeb(volatile u8 *addr, u8 val);
/* Write a 16 bit value to a MMIO register */
extern void mmio_writes(volatile u16 *addr, u16 val);
/* Write a 32 bit value to a MMIO register */
extern void mmio_writel(volatile u32 *addr, u32 val);
/* Write a 64 bit value to a MMIO register */
extern void mmio_writeq(volatile u64 *addr, u64 val);

/* Read an 8 bit value from a MMIO register */
extern u8 mmio_readb(volatile u8 *addr);
/* Read a 16 bit value from a MMIO register */
extern u16 mmio_reads(volatile u16 *addr);
/* Read a 32 bit value from a MMIO register */
extern u32 mmio_readl(volatile u32 *addr);
/* Read a 64 bit value from a MMIO register */
extern u64 mmio_readq(volatile u64 *addr);

#endif /* _ARCH_RISCV64_MMIO_H */
