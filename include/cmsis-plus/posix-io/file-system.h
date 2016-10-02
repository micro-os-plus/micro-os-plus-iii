/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef CMSIS_PLUS_POSIX_IO_FILE_SYSTEM_H_
#define CMSIS_PLUS_POSIX_IO_FILE_SYSTEM_H_

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#include <cstdarg>
#include <sys/stat.h>
#include <utime.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class io;
    class file;
    class directory;
    class device_block;
    class pool;

    // ----------------------------------------------------------------------
    // ----- Non-io, global file system functions -----

    int
    mkdir (const char* path, mode_t mode);

    int
    rmdir (const char* path);

    void
    sync (void);

    // ----------------------------------------------------------------------
    // ----- Non-io, file functions -----

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

    class file_system
    {
      // ----------------------------------------------------------------------

      friend class file;
      friend class directory;
      friend class mount_manager;
      friend class io;

      friend int
      mkdir (const char* path, mode_t mode);

      friend int
      rmdir (const char* path);

      friend void
      sync (void);

      friend int
      chmod (const char* path, mode_t mode);

      friend int
      stat (const char* path, struct stat* buf);

      friend int
      truncate (const char* path, off_t length);

      friend int
      rename (const char* existing, const char* _new);

      friend int
      unlink (const char* path);

      friend int
      utime (const char* path, const struct utimbuf* times);

      // ----------------------------------------------------------------------

    public:

      file_system (pool* filesPool, pool* dirsPool);
      file_system (const file_system&) = delete;

      virtual
      ~file_system ();

      // ----------------------------------------------------------------------

      io*
      open (const char* path, int oflag, std::va_list args);

      directory*
      opendir (const char* dirpath);

      // ----------------------------------------------------------------------
      // Support functions.

      device_block*
      getBlockDevice (void) const;

      pool*
      getFilesPool (void) const;

      pool*
      getDirsPool (void) const;

    protected:

      // ----------------------------------------------------------------------
      // Called from namespace friend functions.

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

      // ----------------------------------------------------------------------
      // Implementations.

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
      do_rmdir (const char* path);

      virtual void
      do_sync (void);

      virtual int
      do_mount (unsigned int flags);

      virtual int
      do_unmount (unsigned int flags);

      // ----------------------------------------------------------------------
      // Support functions.

      void
      setBlockDevice (device_block* blockDevice);

    public:

      const char*
      adjustPath (const char* path);

    private:

      pool* fFilesPool;
      pool* fDirsPool;

      device_block* fBlockDevice;
    };

    inline pool*
    file_system::getFilesPool (void) const
    {
      return fFilesPool;
    }

    inline pool*
    file_system::getDirsPool (void) const
    {
      return fDirsPool;
    }

    inline void
    file_system::setBlockDevice (device_block* blockDevice)
    {
      fBlockDevice = blockDevice;
    }

    inline device_block*
    file_system::getBlockDevice (void) const
    {
      return fBlockDevice;
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_FILE_SYSTEM_H_ */
