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

#include <cmsis-plus/std/chrono>

// ----------------------------------------------------------------------------

namespace os
{
  namespace cmsis
  {
    namespace std
    {
      namespace chrono
      {

        // --------------------------------------------------------------------

        using namespace os::cmsis;

        // Number of seconds from epoch (1 January 1970 00:00:00 UTC)
        // when the system was started.
        // Must be set during startup by reading the RTC.
        uint64_t startup_absolute_seconds;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

        // ====================================================================

        Systick_clock::time_point
        Systick_clock::now () noexcept
        {
          const auto ticks = rtos::Systick_clock::now ();
          return time_point
            { duration
              { ticks } };
        }

        // ====================================================================

        Realtime_clock::time_point
        Realtime_clock::now () noexcept
        {
          const auto secs = rtos::Realtime_clock::now ();
          return time_point
            { duration
              { secs } };
        }

        Realtime_clock::time_point Realtime_clock::startup_time_point;

        // ======================================================================

        system_clock::time_point
        system_clock::now () noexcept
        {
          const auto ticks = rtos::Systick_clock::now ();
          return time_point
            { duration
              { systicks
                { ticks }
                  + Realtime_clock::startup_time_point.time_since_epoch () //
              } //
            };
        }

        time_t
        system_clock::to_time_t (const time_point& t) noexcept
        {
          return time_t (
              ::std::chrono::duration_cast<::std::chrono::seconds> (
                  t.time_since_epoch ()).count ());
        }

        system_clock::time_point
        system_clock::from_time_t (time_t t) noexcept
        {
          return system_clock::time_point (::std::chrono::seconds (t));
        }

        // ====================================================================

        high_resolution_clock::time_point
        high_resolution_clock::now () noexcept
        {
          rtos::Systick_clock::current_t systick;
          rtos::Systick_clock::now (&systick);

          // The duration is the sum of SysTick ticks plus the current
          // count of CPU cycles (computed from the SysTick counter).
          // Notice: a more exact solution would be to compute
          // ticks * divisor + cycles, but this severely reduces the
          // range of ticks.
          return time_point
            { duration
              { systicks
                { systick.ticks } + ::std::chrono::nanoseconds
                { systick.cycles * 1000000000ULL / systick.core_frequency_hz }
                  + Realtime_clock::startup_time_point.time_since_epoch () } //
            };
        }

#pragma GCC diagnostic pop

      // ----------------------------------------------------------------------

      } /* namespace chrono */
    } /* namespace std */
  } /* namespace cmsis */
} /* namespace os */

// ----------------------------------------------------------------------------
