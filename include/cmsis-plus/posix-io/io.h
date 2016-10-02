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

#ifndef CMSIS_PLUS_POSIX_IO_IO_H_
#define CMSIS_PLUS_POSIX_IO_IO_H_

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#include <cmsis-plus/posix-io/types.h>

#include <cstddef>
#include <cstdarg>

// Needed for ssize_t
#include <sys/types.h>

// ----------------------------------------------------------------------------

struct iovec;

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class io;
    class file_system;

    // ------------------------------------------------------------------------

    io*
    open (const char* path, int oflag, ...);

    io*
    vopen (const char* path, int oflag, std::va_list args);

    // ------------------------------------------------------------------------

    class io
    {
      // ----------------------------------------------------------------------

      friend class file_system;
      friend class file_descriptors_manager;

      friend io*
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

      io ();
      io (const io&) = delete;

      virtual
      ~io ();

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

#if 0
      bool
      is_opened (void);

      bool
      is_connected (void);
#endif

    protected:

      // ----------------------------------------------------------------------
      // Implementations.

      // do_vopen() is not here, because it is not common
      // (for example for sockets()).

      virtual int
      do_close (void);

      virtual ssize_t
      do_read (void* buf, std::size_t nbyte);

      virtual ssize_t
      do_write (const void* buf, std::size_t nbyte);

      virtual ssize_t
      do_writev (const struct iovec* iov, int iovcnt);

      virtual int
      do_vfcntl (int cmd, std::va_list args);

      virtual int
      do_isatty (void);

      virtual int
      do_fstat (struct stat* buf);

      // ----------------------------------------------------------------------
      // Support functions.

      // Is called at the end of close, to release objects
      // acquired from a pool.
      virtual void
      do_release (void);

      virtual bool
      do_is_opened (void);

      virtual bool
      do_is_connected (void);

      void
      setFileDescriptor (fileDescriptor_t fildes);

      void
      clearFileDescriptor (void);

      io*
      allocFileDescriptor (void);

      // ----------------------------------------------------------------------

    protected:

      Type fType;

    private:

      fileDescriptor_t fFileDescriptor;
    };

    // ------------------------------------------------------------------------

    inline io::Type
    io::getType (void) const
    {
      return fType;
    }

    inline void
    io::setFileDescriptor (fileDescriptor_t fildes)
    {
      fFileDescriptor = fildes;
    }

    inline void
    io::clearFileDescriptor (void)
    {
      fFileDescriptor = noFileDescriptor;
    }

    inline fileDescriptor_t
    io::getFileDescriptor (void) const
    {
      return fFileDescriptor;
    }

#if 0
  inline bool
  io::is_opened (void)
    {
      return do_is_opened ();
    }

  inline bool
  io::is_connected (void)
    {
      return do_is_connected ();
    }
#endif

}
/* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_IO_H_ */
