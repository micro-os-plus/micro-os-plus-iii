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

#include "posix-io/File.h"
#include "posix-io/FileSystem.h"
#include "posix-io/MountManager.h"
#include "posix-io/Pool.h"

#include <cerrno>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    File*
    File::open (const char* path, int oflag, ...)
    {
      // Forward to the variadic version of the function.
      std::va_list args;
      va_start(args, oflag);
      auto* const ret = vopen (path, oflag, args);
      va_end(args);

      return ret;
    }

    // ------------------------------------------------------------------------

    File::File ()
    {
      fType = Type::FILE;
      fFileSystem = nullptr;
    }

    File::~File ()
    {
      fFileSystem = nullptr;
    }

    // ------------------------------------------------------------------------

    void
    File::doRelease (void)
    {
      // Files is free, return it to the pool.
      auto fs = getFileSystem ();
      if (fs != nullptr)
        {
          auto pool = fs->getFilesPool ();
          if (pool != nullptr)
            {
              pool->release (this);
            }
          setFileSystem (nullptr);
        }
    }

    // ------------------------------------------------------------------------

    off_t
    File::lseek (off_t offset, int whence)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_lseek (offset, whence);
    }

    int
    File::ftruncate (off_t length)
    {
      if (length < 0)
        {
          errno = EINVAL;
          return -1;
        }

      errno = 0;

      // Execute the implementation specific code.
      return do_ftruncate (length);
    }

    int
    File::fsync (void)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_fsync ();
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    off_t
    File::do_lseek (off_t offset, int whence)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    File::do_ftruncate (off_t length)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

#pragma GCC diagnostic pop

    int
    File::do_fsync (void)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
