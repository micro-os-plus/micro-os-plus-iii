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
#include <cmsis-plus/posix-io/file-system.h>
#include <cmsis-plus/posix-io/pool.h>
#include <cmsis-plus/diag/trace.h>

#include <cerrno>
#include <cassert>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    directory*
    opendir (const char* dirname)
    {
      if (dirname == nullptr)
        {
          errno = EFAULT;
          return nullptr;
        }

      if (*dirname == '\0')
        {
          errno = ENOENT;
          return nullptr;
        }

      errno = 0;

      auto adjusted_dirname = dirname;
      auto* const fs = os::posix::file_system::identify_mounted (
          &adjusted_dirname);

      // The manager will return null if there are no file systems
      // registered, no need to check this condition separately.
      if (fs == nullptr)
        {
          errno = EBADF;
          return nullptr;
        }

      // Use the file system implementation to open the directory, using
      // the adjusted path (mount point prefix removed).
      return fs->opendir (adjusted_dirname);
    }

    // ------------------------------------------------------------------------

    directory::directory (void)
    {
      os::trace::printf ("directory::%s()=%p\n", __func__, this);

      file_system_ = nullptr;
    }

    directory::~directory ()
    {
      os::trace::printf ("directory::%s() @%p\n", __func__, this);

      file_system_ = nullptr;
    }

    // ------------------------------------------------------------------------

    struct dirent*
    directory::read (void)
    {
      os::trace::printf ("directory::%s() @%p\n", __func__, this);

      assert(file_system_ != nullptr);

      // POSIX requires not to change errno when end of directory is
      // encountered. However, in this implementation, errno is
      // always cleared when entering system calls.
      errno = 0;

      // Execute the implementation specific code.
      return do_read ();
    }

    void
    directory::rewind (void)
    {
      os::trace::printf ("directory::%s() @%p\n", __func__, this);

      assert(file_system_ != nullptr);

      // POSIX does not mention what to do with errno.
      errno = 0;

      // Execute the implementation specific code.
      do_rewind ();
    }

    int
    directory::close (void)
    {
      os::trace::printf ("directory::%s() @%p\n", __func__, this);

      assert(file_system_ != nullptr);
      errno = 0;

      // Execute the implementation specific code.
      int ret = do_close ();
      auto* const pool = file_system_->dirs_pool ();
      if (pool != nullptr)
        {
          pool->release (this);
        }
      return ret;
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
