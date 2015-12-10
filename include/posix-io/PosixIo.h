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

// ----------------------------------------------------------------------------

namespace os
{
  class PosixIoImplementation;

  class PosixIo
  {
  public:

    PosixIo (PosixIoImplementation& impl);

    virtual
    ~PosixIo ();

    // ------------------------------------------------------------------------

    int
    open (const char *path, int oflag, ...);

    int
    vopen (const char *path, int oflag, va_list args);

    ssize_t
    read (void *buf, size_t nbyte);

    ssize_t
    write (const void *buf, size_t nbyte);

    int
    ioctl (int request, ...);

    int
    vioctl (int request, va_list args);

    int
    close (void);

    // ------------------------------------------------------------------------

    void
    setFileDescriptor (fileDescriptor_t fildes);

    void
    clearFileDescriptor (void);

    fileDescriptor_t
    getFileDescriptor (void) const;

    // ------------------------------------------------------------------------

  protected:

    PosixIoImplementation& fImpl;

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
