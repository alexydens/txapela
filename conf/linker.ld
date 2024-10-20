OUTPUT_FORMAT(elf64-x86-64)

ENTRY(kmain)

PHDRS
{
  requests  PT_LOAD;
  text      PT_LOAD;
  rodata    PT_LOAD;
  data      PT_LOAD;
}

SECTIONS
{
  . = 0xffffffff80000000;

  .requests : {
    KEEP(*(.requests_start))
    KEEP(*(.requests))
    KEEP(*(.requests_end))
  } :requests

  . = ALIGN(CONSTANT(MAXPAGESIZE));
  .text : {
    *(.text .text.*)
  } :text

  . = ALIGN(CONSTANT(MAXPAGESIZE));
  .rodata : {
    *(.rodata .rodata.*)
  } :rodata

  . = ALIGN(CONSTANT(MAXPAGESIZE));
  .data : {
    *(.data .data.*)
  } :data

  .bss : {
    *(.bss .bss.*)
    *(COMMON)
  } :bss

  /DISCARD/ : {
    *(.eh_frame*)
    *(.note .note.*)
  }
}
