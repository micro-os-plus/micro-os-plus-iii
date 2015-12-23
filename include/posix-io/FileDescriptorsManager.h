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

#ifndef POSIX_IO_FILE_DESCRIPTORS_MANAGER_H_
#define POSIX_IO_FILE_DESCRIPTORS_MANAGER_H_

// ----------------------------------------------------------------------------

#include "posix-io/types.h"
#include <cstddef>
#include <cassert>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {

    // ------------------------------------------------------------------------

    class IO;
    class Socket;

    // ------------------------------------------------------------------------

    class FileDescriptorsManager
    {
    public:

      FileDescriptorsManager (std::size_t size);
      FileDescriptorsManager (const FileDescriptorsManager&) = delete;

      ~FileDescriptorsManager ();

      // ----------------------------------------------------------------------

      static size_t
      getSize (void);

      static bool
      isValid (int fildes);

      static IO*
      getIo (int fildes);

      static Socket*
      getSocket (int fildes);

      static int
      alloc (IO* io);

      static int
      free (fileDescriptor_t fildes);

      // ----------------------------------------------------------------------
    private:

      static std::size_t sfSize;

      static IO** sfDescriptorsArray;
    };

    // ------------------------------------------------------------------------

    inline size_t
    FileDescriptorsManager::getSize (void)
    {
      return sfSize;
    }

    inline IO*
    FileDescriptorsManager::getIo (int fildes)
    {
#if !defined(__ARM_EABI__)
      assert((fildes >= 0) && (((std::size_t ) fildes) < sfSize));
#endif

      return sfDescriptorsArray[fildes];
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* POSIX_IO_FILE_DESCRIPTORS_MANAGER_H_ */
