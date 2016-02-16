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

#include <cmsis-plus/iso/condition_variable>
#include <cmsis-plus/iso/system_error>
#include <cstdlib>

// ----------------------------------------------------------------------------

namespace os
{
  namespace estd
  {
    // ======================================================================

    using namespace os;

    void
    condition_variable::notify_one () noexcept
    {
      rtos::result_t res;
      res = ncv_.signal ();
      if (res != rtos::result::ok)
        {
          __throw_cmsis_error ((int) res,
                               "condition_variable::notify_one() failed");
        }
    }

    void
    condition_variable::notify_all () noexcept
    {
      rtos::result_t res;
      res = ncv_.broadcast ();
      if (res != rtos::result::ok)
        {
          __throw_cmsis_error ((int) res,
                               "condition_variable::notify_all() failed");
        }
    }

    void
    condition_variable::wait (unique_lock<mutex>& lk)
    {
      if (!lk.owns_lock ())
        __throw_system_error (EPERM,
                              "condition_variable::wait: mutex not locked");
      rtos::result_t res = ncv_.wait (lk.mutex ()->native_handle ());
      if (res != rtos::result::ok)
        __throw_cmsis_error ((int) res, "condition_variable wait failed");
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    void
    notify_all_at_thread_exit (condition_variable& cond, unique_lock<mutex> lk)
    {
      //__thread_local_data()->notify_all_at_thread_exit(&cond, lk.release());
      std::abort (); // Not implemented
    }

#pragma GCC diagnostic pop

  // ------------------------------------------------------------------------

  } /* namespace estd */
} /* namespace os */

// ----------------------------------------------------------------------------
