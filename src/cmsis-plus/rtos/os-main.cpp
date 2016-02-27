/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
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

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/diag/trace.h>

#include "cmsis_device.h"

#include <cstdlib>

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    namespace thread
    {
      using main_func_t = int (*) (int argc, char* argv[]);

      using main_args_t = struct
        {
          main_func_t func;
          int argc;
          char** argv;
        };

      static void
      main_trampoline (main_args_t* args)
      {
        std::exit (args->func (args->argc, args->argv));
      }

    } /* namespace thread */
  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

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

  static stack::element_t main_stack[256 / sizeof(stack::element_t)];

  static thread::Attributes attr
    { "main" };
  attr.th_stack_address = main_stack;
  attr.th_stack_size_bytes = sizeof(main_stack);

  static Thread main_thread
    { attr, (thread::func_t) thread::main_trampoline,
        (thread::func_args_t) &args };

  scheduler::start ();

  main_thread.join ();

  return 0;
}

// ----------------------------------------------------------------------------
