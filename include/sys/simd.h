/* Include guard */
#ifndef TX_SYS_SIMD_H
#define TX_SYS_SIMD_H

/* Include */
#include <core/base.h>

/* Setup SIMD */
extern bool simd_init(void);
/* Memcpy with SIMD */
extern void *memcpy_simd(void *dst, const void *src, size_t n);

#endif /* TX_SYS_SIMD_H */
