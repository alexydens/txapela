/* Implements sys/sse.h */
#include <sys/sse.h>

/* Setup SSE */
bool sse_init(void) {
  u64 cr0, cr4;

  __asm__ __volatile__ (
      "mov %%cr0, %0\n\t"
      "and $~(1 << 2), %0\n\t"
      "or $(1 << 1), %0\n\t"
      "mov %0, %%cr0\n\t"
      : "=r"(cr0)
  );
  __asm__ __volatile__ (
      "mov %%cr4, %0\n\t"
      "or $((1 << 9) | (1 << 10)), %0\n\t"
      "mov %0, %%cr4\n\t"
      : "=r"(cr4)
  );

  u32 mxcsr = 0x1F80;
  __asm__ __volatile__ ("ldmxcsr %0\n\t" : : "m"(mxcsr));

  return true;
}
