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

#include "posix-io/FileDescriptorsManager.h"
#include "posix-io/IO.h"
#include "posix-io/Device.h"
#include "posix-io/DevicesRegistry.h"
#include "posix-io/File.h"
#include "posix-io/FileSystem.h"
#include "posix-io/MountManager.h"
#include "posix-io/Pool.h"
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
      errno = 0;

      if (path == nullptr)
        {
          errno = EBADF;
          return nullptr;
        }

      // First check if path is a device.
      os::posix::IO* io = os::posix::DevicesRegistry::identifyDevice (path);
      if (io != nullptr)
        {
          // If so, use the implementation to open the device.
          int oret = static_cast<Device*> (io)->do_open (path, oflag, args);
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

    int
    IO::close (void)
    {
      errno = 0;

      // Execute the implementation specific code.
      int ret = do_close ();

      // Remove this IO from the file descriptors registry.
      FileDescriptorsManager::free (fFileDescriptor);
      fFileDescriptor = noFileDescriptor;

      if (getType () == Type::FILE)
        {
          // Files is free, return it to the pool.
          auto file = static_cast<File*> (this);
          auto fs = file->getFileSystem ();
          if (fs != nullptr)
            {
              auto pool = fs->getFilesPool ();
              if (pool != nullptr)
                {
                  pool->release (file);
                }
              file->setFileSystem (nullptr);
            }
        }
      return ret;
    }

    // ------------------------------------------------------------------------

    // All these wrappers are required to clear 'errno'.

    ssize_t
    IO::read (void* buf, std::size_t nbyte)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_read (buf, nbyte);
    }

    ssize_t
    IO::write (const void* buf, std::size_t nbyte)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_write (buf, nbyte);
    }

    ssize_t
    IO::writev (const struct iovec* iov, int iovcnt)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_writev (iov, iovcnt);
    }

    int
    IO::ioctl (int request, ...)
    {
      // Forward to the variadic version of the function.
      std::va_list args;
      va_start(args, request);
      int ret = vioctl (request, args);
      va_end(args);

      return ret;
    }

    int
    IO::vioctl (int request, std::va_list args)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_ioctl (request, args);
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
      errno = 0;

      // Execute the implementation specific code.
      return do_fcntl (cmd, args);
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

    ssize_t
    IO::do_writev (const struct iovec* iov, int iovcnt)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    IO::do_ioctl (int request, std::va_list args)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    IO::do_fcntl (int cmd, std::va_list args)
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
