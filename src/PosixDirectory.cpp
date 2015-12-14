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
#include <cassert>

// ----------------------------------------------------------------------------

namespace os
{

  PosixDirectory::PosixDirectory (PosixFileSystem* fileSystem) :
      fFileSystem (fileSystem)
  {
    ;
  }

  PosixDirectory::~PosixDirectory ()
  {
    fFileSystem = nullptr;
  }

  // --------------------------------------------------------------------------

  int
  PosixDirectory::open (const char* dirname)
  {
    assert(fFileSystem != nullptr);
    errno = 0;

    // Execute the implementation specific code.
    return do_open (dirname);
  }

  struct dirent*
  PosixDirectory::read (void)
  {
    assert(fFileSystem != nullptr);
    errno = 0;

    // Execute the implementation specific code.
    return do_read ();
  }

  void
  PosixDirectory::rewind (void)
  {
    assert(fFileSystem != nullptr);
    errno = 0;

    // Execute the implementation specific code.
    do_rewind ();
  }

  int
  PosixDirectory::close (void)
  {
    assert(fFileSystem != nullptr);
    errno = 0;

    // Execute the implementation specific code.
    return do_close ();
  }

  // --------------------------------------------------------------------------
  // Default implementations; overwrite them with real code.

  // do_open() is not here because it is an abstract virtual to be
  // implemented by derived classes.

  struct dirent*
  PosixDirectory::do_read (void)
  {
    // Return end of directory.
    return nullptr;
  }

  void
  PosixDirectory::do_rewind (void)
  {
    // Ignore rewind.
    return;
  }

  int
  PosixDirectory::do_close (void)
  {
    // Ignore close, return ok.
    return 0;
  }

} /* namespace os */

// ----------------------------------------------------------------------------
