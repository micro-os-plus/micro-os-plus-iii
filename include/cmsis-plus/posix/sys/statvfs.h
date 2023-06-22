/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2018-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#ifndef POSIX_IO_SYS_STATVFS_H_
#define POSIX_IO_SYS_STATVFS_H_

// ----------------------------------------------------------------------------

#include <unistd.h>

#if defined(_POSIX_VERSION)

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wgnu-include-next"
#endif
#include_next <sys/statvfs.h>
#pragma GCC diagnostic pop

#else

#include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

// ----------------------------------------------------------------------------

// Read-only file system.
#define ST_RDONLY 0x00000001

// Does not support the semantics of the ST_ISUID and ST_ISGID file mode bits.
#define ST_NOSUID 0x00000002

  struct statvfs
  {
    unsigned long f_bsize; // File system block size.
    unsigned long f_frsize; // Fundamental file system block size.
    fsblkcnt_t f_blocks; // Total number of blocks on file system in units of f_frsize.
    fsblkcnt_t f_bfree; // Total number of free blocks.
    fsblkcnt_t f_bavail; // Number of free blocks available to non-privileged process.
    fsfilcnt_t f_files; // Total number of file serial numbers.
    fsfilcnt_t f_ffree; // Total number of free file serial numbers.
    fsfilcnt_t f_favail; // Number of file serial numbers available to non-privileged process.
    unsigned long f_fsid; // File system ID.
    unsigned long f_flag; // Bit mask of f_flag values.
    unsigned long f_namemax; // Maximum filename length.
  };

  int
  fstatvfs (int fildes, struct statvfs *buf);

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wshadow"
#endif

  int
  statvfs (const char* path, struct statvfs* buf);

// ----------------------------------------------------------------------------

#pragma GCC diagnostic pop

#ifdef __cplusplus
}
#endif

#endif /* defined(_POSIX_VERSION) */

#endif /* POSIX_IO_SYS_STATVFS_H_ */
