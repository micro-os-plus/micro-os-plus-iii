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

#ifndef POSIX_IO_SYS_UIO_H_
#define POSIX_IO_SYS_UIO_H_

// ----------------------------------------------------------------------------

#include <unistd.h>

#if defined(_POSIX_VERSION)

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wgnu-include-next"
#endif
#include_next <sys/uio.h>
#pragma GCC diagnostic pop

#else

#include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

// ----------------------------------------------------------------------------

  struct iovec
  {
    void* iov_base;  // Base address of a memory region for input or output.
    size_t iov_len;   // The size of the memory pointed to by iov_base.
  };

  ssize_t
  writev (int fildes, const struct iovec* iov, int iovcnt);

// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* defined(_POSIX_VERSION) */

#endif /* POSIX_IO_SYS_UIO_H_ */
