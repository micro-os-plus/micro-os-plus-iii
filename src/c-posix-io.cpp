/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
 *
 * µOS++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 3.
 *
 * µOS++ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "posix-io/FileDescriptorsManager.h"
#include "posix-io/PosixIo.h"
#include <cstdarg>
#include <cerrno>

// ----------------------------------------------------------------------------

#if defined(OS_REDEFINE_POSIX_IO)

#define open __posix_open
#define read __posix_read
#define write __posix_write
#define ioctl __posix_ioctl
#define close __posix_close

#endif

// ----------------------------------------------------------------------------

extern "C"
{
  // Define the OS version of the standard POSIX functions.
  // Names are prefixed to avoid clashes in tests.

  int
  __posix_open (const char *path, int oflag, ...);

  ssize_t
  __posix_read (int fildes, void *buf, size_t nbyte);

  ssize_t
  __posix_write (int fildes, const void *buf, size_t nbyte);

  int
  __posix_ioctl (int fildes, unsigned long request, ...);

  int
  __posix_close (int fildes);
}

// ----------------------------------------------------------------------------

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

int
__posix_open (const char *path, int oflag, ...)
{
  // TODO: this is tricky, it should enumerate all implementations
  // and open the one that it matches.

  return 0;
}

#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif

ssize_t
__posix_read (int fildes, void *buf, size_t nbyte)
{
  PosixIo* io = FileDescriptorsManager::getPosixIo (fildes);
  if (io == nullptr)
    {
      return EBADF;
    }
  return io->read (buf, nbyte);
}

ssize_t
__posix_write (int fildes, const void *buf, size_t nbyte)
{
  PosixIo* io = FileDescriptorsManager::getPosixIo (fildes);
  if (io == nullptr)
    {
      return EBADF;
    }
  return io->write (buf, nbyte);
}

int
__posix_ioctl (int fildes, unsigned long request, ...)
{
  PosixIo* io = FileDescriptorsManager::getPosixIo (fildes);
  if (io == nullptr)
    {
      return EBADF;
    }

  va_list args;
  va_start(args, request);
  int ret;
  ret = io->ioctl (request, args);
  va_end(args);

  return ret;
}

int
__posix_close (int fildes)
{
  PosixIo* io = FileDescriptorsManager::getPosixIo (fildes);
  if (io == nullptr)
    {
      return EBADF;
    }
  return io->close ();
}

// ----------------------------------------------------------------------------

