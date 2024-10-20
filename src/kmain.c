#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <ext/limine.h>

__attribute__ ((used, section(".requests")))
static volatile LIMINE_BASE_REVISION(2)

__attribute__ ((used, section(".requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
  LIMINE_FRAMEBUFFER_REQUEST, 0, NULL
};

__attribute__ ((used, section(".requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER

__attribute__ ((used, section(".requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER

void *memcpy(void *dest, const void *src, size_t n) {
  size_t i;

  for (i = 0; i < n; i++) {
    ((char *)dest)[i] = ((const char *)src)[i];
  }

  return dest;
}

void *memset(void *dest, int c, size_t n) {
  size_t i;

  for (i = 0; i < n; i++) {
    ((char *)dest)[i] = c;
  }

  return dest;
}

void *memmove(void *dest, const void *src, size_t n) {
  uint8_t *pdest = (uint8_t *)dest;
  const uint8_t *psrc = (const uint8_t *)src;
  size_t i;

  if (src > dest) {
    for (i = 0; i < n; i++) {
      pdest[i] = psrc[i];
    }
  } else {
    for (i = n; i > 0; i--) {
      pdest[i - 1] = psrc[i - 1];
    }
  }

  return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  const uint8_t *p1 = (const uint8_t *)s1;
  const uint8_t *p2 = (const uint8_t *)s2;
  size_t i;

  for (i = 0; i < n; i++) {
    if (p1[i] != p2[i]) {
      return p1[i] - p2[i];
    }
  }

  return 0;
}

void kmain(void) {
  size_t i;
  struct limine_framebuffer *fb;
  if (LIMINE_BASE_REVISION_SUPPORTED == false) {
    __asm__ __volatile__ ("hlt");
  }

  if (framebuffer_request.response == NULL
      || framebuffer_request.response->framebuffer_count < 1) {
    __asm__ __volatile__ ("hlt");
  }

  fb = framebuffer_request.response->framebuffers[0];

  for (i = 0; i < 100; i++) {
    volatile uint32_t *ptr = fb->address;
    ptr[i * (fb->pitch / sizeof(uint32_t)) + i] = 0xffffffff;
  }

  __asm__ __volatile__ ("cli;hlt");
}
