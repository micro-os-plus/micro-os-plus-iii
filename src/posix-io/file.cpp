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

#include <cmsis-plus/diag/trace.h>

#include <cerrno>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ========================================================================

    file::file (file_impl& impl, class file_system& fs) :
        io
          { impl, type::file }, //
        file_system_ (&fs)
    {
      trace::printf ("file::%s()=%p\n", __func__, this);
    }

    file::~file ()
    {
      trace::printf ("file::%s() @%p\n", __func__, this);

      file_system_ = nullptr;
    }

    // ------------------------------------------------------------------------

    int
    file::close (void)
    {
      trace::printf ("file::%s() @%p\n", __func__, this);

      int ret = io::close ();

      // Note: the constructor is not called here.

      // Link the file object to a list kept by the file system.
      // It will be deallocated at the next open.
      file_system ()->add_deferred_file (this);

      return ret;
    }

    int
    file::ftruncate (off_t length)
    {
      trace::printf ("file::%s(%u) @%p\n", __func__, length, this);

      if (length < 0)
        {
          errno = EINVAL;
          return -1;
        }

      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_ftruncate (length);
    }

    int
    file::fsync (void)
    {
      trace::printf ("file::%s() @%p\n", __func__, this);

      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_fsync ();
    }

    // ------------------------------------------------------------------------

#if 0

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

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

#endif

    // ========================================================================

    file_impl::file_impl (file& self) :
        io_impl
          { self }
    {
      trace::printf ("file_impl::%s()=%p\n", __func__, this);
    }

    file_impl::~file_impl ()
    {
      trace::printf ("file_impl::%s() @%p\n", __func__, this);
    }

    // ------------------------------------------------------------------------

#if 0
    void
    file_impl::do_release (void)
      {
        // The `file` object is free, return it to the pool.
        auto fs = file_system_;
        if (fs != nullptr)
          {
#if 0
            auto pool = fs->files_pool ();
            if (pool != nullptr)
              {
                pool->release (this);
              }
            // Clear the file_system pointer, to avoid entering again.
#endif
            file_system_ = nullptr;
          }
      }
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    int
    file_impl::do_ftruncate (off_t length)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

#pragma GCC diagnostic pop

    int
    file_impl::do_fsync (void)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

  // ========================================================================
  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
