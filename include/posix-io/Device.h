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

#ifndef POSIX_IO_DEVICE_H_
#define POSIX_IO_DEVICE_H_

// ----------------------------------------------------------------------------

#include <sys/types.h>
#include "posix-io/IO.h"

// ----------------------------------------------------------------------------

#if ! defined(OS_STRING_POSIX_DEVICE_PREFIX)
#define OS_STRING_POSIX_DEVICE_PREFIX "/dev/"
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class Device : public IO
    {
    public:

      Device (const char* name);
      Device (const Device&) = delete;

      virtual
      ~Device ();

      // ----------------------------------------------------------------------

      virtual int
      do_open (const char* path, int oflag, std::va_list args) = 0;

      virtual bool
      matchName (const char* name) const;

      const char*
      getName (void) const;

      static const char*
      getDevicePrefix (void);

    protected:

      const char* fName;

    };

    // ------------------------------------------------------------------------

    inline const char*
    Device::getDevicePrefix (void)
    {
      return OS_STRING_POSIX_DEVICE_PREFIX;
    }

    inline const char*
    Device::getName (void) const
    {
      return fName;
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* POSIX_IO_DEVICE_H_ */
