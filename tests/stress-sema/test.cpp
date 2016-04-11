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

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/diag/trace.h>

#include "cmsis_device.h"

#include <test.h>

#include <cstring>

using namespace os;
using namespace os::rtos;

static void
sema (uint32_t divisor);

int
run_tests ()
{
  int i = 1;
  for (;; i *= 2)
    {
      uint32_t period = tmr.in_clk_hz () / rtos::Systick_clock::frequency_hz
          / i;
      if (period < 10)
        {
          break;
        }

      sema (period);
    }

  trace::puts("\n\nRandom");

  uint32_t from = 10;
  uint32_t to = tmr.in_clk_hz () / rtos::Systick_clock::frequency_hz / 32;

  for (i = 0; i < 100; ++i)
    {
      uint64_t r = (uint64_t)rand ();
      uint64_t period = r * (to - from) / RAND_MAX + from;

      sema ((uint32_t)period);
    }

  trace::puts("\nDone.");
  return 0;
}

constexpr std::size_t max_count = 1000;
uint16_t buf[max_count + 10];
uint16_t cnt;

Semaphore sem;

static void
sema_cb (void)
{
  buf[cnt] = cnt;
  ++cnt;
  sem.post ();
  trace_putchar ('+');

  if (cnt == max_count)
    {
      tmr.stop ();
    }
}

void
sema (uint32_t period)
{
  // Clear buffer
  memset (buf, 0, sizeof(buf));

  cnt = 0;

  sem.reset ();

  tim_callback = sema_cb;

  trace::printf ("\nP=%06d/%08d ", period, tmr.in_clk_hz ()/period);

  tmr.start (period);

  uint32_t i;
  for (i = 0; i < max_count; ++i)
    {
      sem.timed_wait (100);
      trace_putchar ('-');
      assert(buf[i] == i);
    }

}
