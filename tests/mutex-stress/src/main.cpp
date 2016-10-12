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

  printf ("\nMutex stress & uniformity test.\n");
#if defined(__clang__)
  printf ("Built with clang " __VERSION__ ".\n");
#else
  printf ("Built with GCC " __VERSION__ ".\n");
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

