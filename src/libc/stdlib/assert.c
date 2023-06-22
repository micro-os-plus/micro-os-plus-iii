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

#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <cmsis-plus/diag/trace.h>
#include <cmsis-plus/rtos/os-c-api.h>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wreserved-identifier"
#else
#pragma GCC diagnostic ignored "-Wredundant-decls"
#endif
void
__assert_func (const char* file, int line, const char* func,
               const char* failedexpr);
#pragma GCC diagnostic pop

#if !defined(TRACE) && !defined(OS_USE_SEMIHOSTING_SYSCALLS)
void
__attribute__((noreturn))
__assert_func (const char* file __attribute__((unused)),
    int line __attribute__((unused)),
    const char* func __attribute__((unused)),
    const char* failedexpr __attribute__((unused)))
  {
    abort ();
  }

#else
void
__attribute__((noreturn))
__assert_func (const char* file, int line, const char* func,
               const char* failedexpr)
{
  os_irq_critical_enter ();

  // Not atomic, but otherwise the entire string might get too long,
  // and temporary buffer used by trace_printf() will overflow.
#if defined(TRACE)

  trace_printf ("assertion \"%s\" failed\n", failedexpr);
  trace_printf ("file: \"%s\"\n", file);
  trace_printf ("line: %d\n", line);
  if (func != NULL)
    {
      trace_printf ("function: %s\n", func);
    }
  if (!os_irq_in_handler_mode())
    {
      if (os_this_thread() != NULL)
        {
          trace_printf ("this_thread: %s @%p\n", os_thread_get_name(os_this_thread()),
                        os_this_thread());
        }
    }

#elif defined(OS_USE_SEMIHOSTING_SYSCALLS)

  printf ("assertion \"%s\" failed\n", failedexpr);
  printf ("file: \"%s\"\n", file);
  printf ("line: %d\n", line);
  if (func != NULL)
    {
      printf ("function: %s\n", func);
    }
  if (!os_irq_in_handler_mode())
    {
      if (os_this_thread() != NULL)
        {
          printf ("this_thread: %s @%p\n", os_thread_get_name(os_this_thread()),
                        os_this_thread());
        }
    }

#endif
  abort ();
  /* NOTREACHED */
}
#endif

// ----------------------------------------------------------------------------

// This is STM32 specific, but can be used on other platforms too.
// If the application needs it, add the following to your application header:

//#if defined(USE_FULL_ASSERT)
//#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t*)__FILE__, __LINE__)) void assert_failed(uint8_t* file, uint32_t line);
//#else
//#define assert_param(expr) ((void)0)
//#endif /* USE_FULL_ASSERT */

// In the new STM32 HAL, the USE_FULL_ASSERT macro is defined in
// stm32??xx_hal_conf.

void
assert_failed (uint8_t* file, uint32_t line);

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

// Called from the assert_param() macro, usually defined in the stm32f*_conf.h
void
__attribute__((noreturn))
assert_failed (uint8_t* file, uint32_t line)
{
#if defined(TRACE)

  trace_printf ("assert_param() failed: file \"%s\", line %d\n", file, line);
  trace_printf ("this_thread: %s\n", os_thread_get_name(os_this_thread()));

#elif defined(OS_USE_SEMIHOSTING_SYSCALLS)

  printf ("assert_param() failed: file \"%s\", line %d\n", file, (int)line);

#endif

  abort ();
  /* NOTREACHED */
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
