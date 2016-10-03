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

#ifndef CMSIS_PLUS_POSIX_IO_DEVICE_CHAR_REGISTRY_H_
#define CMSIS_PLUS_POSIX_IO_DEVICE_CHAR_REGISTRY_H_

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

    class device_char;

    // ------------------------------------------------------------------------

    class device_char_registry
    {
    public:

      device_char_registry (std::size_t size);
      device_char_registry (const device_char_registry&) = delete;

      ~device_char_registry ();

      // ----------------------------------------------------------------------

      static void
      add (device_char* device);

      static void
      remove (device_char* device);

      static device_char*
      identify_device (const char* path);

      static std::size_t
      size (void);

      static device_char*
      device (std::size_t index);

      // ----------------------------------------------------------------------

    private:

      static std::size_t size__;

      static device_char** registry_array__;
    };
  } /* namespace posix */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    inline std::size_t
    device_char_registry::size (void)
    {
      return size__;
    }

    inline device_char*
    device_char_registry::device (std::size_t index)
    {
      assert (index < size__);
      return registry_array__[index];
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_DEVICE_CHAR_REGISTRY_H_ */
