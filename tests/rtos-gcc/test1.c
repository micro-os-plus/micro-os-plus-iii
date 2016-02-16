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
