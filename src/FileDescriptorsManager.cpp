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
#include <cerrno>
#include <cassert>

namespace os
{

  // ----------------------------------------------------------------------------

  // Static table of all open file descriptors, similar to POSIX
  PosixIo* FileDescriptorsManager::openedFileDescriptors[OS_INTEGER_FILE_DESCRIPTORS_MANAGER_ARRAY_SIZE];

  // --------------------------------------------------------------------------

  FileDescriptorsManager::FileDescriptorsManager ()
  {
    // Superfluous, it should be cleared as the entire BSS.
    for (std::size_t i = 0; i < getSize (); ++i)
      {
        openedFileDescriptors[i] = nullptr;
      }
  }

  bool
  FileDescriptorsManager::checkFileDescriptor (int fildes)
  {
    if ((fildes < 0) || (((std::size_t) fildes) >= getSize ()))
      {
        return false;
      }
    return true;
  }

  PosixIo*
  FileDescriptorsManager::getPosixIo (int fildes)
  {
    assert((fildes > 0) && (((std::size_t ) fildes) < getSize ()));

    return openedFileDescriptors[fildes];
  }

  int
  FileDescriptorsManager::allocFileDescriptor (PosixIo* io)
  {
    if (io->getFileDescriptor () > 0)
      {
        // Already allocated
        errno = EBUSY;
        return -1;
      }

    // Reserve 0, 1, 2 (stdin, stdout, stderr)
    for (std::size_t i = 3; i < getSize (); ++i)
      {
        if (openedFileDescriptors[i] == nullptr)
          {
            openedFileDescriptors[i] = io;
            io->setFileDescriptor (i);
            return i;
          }
      }

    // Too many files open in system.
    errno = ENFILE;
    return -1;
  }

  int
  FileDescriptorsManager::freeFileDescriptor (int fildes)
  {
    if ((fildes < 0) || (((std::size_t) fildes) >= getSize ()))
      {
        errno = EBADF;
        return -1;
      }

    openedFileDescriptors[fildes]->clearFileDescriptor ();
    openedFileDescriptors[fildes] = nullptr;
    return 0;
  }

} // namespace os

// ----------------------------------------------------------------------------

