OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)
SECTIONS
{
  . = 0x8000;
  .text   : { *(.text) }
  .rodata : { 
    *(.rodata*) 
    *(.rdata)
  }
  .data   : { *(.data) }
  .bss    : { *(.bss) }
}