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

#include <cmsis-plus/posix-drivers/cmsis-driver-usart-wrapper.h>
#include "Driver_USART.h"

// ----------------------------------------------------------------------------

namespace os
{
  namespace cmsis
  {
    namespace driver
    {
      // ----------------------------------------------------------------------

      Usart_wrapper::Usart_wrapper (ARM_DRIVER_USART* driver) :
          driver_ (driver)
      {
        ;
      }

      Usart_wrapper::~Usart_wrapper ()
      {
        driver_ = nullptr;
      }

      // ----------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

      ARM_DRIVER_VERSION
      Usart_wrapper::get_version (void)
      {
        return driver_->GetVersion ();
      }

      ARM_USART_CAPABILITIES
      Usart_wrapper::get_capabilities (void)
      {
        return driver_->GetCapabilities ();
      }

#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

      int32_t
      Usart_wrapper::do_initialize ()
      {
        // Nothing more to do, due to the Keil non-reentrant callback,
        // the Initialize() was called before.
        return ARM_DRIVER_OK;
      }

#pragma GCC diagnostic pop

      int32_t
      Usart_wrapper::uninitialize (void)
      {
        return driver_->Uninitialize ();
      }

      int32_t
      Usart_wrapper::power (ARM_POWER_STATE state)
      {
        return driver_->PowerControl (state);
      }

      int32_t
      Usart_wrapper::send (const void* data, uint32_t num)
      {
        return driver_->Send (data, num);
      }

      int32_t
      Usart_wrapper::receive (void* data, uint32_t num)
      {
        return driver_->Receive (data, num);
      }

      int32_t
      Usart_wrapper::transfer (const void* data_out, void* data_in,
                               uint32_t num)
      {
        return driver_->Transfer (data_out, data_in, num);
      }

      uint32_t
      Usart_wrapper::get_tx_count (void)
      {
        return driver_->GetTxCount ();
      }

      uint32_t
      Usart_wrapper::get_rx_count (void)
      {
        return driver_->GetRxCount ();
      }

      int32_t
      Usart_wrapper::control (uint32_t ctrl, uint32_t arg)
      {
        return driver_->Control (ctrl, arg);
      }

      int32_t
      Usart_wrapper::set_modem_control (ARM_USART_MODEM_CONTROL ctrl)
      {
        return driver_->SetModemControl (ctrl);
      }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

      ARM_USART_STATUS
      Usart_wrapper::get_status (void)
      {
        return driver_->GetStatus ();
      }

      ARM_USART_MODEM_STATUS
      Usart_wrapper::get_modem_status (void)
      {
        return driver_->GetModemStatus ();
      }

#pragma GCC diagnostic pop

    } /* namespace driver */
  } /* namespace cmsis */
} /* namespace os */
