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

#if defined(__ARM_EABI__)

#include <cmsis-plus/diag/trace.h>

#include <stdlib.h>
#include <assert.h>

#include "atexit.h"

// ----------------------------------------------------------------------------

/**
 * @brief Request execution of functions at program exit.
 * @param fn
 * @retval 0 The call succeeded in enrolling the function.
 * @retval -1 The call failed, possibly due to memory problems.
 *
 * @details
 *  You can use `atexit()` to enroll functions in a list of functions that
 * will be called when your program terminates normally.  The argument is
 * a pointer to a user-defined function (which must not require arguments and
 * must not return a result).
 *
 * The functions are kept in a LIFO stack; that is, the last function
 * enrolled by `atexit()` will be the first to execute when your program
 * exits.
 *
 * The ANSI standard specifies that
 * there is no built-in limit to the number of functions you can enroll
 * in this list; however, after every group of 32 functions is enrolled,
 * `atexit()` will call `malloc()` to get space for the next part of the
 * list.   The initial list of 32 functions is statically allocated, so
 * you can always count on at least that many slots available.
 *
 * To minimise RAM consumption and to avoid the use of dynamic
 * memory allocations, the above requirement is not met; instead
 * a static array of pointers is used; each application can customise the
 * size of this array to match its needs.
 */
int
atexit (exit_func_t fn)
{
  trace_printf ("%s(%p)\n", __func__, fn);

  return __register_exitproc (__et_atexit, fn, NULL, NULL);
}

// ----------------------------------------------------------------------------

#if !defined(OS_INTEGER_ATEXIT_ARRAY_SIZE)
#define OS_INTEGER_ATEXIT_ARRAY_SIZE (1)
#endif

/**
 * @brief Count of functions registered with atexit().
 */
size_t __atexit_count;

/**
 * @brief Array of pointers to functions registered by atexit().
 * @detail
 * To minimise the RAM consumption, the ANSI requirement to support
 * a minimum of 32 functions and to grow the storage dynamically
 * is not met; instead, this static array is used.
 */
exit_func_t __atexit_functions[OS_INTEGER_ATEXIT_ARRAY_SIZE];

/**
 * @brief Simplified version of atexit() registry.
 * @param type Function type; only __et_atexit supported.
 * @param fn Pointer to function to register.
 * @param arg Function arguments (ignored).
 * @param d Pointer to DSO (ignored).
 * @retval 0 The function was registered.
 * @retval -1 The function was not registered, either the type is
 *  not supported or the static array is full.
 * @details
 * This registry supports only functions passed by atexit(), the
 * more general cxa or dso handles are currently not supported.
 */
int
__register_exitproc (int type, exit_func_t fn,
                     void *arg __attribute__((unused)),
                     void *d __attribute__((unused)))
{
  assert(type == __et_atexit);
  assert(__atexit_count < OS_INTEGER_ATEXIT_ARRAY_SIZE);

#if defined(NDEBUG)
  if ((type != __et_atexit) || (__atexit_count >= OS_INTEGER_ATEXIT_ARRAY_SIZE))
    {
      return -1;
    }
#endif

  // TODO: use scheduler lock to snchronise access to array.
  __atexit_functions[__atexit_count++] = fn;
  return 0;
}

// ----------------------------------------------------------------------------

void
__call_exitprocs (int code __attribute__((unused)),
                  void* d __attribute__((unused)))
{
  // Call registered functions in reverse order.
  for (size_t i = __atexit_count; i > 0;)
    {
      __atexit_functions[--i] ();
    }
}

// ----------------------------------------------------------------------------

#endif /* defined(__ARM_EABI__) */
