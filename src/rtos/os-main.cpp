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
#include <cmsis-plus/rtos/port/os-inlines.h>

// Better be the last, to undef putchar()
#include <cmsis-plus/diag/trace.h>

#if defined(__ARM_EABI__)
#include "cmsis_device.h"
#endif

// ----------------------------------------------------------------------------

namespace
{
  // --------------------------------------------------------------------------

  // Since the native threads have a single argument, and it is better to
  // avoid C++11 tuples and function objects, there is no other simple
  // way than to pack the args in a structure and use it by the
  // trampoline to invoke the os_main().

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
  using main_args_t = struct
    {
      int argc;
      char** argv;
    };
#pragma GCC diagnostic pop

  static main_args_t main_args;

  // --------------------------------------------------------------------------

  [[noreturn]] static void
  _main_trampoline (void)
  {
    int status = os_main (main_args.argc, main_args.argv);
    os::trace::printf ("%s() status = %d\n", __func__, status);
    std::exit (status);
  }

// --------------------------------------------------------------------------

} /* namespace  */

// ----------------------------------------------------------------------------

/**
 * @brief Default implementation of main().
 */
int
#if !defined(__APPLE__)
__attribute__((weak))
#endif
main (int argc, char* argv[])
{
  using namespace os;
  using namespace os::rtos;

  // TODO: make versions configurable.
  trace::printf ("µOS++ v6.1.1 / CMSIS++ RTOS API v0.1.1.\n");
  trace::printf ("Copyright (c) 2016 Liviu Ionescu.\n");

  port::scheduler::greeting ();

  // At this stage the system clock should have already been configured
  // at high speed by __initialise_hardware().
#if defined(__ARM_EABI__)
  trace::printf ("System clock: %u Hz.\n", SystemCoreClock);
#endif

  trace::printf ("Scheduler frequency: %d ticks/sec.\n",
                 rtos::clock_systick::frequency_hz);
  trace::printf ("Default stack size: %d bytes.\n",
                 thread::stack::default_size ());

#if defined(__clang__)
  trace::printf ("Built with clang " __VERSION__);
#else
  trace::printf ("Built with GCC " __VERSION__);
#endif

#if defined(__EXCEPTIONS)
  trace::printf (", with exceptions");
#else
  trace::printf (", no exceptions");
#endif
  trace::puts (".");

#if !defined(OS_INCLUDE_RTOS_PORT_SCHEDULER)
  // Initialise the current thread with a very simple fake
  // thread that at least has a name, so trace messages
  // will not fail with exceptions when printing identity.
  os_thread_t fake_thread;
  fake_thread.name = "none";
  rtos::thread* pth = reinterpret_cast<rtos::thread*> (&fake_thread);

  rtos::scheduler::current_thread_ = pth;
#endif

  scheduler::initialize ();

  // Store the parameters in the static structure, to be used by os_main().
  main_args.argc = argc;
  main_args.argv = argv;

  // Necessarily static, the initial stack will be used for the
  // interrupts, and some implementations (like FreeRTOS) are not
  // able to preserve this stack content.

  static thread::stack::allocation_element_t main_stack[OS_INTEGER_RTOS_MAIN_STACK_SIZE_BYTES
      / sizeof(thread::stack::allocation_element_t)];

  static thread::attributes attr;
  attr.th_stack_address = main_stack;
  attr.th_stack_size_bytes = sizeof(main_stack);

  // Warning: the destructor is registered with atexit()!
#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
  static thread main_thread
    { "main", reinterpret_cast<thread::func_t> (_main_trampoline), nullptr, attr };
#pragma GCC diagnostic pop

  scheduler::start ();

  /* NOTREACHED */
}

// ----------------------------------------------------------------------------
