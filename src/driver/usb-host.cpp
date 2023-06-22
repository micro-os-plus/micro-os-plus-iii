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
 * The code is inspired by ARM CMSIS Driver_USBH.h file, v2.01,
 * and tries to remain functionally close to the CMSIS specifications.
 */

#include <cmsis-plus/driver/usb-host.h>
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

      Host::Host () noexcept
      {
        trace::printf ("%s() %p\n", __func__, this);

        cb_port_func_ = nullptr;
        cb_port_object_ = nullptr;

        cb_pipe_func_ = nullptr;
        cb_pipe_object_ = nullptr;
      }

      Host::~Host () noexcept
      {
        trace::printf ("%s() %p\n", __func__, this);
      }

      void
      Host::register_port_callback (host::signal_port_event_t cb_func,
                                    const void* cb_object) noexcept
      {
        cb_port_func_ = cb_func;
        cb_port_object_ = cb_object;
      }

      void
      Host::register_pipe_callback (host::signal_pipe_event_t cb_func,
                                    const void* cb_object) noexcept
      {
        cb_pipe_func_ = cb_func;
        cb_pipe_object_ = cb_object;
      }

      // ----------------------------------------------------------------------

      return_t
      Host::transfer (pipe_t pipe, uint32_t packet, uint8_t* data,
                      std::size_t num) noexcept
      {
        assert (data != nullptr);
        if (num == 0)
          {
            return RETURN_OK;
          }
        return do_transfer (pipe, packet, data, num);
      }

      // ----------------------------------------------------------------------

      void
      Host::signal_port_event (port_t port, event_t event) noexcept
      {
        if (cb_port_func_ != nullptr)
          {
            // Forward event to registered callback.
            cb_port_func_ (cb_port_object_, port, event);
          }
      }

      void
      Host::signal_pipe_event (pipe_t pipe, event_t event) noexcept
      {
        if (cb_pipe_func_ != nullptr)
          {
            // Forward event to registered callback.
            cb_pipe_func_ (cb_pipe_object_, pipe, event);
          }
      }

    } /* namespace usb */
  } /* namespace driver */
} /* namespace os */

// ----------------------------------------------------------------------------
