/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2018 Liviu Ionescu.
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

#include <cmsis-plus/posix-io/device.h>
#include <cmsis-plus/posix/sys/ioctl.h>

#include <cstring>
#include <cassert>
#include <cerrno>
#include <cstdarg>
#include <unistd.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ========================================================================

    device::device (device_impl& impl, type t, const char* name) :
        io
          { impl, t }, //
        name_ (name)
    {
#if defined(OS_TRACE_POSIX_IO_DEVICE)
      trace::printf ("device::%s(\"%s\")=%p\n", __func__, name_, this);
#endif
    }

    device::~device ()
    {
#if defined(OS_TRACE_POSIX_IO_DEVICE)
      trace::printf ("device::%s() @%p\n", __func__, this);
#endif

      registry_links_.unlink ();

      name_ = nullptr;
    }

    // ------------------------------------------------------------------------

    int
    device::open (const char* path, int oflag, ...)
    {
      // Forward to the variadic version of the function.
      std::va_list args;
      va_start(args, oflag);
      int ret = vopen (path, oflag, args);
      va_end(args);

      return ret;
    }

    int
    device::vopen (const char* path, int oflag, std::va_list args)
    {
#if defined(OS_TRACE_POSIX_IO_DEVICE)
      trace::printf ("device::%s(\"%s\") @%p\n", __func__, path ? path : "",
                     this);
#endif

      errno = 0;

      int ret = 0;
      if (impl ().open_count_ == 0)
        {
          // If so, use the implementation to open the device.
          ret = impl ().do_vopen (path, oflag, args);
          if (ret < 0)
            {
              // Open failed.
              return -1;
            }

          auto iop = alloc_file_descriptor ();
          if (iop == nullptr)
            {
              return -1;
            }

        }
      ++(impl ().open_count_);
      ret = file_descriptor ();
#if defined(OS_TRACE_POSIX_IO_DEVICE)
      trace::printf ("device::%s(\"%s\")=%p fd=%d\n", __func__,
                     path ? path : "", this, ret);
#endif

      return ret;
    }

    int
    device::close (void)
    {
#if defined(OS_TRACE_POSIX_IO_DEVICE)
      trace::printf ("device::%s() @%p\n", __func__, this);
#endif

      errno = 0;

      int ret = 0;
      if (impl ().open_count_ == 1)
        {
          ret = io::close ();
        }

      if (impl ().open_count_ > 0)
        {
          // Must be after close(), to keep do_is_open() true.
          --(impl ().open_count_);
        }

      return ret;
    }

    int
    device::ioctl (int request, ...)
    {
      // Forward to the variadic version of the function.
      std::va_list args;
      va_start(args, request);
      int ret = vioctl (request, args);
      va_end(args);

      return ret;
    }

    int
    device::vioctl (int request, std::va_list args)
    {
#if defined(OS_TRACE_POSIX_IO_DEVICE)
      trace::printf ("device::%s(%d) @%p\n", __func__, request, this);
#endif

      errno = 0;

      return impl ().do_vioctl (request, args);
    }

    void
    device::sync (void)
    {
#if defined(OS_TRACE_POSIX_IO_DEVICE)
      trace::printf ("device::%s() @%p\n", __func__, this);
#endif

      impl ().do_sync ();
    }

    // ------------------------------------------------------------------------

    bool
    device::match_name (const char* name) const
    {
      assert(name != nullptr);
      assert(name_ != nullptr);

      return (std::strcmp (name, name_) == 0);
    }

    // ========================================================================

    device_impl::device_impl (void)
    {
#if defined(OS_TRACE_POSIX_IO_DEVICE)
      trace::printf ("device_impl::%s()=%p\n", __func__, this);
#endif
    }

    device_impl::~device_impl ()
    {
#if defined(OS_TRACE_POSIX_IO_DEVICE)
      trace::printf ("device_impl::%s() @%p\n", __func__, this);
#endif
    }

    // ----------------------------------------------------------------------

    bool
    device_impl::do_is_opened (void)
    {
      return (open_count_ > 0);
    }

  // ========================================================================

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
