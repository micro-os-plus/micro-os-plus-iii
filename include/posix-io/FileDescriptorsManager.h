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

#ifndef INCLUDE_POSIX_IO_FILE_DESCRIPTORS_MANAGER_H_
#define INCLUDE_POSIX_IO_FILE_DESCRIPTORS_MANAGER_H_

#include "posix-io/types.h"
#include <cstddef>

// ----------------------------------------------------------------------------

class FileDescriptorsManager
{
public:
  FileDescriptorsManager ();

  static size_t
  getSize (void);

  static bool
  checkFileDescriptor (int fildes);

  static PosixIo*
  getPosixIo (int fildes);

  static fileDescriptor_t
  allocFileDescriptor (PosixIo* afile);

  static int
  freeFileDescriptor (fileDescriptor_t fildes);
};

// ----------------------------------------------------------------------------

#endif /* INCLUDE_POSIX_IO_FILE_DESCRIPTORS_MANAGER_H_ */
