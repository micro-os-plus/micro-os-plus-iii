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
#include <cmsis-plus/posix-io/mount-manager.h>
#include <cmsis-plus/posix-io/pool.h>
#include <cerrno>
#include <cassert>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    int
    mkdir (const char* path, mode_t mode)
    {
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
      auto* const fs = mount_manager::identify_file_system (&adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          return -1;
        }

      assert (fs->device () != nullptr);
      errno = 0;

      // Execute the implementation specific code.
      return fs->do_mkdir (adjusted_path, mode);
    }

    int
    rmdir (const char* path)
    {
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
      auto* const fs = mount_manager::identify_file_system (&adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          return -1;
        }

      assert (fs->device () != nullptr);
      errno = 0;

      // Execute the implementation specific code.
      return fs->do_rmdir (adjusted_path);
    }

    void
    sync (void)
    {
      errno = 0;

      // Enumerate all mounted file systems and sync them.
      for (std::size_t i = 0; i < mount_manager::size (); ++i)
        {
          auto fs = mount_manager::get_file_system (i);
          if (fs != nullptr)
            {
              fs->do_sync ();
            }
        }
    }

    // ------------------------------------------------------------------------
    // Functions related to files, other than IO. The implementations is
    // specific to each file_system.

    int
    chmod (const char* path, mode_t mode)
    {
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
      auto* const fs = mount_manager::identify_file_system (&adjusted_path);

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
      auto* const fs = mount_manager::identify_file_system (&adjusted_path);

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
      auto* const fs = mount_manager::identify_file_system (&adjusted_path);

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
      auto* const fs = mount_manager::identify_file_system (&adjusted_existing,
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
      auto* const fs = mount_manager::identify_file_system (&adjusted_path);

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
      auto* const fs = mount_manager::identify_file_system (&adjusted_path);

      if (fs == nullptr)
        {
          errno = ENOENT;
          return -1;
        }

      return fs->utime (adjusted_path, times);
    }

    // ------------------------------------------------------------------------

    file_system::file_system (pool* files_pool, pool* dirs_pool)
    {
      files_pool_ = files_pool;
      dirs_pool_ = dirs_pool;
      block_device_ = nullptr;
    }

    file_system::~file_system ()
    {
      block_device_ = nullptr;
    }

    // ------------------------------------------------------------------------

    io*
    file_system::open (const char* path, int oflag, std::va_list args)
    {
      if (block_device_ == nullptr)
        {
          errno = EBADF;
          return nullptr;
        }

      // Get a file object from the pool.
      auto* const f = static_cast<file*> (files_pool_->acquire ());

      // Associate the file with this file system (used, for example,
      // to reach the pools at close).
      f->file_system (this);

      // Execute the file specific implementation code.
      f->do_vopen (path, oflag, args);

      return f;
    }

    directory*
    file_system::opendir (const char* dirpath)
    {
      if (block_device_ == nullptr)
        {
          errno = EBADF;
          return nullptr;
        }

      // Get a directory object from the pool.
      auto* const dir = static_cast<directory*> (dirs_pool_->acquire ());

      // Associate the dir with this file system (used, for example,
      // to reach the pools at close).
      dir->file_system (this);

      // Execute the dir specific implementation code.
      dir->do_vopen (dirpath);

      return dir;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    int
    file_system::chmod (const char* path, mode_t mode)
    {
      assert (block_device_ != nullptr);
      errno = 0;

      // Execute the implementation specific code.
      return do_chmod (path, mode);
    }

    int
    file_system::stat (const char* path, struct stat* buf)
    {
      assert (block_device_ != nullptr);
      errno = 0;

      // Execute the implementation specific code.
      return do_stat (path, buf);
    }

    int
    file_system::truncate (const char* path, off_t length)
    {
      assert (block_device_ != nullptr);
      errno = 0;

      // Execute the implementation specific code.
      return do_truncate (path, length);
    }

    int
    file_system::rename (const char* existing, const char* _new)
    {
      assert (block_device_ != nullptr);
      errno = 0;

      // Execute the implementation specific code.
      return do_rename (existing, _new);
    }

    int
    file_system::unlink (const char* path)
    {
      assert (block_device_ != nullptr);
      errno = 0;

      // Execute the implementation specific code.
      return do_unlink (path);
    }

    int
    file_system::utime (const char* path, const struct utimbuf* times)
    {
      assert (block_device_ != nullptr);
      errno = 0;

      // Execute the implementation specific code.
      return do_utime (path, times);
    }

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
    file_system::do_unmount (unsigned int flags)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

#pragma GCC diagnostic pop

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
