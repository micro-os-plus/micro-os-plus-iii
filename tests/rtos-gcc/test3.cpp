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

#include <cmsis-plus/rtos/os.h>

namespace _3
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// ----------------------------------------------------------------------------

  using args_t = struct
    {
      int n;
    };

  void*
  task1 (const args_t* args);

  void
  timer_callback (const void* args);

// ----------------------------------------------------------------------------

  void*
  task1 (const args_t* args)
  {
    return NULL;
  }

// ----------------------------------------------------------------------------

  void
  timer_callback (const void* args)
  {
    ;
  }

// ----------------------------------------------------------------------------

  extern "C" int
  test3 (void);

  int
  test3 (void)
  {
    using namespace os;

    args_t args
      { 7 };

      {
        // Thread with default attributes
        rtos::Thread th1
          { (rtos::thread::func_t) task1, (rtos::thread::func_args_t) &args };

        static uint8_t stack2[300];
        rtos::thread::Attributes attr2
          { "thread2" };
        attr2.th_stack_address = ((void*) stack2);
        attr2.th_stack_size_bytes = (sizeof(stack2));
        attr2.th_priority = (rtos::thread::priority::normal);

        // Thread with custom attributes
        rtos::Thread th13
          { attr2, (rtos::thread::func_t) task1,
              (rtos::thread::func_args_t) &args };
      }

      {
        rtos::mutex mx1;

        mx1.lock ();

        mx1.try_lock ();
        mx1.timed_lock (7);

        mx1.unlock ();

        rtos::thread::priority_t prio_ceiling;
        mx1.get_prio_ceiling (&prio_ceiling);

        rtos::thread::priority_t old_prio_ceiling;

        mx1.set_prio_ceiling (77, &old_prio_ceiling);

        mx1.set_prio_ceiling (88);

        rtos::mutex mx2
          { rtos::mutex::normal_initializer };

        rtos::mutex mx3
          { rtos::mutex::recursive_initializer };

        rtos::mutex::Attributes attr4
          { "mx4" };
        attr4.mx_priority_ceiling = (77);
        attr4.mx_protocol = (rtos::mutex::protocol::inherit);
        attr4.mx_robustness = (rtos::mutex::robustness::robust);
        attr4.mx_type = (rtos::mutex::type::recursive);

        rtos::mutex mx4
          { attr4 };
      }

      {
        rtos::this_thread::get ();

        rtos::this_thread::yield ();
      }
    return 0;
  }

#pragma GCC diagnostic pop
}
