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
 * The code is inspired by ARM CMSIS Driver_USART.h file, v2.02,
 * and tries to remain functionally close to the CMSIS specifications.
 */

#include <cmsis-plus/drivers/serial.h>

#include <cassert>

// ----------------------------------------------------------------------------

namespace os
{
  namespace cmsis
  {
    namespace driver
    {
      // ----------------------------------------------------------------------

      Serial::Serial () noexcept
      {
        cb_func_ = nullptr;
        cb_object_ = nullptr;

        clean ();
      }

      // ----------------------------------------------------------------------

      void
      Serial::clean (void) noexcept
      {
        status_.rx_break = false;
        status_.rx_busy = false;
        status_.rx_framing_error = false;
        status_.rx_overflow = false;
        status_.rx_parity_error = false;
        status_.tx_busy = false;
        status_.tx_underflow = false;

        modem_status_.cts = false;
        modem_status_.dsr = false;
        modem_status_.dcd = false;
        modem_status_.ri = false;
      }

      // ----------------------------------------------------------------------

      void
      Serial::register_callback (signal_event_t cb_func, const void* cb_object) noexcept
      {
        cb_func_ = cb_func;
        cb_object_ = cb_object;
      }

      return_t
      Serial::send (const void* data, std::size_t num) noexcept
      {
        assert(data != nullptr);
        if (num == 0)
          {
            return RETURN_OK;
          }
        return do_send (data, num);
      }

      return_t
      Serial::receive (void* data, std::size_t num) noexcept
      {
        assert(data != nullptr);
        if (num == 0)
          {
            return RETURN_OK;
          }
        return do_receive (data, num);
      }

      return_t
      Serial::transfer (const void* data_out, void* data_in, std::size_t num) noexcept
      {
        assert(data_out != nullptr);
        assert(data_in != nullptr);
        if (num == 0)
          {
            return RETURN_OK;
          }
        return do_transfer (data_out, data_in, num);
      }

      // ----------------------------------------------------------------------

      void
      Serial::signal_event (event_t event) noexcept
      {
        if (cb_func_ != nullptr)
          {
            // Forward event to registered callback.
            cb_func_ (cb_object_, event);
          }
      }

    } /* namespace driver */
  } /* namespace cmsis */
} /* namespace os */
