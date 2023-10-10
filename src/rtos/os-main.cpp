/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/estd/memory_resource>

// ----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

// ----------------------------------------------------------------------------

using namespace os;

// ----------------------------------------------------------------------------

/**
 * @cond ignore
 */

extern "C" void
os_goodbye (void);

namespace
{
  // --------------------------------------------------------------------------

  // Since the native threads have a single argument, and it is better to
  // avoid C++11 tuples and function objects, there is no other simple
  // way than to pack the args in a structure and use it by the
  // trampoline to invoke the os_main().

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wpadded"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpadded"
#endif

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
    trace::puts ("");
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
extern rtos::thread* os_main_thread;

// Intentionally a raw pointer, to prevent destruction.
rtos::thread* os_main_thread;

#if defined(OS_EXCLUDE_DYNAMIC_MEMORY_ALLOCATIONS)

// Necessarily static, on Cortex-M the reset stack will be used
// as MSP for the interrupts, so the current stack must be freed
// and os_main() shall run on its own stack.
using main_thread = rtos::thread_inclusive<OS_INTEGER_RTOS_MAIN_STACK_SIZE_BYTES>;
static std::aligned_storage<sizeof(main_thread), alignof(main_thread)>::type os_main_thread_;

#endif /* defined(OS_EXCLUDE_DYNAMIC_MEMORY_ALLOCATIONS) */

/**
 * @brief Default implementation of `main()`.
 */
int
#if !defined(__APPLE__)
__attribute__((weak))
#endif
main (int argc, char* argv[])
{
  using namespace os::rtos;

  trace::printf ("\nµOS++ IIIe version " OS_STRING_RTOS_IMPL_VERSION "\n");
  trace::printf ("Copyright (c) 2007-" OS_STRING_RTOS_IMPL_YEAR " Liviu Ionescu\n");

  port::scheduler::greeting ();

  trace::printf ("Scheduler frequency: %u ticks/sec\n",
                 rtos::clock_systick::frequency_hz);
  trace::printf ("Default stack size: %u bytes\n",
                 thread::stack::default_size ());
#if defined(OS_HAS_INTERRUPTS_STACK)
  trace::printf ("Interrupts stack size: %u bytes\n",
                 interrupts::stack ()->size ());
#endif /* defined(OS_HAS_INTERRUPTS_STACK) */

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
  trace::puts ("\n");

  scheduler::initialize ();

  // Store the parameters in the static structure, to be used by os_main().
  main_args.argc = argc;
  main_args.argv = argv;

#if defined(OS_EXCLUDE_DYNAMIC_MEMORY_ALLOCATIONS)

  // Running the constructor manually has the additional advantage of
  // not registering any destructor, and for main this is important,
  // since the destructors are executed on its context, and it cannot
  // destruct itself.
  new (&os_main_thread_) main_thread
    {"main", reinterpret_cast<thread::func_t> (_main_trampoline), nullptr};

  os_main_thread = reinterpret_cast<rtos::thread*>(&os_main_thread_);

#else

  thread::attributes attr = thread::initializer;
  attr.th_stack_size_bytes = OS_INTEGER_RTOS_MAIN_STACK_SIZE_BYTES;
#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wcast-function-type-strict"
#endif
  os_main_thread = new thread (
      "main", reinterpret_cast<thread::func_t> (_main_trampoline), nullptr,
      attr);
#pragma GCC diagnostic pop

#endif /* defined(OS_EXCLUDE_DYNAMIC_MEMORY_ALLOCATIONS) */

#if !defined(OS_USE_RTOS_PORT_SCHEDULER)
  os_startup_create_thread_idle ();
#endif /* !defined(OS_USE_RTOS_PORT_SCHEDULER) */

  // Execution will proceed to first registered thread, possibly
  // "idle", which will immediately lower its priority,
  // and at a certain moment will reach os_main().
  scheduler::start ();

  /* NOTREACHED */
}

void
#if !defined(__APPLE__)
__attribute__((weak))
#endif
os_terminate_goodbye (void)
{
#if defined(TRACE)

  trace::printf ("\n");

#if !defined(OS_EXCLUDE_DYNAMIC_MEMORY_ALLOCATIONS)

  // Application memory.
  estd::pmr::get_default_resource ()->trace_print_statistics ();

#if defined(OS_INTEGER_RTOS_DYNAMIC_MEMORY_SIZE_BYTES)
  rtos::memory::get_default_resource ()->trace_print_statistics ();
#endif /* defined(OS_INTEGER_RTOS_DYNAMIC_MEMORY_SIZE_BYTES) */

#endif /* !defined(OS_EXCLUDE_DYNAMIC_MEMORY_ALLOCATIONS) */

  class rtos::thread::stack& st = os_main_thread->stack ();

  trace::printf ("Main thread stack: %u/%u bytes used\n",
                 st.size () - st.available (), st.size ());

#if defined(OS_HAS_INTERRUPTS_STACK)
  trace::printf (
      "Interrupts stack: %u/%u bytes used\n",
      rtos::interrupts::stack ()->size ()
          - rtos::interrupts::stack ()->available (),
      rtos::interrupts::stack ()->size ());
#endif /* defined(OS_HAS_INTERRUPTS_STACK) */

  trace::printf ("\nHasta la Vista!\n");

#endif /* defined(TRACE) */
}

// ----------------------------------------------------------------------------
