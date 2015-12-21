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
#include "posix-io/IO.h"
#include "posix-io/Socket.h"
#include <cerrno>
#include <cassert>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    std::size_t FileDescriptorsManager::sfSize;
    IO** FileDescriptorsManager::sfDescriptorsArray;

    // ------------------------------------------------------------------------

    FileDescriptorsManager::FileDescriptorsManager (std::size_t size)
    {
      assert(size > 3);

      sfSize = size;
      sfDescriptorsArray = new IO*[size];

      for (std::size_t i = 0; i < getSize (); ++i)
        {
          sfDescriptorsArray[i] = nullptr;
        }
    }

    FileDescriptorsManager::~FileDescriptorsManager ()
    {
      delete[] sfDescriptorsArray;
      sfSize = 0;
    }

    // ------------------------------------------------------------------------

    bool
    FileDescriptorsManager::isValid (int fildes)
    {
      if ((fildes < 0) || (((std::size_t) fildes) >= sfSize))
        {
          return false;
        }
      return true;
    }

    int
    FileDescriptorsManager::alloc (IO* io)
    {
      if (io->getFileDescriptor () >= 0)
        {
          // Already allocated
          errno = EBUSY;
          return -1;
        }

      // Reserve 0, 1, 2 (stdin, stdout, stderr)
      for (std::size_t i = 3; i < sfSize; ++i)
        {
          if (sfDescriptorsArray[i] == nullptr)
            {
              sfDescriptorsArray[i] = io;
              io->setFileDescriptor (i);
              return i;
            }
        }

      // Too many files open in system.
      errno = ENFILE;
      return -1;
    }

    int
    FileDescriptorsManager::free (int fildes)
    {
      if ((fildes < 0) || (((std::size_t) fildes) >= sfSize))
        {
          errno = EBADF;
          return -1;
        }

      sfDescriptorsArray[fildes]->clearFileDescriptor ();
      sfDescriptorsArray[fildes] = nullptr;
      return 0;
    }

    Socket*
    FileDescriptorsManager::getSocket (int fildes)
    {
      assert((fildes >= 0) && (((std::size_t ) fildes) < sfSize));
      auto* const io = sfDescriptorsArray[fildes];
      if (io->getType () != IO::Type::SOCKET)
        {
          return nullptr;
        }
      return reinterpret_cast<Socket*> (io);
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

