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

#include <cmsis-plus/rtos/os.h>

#include <cerrno>
#include <cstdio>

#include <test-unit.h>

// ----------------------------------------------------------------------------

int
os_main (int argc __attribute__ ((unused)),
         char* argv[] __attribute__ ((unused)))
{
  printf ("\n");
  printf ("µOS++ RTOS unit tests\n");
#if defined(__clang__)
  printf ("Built with clang " __VERSION__ "\n");
#else
  printf ("Built with GCC " __VERSION__ "\n");
#endif

  int ret = 0;
  errno = 0;

  if (ret == 0)
    {
      ret = test_unit_memory ();
      printf ("errno=%d\n", errno);
      errno = 0;
    }

  if (ret == 0)
    {
      ret = test_unit_rtos ();
      printf ("errno=%d\n", errno);
      errno = 0;
    }

  if (ret == 0)
    {
      ret = test_unit_c_api ();
      printf ("errno=%d\n", errno);
      errno = 0;
    }

  printf ("done %d\n", ret);
  return ret;
}

// ----------------------------------------------------------------------------
