#include <core/base.h>
#include <io/serial.h>
#include <ext/limine.h>

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
  size_t i;
  struct limine_framebuffer *fb;

  serial_init(12);
  serial_puts(SERIAL_PORT_COM1, "Test...\r\n");

  if (LIMINE_BASE_REVISION_SUPPORTED == false) {
    __asm__ __volatile__ ("hlt");
  }

  if (framebuffer_request.response == NULL
      || framebuffer_request.response->framebuffer_count < 1) {
    __asm__ __volatile__ ("hlt");
  }

  fb = framebuffer_request.response->framebuffers[0];

  for (i = 0; i < 100; i++) {
    volatile u32 *ptr = fb->address;
    ptr[i * (fb->pitch / sizeof(uint32_t)) + i] = 0xffffffff;
  }

  __asm__ __volatile__ ("cli;hlt");
}
