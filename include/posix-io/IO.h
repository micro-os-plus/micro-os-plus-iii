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

#ifndef POSIX_IO_IO_H_
#define POSIX_IO_IO_H_

// ----------------------------------------------------------------------------

#include "posix-io/Types.h"
#include <cstddef>
#include <cstdarg>
// Needed for ssize_t
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class IO;
    class FileSystem;

    // ------------------------------------------------------------------------

    IO*
    open (const char* path, int oflag, ...);

    IO*
    vopen (const char* path, int oflag, std::va_list args);

    // ------------------------------------------------------------------------

    class IO
    {
      // ----------------------------------------------------------------------

      friend class FileSystem;
      friend class FileDescriptorsManager;

      friend IO*
      vopen (const char* path, int oflag, std::va_list args);

      // ----------------------------------------------------------------------

    public:

      using type_t = unsigned int;
      enum Type
        : type_t
          { UNKNOWN = 0,
        NOTSET = 1 << 0,
        DEVICE = 1 << 1,
        FILE = 1 << 2,
        SOCKET = 1 << 3
      };

      // ----------------------------------------------------------------------

      IO ();
      IO (const IO&) = delete;

      virtual
      ~IO ();

      // ----------------------------------------------------------------------

      int
      close (void);

      ssize_t
      read (void* buf, std::size_t nbyte);

      ssize_t
      write (const void* buf, std::size_t nbyte);

      ssize_t
      writev (const struct iovec* iov, int iovcnt);

      int
      ioctl (int request, ...);

      int
      vioctl (int request, std::va_list args);

      int
      fcntl (int cmd, ...);

      int
      vfcntl (int cmd, std::va_list args);

      int
      isatty (void);

      int
      fstat (struct stat* buf);

      // ----------------------------------------------------------------------
      // Support functions.

      Type
      getType (void) const;

      fileDescriptor_t
      getFileDescriptor (void) const;

    protected:

      // ----------------------------------------------------------------------
      // Implementations.

      // do_open() is not here, because it is not common
      // (for example for sockets()).

      virtual int
      do_close (void);

      virtual ssize_t
      do_read (void* buf, std::size_t nbyte);

      virtual ssize_t
      do_write (const void* buf, std::size_t nbyte);

      ssize_t
      do_writev (const struct iovec* iov, int iovcnt);

      virtual int
      do_ioctl (int request, std::va_list args);

      virtual int
      do_fcntl (int cmd, va_list args);

      virtual int
      do_isatty (void);

      virtual int
      do_fstat (struct stat* buf);

      // ----------------------------------------------------------------------
      // Support functions.

      void
      setFileDescriptor (fileDescriptor_t fildes);

      void
      clearFileDescriptor (void);

      IO*
      allocFileDescriptor (void);

      // ----------------------------------------------------------------------

    protected:

      Type fType;

    private:

      fileDescriptor_t fFileDescriptor;
    };

    // ------------------------------------------------------------------------

    inline IO::Type
    IO::getType (void) const
    {
      return fType;
    }

    inline void
    IO::setFileDescriptor (fileDescriptor_t fildes)
    {
      fFileDescriptor = fildes;
    }

    inline void
    IO::clearFileDescriptor (void)
    {
      fFileDescriptor = noFileDescriptor;
    }

    inline fileDescriptor_t
    IO::getFileDescriptor (void) const
    {
      return fFileDescriptor;
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* POSIX_IO_IO_H_ */
