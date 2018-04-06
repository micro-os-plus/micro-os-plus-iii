/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2018 Liviu Ionescu.
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
#pragma GCC diagnostic ignored "-Wshadow"

  int
  statvfs (const char* path, struct statvfs* buf);

// ----------------------------------------------------------------------------

#pragma GCC diagnostic pop

#ifdef __cplusplus
}
#endif

#endif /* defined(_POSIX_VERSION) */

#endif /* POSIX_IO_SYS_STATVFS_H_ */
