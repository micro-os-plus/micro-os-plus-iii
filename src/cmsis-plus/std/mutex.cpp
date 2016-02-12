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

#include <cerrno>

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
        rtos::result_t res;
        res = nm_.lock ();
        if (res != rtos::result::ok)
          {
            __throw_cmsis_error ((int) res, "mutex lock failed");
          }
      }

      bool
      mutex::try_lock ()
      {
        rtos::result_t res;
        res = nm_.try_lock ();
        if (res == rtos::result::ok)
          {
            return true;
          }
        else if (res == ETIMEDOUT)
          {
            return false;
          }

        __throw_cmsis_error ((int) res, "mutex try_lock failed");
        return false;
      }

      void
      mutex::unlock ()
      {
        rtos::result_t res;
        res = nm_.unlock ();
        if (res != rtos::result::ok)
          {
            __throw_cmsis_error ((int) res, "mutex unlock failed");
          }
      }

      // ======================================================================

      void
      recursive_mutex::lock ()
      {
        rtos::result_t res;
        res = nm_.lock ();
        if (res != rtos::result::ok)
          {
            __throw_cmsis_error ((int) res, "recursive_mutex lock failed");
          }
      }

      bool
      recursive_mutex::try_lock () noexcept
      {
        rtos::result_t res;
        res = nm_.try_lock ();
        if (res == rtos::result::ok)
          {
            return true;
          }
        else if (res == ETIMEDOUT)
          {
            return false;
          }

        __throw_cmsis_error ((int) res, "recursive_mutex try_lock failed");
        return false;
      }

      void
      recursive_mutex::unlock ()
      {
        rtos::result_t res;
        res = nm_.unlock ();
        if (res != rtos::result::ok)
          {
            __throw_cmsis_error ((int) res, "recursive_mutex unlock failed");
          }
      }

    // ------------------------------------------------------------------------

    } /* namespace std */
  } /* namespace cmsis */
} /* namespace os */

// ----------------------------------------------------------------------------
