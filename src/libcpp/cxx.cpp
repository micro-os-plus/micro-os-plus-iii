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

#include <cstdlib>
#include <sys/types.h>
#include <cmsis-plus/diag/trace.h>

// ----------------------------------------------------------------------------

// These functions are redefined locally, to avoid references to some
// heavy implementations in the standard C++ library.

namespace __gnu_cxx
{
  [[noreturn]] void
  __verbose_terminate_handler ();

  void
  __verbose_terminate_handler ()
  {
    trace_puts (__func__);
    abort ();
    /* NOTREACHED */
  }
}

// ----------------------------------------------------------------------------

extern "C"
{
  [[noreturn]] void
  __cxa_pure_virtual ();

  void
  __cxa_pure_virtual ()
  {
    // Attempt to use a virtual function before object has been constructed
    trace_puts (__func__);
    abort ();
    /* NOTREACHED */
  }
}

// ----------------------------------------------------------------------------

void *__dso_handle __attribute__ ((weak));

// ----------------------------------------------------------------------------

#endif /* defined(__ARM_EABI__) */
