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

#ifndef INCLUDE_POSIX_IO_POSIX_IO_H_
#define INCLUDE_POSIX_IO_POSIX_IO_H_

#include "posix-io/Types.h"
#include <cstddef>
#include <cstdarg>
// Needed for ssize_t
#include <sys/types.h>
#include <sys/stat.h>

// ----------------------------------------------------------------------------

namespace os
{
  class PosixIo
  {
  public:

    PosixIo ();

    virtual
    ~PosixIo ();

    // ------------------------------------------------------------------------

    // TODO: check if non-static open() is needed,
    // for use in C++ only apps.

    static PosixIo*
    open (const char *path, int oflag, ...);

    static PosixIo*
    vopen (const char *path, int oflag, std::va_list args);

    int
    close (void);

    ssize_t
    read (void *buf, std::size_t nbyte);

    ssize_t
    write (const void *buf, std::size_t nbyte);

    int
    ioctl (int request, ...);

    int
    vioctl (int request, std::va_list args);

    off_t
    lseek (off_t offset, int whence);

    int
    isatty (void);

    int
    fcntl (int cmd, ...);

    int
    vfcntl (int cmd, std::va_list args);

    int
    fstat (struct stat* buf);

    int
    ftruncate (off_t length);

    int
    fsync (void);

    // ------------------------------------------------------------------------

    void
    setFileDescriptor (fileDescriptor_t fildes);

    void
    clearFileDescriptor (void);

    fileDescriptor_t
    getFileDescriptor (void) const;

    // ------------------------------------------------------------------------

    // Implementations

    /**
     * return 0 if success or -1 & errno
     */
    virtual int
    doOpen (const char *path, int oflag, std::va_list args) = 0;

    virtual int
    doClose (void);

    virtual ssize_t
    doRead (void *buf, std::size_t nbyte);

    virtual ssize_t
    doWrite (const void *buf, std::size_t nbyte);

    virtual int
    doIoctl (int request, std::va_list args);

    virtual off_t
    doLseek (off_t offset, int whence);

    virtual int
    doIsatty (void);

    virtual int
    doFcntl (int cmd, va_list args);

    virtual int
    doFstat (struct stat* buf);

    virtual int
    doFtruncate (off_t length);

    virtual int
    doFsync (void);

    // ------------------------------------------------------------------------

  protected:

    fileDescriptor_t fFileDescriptor;
  };

  // --------------------------------------------------------------------------

  inline void
  PosixIo::setFileDescriptor (fileDescriptor_t fildes)
  {
    fFileDescriptor = fildes;
  }

  inline void
  PosixIo::clearFileDescriptor (void)
  {
    fFileDescriptor = noFileDescriptor;
  }

  inline fileDescriptor_t
  PosixIo::getFileDescriptor (void) const
  {
    return fFileDescriptor;
  }

} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* INCLUDE_POSIX_IO_POSIX_IO_H_ */
