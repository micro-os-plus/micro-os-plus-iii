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

#include "posix-io/Directory.h"
#include "posix-io/FileSystem.h"
#include "posix-io/MountManager.h"
#include "posix-io/Pool.h"

#include <cerrno>
#include <cassert>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    Directory*
    opendir (const char* dirname)
    {
      if (dirname == nullptr)
        {
          errno = EFAULT;
          return nullptr;
        }

      if (*dirname == '\0')
        {
          errno = ENOENT;
          return nullptr;
        }

      errno = 0;

      auto adjusted_dirname = dirname;
      auto* const fs = os::posix::MountManager::identifyFileSystem (
          &adjusted_dirname);

      // The manager will return null if there are no file systems
      // registered, no need to check this condition separately.
      if (fs == nullptr)
        {
          errno = EBADF;
          return nullptr;
        }

      // Use the file system implementation to open the directory, using
      // the adjusted path (mount point prefix removed).
      return fs->opendir (adjusted_dirname);
    }

    // ------------------------------------------------------------------------

    Directory::Directory (void)
    {
      fFileSystem = nullptr;
    }

    Directory::~Directory ()
    {
      fFileSystem = nullptr;
    }

    // ------------------------------------------------------------------------

    struct dirent*
    Directory::read (void)
    {
      assert(fFileSystem != nullptr);
      errno = 0;

      // Execute the implementation specific code.
      return do_read ();
    }

    void
    Directory::rewind (void)
    {
      assert(fFileSystem != nullptr);
      errno = 0;

      // Execute the implementation specific code.
      do_rewind ();
    }

    int
    Directory::close (void)
    {
      assert(fFileSystem != nullptr);
      errno = 0;

      // Execute the implementation specific code.
      int ret = do_close ();
      auto* const pool = fFileSystem->getDirsPool ();
      if (pool != nullptr)
        {
          pool->release (this);
        }
      return ret;
    }

    // ------------------------------------------------------------------------
    // Default implementations; overwrite them with real code.

    // do_vopen() is not here because it is an abstract virtual to be
    // implemented by derived classes.

    struct dirent*
    Directory::do_read (void)
    {
      // Return end of directory.
      return nullptr;
    }

    void
    Directory::do_rewind (void)
    {
      // Ignore rewind.
      return;
    }

    int
    Directory::do_close (void)
    {
      // Ignore close, return ok.
      return 0;
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
