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

#include "posix-io/FileDescriptorsManager.h"
#include "posix-io/IO.h"
#include "posix-io/CharDevice.h"
#include "posix-io/CharDevicesRegistry.h"
#include "posix-io/File.h"
#include "posix-io/FileSystem.h"
#include "posix-io/MountManager.h"
#include "posix-io/Pool.h"
#include "posix-io/NetStack.h"

#include "posix/sys/uio.h"

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

    IO*
    open (const char* path, int oflag, ...)
    {
      // Forward to the variadic version of the function.
      std::va_list args;
      va_start(args, oflag);
      IO* const ret = vopen (path, oflag, args);
      va_end(args);

      return ret;
    }

    /**
     * The actual open workhorse. Using the 'path', try to identify the
     * IO object, then call the implementation. If successful, allocate a
     * new POSIX file descriptor, to be used by C functions.
     */
    IO*
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

      // First check if path is a device.
      os::posix::IO* io = os::posix::CharDevicesRegistry::identifyDevice (path);
      if (io != nullptr)
        {
          // If so, use the implementation to open the device.
          int oret = static_cast<CharDevice*> (io)->do_vopen (path, oflag,
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
          auto* const fs = os::posix::MountManager::identifyFileSystem (
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
      // Return a valid pointer to an object derived from IO, or nullptr.
      return io->allocFileDescriptor ();
    }

    // ------------------------------------------------------------------------

    IO*
    IO::allocFileDescriptor (void)
    {

      int fd = FileDescriptorsManager::alloc (this);
      if (fd < 0)
        {
          // If allocation failed, close this object.
          do_close ();
          clearFileDescriptor ();
          return nullptr;
        }

      // Return a valid pointer to an object derived from IO.
      return this;
    }

    // ------------------------------------------------------------------------

    IO::IO ()
    {
      fType = Type::NOTSET;
      fFileDescriptor = noFileDescriptor;
    }

    IO::~IO ()
    {
      fFileDescriptor = noFileDescriptor;
    }

    // ------------------------------------------------------------------------

    int
    IO::close (void)
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
      FileDescriptorsManager::free (fFileDescriptor);
      fFileDescriptor = noFileDescriptor;

      // Release objects acquired from a pool.
      do_release ();
      return ret;
    }

    void
    IO::do_release (void)
    {
      return;
    }

    bool
    IO::do_is_opened (void)
    {
      return true;
    }

    bool
    IO::do_is_connected (void)
    {
      return true;
    }

    // ------------------------------------------------------------------------

    // All these wrappers are required to clear 'errno'.

    ssize_t
    IO::read (void* buf, std::size_t nbyte)
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
    IO::write (const void* buf, std::size_t nbyte)
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
    IO::writev (const struct iovec* iov, int iovcnt)
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
    IO::fcntl (int cmd, ...)
    {
      // Forward to the variadic version of the function.
      std::va_list args;
      va_start(args, cmd);
      int ret = vfcntl (cmd, args);
      va_end(args);

      return ret;
    }

    int
    IO::vfcntl (int cmd, std::va_list args)
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
    IO::isatty (void)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_isatty ();
    }

    // fstat() on a socket returns a zero'd buffer.
    int
    IO::fstat (struct stat* buf)
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
    IO::do_close (void)
    {
      return 0; // Always return success
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    ssize_t
    IO::do_read (void* buf, std::size_t nbyte)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    ssize_t
    IO::do_write (const void* buf, std::size_t nbyte)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    // This is not exactly standard, since POSIX requires writev() to be
    // atomic, but functionally it is close. Override it and implement
    // it properly in the derived class.

    ssize_t
    IO::do_writev (const struct iovec* iov, int iovcnt)
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
    IO::do_vfcntl (int cmd, std::va_list args)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    IO::do_isatty (void)
    {
      errno = ENOTTY; // By default, it is not a TTY.
      return 0;
    }

    int
    IO::do_fstat (struct stat* buf)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

#pragma GCC diagnostic pop

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
