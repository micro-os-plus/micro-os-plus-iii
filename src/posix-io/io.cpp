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

#include <cmsis-plus/posix-io/device-char.h>
#include <cmsis-plus/posix/sys/uio.h>
#include <cmsis-plus/posix-io/device-char-registry.h>
#include <cmsis-plus/posix-io/file.h>
#include <cmsis-plus/posix-io/file-descriptors-manager.h>
#include <cmsis-plus/posix-io/file-system.h>
#include <cmsis-plus/posix-io/io.h>
#include <cmsis-plus/posix-io/mount-manager.h>
#include <cmsis-plus/posix-io/net-stack.h>
#include <cmsis-plus/posix-io/pool.h>

#include <cmsis-plus/diag/trace.h>

#include <cassert>
#include <cerrno>
#include <cstdarg>

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
      va_start (args, oflag);
      io* const ret = vopen (path, oflag, args);
      va_end (args);

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

      // First check if path is a char device.
      os::posix::io* io = os::posix::device_char_registry::identify_device (
          path);
      if (io != nullptr)
        {
          // If so, use the implementation to open the device.
          int oret = static_cast<device_char*> (io)->do_vopen (path, oflag,
                                                               args);
          if (oret < 0)
            {
              // Open failed.
              return nullptr;
            }
        }
      else
        {
          auto adjusted_path = path;
          auto* const fs = os::posix::mount_manager::identify_file_system (
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
          io = fs->open (adjusted_path, oflag, args);
          if (io == nullptr)
            {
              // Open failed.
              return nullptr;
            }
        }

      // If successful, allocate a file descriptor.
      // Return a valid pointer to an object derived from io, or nullptr.

      auto ret = io->alloc_file_descriptor ();
      trace::printf ("%s(\"%s\")=%p fd=%d\n", __func__, path, io,
                     io->file_descriptor ());
      return ret;
    }

    // ------------------------------------------------------------------------

    io*
    io::alloc_file_descriptor (void)
    {

      int fd = file_descriptors_manager::alloc (this);
      if (fd < 0)
        {
          // If allocation failed, close this object.
          do_close ();
          clear_file_descriptor ();
          return nullptr;
        }

      // Return a valid pointer to an object derived from `io`.
      return this;
    }

    // ------------------------------------------------------------------------

    io::io (type t)
    {
      type_ = t;
      file_descriptor_ = no_file_descriptor;
    }

    io::~io ()
    {
      file_descriptor_ = no_file_descriptor;
    }

    // ------------------------------------------------------------------------

    int
    io::close (void)
    {
      errno = 0;

      if (!do_is_opened ())
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      // Execute the implementation specific code.
      int ret = do_close ();

      // Remove this IO from the file descriptors registry.
      file_descriptors_manager::free (file_descriptor_);
      file_descriptor_ = no_file_descriptor;

      // Release objects acquired from a pool.
      do_release ();
      return ret;
    }

    void
    io::do_release (void)
    {
      return;
    }

    bool
    io::do_is_opened (void)
    {
      return true;
    }

    bool
    io::do_is_connected (void)
    {
      return true;
    }

    // ------------------------------------------------------------------------

    // All these wrappers are required to clear 'errno'.

    ssize_t
    io::read (void* buf, std::size_t nbyte)
    {

      if (buf == nullptr)
        {
          errno = EFAULT;
          return -1;
        }

      if (!do_is_opened ())
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      if (!do_is_connected ())
        {
          errno = EIO; // Not opened.
          return -1;
        }

      errno = 0;

      // Execute the implementation specific code.
      return do_read (buf, nbyte);
    }

    ssize_t
    io::write (const void* buf, std::size_t nbyte)
    {
      if (buf == nullptr)
        {
          errno = EFAULT;
          return -1;
        }

      if (!do_is_opened ())
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      if (!do_is_connected ())
        {
          errno = EIO; // Not opened.
          return -1;
        }

      errno = 0;

      if (nbyte == 0)
        {
          return 0; // Nothing to do.
        }

      // Execute the implementation specific code.
      return do_write (buf, nbyte);
    }

    ssize_t
    io::writev (const struct iovec* iov, int iovcnt)
    {
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

      if (!do_is_opened ())
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      if (!do_is_connected ())
        {
          errno = EIO; // Not opened.
          return -1;
        }

      errno = 0;

      // Execute the implementation specific code.
      return do_writev (iov, iovcnt);
    }

    int
    io::fcntl (int cmd, ...)
    {
      // Forward to the variadic version of the function.
      std::va_list args;
      va_start (args, cmd);
      int ret = vfcntl (cmd, args);
      va_end (args);

      return ret;
    }

    int
    io::vfcntl (int cmd, std::va_list args)
    {
      if (!do_is_opened ())
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      if (!do_is_connected ())
        {
          errno = EIO; // Not opened.
          return -1;
        }

      errno = 0;

      // Execute the implementation specific code.
      return do_vfcntl (cmd, args);
    }

    int
    io::isatty (void)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_isatty ();
    }

    // fstat() on a socket returns a zero'd buffer.
    int
    io::fstat (struct stat* buf)
    {
      if (buf == nullptr)
        {
          errno = EFAULT;
          return -1;
        }

      if (!do_is_opened ())
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      if (!do_is_connected ())
        {
          errno = EIO; // Not opened.
          return -1;
        }

      errno = 0;

      // Execute the implementation specific code.
      return do_fstat (buf);
    }

    // ------------------------------------------------------------------------

    // doOpen() is not here because it is virtual,
    // it must be implemented by derived classes.

    int
    io::do_close (void)
    {
      return 0; // Always return success
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    ssize_t
    io::do_read (void* buf, std::size_t nbyte)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    ssize_t
    io::do_write (const void* buf, std::size_t nbyte)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    // This is not exactly standard, since POSIX requires writev() to be
    // atomic, but functionally it is close. Override it and implement
    // it properly in the derived class.

    ssize_t
    io::do_writev (const struct iovec* iov, int iovcnt)
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

    int
    io::do_vfcntl (int cmd, std::va_list args)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    io::do_isatty (void)
    {
      errno = ENOTTY; // By default, it is not a TTY.
      return 0;
    }

    int
    io::do_fstat (struct stat* buf)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

#pragma GCC diagnostic pop

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
