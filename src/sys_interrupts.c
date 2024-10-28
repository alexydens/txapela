/* Implements sys/interrupts.h */
#include <sys/interrupts.h>

/* Consts */
#define INT_NAME_RESERVED "<RESERVED>"
#define INT_NAME_UNSET    "<UNSET>"

/* Globals */
static char interrupt_names[256][32] = {
  "DIVIDE ERROR",
  "DEBUG",
  INT_NAME_RESERVED,
  "BREAKPOINT",
  "OVERFLOW",
  "BOUND RANGE EXCEEDED",
  "INVALID OPCODE",
  "DEVICE NOT AVAILABLE",
  "DOUBLE FAULT",
  INT_NAME_RESERVED,
  "INVALID TSS",
  "SEGMENT NOT PRESENT",
  "STACK SEGMENT FAULT",
  "GENERAL PROTECTION FAULT",
  "PAGE FAULT",
  INT_NAME_RESERVED,
  "FLOATING POINT ERROR",
  "ALIGNMENT CHECK",
  "MACHINE CHECK",
  "SIMD FLOATING POINT NUMERIC ERR"
};
static interrupt_handler interrupt_handlers[256];
static struct buffer interrupt_data[256];
static struct idt_entry {
  u16 offset_low;
  u16 segment;
  u8 ist;
  u8 attributes;
  u16 offset_middle;
  u32 offset_high;
  u32 reserved;
} __packed idt_entries[256] __aligned(0x10);
static struct idtr {
  u16 limit;
  u64 base;
} __packed idtr __aligned(0x10);

/* Set an IDT entry */
static inline void set_idt_entry(
    u8 index,
    u64 base,
    u16 selector,
    u8 ist,
    u8 attributes
) {
  idt_entries[index].offset_low    = (u16)base;
  idt_entries[index].segment       = selector;
  idt_entries[index].ist           = ist;
  idt_entries[index].attributes    = attributes;
  idt_entries[index].offset_middle = (u16)(base >> 16) & 0xFFFF;
  idt_entries[index].offset_high   = (u32)(base >> 32) & 0xFFFFFFFF;
  idt_entries[index].reserved      = 0;
}
#define SET_EXCEPTION_ENTRY(index) set_idt_entry(\
    index,\
    (u64)_isr_##index,\
    SEGMENT_KERNEL_CODE,\
    0, 0x8E)

/* The common exception handler */
struct isr_args {
  u64 rax, rbx, rcx, rdx, rsi, rdi, rbp, r8, r9, r10, r11, r12, r13, r14, r15;
  u64 interrupt_number;
  u64 error_code;
  u64 rip, cs, rflags, rsp, ss;
} __packed;
void isr_handler_common(struct isr_args *args) {
  tty_printf(
      "EXCEPTION %d (%s)\r\n",
      args->interrupt_number,
      interrupt_names[args->interrupt_number]
  );
  if (args->interrupt_number == 14) {
    u64 cr2;
    __asm__ __volatile__ ("mov %%cr2, %0" : "=r"(cr2));
    tty_printf("CR2 = 0x%08x%08x\r\n", (u32)((u64)cr2 >> 32), (u32)((u64)cr2));
    tty_printf(
        "%s process tried to %s a %s page.\r\n",
        args->error_code & 0x4 ? "User" : "Supervisor",
        args->error_code & 0x2 ? "read" : "write",
        args->error_code & 0x1 ? "present" : "non-present"
    );
  }
}

/* Interrupt service routines */
extern void _isr_0(void);
extern void _isr_1(void);
extern void _isr_2(void);
extern void _isr_3(void);
extern void _isr_4(void);
extern void _isr_5(void);
extern void _isr_6(void);
extern void _isr_7(void);
extern void _isr_8(void);
extern void _isr_9(void);
extern void _isr_10(void);
extern void _isr_11(void);
extern void _isr_12(void);
extern void _isr_13(void);
extern void _isr_14(void);
extern void _isr_15(void);
extern void _isr_16(void);
extern void _isr_17(void);
extern void _isr_18(void);
extern void _isr_19(void);
extern void _isr_20(void);
extern void _isr_21(void);
extern void _isr_22(void);
extern void _isr_23(void);
extern void _isr_24(void);
extern void _isr_25(void);
extern void _isr_26(void);
extern void _isr_27(void);
extern void _isr_28(void);
extern void _isr_29(void);
extern void _isr_30(void);
extern void _isr_31(void);

/* Initialize interrupts */
bool interrupts_init(void) {
  /* Set all unset interrupt names to INT_NAME_UNSET */
  for (size_t i = 20; i < ARRLEN(interrupt_names); i++) {
    const char *name = INT_NAME_UNSET;
    memcpy(interrupt_names[i], name, 32);
  }
  /* Set all handlers to NULL */
  memset(interrupt_handlers, 0, sizeof(interrupt_handlers));
  /* Set all data to NULL */
  memset(interrupt_data, 0, sizeof(interrupt_data));

  /* Set all exceptions */
  memset(idt_entries, 0, sizeof(idt_entries));
  SET_EXCEPTION_ENTRY(0);
  SET_EXCEPTION_ENTRY(1);
  SET_EXCEPTION_ENTRY(2);
  SET_EXCEPTION_ENTRY(3);
  SET_EXCEPTION_ENTRY(4);
  SET_EXCEPTION_ENTRY(5);
  SET_EXCEPTION_ENTRY(6);
  SET_EXCEPTION_ENTRY(7);
  SET_EXCEPTION_ENTRY(8);
  SET_EXCEPTION_ENTRY(9);
  SET_EXCEPTION_ENTRY(10);
  SET_EXCEPTION_ENTRY(11);
  SET_EXCEPTION_ENTRY(12);
  SET_EXCEPTION_ENTRY(13);
  SET_EXCEPTION_ENTRY(14);
  SET_EXCEPTION_ENTRY(15);
  SET_EXCEPTION_ENTRY(16);
  SET_EXCEPTION_ENTRY(17);
  SET_EXCEPTION_ENTRY(18);
  SET_EXCEPTION_ENTRY(19);
  SET_EXCEPTION_ENTRY(20);
  SET_EXCEPTION_ENTRY(21);
  SET_EXCEPTION_ENTRY(22);
  SET_EXCEPTION_ENTRY(23);
  SET_EXCEPTION_ENTRY(24);
  SET_EXCEPTION_ENTRY(25);
  SET_EXCEPTION_ENTRY(26);
  SET_EXCEPTION_ENTRY(27);
  SET_EXCEPTION_ENTRY(28);
  SET_EXCEPTION_ENTRY(29);
  SET_EXCEPTION_ENTRY(30);
  SET_EXCEPTION_ENTRY(31);

  /* Set the IDTR */
  idtr.limit = (u16)sizeof(idt_entries) - 1;
  idtr.base = (u64)idt_entries;

  /* Load the IDT */
  __asm__ __volatile__ ("lidt %0" : : "m"(idtr));
  /* Long jump */
  __asm__ __volatile__ (
      "push $0x0010\n\t"
      "leaq 1f(%rip), %rax\n\t"
      "push %rax\n\t"
      "lretq\n\t"
      "1:"
  );
  /* Enable interrupts */
  __asm__ __volatile__ ("sti");

  return true;
}

/*
 * Add an interrupt handler:
 * - The name will be cut short if over 32 characters.
 */
void interrupt_add_handler(
    u32 interrupt_number,
    const char *name,
    interrupt_handler handler,
    struct buffer data
) {
  if (interrupt_number >= 256) {
    return;
  }
  if (handler == NULL) {
    return;
  }
  memset(interrupt_names[interrupt_number], 0, 32);
  memcpy(
      interrupt_names[interrupt_number],
      name,
      strlen(name) < 32 ? strlen(name) : 31
  );
  interrupt_handlers[interrupt_number] = handler;
  interrupt_data[interrupt_number] = data;
}
