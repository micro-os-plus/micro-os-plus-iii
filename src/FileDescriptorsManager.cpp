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

#include <posix-io/FileDescriptorsManager.h>
#include "posix-io/PosixIo.h"
#include <cerrno>
#include <cassert>

// ----------------------------------------------------------------------------

// This definition should be somewhere else, in a configuration header
#if !defined(OS_INTEGER_FILE_DESCRIPTORS_MANAGER_ARRAY_SIZE)
#define OS_INTEGER_FILE_DESCRIPTORS_MANAGER_ARRAY_SIZE   10
#endif

// ----------------------------------------------------------------------------

// Table of all open files, similar to POSIX
static PosixIo* openedFileDescriptors[OS_INTEGER_FILE_DESCRIPTORS_MANAGER_ARRAY_SIZE];

// ----------------------------------------------------------------------------

FileDescriptorsManager::FileDescriptorsManager ()
{
  for (size_t i = 0;
      i < (sizeof(openedFileDescriptors) / sizeof(openedFileDescriptors[0]));
      ++i)
    {
      openedFileDescriptors[i] = nullptr;
    }
}

size_t
FileDescriptorsManager::getSize (void)
{
  return sizeof(openedFileDescriptors) / sizeof(openedFileDescriptors[0]);
}

bool
FileDescriptorsManager::checkFileDescriptor (int fildes)
{
  if ((fildes < 0)
      || (((size_t) fildes)
          >= (sizeof(openedFileDescriptors) / sizeof(openedFileDescriptors[0]))))
    {
      return false;
    }
  return true;
}

PosixIo*
FileDescriptorsManager::getPosixIo (int fildes)
{
  assert(
      (fildes > 0)
          && (((size_t ) fildes)
              < (sizeof(openedFileDescriptors)
                  / sizeof(openedFileDescriptors[0]))));

  return openedFileDescriptors[fildes];
}

fileDescriptor_t
FileDescriptorsManager::allocFileDescriptor (PosixIo* afile)
{
  if (afile->getFileDescriptor () > 0)
    {
      // Already allocated
      return EBUSY;
    }

  for (size_t i = 3;
      i < (sizeof(openedFileDescriptors) / sizeof(openedFileDescriptors[0]));
      ++i)
    {
      if (openedFileDescriptors[i] == nullptr)
        {
          openedFileDescriptors[i] = afile;
          afile->setFileDescriptor (i);
          return i;
        }
    }

  // Too many files open in system.
  return ENFILE;
}

int
FileDescriptorsManager::freeFileDescriptor (int fildes)
{
  if ((fildes < 0)
      || (((size_t) fildes)
          >= (sizeof(openedFileDescriptors) / sizeof(openedFileDescriptors[0]))))
    {
      return EBADF;
    }
  openedFileDescriptors[fildes]->clearFileDescriptor ();
  openedFileDescriptors[fildes] = nullptr;
  return 0;
}

// ----------------------------------------------------------------------------

