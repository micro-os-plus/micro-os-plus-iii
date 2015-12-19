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

#ifndef POSIX_IO_FILE_SYSTEM_H_
#define POSIX_IO_FILE_SYSTEM_H_

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
    // ----- Non-io functions -----

    int
    mkdir (const char* path, mode_t mode);

    int
    rmdir (const char* path);

    void
    sync (void);

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
      // --- Support functions.

      BlockDevice*
      getBlockDevice (void) const;

      Pool*
      getFilesPool (void) const;

      Pool*
      getDirsPool (void) const;

      // ----------------------------------------------------------------------
    protected:

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
      do_rmdir (const char* path);

      virtual void
      do_sync (void);

      virtual int
      do_mount (unsigned int flags);

      virtual int
      do_unmount (unsigned int flags);

      // ----------------------------------------------------------------------
      // --- Support functions.

      void
      setBlockDevice (BlockDevice* blockDevice);

    public:

      const char*
      adjustPath (const char* path);

    protected:

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

#endif /* POSIX_IO_FILE_SYSTEM_H_ */
