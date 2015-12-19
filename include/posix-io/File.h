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

#ifndef POSIX_IO_FILE_H_
#define POSIX_IO_FILE_H_

// ----------------------------------------------------------------------------

#include "posix-io/IO.h"

#if defined(__ARM_EABI__)
#include "posix-io/utime.h"
#else
#include <utime.h>
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class FileSystem;
    class Pool;

    // ------------------------------------------------------------------------

    class File : public IO
    {
      friend class FileSystem;
      friend class IO;

    public:

      File ();
      File (const File&) = delete;

      ~File ();

      // ----------------------------------------------------------------------
      // Support functions.

      FileSystem*
      getFileSystem (void) const;

    protected:

      void
      setFileSystem (FileSystem* fileSystem);

    protected:

      FileSystem* fFileSystem;
    };

    // ------------------------------------------------------------------------

    inline void
    File::setFileSystem (FileSystem* fileSystem)
    {
      fFileSystem = fileSystem;
    }

    inline FileSystem*
    File::getFileSystem (void) const
    {
      return fFileSystem;
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* POSIX_IO_FILE_H_ */
