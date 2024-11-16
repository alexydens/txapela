/* Include guard */
#if !defined(_ARCH_AARCH64_MMIO_H_)
#define _ARCH_AARCH64_MMIO_H_

/* Includes */
#include <core/base.h>

/* Write to an MMIO register */
extern void mmio_write(volatile u8 *addr, u8 value);
/* Read from an MMIO register */
extern u8 mmio_read(volatile u8 *addr);

#endif /* _ARCH_AARCH64_MMIO_H_ */
