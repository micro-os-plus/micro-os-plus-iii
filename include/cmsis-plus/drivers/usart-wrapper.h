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
  namespace cmsis
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

        virtual
        ~Usart_wrapper () noexcept;

        // --------------------------------------------------------------------

      protected:

        virtual const Version&
        do_get_version (void) noexcept override;

        virtual const serial::Capabilities&
        do_get_capabilities (void) noexcept override;

        virtual status_t
        do_power (Power state) noexcept override;

        virtual status_t
        do_send (const void* data, std::size_t num) noexcept override;

        virtual status_t
        do_receive (void* data, std::size_t num) noexcept override;

        virtual status_t
        do_transfer (const void* data_out, void* data_in, std::size_t num)
            noexcept override;

        virtual std::size_t
        do_get_tx_count (void) noexcept override;

        virtual std::size_t
        do_get_rx_count (void) noexcept override;

        virtual status_t
        do_configure (serial::config_t ctrl, serial::config_arg_t arg)
            noexcept override;

        virtual status_t
        do_control (serial::control_t ctrl) noexcept override;

        virtual serial::Status&
        do_get_status (void) noexcept override;

        virtual status_t
        do_control_modem_line (serial::Modem_control ctrl) noexcept override;

        virtual serial::Modem_status&
        do_get_modem_status (void) noexcept override;

        // --------------------------------------------------------------------

      private:

        /// Pointer to CMSIS USART Keil driver.
        ARM_DRIVER_USART* driver_;

        /// Pointer to non-reentrant callback. Must be stored because
        /// Initialise() is now delayed just before PowerControl(FULL).
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
  } /* namespace cmsis */
} /* namespace os */

#endif /* CMSIS_DRIVER_USART_WRAPPER_H_ */
