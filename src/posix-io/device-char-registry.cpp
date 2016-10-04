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

#include <cmsis-plus/posix-io/device-char-registry.h>
#include <cmsis-plus/posix-io/device-char.h>
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

    /**
     * @cond ignore
     */

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif

    // Initialised to 0 by BSS.
    device_char_registry::device_list device_char_registry::registry_list__;

#pragma GCC diagnostic pop

    /**
     * @endcond
     */

    // ------------------------------------------------------------------------
    void
    device_char_registry::link (device_char* device)
    {
#if defined(DEBUG)
      for (auto&& d : registry_list__)
        {
          // Validate the device name by checking duplicates.
          if (std::strcmp (device->name (), d.name ()) == 0)
            {
              trace_puts ("Duplicate char device name. Abort.");
              std::abort ();
            }
        }
#endif // DEBUG

      registry_list__.link (*device);
    }

    /**
     * return pointer to device or nullptr if not found.
     */
    device_char*
    device_char_registry::identify_device (const char* path)
    {
      assert (path != nullptr);

      auto prefix = device_char::device_prefix ();
      if (std::strncmp (prefix, path, std::strlen (prefix)) != 0)
        {
          // The device prefix does not match, not a device.
          return nullptr;
        }

      // The prefix was identified; try to match the rest of the path.
      auto name = path + std::strlen (prefix);

      for (auto&& p : registry_list__)
        {
          if (p.match_name (name))
            {
              return &p;
            }
        }

      // Not a known device.
      return nullptr;
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
