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

#include <cmsis-plus/posix-io/CharDevice.h>

#include <cstring>
#include <cassert>
#include <cerrno>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    CharDevice::CharDevice (const char* name)
    {
      fType = Type::DEVICE;
      fName = name;
    }

    CharDevice::~CharDevice ()
    {
      fName = nullptr;
    }

    // ------------------------------------------------------------------------

    int
    CharDevice::ioctl (int request, ...)
    {
      // Forward to the variadic version of the function.
      std::va_list args;
      va_start (args, request);
      int ret = vioctl (request, args);
      va_end (args);

      return ret;
    }

    int
    CharDevice::vioctl (int request, std::va_list args)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_vioctl (request, args);
    }

    // ------------------------------------------------------------------------

    bool
    CharDevice::matchName (const char* name) const
    {
      assert (name != nullptr);
      assert (fName != nullptr);

      return (std::strcmp (name, fName) == 0);
    }

    int
    CharDevice::do_isatty (void)
    {
      return 1; // Yes, it is a TTY
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    int
    CharDevice::do_vioctl (int request, std::va_list args)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

#pragma GCC diagnostic pop

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
