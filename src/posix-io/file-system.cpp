/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#include <cmsis-plus/posix-io/file-system.h>
#include <cmsis-plus/posix-io/block-device.h>
#include <cmsis-plus/posix-io/device-registry.h>

#include <cerrno>
#include <cassert>
#include <cstring>

// ----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    /**
     * @cond ignore
     */

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

    file_system::mounted_list file_system::mounted_list__;

#pragma GCC diagnostic pop

    /* class */ file_system* file_system::mounted_root__;

    /**
     * @endcond
     */

    // ------------------------------------------------------------------------
    int
    mkdir (const char* path, mode_t mode)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("%s(\"%s\", %u)\n", __func__, path, mode);
#endif

      if (path == nullptr)
        {
          errno = EFAULT;
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          return -1;
        }

      auto adjusted_path = path;
      auto* const fs = file_system::identify_mounted (&adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          return -1;
        }

      // Execute the implementation specific code.
      return fs->mkdir (adjusted_path, mode);
    }

    int
    rmdir (const char* path)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("%s(\"%s\")\n", __func__, path);
#endif

      if (path == nullptr)
        {
          errno = EFAULT;
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          return -1;
        }

      auto adjusted_path = path;
      auto* const fs = file_system::identify_mounted (&adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          return -1;
        }

      // Execute the implementation specific code.
      return fs->rmdir (adjusted_path);
    }

    void
    sync (void)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("%s()\n", __func__);
#endif

      // Enumerate all mounted file systems and sync them.
#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Waggregate-return"
#endif
      for (auto&& fs : file_system::mounted_list__)
#pragma GCC diagnostic pop
        {
          fs.sync ();
        }

      if (file_system::mounted_root__ != nullptr)
        {
          file_system::mounted_root__->sync ();
        }
    }

    // ------------------------------------------------------------------------
    // Functions related to files, other than IO. The implementations is
    // specific to each file_system.

    int
    chmod (const char* path, mode_t mode)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("%s(\"%s\", %u)\n", __func__, path, mode);
#endif

      if (path == nullptr)
        {
          errno = EFAULT;
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          return -1;
        }

      const char* adjusted_path = path;
      auto* const fs = file_system::identify_mounted (&adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          return -1;
        }

      return fs->chmod (adjusted_path, mode);
    }

    int
    stat (const char* path, struct stat* buf)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("%s(\"%s\", %p)\n", __func__, path, buf);
#endif

      if ((path == nullptr) || (buf == nullptr))
        {
          errno = EFAULT;
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          return -1;
        }

      const char* adjusted_path = path;
      auto* const fs = file_system::identify_mounted (&adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          return -1;
        }

      return fs->stat (adjusted_path, buf);
    }

    int
    truncate (const char* path, off_t length)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("%s(\"%s\", %u)\n", __func__, path, length);
#endif

      if (path == nullptr)
        {
          errno = EFAULT;
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          return -1;
        }

      const char* adjusted_path = path;
      auto* const fs = file_system::identify_mounted (&adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          return -1;
        }

      if (length < 0)
        {
          errno = EINVAL;
          return -1;
        }

      return fs->truncate (adjusted_path, length);
    }

    int
    rename (const char* existing, const char* _new)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("%s(\"%s\",\"%s\")\n", __func__, existing, _new);
#endif

      if ((existing == nullptr) || (_new == nullptr))
        {
          errno = EFAULT;
          return -1;
        }

      if ((*existing == '\0') || (*_new == '\0'))
        {
          errno = ENOENT;
          return -1;
        }

      auto adjusted_existing = existing;
      auto adjusted_new = _new;
      auto* const fs = file_system::identify_mounted (&adjusted_existing,
                                                      &adjusted_new);

      if (fs == nullptr)
        {
          errno = ENOENT;
          return -1;
        }

      return fs->rename (adjusted_existing, adjusted_new);
    }

    int
    unlink (const char* path)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("%s(\"%s\")\n", __func__, path);
#endif

      if (path == nullptr)
        {
          errno = EFAULT;
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          return -1;
        }

      auto adjusted_path = path;
      auto* const fs = file_system::identify_mounted (&adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          return -1;
        }

      return fs->unlink (adjusted_path);
    }

    int
    utime (const char* path, const /* struct */ utimbuf* times)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("%s(\"%s\", %p)\n", __func__, path, times);
#endif

      if ((path == nullptr) || (times == nullptr))
        {
          errno = EFAULT;
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          return -1;
        }

      auto adjusted_path = path;
      auto* const fs = file_system::identify_mounted (&adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          return -1;
        }

      return fs->utime (adjusted_path, times);
    }

    int
    statvfs (const char* path, struct statvfs* buf)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("%s(\"%s\", %p)\n", __func__, path, buf);
#endif

      if ((path == nullptr) || (buf == nullptr))
        {
          errno = EFAULT;
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          return -1;
        }

      auto adjusted_path = path;
      auto* const fs = file_system::identify_mounted (&adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          return -1;
        }

      return fs->statvfs (buf);
    }

    directory*
    opendir (const char* dirpath)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("%s(\"%s\")\n", __func__, dirpath);
#endif

      if (dirpath == nullptr)
        {
          errno = EFAULT;
          return nullptr;
        }

      if (*dirpath == '\0')
        {
          errno = ENOENT;
          return nullptr;
        }

      errno = 0;

      os::posix::directory* dir;

      while (true)
        {
          // Check if path is a device.
          os::posix::io* io;
          io = os::posix::device_registry<device>::identify_device (dirpath);
          if (io != nullptr)
            {
              // Cannot list devices (for now).
              return nullptr;
            }

          // Check if a regular folder.
          auto adjusted_path = dirpath;
          auto* const fs = os::posix::file_system::identify_mounted (
              &adjusted_path);

          // The manager will return null if there are no file systems
          // registered, no need to check this condition separately.
          if (fs == nullptr)
            {
              errno = EBADF;
              return nullptr;
            }

          // Use the file system implementation to open the directory, using
          // the adjusted path (mount point prefix removed).
          dir = fs->opendir (adjusted_path);
          if (dir == nullptr)
            {
              // Open failed.
              return nullptr;
            }

          break;
        }

      // Return a valid pointer to an object derived from directory, or nullptr.

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("%s(\"%s\")=%p\n", __func__, dirpath, dir);
#endif
      return dir;
    }

    // ========================================================================

    file_system::file_system (file_system_impl& impl, const char* name) :
        name_ (name), //
        impl_ (impl)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(\"%s\")=%p\n", __func__, name_, this);
#endif
      deferred_files_list_.clear ();
      deferred_directories_list_.clear ();
    }

    file_system::~file_system ()
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s() @%p %s\n", __func__, this, name_);
#endif
    }

    // ------------------------------------------------------------------------

    int
    file_system::mkfs (int options, ...)
    {
      // Forward to the variadic version of the function.
      std::va_list args;
      va_start(args, options);
      int ret = vmkfs (options, args);
      va_end(args);

      return ret;
    }

    int
    file_system::vmkfs (int options, std::va_list args)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(%u) @%p\n", __func__, options, this);
#endif

      if (mounted_path_ != nullptr)
        {
          // File system already mounted.
          errno = EBUSY;
          return -1;
        }

      errno = 0;

      int ret;
      ret = impl ().do_vmkfs (options, args);

      return ret;
    }

    int
    file_system::mount (const char* path, unsigned int flags, ...)
    {
      // Forward to the variadic version of the function.
      std::va_list args;
      va_start(args, flags);
      int ret = vmount (path, flags, args);
      va_end(args);

      return ret;
    }

    int
    file_system::vmount (const char* path, unsigned int flags,
                         std::va_list args)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(\"%s\", %u) @%p\n", __func__,
                     path ? path : "nullptr", flags, this);
#endif

      if (mounted_path_ != nullptr)
        {
          // File system already mounted.
          errno = EBUSY;
          return -1;
        }

      if (path != nullptr)
        {
#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Waggregate-return"
#endif
          for (auto&& fs : mounted_list__)
#pragma GCC diagnostic pop
            {
              // Validate the device name by checking duplicates.
              if (std::strcmp (path, fs.mounted_path_) == 0)
                {
                  trace::printf ("Path \"%s\" already mounted.", path);

                  errno = EBUSY;
                  return -1;
                }
            }
        }

      char* p = const_cast<char*> (path);
      if (p != nullptr)
        {
          if (strcmp ("/", path) == 0)
            {
              p = nullptr;
            }
        }

      errno = 0;

      int ret = impl ().do_vmount (flags, args);
      if (ret < 0)
        {
          return -1;
        }

      if (p == nullptr)
        {
          mounted_root__ = this;
          mounted_path_ = "/";
        }
      else
        {
          mounted_list__.link (*this);
          mounted_path_ = path;
        }

      return 0;
    }

    /**
     * @details
     * The root file system must be unmounted last, it cannot be
     * unmounted if other mount points exists.
     */
    int
    file_system::umount (int unsigned flags)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(%u) @%p\n", __func__, flags, this);
#endif

      mount_manager_links_.unlink ();
      mounted_path_ = nullptr;

      if (this == mounted_root__)
        {
          if (!mounted_list__.empty ())
            {
              errno = EBUSY;
              return -1;
            }

          mounted_root__ = nullptr;
        }

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      impl ().do_sync ();
      int ret = impl ().do_umount (flags);
      return ret;
    }

    file_system*
    file_system::identify_mounted (const char** path1, const char** path2)
    {
      assert(path1 != nullptr);
      assert(*path1 != nullptr);

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Waggregate-return"
#endif
      for (auto&& fs : mounted_list__)
#pragma GCC diagnostic pop
        {
          auto len = std::strlen (fs.mounted_path_);

          // Check if path1 starts with the mounted path.
          if (std::strncmp (fs.mounted_path_, *path1, len) == 0)
            {
              // If so, adjust paths to skip over prefix, but keep '/'.
              *path1 = (*path1 + len - 1);
              while ((*path1)[1] == '/')
                {
                  *path1 = (*path1 + 1);
                }

              if ((path2 != nullptr) && (*path2 != nullptr))
                {
                  *path2 = (*path2 + len - 1);
                  while ((*path2)[1] == '/')
                    {
                      *path2 = (*path2 + 1);
                    }
                }

              return &fs;
            }
        }

      // If root file system defined, return it.
      if (mounted_root__ != nullptr)
        {
          return mounted_root__;
        }

      // Not found.
      return nullptr;
    }

    // ------------------------------------------------------------------------

    file*
    file_system::open (const char* path, int oflag, ...)
    {
      // Forward to the variadic version of the function.
      std::va_list args;
      va_start(args, oflag);
      file* ret = vopen (path, oflag, args);
      va_end(args);

      return ret;
    }

    file*
    file_system::vopen (const char* path, int oflag, std::va_list args)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(\"%s\", %u)\n", __func__, path, oflag);
#endif

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          return nullptr;
        }

      errno = 0;

      // Execute the file specific implementation code.
      // Allocation is done by the implementation, where
      // the size is known.
      file* fil = impl ().do_vopen (*this, path, oflag, args);
      if (fil == nullptr)
        {
          return nullptr;
        }

      // If successful, allocate a file descriptor.
      fil->alloc_file_descriptor ();

      return fil;
    }

    directory*
    file_system::opendir (const char* dirpath)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(\"%s\")\n", __func__, dirpath);
#endif

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          return nullptr;
        }

      errno = 0;

      // Execute the dir specific implementation code.
      // Allocation is done by the implementation, where
      // the size is known.
      directory* dir = impl ().do_opendir (*this, dirpath);
      if (dir == nullptr)
        {
          return nullptr;
        }

      return dir;
    }

    // ------------------------------------------------------------------------

    int
    file_system::mkdir (const char* path, mode_t mode)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(\"%s\", %u)\n", __func__, path, mode);
#endif

      if (path == nullptr)
        {
          errno = EFAULT;
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          return -1;
        }

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      errno = 0;

      return impl ().do_mkdir (path, mode);
    }

    int
    file_system::rmdir (const char* path)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(\"%s\")\n", __func__, path);
#endif

      if (path == nullptr)
        {
          errno = EFAULT;
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          return -1;
        }

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      errno = 0;

      return impl ().do_rmdir (path);
    }

    void
    file_system::sync (void)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s() @%p\n", __func__, this);
#endif

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          return;
        }

      errno = 0;

      impl ().do_sync ();
    }

    // ------------------------------------------------------------------------

    int
    file_system::chmod (const char* path, mode_t mode)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(\"%s\", %u)\n", __func__, path, mode);
#endif

      if (path == nullptr)
        {
          errno = EFAULT;
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          return -1;
        }

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_chmod (path, mode);
    }

    int
    file_system::stat (const char* path, struct stat* buf)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(\"%s\", %p)\n", __func__, path, buf);
#endif

      if ((path == nullptr) || (buf == nullptr))
        {
          errno = EFAULT;
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          return -1;
        }

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_stat (path, buf);
    }

    int
    file_system::truncate (const char* path, off_t length)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(\"%s\", %u)\n", __func__, path, length);
#endif

      if (path == nullptr)
        {
          errno = EFAULT;
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          return -1;
        }

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_truncate (path, length);
    }

    int
    file_system::rename (const char* existing, const char* _new)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(\"%s\",\"%s\")\n", __func__, existing,
                     _new);
#endif

      if ((existing == nullptr) || (_new == nullptr))
        {
          errno = EFAULT;
          return -1;
        }

      if ((*existing == '\0') || (*_new == '\0'))
        {
          errno = ENOENT;
          return -1;
        }

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_rename (existing, _new);
    }

    int
    file_system::unlink (const char* path)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(\"%s\")\n", __func__, path);
#endif

      if (path == nullptr)
        {
          errno = EFAULT;
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          return -1;
        }

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_unlink (path);
    }

    // http://pubs.opengroup.org/onlinepubs/9699919799/functions/utime.html
    int
    file_system::utime (const char* path, const /* struct */ utimbuf* times)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(\"%s\", %p)\n", __func__, path, times);
#endif

      if ((path == nullptr) || (times == nullptr))
        {
          errno = EFAULT;
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          return -1;
        }

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      errno = 0;

      /* struct */ utimbuf tmp;
      if (times == nullptr)
        {
          // If times is a null pointer, the access and modification times
          // of the file shall be set to the current time.
          tmp.actime = time (nullptr);
          tmp.modtime = tmp.actime;
          return impl ().do_utime (path, &tmp);
        }
      else
        {
          // Execute the implementation specific code.
          return impl ().do_utime (path, times);
        }
    }

    // http://pubs.opengroup.org/onlinepubs/9699919799/functions/fstatvfs.html
    int
    file_system::statvfs (struct statvfs* buf)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(%p)\n", __func__, buf);
#endif

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      return impl ().do_statvfs (buf);
    }
    // TODO: check if the file system should keep a static current path for
    // relative paths.

    // http://pubs.opengroup.org/onlinepubs/9699919799/functions/chdir.html
    // ------------------------------------------------------------------------

    // ========================================================================

    file_system_impl::file_system_impl (block_device& device) :
        device_ (device)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system_impl::%s()=%p\n", __func__, this);
#endif
    }

    file_system_impl::~file_system_impl ()
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system_impl::%s() @%p\n", __func__, this);
#endif
    }

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
