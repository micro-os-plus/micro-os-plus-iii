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

void
thread (const void* args)
{
  ;
}
osThreadDef(thread, osPriorityNormal, 1, 0);

void
timer_callback (const void* args)
{
  ;
}
osTimerDef(timer1, timer_callback);

int
main (int argc, char* argv[])
{
    {
      // Keil API.

      osThreadId th = osThreadCreate (osThread(thread), nullptr);
      osThreadTerminate (th);

      osTimerId tm = osTimerCreate (osTimer(timer1), osTimerOnce, nullptr);
      osTimerDelete (tm);
    }

    {
      // Extended API.

      osThread th;
      osThreadCreateEx (&th, "thread", thread, osPriorityNormal, nullptr, 0, 1,
                        nullptr);

      osThreadTerminate (&th);

      osTimer tm;
      osTimerCreateEx (&tm, "timer", timer_callback, osTimerOnce, nullptr);

      osTimerDelete (&tm);
    }

  printf ("%s done.", argv[0]);
  return 0;
}

#pragma GCC diagnostic pop
