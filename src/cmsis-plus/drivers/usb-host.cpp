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
 * The code is inspired by ARM CMSIS Driver_USBH.h file, v2.01,
 * and tries to remain functionally close to the CMSIS specifications.
 */

#include <cmsis-plus/drivers/usb-host.h>
#include <cassert>

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
          cb_port_func_ = nullptr;
          cb_port_object_ = nullptr;

          cb_pipe_func_ = nullptr;
          cb_pipe_object_ = nullptr;
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
          assert(data != nullptr);
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
