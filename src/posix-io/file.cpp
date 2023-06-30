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

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/posix-io/file.h>
#include <cmsis-plus/posix-io/file-system.h>

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

    file::file (file_impl& impl) :
        io
          { impl, type::file }
    {
#if defined(OS_TRACE_POSIX_IO_FILE)
      trace::printf ("file::%s()=%p\n", __func__, this);
#endif
    }

    file::~file ()
    {
#if defined(OS_TRACE_POSIX_IO_FILE)
      trace::printf ("file::%s() @%p\n", __func__, this);
#endif
    }

    // ------------------------------------------------------------------------

    int
    file::close (void)
    {
#if defined(OS_TRACE_POSIX_IO_FILE)
      trace::printf ("file::%s() @%p\n", __func__, this);
#endif

      int ret = io::close ();

      // Note: the constructor is not called here.

      // Link the file object to a list kept by the file system.
      // It will be deallocated at the next open.
      get_file_system ().add_deferred_file (this);

      return ret;
    }

    int
    file::ftruncate (off_t length)
    {
#if defined(OS_TRACE_POSIX_IO_FILE)
      trace::printf ("file::%s(%u) @%p\n", __func__, length, this);
#endif

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
#if defined(OS_TRACE_POSIX_IO_FILE)
      trace::printf ("file::%s() @%p\n", __func__, this);
#endif

      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_fsync ();
    }

    int
    file::fstatvfs (struct statvfs *buf)
    {
#if defined(OS_TRACE_POSIX_IO_FILE)
      trace::printf ("file::%s(%p) @%p\n", __func__, buf, this);
#endif

      errno = 0;

      // Execute the file system code. Might be locked there.
      return get_file_system ().statvfs (buf);
    }

    // ========================================================================

    file_impl::file_impl (/* class */ file_system& fs) :
        file_system_ (fs)
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

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
