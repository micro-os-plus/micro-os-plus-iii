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

#include <cmsis-plus/std/condition_variable>
#include <cmsis-plus/std/system_error>

using namespace os::cmsis;

// ----------------------------------------------------------------------------

namespace os
{
  namespace cmsis
  {
    namespace std
    {
      // ======================================================================

      void
      condition_variable::notify_one () noexcept
      {
        rtos::status_t ret;
        ret = ncv_.notify_one ();
        if (ret != rtos::status::ok)
          {
            __throw_cmsis_error ((int) ret,
                                 "condition_variable::notify_one() failed");
          }
      }

      void
      condition_variable::notify_all () noexcept
      {
        rtos::status_t ret;
        ret = ncv_.notify_all ();
        if (ret != rtos::status::ok)
          {
            __throw_cmsis_error ((int) ret,
                                 "condition_variable::notify_all() failed");
          }
      }

#if 0
      void
      condition_variable::__do_timed_wait (
          unique_lock<mutex>& lk,
          ::std::chrono::time_point<::std::chrono::system_clock,
              ::std::chrono::nanoseconds> tp) noexcept
      {
        using namespace ::std::chrono;

        if (!lk.owns_lock ())
          __throw_system_error (
              EPERM, "condition_variable::timed wait: mutex not locked");
        nanoseconds d = tp.time_since_epoch ();
        if (d > nanoseconds (0x59682F000000E941))
          d = nanoseconds (0x59682F000000E941);
        timespec ts;
        seconds s = duration_cast<seconds> (d);
        typedef decltype(ts.tv_sec) ts_sec;
        constexpr ts_sec ts_sec_max = ::std::numeric_limits<ts_sec>::max ();
        if (s.count () < ts_sec_max)
          {
            ts.tv_sec = static_cast<ts_sec> (s.count ());
            ts.tv_nsec = static_cast<decltype(ts.tv_nsec)> ((d - s).count ());
          }
        else
          {
            ts.tv_sec = ts_sec_max;
            ts.tv_nsec = ::std::giga::num - 1;
          }
        int ec = pthread_cond_timedwait (&__cv_, lk.mutex ()->native_handle (),
                                         &ts);
        if (ec != 0 && ec != ETIMEDOUT)
          __throw_system_error (ec, "condition_variable timed_wait failed");
      }
#endif
    } /* namespace std */
  } /* namespace cmsis */
} /* namespace os */

// ----------------------------------------------------------------------------
