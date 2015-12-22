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

#ifndef POSIX_IO_CHAR_DEVICES_MANAGER_H_
#define POSIX_IO_CHAR_DEVICES_MANAGER_H_

// ----------------------------------------------------------------------------

#include <cstddef>
#include <cassert>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class CharDevice;

    // ------------------------------------------------------------------------

    class CharDevicesRegistry
    {
    public:

      CharDevicesRegistry (std::size_t size);
      CharDevicesRegistry (const CharDevicesRegistry&) = delete;

      ~CharDevicesRegistry ();

      // ----------------------------------------------------------------------

      static void
      add (CharDevice* device);

      static void
      remove (CharDevice* device);

      static CharDevice*
      identifyDevice (const char* path);

      static std::size_t
      getSize (void);

      static CharDevice*
      getDevice (std::size_t index);

      // ----------------------------------------------------------------------

    private:

      static std::size_t sfSize;

      static CharDevice** sfRegistryArray;
    };

    // ------------------------------------------------------------------------

    inline std::size_t
    CharDevicesRegistry::getSize (void)
    {
      return sfSize;
    }

    inline CharDevice*
    CharDevicesRegistry::getDevice (std::size_t index)
    {
      assert(index < sfSize);
      return sfRegistryArray[index];
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* POSIX_IO_Char_DEVICES_MANAGER_H_ */
