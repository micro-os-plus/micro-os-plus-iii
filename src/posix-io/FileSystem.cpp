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

#include "posix-io/IO.h"
#include "posix-io/FileSystem.h"
#include "posix-io/File.h"
#include "posix-io/Directory.h"
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

    int
    mkdir (const char* path, mode_t mode)
    {
      if (path == nullptr)
        {
          errno = EFAULT;
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          return -1;
        }

      auto adjusted_path = path;
      auto* const fs = os::posix::MountManager::identifyFileSystem (
          &adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          return -1;
        }

      assert(fs->getBlockDevice () != nullptr);
      errno = 0;

      // Execute the implementation specific code.
      return fs->do_mkdir (adjusted_path, mode);
    }

    int
    rmdir (const char* path)
    {
      if (path == nullptr)
        {
          errno = EFAULT;
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          return -1;
        }

      auto adjusted_path = path;
      auto* const fs = os::posix::MountManager::identifyFileSystem (
          &adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          return -1;
        }

      assert(fs->getBlockDevice () != nullptr);
      errno = 0;

      // Execute the implementation specific code.
      return fs->do_rmdir (adjusted_path);
    }

    void
    sync (void)
    {
      errno = 0;

      // Enumerate all mounted file systems and sync them.
      for (std::size_t i = 0; i < MountManager::getSize (); ++i)
        {
          auto fs = MountManager::getFileSystem (i);
          if (fs != nullptr)
            {
              fs->do_sync ();
            }
        }
    }

    // ------------------------------------------------------------------------
    // Functions related to files, other than IO. The implementations is
    // specific to each FileSystem.

    int
    chmod (const char* path, mode_t mode)
    {
      if (path == nullptr)
        {
          errno = EFAULT;
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          return -1;
        }

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
    stat (const char* path, struct stat* buf)
    {
      if ((path == nullptr) || (buf == nullptr))
        {
          errno = EFAULT;
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          return -1;
        }

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
    truncate (const char* path, off_t length)
    {
      if (path == nullptr)
        {
          errno = EFAULT;
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          return -1;
        }

      const char* adjusted_path = path;
      auto* const fs = os::posix::MountManager::identifyFileSystem (
          &adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          return -1;
        }

      if (length < 0)
        {
          errno = EINVAL;
          return -1;
        }

      return fs->truncate (adjusted_path, length);
    }

    int
    rename (const char* existing, const char* _new)
    {
      if ((existing == nullptr) || (_new == nullptr))
        {
          errno = EFAULT;
          return -1;
        }

      if ((*existing == '\0') || (*_new == '\0'))
        {
          errno = ENOENT;
          return -1;
        }

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
    unlink (const char* path)
    {
      if (path == nullptr)
        {
          errno = EFAULT;
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          return -1;
        }

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
    utime (const char* path, const struct utimbuf* times)
    {
      if ((path == nullptr) || (times == nullptr))
        {
          errno = EFAULT;
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          return -1;
        }

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

    // ------------------------------------------------------------------------

    FileSystem::FileSystem (Pool* filesPool, Pool* dirsPool)
    {
      fFilesPool = filesPool;
      fDirsPool = dirsPool;
      fBlockDevice = nullptr;
    }

    FileSystem::~FileSystem ()
    {
      fBlockDevice = nullptr;
    }

    // ------------------------------------------------------------------------

    IO*
    FileSystem::open (const char* path, int oflag, std::va_list args)
    {
      if (fBlockDevice == nullptr)
        {
          errno = EBADF;
          return nullptr;
        }

      // Get a File object from the pool.
      auto* const file = static_cast<File*> (fFilesPool->aquire ());

      // Associate the file with this file system (used, for example,
      // to reach the pools at close).
      file->setFileSystem (this);

      // Execute the file specific implementation code.
      file->do_vopen (path, oflag, args);

      return file;
    }

    Directory*
    FileSystem::opendir (const char* dirpath)
    {
      if (fBlockDevice == nullptr)
        {
          errno = EBADF;
          return nullptr;
        }

      // Get a Directory object from the pool.
      auto* const dir = static_cast<Directory*> (fDirsPool->aquire ());

      // Associate the dir with this file system (used, for example,
      // to reach the pools at close).
      dir->setFileSystem (this);

      // Execute the dir specific implementation code.
      dir->do_vopen (dirpath);

      return dir;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    int
    FileSystem::chmod (const char* path, mode_t mode)
    {
      assert(fBlockDevice != nullptr);
      errno = 0;

      // Execute the implementation specific code.
      return do_chmod (path, mode);
    }

    int
    FileSystem::stat (const char* path, struct stat* buf)
    {
      assert(fBlockDevice != nullptr);
      errno = 0;

      // Execute the implementation specific code.
      return do_stat (path, buf);
    }

    int
    FileSystem::truncate (const char* path, off_t length)
    {
      assert(fBlockDevice != nullptr);
      errno = 0;

      // Execute the implementation specific code.
      return do_truncate (path, length);
    }

    int
    FileSystem::rename (const char* existing, const char* _new)
    {
      assert(fBlockDevice != nullptr);
      errno = 0;

      // Execute the implementation specific code.
      return do_rename (existing, _new);
    }

    int
    FileSystem::unlink (const char* path)
    {
      assert(fBlockDevice != nullptr);
      errno = 0;

      // Execute the implementation specific code.
      return do_unlink (path);
    }

    int
    FileSystem::utime (const char* path, const struct utimbuf* times)
    {
      assert(fBlockDevice != nullptr);
      errno = 0;

      // Execute the implementation specific code.
      return do_utime (path, times);
    }

    // ------------------------------------------------------------------------

    const
    char*
    FileSystem::adjustPath (const char* path)
    {
      return path;
    }

    // ------------------------------------------------------------------------

    int
    FileSystem::do_chmod (const char* path, mode_t mode)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    FileSystem::do_stat (const char* path, struct stat* buf)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    FileSystem::do_truncate (const char* path, off_t length)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    FileSystem::do_rename (const char* existing, const char* _new)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    FileSystem::do_unlink (const char* path)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    FileSystem::do_utime (const char* path, const struct utimbuf* times)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    FileSystem::do_mkdir (const char* path, mode_t mode)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    FileSystem::do_rmdir (const char* path)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    void
    FileSystem::do_sync (void)
    {
      errno = ENOSYS; // Not implemented
    }

    int
    FileSystem::do_mount (unsigned int flags)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    FileSystem::do_unmount (unsigned int flags)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

#pragma GCC diagnostic pop

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
