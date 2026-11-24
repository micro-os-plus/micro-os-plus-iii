/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2026 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can be
 * obtained from https://opensource.org/licenses/mit.
 */

/*
 * Functional tests of the C API.
 */

#include <cmsis-plus/rtos/os-c-api.h>

#include <stdio.h>

#include <test-unit.h>

// ----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wdeclaration-after-statement"
#endif

static const char* test_name = "Unit C API";

// Counts the timer callbacks, to check one-shot and periodic timers.
static volatile unsigned int tm_count;

static void
tmfunc_count (void* args __attribute__ ((unused)))
{
  ++tm_count;
}

// ----------------------------------------------------------------------------

int
test_unit_c_api (void)
{
  printf ("\n%s - started\n", test_name);

  printf ("\n%s - Timers\n", test_name);

  {
    // NULL attributes create a one-shot timer, as in C++.
    os_timer_t tm4;
    os_timer_construct (&tm4, "tm4", tmfunc_count, NULL, NULL);

    tm_count = 0;
    os_sysclock_sleep_for (1); // Sync
    os_timer_start (&tm4, 1);

    os_sysclock_sleep_for (5);

    os_timer_stop (&tm4); // EAGAIN if already expired.
    os_timer_destruct (&tm4);

    if (tm_count != 1)
      {
        printf ("%s - default timer fired %u times, expected 1\n", test_name,
                tm_count);
        return 1;
      }
  }

  {
    // An explicit periodic attribute repeats.
    os_timer_t tm5;
    os_timer_construct (&tm5, "tm5", tmfunc_count, NULL,
                        os_timer_attr_get_periodic ());

    tm_count = 0;
    os_sysclock_sleep_for (1); // Sync
    os_timer_start (&tm5, 1);

    os_sysclock_sleep_for (5);

    os_timer_stop (&tm5);
    os_timer_destruct (&tm5);

    if (tm_count < 2)
      {
        printf ("%s - periodic timer fired %u times, expected at least 2\n",
                test_name, tm_count);
        return 1;
      }
  }

  printf ("%s - default one-shot and periodic timers ok\n", test_name);

  printf ("\n%s - done\n", test_name);

  return 0;
}

// ----------------------------------------------------------------------------
