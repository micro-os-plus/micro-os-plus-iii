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

    class IO;
    class File;
    class Directory;
    class BlockDevice;
    class Pool;

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

    class FileSystem
    {
      // ----------------------------------------------------------------------

      friend class File;
      friend class Directory;
      friend class MountManager;
      friend class IO;

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

      FileSystem (Pool* filesPool, Pool* dirsPool);
      FileSystem (const FileSystem&) = delete;

      virtual
      ~FileSystem ();

      // ----------------------------------------------------------------------

      IO*
      open (const char* path, int oflag, std::va_list args);

      Directory*
      opendir (const char* dirpath);

      // ----------------------------------------------------------------------
      // Support functions.

      BlockDevice*
      getBlockDevice (void) const;

      Pool*
      getFilesPool (void) const;

      Pool*
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
      setBlockDevice (BlockDevice* blockDevice);

    public:

      const char*
      adjustPath (const char* path);

    private:

      Pool* fFilesPool;
      Pool* fDirsPool;

      BlockDevice* fBlockDevice;
    };

    inline Pool*
    FileSystem::getFilesPool (void) const
    {
      return fFilesPool;
    }

    inline Pool*
    FileSystem::getDirsPool (void) const
    {
      return fDirsPool;
    }

    inline void
    FileSystem::setBlockDevice (BlockDevice* blockDevice)
    {
      fBlockDevice = blockDevice;
    }

    inline BlockDevice*
    FileSystem::getBlockDevice (void) const
    {
      return fBlockDevice;
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_FILE_SYSTEM_H_ */
