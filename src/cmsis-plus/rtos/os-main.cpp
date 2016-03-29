/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <cstdlib>

#include <cmsis-plus/rtos/os.h>

// Better be the last, to undef putchar()
#include <cmsis-plus/diag/trace.h>

#include "cmsis_device.h"

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    namespace thread
    {
      // ----------------------------------------------------------------------

      using main_func_t = int (*) (int argc, char* argv[]);

      using main_args_t = struct
        {
          main_func_t func;
          int argc;
          char** argv;
        };

      // ----------------------------------------------------------------------

      static void
      _main_trampoline (main_args_t* args)
      {
        std::exit (args->func (args->argc, args->argv));
      }

    // ------------------------------------------------------------------------

    } /* namespace thread */
  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

/**
 * @brief Default implementaion of main().
 */
int __attribute__((weak))
main (int argc, char* argv[])
{
  using namespace os;
  using namespace os::rtos;

  // At this stage the system clock should have already been configured
  // at high speed.
  trace::printf ("System clock: %u Hz\n", SystemCoreClock);

  scheduler::initialize ();

  // Since the native threads have a single argument, and we prefer to
  // avoid C++11 tuples and function objects, there is no other simple
  // way than to pack the 3 args in a structure and pass it via the
  // trampoline to invoke the os_main().

  static thread::main_args_t args;
  args.func = os_main;
  args.argc = argc;
  args.argv = argv;

  // Necessarily static, the initial stack will be used for the
  // interrupts, and some implementations (like FreeRTOS) are not
  // able to preserve this stack content.

  static stack::element_t main_stack[OS_INTEGER_RTOS_MAIN_STACK_SIZE_BYTES
      / sizeof(stack::element_t)];

  static thread::Attributes attr
    { "main" };
  attr.th_stack_address = main_stack;
  attr.th_stack_size_bytes = sizeof(main_stack);

  static Thread main_thread
    { attr, (thread::func_t) thread::_main_trampoline,
        (thread::func_args_t) &args };

  scheduler::start ();

  // Some (most?) embedded schedulers do not return after start(),
  // but on POSIX synthetic platforms they do,
  // so wait for the main thread to terminate.
  main_thread.join ();

  return 0;
}

// ----------------------------------------------------------------------------
