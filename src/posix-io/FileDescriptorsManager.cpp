/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <cmsis-plus/posix-io/FileDescriptorsManager.h>
#include <cmsis-plus/posix-io/IO.h>
#include <cmsis-plus/posix-io/Socket.h>

#include <cerrno>
#include <cassert>
#include <cstddef>

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

    IO*
    FileDescriptorsManager::getIo (int fildes)
    {
      // Check if valid descriptor or buffer not yet initialised
      if ((fildes < 0) || (static_cast<std::size_t> (fildes) >= sfSize)
          || (sfDescriptorsArray == nullptr))
        {
          return nullptr;
        }
      return sfDescriptorsArray[fildes];
    }

    bool
    FileDescriptorsManager::isValid (int fildes)
    {
      if ((fildes < 0) || (static_cast<std::size_t> (fildes) >= sfSize))
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
              io->setFileDescriptor (static_cast<int>(i));
              return static_cast<int>(i);
            }
        }

      // Too many files open in system.
      errno = ENFILE;
      return -1;
    }

    int
    FileDescriptorsManager::assign (fileDescriptor_t fildes, IO* io)
    {
      if ((fildes < 0) || (static_cast<std::size_t> (fildes) >= sfSize))
        {
          errno = EBADF;
          return -1;
        }

      if (io->getFileDescriptor () >= 0)
        {
          // Already allocated
          errno = EBUSY;
          return -1;
        }

      sfDescriptorsArray[fildes] = io;
      io->setFileDescriptor (fildes);
      return fildes;
    }

    int
    FileDescriptorsManager::free (int fildes)
    {
      if ((fildes < 0) || (static_cast<std::size_t> (fildes) >= sfSize))
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
      assert((fildes >= 0) && (static_cast<std::size_t> (fildes) < sfSize));
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

