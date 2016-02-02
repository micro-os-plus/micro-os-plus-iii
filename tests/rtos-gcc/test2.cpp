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

#include <cmsis_os_ex.h>
#include <stdio.h>

namespace _2
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// ----------------------------------------------------------------------------

  using args_t = struct
    {
      int n;
    };

  void*
  task_function (const args_t* args);

  void
  timer_callback (const void* args);

// ----------------------------------------------------------------------------

  void*
  task_function (const args_t* args)
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
  test2 (void);

  int
  test2 (void)
  {
    args_t args;

      {
        // Keil API.

        // Thread is static, but stack is dynamically allocated.
        static osThreadDef(task_function, osPriorityNormal, 1, 0);
        osThreadId th = osThreadCreate (osThread(task_function), &args);

        osThreadGetPriority(th);
        osThreadTerminate (th);

        static osTimerDef(timer, timer_callback);
        osTimerId tm = osTimerCreate (osTimer(timer), osTimerOnce, NULL);
        osTimerDelete (tm);
      }

    return 0;
  }

#pragma GCC diagnostic pop
}
