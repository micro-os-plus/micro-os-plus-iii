/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
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

#include <cstring>
#include <cstdio>

#include <test.h>

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/diag/trace.h>

using namespace os;
using namespace os::rtos;

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#endif

mutex mx;

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

class periodic;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

class mutex_test
{
public:

  mutex_test (const char* name);

  void*
  object_main (void);

  rtos::thread&
  thread (void)
  {
    return th_;
  }

protected:

  friend class periodic;

  unsigned int min_micros_ = 10;
  unsigned int max_micros_ = 90;
  unsigned int min_ticks_ = 10;
  unsigned int max_ticks_ = 200;

  clock::duration_t ticks_ = 0;
  unsigned int accumulated_count_ = 0;
  unsigned int count_ = 0;

  rtos::thread th_;
};

#pragma GCC diagnostic pop

mutex_test::mutex_test (const char* name) :
    th_
      { name, [](void* attr)-> void*
        { return static_cast<mutex_test*> (attr)->object_main ();}, this }
{
  trace::printf ("%s @%p %s\n", __func__, this, name);
}

void*
mutex_test::object_main (void)
{
  while (!thread ().interrupted ())
    {
      unsigned int nbusy = (static_cast<unsigned int> (rand ())
          % (max_micros_ - min_micros_)) + min_micros_;
      unsigned int nsleep = (static_cast<unsigned int> (rand ())
          % (max_ticks_ - min_ticks_)) + min_ticks_;

      // simulate a period of intense activity
      busy_wait (nbusy);

      // simulate a period of waiting for an external event
      sysclock.sleep_for (nsleep);
      ticks_ += nsleep;

      mx.lock ();
        {
          nbusy = (static_cast<unsigned int> (rand ())
              % (max_micros_ / 10 - min_micros_ / 10)) + min_micros_ / 10;
          nsleep = (static_cast<unsigned int> (rand ())
              % (max_ticks_ / 10 - min_ticks_ / 10)) + min_ticks_ / 10;

          // simulate a period of intense activity
          busy_wait (nbusy);

          // simulate a period of waiting for an external event
          sysclock.sleep_for (nsleep);
          ticks_ += nsleep;

          accumulated_count_++;
          count_++;
        }
      mx.unlock ();
    }
  return nullptr;
}

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#endif
mutex_test* mt[10];
#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

class periodic
{
public:
  periodic (unsigned int seconds);

  void*
  object_main (void);

  rtos::thread&
  thread (void)
  {
    return th_;
  }

protected:
  unsigned int seconds_;
  rtos::thread th_;
};

#pragma GCC diagnostic pop

periodic::periodic (unsigned int seconds) :
    seconds_ (seconds), //
    th_
      { "P", [](void* attr)-> void*
        { return static_cast<periodic*> (attr)->object_main ();}, this }
{
  trace::printf ("%s @%p\n", __func__, this);
}

void*
periodic::object_main (void)
{
  th_.sched_prio (thread::priority::above_normal);

  unsigned int t = 0;
  while (true)
    {
      //realtime_clock.sleep_for (5);
      sysclock.sleep_for (5000);
      t += 5;

        {
          scheduler::critical_section scs;

          printf ("[%3us] ", t);

          unsigned int sum = 0;
          for (auto m : mt)
            {
              unsigned int cnt = m->accumulated_count_;

              sum += cnt;

              //os::core::timer::ticks_t ticks = pTask->getTicks();
              printf ("%s:%-4u ", m->thread ().name (), cnt);
            }
          int average = static_cast<int>((sum + ((sizeof(mt) / sizeof(mt[0])) / 2))
              / (sizeof(mt) / sizeof(mt[0])));

          printf ("sum=%u, avg=%u", sum, average);

          int min = 0;
          int max = 0;

          for (auto m : mt)
            {
              int delta = static_cast<int> (m->accumulated_count_);
              delta -= average;

              if (delta < min)
                min = delta;

              if (delta > max)
                max = delta;
            }

          printf (", delta in [%d,%d] [%d%%,%d%%]", min, max,
                  (min * 100 + average / 2) / average,
                  (max * 100 + average / 2) / average);

          puts ("");

          // ----- end of critical section ------------------------------------

        }

      if (seconds_ != 0 && t > seconds_)
        break;
    }

  for (auto m : mt)
    {
      m->thread ().interrupt ();
      m->thread ().join ();
    }
  return nullptr;
}

// ----------------------------------------------------------------------------

int
run_tests (unsigned int seconds)
{
#if 1
  mutex_test mt0 ("t0");
  mutex_test mt1 ("t1");
  mutex_test mt2 ("t2");
  mutex_test mt3 ("t3");
  mutex_test mt4 ("t4");
  mutex_test mt5 ("t5");
  mutex_test mt6 ("t6");
  mutex_test mt7 ("t7");
  mutex_test mt8 ("t8");
  mutex_test mt9 ("t9");

  mt[0] = &mt0;
  mt[1] = &mt1;
  mt[2] = &mt2;
  mt[3] = &mt3;
  mt[4] = &mt4;
  mt[5] = &mt5;
  mt[6] = &mt6;
  mt[7] = &mt7;
  mt[8] = &mt8;
  mt[9] = &mt9;
#endif

  periodic pm
    { seconds };

  pm.thread ().join ();

  puts ("Done.");
  return 0;
}

