/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
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

#include <cmsis-plus/driver/usart-wrapper.h>
#include <Driver_USART.h>

#include <utility>
#include <stdio.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace driver
  {
    // ----------------------------------------------------------------------

    usart_wrapper::usart_wrapper (ARM_DRIVER_USART* driver,
                                  ARM_USART_SignalEvent_t c_cb_func) noexcept :
    driver_ (driver),
    c_cb_func_ (c_cb_func)
      {
        ;
      }

    usart_wrapper::~usart_wrapper () noexcept
    {
      driver_ = nullptr;
    }

    // ----------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"
#pragma GCC diagnostic ignored "-Wstrict-aliasing"

    const Version&
    usart_wrapper::do_get_version (void) noexcept
    {
      // Overwrite the C++ instance. Assume same layout.
      *(reinterpret_cast<ARM_DRIVER_VERSION*> (&version_)) =
          driver_->GetVersion ();
      return version_;
    }

    const serial::Capabilities&
    usart_wrapper::do_get_capabilities (void) noexcept
    {
      // Overwrite the C++ instance. Assume same layout.
      *(reinterpret_cast<ARM_USART_CAPABILITIES*> (&capa_)) =
          driver_->GetCapabilities ();
      return capa_;
    }

    serial::Status&
    usart_wrapper::do_get_status (void) noexcept
    {
      // Overwrite the C++ instance. Assume same layout.
      *(reinterpret_cast<ARM_USART_STATUS*> (&status_)) = driver_->GetStatus ();
      return status_;
    }

    serial::Modem_status&
    usart_wrapper::do_get_modem_status (void) noexcept
    {
      // Overwrite the C++ instance. Assume same layout.
      *(reinterpret_cast<ARM_USART_MODEM_STATUS*> (&modem_status_)) =
          driver_->GetModemStatus ();
      return modem_status_;
    }

#pragma GCC diagnostic pop

    return_t
    usart_wrapper::do_power (Power state) noexcept
    {
      return_t status;

      if (state == Power::full)
        {
          status = driver_->Initialize (c_cb_func_);
          if (status != ARM_DRIVER_OK)
            {
              return status;
            }
        }

      status = driver_->PowerControl (static_cast<ARM_POWER_STATE> (state));

      if (state == Power::off)
        {
          driver_->Uninitialize ();
        }

      return status;
    }

    return_t
    usart_wrapper::do_send (const void* data, std::size_t num) noexcept
    {
      return driver_->Send (data, static_cast<uint32_t> (num));
    }

    return_t
    usart_wrapper::do_receive (void* data, std::size_t num) noexcept
    {
      return driver_->Receive (data, static_cast<uint32_t> (num));
    }

    return_t
    usart_wrapper::do_transfer (const void* data_out, void* data_in,
                                std::size_t num) noexcept
    {
      return driver_->Transfer (data_out, data_in, static_cast<uint32_t> (num));
    }

    std::size_t
    usart_wrapper::do_get_tx_count (void) noexcept
    {
      return driver_->GetTxCount ();
    }

    std::size_t
    usart_wrapper::do_get_rx_count (void) noexcept
    {
      return driver_->GetRxCount ();
    }

    return_t
    usart_wrapper::do_configure (serial::config_t cfg, serial::config_arg_t arg) noexcept
    {
      return driver_->Control (cfg, arg);
    }

    return_t
    usart_wrapper::do_control (serial::control_t ctrl) noexcept
    {
      switch (ctrl)
        {
        case serial::Control::disable_tx:
        case serial::Control::disable_rx:
        case serial::Control::disable_break:
          return driver_->Control (
              ctrl - (serial::Control::disable_tx - serial::Control::enable_tx),
              0);
        }
      return driver_->Control (ctrl, 1);
    }

    return_t
    usart_wrapper::do_control_modem_line (serial::Modem_control ctrl) noexcept
    {
      return driver_->SetModemControl (
          static_cast<ARM_USART_MODEM_CONTROL> (ctrl));
    }

  } /* namespace driver */
} /* namespace os */

// ----------------------------------------------------------------------------
