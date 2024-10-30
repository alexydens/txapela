/* Implements mem/pfa.h */
#include <mem/pfa.h>

/* Globals */
///* Page frames limited to 0x00000000 - 0xffffffff */
static u32 bitmap[0x100000000/(0x1000*sizeof(u32))];

/* Initialize page frame allocator */
bool pfa_init(struct limine_memmap_entry **mmap, size_t mmap_size) {
  /* Set all of bitmap to used */
  memset(bitmap, 0xff, sizeof(bitmap));
  /* Loop over memory map */
  tty_printf("     - MEMORY MAP:\r\n");
  for (size_t i = 0; i < mmap_size; i++) {
    tty_printf(
        "       -> BASE: 0x%08x%08x ",
        (u32)(mmap[i]->base >> 32),
        (u32)mmap[i]->base
    );
    tty_printf(
        "LENGTH: 0x%08x%08x ",
        (u32)(mmap[i]->length >> 32),
        (u32)mmap[i]->length
    );
    switch (mmap[i]->type) {
      case LIMINE_MEMMAP_USABLE:
        {
          /* Print message as usual */
          tty_printf("(USABLE)\r\n");

          /* Mark area as free */
          for (
              u64 j = (u64)mmap[i]->base;
              j < (u64)mmap[i]->base + mmap[i]->length;
              j += 0x1000
          ) {
            pfa_free_page((void *)j);
          }
        } break;
      case LIMINE_MEMMAP_RESERVED:
        tty_printf("(RESERVED)\r\n");
        break;
      case LIMINE_MEMMAP_ACPI_NVS:
        tty_printf("(ACPI NVS)\r\n");
        break;
      case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
        tty_printf("(ACPI RECLAIMABLE)\r\n");
        break;
      case LIMINE_MEMMAP_BAD_MEMORY:
        tty_printf("(BAD MEMORY)\r\n");
        break;
      case LIMINE_MEMMAP_FRAMEBUFFER:
        tty_printf("(FRAMEBUFFER)\r\n");
        break;
      case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
        tty_printf("(BOOTLOADER RECLAIMABLE)\r\n");
        break;
      case LIMINE_MEMMAP_KERNEL_AND_MODULES:
        tty_printf("(KERNEL AND MODULES)\r\n");
        break;
    }
  }
  return true;
}

/* Get a free page from the page frame allocator, and set it as used */
void *pfa_get_page(void) {
  for (size_t i = 0; i < ARRLEN(bitmap); i++) {
    if (bitmap[i] == 0xff) continue;
    for (size_t j = 0; j < sizeof(u32); j++) {
      if (!(bitmap[i] & (1 << j))) {
        bitmap[i] |= 1 << j;
        return (void*)(i*0x1000*sizeof(u32) + j*0x1000);
      }
    }
  }
  kpanic("NO FREE PAGE FRAMES");
  return NULL;
}
/* Mark a page as free */
void pfa_free_page(void *page) {
  size_t page_index = (u64)page / 0x1000;
  size_t i = page_index / sizeof(u32);
  size_t j = page_index % sizeof(u32);
  if (i > ARRLEN(bitmap)) return;
  bitmap[i] &= ~(1 << j);
}
