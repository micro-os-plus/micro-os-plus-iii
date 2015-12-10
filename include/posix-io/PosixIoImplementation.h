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

#ifndef POSIX_IO_IMPLEMENTATION_H_
#define POSIX_IO_IMPLEMENTATION_H_

#include <cstdarg>

// ----------------------------------------------------------------------------

namespace os
{

  class PosixIoImplementation
  {
  public:

    PosixIoImplementation ();

    virtual
    ~PosixIoImplementation ();

    // ------------------------------------------------------------------------

    virtual int
    open (const char *path, int oflag, va_list args) = 0;

    virtual int
    close (void);

    virtual ssize_t
    read (void *buf, size_t nbyte);

    virtual ssize_t
    write (const void *buf, size_t nbyte);

    virtual int
    ioctl (int request, va_list args);

  };

} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* POSIX_IO_IMPLEMENTATION_H_ */
