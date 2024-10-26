#include <ext/limine.h>
#include <core/base.h>
#include <core/printf.h>
#include <io/tty.h>
#include <sys/segmentation.h>

__section(".requests")
static volatile LIMINE_BASE_REVISION(2);

__section(".requests")
static volatile struct limine_framebuffer_request framebuffer_request = {
  //LIMINE_FRAMEBUFFER_REQUEST, 0, NULL
  .id = LIMINE_FRAMEBUFFER_REQUEST,
  .revision = 0,
  .response = NULL
};

__section(".requests_start")
static volatile LIMINE_REQUESTS_START_MARKER;

__section(".requests_end")
static volatile LIMINE_REQUESTS_END_MARKER;

void kmain(void) {
  /* Check support */
  if (LIMINE_BASE_REVISION_SUPPORTED == false) {
    __asm__ __volatile__ ("hlt");
  }
  if (framebuffer_request.response == NULL
      || framebuffer_request.response->framebuffer_count < 1) {
    __asm__ __volatile__ ("hlt");
  }

  /* Proccess framebuffer response */
  struct limine_framebuffer *fb;
  fb = framebuffer_request.response->framebuffers[0];
  struct framebuffer framebuffer = {
    .ptr = fb->address,
    .width = fb->width,
    .height = fb->height,
    .bpp = fb->bpp
  };

  /* Initialize TTY */
  if (!tty_init(&framebuffer, 1)) {
    __asm__ __volatile__ ("hlt");
  }
  tty_clear();
  tty_set_cursor(0, 0);
  tty_printf("===> Initialized TTY\r\n");
  /* Initialize segmentation */
  if (!segmentation_init()) {
    __asm__ __volatile__ ("hlt");
  }
  tty_printf("===> Initialized segmentation\r\n");
  tty_printf("     - KERNEL CODE SEGMENT: 0x%04x\r\n", SEGMENT_KERNEL_CODE);
  tty_printf("     - KERNEL DATA SEGMENT: 0x%04x\r\n", SEGMENT_KERNEL_DATA);
  tty_printf("     - USER CODE SEGMENT:   0x%04x\r\n", SEGMENT_USER_CODE);
  tty_printf("     - USER DATA SEGMENT:   0x%04x\r\n", SEGMENT_USER_DATA);
  tty_printf("     - TASK STATE SEGMENT:  0x%04x\r\n", SEGMENT_TSS);

  __asm__ __volatile__ ("cli;hlt");
}
