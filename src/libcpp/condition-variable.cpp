/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <cmsis-plus/iso/condition_variable>
#include <cmsis-plus/iso/system_error>
#include <cstdlib>

// ----------------------------------------------------------------------------

namespace os
{
  namespace estd
  {
    // ========================================================================

    using namespace os;

    void
    condition_variable::notify_one () noexcept
    {
      rtos::result_t res;
      res = ncv_.signal ();
      if (res != rtos::result::ok)
        {
          __throw_cmsis_error (static_cast<int> (res),
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
          __throw_cmsis_error (static_cast<int> (res),
                               "condition_variable::notify_all() failed");
        }
    }

    void
    condition_variable::wait (unique_lock<mutex>& lk)
    {
      if (!lk.owns_lock ())
        __throw_system_error (EPERM,
                              "condition_variable::wait: mutex not locked");
      rtos::result_t res = ncv_.wait (
      /*(rtos::mutex&)*/(*(lk.mutex ()->native_handle ())));
      if (res != rtos::result::ok)
        __throw_cmsis_error (static_cast<int> (res),
                             "condition_variable wait failed");
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-noreturn"

    void
    notify_all_at_thread_exit (condition_variable& cond, unique_lock<mutex> lk)
    {
      //__thread_local_data()->notify_all_at_thread_exit(&cond, lk.release());
      std::abort (); // Not implemented
    }

#pragma GCC diagnostic pop

  // --------------------------------------------------------------------------

  } /* namespace estd */
} /* namespace os */

// ----------------------------------------------------------------------------

