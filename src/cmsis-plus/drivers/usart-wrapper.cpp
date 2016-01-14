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

      const Version&
      Usart_wrapper::get_version (void) noexcept
      {
        u_v_.c_version = driver_->GetVersion ();
        return u_v_.version;;
      }

      const serial::Capabilities&
      Usart_wrapper::get_capabilities (void) noexcept
      {
        // C++ magic, get the address from inside the Keil driver.
        u_c_.c_capa = driver_->GetCapabilities ();
        return u_c_.capa;
      }

#pragma GCC diagnostic pop

      status_t
      Usart_wrapper::power (Power state) noexcept
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

      std::size_t
      Usart_wrapper::get_tx_count (void) noexcept
      {
        return driver_->GetTxCount ();
      }

      std::size_t
      Usart_wrapper::get_rx_count (void) noexcept
      {
        return driver_->GetRxCount ();
      }

      status_t
      Usart_wrapper::configure (serial::config_t cfg, serial::config_arg_t arg) noexcept
      {
        return driver_->Control (cfg, arg);
      }

      status_t
      Usart_wrapper::control (serial::control_t ctrl) noexcept
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
      Usart_wrapper::control_modem_line (serial::Modem_control ctrl) noexcept
      {
        return driver_->SetModemControl ((ARM_USART_MODEM_CONTROL) ctrl);
      }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

      serial::Status&
      Usart_wrapper::get_status (void) noexcept
      {
        u_s_.c_status = driver_->GetStatus ();
        return u_s_.status;
      }

      serial::Modem_status&
      Usart_wrapper::get_modem_status (void) noexcept
      {
        // C++ magic, get the address from inside the Keil driver.
        u_m_.c_modem_status = driver_->GetModemStatus ();
        return u_m_.modem_status;
      }

#pragma GCC diagnostic pop

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

    } /* namespace driver */
  } /* namespace cmsis */
} /* namespace os */
