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

#include "posix-io/Types.h"
#include <cstddef>
#include <cassert>

// ----------------------------------------------------------------------------

namespace os
{

  class PosixIo;

  class FileDescriptorsManager
  {
  public:

    FileDescriptorsManager (size_t size);

    ~FileDescriptorsManager ();

    // ------------------------------------------------------------------------

    static size_t
    getSize (void);

    static bool
    isValid (int fildes);

    static PosixIo*
    getObject (int fildes);

    static int
    alloc (PosixIo* io);

    static int
    free (fileDescriptor_t fildes);

    // ------------------------------------------------------------------------
  private:

    static std::size_t sfSize;

    static PosixIo** sfDescriptorsArray;
  };

  // --------------------------------------------------------------------------

  inline size_t
  FileDescriptorsManager::getSize (void)
  {
    return sfSize;
  }

  inline PosixIo*
  FileDescriptorsManager::getObject (int fildes)
  {
    assert((fildes >= 0) && (((std::size_t ) fildes) < sfSize));

    return sfDescriptorsArray[fildes];
  }

} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* INCLUDE_POSIX_IO_FILE_DESCRIPTORS_MANAGER_H_ */
