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

#include <cmsis-plus/diag/trace.h>

#if defined(__ARM_EABI__)
#include "cmsis_device.h"
#endif

// ----------------------------------------------------------------------------

using namespace os;
using namespace os::rtos;

/**
 * @cond ignore
 */

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
    trace::dump_args (main_args.argc, main_args.argv);

    int code = os_main (main_args.argc, main_args.argv);
    trace::printf ("%s() exit = %d\n", __func__, code);
    // Exit will run the atexit() and destructors, then
    // terminate gracefully.
    std::exit (code);
  }

// --------------------------------------------------------------------------

} /* namespace  */

/**
 * @endcond
 */

// ----------------------------------------------------------------------------
// Necessarily static, on Cortex-M the reset stack will be used
// as MSP for the interrupts, so the current stack must be freed
// and os_main() shall run on its own stack.
using main_thread = thread_static<OS_INTEGER_RTOS_MAIN_STACK_SIZE_BYTES>;
static std::aligned_storage<sizeof(main_thread), alignof(main_thread)>::type os_main_thread;

/**
 * @brief Default implementation of main().
 */
int
#if !defined(__APPLE__)
__attribute__((weak))
#endif
main (int argc, char* argv[])
{
  trace::printf ("\nµOS++ v" OS_STRING_RTOS_IMPL_VERSION
  " / CMSIS++ RTOS API v" OS_STRING_RTOS_API_VERSION ".\n");
  trace::printf ("Copyright (c) 2016 Liviu Ionescu.\n");

  port::scheduler::greeting ();

  // At this stage the system clock should have already been configured
  // at high speed by __initialise_hardware().
#if defined(__ARM_EABI__)
  trace::printf ("System clock: %u Hz.\n", SystemCoreClock);
#endif

  trace::printf ("Scheduler frequency: %u ticks/sec.\n",
                 rtos::clock_systick::frequency_hz);
  trace::printf ("Default stack size: %u bytes.\n",
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

  scheduler::initialize ();

  // Store the parameters in the static structure, to be used by os_main().
  main_args.argc = argc;
  main_args.argv = argv;

  // Running the constructor manually has the additional advantage of
  // not registering any destructor, and for main this is important,
  // since the destructors are executed on its context, and it cannot
  // destruct itself.
  new (&os_main_thread) main_thread
    { "main", reinterpret_cast<thread::func_t> (_main_trampoline), nullptr };

  // Execution will proceed to first registered thread, possibly
  // "idle", which will immediately lower its priority,
  // and at a certain moment will reach os_main().
  scheduler::start ();

  /* NOTREACHED */
}

// ----------------------------------------------------------------------------
