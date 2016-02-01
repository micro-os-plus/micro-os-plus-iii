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

#include <cmsis-plus/std/mutex>

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
      mutex::lock ()
      {
        rtos::status_t ret;
        ret = nm_.lock ();
        if (ret != rtos::status::ok)
          {
            __throw_cmsis_error ((int) ret, "mutex lock failed");
          }
      }

      bool
      mutex::try_lock ()
      {
        rtos::status_t ret;
        ret = nm_.try_lock ();
        if (ret == rtos::status::ok)
          {
            return true;
          }
        else if (ret == rtos::status::error_timeout_resource)
          {
            return false;
          }

        __throw_cmsis_error ((int) ret, "mutex try_lock failed");
        return false;
      }

      void
      mutex::unlock ()
      {
        rtos::status_t ret;
        ret = nm_.unlock ();
        if (ret != rtos::status::ok)
          {
            __throw_cmsis_error ((int) ret, "mutex unlock failed");
          }
      }

      // ======================================================================

      void
      recursive_mutex::lock ()
      {
        rtos::status_t ret;
        ret = nm_.lock ();
        if (ret != rtos::status::ok)
          {
            __throw_cmsis_error ((int) ret, "recursive_mutex lock failed");
          }
      }

      bool
      recursive_mutex::try_lock () noexcept
      {
        rtos::status_t ret;
        ret = nm_.try_lock ();
        if (ret == rtos::status::ok)
          {
            return true;
          }
        else if (ret == rtos::status::error_timeout_resource)
          {
            return false;
          }

        __throw_cmsis_error ((int) ret, "recursive_mutex try_lock failed");
        return false;
      }

      void
      recursive_mutex::unlock ()
      {
        rtos::status_t ret;
        ret = nm_.unlock ();
        if (ret != rtos::status::ok)
          {
            __throw_cmsis_error ((int) ret, "recursive_mutex unlock failed");
          }
      }

    // ======================================================================

    } /* namespace std */
  } /* namespace cmsis */
} /* namespace os */

// ----------------------------------------------------------------------------
