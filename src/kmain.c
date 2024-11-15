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
/* Entry point request */
void kmain(void);
__attribute__((used, section(".requests")))
static volatile struct limine_entry_point_request limine_entry_point_request = {
  .id = LIMINE_ENTRY_POINT_REQUEST,
  .revision = 1,
  .response = ((void*)0),
  .entry = kmain
};
/* Framebuffer request */
__attribute__((used, section(".requests")))
static volatile struct limine_framebuffer_request limine_framebuffer_request = {
  .id = LIMINE_FRAMEBUFFER_REQUEST,
  .revision = 1,
  .response = ((void*)0)
};

/* Halt execution */
static inline void halt(void) {
  __asm__ __volatile__ ("xchg %bx, %bx");
  while (1) __asm__ __volatile__ ("hlt");
}

/* Entry point */
void kmain(void) {
  /* Check base version */
  if (!LIMINE_BASE_REVISION_SUPPORTED) halt();
  /* Check entry point request */
  if (!limine_entry_point_request.response) halt();
  /* Check framebuffer request */
  if (!limine_framebuffer_request.response) halt();

  /* Write some white pixels to the framebuffer */
  struct limine_framebuffer *fb = limine_framebuffer_request.response->framebuffers[0];
  for (int i = 0; i < 100; i++) {
    volatile unsigned int *p = (unsigned int*)(fb->address) + i * fb->width + i;
    *p = 0x00FFFFFF;
  }

  /* Halt */
  while (1);
  halt();
}
