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

#include <cstdio>

extern void
test_serial ();

extern void
test_usbd (void);

extern void
test_usbh (void);

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

int
main (int argc, char* argv[])
{
  test_serial ();

  test_usbd ();

  test_usbh ();

  printf ("%s done.", argv[0]);
  return 0;
}

#pragma GCC diagnostic pop
