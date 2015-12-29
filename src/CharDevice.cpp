/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
 *
 * µOS++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 3.
 *
 * µOS++ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "posix-io/CharDevice.h"

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
