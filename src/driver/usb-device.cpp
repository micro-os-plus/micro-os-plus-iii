/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 * Copyright (c) 2013-2014 ARM Ltd.
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

/*
 * This file is part of the CMSIS++ proposal, intended as a CMSIS
 * replacement for C++ applications.
 *
 * The code is inspired by ARM CMSIS Driver_USBD.h file, v2.01,
 * and tries to remain functionally close to the CMSIS specifications.
 */

#include <cmsis-plus/driver/usb-device.h>
#include <cassert>

// ----------------------------------------------------------------------------

namespace os
{
  namespace driver
  {
    namespace usb
    {
      // ----------------------------------------------------------------------

      Device::Device () noexcept
      {
        cb_device_func_ = nullptr;
        cb_device_object_ = nullptr;

        cb_endpoint_func_ = nullptr;
        cb_endpoint_object_ = nullptr;
      }

      Device::~Device () noexcept
      {
        ;
      }

      void
      Device::register_device_callback (device::signal_device_event_t cb_func,
                                        const void* cb_object) noexcept
      {
        cb_device_func_ = cb_func;
        cb_device_object_ = cb_object;
      }

      void
      Device::register_endpoint_callback (
          device::signal_endpoint_event_t cb_func, const void* cb_object) noexcept
      {
        cb_endpoint_func_ = cb_func;
        cb_endpoint_object_ = cb_object;
      }

      // ----------------------------------------------------------------------

      return_t
      Device::read_setup_packet (uint8_t* buf) noexcept
      {
        assert (buf != nullptr);
        return do_read_setup_packet (buf);
      }

      return_t
      Device::transfer (endpoint_t ep_addr, uint8_t* data, std::size_t num) noexcept
      {
        assert (data != nullptr);
        if (num == 0)
          {
            return RETURN_OK;
          }
        return do_transfer (ep_addr, data, num);
      }

      // ----------------------------------------------------------------------

      void
      Device::signal_device_event (event_t event) noexcept
      {
        if (cb_device_func_ != nullptr)
          {
            // Forward event to registered callback.
            cb_device_func_ (cb_device_object_, event);
          }
      }

      void
      Device::signal_endpoint_event (endpoint_t ep_addr, event_t event) noexcept
      {
        if (cb_endpoint_func_ != nullptr)
          {
            // Forward event to registered callback.
            cb_endpoint_func_ (cb_endpoint_object_, ep_addr, event);
          }
      }

    } /* namespace usb */
  } /* namespace driver */
} /* namespace os */

// ----------------------------------------------------------------------------
