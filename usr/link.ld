OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)
SECTIONS
{
  . = 0x600000;
  .text   : ALIGN(0x1000) { *(.text) }
  /*.rodata : ALIGN(0x1000) { *(.rodata*) }
  .data   : ALIGN(0x1000) { *(.data) }
  .bss    : ALIGN(0x1000) { *(.bss)  }*/
}
