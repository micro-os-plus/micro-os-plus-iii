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

#ifndef POSIX_FILE_SYSTEM_H_
#define POSIX_FILE_SYSTEM_H_

// ----------------------------------------------------------------------------

#include <cstdarg>
#include <sys/stat.h>
#include <utime.h>

// ----------------------------------------------------------------------------

namespace os
{
  // --------------------------------------------------------------------------

  class PosixIo;
  class PosixFile;
  class PosixDir;
  class BlockDevice;

  // --------------------------------------------------------------------------

  class PosixFileSystem
  {
    // ------------------------------------------------------------------------

    friend class PosixFile;
    friend class PosixFileSystemsManager;

    // ------------------------------------------------------------------------

  public:

    PosixFileSystem (BlockDevice* blockDevice);

    virtual
    ~PosixFileSystem ();

    // ------------------------------------------------------------------------

    PosixIo*
    open (const char *path, int oflag, std::va_list args);

    PosixDir*
    opendir (const char *dirpath);

    // ------------------------------------------------------------------------
    // ----- Non-io functions -----

    static int
    mkdir (const char* path, mode_t mode);

    static int
    rmdir (const char *path);

    static void
    sync (void);

    // ------------------------------------------------------------------------
  protected:

    virtual PosixIo*
    do_open (const char *path, int oflag, std::va_list args);

    virtual PosixDir*
    do_opendir (const char *dirpath);

    // ------------------------------------------------------------------------

    int
    chmod (const char* path, mode_t mode);

    int
    stat (const char* path, struct stat* buf);

    int
    truncate (const char* path, off_t length);

    int
    rename (const char* existing, const char* _new);

    int
    unlink (const char* path);

    int
    utime (const char* path, const struct utimbuf* times);

    // ------------------------------------------------------------------------
    // ----- Implementations -----

    virtual int
    do_chmod (const char* path, mode_t mode);

    virtual int
    do_stat (const char* path, struct stat* buf);

    virtual int
    do_truncate (const char* path, off_t length);

    virtual int
    do_rename (const char* existing, const char* _new);

    virtual int
    do_unlink (const char* path);

    virtual int
    do_utime (const char* path, const struct utimbuf* times);

    virtual int
    do_mkdir (const char* path, mode_t mode);

    virtual int
    do_rmdir (const char *path);

    virtual void
    do_sync (void);

    virtual int
    do_mount (int flags);

    virtual int
    do_unmount (int flags);

    // ------------------------------------------------------------------------
    // --- Support functions.

  public:

    const char*
    adjustPath (const char* path);

  protected:

    BlockDevice* fBlockDevice;
  };

} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* POSIX_FILE_SYSTEM_H_ */
