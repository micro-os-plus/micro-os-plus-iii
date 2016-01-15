/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
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

#include <cmsis-plus/drivers/usart-wrapper.h>

#include <Driver_USART.h>

#include <utility>
#include <stdio.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace cmsis
  {
    namespace driver
    {
      // ----------------------------------------------------------------------

      Usart_wrapper::Usart_wrapper (ARM_DRIVER_USART* driver,
                                    ARM_USART_SignalEvent_t c_cb_func) noexcept :
      driver_ (driver),
      c_cb_func_ (c_cb_func)
        {
          ;
        }

      Usart_wrapper::~Usart_wrapper () noexcept
      {
        driver_ = nullptr;
      }

      // ----------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"
#pragma GCC diagnostic ignored "-Wstrict-aliasing"

      const Version&
      Usart_wrapper::do_get_version (void) noexcept
      {
        // Overwrite the C++ instance. Assume same layout.
        *((ARM_DRIVER_VERSION*) (&version_)) = driver_->GetVersion ();
        return version_;
      }

      const serial::Capabilities&
      Usart_wrapper::do_get_capabilities (void) noexcept
      {
        // Overwrite the C++ instance. Assume same layout.
        *((ARM_USART_CAPABILITIES*) (&capa_)) = driver_->GetCapabilities ();
        return capa_;
      }

      serial::Status&
      Usart_wrapper::do_get_status (void) noexcept
      {
        // Overwrite the C++ instance. Assume same layout.
        *((ARM_USART_STATUS*) (&status_)) = driver_->GetStatus ();
        return status_;
      }

      serial::Modem_status&
      Usart_wrapper::do_get_modem_status (void) noexcept
      {
        // Overwrite the C++ instance. Assume same layout.
        *((ARM_USART_MODEM_STATUS*) (&modem_status_)) =
            driver_->GetModemStatus ();
        return modem_status_;
      }

#pragma GCC diagnostic pop

      status_t
      Usart_wrapper::do_power (Power state) noexcept
      {
        status_t status;

        if (state == Power::full)
          {
            status = driver_->Initialize (c_cb_func_);
            if (status != ARM_DRIVER_OK)
              {
                return status;
              }
          }

        status = driver_->PowerControl ((ARM_POWER_STATE) state);

        if (state == Power::off)
          {
            driver_->Uninitialize ();
          }

        return status;
      }

      status_t
      Usart_wrapper::do_send (const void* data, std::size_t num) noexcept
      {
        return driver_->Send (data, num);
      }

      status_t
      Usart_wrapper::do_receive (void* data, std::size_t num) noexcept
      {
        return driver_->Receive (data, num);
      }

      status_t
      Usart_wrapper::do_transfer (const void* data_out, void* data_in,
                                  std::size_t num) noexcept
      {
        return driver_->Transfer (data_out, data_in, num);
      }

      std::size_t
      Usart_wrapper::do_get_tx_count (void) noexcept
      {
        return driver_->GetTxCount ();
      }

      std::size_t
      Usart_wrapper::do_get_rx_count (void) noexcept
      {
        return driver_->GetRxCount ();
      }

      status_t
      Usart_wrapper::do_configure (serial::config_t cfg,
                                   serial::config_arg_t arg) noexcept
      {
        return driver_->Control (cfg, arg);
      }

      status_t
      Usart_wrapper::do_control (serial::control_t ctrl) noexcept
      {
        switch (ctrl)
          {
          case serial::DISABLE_TX:
          case serial::DISABLE_RX:
          case serial::DISABLE_BREAK:
            return driver_->Control (
                ctrl - (serial::DISABLE_TX - serial::ENABLE_TX), 0);
          }
        return driver_->Control (ctrl, 1);
      }

      status_t
      Usart_wrapper::do_control_modem_line (serial::Modem_control ctrl) noexcept
      {
        return driver_->SetModemControl ((ARM_USART_MODEM_CONTROL) ctrl);
      }

    } /* namespace driver */
  } /* namespace cmsis */
} /* namespace os */
