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

#include <cstdio>

#include <test-cpp-api.h>
#include <test-c-api.h>
#include <test-iso-api.h>
#include <test-posix-io-api.h>
#include <test-chan-fatfs.h>

#include <test-cpp-mem.h>

#if defined(OS_USE_SEMIHOSTING_SYSCALLS)
#include <cmsis-plus/arm/semihosting.h>
#endif

// ----------------------------------------------------------------------------

int
os_main (int argc __attribute__((unused)), char* argv[] __attribute__((unused)))
{
  printf ("\n");
  printf ("µOS++ RTOS simple APIs test\n");
#if defined(__clang__)
  printf ("Built with clang " __VERSION__ "\n");
#else
  printf ("Built with GCC " __VERSION__ "\n");
#endif

#if defined(MICRO_OS_PLUS_PLATFORM_NUCLEO_F767ZI)
  // workaround for the openocd bug.
  printf("\n");
#endif

  // fflush(stdout);


#if 0
  // Forever loop trying to diagnose openocd bug.
  for (int i = 0; ; ++i) {
    call_host(SEMIHOSTING_SYS_ERRNO, NULL);
    // putchar('.');
    if (i % 100000 == 0) {
      puts(".");
    }
  }
#endif

  int ret = 0;
  errno = 0;

#if 0
  if (ret == 0)
    {
      ret = test_cpp_mem ();
      printf ("errno=%d\n", errno);
      // fflush(stdout);
      errno = 0;
    }
#endif

#if 1
  if (ret == 0)
    {
      ret = test_cpp_api ();
      printf ("errno=%d\n", errno);
      // fflush(stdout);
      errno = 0;
    }
#endif

#if 1
  if (ret == 0)
    {
      ret = test_c_api ();
      printf ("errno=%d\n", errno);
      // fflush(stdout);
      errno = 0;
    }
#endif

#if 1
  if (ret == 0)
    {
      ret = test_iso_api (false);
      printf ("errno=%d\n", errno);
      // fflush(stdout);
      errno = 0;
    }
#endif

#if 1
  if (ret == 0)
    {
      ret = test_posix_io_api (false);
      printf ("errno=%d\n", errno);
      // fflush(stdout);
      errno = 0;
    }
#endif

#if 1
  if (ret == 0)
    {
      ret = test_chan_fatfs (false);
      printf ("errno=%d\n", errno);
      // fflush(stdout);
      errno = 0;
    }
#endif

  printf ("done\n");
  // fflush(stdout);
  return ret;
}

// ----------------------------------------------------------------------------
