/* Include guard */
#ifndef TX_CORE_PANIC_H
#define TX_CORE_PANIC_H

/* Includes */
#include <core/base.h>
#include <io/tty.h>
/* Dump state */
static inline void dump_state(void) {
  u64 rax, rbx, rcx, rdx, rsi, rdi,
      r8, r9, r10, r11, r12, r13, r14, r15;
  u64 rsp, rbp;
  u64 rip;
  u16 cs, ds, ss, es, fs, gs;
  u64 rflags;
  u64 cr0, cr2, cr3, cr4;
  __asm__ __volatile__ ("mov %%rax, %0" : "=r"(rax));
  __asm__ __volatile__ ("mov %%rbx, %0" : "=r"(rbx));
  __asm__ __volatile__ ("mov %%rcx, %0" : "=r"(rcx));
  __asm__ __volatile__ ("mov %%rdx, %0" : "=r"(rdx));
  __asm__ __volatile__ ("mov %%rsi, %0" : "=r"(rsi));
  __asm__ __volatile__ ("mov %%rdi, %0" : "=r"(rdi));
  __asm__ __volatile__ ("mov %%r8, %0" : "=r"(r8));
  __asm__ __volatile__ ("mov %%r9, %0" : "=r"(r9));
  __asm__ __volatile__ ("mov %%r10, %0" : "=r"(r10));
  __asm__ __volatile__ ("mov %%r11, %0" : "=r"(r11));
  __asm__ __volatile__ ("mov %%r12, %0" : "=r"(r12));
  __asm__ __volatile__ ("mov %%r13, %0" : "=r"(r13));
  __asm__ __volatile__ ("mov %%r14, %0" : "=r"(r14));
  __asm__ __volatile__ ("mov %%r15, %0" : "=r"(r15));
  __asm__ __volatile__ ("mov %%rsp, %0" : "=r"(rsp));
  __asm__ __volatile__ ("mov %%rbp, %0" : "=r"(rbp));
  __asm__ __volatile__ ("leaq (%%rip), %0" : "=r"(rip));
  __asm__ __volatile__ ("pushf; pop %0" : "=r"(rflags));
  __asm__ __volatile__ ("mov %%cr0, %0" : "=r"(cr0));
  __asm__ __volatile__ ("mov %%cr2, %0" : "=r"(cr2));
  __asm__ __volatile__ ("mov %%cr3, %0" : "=r"(cr3));
  __asm__ __volatile__ ("mov %%cr4, %0" : "=r"(cr4));
  __asm__ __volatile__ ("mov %%cs, %0" : "=r"(cs));
  __asm__ __volatile__ ("mov %%ds, %0" : "=r"(ds));
  __asm__ __volatile__ ("mov %%ss, %0" : "=r"(ss));
  __asm__ __volatile__ ("mov %%es, %0" : "=r"(es));
  __asm__ __volatile__ ("mov %%fs, %0" : "=r"(fs));
  __asm__ __volatile__ ("mov %%gs, %0" : "=r"(gs));
  tty_printf("CPU STATE:\r\n");
  tty_printf("- GENERAL PURPOSE REGISTERS:\r\n");
  tty_printf("- rax:    0x%08x%08x\r\n", (u32)(rax >> 32), (u32)rax);
  tty_printf("- rbx:    0x%08x%08x\r\n", (u32)(rbx >> 32), (u32)rbx);
  tty_printf("- rcx:    0x%08x%08x\r\n", (u32)(rcx >> 32), (u32)rcx);
  tty_printf("- rdx:    0x%08x%08x\r\n", (u32)(rdx >> 32), (u32)rdx);
  tty_printf("- rsi:    0x%08x%08x\r\n", (u32)(rsi >> 32), (u32)rsi);
  tty_printf("- rdi:    0x%08x%08x\r\n", (u32)(rdi >> 32), (u32)rdi);
  tty_printf("- r8:     0x%08x%08x\r\n", (u32)(r8 >> 32), (u32)r8);
  tty_printf("- r9:     0x%08x%08x\r\n", (u32)(r9 >> 32), (u32)r9);
  tty_printf("- r10:    0x%08x%08x\r\n", (u32)(r10 >> 32), (u32)r10);
  tty_printf("- r11:    0x%08x%08x\r\n", (u32)(r11 >> 32), (u32)r11);
  tty_printf("- r12:    0x%08x%08x\r\n", (u32)(r12 >> 32), (u32)r12);
  tty_printf("- r13:    0x%08x%08x\r\n", (u32)(r13 >> 32), (u32)r13);
  tty_printf("- r14:    0x%08x%08x\r\n", (u32)(r14 >> 32), (u32)r14);
  tty_printf("- r15:    0x%08x%08x\r\n", (u32)(r15 >> 32), (u32)r15);
  tty_printf("- POINTER REGISTERS:\r\n");
  tty_printf("- rsp:    0x%08x%08x\r\n", (u32)(rsp >> 32), (u32)rsp);
  tty_printf("- rbp:    0x%08x%08x\r\n", (u32)(rbp >> 32), (u32)rbp);
  tty_printf("- rip:    0x%08x%08x\r\n", (u32)(rip >> 32), (u32)rip);
  tty_printf("- CONTROL REGISTERS:\r\n");
  tty_printf("- rflags: 0x%08x%08x\r\n", (u32)(rflags >> 32), (u32)rflags);
  tty_printf("- cr0:    0x%08x%08x\r\n", (u32)(cr0 >> 32), (u32)cr0);
  tty_printf("- cr2:    0x%08x%08x\r\n", (u32)(cr2 >> 32), (u32)cr2);
  tty_printf("- cr3:    0x%08x%08x\r\n", (u32)(cr3 >> 32), (u32)cr3);
  tty_printf("- cr4:    0x%08x%08x\r\n", (u32)(cr4 >> 32), (u32)cr4);
  tty_printf("- SEGMENT REGISTERS:\r\n");
  tty_printf("- cs:     0x%04x\r\n", cs);
  tty_printf("- ds:     0x%04x\r\n", ds);
  tty_printf("- ss:     0x%04x\r\n", ss);
  tty_printf("- es:     0x%04x\r\n", es);
  tty_printf("- fs:     0x%04x\r\n", fs);
  tty_printf("- gs:     0x%04x\r\n", gs);
  tty_printf("\r\n");
}

/* Kernel panic */
#define kpanic(...) do { \
  tty_printf("\r\n!!!KERNEL PANIC!!! (" __VA_ARGS__); \
  tty_printf(")\r\n"); \
  dump_state(); \
  __asm__ __volatile__ ("cli;hlt"); \
} while (0)

#endif /* TX_CORE_PANIC_H */
