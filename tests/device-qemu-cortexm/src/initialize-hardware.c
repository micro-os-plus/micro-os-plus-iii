/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2016-2025 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can be
 * obtained from https://opensource.org/licenses/mit.
 */

// ----------------------------------------------------------------------------

#include <cmsis-plus/rtos/os-hooks.h>
#include <cmsis_device.h>

#include <stdint.h>
#include <stdlib.h>

// ----------------------------------------------------------------------------

// This is the application hardware initialisation routine,
// redefined to add more inits.
//
// Called early from _start(), right after data & bss init, before
// constructors.
//
// After Reset the Cortex-M processor is in Thread mode,
// priority is Privileged, and the Stack is set to Main.
//
// Warning: The HAL requires the system timer, running at 1000 Hz
// and calling HAL_IncTick().

// Incremented by SysTick_Handler().
extern volatile uint32_t qemu_early_ticks;

void
os_startup_initialize_hardware (void)
{
  // Call the CMSIS system clock routine to store the clock frequency
  // in the SystemCoreClock global RAM location.
  SystemCoreClockUpdate ();

  // Do what the STM32 HAL_Init() does: start SysTick at 1 kHz, before
  // the static constructors and the scheduler. Then wait for two ticks,
  // as HAL_Delay() would, to check that the interrupts are enabled at
  // this point and that the SysTick handler (which also calls
  // os_systick_handler()) is safe before the clocks are constructed.
  // The scheduler reprograms SysTick when it starts.
  SysTick_Config (SystemCoreClock / 1000u);

  uint32_t begin = qemu_early_ticks;
  for (uint32_t i = 0; (qemu_early_ticks - begin) < 2u; ++i)
    {
      if (i >= 100000000u)
        {
          // The SysTick interrupt did not run; the interrupts are
          // probably still masked.
          abort ();
        }
    }
}

// ----------------------------------------------------------------------------
