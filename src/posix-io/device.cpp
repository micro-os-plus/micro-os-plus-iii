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

#include <cmsis-plus/diag/trace.h>

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
    // ------------------------------------------------------------------------

    device::device (type t, const char* name) :
        io (t), //
        name_ (name)
    {
      trace::printf ("device::%s(\"%s\")=%p\n", __func__, name_, this);
    }

    device::~device ()
    {
      trace::printf ("device::%s() @%p %s\n", __func__, this, name_);

      registry_links_.unlink ();

      name_ = nullptr;
    }

    // ------------------------------------------------------------------------

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
      return do_vioctl (request, args);
    }

    // ------------------------------------------------------------------------

    bool
    device::match_name (const char* name) const
    {
      assert(name != nullptr);
      assert(name_ != nullptr);

      return (std::strcmp (name, name_) == 0);
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    int
    device::do_vioctl (int request, std::va_list args)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

#pragma GCC diagnostic pop

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
