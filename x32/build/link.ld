ENTRY(Kmain)

SECTIONS {
  . = 0x00100000;     /* load kernel at 1 MiB */

  .text.start : { KEEP(*(.text.start)) }    /* make sure kmain is at the top*/
  .text : { *(.text*) }
  .rodata : { *(.rodata*) }
  .data : { *(.data*) }
  .bss : {
      *(.bss*)
      *(COMMON)
  }
}
