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

#include <cstdio>
#include <cstdint>
#include <iostream>
#include <chrono>

#include <cmsis-plus/iso/chrono>
#include <cmsis-plus/iso/condition_variable>
#include <cmsis-plus/iso/mutex>
#include <cmsis-plus/iso/thread>
//#include <atomic>

#include <test-iso-api.h>

// ----------------------------------------------------------------------------

static const char* test_name = "Test ISO C++ API";

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
  printf ("%s()\n", __func__);
}

void
task2 (const void* args)
{
  printf ("%s(%p)\n", __func__, args);
}

void
task3 (void* args)
{
  printf ("%s(%p)\n", __func__, args);
}

void
task4 (int n, const char* str)
{
  printf ("%s(%d,%s)\n", __func__, n, str);
}

bool
is_ready (void)
{
  return true;
}

// ----------------------------------------------------------------------------

#if 0
extern "C" void
sleep_for_ticks (uint32_t);

void
sleep_for_ticks (uint32_t)
  {

  }
#endif

// ----------------------------------------------------------------------------

using namespace std::chrono;
using namespace os::estd::chrono;
using namespace os::estd;
using namespace os;

int
test_iso_api (bool extra)
{
  char c;

  // ==========================================================================
  printf ("\n%s - Threads.\n", test_name);
    {
        {
          thread th11
            { task1 };

          th11.join ();
        }

        {
          thread th21
            { task2, &c };

          thread th31
            { task3, &c };

          thread th41
            { task4, 7, "xyz" };

          th21.join ();
          th31.join ();
          th41.join ();
        }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

        {
          thread::id id = this_thread::get_id ();

          this_thread::yield ();
        }

#pragma GCC diagnostic pop

    }

  // ==========================================================================
  printf ("\n%s - Mutexes.\n", test_name);

    {
        {
          mutex mx1;

          mx1.lock ();
          mx1.unlock ();

          mx1.try_lock ();
          mx1.unlock ();
        }

        {
          timed_mutex mx2;

          mx2.lock ();
          mx2.unlock ();

          mx2.try_lock ();
          mx2.unlock ();

          mx2.try_lock_for (systicks (2999));
          mx2.unlock ();
          mx2.try_lock_for (seconds (3));
          mx2.unlock ();
          mx2.try_lock_for (milliseconds (3001)); // 3001 ticks
          mx2.unlock ();
          mx2.try_lock_for (microseconds (3001001)); // 3002 ticks
          mx2.unlock ();
          mx2.try_lock_for (nanoseconds (3002000001ul)); // 3003 ticks
          mx2.unlock ();

          mx2.try_lock_for (microseconds (1)); // 1 tick
          mx2.unlock ();
          mx2.try_lock_for (nanoseconds (1)); // 1 tick
          mx2.unlock ();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

          mx2.try_lock_until (chrono::system_clock::now () + 1000us);
          mx2.unlock ();
          mx2.try_lock_until (chrono::system_clock::now () + 1ms);
          mx2.unlock ();

          mx2.try_lock_until (chrono::systick_clock::now () + 1us);
          mx2.unlock ();
          mx2.try_lock_until (chrono::systick_clock::now () + 1ms);
          mx2.unlock ();

          mx2.try_lock_until (chrono::realtime_clock::now () + 10ms);
          mx2.unlock ();
          mx2.try_lock_until (chrono::realtime_clock::now () + 100ms);
          mx2.unlock ();

#pragma GCC diagnostic pop

        }
    }

  // ==========================================================================

  printf ("\n%s - Condition variables.\n", test_name);
    {
      condition_variable cv1;
      cv1.notify_one ();
      cv1.notify_all ();

      if (extra)
        {

          mutex mxl;
          unique_lock<mutex> lock
            { mxl };

          cv1.wait (lock);

          auto pred = []()
            { return is_ready();};

          cv1.wait (lock, pred);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

          cv1.wait_until (lock, chrono::system_clock::now () + 10ms);
          cv1.wait_until (lock, chrono::systick_clock::now () + 10ms);
          cv1.wait_until (lock, chrono::realtime_clock::now () + 1s);

          cv1.wait_until (lock, chrono::system_clock::now () + 10ms, pred);
          cv1.wait_until (lock, chrono::systick_clock::now () + 10ms, pred);
          cv1.wait_until (lock, chrono::realtime_clock::now () + 1s, pred);

          cv1.wait_for (lock, 10_ticks);
          cv1.wait_for (lock, 10ms);
          cv1.wait_for (lock, 10000us);

          cv1.wait_for (lock, 12_ticks, pred);
          cv1.wait_for (lock, 12ms, pred);
          cv1.wait_for (lock, 12000us, pred);

          if (extra)
            {
              cv1.wait_for (lock, 2999_ticks);
              cv1.wait_for (lock, 3s);
              cv1.wait_for (lock, 3001ms);
              cv1.wait_for (lock, 3001001us); // 3002 ticks

              cv1.wait_for (lock, 2999_ticks, pred);
              cv1.wait_for (lock, 3s, pred);
              cv1.wait_for (lock, 3001ms, pred);
              cv1.wait_for (lock, 3001001us, pred); // 3002 ticks
            }

#pragma GCC diagnostic pop

        }
    }

  // ==========================================================================

  printf ("\n%s - Chrono.\n", test_name);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

  chrono::realtime_clock::startup_time_point = chrono::realtime_clock::now ();

#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

  this_thread::sleep_for (5_ticks);

  auto tp = chrono::systick_clock::now ();
  printf ("Systick_clock::now() = %u ticks\n",
          static_cast<uint32_t> (tp.time_since_epoch ().count ()));

  auto tp2 = os::estd::chrono::system_clock::now ();
  printf ("system_clock::now() = %u us\n",
          static_cast<uint32_t> (tp2.time_since_epoch ().count ()));

  auto tp3 = os::estd::chrono::high_resolution_clock::now ();
  printf ("high_resolution_clock::now() = %u ns\n",
          static_cast<uint32_t> (tp3.time_since_epoch ().count ()));

  this_thread::sleep_for (5_ticks);
  this_thread::sleep_for (5ms);
  this_thread::sleep_for (5001us); // ticks
  this_thread::sleep_for (5002000ns); // 3003 ticks

  this_thread::sleep_for (microseconds (1)); // 1 ticks
  this_thread::sleep_for (nanoseconds (1)); // 1 tick

  this_thread::sleep_for<chrono::systick_clock> (4_ticks);
  this_thread::sleep_for<chrono::systick_clock> (4ms);

  this_thread::sleep_for<chrono::realtime_clock> (1s);
  this_thread::sleep_for<chrono::realtime_clock> (1001ms);

  if (extra)
    {
      this_thread::sleep_for (2999_ticks);
      this_thread::sleep_for (3s);
      this_thread::sleep_for (3001ms);
      this_thread::sleep_for (3001001us); // 3002 ticks
      this_thread::sleep_for (3002000001ns); // 3003 ticks

      this_thread::sleep_for<chrono::systick_clock> (3999_ticks);
      this_thread::sleep_for<chrono::systick_clock> (4s);
      this_thread::sleep_for<chrono::systick_clock> (4001ms);

      this_thread::sleep_for<chrono::realtime_clock> (5s);
      this_thread::sleep_for<chrono::realtime_clock> (5001ms);
    }

  this_thread::sleep_until (chrono::system_clock::now () + 1000us);
  this_thread::sleep_until (chrono::system_clock::now () + 1ms);

  this_thread::sleep_until (chrono::systick_clock::now () + 1us);
  this_thread::sleep_until (chrono::systick_clock::now () + 1ms);

  this_thread::sleep_until (chrono::realtime_clock::now () + 10ms);
  this_thread::sleep_until (chrono::realtime_clock::now () + 100ms);
  this_thread::sleep_until (chrono::realtime_clock::now () + 1000ms);

  this_thread::sleep_until (chrono::realtime_clock::now () + 1s);

  if (extra)
    {
      this_thread::sleep_until (chrono::system_clock::now () + 1s);

      this_thread::sleep_until (chrono::systick_clock::now () + 1s);

      this_thread::sleep_until (chrono::realtime_clock::now () + 10ms);
      this_thread::sleep_until (chrono::realtime_clock::now () + 10s);
      this_thread::sleep_until (chrono::realtime_clock::now () + 1min);
    }

#pragma GCC diagnostic pop

  //std::chrono::system_clock::time_point tp1 = std::chrono::system_clock::now() + 1000ms;
  //this_thread::sleep_until (std::chrono::system_clock::now());

  //Systick_clock::time_point tp1 = Systick_clock::now() + 1000_ticks;
  //this_thread::sleep_until (tp1);

  my_sleep (70);

  printf ("\n%s - Done.\n", test_name);
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
