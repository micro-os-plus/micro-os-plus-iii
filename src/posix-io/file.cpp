/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2015-2025 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can be
 * obtained from https://opensource.org/licenses/mit.
 */

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/posix-io/file.h>
#include <cmsis-plus/posix-io/file-system.h>

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/diag/instrumentation.h>

#include <cmsis-plus/diag/trace.h>

#include <cerrno>

// ----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ========================================================================

    file::file (file_impl& impl) : io{ impl, type::file }
    {
      instrumentation::posix::file::create (
          this, static_cast<unsigned int> (type::file));

#if defined(OS_TRACE_POSIX_IO_FILE)
      trace::printf ("file::%s()=%p\n", __func__, this);
#endif
    }

    file::~file ()
    {
      instrumentation::posix::file::destroy (this);

#if defined(OS_TRACE_POSIX_IO_FILE)
      trace::printf ("file::%s() @%p\n", __func__, this);
#endif
    }

    // ------------------------------------------------------------------------

    int
    file::close (void)
    {
      instrumentation::posix::file::close (this);

#if defined(OS_TRACE_POSIX_IO_FILE)
      trace::printf ("file::%s() @%p\n", __func__, this);
#endif

      int ret = io::close ();

      // Note: the constructor is not called here.

      // Link the file object to a list kept by the file system.
      // It will be deallocated at the next open.
      get_file_system ().add_deferred_file (this);

      instrumentation::posix::file::close_retval (this, ret);
      return ret;
    }

    int
    file::ftruncate (off_t length)
    {
      instrumentation::posix::file::ftruncate (this, length);

#if defined(OS_TRACE_POSIX_IO_FILE)
      trace::printf ("file::%s(%u) @%p\n", __func__, length, this);
#endif

      if (length < 0)
        {
          errno = EINVAL;

          instrumentation::posix::file::ftruncate_retval (this, -1);
          return -1;
        }

      errno = 0;

      // Execute the implementation specific code.
      int ret = impl ().do_ftruncate (length);

      instrumentation::posix::file::ftruncate_retval (this, ret);
      return ret;
    }

    int
    file::fsync (void)
    {
      instrumentation::posix::file::fsync (this);

#if defined(OS_TRACE_POSIX_IO_FILE)
      trace::printf ("file::%s() @%p\n", __func__, this);
#endif

      errno = 0;

      // Execute the implementation specific code.
      int ret = impl ().do_fsync ();

      instrumentation::posix::file::fsync_retval (this, ret);
      return ret;
    }

    int
    file::fstatvfs (struct statvfs* buf)
    {
      instrumentation::posix::file::fstatvfs (this, buf);

#if defined(OS_TRACE_POSIX_IO_FILE)
      trace::printf ("file::%s(%p) @%p\n", __func__, buf, this);
#endif

      errno = 0;

      // Execute the file system code. Might be locked there.
      int ret = get_file_system ().statvfs (buf);

      instrumentation::posix::file::fstatvfs_retval (this, ret);
      return ret;
    }

    // ========================================================================

    file_impl::file_impl (/* class */ file_system& fs) : file_system_ (fs)
    {
#if defined(OS_TRACE_POSIX_IO_FILE)
      trace::printf ("file_impl::%s()=%p\n", __func__, this);
#endif
    }

    file_impl::~file_impl ()
    {
#if defined(OS_TRACE_POSIX_IO_FILE)
      trace::printf ("file_impl::%s() @%p\n", __func__, this);
#endif
    }

    // ------------------------------------------------------------------------

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

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
