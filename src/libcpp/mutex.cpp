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

#include <cmsis-plus/estd/mutex>

// ----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace estd
  {
    // ========================================================================

    void
    mutex::lock ()
    {
      os::rtos::result_t res;
      res = nm_.lock ();
      if (res != os::rtos::result::ok)
        {
          os::estd::__throw_cmsis_error (static_cast<int> (res),
                                         "mutex lock failed");
        }
    }

    bool
    mutex::try_lock ()
    {
      os::rtos::result_t res;
      res = nm_.try_lock ();
      if (res == os::rtos::result::ok)
        {
          return true;
        }
      else if (res == EWOULDBLOCK)
        {
          return false;
        }

      os::estd::__throw_cmsis_error (static_cast<int> (res),
                                     "mutex try_lock failed");
      // return false;
    }

    void
    mutex::unlock ()
    {
      os::rtos::result_t res;
      res = nm_.unlock ();
      if (res != os::rtos::result::ok)
        {
          os::estd::__throw_cmsis_error (static_cast<int> (res),
                                         "mutex unlock failed");
        }
    }

    // ========================================================================

    void
    recursive_mutex::lock ()
    {
      os::rtos::result_t res;
      res = nm_.lock ();
      if (res != os::rtos::result::ok)
        {
          os::estd::__throw_cmsis_error (static_cast<int> (res),
                                         "recursive_mutex lock failed");
        }
    }

    bool
    recursive_mutex::try_lock () noexcept
    {
      os::rtos::result_t res;
      res = nm_.try_lock ();
      if (res == os::rtos::result::ok)
        {
          return true;
        }
      else if (res == EWOULDBLOCK)
        {
          return false;
        }

      os::estd::__throw_cmsis_error (static_cast<int> (res),
                                     "recursive_mutex try_lock failed");
      //return false;
    }

    void
    recursive_mutex::unlock ()
    {
      os::rtos::result_t res;
      res = nm_.unlock ();
      if (res != os::rtos::result::ok)
        {
          os::estd::__throw_cmsis_error (static_cast<int> (res),
                                         "recursive_mutex unlock failed");
        }
    }

  // ==========================================================================

  } /* namespace estd */
} /* namespace os */

// ----------------------------------------------------------------------------
