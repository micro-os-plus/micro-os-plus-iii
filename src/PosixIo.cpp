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
#include "posix-io/PosixIo.h"
#include "posix-io/PosixDevice.h"
#include "posix-io/PosixDevicesRegistry.h"
#include "posix-io/PosixFile.h"
#include "posix-io/PosixFileSystem.h"
#include "posix-io/PosixFileSystemsManager.h"
#include "posix-io/PosixPool.h"
#include <cassert>
#include <cerrno>
#include <cstdarg>

// ----------------------------------------------------------------------------

// Variadic calls are processed in two steps, first prepare a
// va_list structure, then call implementation functions like doOpen()
// doIoctl(), that use 'va_list args'.

namespace os
{

  // --------------------------------------------------------------------------

  PosixIo::PosixIo ()
  {
    fType = Type::UNKNOWN;
    fFileDescriptor = noFileDescriptor;
  }

  PosixIo::~PosixIo ()
  {
    fFileDescriptor = noFileDescriptor;
  }

  // --------------------------------------------------------------------------

  PosixIo*
  PosixIo::open (const char* path, int oflag, ...)
  {
    // Forward to the variadic version of the function.
    std::va_list args;
    va_start(args, oflag);
    PosixIo* ret = vopen (path, oflag, args);
    va_end(args);

    return ret;
  }

  /**
   * The actual open workhorse. Using the 'path', try to identify the
   * IO object, then call the implementation. If successful, allocate a
   * new POSIX file descriptor, to be used by C functions.
   */
  PosixIo*
  PosixIo::vopen (const char* path, int oflag, std::va_list args)
  {
    errno = 0;

    if (path == nullptr)
      {
        errno = EBADF;
        return nullptr;
      }

    // First check if path is a device.
    os::PosixIo* io = os::PosixDevicesRegistry::identifyDevice (path);
    if (io != nullptr)
      {
        // If so, use the implementation to open the device.
        int oret = io->do_open (path, oflag, args);
        if (oret < 0)
          {
            // Open failed.
            return nullptr;
          }
      }
    else
      {
        const char* adjusted_path = path;
        os::PosixFileSystem* fs =
            os::PosixFileSystemsManager::identifyFileSystem (&adjusted_path);

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
    int fd = FileDescriptorsManager::alloc (io);
    if (fd < 0)
      {
        // If allocation failed, close this object.
        if (io != nullptr)
          {
            io->do_close ();
            io->clearFileDescriptor ();
          }
        return nullptr;
      }

    // Return a valid pointer to an object derived from PosixIo.
    return io;
  }

  int
  PosixIo::close (void)
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
        PosixFile* file = static_cast<PosixFile*> (this);
        PosixFileSystem* fs = file->getFileSystem ();
        if (fs != nullptr)
          {
            PosixPool* pool = fs->getFilesPool ();
            if (pool != nullptr)
              {
                pool->release (file);
              }
          }
      }
    return ret;
  }

  // --------------------------------------------------------------------------

// All these wrappers are required to clear 'errno'.

  ssize_t
  PosixIo::read (void* buf, std::size_t nbyte)
  {
    errno = 0;

    // Execute the implementation specific code.
    return do_read (buf, nbyte);
  }

  ssize_t
  PosixIo::write (const void* buf, std::size_t nbyte)
  {
    errno = 0;

    // Execute the implementation specific code.
    return do_write (buf, nbyte);
  }

  ssize_t
  PosixIo::writev (const struct iovec* iov, int iovcnt)
  {
    errno = 0;

    // Execute the implementation specific code.
    return do_writev (iov, iovcnt);
  }

  int
  PosixIo::ioctl (int request, ...)
  {
    // Forward to the variadic version of the function.
    std::va_list args;
    va_start(args, request);
    int ret = vioctl (request, args);
    va_end(args);

    return ret;
  }

  int
  PosixIo::vioctl (int request, std::va_list args)
  {
    errno = 0;

    // Execute the implementation specific code.
    return do_ioctl (request, args);
  }

  off_t
  PosixIo::lseek (off_t offset, int whence)
  {
    errno = 0;

    // Execute the implementation specific code.
    return do_lseek (offset, whence);
  }

  int
  PosixIo::isatty (void)
  {
    errno = 0;

    // Execute the implementation specific code.
    return do_isatty ();
  }

  int
  PosixIo::fcntl (int cmd, ...)
  {
    // Forward to the variadic version of the function.
    std::va_list args;
    va_start(args, cmd);
    int ret = vfcntl (cmd, args);
    va_end(args);

    return ret;
  }

  int
  PosixIo::vfcntl (int cmd, std::va_list args)
  {
    errno = 0;

    // Execute the implementation specific code.
    return do_fcntl (cmd, args);
  }

  int
  PosixIo::fstat (struct stat* buf)
  {
    errno = 0;

    // Execute the implementation specific code.
    return do_fstat (buf);
  }

  int
  PosixIo::ftruncate (off_t length)
  {
    errno = 0;

    // Execute the implementation specific code.
    return do_ftruncate (length);
  }

  int
  PosixIo::fsync (void)
  {
    errno = 0;

    // Execute the implementation specific code.
    return do_fsync ();
  }

  // --------------------------------------------------------------------------

  // doOpen() is not here because it is virtual,
  // it must be implemented by derived classes.

  int
  PosixIo::do_close (void)
  {
    return 0; // Always return success
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

  ssize_t
  PosixIo::do_read (void* buf, std::size_t nbyte)
  {
    errno = ENOSYS; // Not implemented
    return -1;
  }

  ssize_t
  PosixIo::do_write (const void* buf, std::size_t nbyte)
  {
    errno = ENOSYS; // Not implemented
    return -1;
  }

  ssize_t
  PosixIo::do_writev (const struct iovec* iov, int iovcnt)
  {
    errno = ENOSYS; // Not implemented
    return -1;
  }

  int
  PosixIo::do_ioctl (int request, std::va_list args)
  {
    errno = ENOSYS; // Not implemented
    return -1;
  }

  off_t
  PosixIo::do_lseek (off_t offset, int whence)
  {
    errno = ENOSYS; // Not implemented
    return -1;
  }

  int
  PosixIo::do_isatty (void)
  {
    errno = ENOTTY; // Not a TTY
    return 0;
  }

  int
  PosixIo::do_fcntl (int cmd, std::va_list args)
  {
    errno = ENOSYS; // Not implemented
    return -1;
  }

  int
  PosixIo::do_fstat (struct stat* buf)
  {
    errno = ENOSYS; // Not implemented
    return -1;
  }

  int
  PosixIo::do_ftruncate (off_t length)
  {
    errno = ENOSYS; // Not implemented
    return -1;
  }

  int
  PosixIo::do_fsync (void)
  {
    errno = ENOSYS; // Not implemented
    return -1;
  }

#pragma GCC diagnostic pop

} /* namespace os */

// ----------------------------------------------------------------------------
