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

#include <cmsis-plus/posix-io/device.h>
#include <cmsis-plus/posix/sys/uio.h>
#include <cmsis-plus/posix-io/device-registry.h>
#include <cmsis-plus/posix-io/file.h>
#include <cmsis-plus/posix-io/file-descriptors-manager.h>
#include <cmsis-plus/posix-io/file-system.h>
#include <cmsis-plus/posix-io/io.h>

#include <cmsis-plus/diag/trace.h>

#include <cassert>
#include <cerrno>
#include <cstdarg>

// ----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

// ----------------------------------------------------------------------------

// Variadic calls are processed in two steps, first prepare a
// va_list structure, then call implementation functions like doOpen()
// doIoctl(), that use 'va_list args'.

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    io*
    open (const char* path, int oflag, ...)
    {
      // Forward to the variadic version of the function.
      std::va_list args;
      va_start(args, oflag);
      io* const ret = vopen (path, oflag, args);
      va_end(args);

      return ret;
    }

    /**
     * The actual open workhorse. Using the _path_, try to identify the
     * `io` object, then call the implementation. If successful, allocate a
     * new POSIX file descriptor, to be used by C functions.
     */
    io*
    vopen (const char* path, int oflag, std::va_list args)
    {
#if defined(OS_TRACE_POSIX_IO_IO)
      trace::printf ("io::%s(\"%s\")\n", __func__, path ? path : "");
#endif

      if (path == nullptr)
        {
          errno = EFAULT;
          return nullptr;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          return nullptr;
        }

      errno = 0;

      os::posix::io* io;
      while (true)
        {
          // Check if path is a device.
          io = os::posix::device_registry<device>::identify_device (path);
          if (io != nullptr)
            {
              // If so, use the implementation to open the device.
              int oret = static_cast<device*> (io)->vopen (path, oflag, args);
              if (oret < 0)
                {
                  // Open failed.
                  return nullptr;
                }

              // File descriptor already allocated by device.
              break;
            }

          // Check if a regular file.
          auto adjusted_path = path;
          auto* const fs = os::posix::file_system::identify_mounted (
              &adjusted_path);

          // The manager will return null if there are no file systems
          // registered, no need to check this condition separately.
          if (fs == nullptr)
            {
              errno = EBADF;
              return nullptr;
            }

          // Use the file system implementation to open the file, using
          // the adjusted path (mount point prefix removed).
          io = fs->vopen (adjusted_path, oflag, args);
          if (io == nullptr)
            {
              // Open failed.
              return nullptr;
            }

          break;
        }

      // Return a valid pointer to an object derived from io, or nullptr.

#if defined(OS_TRACE_POSIX_IO_IO)
      trace::printf ("io::%s(\"%s\")=%p fd=%d\n", __func__, path, io,
                     io->file_descriptor ());
#endif
      return io;
    }

    // ========================================================================

    io::io (io_impl& impl, type t) :
        impl_ (impl), //
        type_ (t)
    {
#if defined(OS_TRACE_POSIX_IO_IO)
      trace::printf ("io::%s()=%p\n", __func__, this);
#endif

      file_descriptor_ = no_file_descriptor;
    }

    io::~io ()
    {
#if defined(OS_TRACE_POSIX_IO_IO)
      trace::printf ("io::%s() @%p\n", __func__, this);
#endif

      file_descriptor_ = no_file_descriptor;
    }

    // ------------------------------------------------------------------------

    int
    io::close (void)
    {
#if defined(OS_TRACE_POSIX_IO_IO)
      trace::printf ("io::%s() @%p\n", __func__, this);
#endif

      if (!impl ().do_is_opened ())
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      errno = 0;

      // Execute the implementation specific code.
      int ret = impl ().do_close ();

      // Remove this IO from the file descriptors registry.
      file_descriptors_manager::deallocate (file_descriptor_);
      file_descriptor_ = no_file_descriptor;

      return ret;
    }

    io*
    io::alloc_file_descriptor (void)
    {
#if defined(OS_TRACE_POSIX_IO_IO)
      trace::printf ("io::%s() @%p\n", __func__, this);
#endif

      int fd = file_descriptors_manager::allocate (this);
      if (fd < 0)
        {
          // If allocation failed, close this object.
          impl ().do_close ();
          clear_file_descriptor ();
          return nullptr;
        }

#if defined(OS_TRACE_POSIX_IO_IO)
      trace::printf ("io::%s() @%p fd=%d\n", __func__, this, fd);
#endif

      // Return a valid pointer to an object derived from `io`.
      return this;
    }

    // ------------------------------------------------------------------------

    // All these wrappers are required to clear 'errno'.

    ssize_t
    io::read (void* buf, std::size_t nbyte)
    {
#if defined(OS_TRACE_POSIX_IO_IO)
      trace::printf ("io::%s(0x0%X, %u) @%p\n", __func__, buf, nbyte, this);
#endif

      if (buf == nullptr)
        {
          errno = EFAULT;
          return -1;
        }

      if (!impl ().do_is_opened ())
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      if (!impl ().do_is_connected ())
        {
          errno = EIO; // Not opened.
          return -1;
        }

      errno = 0;

      // http://pubs.opengroup.org/onlinepubs/9699919799/functions/read.html
      // Before any action described below is taken, and if nbyte is zero,
      // the read() function may detect and return errors as described below.
      // In the absence of errors, or if error detection is not performed,
      // the read() function shall return zero and have no other results.
      if (nbyte == 0)
        {
          return 0; // Nothing to do.
        }

      // Execute the implementation specific code.
      ssize_t ret = impl ().do_read (buf, nbyte);
      if (ret >= 0)
        {
          impl ().offset_ += ret;
        }

#if defined(OS_TRACE_POSIX_IO_IO)
      trace::printf ("io::%s(0x0%X, %u) @%p n=%d\n", __func__, buf, nbyte, this,
                     ret);
#endif
      return ret;
    }

    ssize_t
    io::write (const void* buf, std::size_t nbyte)
    {
#if defined(OS_TRACE_POSIX_IO_IO)
      trace::printf ("io::%s(0x0%X, %u) @%p\n", __func__, buf, nbyte, this);
#endif

      if (buf == nullptr)
        {
          errno = EFAULT;
          return -1;
        }

      if (!impl ().do_is_opened ())
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      if (!impl ().do_is_connected ())
        {
          errno = EIO; // Not opened.
          return -1;
        }

      errno = 0;

      // http://pubs.opengroup.org/onlinepubs/9699919799/functions/pwrite.html
      // Before any action described below is taken, and if nbyte is zero
      // and the file is a regular file, the write() function may detect and
      // return errors as described below. In the absence of errors, or if
      // error detection is not performed, the write() function shall return
      // zero and have no other results. If nbyte is zero and the file is
      // not a regular file, the results are unspecified.
      if (nbyte == 0)
        {
          return 0; // Nothing to do.
        }

      // Execute the implementation specific code.
      ssize_t ret = impl ().do_write (buf, nbyte);
      if (ret >= 0)
        {
          impl ().offset_ += ret;
        }

#if defined(OS_TRACE_POSIX_IO_IO)
      trace::printf ("io::%s(0x0%X, %u) @%p n=%d\n", __func__, buf, nbyte, this,
                     ret);
#endif
      return ret;
    }

    ssize_t
    io::writev (const struct iovec* iov, int iovcnt)
    {
#if defined(OS_TRACE_POSIX_IO_IO)
      trace::printf ("io::%s(0x0%X, %d) @%p\n", __func__, iov, iovcnt, this);
#endif

      if (iov == nullptr)
        {
          errno = EFAULT;
          return -1;
        }

      if (iovcnt <= 0)
        {
          errno = EINVAL;
          return -1;
        }

      if (!impl ().do_is_opened ())
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      if (!impl ().do_is_connected ())
        {
          errno = EIO; // Not opened.
          return -1;
        }

      errno = 0;

      // Execute the implementation specific code.
      ssize_t ret = impl ().do_writev (iov, iovcnt);
      if (ret >= 0)
        {
          impl ().offset_ += ret;
        }
      return ret;
    }

    int
    io::fcntl (int cmd, ...)
    {
      // Forward to the variadic version of the function.
      std::va_list args;
      va_start(args, cmd);
      int ret = vfcntl (cmd, args);
      va_end(args);

      return ret;
    }

    int
    io::vfcntl (int cmd, std::va_list args)
    {
#if defined(OS_TRACE_POSIX_IO_IO)
      trace::printf ("io::%s(%d) @%p\n", __func__, cmd, this);
#endif

      if (!impl ().do_is_opened ())
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      if (!impl ().do_is_connected ())
        {
          errno = EIO; // Not opened.
          return -1;
        }

      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_vfcntl (cmd, args);
    }

    int
    io::isatty (void)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_isatty ();
    }

    // fstat() on a socket returns a zero'd buffer.
    int
    io::fstat (struct stat* buf)
    {
#if defined(OS_TRACE_POSIX_IO_IO)
      trace::printf ("io::%s(%p) @%p\n", __func__, buf, this);
#endif

      if (buf == nullptr)
        {
          errno = EFAULT;
          return -1;
        }

      if (!impl ().do_is_opened ())
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      if (!impl ().do_is_connected ())
        {
          errno = EIO; // Not opened.
          return -1;
        }

      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_fstat (buf);
    }

    off_t
    io::lseek (off_t offset, int whence)
    {
#if defined(OS_TRACE_POSIX_IO_IO)
      trace::printf ("io::%s(%d, %d) @%p\n", __func__, offset, whence, this);
#endif

      if (!impl ().do_is_opened ())
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_lseek (offset, whence);
    }

    // ========================================================================

    io_impl::io_impl (void)
    {
#if defined(OS_TRACE_POSIX_IO_IO)
      trace::printf ("io_impl::%s()=%p\n", __func__, this);
#endif
    }

    io_impl::~io_impl ()
    {
#if defined(OS_TRACE_POSIX_IO_IO)
      trace::printf ("io_impl::%s() @%p\n", __func__, this);
#endif
    }

    void
    io_impl::do_deallocate (void)
    {
      return;
    }

    bool
    io_impl::do_is_connected (void)
    {
      return true;
    }

    ssize_t
    io_impl::do_writev (const struct iovec* iov, int iovcnt)
    {
      ssize_t total = 0;

      const struct iovec* p = iov;
      for (int i = 0; i < iovcnt; ++i, ++p)
        {
          ssize_t ret = do_write (p->iov_base, p->iov_len);
          if (ret < 0)
            {
              return ret;
            }
          total += ret;
        }
      return total;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    int
    io_impl::do_vfcntl (int cmd, std::va_list args)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    io_impl::do_isatty (void)
    {
      errno = ENOTTY; // By default, it is not a TTY.
      return 0;
    }

    int
    io_impl::do_fstat (struct stat* buf)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

#pragma GCC diagnostic pop

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
