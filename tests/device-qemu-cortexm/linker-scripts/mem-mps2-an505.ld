/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2022-2026 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can be
 * obtained from https://opensource.org/licenses/mit.
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
 *
 * Also see DAI0505B_example_iot_kit_subsystem_for_v2m_mps2.pdf
 *
 * QEMU models the mps2-an505 board (hw/arm/mps2-tz.c) with the
 * Cortex-M33 CPU booting in the Secure state, with the Secure
 * vector table offset register (VTOR) hardwired to 0x10000000 at
 * reset (the Secure alias of the code SRAM physically present at
 * 0x00000000). The code region is therefore placed at 0x10000000,
 * not at 0x00000000.
 *
 * Similarly, QEMU only backs actual RAM for this board at
 * 0x28000000 and 0x28200000 (two 2 MB blocks behind separate
 * MPCs); there is no memory at all mapped at 0x20000000. The
 * Secure alias of that RAM is at 0x38000000, which is the address
 * used below; nothing is backed at the conventional ARMv8-M
 * 0x20000000 Non-secure RAM alias for this specific board.
 */

MEMORY
{
  FLASH (rx) : ORIGIN = 0x10000000, LENGTH = 4M
  RAM (xrw) : ORIGIN = 0x38000000, LENGTH = 4M
}
