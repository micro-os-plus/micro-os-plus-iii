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

#include "posix-io/PosixDevicesRegistry.h"
#include "posix-io/PosixDevice.h"
#include <cassert>
#include <cstring>

namespace os
{

  // --------------------------------------------------------------------------

  size_t PosixDevicesRegistry::sfSize;

  PosixDevice** PosixDevicesRegistry::sfRegistryArray;

  // --------------------------------------------------------------------------

  PosixDevicesRegistry::PosixDevicesRegistry (size_t size)
  {
    assert(size > 0);

    sfSize = size;
    sfRegistryArray = new PosixDevice*[size];

    for (std::size_t i = 0; i < getSize (); ++i)
      {
        sfRegistryArray[i] = nullptr;
      }

  }

  PosixDevicesRegistry::~PosixDevicesRegistry ()
  {
    delete sfRegistryArray;
    sfSize = 0;
  }

  // --------------------------------------------------------------------------

  void
  PosixDevicesRegistry::add (PosixDevice* driver)
  {
    bool found = false;
    for (std::size_t i = 0; i < sfSize; ++i)
      {
        if (sfRegistryArray[i] == nullptr)
          {
            sfRegistryArray[i] = driver;
            found = true;
            continue;
          }

#if defined(DEBUG)
        // Validate the device name by checking duplicates.
        if (std::strcmp (driver->getName (), sfRegistryArray[i]->getName ())
            == 0)
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
  PosixDevicesRegistry::remove (PosixDevice* driver)
  {
    for (std::size_t i = 0; i < sfSize; ++i)
      {
        if (sfRegistryArray[i] == driver)
          {
            sfRegistryArray[i] = nullptr;
            return;
          }
      }

    // Not found... It would be good to tell.
  }

  /**
   * return pointer to device or nullptr if not found.
   */
  PosixDevice*
  PosixDevicesRegistry::identifyDevice (const char* path)
  {
    assert(path != nullptr);

    const char* prefix = PosixDevice::getDevicePrefix ();
    if (std::strncmp (prefix, path, std::strlen (prefix)) != 0)
      {
        // The device prefix does not match, not a device.
        return nullptr;
      }

    // The prefix was identified; try to match the rest of the path.
    const char* name = path + std::strlen (prefix);
    for (std::size_t i = 0; i < sfSize; ++i)
      {
        if (sfRegistryArray[i] != nullptr
            && sfRegistryArray[i]->matchName (name))
          {
            // Return the first device that matches the path.
            return sfRegistryArray[i];
          }
      }

    // Not a known device.
    return nullptr;
  }

} /* namespace os */

// ----------------------------------------------------------------------------
