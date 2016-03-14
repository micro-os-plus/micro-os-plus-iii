/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
 *
 * µOS++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 3.
 *
 * µOS++ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <stddef.h>

#if defined(__ARM_EABI__)

// ----------------------------------------------------------------------------

void*
_sbrk (ptrdiff_t incr);

// ----------------------------------------------------------------------------

// The definitions used here should be kept in sync with the
// stack definitions in the linker script.

void*
_sbrk (ptrdiff_t incr)
{
  extern char _Heap_Begin; // Defined by the linker.
  extern char _Heap_Limit; // Defined by the linker.

  static char* current_heap_end; // STATIC!
  char* current_block_address;

  if (current_heap_end == 0)
    {
      current_heap_end = &_Heap_Begin;
    }

  current_block_address = current_heap_end;

  // Need to align heap to word boundary, else will get
  // hard faults on Cortex-M0. So we assume that heap starts on
  // word boundary, hence make sure we always add a multiple of
  // 4 to it.
  incr = (incr + 3) & (~3); // align value to 4
  if (current_heap_end + incr > &_Heap_Limit)
    {
      // Some of the libstdc++-v3 tests rely upon detecting
      // out of memory errors, so DO NOT abort here.

      errno = ENOMEM; // Heap has overflowed.
      return (caddr_t) -1;
    }

  current_heap_end += incr;

  return (caddr_t) current_block_address;
}

void* __attribute__((weak, alias ("_sbrk")))
sbrk (ptrdiff_t incr);

void*
_sbrk_r (void* impure, ptrdiff_t incr);

void*
_sbrk_r (void* impure __attribute__((unused)), ptrdiff_t incr)
{
  return _sbrk (incr);
}

// ----------------------------------------------------------------------------

#endif /* defined(__ARM_EABI__) */
