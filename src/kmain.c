#include <core/base.h>
//#include <ext/font8x8.h>
#include <ext/limine.h>
#include <io/tty.h>

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
  struct limine_framebuffer *fb;

  if (LIMINE_BASE_REVISION_SUPPORTED == false) {
    __asm__ __volatile__ ("hlt");
  }

  if (framebuffer_request.response == NULL
      || framebuffer_request.response->framebuffer_count < 1) {
    __asm__ __volatile__ ("hlt");
  }

  fb = framebuffer_request.response->framebuffers[0];
  struct framebuffer framebuffer = {
    .ptr = fb->address,
    .width = fb->width,
    .height = fb->height,
    .bpp = fb->bpp
  };

  if (!tty_init(&framebuffer, 2)) {
    __asm__ __volatile__ ("hlt");
  }

  tty_clear();
  tty_set_cursor(0, 0);
  tty_puts("Hello world!\r\n...");
  tty_scale(1);
  tty_puts("Hello world!\r\n...");

  __asm__ __volatile__ ("cli;hlt");
}
