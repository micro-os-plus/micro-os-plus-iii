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
    using namespace os::cmsis;

    args_t args
      { 7 };

      {
        // Thread with default attributes
        rtos::Thread th1
          { (rtos::thread::func_t) task1, (rtos::thread::func_args_t) &args };

        static uint8_t stack2[300];
        rtos::thread::Attributes attr2
          { "thread2" };
        attr2.set_stack_address ((void*) stack2);
        attr2.set_stack_size_bytes (sizeof(stack2));
        attr2.set_priority (rtos::thread::priority::normal);

        // Thread with custom attributes
        rtos::Thread th13
          { attr2, (rtos::thread::func_t) task1,
              (rtos::thread::func_args_t) &args };
      }

      {
        rtos::Mutex mx1;

        mx1.lock ();

        mx1.try_lock ();
        mx1.timed_lock (7);

        mx1.unlock ();

        rtos::thread::priority_t prio_ceiling;
        mx1.get_prio_ceiling (&prio_ceiling);

        rtos::thread::priority_t old_prio_ceiling;

        mx1.set_prio_ceiling (77, &old_prio_ceiling);

        mx1.set_prio_ceiling (88);

        rtos::Mutex mx2
          { rtos::mutex::normal_initializer };

        rtos::Mutex mx3
          { rtos::mutex::recursive_initializer };

        rtos::mutex::Attributes attr4
          { "mx4" };
        attr4.set_prio_ceiling (77);
        attr4.set_protocol (rtos::mutex::protocol::inherit);
        attr4.set_robustness (rtos::mutex::robustness::robust);
        attr4.set_type (rtos::mutex::type::recursive);

        rtos::Mutex mx4
          { attr4 };
      }

    return 0;
  }

#pragma GCC diagnostic pop
}
