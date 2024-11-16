/* Implements arch/riscv64/mmio.h */
#include <arch/riscv64/mmio.h>

#if defined(__arch_riscv64__)
#endif

/* Write an 8 bit value to a MMIO register */
void mmio_writeb(volatile u8 *addr, u8 val) {
  *addr = val;
  __asm__ __volatile__ ("fence io, io" ::: "memory");
}
/* Write a 16 bit value to a MMIO register */
void mmio_writes(volatile u16 *addr, u16 val) {
  *addr = val;
  __asm__ __volatile__ ("fence io, io" ::: "memory");
}
/* Write a 32 bit value to a MMIO register */
void mmio_writel(volatile u32 *addr, u32 val) {
  *addr = val;
  __asm__ __volatile__ ("fence io, io" ::: "memory");
}
/* Write a 64 bit value to a MMIO register */
void mmio_writeq(volatile u64 *addr, u64 val) {
  *addr = val;
  __asm__ __volatile__ ("fence io, io" ::: "memory");
}

/* Read an 8 bit value from a MMIO register */
u8 mmio_readb(volatile u8 *addr) {
  __asm__ __volatile__ ("fence io, io" ::: "memory");
  return *addr;
}
/* Read a 16 bit value from a MMIO register */
u16 mmio_reads(volatile u16 *addr) {
  __asm__ __volatile__ ("fence io, io" ::: "memory");
  return *addr;
}
/* Read a 32 bit value from a MMIO register */
u32 mmio_readl(volatile u32 *addr) {
  __asm__ __volatile__ ("fence io, io" ::: "memory");
  return *addr;
}
/* Read a 64 bit value from a MMIO register */
u64 mmio_readq(volatile u64 *addr) {
  __asm__ __volatile__ ("fence io, io" ::: "memory");
  return *addr;
}
