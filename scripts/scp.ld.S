/*
 * Copyright © 2012-2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#include <platform/memory.h>

OUTPUT_ARCH(or1k)
OUTPUT_FORMAT(elf32-or1k)

ENTRY (start)

MEMORY {
  VECTORS (rwx): ORIGIN = 0x00000000, LENGTH = 0x00002000
  SRAM_A2 (rwx): ORIGIN = SRAM_A2_ARISC_BASE, LENGTH = SRAM_A2_SIZE
}

SECTIONS
{
  . = 0x00000000;
  .vectors . : {
    KEEP(*(.vectors))
  } >VECTORS

  . = SRAM_A2_ARISC_BASE;
  .text . : ALIGN(4) SUBALIGN(4) {
    *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.text*)))
    . = ALIGN(4);
  } >SRAM_A2

  .data . : ALIGN(4) SUBALIGN(4) {
    __device_list = .;
    KEEP(*(.device))
    __device_list_end = .;
    __driver_list = .;
    KEEP(*(.driver))
    __driver_list_end = .;
    *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
    *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.data*)))
    . = ALIGN(4);
  } >SRAM_A2

  . = ALIGN(16);
  .bss . : ALIGN(16) SUBALIGN(4) {
    __bss_start = .;
    *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.bss*)))
    . = ALIGN(4);
    __bss_end = .;
  } >SRAM_A2

  /DISCARD/ : {
    *(.comment*)
    *(.note*)
  }
}
