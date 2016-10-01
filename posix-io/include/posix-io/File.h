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
