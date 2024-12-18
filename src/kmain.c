/* Includes */
#include <ext/limine.h>
#include <core/base.h>
#include <core/panic.h>
#include <io/tty.h>
#include <sys/sse.h>
#include <sys/segmentation.h>
#include <sys/interrupts.h>
#include <mem/pfa.h>

/* The requests start marker */
__section(".requests_start")
static volatile LIMINE_REQUESTS_START_MARKER;

/* The base revision request */
__section(".requests")
static volatile LIMINE_BASE_REVISION(2);
/* The bootloader info request */
__section(".requests")
static volatile struct limine_bootloader_info_request bootloader_info_request={
  .id = LIMINE_BOOTLOADER_INFO_REQUEST,
  .revision = 0,
  .response = NULL
};
/* The firmware type request */
__section(".requests")
static volatile struct limine_firmware_type_request firmware_type_request = {
  .id = LIMINE_FIRMWARE_TYPE_REQUEST,
  .revision = 0,
  .response = NULL
};
/* The framebuffer request */
__section(".requests")
static volatile struct limine_framebuffer_request framebuffer_request = {
  .id = LIMINE_FRAMEBUFFER_REQUEST,
  .revision = 0,
  .response = NULL
};
/* The memory map request */
__section(".requests")
static volatile struct limine_memmap_request memmap_request = {
  .id = LIMINE_MEMMAP_REQUEST,
  .revision = 0,
  .response = NULL
};

/* The requests end marker */
__section(".requests_end")
static volatile LIMINE_REQUESTS_END_MARKER;

/* Entry point for kernel */
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
  struct framebuffer framebuffer;
  for (
      size_t i = 0;
      i < framebuffer_request.response->framebuffer_count;
      i++
  ) {
    fb = framebuffer_request.response->framebuffers[i];
    if (fb->bpp == 32) {
      framebuffer.ptr = fb->address;
      framebuffer.width = fb->width;
      framebuffer.height = fb->height;
      framebuffer.bpp = fb->bpp;
      break;
    }
  }
  if (framebuffer.bpp != 32) {
    __asm__ __volatile__ ("hlt");
  }

  /* Enable SSE */
  if (!sse_init()) {
    __asm__ __volatile__ ("hlt");
  }

  /* Initialize TTY */
  if (!tty_init(&framebuffer)) {
    __asm__ __volatile__ ("hlt");
  }
  tty_clear();
  tty_set_cursor(0, 0);
  tty_printf("===> Initialized TTY\r\n");
  tty_printf("     - FRAMEBUFFER WIDTH:   %u\r\n", framebuffer.width);
  tty_printf("     - FRAMEBUFFER HEIGHT:  %u\r\n", framebuffer.height);
  tty_printf("     - FRAMEBUFFER BPP:     %u\r\n", framebuffer.bpp);
  tty_printf("     - BOOTLOADER:          %s (ver %s)\r\n", 
    bootloader_info_request.response->name,
    bootloader_info_request.response->version
  );
  /* Print boot information */
  switch (firmware_type_request.response->firmware_type) {
    case LIMINE_FIRMWARE_TYPE_X86BIOS:
      tty_printf("     - FIRMWARE TYPE:       x86 BIOS\r\n");
      break;
    case LIMINE_FIRMWARE_TYPE_UEFI32:
      tty_printf("     - FIRMWARE TYPE:       UEFI32\r\n");
      break;
    case LIMINE_FIRMWARE_TYPE_UEFI64:
      tty_printf("     - FIRMWARE TYPE:       UEFI64\r\n");
      break;
    default:
      tty_printf("     - UNRECOGNIZED FIRMWARE TYPE! HALTING...\r\n");
      __asm__ __volatile__ ("hlt");
      break;
  }
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
  /* Initialize interrupts */
  if (!interrupts_init()) {
    __asm__ __volatile__ ("hlt");
  }
  tty_printf("===> Initialized interrupts\r\n");
  /* Initialize page frame allocator */
  if (!pfa_init(
        memmap_request.response->entries,
        memmap_request.response->entry_count
      )) {
    __asm__ __volatile__ ("hlt");
  }
  tty_printf("===> Initialized page frame allocator\r\n");

  while (1);
  __asm__ __volatile__ ("cli;hlt");
}
