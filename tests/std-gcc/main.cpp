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
#include <cmsis-plus/std/chrono>
#include <cmsis-plus/std/mutex>
#include <cmsis-plus/std/condition_variable>
#include <cmsis-plus/diag/trace.h>

#include <cstdio>
#include <iostream>
#include <chrono>
//#include <atomic>

using namespace os;

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

void
my_sleep (int n);

bool
is_ready (void);

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

bool
is_ready (void)
{
  return true;
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

using namespace ::std::chrono;
using namespace os::std::chrono;
using namespace os::std;
using namespace os;

int
main (int argc, char* argv[])
{

    {
      char c;
      thread th11
        { task1 };

      new int ();

      static uint8_t stack12[300];
      rtos::thread::Attributes attr12
        { "th12" };
      attr12.th_stack_address = ((void*) stack12);
      attr12.th_stack_size_bytes = (sizeof(stack12));

      thread th12
        { attr12, task1 };

      static uint8_t stack13[300];
      rtos::thread::Attributes attr13
        { "th13" };
      attr13.th_stack_address = ((void*) stack13);
      attr13.th_stack_size_bytes = (sizeof(stack13));
      attr13.th_priority = (rtos::thread::priority::normal);

      thread th13
        { attr13, task1 };

#if 1

      thread th21
        { task2, &c };

      thread th31
        { task3, &c };

      thread th41
        { task4, 7, "xyz" };

#if 0
      // Fails, nullptr is not matched with attributes
      thread th51
        { nullptr, task4, 7, "xyz"};
#endif

      th11.join ();
      th12.join ();
      th13.join ();
      th21.join ();
      th31.join ();
      th41.join ();
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

  chrono::Realtime_clock::startup_time_point = chrono::Realtime_clock::now ();

#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

  auto tp = chrono::Systick_clock::now ();
  trace::printf ("Systick_clock::now() = %d ticks\n", tp);

  auto tp2 = os::std::chrono::system_clock::now ();
  trace::printf ("system_clock::now() = %ld us\n", tp2);

  auto tp3 = os::std::chrono::high_resolution_clock::now ();
  trace::printf ("high_resolution_clock::now() = %ld ns\n", tp3);

#pragma GCC diagnostic pop

  this_thread::sleep_for (2999_ticks);
  this_thread::sleep_for (3s);
  this_thread::sleep_for (3001ms);
  this_thread::sleep_for (3001001us); // 3002 ticks
  this_thread::sleep_for (3002000001ns); // 3003 ticks

  this_thread::sleep_for (microseconds (1)); // 1 ticks
  this_thread::sleep_for (nanoseconds (1)); // 1 tick

  this_thread::sleep_for<chrono::Systick_clock> (3999_ticks);
  this_thread::sleep_for<chrono::Systick_clock> (4s);
  this_thread::sleep_for<chrono::Systick_clock> (4001ms);

  this_thread::sleep_for<chrono::Realtime_clock> (5s);
  this_thread::sleep_for<chrono::Realtime_clock> (5001ms);

  //this_thread::sleep_for<chrono::system_clock> (3999_ticks);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

  this_thread::sleep_until (chrono::system_clock::now () + 1s);

  this_thread::sleep_until (chrono::Systick_clock::now () + 1us);
  this_thread::sleep_until (chrono::Systick_clock::now () + 1ms);
  this_thread::sleep_until (chrono::Systick_clock::now () + 1s);

  this_thread::sleep_until (chrono::Realtime_clock::now () + 10ms);
  this_thread::sleep_until (chrono::Realtime_clock::now () + 10s);
  this_thread::sleep_until (chrono::Realtime_clock::now () + 1min);

#pragma GCC diagnostic pop

  //::std::chrono::system_clock::time_point tp1 = ::std::chrono::system_clock::now() + 1000ms;
  //this_thread::sleep_until (::std::chrono::system_clock::now());

  //Systick_clock::time_point tp1 = Systick_clock::now() + 1000_ticks;
  //this_thread::sleep_until (tp1);

  my_sleep (70);

#endif

    {
      mutex mx1;
      mx1.lock ();
      mx1.unlock ();
      mx1.try_lock ();
    }

    {
      timed_mutex mx2;
      mx2.try_lock_for (systicks (2999));
      mx2.try_lock_for (seconds (3));
      mx2.try_lock_for (milliseconds (3001)); // 3001 ticks
      mx2.try_lock_for (microseconds (3001001)); // 3002 ticks
      mx2.try_lock_for (nanoseconds (3002000001ul)); // 3003 ticks

      mx2.try_lock_for (microseconds (1)); // 1 tick
      mx2.try_lock_for (nanoseconds (1)); // 1 tick
    }

    {
      condition_variable cv1;
      cv1.notify_one ();
      cv1.notify_all ();

      mutex mxl;
      unique_lock<mutex> lock
        { mxl };

      cv1.wait (lock);

      auto pred = []()
        { return is_ready();};

      cv1.wait (lock, pred);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

      cv1.wait_until (lock, chrono::system_clock::now () + 1s);
      cv1.wait_until (lock, chrono::Systick_clock::now () + 1s);
      cv1.wait_until (lock, chrono::Realtime_clock::now () + 1s);

      cv1.wait_until (lock, chrono::system_clock::now () + 1s, pred);
      cv1.wait_until (lock, chrono::Systick_clock::now () + 1s, pred);
      cv1.wait_until (lock, chrono::Realtime_clock::now () + 1s, pred);

#pragma GCC diagnostic pop

      cv1.wait_for (lock, 2999_ticks);
      cv1.wait_for (lock, 3s);
      cv1.wait_for (lock, 3001ms);
      cv1.wait_for (lock, 3001001us); // 3002 ticks

      cv1.wait_for (lock, 2999_ticks, pred);
      cv1.wait_for (lock, 3s, pred);
      cv1.wait_for (lock, 3001ms, pred);
      cv1.wait_for (lock, 3001001us, pred); // 3002 ticks
    }

    {
      thread::id id = this_thread::get_id ();

      this_thread::yield ();
    }
  trace::printf ("%s done.\n", argv[0]);
  return 0;
}

void
my_sleep (int n)
{
#if 1
  this_thread::sleep_for (systicks (n));
#endif
}

#pragma GCC diagnostic pop
