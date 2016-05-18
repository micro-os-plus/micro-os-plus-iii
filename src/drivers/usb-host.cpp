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
