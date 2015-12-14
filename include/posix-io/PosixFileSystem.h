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

#include <cstdarg>
#include <sys/stat.h>
#include <utime.h>

namespace os
{
  class PosixIo;
  class PosixDirectory;

  class PosixFileSystem
  {
  public:

    PosixFileSystem ();

    virtual
    ~PosixFileSystem ();

    PosixIo*
    open (const char *path, int oflag, std::va_list args);

    PosixDirectory*
    opendir(const char *dirpath);

    // --- File non-io functions.

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

    int
    mkdir (const char* path, mode_t mode);

    int
    rmdir (const char *path);

    // --- Support functions.

    const char*
    adjustPath (const char* path);

  };

} /* namespace os */

#endif /* POSIX_FILE_SYSTEM_H_ */
