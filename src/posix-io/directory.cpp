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

#include <cmsis-plus/diag/trace.h>

#include <cerrno>
#include <cassert>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ========================================================================

    directory::directory (directory_impl& impl) :
        impl_ (impl)
    {
#if defined(OS_TRACE_POSIX_IO_DIRECTORY)
      trace::printf ("directory::%s()=%p\n", __func__, this);
#endif
    }

    directory::~directory ()
    {
#if defined(OS_TRACE_POSIX_IO_DIRECTORY)
      trace::printf ("directory::%s() @%p\n", __func__, this);
#endif
    }

    // ------------------------------------------------------------------------

    struct dirent*
    directory::read (void)
    {
#if defined(OS_TRACE_POSIX_IO_DIRECTORY)
      trace::printf ("directory::%s() @%p\n", __func__, this);
#endif

      // assert(file_system_ != nullptr);

      // POSIX requires not to change errno when end of directory is
      // encountered. However, in this implementation, errno is
      // always cleared when entering system calls.
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_read ();
    }

    void
    directory::rewind (void)
    {
#if defined(OS_TRACE_POSIX_IO_DIRECTORY)
      trace::printf ("directory::%s() @%p\n", __func__, this);
#endif

      // assert(file_system_ != nullptr);

      // POSIX does not mention what to do with errno.
      errno = 0;

      // Execute the implementation specific code.
      impl ().do_rewind ();
    }

    int
    directory::close (void)
    {
#if defined(OS_TRACE_POSIX_IO_DIRECTORY)
      trace::printf ("directory::%s() @%p\n", __func__, this);
#endif

      // assert(file_system_ != nullptr);
      errno = 0;

      // Execute the implementation specific code.
      int ret = impl ().do_close ();

      // The file object will be deallocated at the next open.
      file_system ().add_deferred_directory (this);

      return ret;
    }

    // ========================================================================

    directory_impl::directory_impl (class file_system& fs) :
        file_system_ (fs)
    {
#if defined(OS_TRACE_POSIX_IO_DIRECTORY)
      trace::printf ("directory_impl::%s()=%p\n", __func__, this);
#endif
    }

    directory_impl::~directory_impl ()
    {
#if defined(OS_TRACE_POSIX_IO_DIRECTORY)
      trace::printf ("directory_impl::%s() @%p\n", __func__, this);
#endif
    }

  // ========================================================================

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
