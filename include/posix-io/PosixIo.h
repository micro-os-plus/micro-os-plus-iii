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

#ifndef INCLUDE_POSIX_IO_POSIXIO_H_
#define INCLUDE_POSIX_IO_POSIXIO_H_

#include "posix-io/types.h"
#include <cstddef>

// ----------------------------------------------------------------------------

// Abstract class
class PosixIo
{
public:
  PosixIo ();

  virtual
  ~PosixIo ();

  // --------------------------------------------------------------------------

  int
  open (void);

  virtual ssize_t
  read (void *buf, size_t nbyte) = 0;

  virtual ssize_t
  write (const void *buf, size_t nbyte) = 0;

  virtual int
  ioctl (unsigned long request, ...) = 0;

  int
  close (void);

  virtual int
  closeImplementation (void) = 0;

  // --------------------------------------------------------------------------

  void
  setFileDescriptor (fileDescriptor_t fildes);
  void
  clearFileDescriptor (void);
  fileDescriptor_t
  getFileDescriptor (void);

  // --------------------------------------------------------------------------

protected:
  fileDescriptor_t fFileDescriptor;

  // --------------------------------------------------------------------------

};

// ----------------------------------------------------------------------------

#endif /* INCLUDE_POSIX_IO_POSIXIO_H_ */
