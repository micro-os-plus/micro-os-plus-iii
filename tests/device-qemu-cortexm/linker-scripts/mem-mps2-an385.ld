/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus/)
 * Copyright (c) 2022 Liviu Ionescu.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/MIT/.
 */

/*
 * Memory Spaces Definitions for the QEMU generic boards.
 *
 *   FLASH.ORIGIN: starting address of flash
 *   FLASH.LENGTH: length of flash
 *   RAM.ORIGIN: starting address of RAM bank 0
 *   RAM.LENGTH: length of RAM bank 0
 *
 * The values below can be addressed in further linker scripts
 * using functions like 'ORIGIN(RAM)' or 'LENGTH(RAM)'.
 */

MEMORY
{
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x008000000 /* 8M */
  RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 0x00800000 /* 8M */
  /* Semihosting SYS_HEAPINFO returns it. */
  HEAP (xrw) : ORIGIN = 0x21000000, LENGTH = 0x01000000 /* 16M */
}

