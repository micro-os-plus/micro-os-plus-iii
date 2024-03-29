/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#ifndef CMSIS_PLUS_DRIVER_USART_WRAPPER_H_
#define CMSIS_PLUS_DRIVER_USART_WRAPPER_H_

#include <cmsis-plus/driver/serial.h>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

// ----------------------------------------------------------------------------

extern "C"
{
  // Avoid to include <Driver_USART.h>
  typedef void
  (*ARM_USART_SignalEvent_t) (uint32_t event);

  typedef struct _ARM_DRIVER_USART const ARM_DRIVER_USART;
}

namespace os
{
  namespace driver
  {
    // ======================================================================

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wpadded"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpadded"
#endif

    // This wrapper makes a CMSIS USART Keil driver behave like a
    // µOS++ Serial driver.

    class usart_wrapper : public Serial
    {
    public:

      // --------------------------------------------------------------------

      usart_wrapper (ARM_DRIVER_USART* driver,
                     ARM_USART_SignalEvent_t c_cb_func) noexcept;

      usart_wrapper (const usart_wrapper&) = delete;

      usart_wrapper (usart_wrapper&&) = delete;

      usart_wrapper&
      operator= (const usart_wrapper&) = delete;

      usart_wrapper&
      operator= (usart_wrapper&&) = delete;

      virtual
      ~usart_wrapper () noexcept override;

      // --------------------------------------------------------------------

    protected:

      virtual const Version&
      do_get_version (void) noexcept override;

      virtual const serial::Capabilities&
      do_get_capabilities (void) noexcept override;

      virtual return_t
      do_power (Power state) noexcept override;

      virtual return_t
      do_send (const void* data, std::size_t num) noexcept override;

      virtual return_t
      do_receive (void* data, std::size_t num) noexcept override;

      virtual return_t
      do_transfer (const void* data_out, void* data_in, std::size_t num)
          noexcept override;

      virtual std::size_t
      do_get_tx_count (void) noexcept override;

      virtual std::size_t
      do_get_rx_count (void) noexcept override;

      virtual return_t
      do_configure (serial::config_t ctrl, serial::config_arg_t arg)
          noexcept override;

      virtual return_t
      do_control (serial::control_t ctrl) noexcept override;

      virtual serial::Status&
      do_get_status (void) noexcept override;

      virtual return_t
      do_control_modem_line (serial::Modem_control ctrl) noexcept override;

      virtual serial::Modem_status&
      do_get_modem_status (void) noexcept override;

      // --------------------------------------------------------------------

    private:

      /// Pointer to CMSIS USART Keil driver.
      ARM_DRIVER_USART* driver_;

      /// Pointer to non-reentrant callback. Must be stored because
      /// Initialize() is now delayed just before PowerControl(FULL).
      ARM_USART_SignalEvent_t c_cb_func_;

      // Attempts to somehow use && failed, since the Keil driver
      // functions return temporary objects. So the only portable
      // solution was to copy these objects here and return
      // references to these objects.
      // (Not particularly proud of this solution, but could not find
      // a better one.)

      Version version_
        { 0, 0 };
      serial::Capabilities capa_;
      // serial::Status status_;
      // serial::Modem_status modem_status_;

    };

#pragma GCC diagnostic pop

  } /* namespace driver */
} /* namespace os */

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_DRIVER_USART_WRAPPER_H_ */
