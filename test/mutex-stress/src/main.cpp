/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/diag/trace.h>

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <sys/time.h>

#include <test.h>

using namespace os;
using namespace os::rtos;

#if defined(__ARM_EABI__)

void
busy_wait (unsigned int micros)
{
  clock::timestamp_t start = hrclock.now ();
  clock::timestamp_t until_cycles = start
      + hrclock.input_clock_frequency_hz() * micros / 1000000;

  clock::timestamp_t now_cycles;
  do
    {
      now_cycles = hrclock.now ();
    }
  while (now_cycles < until_cycles);
}

#else

void
busy_wait (unsigned int micros)
  {
    struct timeval tp;
    gettimeofday (&tp, nullptr);
    uint64_t until_micros;
    until_micros = static_cast<uint64_t> (tp.tv_sec * 1000000 + tp.tv_usec)
    + micros;

    uint64_t now_micros;
    do
      {
        gettimeofday (&tp, nullptr);
        now_micros = static_cast<uint64_t> (tp.tv_sec * 1000000 + tp.tv_usec);
      }
    while (now_micros < until_micros);
  }

#endif

int
os_main (int argc, char* argv[])
{
  unsigned int seconds = 30;
  if (argc > 1)
    {
      seconds = static_cast<unsigned int> (atoi (argv[1]));
    }

  printf ("\nMutex stress & uniformity test\n");
#if defined(__clang__)
  printf ("Built with clang " __VERSION__ "\n");
#else
  printf ("Built with GCC " __VERSION__ "\n");
#endif

  uint32_t seed;

  int status;
  struct timeval tp;
  gettimeofday (&tp, nullptr);
  // Use some large prime numbers and the current time.
  // Must be a 32-bits value, to overflows and mess things further.
  seed =
      static_cast<uint32_t> ((tp.tv_sec + tp.tv_usec + 15485863) * 179424673);

  printf ("Seed %u\n", static_cast<unsigned int> (seed));

  srand (seed);

  status = run_tests (seconds);
  return status;
}

