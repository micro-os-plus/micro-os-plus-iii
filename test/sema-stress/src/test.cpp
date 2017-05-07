/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
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

#include <cstring>
#include <cmsis_device.h>

#include <test.h>

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/diag/trace.h>

using namespace os;
using namespace os::rtos;

static void
sema (uint32_t divisor);

void*
sleep_stress (void* args);

void*
sleep_stress (void* args __attribute__((unused)))
{
  this_thread::thread ().priority (thread::priority::below_normal);

  while (!this_thread::thread ().interrupted ())
    {
      sysclock.sleep_for (1);
    }
  return nullptr;
}

int
run_tests ()
{
#if 0
  sema (tmr.in_clk_hz ()/20);
#else

  thread low
    { "low", sleep_stress, nullptr };

  int i = 1;
  for (;; i *= 2)
    {
      uint32_t period = tmr.in_clk_hz () / rtos::clock_systick::frequency_hz
          / i;
      if (period < 10)
        {
          break;
        }

      sema (period);
    }

  puts ("\n\nRandom");

  uint32_t from = 10;
  uint32_t to = tmr.in_clk_hz () / rtos::clock_systick::frequency_hz / 32;

  for (i = 0; i < 100; ++i)
    {
      uint64_t r = (uint64_t) rand ();
      uint64_t period = r * (to - from) / RAND_MAX + from;

      sema ((uint32_t) period);
    }
#endif

  puts ("Done.");
  return 0;
}

constexpr std::size_t max_count = 1000;
//constexpr std::size_t max_count = 200;
uint32_t volatile buf[max_count + 10];
uint32_t volatile cnt;
uint32_t volatile delayed;
uint32_t volatile max_delayed;

semaphore_counting sem { max_count, 0 };

static void
sema_cb (void)
{
  buf[cnt] = cnt;
  ++cnt;
  delayed = 0;
  max_delayed = 0;

#if 1
  sem.post ();
#endif
  trace_putchar ('+');

  if (cnt == max_count)
    {
      tmr.stop ();
    }
}

void
sema (uint32_t cycles)
{
  // Clear buffer
  memset ((void*) buf, 0, sizeof(buf));

  cnt = 0;
  delayed = 0;

  sem.reset ();

  tim_callback = sema_cb;

  printf ("%7lu cy %4lu kHz ", cycles, tmr.in_clk_hz () / cycles / 1000);

  tmr.start (cycles);

#if 1
  uint32_t i;
  for (i = 0; i < max_count; ++i)
    {
      result_t res = sem.timed_wait (rtos::clock_systick::frequency_hz);
      assert(res == result::ok);

      trace_putchar ('-');
      assert(buf[i] == i);
      delayed++;
      if (delayed > max_delayed)
        {
          max_delayed = delayed;
        }
    }

#else
  while (cnt < max_count)
  ;
#endif

  // systick_clock.sleep_for (10);
  max_delayed--;
  if (max_delayed > 0)
    {
      printf ("%4lu late \n", max_delayed);
    }
  else
    {
      puts ("");
    }
}
