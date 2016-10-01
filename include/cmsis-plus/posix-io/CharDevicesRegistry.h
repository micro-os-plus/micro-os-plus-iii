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

#ifndef CMSIS_PLUS_POSIX_IO_CHAR_DEVICES_REGISTRY_H_
#define CMSIS_PLUS_POSIX_IO_CHAR_DEVICES_REGISTRY_H_

#if defined(__cplusplus)

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
      assert (index < sfSize);
      return sfRegistryArray[index];
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_CHAR_DEVICES_REGISTRY_H_ */
