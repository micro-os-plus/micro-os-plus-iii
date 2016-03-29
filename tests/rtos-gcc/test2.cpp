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
