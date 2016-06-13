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

#include <cmsis-plus/iso/chrono>
#include <cmsis-plus/rtos/port/os-inlines.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace estd
  {
    namespace chrono
    {

      // ----------------------------------------------------------------------

      using namespace os;

      // Number of seconds from epoch (1 January 1970 00:00:00 UTC)
      // when the system was started.
      // Must be set during startup by reading the RTC.
      // uint64_t startup_absolute_seconds;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

      // ======================================================================

      systick_clock::time_point
      systick_clock::now () noexcept
      {
        const auto ticks = rtos::sysclock.now ();
        return time_point
          { duration
            { ticks } };
      }

      // ======================================================================

      realtime_clock::time_point
      realtime_clock::now () noexcept
      {
        const auto secs = rtos::rtclock.now ();
        return time_point
          { duration
            { secs } };
      }

      realtime_clock::time_point realtime_clock::startup_time_point;

      // ======================================================================

      system_clock::time_point
      system_clock::now () noexcept
      {
        const auto ticks = rtos::sysclock.now ();
        return time_point
          { duration
            { systicks
              { ticks } + realtime_clock::startup_time_point.time_since_epoch () //
            } //
          };
      }

      time_t
      system_clock::to_time_t (const time_point& t) noexcept
      {
        return time_t (
            std::chrono::duration_cast<std::chrono::seconds> (
                t.time_since_epoch ()).count ());
      }

      system_clock::time_point
      system_clock::from_time_t (time_t t) noexcept
      {
        return system_clock::time_point (std::chrono::seconds (t));
      }

      // ======================================================================

      high_resolution_clock::time_point
      high_resolution_clock::now () noexcept
      {
        auto cycles = rtos::hrclock.now ();

        // The duration is the number of sum of SysTick ticks plus the current
        // count of CPU cycles (computed from the SysTick counter).
        // Notice: a more exact solution would be to compute
        // ticks * divisor + cycles, but this severely reduces the
        // range of ticks.
        return time_point
          { duration
            { duration
              { cycles * 1000000000ULL
                  / rtos::hrclock.input_clock_frequency_hz () }
                + realtime_clock::startup_time_point.time_since_epoch () } //
          };
      }

#pragma GCC diagnostic pop

    // ------------------------------------------------------------------------

    } /* namespace chrono */
  } /* namespace estd */
} /* namespace os */

// ----------------------------------------------------------------------------
