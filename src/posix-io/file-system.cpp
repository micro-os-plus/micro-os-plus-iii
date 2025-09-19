/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2015-2025 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit.
 */

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/posix-io/file-system.h>
#include <cmsis-plus/posix-io/block-device.h>
#include <cmsis-plus/posix-io/device-registry.h>

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/diag/instrumentation.h>

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
      instrumentation::posix::mkdir (path, mode);

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("%s(\"%s\", %u)\n", __func__, path, mode);
#endif

      if (path == nullptr)
        {
          errno = EFAULT;
          instrumentation::posix::mkdir_retval (-1);
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          instrumentation::posix::mkdir_retval (-1);
          return -1;
        }

      auto adjusted_path = path;
      auto* const fs = file_system::identify_mounted (&adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          instrumentation::posix::mkdir_retval (-1);
          return -1;
        }

      // Execute the implementation specific code.
      int ret = fs->mkdir (adjusted_path, mode);

      instrumentation::posix::mkdir_retval (ret);
      return ret;
    }

    int
    rmdir (const char* path)
    {
      instrumentation::posix::rmdir (path);

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("%s(\"%s\")\n", __func__, path);
#endif

      if (path == nullptr)
        {
          errno = EFAULT;
          instrumentation::posix::rmdir_retval (-1);
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          instrumentation::posix::rmdir_retval (-1);
          return -1;
        }

      auto adjusted_path = path;
      auto* const fs = file_system::identify_mounted (&adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          instrumentation::posix::rmdir_retval (-1);
          return -1;
        }

      // Execute the implementation specific code.
      int ret = fs->rmdir (adjusted_path);

      instrumentation::posix::rmdir_retval (ret);
      return ret;
    }

    void
    sync (void)
    {
      instrumentation::posix::sync ();

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

      instrumentation::posix::sync_return ();
    }

    // ------------------------------------------------------------------------
    // Functions related to files, other than IO. The implementations is
    // specific to each file_system.

    int
    chmod (const char* path, mode_t mode)
    {
      instrumentation::posix::chmod (path, mode);

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("%s(\"%s\", %u)\n", __func__, path, mode);
#endif

      if (path == nullptr)
        {
          errno = EFAULT;
          instrumentation::posix::chmod_retval (-1);
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          instrumentation::posix::chmod_retval (-1);
          return -1;
        }

      const char* adjusted_path = path;
      auto* const fs = file_system::identify_mounted (&adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          instrumentation::posix::chmod_retval (-1);
          return -1;
        }

      int ret = fs->chmod (adjusted_path, mode);

      instrumentation::posix::chmod_retval (ret);
      return ret;
    }

    int
    stat (const char* path, struct stat* buf)
    {
      instrumentation::posix::stat (path, buf);

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("%s(\"%s\", %p)\n", __func__, path, buf);
#endif

      if ((path == nullptr) || (buf == nullptr))
        {
          errno = EFAULT;
          instrumentation::posix::stat_retval (-1);
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          instrumentation::posix::stat_retval (-1);
          return -1;
        }

      const char* adjusted_path = path;
      auto* const fs = file_system::identify_mounted (&adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          instrumentation::posix::stat_retval (-1);
          return -1;
        }

      int ret = fs->stat (adjusted_path, buf);

      instrumentation::posix::stat_retval (ret);
      return ret;
    }

    int
    truncate (const char* path, off_t length)
    {
      instrumentation::posix::truncate (path, length);

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("%s(\"%s\", %u)\n", __func__, path, length);
#endif

      if (path == nullptr)
        {
          errno = EFAULT;
          instrumentation::posix::truncate_retval (-1);
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          instrumentation::posix::truncate_retval (-1);
          return -1;
        }

      const char* adjusted_path = path;
      auto* const fs = file_system::identify_mounted (&adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          instrumentation::posix::truncate_retval (-1);
          return -1;
        }

      if (length < 0)
        {
          errno = EINVAL;
          instrumentation::posix::truncate_retval (-1);
          return -1;
        }

      int ret = fs->truncate (adjusted_path, length);

      instrumentation::posix::truncate_retval (ret);
      return ret;
    }

    int
    rename (const char* existing, const char* _new)
    {
      instrumentation::posix::rename (existing, _new);

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("%s(\"%s\",\"%s\")\n", __func__, existing, _new);
#endif

      if ((existing == nullptr) || (_new == nullptr))
        {
          errno = EFAULT;
          instrumentation::posix::rename_retval (-1);
          return -1;
        }

      if ((*existing == '\0') || (*_new == '\0'))
        {
          errno = ENOENT;
          instrumentation::posix::rename_retval (-1);
          return -1;
        }

      auto adjusted_existing = existing;
      auto adjusted_new = _new;
      auto* const fs
          = file_system::identify_mounted (&adjusted_existing, &adjusted_new);

      if (fs == nullptr)
        {
          errno = ENOENT;
          instrumentation::posix::rename_retval (-1);
          return -1;
        }

      int ret = fs->rename (adjusted_existing, adjusted_new);

      instrumentation::posix::rename_retval (ret);
      return ret;
    }

    int
    unlink (const char* path)
    {
      instrumentation::posix::unlink (path);

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("%s(\"%s\")\n", __func__, path);
#endif

      if (path == nullptr)
        {
          errno = EFAULT;
          instrumentation::posix::unlink_retval (-1);
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          instrumentation::posix::unlink_retval (-1);
          return -1;
        }

      auto adjusted_path = path;
      auto* const fs = file_system::identify_mounted (&adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          instrumentation::posix::unlink_retval (-1);
          return -1;
        }

      int ret = fs->unlink (adjusted_path);

      instrumentation::posix::unlink_retval (ret);
      return ret;
    }

    int
    utime (const char* path, const /* struct */ utimbuf* times)
    {
      instrumentation::posix::utime (path, times);

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("%s(\"%s\", %p)\n", __func__, path, times);
#endif

      if ((path == nullptr) || (times == nullptr))
        {
          errno = EFAULT;
          instrumentation::posix::utime_retval (-1);
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          instrumentation::posix::utime_retval (-1);
          return -1;
        }

      auto adjusted_path = path;
      auto* const fs = file_system::identify_mounted (&adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          instrumentation::posix::utime_retval (-1);
          return -1;
        }

      int ret = fs->utime (adjusted_path, times);

      instrumentation::posix::utime_retval (ret);
      return ret;
    }

    int
    statvfs (const char* path, struct statvfs* buf)
    {
      instrumentation::posix::statvfs (path, buf);

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("%s(\"%s\", %p)\n", __func__, path, buf);
#endif

      if ((path == nullptr) || (buf == nullptr))
        {
          errno = EFAULT;
          instrumentation::posix::statvfs_retval (-1);
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          instrumentation::posix::statvfs_retval (-1);
          return -1;
        }

      auto adjusted_path = path;
      auto* const fs = file_system::identify_mounted (&adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          instrumentation::posix::statvfs_retval (-1);
          return -1;
        }

      int ret = fs->statvfs (buf);

      instrumentation::posix::statvfs_retval (ret);
      return ret;
    }

    directory*
    opendir (const char* dirpath)
    {
      instrumentation::posix::opendir (dirpath);

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("%s(\"%s\")\n", __func__, dirpath);
#endif

      if (dirpath == nullptr)
        {
          errno = EFAULT;
          instrumentation::posix::opendir_retval (nullptr);
          return nullptr;
        }

      if (*dirpath == '\0')
        {
          errno = ENOENT;
          instrumentation::posix::opendir_retval (nullptr);
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
              instrumentation::posix::opendir_retval (nullptr);
              return nullptr;
            }

          // Check if a regular folder.
          auto adjusted_path = dirpath;
          auto* const fs
              = os::posix::file_system::identify_mounted (&adjusted_path);

          // The manager will return null if there are no file systems
          // registered, no need to check this condition separately.
          if (fs == nullptr)
            {
              errno = EBADF;
              instrumentation::posix::opendir_retval (nullptr);
              return nullptr;
            }

          // Use the file system implementation to open the directory, using
          // the adjusted path (mount point prefix removed).
          dir = fs->opendir (adjusted_path);
          if (dir == nullptr)
            {
              // Open failed.
              instrumentation::posix::opendir_retval (nullptr);
              return nullptr;
            }

          break;
        }

        // Return a valid pointer to an object derived from directory, or
        // nullptr.

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("%s(\"%s\")=%p\n", __func__, dirpath, dir);
#endif

      instrumentation::posix::opendir_retval (dir);
      return dir;
    }

    // ========================================================================

    file_system::file_system (file_system_impl& impl, const char* name)
        : name_ (name), //
          impl_ (impl)
    {
      instrumentation::posix::file_system::create (this);

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(\"%s\")=%p\n", __func__, name_, this);
#endif
      deferred_files_list_.clear ();
      deferred_directories_list_.clear ();
    }

    file_system::~file_system ()
    {
      instrumentation::posix::file_system::destroy (this);

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
      va_start (args, options);
      int ret = vmkfs (options, args);
      va_end (args);

      return ret;
    }

    int
    file_system::vmkfs (int options, std::va_list args)
    {
      instrumentation::posix::file_system::vmkfs (this, options);

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(%u) @%p\n", __func__, options, this);
#endif

      if (mounted_path_ != nullptr)
        {
          // File system already mounted.
          errno = EBUSY;
          instrumentation::posix::file_system::vmkfs_retval (this, -1);
          return -1;
        }

      errno = 0;

      int ret;
      ret = impl ().do_vmkfs (options, args);

      instrumentation::posix::file_system::vmkfs_retval (this, ret);
      return ret;
    }

    int
    file_system::mount (const char* path, unsigned int flags, ...)
    {
      // Forward to the variadic version of the function.
      std::va_list args;
      va_start (args, flags);
      int ret = vmount (path, flags, args);
      va_end (args);

      return ret;
    }

    int
    file_system::vmount (const char* path, unsigned int flags,
                         std::va_list args)
    {
      instrumentation::posix::file_system::vmount (this, path, flags);

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(\"%s\", %u) @%p\n", __func__,
                     path ? path : "nullptr", flags, this);
#endif

      if (mounted_path_ != nullptr)
        {
          // File system already mounted.
          errno = EBUSY;
          instrumentation::posix::file_system::vmount_retval (this, -1);
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
                  instrumentation::posix::file_system::vmount_retval (this,
                                                                      -1);
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
          instrumentation::posix::file_system::vmount_retval (this, -1);
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

      instrumentation::posix::file_system::vmount_retval (this, 0);
      return 0;
    }

    /**
     * @details
     * The root file system must be unmounted last, it cannot be
     * unmounted if other mount points exists.
     */
    int
    file_system::umount (unsigned int flags)
    {
      instrumentation::posix::file_system::umount (this, flags);

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
              instrumentation::posix::file_system::umount_retval (this, -1);
              return -1;
            }

          mounted_root__ = nullptr;
        }

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          instrumentation::posix::file_system::umount_retval (this, -1);
          return -1;
        }

      impl ().do_sync ();
      int ret = impl ().do_umount (flags);

      instrumentation::posix::file_system::umount_retval (this, ret);
      return ret;
    }

    file_system*
    file_system::identify_mounted (const char** path1, const char** path2)
    {
      assert (path1 != nullptr);
      assert (*path1 != nullptr);

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
#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
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
#pragma GCC diagnostic pop

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
      va_start (args, oflag);
      file* ret = vopen (path, oflag, args);
      va_end (args);

      return ret;
    }

    file*
    file_system::vopen (const char* path, int oflag, std::va_list args)
    {
      instrumentation::posix::file_system::vopen (this, path, oflag);

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(\"%s\", %u)\n", __func__, path, oflag);
#endif

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          instrumentation::posix::file_system::vopen_retval (this, nullptr);
          return nullptr;
        }

      errno = 0;

      // Execute the file specific implementation code.
      // Allocation is done by the implementation, where
      // the size is known.
      file* fil = impl ().do_vopen (*this, path, oflag, args);
      if (fil == nullptr)
        {
          instrumentation::posix::file_system::vopen_retval (this, nullptr);
          return nullptr;
        }

      // If successful, allocate a file descriptor.
      fil->alloc_file_descriptor ();

      instrumentation::posix::file_system::vopen_retval (this, fil);
      return fil;
    }

    directory*
    file_system::opendir (const char* dirpath)
    {
      instrumentation::posix::file_system::opendir (this, dirpath);

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(\"%s\")\n", __func__, dirpath);
#endif

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          instrumentation::posix::file_system::opendir_retval (this, nullptr);
          return nullptr;
        }

      errno = 0;

      // Execute the dir specific implementation code.
      // Allocation is done by the implementation, where
      // the size is known.
      directory* dir = impl ().do_opendir (*this, dirpath);
      if (dir == nullptr)
        {
          instrumentation::posix::file_system::opendir_retval (this, nullptr);
          return nullptr;
        }

      instrumentation::posix::file_system::opendir_retval (this, dir);
      return dir;
    }

    // ------------------------------------------------------------------------

    int
    file_system::mkdir (const char* path, mode_t mode)
    {
      instrumentation::posix::file_system::mkdir (this, path, mode);
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(\"%s\", %u)\n", __func__, path, mode);
#endif

      if (path == nullptr)
        {
          errno = EFAULT;
          instrumentation::posix::file_system::mkdir_retval (this, -1);
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          instrumentation::posix::file_system::mkdir_retval (this, -1);
          return -1;
        }

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          instrumentation::posix::file_system::mkdir_retval (this, -1);
          return -1;
        }

      errno = 0;

      int ret = impl ().do_mkdir (path, mode);

      instrumentation::posix::file_system::mkdir_retval (this, ret);
      return ret;
    }

    int
    file_system::rmdir (const char* path)
    {
      instrumentation::posix::file_system::rmdir (this, path);

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(\"%s\")\n", __func__, path);
#endif

      if (path == nullptr)
        {
          errno = EFAULT;
          instrumentation::posix::file_system::rmdir_retval (this, -1);
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          instrumentation::posix::file_system::rmdir_retval (this, -1);
          return -1;
        }

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          instrumentation::posix::file_system::rmdir_retval (this, -1);
          return -1;
        }

      errno = 0;

      int ret = impl ().do_rmdir (path);

      instrumentation::posix::file_system::rmdir_retval (this, ret);
      return ret;
    }

    void
    file_system::sync (void)
    {
      instrumentation::posix::file_system::sync (this);

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s() @%p\n", __func__, this);
#endif

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          instrumentation::posix::file_system::sync_return (this);
          return;
        }

      errno = 0;

      impl ().do_sync ();

      instrumentation::posix::file_system::sync_return (this);
    }

    // ------------------------------------------------------------------------

    int
    file_system::chmod (const char* path, mode_t mode)
    {
      instrumentation::posix::file_system::chmod (this, path, mode);

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(\"%s\", %u)\n", __func__, path, mode);
#endif

      if (path == nullptr)
        {
          errno = EFAULT;
          instrumentation::posix::file_system::chmod_retval (this, -1);
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          instrumentation::posix::file_system::chmod_retval (this, -1);
          return -1;
        }

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          instrumentation::posix::file_system::chmod_retval (this, -1);
          return -1;
        }

      errno = 0;

      // Execute the implementation specific code.
      int ret = impl ().do_chmod (path, mode);

      instrumentation::posix::file_system::chmod_retval (this, ret);
      return ret;
    }

    int
    file_system::stat (const char* path, struct stat* buf)
    {
      instrumentation::posix::file_system::stat (this, path, buf);

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(\"%s\", %p)\n", __func__, path, buf);
#endif

      if ((path == nullptr) || (buf == nullptr))
        {
          errno = EFAULT;
          instrumentation::posix::file_system::stat_retval (this, -1);
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          instrumentation::posix::file_system::stat_retval (this, -1);
          return -1;
        }

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          instrumentation::posix::file_system::stat_retval (this, -1);
          return -1;
        }

      errno = 0;

      // Execute the implementation specific code.
      int ret = impl ().do_stat (path, buf);

      instrumentation::posix::file_system::stat_retval (this, ret);
      return ret;
    }

    int
    file_system::truncate (const char* path, off_t length)
    {
      instrumentation::posix::file_system::truncate (this, path, length);

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(\"%s\", %u)\n", __func__, path, length);
#endif

      if (path == nullptr)
        {
          errno = EFAULT;
          instrumentation::posix::file_system::truncate_retval (this, -1);
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          instrumentation::posix::file_system::truncate_retval (this, -1);
          return -1;
        }

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          instrumentation::posix::file_system::truncate_retval (this, -1);
          return -1;
        }

      errno = 0;

      // Execute the implementation specific code.
      int ret = impl ().do_truncate (path, length);

      instrumentation::posix::file_system::truncate_retval (this, ret);
      return ret;
    }

    int
    file_system::rename (const char* existing, const char* _new)
    {
      instrumentation::posix::file_system::rename (this, existing, _new);

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(\"%s\",\"%s\")\n", __func__, existing,
                     _new);
#endif

      if ((existing == nullptr) || (_new == nullptr))
        {
          errno = EFAULT;
          instrumentation::posix::file_system::rename_retval (this, -1);
          return -1;
        }

      if ((*existing == '\0') || (*_new == '\0'))
        {
          errno = ENOENT;
          instrumentation::posix::file_system::rename_retval (this, -1);
          return -1;
        }

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          instrumentation::posix::file_system::rename_retval (this, -1);
          return -1;
        }

      errno = 0;

      // Execute the implementation specific code.
      int ret = impl ().do_rename (existing, _new);

      instrumentation::posix::file_system::rename_retval (this, ret);
      return ret;
    }

    int
    file_system::unlink (const char* path)
    {
      instrumentation::posix::file_system::unlink (this, path);

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(\"%s\")\n", __func__, path);
#endif

      if (path == nullptr)
        {
          errno = EFAULT;
          instrumentation::posix::file_system::unlink_retval (this, -1);
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          instrumentation::posix::file_system::unlink_retval (this, -1);
          return -1;
        }

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          instrumentation::posix::file_system::unlink_retval (this, -1);
          return -1;
        }

      errno = 0;

      // Execute the implementation specific code.
      int ret = impl ().do_unlink (path);

      instrumentation::posix::file_system::unlink_retval (this, ret);
      return ret;
    }

    // http://pubs.opengroup.org/onlinepubs/9699919799/functions/utime.html
    int
    file_system::utime (const char* path, const /* struct */ utimbuf* times)
    {
      instrumentation::posix::file_system::utime (this, path, times);

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(\"%s\", %p)\n", __func__, path, times);
#endif

      if ((path == nullptr) || (times == nullptr))
        {
          errno = EFAULT;
          instrumentation::posix::file_system::utime_retval (this, -1);
          return -1;
        }

      if (*path == '\0')
        {
          errno = ENOENT;
          instrumentation::posix::file_system::utime_retval (this, -1);
          return -1;
        }

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          instrumentation::posix::file_system::utime_retval (this, -1);
          return -1;
        }

      errno = 0;

      /* struct */ utimbuf tmp;
      int ret;
      if (times == nullptr)
        {
          // If times is a null pointer, the access and modification times
          // of the file shall be set to the current time.
          tmp.actime = time (nullptr);
          tmp.modtime = tmp.actime;
          ret = impl ().do_utime (path, &tmp);
        }
      else
        {
          // Execute the implementation specific code.
          ret = impl ().do_utime (path, times);
        }

      instrumentation::posix::file_system::utime_retval (this, ret);
      return ret;
    }

    // http://pubs.opengroup.org/onlinepubs/9699919799/functions/fstatvfs.html
    int
    file_system::statvfs (struct statvfs* buf)
    {
      instrumentation::posix::file_system::statvfs (this, buf);

#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("file_system::%s(%p)\n", __func__, buf);
#endif

      if (!device ().is_opened ())
        {
          errno = EBADF; // Not opened.
          instrumentation::posix::file_system::statvfs_retval (this, -1);
          return -1;
        }

      int ret = impl ().do_statvfs (buf);

      instrumentation::posix::file_system::statvfs_retval (this, ret);
      return ret;
    }
    // TODO: check if the file system should keep a static current path for
    // relative paths.

    // http://pubs.opengroup.org/onlinepubs/9699919799/functions/chdir.html
    // ------------------------------------------------------------------------

    // ========================================================================

    file_system_impl::file_system_impl (block_device& device)
        : device_ (device)
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

    // ========================================================================
  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
