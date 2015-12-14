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

#ifndef POSIX_DIRECTORY_H_
#define POSIX_DIRECTORY_H_

#include "posix-io/PosixFileSystem.h"

#if defined(__ARM_EABI__)
#include "posix-io/dirent.h"
#else
#include <dirent.h>
#endif

namespace os
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  class PosixDirectory
  {
  public:

    PosixDirectory (PosixFileSystem& fileSystem);

    virtual
    ~PosixDirectory ();

    // int
    // opendir (const char* dirname);

    struct dirent *
    read (void);

    void
    rewind (void);

    int
    close (void);

    virtual struct dirent*
    doRead (void);

    virtual void
    doRewind (void);

    virtual int
    doClose (void);

    PosixFileSystem&
    getFileSystem (void);

  protected:
    PosixFileSystem& fFileSystem;
    struct dirent fDirEntry;
  };

#pragma GCC diagnostic pop

  inline PosixFileSystem&
  PosixDirectory::getFileSystem (void)
  {
    return fFileSystem;
  }
}

#endif /* POSIX_DIRECTORY_H_ */
