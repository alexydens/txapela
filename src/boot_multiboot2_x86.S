/* Specific to multiboot2 and x86 */
#if defined(__boot_multiboot2__) && defined(__arch_x86__)

/* Includes */
#include <ext/multiboot2.h>

.section .multiboot2
.align 4
mb2_header_start:
  /* Header */
  .long MULTIBOOT2_HEADER_MAGIC           /* Magic number */
  .long GRUB_MULTIBOOT_ARCHITECTURE_I386  /* Architecture */
  .long mb2_header_end - mb2_header_start /* Header length */
  /* Checksum */
  .long 0x100000000 - (0xe85250d6 + 0 + (mb2_header_end - mb2_header_start))
  
  /* Tags */
    /* End tag */
    .long 0   /* Type */
    .long 0   /* Flags */
    .long 8   /* Size */

mb2_header_end:

.section .text
.align 4
.globl _start
_start:
  mov $('A' | 0xea00), %ax
  mov $0xb8000, %edi
  mov %ax, %es:(%edi)
1:
  hlt
  jmp 1b

#endif
