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

#include <cmsis-plus/estd/condition_variable>

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
    condition_variable::notify_one () noexcept
    {
      os::rtos::result_t res;
      res = ncv_.signal ();
      if (res != os::rtos::result::ok)
        {
          os::estd::__throw_cmsis_error (
              static_cast<int> (res),
              "condition_variable::notify_one() failed");
        }
    }

    void
    condition_variable::notify_all () noexcept
    {
      os::rtos::result_t res;
      res = ncv_.broadcast ();
      if (res != os::rtos::result::ok)
        {
          os::estd::__throw_cmsis_error (
              static_cast<int> (res),
              "condition_variable::notify_all() failed");
        }
    }

    void
    condition_variable::wait (std::unique_lock<mutex>& lk)
    {
      if (!lk.owns_lock ())
        {
          os::estd::__throw_system_error (
              EPERM, "condition_variable::wait: mutex not locked");
        }
      os::rtos::result_t res = ncv_.wait ((*(lk.mutex ()->native_handle ())));
      if (res != os::rtos::result::ok)
        {
          os::estd::__throw_cmsis_error (static_cast<int> (res),
                                         "condition_variable wait failed");
        }
    }

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-noreturn"
#endif

    void
    notify_all_at_thread_exit (condition_variable& cond,
                               std::unique_lock<mutex> lk)
    {
      //__thread_local_data()->notify_all_at_thread_exit(&cond, lk.release());
      std::abort (); // Not implemented
    }

#pragma GCC diagnostic pop

  // ==========================================================================
  } /* namespace estd */
} /* namespace os */

// ----------------------------------------------------------------------------

