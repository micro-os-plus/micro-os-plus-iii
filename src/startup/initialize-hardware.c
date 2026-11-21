/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2015-2025 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can be
 * obtained from https://opensource.org/licenses/mit.
 */

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wempty-translation-unit"
#endif

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif
#include <cmsis-plus/os-versions.h>

// Part of the bare-metal runtime, compiled for all cross (embedded)
// builds; the native builds use the host C library.
#if defined(OS_IS_CROSS_BUILD)

// ----------------------------------------------------------------------------

#include <cmsis-plus/rtos/os-hooks.h>

#if defined(__ARM_ARCH_PROFILE) && (__ARM_ARCH_PROFILE == 'M')

// Cortex-M: the defaults below use CMSIS.
#include <cmsis_device.h>

// Mainline: v7-M, v7E-M, v8-M Main, v8.1-M (M3, M4, M7, M33, M55, M85).
#if (__ARM_ARCH_ISA_THUMB == 2)
extern unsigned int __vectors_start;
#endif

#endif /* Cortex-M */

// ----------------------------------------------------------------------------

#if defined(OS_HAS_SYSTEMCLOCK_CONFIG)
// Up to 7.1.0, the default os_startup_initialize_hardware_early() called
// SystemClock_Config() when this was defined. STM32CubeMX clock code uses
// the HAL, which requires HAL_Init() first and initialised globals, so it
// cannot run before DATA & BSS init; it was removed.
#error \
    "OS_HAS_SYSTEMCLOCK_CONFIG is no longer used; call HAL_Init(), SystemClock_Config() and SystemCoreClockUpdate() from os_startup_initialize_hardware()."
#endif

/**
 * @details
 * This is the default early hardware initialisation routine.
 *
 * It is called right at the beginning of `_start()`, to switch clocks
 * to higher frequencies and have the rest of the initialisations run faster.
 *
 * The application can redefine it
 * for more complex cases that requires inits before DATA and BSS init.
 *
 * It is mandatory on platforms like Kinetis, which start with the
 * watch dog enabled and require an early sequence to disable it.
 *
 * Also useful on platform with external RAM, that need to be
 * initialised before filling the BSS section.
 *
 * @warning This function runs before the DATA section is copied and
 *  the BSS section is cleared, so it must only access registers; it
 *  must not use initialised global variables, and must not call
 *  vendor HAL functions (like the STM32CubeMX `SystemClock_Config()`),
 *  which use them. Such calls belong in `os_startup_initialize_hardware()`.
 */
void __attribute__ ((weak))
os_startup_initialize_hardware_early (void)
{
#if defined(__ARM_ARCH_PROFILE) && (__ARM_ARCH_PROFILE == 'M')

  // Call the CSMSIS system initialisation routine.
  SystemInit ();

// Mainline: v7-M, v7E-M, v8-M Main, v8.1-M (M3, M4, M7, M33, M55, M85).
#if (__ARM_ARCH_ISA_THUMB == 2)

  // Set VTOR to the actual address, provided by the linker script.
  // Override the manual, possibly wrong, SystemInit() setting.
  SCB->VTOR = (uint32_t)(&__vectors_start);
  // Ensure all subsequence instructions use the new configuration.
  __DSB ();

#endif /* (__ARM_ARCH_ISA_THUMB == 2) */

  // The current version of SystemInit() leaves the value of the clock
  // in a RAM variable (SystemCoreClock), which will be cleared shortly,
  // so it needs to be recomputed after the RAM initialisations
  // are completed.

#if defined(OS_INCLUDE_STARTUP_INIT_FP) || defined(__ARM_FP)

  // Normally FP init is done by SystemInit(). In case this is not done
  // there, it is possible to force its inclusion by defining
  // OS_INCLUDE_STARTUP_INIT_FP.

  // Enable the Cortex-M4 FPU only when -mfloat-abi=hard or -mfloat-abi=softfp.
  // Code taken from Section 7.1, Cortex-M4 TRM (DDI0439C)

  // Set bits 20-23 to enable CP10 and CP11 coprocessor.
  SCB->CPACR |= (0xF << 20);

  // Lazy save.
  FPU->FPCCR |= FPU_FPCCR_ASPEN_Msk | FPU_FPCCR_LSPEN_Msk;

#endif /* defined(OS_INCLUDE_STARTUP_INIT_FP) || defined (__ARM_FP) */

#if defined(OS_DEBUG_SEMIHOSTING_FAULTS)

  SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk;

#endif

#endif /* Cortex-M */

  // Other architectures: by default there is nothing to do this early;
  // the port or the application may redefine it.
}

/**
 * @details
 * This is the default implementation for the second hardware
 * initialisation routine.
 *
 * It is called from `_start()`, right after DATA & BSS init, before
 * the static constructors.
 *
 * The application can
 * redefine it for more complex cases that
 * require custom inits (before constructors), otherwise these inits can
 * be done in main().
 */
void __attribute__ ((weak))
os_startup_initialize_hardware (void)
{
#if defined(__ARM_ARCH_PROFILE) && (__ARM_ARCH_PROFILE == 'M')

  // Call the CSMSIS system clock routine to store the clock frequency
  // in the SystemCoreClock global RAM location.
  SystemCoreClockUpdate ();

#endif /* Cortex-M */

  // Other architectures: by default there is nothing to do; the port or
  // the application may redefine it.
}

// ----------------------------------------------------------------------------

#endif /* defined(OS_IS_CROSS_BUILD) */
