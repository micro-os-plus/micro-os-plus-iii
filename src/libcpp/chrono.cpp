/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/estd/chrono>

// ----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

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
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Waggregate-return"
#endif

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

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wuseless-cast"
#endif

      time_t
      system_clock::to_time_t (const time_point& t) noexcept
      {
        return time_t (
            std::chrono::duration_cast<std::chrono::seconds> (
                t.time_since_epoch ()).count ());
      }

#pragma GCC diagnostic pop

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

#pragma GCC diagnostic push
#if defined(__clang__)
// error: 'long long' is incompatible with C++98 [-Werror,-Wc++98-compat-pedantic]
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif
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
#pragma GCC diagnostic pop
      }

#pragma GCC diagnostic pop

    // ------------------------------------------------------------------------

    } /* namespace chrono */
  } /* namespace estd */
} /* namespace os */

// ----------------------------------------------------------------------------
