/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2023-2025 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can be
 * obtained from https://opensource.org/licenses/mit.
 */

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/cortexm/exception-handlers.h>
#include <cmsis-plus/rtos/os-c-decls.h>

#include <stdint.h>

// Counted like the STM32 HAL `uwTick` (`HAL_IncTick()`), to check that
// the SysTick interrupt runs during the hardware initialisation;
// see `os_startup_initialize_hardware()`.
extern "C" volatile uint32_t qemu_early_ticks;
volatile uint32_t qemu_early_ticks;

void __attribute__ ((section (".after_vectors")))
SysTick_Handler (void)
{
  // DO NOT loop, just return.
  // Useful in case someone (like STM HAL) always enables SysTick.

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wvolatile"
#endif
  ++qemu_early_ticks;
#pragma GCC diagnostic pop

  // Does nothing until the scheduler is started, even if the clocks
  // are not yet constructed.
  os_systick_handler ();
}

// ----------------------------------------------------------------------------
