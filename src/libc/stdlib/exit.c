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

#if defined(__ARM_EABI__)

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/rtos/os-hooks.h>
#include <cmsis-plus/diag/trace.h>
#include <cmsis_device.h>

#include <stdlib.h>
#include <stdbool.h>
#include "atexit.h"

// ----------------------------------------------------------------------------

void
__attribute__ ((noreturn))
os_exit (int code);

extern void
os_goodbye (void);

// ----------------------------------------------------------------------------

void __attribute__((weak,noreturn))
abort (void)
{
  trace_puts ("abort(), exiting...");

  _Exit (1);
  /* NOTREACHED */
}

// ----------------------------------------------------------------------------

/**
 * @param code Exit code.
 *
 * @details
 * `exit()` does several cleanups before ending the application:
 *
 * - calls all application-defined cleanup functions enrolled with `atexit()`;
 * - files and streams are
 * cleaned up: any pending output is delivered to the host system, each
 * open file or stream is closed, and files created by `tmpfile()` are
 * deleted (wishful thinking, not implemented);
 * - call the static destructors (in reverse order of constructors)
 *
 * When all cleanups are done, `_Exit()` is called to perform
 * the actual termination.
 */
void
__attribute__ ((noreturn))
exit (int code)
{
  trace_printf ("%s(%d)\n", __func__, code);

  // Call the cleanup functions enrolled with atexit().
  __call_exitprocs (code, NULL);

  // Run the C++ static destructors.
  os_run_fini_array ();

  // This should normally be the end of it.
  _Exit (code);

  // Reset again, in case _Exit() did not kill it.
  // This normally should not happen, but since it can be
  // overloaded by the application, better safe than sorry.
  os_terminate (code);

  // If it does not want o die, loop.
  while (true)
    {
      __NOP ();
    }
  /* NOTREACHED */
}

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

// On Release, call the hardware reset procedure.
// On Debug, use a breakpoint to notify the debugger.
//
// It can be redefined by the application, if more functionality
// is required. For example, when semihosting is used, this
// function sends the return code to the host.

void __attribute__((weak, noreturn))
_Exit (int code)
{
  trace_printf ("%s()\n", __func__);

  // Print some statistics about memory use.
  os_terminate_goodbye ();

  // Gracefully terminate the trace session.
  trace_flush ();

// By default disable it, since it prevents standalone tests
// to terminate properly.
#if defined(DEBUG) && defined(OS_ENABLE_BKPT_ON_EXIT)

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
  if ((CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) != 0)
    {
      // Break only if the debugger is connected.
      __BKPT(0);
    }
#endif /* defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__) */

#endif /* defined(DEBUG) */

  // Reset hardware or terminate the semihosting session.
  os_terminate (code);

  while (true)
    {
      __NOP ();
    }
  /* NOTREACHED */
}

#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-attributes"

void __attribute__((weak, alias ("_Exit"), noreturn))
_exit (int status);

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

// Semihosting defines this function to terminate the semihosting session.
#if !defined(OS_USE_SEMIHOSTING_SYSCALLS)

/**
 * @details
 * The freestanding version of this function resets the MCU core,
 * using the NVIC features.
 */
void
__attribute__ ((noreturn,weak))
os_terminate(int code __attribute__((unused)))
  {
    NVIC_SystemReset ();
    while(1)
      ;
    /* NOTREACHED */
  }

#endif

// ----------------------------------------------------------------------------

#endif /* defined(__ARM_EABI__) */
