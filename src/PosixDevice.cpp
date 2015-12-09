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

#include "posix-io/PosixDevice.h"
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <cassert>

namespace os
{

  // ----------------------------------------------------------------------------

  PosixDevice* PosixDevice::registryArray[OS_INTEGER_POSIX_DEVICE_ARRAY_SIZE];

  PosixDevice::PosixDevice (const char* deviceName)
  {
    fDeviceName = deviceName;
    registerDevice ();
  }

  PosixDevice::~PosixDevice ()
  {
    deRegisterDevice ();
  }

  void
  PosixDevice::registerDevice (void)
  {
    bool found = false;
    for (std::size_t i = 0; i < getRegistrySize (); ++i)
      {
        if (registryArray[i] == nullptr)
          {
            registryArray[i] = this;
            found = true;
          }
#if defined(DEBUG)
        // Validate the device name by checking duplicates.
        if (std::strcmp (fDeviceName, registryArray[i]->fDeviceName) == 0)
          {
            const char* msg = "Duplicate PosixDevice name. Abort.\n";
#if defined(OS_INCLUDE_TRACE_PRINTF)
            trace_printf(msg);
#else
            ::write (2, msg, strlen (msg));
#endif
            abort ();
          }
#endif
      }

    if (found)
      {
        return;
      }

    // TODO: call trace_printf() from the separate package, when available.
    const char* msg = "Max number of PosixDevices reached. Abort.\n";
#if defined(OS_INCLUDE_TRACE_PRINTF)
    trace_printf(msg);
#else
    ::write (2, msg, strlen (msg));
#endif
    abort ();
  }

  void
  PosixDevice::deRegisterDevice (void)
  {
    for (std::size_t i = 0; i < getRegistrySize (); ++i)
      {
        if (registryArray[i] == this)
          {
            registryArray[i] = nullptr;
            return;
          }
      }

    // Not found... It would be good to tell.
  }

  PosixIo*
  PosixDevice::identifyPosixDevice (const char* path)
  {
    assert(path != nullptr);

    const char* prefix = PosixDevice::getDevicePrefix ();
    if (std::strncmp (prefix, path, std::strlen (prefix)) != 0)
      {
        // The device prefix does not match, not a device.
        return nullptr;
      }

    // The prefix was identified; try to identify the rest of the path
    const char* name = path + std::strlen (prefix);
    for (std::size_t i = 0; i < getRegistrySize (); ++i)
      {
        if (std::strcmp (name, registryArray[i]->fDeviceName) == 0)
          {
            return registryArray[i];
          }
      }

    return nullptr;
  }

} // namespace os

// ----------------------------------------------------------------------------
