/* Includes */
#include <ext/limine.h>

/* LIMINE REQUESTS START AND END MARKER */
__attribute__((used, section(".requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;

/* LIMINE REQUESTS */
/* Base version request */
__attribute__((used, section(".requests")))
static volatile LIMINE_BASE_REVISION(2);
/* Framebuffer request */
__attribute__((used, section(".requests")))
static volatile struct limine_framebuffer_request limine_framebuffer_request = {
  .id = LIMINE_FRAMEBUFFER_REQUEST,
  .revision = 1,
  .response = ((void*)0)
};

/* Entry point */
void kmain(void) {
  /* Check base version */
  if (!LIMINE_BASE_REVISION_SUPPORTED) __asm__ __volatile__ ("hlt");
  /* Check framebuffer request */
  if (!limine_framebuffer_request.response) __asm__ __volatile__ ("hlt");

  /* Write some white pixels to the framebuffer */
  struct limine_framebuffer *fb = limine_framebuffer_request.response->framebuffers[0];
  for (int i = 0; i < 100; i++) {
    volatile unsigned int *p = (unsigned int*)(fb->address) + i * fb->width + i;
    *p = 0x00FFFFFF;
  }

  /* Halt */
  while (1);
  while (1) __asm__ __volatile__ ("hlt");
}
