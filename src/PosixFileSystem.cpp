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

#include "posix-io/PosixIo.h"
#include "posix-io/PosixFileSystem.h"
#include "posix-io/PosixDir.h"
#include "posix-io/PosixFileSystemsManager.h"
#include <cerrno>
#include <cassert>

// ----------------------------------------------------------------------------

namespace os
{

  // --------------------------------------------------------------------------

  PosixFileSystem::PosixFileSystem ()
  {
    fBlockDevice = nullptr;
  }

  PosixFileSystem::~PosixFileSystem ()
  {
    fBlockDevice = nullptr;
  }

  // --------------------------------------------------------------------------

  PosixIo*
  PosixFileSystem::open (const char *path, int oflag, std::va_list args)
  {
    if (fBlockDevice == nullptr)
      {
        errno = EBADF;
        return nullptr;
      }

    // Execute the implementation specific code.
    return do_open (path, oflag, args);
  }

  PosixDir*
  PosixFileSystem::opendir (const char *dirpath)
  {
    if (fBlockDevice == nullptr)
      {
        errno = EBADF;
        return nullptr;
      }

    // Execute the implementation specific code.
    return do_opendir (dirpath);

  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

  PosixIo*
  PosixFileSystem::do_open (const char *path, int oflag, std::va_list args)
  {
    // TODO: implement and return the object.
    return nullptr;
  }

  PosixDir*
  PosixFileSystem::do_opendir (const char *dirpath)
  {
    // TODO: implement and return the object.
    return nullptr;
  }

  // --------------------------------------------------------------------------

  int
  PosixFileSystem::chmod (const char* path, mode_t mode)
  {
    assert(fBlockDevice != nullptr);
    errno = 0;

    // Execute the implementation specific code.
    return do_chmod (path, mode);
  }

  int
  PosixFileSystem::stat (const char* path, struct stat* buf)
  {
    assert(fBlockDevice != nullptr);
    errno = 0;

    // Execute the implementation specific code.
    return do_stat (path, buf);
  }

  int
  PosixFileSystem::truncate (const char* path, off_t length)
  {
    assert(fBlockDevice != nullptr);
    errno = 0;

    // Execute the implementation specific code.
    return do_truncate (path, length);
  }

  int
  PosixFileSystem::rename (const char* existing, const char* _new)
  {
    assert(fBlockDevice != nullptr);
    errno = 0;

    // Execute the implementation specific code.
    return do_rename (existing, _new);
  }

  int
  PosixFileSystem::unlink (const char* path)
  {
    assert(fBlockDevice != nullptr);
    errno = 0;

    // Execute the implementation specific code.
    return do_unlink (path);
  }

  int
  PosixFileSystem::utime (const char* path, const struct utimbuf* times)
  {
    assert(fBlockDevice != nullptr);
    errno = 0;

    // Execute the implementation specific code.
    return do_utime (path, times);
  }

  // --------------------------------------------------------------------------

  int
  PosixFileSystem::mkdir (const char* path, mode_t mode)
  {
    const char* adjusted_path = path;
    os::PosixFileSystem* fs = os::PosixFileSystemsManager::identifyFileSystem (
        &adjusted_path);

    if (fs == nullptr)
      {
        errno = ENOENT;
        return -1;
      }

    assert(fs->fBlockDevice != nullptr);
    errno = 0;

    // Execute the implementation specific code.
    return fs->do_mkdir (path, mode);
  }

  int
  PosixFileSystem::rmdir (const char *path)
  {
    const char* adjusted_path = path;
    os::PosixFileSystem* fs = os::PosixFileSystemsManager::identifyFileSystem (
        &adjusted_path);

    if (fs == nullptr)
      {
        errno = ENOENT;
        return -1;
      }

    assert(fs->fBlockDevice != nullptr);
    errno = 0;

    // Execute the implementation specific code.
    return fs->do_rmdir (path);
  }

  void
  PosixFileSystem::sync (void)
  {
    errno = 0;

    // Enumerate all mounted file systems and sync them.
    for (size_t i = 0; i < PosixFileSystemsManager::getSize (); ++i)
      {
        PosixFileSystem* fs = PosixFileSystemsManager::getFileSystem (i);
        if (fs != nullptr)
          {
            fs->do_sync ();
          }
      }
  }

  // --------------------------------------------------------------------------

  const
  char*
  PosixFileSystem::adjustPath (const char* path)
  {
    return path;
  }

  // --------------------------------------------------------------------------

  int
  PosixFileSystem::do_chmod (const char* path, mode_t mode)
  {
    errno = ENOSYS; // Not implemented
    return -1;
  }

  int
  PosixFileSystem::do_stat (const char* path, struct stat* buf)
  {
    errno = ENOSYS; // Not implemented
    return -1;
  }

  int
  PosixFileSystem::do_truncate (const char* path, off_t length)
  {
    errno = ENOSYS; // Not implemented
    return -1;
  }

  int
  PosixFileSystem::do_rename (const char* existing, const char* _new)
  {
    errno = ENOSYS; // Not implemented
    return -1;
  }

  int
  PosixFileSystem::do_unlink (const char* path)
  {
    errno = ENOSYS; // Not implemented
    return -1;
  }

  int
  PosixFileSystem::do_utime (const char* path, const struct utimbuf* times)
  {
    errno = ENOSYS; // Not implemented
    return -1;
  }

  int
  PosixFileSystem::do_mkdir (const char* path, mode_t mode)
  {
    errno = ENOSYS; // Not implemented
    return -1;
  }

  int
  PosixFileSystem::do_rmdir (const char *path)
  {
    errno = ENOSYS; // Not implemented
    return -1;
  }

  void
  PosixFileSystem::do_sync (void)
  {
    errno = ENOSYS; // Not implemented
  }

  int
  PosixFileSystem::do_mount (unsigned int flags)
  {
    errno = ENOSYS; // Not implemented
    return -1;
  }

  int
  PosixFileSystem::do_unmount (unsigned int flags)
  {
    errno = ENOSYS; // Not implemented
    return -1;
  }

#pragma GCC diagnostic pop

} /* namespace os */

// ----------------------------------------------------------------------------
