/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#ifndef POSIX_UTIME_H_
#define POSIX_UTIME_H_

// ----------------------------------------------------------------------------

#include <unistd.h>

#if defined(_POSIX_VERSION)

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wgnu-include-next"
#endif
#include_next <utime.h>
// #include <sys/select.h>
#pragma GCC diagnostic pop

#else

#include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

// ----------------------------------------------------------------------------

  int
  utime (const char *path, const struct utimbuf* times);

// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* defined(_POSIX_VERSION) */

#endif /* POSIX_UTIME_H_ */
