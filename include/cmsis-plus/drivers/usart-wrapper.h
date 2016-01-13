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
#include <Driver_USART.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace cmsis
  {
    namespace driver
    {
      // ======================================================================

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

        virtual Version&
        get_version (void) noexcept override;

        virtual serial::Capabilities&
        get_capabilities (void) noexcept override;

        virtual status_t
        power (Power state) noexcept override;

        virtual std::size_t
        get_tx_count (void) noexcept override;

        virtual std::size_t
        get_rx_count (void) noexcept override;

        virtual status_t
        configure (serial::config_t ctrl, serial::config_arg_t arg)
            noexcept override;

        virtual serial::Status&
        get_status (void) noexcept override;

        virtual status_t
        configure_modem_line (serial::Modem_config control) noexcept override;

        virtual serial::Modem_status&
        get_modem_status (void) noexcept override;

        // --------------------------------------------------------------------

      protected:

        virtual status_t
        do_send (const void* data, std::size_t num) noexcept override;

        virtual status_t
        do_receive (void* data, std::size_t num) noexcept override;

        virtual status_t
        do_transfer (const void* data_out, void* data_in, std::size_t num)
            noexcept override;

        // --------------------------------------------------------------------

      private:

        /// Pointer to CMSIS USART Keil driver.
        ARM_DRIVER_USART* driver_;

        /// Pointer to non-reentrant callback.
        ARM_USART_SignalEvent_t c_cb_func_;

      };

    } /* namespace driver */
  } /* namespace cmsis */
} /* namespace os */

#endif /* CMSIS_DRIVER_USART_WRAPPER_H_ */
