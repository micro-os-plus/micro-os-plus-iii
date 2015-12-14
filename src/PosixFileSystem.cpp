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
#include "posix-io/PosixDirectory.h"
#include <cerrno>

// ----------------------------------------------------------------------------

namespace os
{

  PosixFileSystem::PosixFileSystem ()
  {
    // TODO Auto-generated constructor stub

  }

  PosixFileSystem::~PosixFileSystem ()
  {
    // TODO Auto-generated destructor stub
  }

  // --------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

  // TODO: Should we allow C++ to open files directly using
  // the derived objects, or ask everything to go via PosixIo::open()?
  // If so, we must allocate a file descriptor here, not in PosixIo?

  PosixIo*
  PosixFileSystem::open (const char *path, int oflag, std::va_list args)
  {
    // TODO: implement
    return nullptr;
  }

  PosixDirectory*
  PosixFileSystem::opendir (const char *dirpath)
  {
    // TODO: implement
    return nullptr;
  }

  // --------------------------------------------------------------------------

  int
  PosixFileSystem::chmod (const char* path, mode_t mode)
  {
    errno = 0;

    // Execute the implementation specific code.
    return do_chmod (path, mode);
  }

  int
  PosixFileSystem::stat (const char* path, struct stat* buf)
  {
    errno = 0;

    // Execute the implementation specific code.
    return do_stat (path, buf);
  }

  int
  PosixFileSystem::truncate (const char* path, off_t length)
  {
    errno = 0;

    // Execute the implementation specific code.
    return do_truncate (path, length);
  }

  int
  PosixFileSystem::rename (const char* existing, const char* _new)
  {
    errno = 0;

    // Execute the implementation specific code.
    return do_rename (existing, _new);
  }

  int
  PosixFileSystem::unlink (const char* path)
  {
    errno = 0;

    // Execute the implementation specific code.
    return do_unlink (path);
  }

  int
  PosixFileSystem::utime (const char* path, const struct utimbuf* times)
  {
    errno = 0;

    // Execute the implementation specific code.
    return do_utime (path, times);
  }

  int
  PosixFileSystem::mkdir (const char* path, mode_t mode)
  {
    errno = 0;

    // Execute the implementation specific code.
    return do_mkdir (path, mode);
  }

  int
  PosixFileSystem::rmdir (const char *path)
  {
    errno = 0;

    // Execute the implementation specific code.
    return do_rmdir (path);
  }

  // ---
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

#pragma GCC diagnostic pop

} /* namespace os */

// ----------------------------------------------------------------------------
