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

#include "posix-io/PosixDevicesManager.h"
#include "posix-io/PosixDevice.h"
#include <cassert>
#include <cstring>

namespace os
{

  // --------------------------------------------------------------------------

  size_t PosixDevicesManager::sfSize;

  PosixDevice** PosixDevicesManager::sfRegistryArray;

  // --------------------------------------------------------------------------

  PosixDevicesManager::PosixDevicesManager (size_t size)
  {
    assert(size > 0);

    sfSize = size;
    sfRegistryArray = new PosixDevice*[size];

    for (std::size_t i = 0; i < getSize (); ++i)
      {
        sfRegistryArray[i] = nullptr;
      }

  }

  PosixDevicesManager::~PosixDevicesManager ()
  {
    delete sfRegistryArray;
    sfSize = 0;
  }

  // --------------------------------------------------------------------------

  void
  PosixDevicesManager::registerDevice (PosixDevice* device)
  {
    for (std::size_t i = 0; i < sfSize; ++i)
      {
        if (sfRegistryArray[i] == nullptr)
          {
            sfRegistryArray[i] = device;
            return;
          }
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
  PosixDevicesManager::deRegisterDevice (PosixDevice* device)
  {
    for (std::size_t i = 0; i < sfSize; ++i)
      {
        if (sfRegistryArray[i] == device)
          {
            sfRegistryArray[i] = nullptr;
            return;
          }
      }

    // Not found... It would be good to tell.
  }

  PosixDevice*
  PosixDevicesManager::identifyPosixDevice (const char* path)
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
    for (std::size_t i = 0; i < sfSize; ++i)
      {
        if (sfRegistryArray[i] != nullptr
            && sfRegistryArray[i]->matchName (name))
          {
            return sfRegistryArray[i];
          }
      }

    return nullptr;
  }

} /* namespace os */

// ----------------------------------------------------------------------------
