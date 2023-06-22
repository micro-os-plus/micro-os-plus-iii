/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016-2023 Liviu Ionescu. All rights reserved.
 * Copyright (c) 2013-2014 ARM Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

/*
 * The code is inspired by ARM CMSIS Driver_USBD.h file, v2.01,
 * and tries to remain functionally close to the CMSIS specifications.
 */

#include <cmsis-plus/driver/usb-device.h>
#include <cmsis-plus/diag/trace.h>
#include <cassert>

// ----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

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
        trace::printf ("%s() %p\n", __func__, this);

        cb_device_func_ = nullptr;
        cb_device_object_ = nullptr;

        cb_endpoint_func_ = nullptr;
        cb_endpoint_object_ = nullptr;
      }

      Device::~Device () noexcept
      {
        trace::printf ("%s() %p\n", __func__, this);
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
