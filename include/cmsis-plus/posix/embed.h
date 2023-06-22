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

#ifndef POSIX_EMBED_H_
#define POSIX_EMBED_H_

// ----------------------------------------------------------------------------

#include <unistd.h>

#if !defined(_POSIX_VERSION)

#ifdef __cplusplus
extern "C"
{
#endif

// ----------------------------------------------------------------------------

  int __attribute__((weak))
  ioctl (int fildes, unsigned long request, ...);

  void
  sync (void);

// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* !defined(_POSIX_VERSION) */

#endif /* POSIX_EMBED_H_ */
