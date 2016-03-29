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

#ifndef CMSIS_DRIVER_USART_WRAPPER_H_
#define CMSIS_DRIVER_USART_WRAPPER_H_

#include <cmsis-plus/drivers/serial.h>

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
#pragma GCC diagnostic ignored "-Wpadded"

    // This wrapper makes a CMSIS USART Keil driver behave like a
    // CMSIS++ Serial driver.

    class Usart_wrapper : public Serial
    {
    public:

      // --------------------------------------------------------------------

      Usart_wrapper (ARM_DRIVER_USART* driver,
                     ARM_USART_SignalEvent_t c_cb_func) noexcept;

      Usart_wrapper (const Usart_wrapper&) = delete;

      Usart_wrapper (Usart_wrapper&&) = delete;

      Usart_wrapper&
      operator= (const Usart_wrapper&) = delete;

      Usart_wrapper&
      operator= (Usart_wrapper&&) = delete;

      virtual
      ~Usart_wrapper () noexcept;

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
      serial::Status status_;
      serial::Modem_status modem_status_;

    };

#pragma GCC diagnostic pop

  } /* namespace driver */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* CMSIS_DRIVER_USART_WRAPPER_H_ */
