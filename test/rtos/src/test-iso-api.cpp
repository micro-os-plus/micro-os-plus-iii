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

#include <test-iso-api.h>
#include <cmsis-plus/estd/chrono>
#include <cmsis-plus/estd/condition_variable>
#include <cmsis-plus/estd/mutex>
#include <cmsis-plus/estd/thread>
#include <type_traits>
#include <atomic>

// ----------------------------------------------------------------------------

static const char* test_name = "Test ISO C++ API";

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

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
using namespace os;

int
test_iso_api (bool extra)
{

  // ==========================================================================

  printf ("\n%s - Threads.\n", test_name);
    {
        {
          estd::thread th11
            { task1 };

          th11.join ();
        }

#if defined(OS_HAS_STD_THREADS) && defined(OS_INCLUDE_STD_THREADS)

        {
          std::thread th12
            { task1};

          th12.join ();
        }

#endif

#if 1
      // TODO: check thread termination; at a certain moment some user faults were triggered.
        {
          char c;

          estd::thread th21
            { task2, &c };

          estd::thread th31
            { task3, &c };

          estd::thread th41
            { task4, 7, "xyz" };

          th21.join ();
          th31.join ();
          th41.join ();
        }

#if defined(OS_HAS_STD_THREADS) && defined(OS_INCLUDE_STD_THREADS)

        {
          char c;

          std::thread th22
            { task2, &c};

          std::thread th32
            { task3, &c};

          std::thread th42
            { task4, 7, "xyz"};

          th22.join ();
          th32.join ();
          th42.join ();
        }

#endif

#endif

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Waggregate-return"
#endif
#pragma GCC diagnostic ignored "-Wunused-variable"

        {
          estd::thread::id id = estd::this_thread::get_id ();

          estd::this_thread::yield ();
        }

#if defined(OS_HAS_STD_THREADS) && defined(OS_INCLUDE_STD_THREADS)

        {
          std::thread::id id = std::this_thread::get_id ();

          std::this_thread::yield ();
        }

#endif

#pragma GCC diagnostic pop

    }

  // ==========================================================================
  printf ("\n%s - Mutexes.\n", test_name);

    {
        {
          estd::mutex mx11;

          mx11.lock ();
          mx11.unlock ();

          mx11.try_lock ();
          mx11.unlock ();
        }

        {
          estd::timed_mutex mx21;

          mx21.lock ();
          mx21.unlock ();

          mx21.try_lock ();
          mx21.unlock ();

          mx21.try_lock_for (systicks (2999));
          mx21.unlock ();
          mx21.try_lock_for (seconds (3));
          mx21.unlock ();
          mx21.try_lock_for (milliseconds (3001)); // 3001 ticks
          mx21.unlock ();
          mx21.try_lock_for (microseconds (3001001)); // 3002 ticks
          mx21.unlock ();
          mx21.try_lock_for (nanoseconds (3002000001ul)); // 3003 ticks
          mx21.unlock ();

          mx21.try_lock_for (microseconds (1)); // 1 tick
          mx21.unlock ();
          mx21.try_lock_for (nanoseconds (1)); // 1 tick
          mx21.unlock ();

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Waggregate-return"
#endif

          if (mx21.try_lock_until (estd::chrono::system_clock::now () + 5000us))
            mx21.unlock ();
          if (mx21.try_lock_until (estd::chrono::system_clock::now () + 5ms))
            mx21.unlock ();

          if (mx21.try_lock_until (estd::chrono::systick_clock::now () + 5us))
            mx21.unlock ();
          if (mx21.try_lock_until (estd::chrono::systick_clock::now () + 5ms))
            mx21.unlock ();

          if (mx21.try_lock_until (estd::chrono::realtime_clock::now () + 10ms))
            mx21.unlock ();
          if (mx21.try_lock_until (
              estd::chrono::realtime_clock::now () + 100ms))
            mx21.unlock ();

#pragma GCC diagnostic pop

        }

#if defined(OS_HAS_STD_THREADS) && defined(OS_INCLUDE_STD_THREADS)

        {
          std::mutex mx12;

          mx12.lock ();
          mx12.unlock ();

          mx12.try_lock ();
          mx12.unlock ();
        }

        {
          std::timed_mutex mx22;

          mx22.lock ();
          mx22.unlock ();

          mx22.try_lock ();
          mx22.unlock ();

          mx22.try_lock_for (systicks (2999));
          mx22.unlock ();
          mx22.try_lock_for (seconds (3));
          mx22.unlock ();
          mx22.try_lock_for (milliseconds (3001)); // 3001 ticks
          mx22.unlock ();
          mx22.try_lock_for (microseconds (3001001));// 3002 ticks
          mx22.unlock ();
          mx22.try_lock_for (nanoseconds (3002000001ul));// 3003 ticks
          mx22.unlock ();

          mx22.try_lock_for (microseconds (1));// 1 tick
          mx22.unlock ();
          mx22.try_lock_for (nanoseconds (1));// 1 tick
          mx22.unlock ();

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Waggregate-return"
#endif

          if (mx22.try_lock_until (estd::chrono::system_clock::now () + 5000us))
          mx22.unlock ();
          if (mx22.try_lock_until (estd::chrono::system_clock::now () + 5ms))
          mx22.unlock ();

          if (mx22.try_lock_until (estd::chrono::systick_clock::now () + 5us))
          mx22.unlock ();
          if (mx22.try_lock_until (estd::chrono::systick_clock::now () + 5ms))
          mx22.unlock ();

          if (mx22.try_lock_until (estd::chrono::realtime_clock::now () + 10ms))
          mx22.unlock ();
          if (mx22.try_lock_until (estd::chrono::realtime_clock::now () + 100ms))
          mx22.unlock ();

#pragma GCC diagnostic pop

        }

#endif
    }

  // ==========================================================================

  printf ("\n%s - Condition variables.\n", test_name);
    {
      estd::condition_variable cv11;
      cv11.notify_one ();
      cv11.notify_all ();

      if (extra)
        {

          estd::mutex mxl;
          std::unique_lock<estd::mutex> lock
            { mxl };

          cv11.wait (lock);

          auto pred = []()
            { return is_ready();};

          cv11.wait (lock, pred);

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Waggregate-return"
#endif

          cv11.wait_until (lock, estd::chrono::system_clock::now () + 10ms);
          cv11.wait_until (lock, estd::chrono::systick_clock::now () + 10ms);
          cv11.wait_until (lock, estd::chrono::realtime_clock::now () + 1s);

          cv11.wait_until (lock, estd::chrono::system_clock::now () + 10ms,
                           pred);
          cv11.wait_until (lock, estd::chrono::systick_clock::now () + 10ms,
                           pred);
          cv11.wait_until (lock, estd::chrono::realtime_clock::now () + 1s,
                           pred);

          cv11.wait_for (lock, 10_ticks);
          cv11.wait_for (lock, 10ms);
          cv11.wait_for (lock, 10000us);

          cv11.wait_for (lock, 12_ticks, pred);
          cv11.wait_for (lock, 12ms, pred);
          cv11.wait_for (lock, 12000us, pred);

          if (extra)
            {
              cv11.wait_for (lock, 2999_ticks);
              cv11.wait_for (lock, 3s);
              cv11.wait_for (lock, 3001ms);
              cv11.wait_for (lock, 3001001us); // 3002 ticks

              cv11.wait_for (lock, 2999_ticks, pred);
              cv11.wait_for (lock, 3s, pred);
              cv11.wait_for (lock, 3001ms, pred);
              cv11.wait_for (lock, 3001001us, pred); // 3002 ticks
            }

#pragma GCC diagnostic pop

        }
    }

#if defined(OS_HAS_STD_THREADS) && defined(OS_INCLUDE_STD_THREADS)

    {
      std::condition_variable cv12;
      cv12.notify_one ();
      cv12.notify_all ();

      if (extra)
        {

          std::mutex mxl;
          std::unique_lock<estd::mutex> lock
            { mxl};

          cv12.wait (lock);

          auto pred = []()
            { return is_ready();};

          cv12.wait (lock, pred);

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Waggregate-return"
#endif

          cv12.wait_until (lock, estd::chrono::system_clock::now () + 10ms);
          cv12.wait_until (lock, estd::chrono::systick_clock::now () + 10ms);
          cv12.wait_until (lock, estd::chrono::realtime_clock::now () + 1s);

          cv12.wait_until (lock, estd::chrono::system_clock::now () + 10ms, pred);
          cv12.wait_until (lock, estd::chrono::systick_clock::now () + 10ms, pred);
          cv12.wait_until (lock, estd::chrono::realtime_clock::now () + 1s, pred);

          cv12.wait_for (lock, 10_ticks);
          cv12.wait_for (lock, 10ms);
          cv12.wait_for (lock, 10000us);

          cv12.wait_for (lock, 12_ticks, pred);
          cv12.wait_for (lock, 12ms, pred);
          cv12.wait_for (lock, 12000us, pred);

          if (extra)
            {
              cv12.wait_for (lock, 2999_ticks);
              cv12.wait_for (lock, 3s);
              cv12.wait_for (lock, 3001ms);
              cv12.wait_for (lock, 3001001us); // 3002 ticks

              cv12.wait_for (lock, 2999_ticks, pred);
              cv12.wait_for (lock, 3s, pred);
              cv12.wait_for (lock, 3001ms, pred);
              cv12.wait_for (lock, 3001001us, pred);// 3002 ticks
            }

#pragma GCC diagnostic pop

        }
    }

#endif

  // ==========================================================================

  printf ("\n%s - Chrono.\n", test_name);

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Waggregate-return"
#endif

  estd::chrono::realtime_clock::startup_time_point =
      estd::chrono::realtime_clock::now ();

  estd::this_thread::sleep_for (5_ticks);

  auto tp = estd::chrono::systick_clock::now ();
  printf ("Systick_clock::now() = %lu ticks\n",
          static_cast<unsigned long int> (tp.time_since_epoch ().count ()));

  auto tp2 = os::estd::chrono::system_clock::now ();
  printf ("system_clock::now() = %lu us\n",
          static_cast<unsigned long int> (tp2.time_since_epoch ().count ()));

  auto tp3 = os::estd::chrono::high_resolution_clock::now ();
  printf ("high_resolution_clock::now() = %lu ns\n",
          static_cast<unsigned long int> (tp3.time_since_epoch ().count ()));

  estd::this_thread::sleep_for (5_ticks);
  estd::this_thread::sleep_for (5ms);
  estd::this_thread::sleep_for (5001us); // 5 ticks
  estd::this_thread::sleep_for (5002000ns); // 5 ticks

  estd::this_thread::sleep_for (microseconds (1)); // 1 tick
  estd::this_thread::sleep_for (nanoseconds (1)); // 1 tick

  estd::this_thread::sleep_for<estd::chrono::systick_clock> (4_ticks);
  estd::this_thread::sleep_for<estd::chrono::systick_clock> (4ms);

  printf ("sleep_for<chrono::realtime_clock> (1s)\n");
  estd::this_thread::sleep_for<estd::chrono::realtime_clock> (1s);
  printf ("sleep_for<chrono::realtime_clock> (1001ms)\n");
  estd::this_thread::sleep_for<estd::chrono::realtime_clock> (1001ms);

  if (extra)
    {
      estd::this_thread::sleep_for (2999_ticks);
      estd::this_thread::sleep_for (3s);
      estd::this_thread::sleep_for (3001ms);
      estd::this_thread::sleep_for (3001001us); // 3002 ticks
      estd::this_thread::sleep_for (3002000001ns); // 3003 ticks

      estd::this_thread::sleep_for<estd::chrono::systick_clock> (3999_ticks);
      estd::this_thread::sleep_for<estd::chrono::systick_clock> (4s);
      estd::this_thread::sleep_for<estd::chrono::systick_clock> (4001ms);

      estd::this_thread::sleep_for<estd::chrono::realtime_clock> (5s);
      estd::this_thread::sleep_for<estd::chrono::realtime_clock> (5001ms);
    }

  estd::this_thread::sleep_until (estd::chrono::system_clock::now () + 1000us);
  estd::this_thread::sleep_until (estd::chrono::system_clock::now () + 1ms);

  estd::this_thread::sleep_until (estd::chrono::systick_clock::now () + 1us);
  estd::this_thread::sleep_until (estd::chrono::systick_clock::now () + 1ms);

  estd::this_thread::sleep_until (estd::chrono::realtime_clock::now () + 10ms);
  estd::this_thread::sleep_until (estd::chrono::realtime_clock::now () + 100ms);
  printf ("sleep_until (chrono::realtime_clock::now () + 1000ms)\n");
  estd::this_thread::sleep_until (
      estd::chrono::realtime_clock::now () + 1000ms);

  printf ("sleep_until (chrono::realtime_clock::now () + 1s)\n");
  estd::this_thread::sleep_until (estd::chrono::realtime_clock::now () + 1s);

  if (extra)
    {
      estd::this_thread::sleep_until (estd::chrono::system_clock::now () + 1s);

      estd::this_thread::sleep_until (estd::chrono::systick_clock::now () + 1s);

      estd::this_thread::sleep_until (
          estd::chrono::realtime_clock::now () + 10ms);
      estd::this_thread::sleep_until (
          estd::chrono::realtime_clock::now () + 10s);
      estd::this_thread::sleep_until (
          estd::chrono::realtime_clock::now () + 1min);
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
  estd::this_thread::sleep_for (systicks (n));
#endif
}

#pragma GCC diagnostic pop
