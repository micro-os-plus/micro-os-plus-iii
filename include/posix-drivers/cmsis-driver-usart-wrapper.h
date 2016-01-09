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

#include "posix-drivers/cmsis-driver-serial.h"

// ----------------------------------------------------------------------------

// Forward definitions to avoid "Driver_USART.h"
extern "C"
{
  typedef struct _ARM_DRIVER_USART const ARM_DRIVER_USART;
}

// ----------------------------------------------------------------------------

namespace os
{
  namespace cmsis
  {
    namespace driver
    {
      // ----------------------------------------------------------------------

      class Usart_wrapper : public Serial
      {
      public:

        // --------------------------------------------------------------------

        Usart_wrapper (ARM_DRIVER_USART* driver);

        virtual
        ~Usart_wrapper ();

        // --------------------------------------------------------------------

        virtual ARM_DRIVER_VERSION
        get_version (void) override;

        virtual ARM_USART_CAPABILITIES
        get_capabilities (void) override;

        virtual int32_t
        uninitialize (void) override;

        virtual int32_t
        power (ARM_POWER_STATE state) override;

        virtual int32_t
        send (const void* data, uint32_t num) override;

        virtual int32_t
        receive (void* data, uint32_t num) override;

        virtual int32_t
        transfer (const void* data_out, void* data_in, uint32_t num) override;

        virtual uint32_t
        get_tx_count (void) override;

        virtual uint32_t
        get_rx_count (void) override;

        virtual int32_t
        control (uint32_t control, uint32_t arg) override;

        virtual ARM_USART_STATUS
        get_status (void) override;

        virtual int32_t
        set_modem_control (ARM_USART_MODEM_CONTROL control) override;

        virtual ARM_USART_MODEM_STATUS
        get_modem_status (void) override;

        // --------------------------------------------------------------------

      protected:

        virtual int32_t
        do_initialize (void) override;

      private:

        ARM_DRIVER_USART* driver_;
      };

    } /* namespace driver */
  } /* namespace cmsis */
} /* namespace os */

#endif /* CMSIS_DRIVER_USART_WRAPPER_H_ */
