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

#ifndef CMSIS_PLUS_POSIX_IO_DIRECTORY_H_
#define CMSIS_PLUS_POSIX_IO_DIRECTORY_H_

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#include <cmsis-plus/posix-io/FileSystem.h>

#include <cmsis-plus/posix/dirent.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class Directory;

    // ------------------------------------------------------------------------

    Directory*
    opendir (const char* dirname);

    // ------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    class Directory
    {
      friend class FileSystem;

    public:

      Directory (void);
      Directory (const Directory&) = delete;

      virtual
      ~Directory ();

      // ----------------------------------------------------------------------

      struct dirent *
      read (void);

      void
      rewind (void);

      int
      close (void);

      // ----------------------------------------------------------------------
      // Support functions.

      struct dirent*
      getDirEntry (void);

      const char*
      getName (void) const;

      FileSystem*
      getFileSystem (void) const;

    protected:

      // ----------------------------------------------------------------------
      // Implementations.

      /**
       * @return object if successful, otherwise nullptr and errno.
       */
      virtual Directory*
      do_vopen (const char* dirname) = 0;

      virtual struct dirent*
      do_read (void);

      virtual void
      do_rewind (void);

      virtual int
      do_close (void);

      // ----------------------------------------------------------------------
      // Support functions.

      void
      setFileSystem (FileSystem* fileSystem);

    private:

      FileSystem* fFileSystem;
      struct dirent fDirEntry;
    };

#pragma GCC diagnostic pop

    // ------------------------------------------------------------------------

    inline void
    Directory::setFileSystem (FileSystem* fileSystem)
    {
      fFileSystem = fileSystem;
    }

    inline FileSystem*
    Directory::getFileSystem (void) const
    {
      return fFileSystem;
    }

    inline struct dirent*
    Directory::getDirEntry (void)
    {
      return &fDirEntry;
    }

    inline const char*
    Directory::getName (void) const
    {
      return &fDirEntry.d_name[0];
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_DIRECTORY_H_ */
