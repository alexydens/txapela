OUTPUT_FORMAT("elf32-i386")
ENTRY(_start)

PHDRS
{
  text   PT_LOAD;
  rodata PT_LOAD;
  data   PT_LOAD;
  bss    PT_LOAD;
}

SECTIONS
{
  . = 0x00100000;
  .text : {
    *(.multiboot2)
    *(.text)
  } :text
  .rodata : {
    *(.rodata)
  } :rodata
  .data : {
    *(.data)
  } :data
  .bss : {
    *(.bss)
    *(COMMON)
  } :data
}
