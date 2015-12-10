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

#ifndef POSIX_DEVICES_MANAGER_H_
#define POSIX_DEVICES_MANAGER_H_

#include <cstddef>
#include <cassert>

// ----------------------------------------------------------------------------

namespace os
{
  class PosixDevice;

  class PosixDevicesManager
  {
  public:

    PosixDevicesManager (size_t size);

    ~PosixDevicesManager ();

    // ------------------------------------------------------------------------

    static void
    registerDevice (PosixDevice* device);

    static void
    deRegisterDevice (PosixDevice* device);

    static PosixDevice*
    identifyPosixDevice (const char* path);

    static std::size_t
    getSize (void);

    static PosixDevice*
    getRegisteredDevice (std::size_t index);

    // ------------------------------------------------------------------------

  private:

    static size_t sfSize;

    static PosixDevice** sfRegistryArray;
  };

  // --------------------------------------------------------------------------

  inline std::size_t
  PosixDevicesManager::getSize (void)
  {
    return sfSize;
  }

  inline PosixDevice*
  PosixDevicesManager::getRegisteredDevice (std::size_t index)
  {
    assert(index < sfSize);
    return sfRegistryArray[index];
  }

} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* POSIX_DEVICES_MANAGER_H_ */
