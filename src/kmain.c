/* Includes */
#include <core/base.h>
#include <core/log.h>
#include <ext/limine.h>
#include <dev/uart.h>

/* LIMINE REQUESTS START AND END MARKER */
__attribute__((used, section(".requests_start")))
volatile LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".requests_end")))
volatile LIMINE_REQUESTS_END_MARKER;

/* LIMINE REQUESTS */
/* Base version request */
__attribute__((used, section(".requests")))
volatile LIMINE_BASE_REVISION(2);
/* Entry point request */
void kmain(void);
__attribute__((used, section(".requests")))
volatile struct limine_entry_point_request limine_entry_point_request = {
  .id = LIMINE_ENTRY_POINT_REQUEST,
  .revision = 1,
  .response = NULL,
  .entry = kmain
};
/* Higher half direct map request */
__attribute__((used, section(".requests")))
volatile struct limine_hhdm_request limine_hhdm_request = {
  .id = LIMINE_HHDM_REQUEST,
  .revision = 1,
  .response = NULL
};
/* Framebuffer request */
__attribute__((used, section(".requests")))
volatile struct limine_framebuffer_request limine_framebuffer_request = {
  .id = LIMINE_FRAMEBUFFER_REQUEST,
  .revision = 1,
  .response = NULL
};

/* Halt execution */
__attribute__((noreturn))
void halt(void) {
  while (1);
}
/* Test framebuffer by drawing line */
void testfb(void) {
  /* Write some white pixels to the framebuffer */
  struct limine_framebuffer *fb = limine_framebuffer_request.response->framebuffers[0];
  for (int i = 0; i < 100; i++) {
    volatile u32 *p = (u32 *)(fb->address) + i * fb->width + i;
    *p = 0x00FFFFFF;
  }
}

/* Entry point */
__attribute__((noreturn))
void kmain(void) {
  /* Variables */
  /* Is there framebuffer support? */
  bool fb_support = true;

  /* Check base version */
  if (!LIMINE_BASE_REVISION_SUPPORTED) halt();
  /* Check entry point request */
  if (!limine_entry_point_request.response) halt();
  /* Check higher half direct map request */
  if (!limine_hhdm_request.response) halt();
  /* Check framebuffer request */
  if (!limine_framebuffer_request.response) halt();
  if (!limine_framebuffer_request.response->framebuffer_count)
    fb_support = false;

  /* Initialize UART */
  uart_com_init(UART_COM1);
  uart_def_com_printf(LOG_SUCCESS "Initialized UART-16550A.\n");

  /* Test the framebuffer */
  if (fb_support) {
    uart_def_com_printf(LOG_INFO "Testing framebuffer.\n");
    testfb();
  }

  /* Test getc */
  while (1) {
    char c = uart_def_com_getc();
    if (c) uart_def_com_printf(LOG_INFO "Char in: %c.\n", c);
  }

  /* Halt */
  while (1);
  halt();
  __builtin_unreachable();
}
