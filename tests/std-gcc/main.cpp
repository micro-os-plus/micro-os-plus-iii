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

  thread th12
    { new rtos::Thread
      { "th12", 777, rtos::Priority::normal, task1 } };

  th12.native_handle ()->__run_function ();

  thread th13
    { new rtos::Thread
      { (char*) "th13", 777, rtos::Priority::normal, task1 } };

  th13.native_handle ()->__run_function ();

#if 1

  thread th21
    { task2, &c };

  th21.native_handle ()->__run_function ();

  thread th31
    { task3, &c };

  th31.native_handle ()->__run_function ();

  this_thread::sleep_for (microseconds (3001001));
  this_thread::sleep_for (milliseconds (3001));
  this_thread::sleep_for (seconds (3));

#if 0
  int n = 70;
  this_thread::sleep_for (milliseconds (n));
#endif

  th11.native_handle ()->set_priority (rtos::Priority::high);

  th11.join ();
  th12.join ();
  th13.join ();
  th21.join ();
  th31.join ();
#endif

  trace::printf ("%s done.\n", argv[0]);
  return 0;
}

#pragma GCC diagnostic pop
