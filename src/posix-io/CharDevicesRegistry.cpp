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

#include <cmsis-plus/posix-io/CharDevicesRegistry.h>
#include <cmsis-plus/posix-io/CharDevice.h>
#include <cmsis-plus/diag/trace.h>

#include <cassert>
#include <cstring>
#include <cstdlib>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    std::size_t CharDevicesRegistry::sfSize;

    CharDevice** CharDevicesRegistry::sfRegistryArray;

    // ------------------------------------------------------------------------

    CharDevicesRegistry::CharDevicesRegistry (std::size_t size)
    {
      assert (size > 0);

      sfSize = size;
      sfRegistryArray = new CharDevice*[size];

      for (std::size_t i = 0; i < size; ++i)
        {
          sfRegistryArray[i] = nullptr;
        }
    }

    CharDevicesRegistry::~CharDevicesRegistry ()
    {
      delete[] sfRegistryArray;
      sfSize = 0;
    }

    // ------------------------------------------------------------------------

    void
    CharDevicesRegistry::add (CharDevice* device)
    {
#if defined(DEBUG)
      for (std::size_t i = 0; i < sfSize; ++i)
        {
          if (sfRegistryArray[i] == nullptr)
            {
              continue;
            }

          // Validate the device name by checking duplicates.
          if (std::strcmp (device->getName (), sfRegistryArray[i]->getName ())
              == 0)
            {
              trace_puts ("Duplicate Device name. Abort.");
              std::abort ();
            }

        }
#endif // DEBUG

      for (std::size_t i = 0; i < sfSize; ++i)
        {
          if (sfRegistryArray[i] == nullptr)
            {
              sfRegistryArray[i] = device;
              return;
            }
        }

      trace_puts ("Max number of Devices reached. Abort.");
      std::abort ();
    }

    void
    CharDevicesRegistry::remove (CharDevice* device)
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

    /**
     * return pointer to device or nullptr if not found.
     */
    CharDevice*
    CharDevicesRegistry::identifyDevice (const char* path)
    {
      assert (path != nullptr);

      auto prefix = CharDevice::getDevicePrefix ();
      if (std::strncmp (prefix, path, std::strlen (prefix)) != 0)
        {
          // The device prefix does not match, not a device.
          return nullptr;
        }

      // The prefix was identified; try to match the rest of the path.
      auto name = path + std::strlen (prefix);
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

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
