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

#include <cmsis-plus/posix-io/directory.h>
#include <cmsis-plus/posix-io/file.h>
#include <cmsis-plus/posix-io/file-system.h>
#include <cmsis-plus/posix-io/io.h>
#include <cmsis-plus/posix-io/pool.h>
#include <cmsis-plus/posix-io/device-block.h>
#include <cmsis-plus/diag/trace.h>

#include <cerrno>
#include <cassert>
#include <cstring>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    /**
     * @cond ignore
     */

    file_system::mounted_list file_system::mounted_list__;

    class file_system* file_system::mounted_root__;

    /**
     * @endcond
     */

    // ------------------------------------------------------------------------
    int
    mkdir (const char* path, mode_t mode)
    {
      os::trace::printf ("%s(\"%s\", %u)\n", __func__, path, mode);

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

      errno = 0;

      // Execute the implementation specific code.
      return fs->do_mkdir (adjusted_path, mode);
    }

    int
    rmdir (const char* path)
    {
      os::trace::printf ("%s(\"%s\")\n", __func__, path);

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

      errno = 0;

      // Execute the implementation specific code.
      return fs->do_rmdir (adjusted_path);
    }

    void
    sync (void)
    {
      os::trace::printf ("%s()\n", __func__);

      errno = 0;

      // Enumerate all mounted file systems and sync them.
      for (auto&& fs : file_system::mounted_list__)
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
      os::trace::printf ("%s(\"%s\", %u)\n", __func__, path, mode);

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
      os::trace::printf ("%s(\"%s\", %p)\n", __func__, path, buf);

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
      os::trace::printf ("%s(\"%s\", %u)\n", __func__, path, length);

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
      os::trace::printf ("%s(\"%s\",\"%s\")\n", __func__, existing, _new);

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
      os::trace::printf ("%s(\"%s\")\n", __func__, path);

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
    utime (const char* path, const struct utimbuf* times)
    {
      os::trace::printf ("%s(\"%s\", %p)\n", __func__, path, times);

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

    // ------------------------------------------------------------------------

    file_system::file_system (device_block& device, pool* files_pool,
                              pool* dirs_pool) :
        block_device_ (device)
    {
      os::trace::printf ("file_system::%s()=%p\n", __func__, this);

      files_pool_ = files_pool;
      dirs_pool_ = dirs_pool;
    }

    file_system::~file_system ()
    {
      trace::printf ("file_system::%s() @%p\n", __func__, this);
    }

    // ------------------------------------------------------------------------

    int
    file_system::mount (const char* path, unsigned int flags)
    {
      if (mounted_path_ != nullptr)
        {
          // File system already mounted.
          errno = EBUSY;
          return -1;
        }

      if (path != nullptr)
        {
          for (auto&& fs : mounted_list__)
            {
              // Validate the device name by checking duplicates.
              if (std::strcmp (path, fs.mounted_path_) == 0)
                {
                  os::trace::printf ("Path \"%s\" already mounted.", path);

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
      int ret = do_mount (flags);
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

      this->sync ();
      int ret = do_umount (flags);
      return ret;
    }

    file_system*
    file_system::identify_mounted (const char** path1, const char** path2)
    {
      assert(path1 != nullptr);
      assert(*path1 != nullptr);

      for (auto&& fs : mounted_list__)
        {
          auto len = std::strlen (fs.mounted_path_);

          // Check if path1 starts with the mounted path.
          if (std::strncmp (fs.mounted_path_, *path1, len) == 0)
            {
              // If so, adjust paths to skip over prefix, but keep '/'.
              *path1 = (*path1 + len - 1);

              if ((path2 != nullptr) && (*path2 != nullptr))
                {
                  *path2 = (*path2 + len - 1);
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
      // Execute the file specific implementation code.
      file* f = do_vopen (path, oflag, args);
      if (f != nullptr)
        {
          // If successful, allocate a file descriptor.
          f->alloc_file_descriptor ();
        }

      return f;
    }

    directory*
    file_system::opendir (const char* dirpath)
    {
      // Execute the dir specific implementation code.
      directory* d = do_opendir (dirpath);

      return d;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    int
    file_system::chmod (const char* path, mode_t mode)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_chmod (path, mode);
    }

    int
    file_system::stat (const char* path, struct stat* buf)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_stat (path, buf);
    }

    int
    file_system::truncate (const char* path, off_t length)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_truncate (path, length);
    }

    int
    file_system::rename (const char* existing, const char* _new)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_rename (existing, _new);
    }

    int
    file_system::unlink (const char* path)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_unlink (path);
    }

    // http://pubs.opengroup.org/onlinepubs/9699919799/functions/utime.html
    int
    file_system::utime (const char* path, const struct utimbuf* times)
    {
      errno = 0;

      struct utimbuf tmp;
      if (times == nullptr)
        {
          // If times is a null pointer, the access and modification times
          // of the file shall be set to the current time.
          tmp.actime = time (nullptr);
          tmp.modtime = tmp.actime;
          return do_utime (path, &tmp);
        }
      else
        {
          // Execute the implementation specific code.
          return do_utime (path, times);
        }
    }

    void
    file_system::sync (void)
    {
      trace::printf ("file_system::%s() @%p\n", __func__, this);

      do_sync ();
    }

    // http://pubs.opengroup.org/onlinepubs/9699919799/functions/chdir.html
    // ------------------------------------------------------------------------

    const
    char*
    file_system::adjust_path (const char* path)
    {
      return path;
    }

    // ------------------------------------------------------------------------

    int
    file_system::do_chmod (const char* path, mode_t mode)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    file_system::do_stat (const char* path, struct stat* buf)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    file_system::do_truncate (const char* path, off_t length)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    file_system::do_rename (const char* existing, const char* _new)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    file_system::do_unlink (const char* path)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    file_system::do_utime (const char* path, const struct utimbuf* times)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    file_system::do_mkdir (const char* path, mode_t mode)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    file_system::do_rmdir (const char* path)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    void
    file_system::do_sync (void)
    {
      errno = ENOSYS; // Not implemented
    }

    int
    file_system::do_mount (unsigned int flags)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    file_system::do_umount (unsigned int flags)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

#pragma GCC diagnostic pop

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
