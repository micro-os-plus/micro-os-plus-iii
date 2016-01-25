/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 * Copyright (c) 2013-2014 ARM Ltd.
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

/*
 * This file is part of the CMSIS++ proposal, intended as a CMSIS
 * replacement for C++ applications.
 *
 * The code is inspired by ARM CMSIS Driver_USBD.h file, v2.01,
 * and tries to remain functionally close to the CMSIS specifications.
 */

#include <cmsis-plus/drivers/usb-device.h>
#include <cassert>

// ----------------------------------------------------------------------------

namespace os
{
  namespace cmsis
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
          assert(buf != nullptr);
          return do_read_setup_packet (buf);
        }

        return_t
        Device::transfer (endpoint_t ep_addr, uint8_t* data, std::size_t num) noexcept
        {
          assert(data != nullptr);
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
  } /* namespace cmsis */
} /* namespace os */

// ----------------------------------------------------------------------------
