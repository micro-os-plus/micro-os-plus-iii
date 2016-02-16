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

#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

#include <cmsis-plus/diag/trace.h>

// ----------------------------------------------------------------------------

void
__attribute__((noreturn))
__assert_func (const char* file, int line, const char* func,
               const char* failedexpr)
{
  // Not atomic, but otherwise the entire string might get too long,
  // and temporary buffer used by trace_printf() will overflow.
  trace_printf ("assertion \"%s\" failed\n", failedexpr);
  trace_printf ("file: \"%s\"\n", file);
  trace_printf ("line: %d\n", line);
  if (func != NULL)
    {
      trace_printf ("function: %s\n", func);
    }

  abort ();
  /* NOTREACHED */
}

// ----------------------------------------------------------------------------

// This is STM32 specific, but can be used on other platforms too.
// If you need it, add the following to your application header:

//#if defined(USE_FULL_ASSERT)
//#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t*)__FILE__, __LINE__)) void assert_failed(uint8_t* file, uint32_t line);
//#else
//#define assert_param(expr) ((void)0)
//#endif /* USE_FULL_ASSERT */

#if defined(USE_FULL_ASSERT)

void
assert_failed (uint8_t* file, uint32_t line);

// Called from the assert_param() macro, usually defined in the stm32f*_conf.h
void
__attribute__((noreturn))
assert_failed (uint8_t* file, uint32_t line)
{
  trace_printf ("assert_param() failed: file \"%s\", line %d\n", file, line);
  abort ();
  /* NOTREACHED */
}

#endif /* defined(USE_FULL_ASSERT) */

// ----------------------------------------------------------------------------
