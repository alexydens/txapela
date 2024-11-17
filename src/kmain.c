/* Includes */
#include <core/base.h>
#include <core/log.h>
#include <ext/limine.h>
#include <arch/x86_64/gdt.h>
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
#if defined(__arch_riscv64__) || defined(__arch_aarch64__)
/* Device tree request */
__attribute__((used, section(".requests")))
volatile struct limine_dtb_request limine_dtb_request = {
  .id = LIMINE_DTB_REQUEST,
  .revision = 1,
  .response = NULL
};
#endif
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
  /* Is there a device tree provided by the bootloader? */
#if defined(__arch_riscv64__) || defined(__arch_aarch64__)
  bool dtb_found = true;
#endif

  /* Check base version */
  if (!LIMINE_BASE_REVISION_SUPPORTED) halt();
  /* Check entry point request */
  if (!limine_entry_point_request.response) halt();
  /* Check higher half direct map request */
  if (!limine_hhdm_request.response) halt();
  /* Check device tree request */
#if defined(__arch_riscv64__) || defined(__arch_aarch64__)
  if (!limine_dtb_request.response) dtb_found = false;
#endif
  /* Check framebuffer request */
  if (!limine_framebuffer_request.response) halt();
  if (!limine_framebuffer_request.response->framebuffer_count)
    fb_support = false;

  /* Initialize UART */
  uart_com_init(UART_COM1);
#if defined(__arch_x86_64__)
  uart_def_com_printf(LOG_INFO "Architecture: x86_64\n");
#elif defined(__arch_riscv64__)
  uart_def_com_printf(LOG_INFO "Architecture: riscv64\n");
#elif defined(__arch_aarch64__)
  uart_def_com_printf(LOG_INFO "Architecture: aarch64\n");
#else
#error "Unrecognized architecture!"
#endif
#if defined(__boot_limine__)
  uart_def_com_printf(LOG_INFO "Bootloader: limine\n");
#else
#error "Unrecognized bootloader!"
#endif
  uart_def_com_printf(LOG_SUCCESS "Initialized UART-16550A.\n");

  /* If on x86_64, initialize the GDT */
#if defined(__arch_x86_64__)
  if (!gdt_init()) {
    uart_def_com_printf(LOG_PANIC "Failed to initialize GDT. Halting...\n");
    halt();
    __builtin_unreachable();
  }
  uart_def_com_printf(LOG_SUCCESS "Initialized Global Descriptor Table.\n");
  uart_def_com_printf(LOG_INFO "GDT Segment Selectors:\n");
  uart_def_com_printf("\t-> Kernel Code Segment: %#02x.\n", GDT_SEG_KERNEL_CODE);
  uart_def_com_printf("\t-> Kernel Data Segment: %#02x.\n", GDT_SEG_KERNEL_DATA);
  uart_def_com_printf("\t-> User Code Segment:   %#02x.\n", GDT_SEG_USER_CODE);
  uart_def_com_printf("\t-> User Data Segment:   %#02x.\n", GDT_SEG_USER_DATA);
  uart_def_com_printf("\t-> Task State Segment:  %#02x.\n", GDT_SEG_TSS);
#endif

  /* Test the framebuffer */
  if (fb_support) {
    uart_def_com_printf(LOG_INFO "Testing framebuffer.\n");
    testfb();
  } else {
    uart_def_com_printf(LOG_INFO "No framebuffer found.\n");
  }
#if defined(__arch_riscv64__) || defined(__arch_aarch64__)
  /* Check for device tree */
  if (dtb_found) {
    uart_def_com_printf(LOG_INFO "Device tree found.\n");
  } else {
    uart_def_com_printf(LOG_WARN "No device tree found.\n");
  }
#endif

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
