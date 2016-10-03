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

#include <cmsis-plus/posix-io/file.h>
#include <cmsis-plus/posix-io/file-system.h>
#include <cmsis-plus/posix-io/mount-manager.h>
#include <cmsis-plus/posix-io/pool.h>
#include <cerrno>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    file*
    file::open (const char* path, int oflag, ...)
    {
      // Forward to the variadic version of the function.
      std::va_list args;
      va_start (args, oflag);
      auto* const ret = vopen (path, oflag, args);
      va_end (args);

      return ret;
    }

    // ------------------------------------------------------------------------

    file::file () :
        io (type::file)
    {
      file_system_ = nullptr;
    }

    file::~file ()
    {
      file_system_ = nullptr;
    }

    // ------------------------------------------------------------------------

    void
    file::do_release (void)
    {
      // Files is free, return it to the pool.
      auto fs = file_system ();
      if (fs != nullptr)
        {
          auto pool = fs->files_pool ();
          if (pool != nullptr)
            {
              pool->release (this);
            }
          file_system (nullptr);
        }
    }

    // ------------------------------------------------------------------------

    off_t
    file::lseek (off_t offset, int whence)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_lseek (offset, whence);
    }

    int
    file::ftruncate (off_t length)
    {
      if (length < 0)
        {
          errno = EINVAL;
          return -1;
        }

      errno = 0;

      // Execute the implementation specific code.
      return do_ftruncate (length);
    }

    int
    file::fsync (void)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_fsync ();
    }

    // ------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    off_t
    file::do_lseek (off_t offset, int whence)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    file::do_ftruncate (off_t length)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

#pragma GCC diagnostic pop

    int
    file::do_fsync (void)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
