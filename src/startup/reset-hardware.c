/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wempty-translation-unit"
#endif

// ----------------------------------------------------------------------------

#if defined(__ARM_EABI__)

// ----------------------------------------------------------------------------

#include <cmsis-plus/diag/trace.h>
#include <cmsis_device.h>

// ----------------------------------------------------------------------------

// cmsis_device.h
// extern void __attribute__((noreturn))
// NVIC_SystemReset (void);

// ----------------------------------------------------------------------------

// Forward declarations

void
__reset_hardware (void);

// ----------------------------------------------------------------------------

// This is the default hardware reset routine; it can be
// redefined in the application for more complex applications.
//
// Called from _exit().

void __attribute__((weak,noreturn))
__reset_hardware (void)
{
#if defined(DEBUG) || defined(OS_DISABLE_RESET_HARDWARE)
  trace_printf("__reset_hardware()");
  while (1)
    __WFI();
#else
  NVIC_SystemReset ();
  __builtin_unreachable ();
#endif
}

// ----------------------------------------------------------------------------

#endif /* defined(__ARM_EABI__) */
