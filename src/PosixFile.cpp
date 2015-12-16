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

#include "posix-io/PosixFile.h"
#include "posix-io/PosixFileSystem.h"
#include "posix-io/PosixFileSystemsManager.h"
#include <cerrno>

// ----------------------------------------------------------------------------

namespace os
{

  // --------------------------------------------------------------------------

  PosixFile::PosixFile ()
  {
    fType = Type::FILE;
    fFileSystem = nullptr;
  }

  PosixFile::~PosixFile ()
  {
    fFileSystem = nullptr;
  }

  // ------------------------------------------------------------------------
  // Functions related to files, other than IO. The implementations is
  // specific to the FileSystem object.

  int
  PosixFile::chmod (const char* path, mode_t mode)
  {
    const char* adjusted_path = path;
    os::PosixFileSystem* fs = os::PosixFileSystemsManager::identifyFileSystem (
        &adjusted_path);

    if (fs == nullptr)
      {
        errno = ENOENT;
        return -1;
      }

    return fs->chmod (adjusted_path, mode);
  }

  int
  PosixFile::stat (const char* path, struct stat* buf)
  {
    const char* adjusted_path = path;
    os::PosixFileSystem* fs = os::PosixFileSystemsManager::identifyFileSystem (
        &adjusted_path);

    if (fs == nullptr)
      {
        errno = ENOENT;
        return -1;
      }

    return fs->stat (adjusted_path, buf);
  }

  int
  PosixFile::truncate (const char* path, off_t length)
  {
    const char* adjusted_path = path;
    os::PosixFileSystem* fs = os::PosixFileSystemsManager::identifyFileSystem (
        &adjusted_path);

    if (fs == nullptr)
      {
        errno = ENOENT;
        return -1;
      }

    return fs->truncate (adjusted_path, length);
  }

  int
  PosixFile::rename (const char* existing, const char* _new)
  {
    const char* adjusted_existing = existing;
    const char* adjusted_new = _new;
    os::PosixFileSystem* fs = os::PosixFileSystemsManager::identifyFileSystem (
        &adjusted_existing, &adjusted_new);

    if (fs == nullptr)
      {
        errno = ENOENT;
        return -1;
      }

    return fs->rename (adjusted_existing, adjusted_new);
  }

  int
  PosixFile::unlink (const char* path)
  {
    const char* adjusted_path = path;
    os::PosixFileSystem* fs = os::PosixFileSystemsManager::identifyFileSystem (
        &adjusted_path);

    if (fs == nullptr)
      {
        errno = ENOENT;
        return -1;
      }

    return fs->unlink (adjusted_path);
  }

  int
  PosixFile::utime (const char* path, const struct utimbuf* times)
  {
    const char* adjusted_path = path;
    os::PosixFileSystem* fs = os::PosixFileSystemsManager::identifyFileSystem (
        &adjusted_path);

    if (fs == nullptr)
      {
        errno = ENOENT;
        return -1;
      }

    return fs->utime (adjusted_path, times);
  }

} /* namespace os */

// ----------------------------------------------------------------------------
