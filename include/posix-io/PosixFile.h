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

// ----------------------------------------------------------------------------

namespace os
{
  class PosixFileSystem;

  class PosixFile : public PosixIo
  {
  public:

    PosixFile (PosixFileSystem* fileSystem);

    ~PosixFile ();

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
