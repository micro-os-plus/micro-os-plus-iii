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

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <stddef.h>

#pragma GCC diagnostic ignored "-Wnested-externs"

// ----------------------------------------------------------------------------

void*
_sbrk (ptrdiff_t incr);

// ----------------------------------------------------------------------------

// The definitions used here should be kept in sync with the
// stack definitions in the linker script.

void*
_sbrk (ptrdiff_t incr)
{
  extern unsigned int _Heap_Begin; // Defined by the linker.
  extern unsigned int _Heap_Limit; // Defined by the linker.

  static char* current_heap_end; // STATIC! Zero after BSS init.
  char* current_block_address;

  if (current_heap_end == 0)
    {
      current_heap_end = (char*)&_Heap_Begin;
    }

  current_block_address = current_heap_end;

  // Need to align heap to word boundary, else will get
  // hard faults on Cortex-M0. So we assume that heap starts on
  // word boundary, hence make sure we always add a multiple of
  // 4 to it.
  incr = (incr + 3) & (~3); // align value to 4
  if (current_heap_end + incr > ((char*)&_Heap_Limit))
    {
      // Some of the libstdc++-v3 tests rely upon detecting
      // out of memory errors, so DO NOT abort here.

      errno = ENOMEM; // Heap has overflowed.
      return (caddr_t) -1;
    }

  current_heap_end += incr;

  return (caddr_t) current_block_address;
}

void*
__attribute__((weak, alias ("_sbrk")))
sbrk (ptrdiff_t incr);

void*
_sbrk_r (struct _reent* impure, ptrdiff_t incr);

void*
_sbrk_r (struct _reent* impure __attribute__((unused)), ptrdiff_t incr)
{
  return _sbrk (incr);
}

// ----------------------------------------------------------------------------

#endif /* defined(__ARM_EABI__) */

// ----------------------------------------------------------------------------
