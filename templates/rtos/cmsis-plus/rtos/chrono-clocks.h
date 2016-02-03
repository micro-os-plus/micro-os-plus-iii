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

#ifndef CMSIS_PLUS_CHRONO_CLOCKS_H_
#define CMSIS_PLUS_CHRONO_CLOCKS_H_

// ----------------------------------------------------------------------------

#include <cmsis-plus/rtos/os.h>
#include <chrono>

// ----------------------------------------------------------------------------

namespace os
{
  namespace cmsis
  {
    namespace std
    {
      // ======================================================================

      // CMSIS++ SysTick clock.

      class Systick_clock
      {
      public:

        /// type of tick counter
        using rep = uint64_t;
        /// std::ratio type representing the tick period of the clock, in seconds
        using period = ::std::ratio<1, os::cmsis::rtos::kernel::sys_tick_frequency_hz>;
        /// basic duration type of clock
        using duration = ::std::chrono::duration<rep, period>;
        /// basic time_point type of clock
        using time_point = ::std::chrono::time_point<Systick_clock>;
        static constexpr bool is_steady
          { true };

        /**
         * @return time_point representing the current value of the clock
         */
        static time_point
        now () noexcept;
      };

      using systicks = Systick_clock::duration;

      // ======================================================================

      // CMSIS++ RTC.

      class Realtime_clock
      {
      public:

        using duration = ::std::chrono::seconds;
        using rep = duration::rep;
        using period = duration::period;
        using time_point = ::std::chrono::time_point<Realtime_clock>;

        // Non-monotonic, may be adjusted back in time.
        static constexpr const bool is_steady
          { false };

        static time_point
        now () noexcept;

        // --------------------------------------------------------------------
        // Extension to ISO

        // Number of seconds from epoch (1 January 1970 00:00:00 UTC)
        // Must be initialised during startup with the value of now().
        // Realtime_clock::startup_time_point = Realtime_clock::now();
        static time_point startup_time_point;
      };

      // ======================================================================

      // The system_clock is basically derived from the SysTick counts.
      // The counter is monotonic, and normally should not be adjusted,
      // so the clock is steady.

      class system_clock
      {
      public:

        using duration = ::std::chrono::microseconds;
        using rep = duration::rep;
        using period = duration::period;
        using time_point = ::std::chrono::time_point<system_clock>;

        // Monotonic, never adjusted back in time.
        static constexpr const bool is_steady
          { true };

        static time_point
        now () noexcept;

        // It is the only C++ clock that has the ability to map its
        // time points to C-style time, and, therefore, to be displayed.
        static time_t
        to_time_t (const time_point& tp) noexcept;
        static time_point
        from_time_t (time_t t) noexcept;

      };

      // ======================================================================

      // To simplify things, we assumed the system clock is already steady.
      using steady_clock = system_clock;

      // ======================================================================

      // The high resolution clock is also based on SysTick, but also uses
      // the counter instant value, which gives 1 CPU cycle resolution.

      class high_resolution_clock
      {
      public:

        using duration = ::std::chrono::nanoseconds;
        using rep = duration::rep;
        using period = duration::period;
        using time_point = ::std::chrono::time_point<high_resolution_clock>;

        // Monotonic, never adjusted back in time.
        static constexpr const bool is_steady
          { true };

        static time_point
        now () noexcept;
      };

    } /* namespace std */
  } /* namespace cmsis */
} /* namespace os */

#endif /* CMSIS_PLUS_CHRONO_CLOCKS_H_ */
