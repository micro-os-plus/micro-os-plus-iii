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

#ifndef POSIX_IO_POSIX_FILE_H_
#define POSIX_IO_POSIX_FILE_H_

// ----------------------------------------------------------------------------

#include "posix-io/PosixIo.h"

#if defined(__ARM_EABI__)
#include "posix-io/utime.h"
#else
#include <utime.h>
#endif

// ----------------------------------------------------------------------------

namespace os
{
  // --------------------------------------------------------------------------

  class PosixFileSystem;
  class PosixPool;

  // --------------------------------------------------------------------------

  class PosixFile : public PosixIo
  {
  public:

    PosixFile ();

    ~PosixFile ();

    // ------------------------------------------------------------------------
    // There is a small catch here, these functions are implemented in the
    // PosixFileSystem object, but since the path refers to a file, it seems
    // more natural to address them within PosixFile.

    static int
    chmod (const char* path, mode_t mode);

    static int
    stat (const char* path, struct stat* buf);

    static int
    truncate (const char* path, off_t length);

    static int
    rename (const char* existing, const char* _new);

    static int
    unlink (const char* path);

    static int
    utime (const char* path, const struct utimbuf* times);

    // ------------------------------------------------------------------------

    void
    setFileSystem (PosixFileSystem* fileSystem);

    PosixFileSystem*
    getFileSystem (void);

  protected:

    PosixFileSystem* fFileSystem;
  };

  // --------------------------------------------------------------------------

  inline void
  PosixFile::setFileSystem (PosixFileSystem* fileSystem)
  {
    fFileSystem = fileSystem;
  }

  inline PosixFileSystem*
  PosixFile::getFileSystem (void)
  {
    return fFileSystem;
  }

} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* POSIX_IO_POSIX_FILE_H_ */
