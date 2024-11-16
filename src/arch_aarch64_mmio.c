/* Implements arch/aarch64/mmio.h */
#include <arch/aarch64/mmio.h>

/* Specific to aarch64 */
#if defined(__arch_aarch64__)

/* Write to an MMIO register */
void mmio_write(volatile u8 *addr, u8 value) {
  *addr = value;
  //__asm__ __volatile__ ("dmb ish" ::: "memory");
}
/* Read from an MMIO register */
u8 mmio_read(volatile u8 *addr) {
  //__asm__ __volatile__ ("dc civac, %0" :: "r"(addr) : "memory");
  //__asm__ __volatile__ ("dmb ish" ::: "memory");
  return *addr;
}

#endif
