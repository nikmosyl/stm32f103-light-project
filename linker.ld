ENTRY(Reset_Handler)

MEMORY
{
  FLASH (rx)   : ORIGIN = 0x08000000, LENGTH = 63K
  VER_FLASH (rx) : ORIGIN = 0x0800FC00, LENGTH = 1K
  RAM   (rwx)  : ORIGIN = 0x20000000, LENGTH = 20K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
  .isr_vector :
  {
    KEEP(*(.isr_vector))
  } > FLASH

  .text :
  {
    *(.text*)
    *(.rodata*)
    *(.glue_7)
    *(.glue_7t)
    *(.eh_frame*)
    _etext = .;
  } > FLASH

  _sidata = LOADADDR(.data);

  .data :
  {
    _sdata = .;
    *(.data*)
    _edata = .;
  } > RAM AT > FLASH

  .bss :
  {
    _sbss = .;
    *(.bss*)
    *(COMMON)
    _ebss = .;
  } > RAM

  .fw_version ORIGIN(VER_FLASH) :
  {
    KEEP(*(.fw_version))
  } > VER_FLASH

  end = .;
}
