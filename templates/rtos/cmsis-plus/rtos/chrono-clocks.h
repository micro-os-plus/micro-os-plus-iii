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

    } /* namespace std */
  } /* namespace cmsis */
} /* namespace os */

#endif /* CMSIS_PLUS_CHRONO_CLOCKS_H_ */
