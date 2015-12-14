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

#include "posix-io/PosixDirectory.h"
#include "posix-io/PosixFileSystem.h"
#include <cerrno>

namespace os
{

  PosixDirectory::PosixDirectory (PosixFileSystem& fileSystem) :
      fFileSystem (fileSystem)
  {
    ;
  }

  PosixDirectory::~PosixDirectory ()
  {
    ;
  }

  struct dirent *
  PosixDirectory::read (void)
  {
    errno = 0;

    return doRead ();
  }

  void
  PosixDirectory::rewind (void)
  {
    errno = 0;

    doRewind ();
  }

  int
  PosixDirectory::close (void)
  {
    errno = 0;

    return doClose ();
  }

  // Default implementations; overwrite them with real code.

  struct dirent*
  PosixDirectory::doRead (void)
  {
    // Return end of directory.
    return nullptr;
  }

  void
  PosixDirectory::doRewind (void)
  {
    // Ignore.
    return;
  }

  int
  PosixDirectory::doClose (void)
  {
    // Ignore, return ok.
    return 0;
  }

}
