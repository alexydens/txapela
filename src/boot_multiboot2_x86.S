/* Specific to multiboot2 and x86 */
#if defined(__boot_multiboot2__) && defined(__arch_x86__)

.section .multiboot2
.align 4
mb2_header_start:
  /* Header */
  .long 0xe85250d6                        /* Magic number */
  .long 0                                 /* Architecture */
  .long mb2_header_end - mb2_header_start /* Header length */
  /* Checksum */
  .long 0x100000000 - (0xe85250d6 + 0 + (mb2_header_end - mb2_header_start))
  
  /* Tags */
mb2_header_tags:
  /* Framebuffer tag */
mb2_framebuffer_tag:
  .short 0x0005                     /* Type */
  .short 0                          /* Flags */
  .long mb2_framebuffer_tag_end-mb2_framebuffer_tag /* Size */
  .long 1024                        /* Width */
  .long 768                         /* Height */
  .long 32                          /* Bits per pixel */
mb2_framebuffer_tag_end:
  /* End tag */
mb2_end_tag:
  .long 0                           /* Type */
  .long 0                           /* Flags */
  .long mb2_end_tag-mb2_end_tag_end /* Size */
mb2_end_tag_end:

mb2_header_end:

.section .text
.align 4
.globl _start
_start:
  mov $128, %eax
  mov $0xa0000, %edi
1:
  movl $0x00ffffff, %es:(%edi)
  add $(1024 * 4 + 4), %edi
  dec %eax
  cmp $0, %eax
  jne 1b
1:
  hlt
  jmp 1b

#endif
