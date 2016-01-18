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

#ifndef POSIX_IO_FILE_H_
#define POSIX_IO_FILE_H_

// ----------------------------------------------------------------------------

#include "posix-io/IO.h"

#include "posix/utime.h"

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class FileSystem;
    class Pool;

    // ------------------------------------------------------------------------

    class File : public IO
    {
      friend class FileSystem;
      friend class IO;

    public:

      File ();
      File (const File&) = delete;

      ~File ();

      // ----------------------------------------------------------------------

      static File*
      open (const char* path, int oflag, ...);

      static File*
      vopen (const char* path, int oflag, std::va_list args);

      // ----------------------------------------------------------------------

      off_t
      lseek (off_t offset, int whence);

      int
      ftruncate (off_t length);

      int
      fsync (void);

      // ----------------------------------------------------------------------
      // Support functions.

      FileSystem*
      getFileSystem (void) const;

    protected:

      // ----------------------------------------------------------------------
      // Implementations

      /**
       * return 0 if success or -1 & errno
       */
      virtual int
      do_vopen (const char* path, int oflag, std::va_list args) = 0;

      virtual off_t
      do_lseek (off_t offset, int whence);

      virtual int
      do_ftruncate (off_t length);

      virtual int
      do_fsync (void);

      virtual void
      do_release (void) override;

      // ----------------------------------------------------------------------
      // Support functions.

      void
      setFileSystem (FileSystem* fileSystem);

    private:

      FileSystem* fFileSystem;
    };

    // ------------------------------------------------------------------------

    inline File*
    File::vopen (const char* path, int oflag, std::va_list args)
    {
      return static_cast<File*> (os::posix::vopen (path, oflag, args));
    }

    inline void
    File::setFileSystem (FileSystem* fileSystem)
    {
      fFileSystem = fileSystem;
    }

    inline FileSystem*
    File::getFileSystem (void) const
    {
      return fFileSystem;
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* POSIX_IO_FILE_H_ */
