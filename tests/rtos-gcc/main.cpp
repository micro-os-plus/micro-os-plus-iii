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
#include <cstdio>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// ----------------------------------------------------------------------------

void
task_function (const void* args);

void
timer_callback (const void* args);

// ----------------------------------------------------------------------------

void
task_function (const void* args)
{
  ;
}

osThreadDef(task_function, osPriorityNormal, 1, 0);

// ----------------------------------------------------------------------------

void
timer_callback (const void* args)
{
  ;
}

osTimerDef(timer, timer_callback);

// ----------------------------------------------------------------------------

int
main (int argc, char* argv[])
{
    {
      // Keil API.

      osThreadId th = osThreadCreate (osThread(task_function), nullptr);
      osThreadTerminate (th);

      osTimerId tm = osTimerCreate (osTimer(timer), osTimerOnce, nullptr);
      osTimerDelete (tm);
    }

    {
      // Extended API.

      osThread th;
      osThreadCreateEx (&th, "thread", osPriorityNormal, nullptr, 0,
                        task_function, nullptr);

      osThreadTerminate (&th);

      osTimer tm;
      osTimerCreateEx (&tm, "timer", timer_callback, osTimerOnce, nullptr);

      osTimerDelete (&tm);
    }

  printf ("%s done.", argv[0]);
  return 0;
}

#pragma GCC diagnostic pop
