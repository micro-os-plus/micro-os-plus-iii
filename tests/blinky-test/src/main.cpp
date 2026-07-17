/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2016-2025 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can be
 * obtained from https://opensource.org/licenses/mit.
 */

// ----------------------------------------------------------------------------

#include <cmsis-plus/rtos/os.h>

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <sys/time.h>

// The LED is defined in the platform.
#include "blink-led.h"

// ----------------------------------------------------------------------------

using namespace os;
using namespace os::rtos;

int
os_main (int argc, char* argv[])
{
  unsigned int ticks
      = (argc > 1) ? static_cast<unsigned int> (std::atoi (argv[1])) : 100;
  unsigned int cycles
      = (argc > 2) ? static_cast<unsigned int> (std::atoi (argv[2])) : 10;

  trace::printf ("os_main() started with %u ticks and %u cycles.\n", ticks,
                 cycles);

  blink_led led;

  led.power_up ();

  for (unsigned int count = 0; count < cycles; count++)
    {
      led.blink ();
      sysclock.sleep_for (ticks);
    }

  led.power_down ();

  return 0;
}

// ----------------------------------------------------------------------------
