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
#include <cerrno>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
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

    File*
    File::open (const char* path, int oflag, ...)
    {
      // Forward to the variadic version of the function.
      std::va_list args;
      va_start(args, oflag);
      File* const ret = vopen (path, oflag, args);
      va_end(args);

      return ret;
    }

    // ------------------------------------------------------------------------
    // Functions related to files, other than IO. The implementations is
    // specific to the FileSystem object.

    int
    File::chmod (const char* path, mode_t mode)
    {
      const char* adjusted_path = path;
      auto* const fs = os::posix::MountManager::identifyFileSystem (
          &adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          return -1;
        }

      return fs->chmod (adjusted_path, mode);
    }

    int
    File::stat (const char* path, struct stat* buf)
    {
      const char* adjusted_path = path;
      auto* const fs = os::posix::MountManager::identifyFileSystem (
          &adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          return -1;
        }

      return fs->stat (adjusted_path, buf);
    }

    int
    File::truncate (const char* path, off_t length)
    {
      const char* adjusted_path = path;
      auto* const fs = os::posix::MountManager::identifyFileSystem (
          &adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          return -1;
        }

      return fs->truncate (adjusted_path, length);
    }

    int
    File::rename (const char* existing, const char* _new)
    {
      auto adjusted_existing = existing;
      auto adjusted_new = _new;
      auto* const fs = os::posix::MountManager::identifyFileSystem (
          &adjusted_existing, &adjusted_new);

      if (fs == nullptr)
        {
          errno = ENOENT;
          return -1;
        }

      return fs->rename (adjusted_existing, adjusted_new);
    }

    int
    File::unlink (const char* path)
    {
      auto adjusted_path = path;
      auto* const fs = os::posix::MountManager::identifyFileSystem (
          &adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          return -1;
        }

      return fs->unlink (adjusted_path);
    }

    int
    File::utime (const char* path, const struct utimbuf* times)
    {
      auto adjusted_path = path;
      auto* const fs = os::posix::MountManager::identifyFileSystem (
          &adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          return -1;
        }

      return fs->utime (adjusted_path, times);
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
