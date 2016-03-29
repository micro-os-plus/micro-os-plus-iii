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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// ----------------------------------------------------------------------------

static void*
task_function (const void* args);

static void
timer_callback (const void* args);

// ----------------------------------------------------------------------------

void*
task_function (const void* args)
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

int
test1 (void);

int
test1 (void)
{
  struct
  {
    int n;
  } args;

    {
      // Keil API.

      // Thread is static, but stack is dynamically allocated.
      static osThreadDef(task_function, osPriorityNormal, 1, 0)
      ;

      osThreadId th = osThreadCreate (osThread(task_function), &args);

      osThreadGetPriority (th);
      osThreadTerminate (th);

      static osTimerDef(timer, timer_callback)
      ;
      osTimerId tm = osTimerCreate (osTimer(timer), osTimerOnce, NULL);
      osTimerDelete (tm);
    }

#if 0
    {
      // Extended API.

      // POSIX Thread with default settings (stack is dynamically allocated).
      static osThread thread1;
      osThreadCreateEx (&thread1, NULL, (os_pthread) task_function, &args);

      osThreadGetPriority (&thread1);
      osThreadTerminate (&thread1);

      // POSIX Thread with static stack and full settings
      static uint8_t stack[300];
      const osThreadAttr attr =
        {
          //
          .name = "th2",//
          .stack_addr = stack,//
          .stack_size_bytes = sizeof(stack),//
          .priority = osPriorityNormal//
        };
      static osThread th2;
      osThreadCreateEx (&th2, &attr, (os_pthread) task_function, &args);

      osThreadGetPriority (&th2);
      osThreadTerminate (&th2);

      static osTimer tm;
      osTimerCreateEx (&tm, NULL, timer_callback, NULL);

      osTimerDelete (&tm);
    }
#endif

    {
      osThreadId id;
      id = osThreadGetId ();

      osThreadYield ();

    }
  return 0;
}

#pragma GCC diagnostic pop
