/* Implements sys/simd.h */
#include <sys/simd.h>

/* Setup SIMD */
bool simd_init(void) {
  u64 cr0, cr4;
  u32 eax, ebx, ecx, edx;

  /* Check for support */
  __asm__ __volatile__ (
      "cpuid"
      : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
      : "a"(1)
  );
  /* Check for SSE */
  if ((edx & (1 << 25)) == 0) {
    return false;
  }
  /* Check for AVX */
  if ((edx & (1 << 28)) == 0) {
    return false;
  }

  /* cr0 */
  __asm__ __volatile__ (
      "mov %%cr0, %0\n\t"
      "and $~(1 << 2), %0\n\t"
      "or $((1 << 1) | (1 << 2)), %0\n\t"
      "mov %0, %%cr0\n\t"
      : "=r"(cr0)
  );
  /* cr4 */
  __asm__ __volatile__ (
      "mov %%cr4, %0\n\t"
      "or $((1 << 9) | (1 << 10) | (1 << 18)), %0\n\t"
      "mov %0, %%cr4\n\t"
      : "=r"(cr4)
  );

  /* Reset MXCSR */
  u32 mxcsr = 0x1F80;
  __asm__ __volatile__ ("ldmxcsr %0\n\t" : : "m"(mxcsr));

  return true;
}

/* SIMD memcpy */
void *memcpy_simd(void *dst, const void *src, size_t n) {
  size_t i;
  u8 *d = (u8 *)dst;
  const u8 *s = (const u8 *)src;

  for (i = 0; i < n / 32; i++) {
    __asm__ __volatile__ (
      "vmovups (%0), %%ymm0\n\t"
      "vmovups %%ymm0, (%1)\n\t"
      : : "r"(s + i * 32), "r"(d + i * 32)
    );
  }

  for (i = (n / 32) * 32; i < n; i++) {
    d[i] = s[i];
  }

  return dst;
}
