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

#include "posix-io/PosixIoImplementation.h"

#include <cerrno>

namespace os
{

  // --------------------------------------------------------------------------

  PosixIoImplementation::PosixIoImplementation ()
  {
    ;
  }

  PosixIoImplementation::~PosixIoImplementation ()
  {
    ;
  }

  // --------------------------------------------------------------------------

  // open() is not here because it is virtual,
  // it must be implemented by derived classes.

  int
  PosixIoImplementation::close (void)
  {
    return 0; // Always return success
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

  ssize_t
  PosixIoImplementation::read (void *buf, size_t nbyte)
  {
    errno = ENOSYS; // Not implemented
    return -1;
  }

  ssize_t
  PosixIoImplementation::write (const void *buf, size_t nbyte)
  {
    errno = ENOSYS; // Not implemented
    return -1;
  }

  int
  PosixIoImplementation::ioctl (int request, va_list args)
  {
    errno = ENOSYS; // Not implemented
    return -1;
  }

#pragma GCC diagnostic pop

} /* namespace os */

// ----------------------------------------------------------------------------
