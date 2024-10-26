/* Implements sys/segmentation.h */
#include <sys/segmentation.h>

/* Globals */
static struct gdt_entry {
  u16 limit_low;
  u16 base_low;
  u8 base_middle;
  u8 access;
  u8 limit_high: 4;
  u8 flags: 4;
  u8 base_high;
  u32 base_upper;
  u32 reserved;
} __packed gdt_entries[6] __aligned(0x10);
static struct gdtr {
  u16 limit;
  u64 base;
} __packed gdtr __aligned(0x10);
static struct tss {
  u32 reserved0;          /* Reserved (0x00) */
  u64 rsp0;               /* Stack pointer for Ring 0 (0x04) */
  u64 rsp1;               /* Stack pointer for Ring 1 (0x0C) */
  u64 rsp2;               /* Stack pointer for Ring 2 (0x14) */
  u64 reserved1;          /* Reserved (0x1C) */
  u64 ist1;               /* Interrupt Stack Table 1 (0x20) */
  u64 ist2;               /* Interrupt Stack Table 2 (0x28) */
  u64 ist3;               /* Interrupt Stack Table 3 (0x30) */
  u64 ist4;               /* Interrupt Stack Table 4 (0x38) */
  u64 ist5;               /* Interrupt Stack Table 5 (0x40) */
  u64 ist6;               /* Interrupt Stack Table 6 (0x48) */
  u64 ist7;               /* Interrupt Stack Table 7 (0x50) */
  u64 reserved2;          /* Reserved (0x58) */
  u16 reserved3;          /* Reserved (0x60) */
  u16 io_map_base;        /* I/O map base address (0x62) */
} __packed tss __aligned(0x10);

/* Set a GDT entry */
static inline void set_gdt_entry(
    u8 index,
    u64 base,
    u32 limit,
    u8 access,
    u8 flags
) {
  gdt_entries[index].base_low    = (u16)base;
  gdt_entries[index].base_middle = (u8)(base >> 16) & 0xFF;
  gdt_entries[index].base_high   = (u8)(base >> 24) & 0xFF;
  gdt_entries[index].base_upper  = (u32)(base >> 32) & 0xFFFFFFFF;
  gdt_entries[index].limit_low   = (u16)limit;
  gdt_entries[index].limit_high  = (u8)(limit >> 16) & 0x0F;
  gdt_entries[index].access      = access;
  gdt_entries[index].flags       = flags;
}
/* Set the TSS entry */
static inline void set_tss_entry(void) {
  /* Set the GDT entry for it */
  gdt_entries[5].base_low    = (u16)((u64)&tss >> 0) & 0xFFFF;
  gdt_entries[5].base_middle = (u8)((u64)&tss >> 16) & 0xFF;
  gdt_entries[5].base_high   = (u8)((u64)&tss >> 24) & 0xFF;
  gdt_entries[5].base_upper  = (u32)((u64)&tss >> 32) & 0xFFFFFFFF;
  gdt_entries[5].limit_low   = sizeof(tss);
  gdt_entries[5].limit_high  = 0x0F;
  gdt_entries[5].access      = 0x89; /* Present, 64-bit TSS */
  gdt_entries[5].flags       = 0x00;

  /* Clear TSS */
  memset(&tss, 0, sizeof(tss));
}

/* Initialize segmentation */
bool segmentation_init(void) {
  /* Clear the GDT */
  memset(gdt_entries, 0, sizeof(gdt_entries));
  /* Set the GDT */
  /* Null entry */
  set_gdt_entry(0, 0, 0, 0, 0);
  /* 
   * Kernel code segment:
   * - present, executable/data, executable, readable.
   */
  set_gdt_entry(1, 0, 0xFFFFFFFF, 0x9B, 0x02);
  /* 
   * Kernel data segment:
   * - present, executable/data, writable.
   */
  set_gdt_entry(2, 0, 0xFFFFFFFF, 0x93, 0x02);
  /* 
   * Kernel code segment:
   * - present, ring 3, executable/data, executable, readable.
   */
  set_gdt_entry(3, 0, 0xFFFFFFFF, 0xFB, 0x02);
  /* 
   * Kernel data segment:
   * - present, ring 3, executable/data, writable.
   */
  set_gdt_entry(4, 0, 0xFFFFFFFF, 0xF3, 0x02);
  /* Task state segment */
  set_tss_entry();

  /* Set the GDTR */
  gdtr.limit = sizeof(gdt_entries) - 1;
  gdtr.base  = (u64)gdt_entries;

  /* Set the GDTR */
  __asm__ __volatile__ ("lgdt %0" : : "m"(gdtr));

  /* Reload the segment registers */
  __asm__ __volatile__ ("mov $0x0020, %ax");
  __asm__ __volatile__ ("mov %ax, %ds");
  __asm__ __volatile__ ("mov %ax, %es");
  __asm__ __volatile__ ("mov %ax, %fs");
  __asm__ __volatile__ ("mov %ax, %gs");
  __asm__ __volatile__ ("mov %ax, %ss");

  /* Long jump to set cs */
  __asm__ __volatile__ (
      "push $0x0010\n\t"
      "leaq 1f(%rip), %rax\n\t"
      "push %rax\n\t"
      "lretq\n\t"
      "1:"
  );

  /* Set the TSS */
  __asm__ __volatile__ ("ltr %0" : : "r"(SEGMENT_TSS));

  return true;
}
