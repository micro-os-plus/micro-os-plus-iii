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

#include <cmsis-plus/std/thread>
#include <cmsis-plus/rtos/chrono-clocks.h>
#include <cmsis-plus/diag/trace.h>

#include <cstdio>
#include <iostream>
#include <chrono>

using namespace os::cmsis;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// ----------------------------------------------------------------------------

void
task1 (void);

void
task2 (const void* args);

void
task3 (void* args);

void
task4 (int n, const char* str);

// ----------------------------------------------------------------------------

void
task1 (void)
{
  trace::printf ("task1()\n");
}

void
task2 (const void* args)
{
  trace::printf ("task2(%p)\n", args);
}

void
task3 (void* args)
{
  trace::printf ("task3(%p)\n", args);
}

void
task4 (int n, const char* str)
{
  trace::printf ("task4(%d, %s)\n", n, str);
}

// ----------------------------------------------------------------------------

extern "C" void
sleep_for_ticks (uint32_t);

void
sleep_for_ticks (uint32_t)
{

}

// ----------------------------------------------------------------------------

#if !defined(OS_INCLUDE_CMSIS_THREAD_VARIADICS)
#error OS_INCLUDE_CMSIS_THREAD_VARIADICS
#endif

int
main (int argc, char* argv[])
{
  using namespace ::std::chrono;
  using namespace os::cmsis::std;
  using namespace os::cmsis;
  using namespace os;

  char c;
  thread th11
    { task1 };

  th11.native_handle ()->__run_function ();

  new int ();

  static uint8_t stack12[300];
  rtos::thread::Attributes attr12
    { "th12" };
  attr12.set_stack_address ((void*) stack12);
  attr12.set_stack_size_bytes (sizeof(stack12));

  thread th12
    { attr12, task1 };

  th12.native_handle ()->__run_function ();

  static uint8_t stack13[300];
  rtos::thread::Attributes attr13
    { "th13" };
  attr13.set_stack_address ((void*) stack13);
  attr13.set_stack_size_bytes (sizeof(stack13));
  attr13.set_priority (rtos::thread::priority::normal);

  thread th13
    { attr13, task1 };

  th13.native_handle ()->__run_function ();

#if 1

  thread th21
    { task2, &c };

  th21.native_handle ()->__run_function ();

  thread th31
    { task3, &c };

  th31.native_handle ()->__run_function ();

  thread th41
    { task4, 7, "xyz" };

  th41.native_handle ()->__run_function ();

#if 0
  // Fails, nullptr is not matched with attributes
  thread th51
    { nullptr, task4, 7, "xyz"};
#endif

  this_thread::sleep_for (microseconds (3001001));
  this_thread::sleep_for (milliseconds (3001));
  this_thread::sleep_for (seconds (3));
  this_thread::sleep_for (systicks (3003));

  Realtime_clock::startup_time_point = Realtime_clock::now();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

  auto tp = Systick_clock::now();
  trace::printf ("Systick_clock::now() = %d ticks\n", tp);

  auto tp2 = cmsis::std::system_clock::now();
  trace::printf ("system_clock::now() = %ld us\n", tp2);

  auto tp3 = cmsis::std::high_resolution_clock::now();
  trace::printf ("high_resolution_clock::now() = %ld ns\n", tp3);

#pragma GCC diagnostic pop

#if 0
  int n = 70;
  this_thread::sleep_for (milliseconds (n));
#endif

  //th11.native_handle ()->set_priority (rtos::Priority::high);

  th11.join ();
  th12.join ();
  th13.join ();
  th21.join ();
  th31.join ();
  th41.join ();

#endif

  trace::printf ("%s done.\n", argv[0]);
  return 0;
}

#pragma GCC diagnostic pop
