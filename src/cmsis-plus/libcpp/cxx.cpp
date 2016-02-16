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

#include <cstdlib>
#include <sys/types.h>
#include <cmsis-plus/diag/trace.h>

// ----------------------------------------------------------------------------

// These functions are redefined locally, to avoid references to some
// heavy implementations in the standard C++ library.

namespace __gnu_cxx
{
  void __attribute__((noreturn))
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
  void __attribute__((noreturn))
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

