/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
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

#ifndef POSIX_DRIVERS_CMSIS_DRIVER_SERIAL_H_
#define POSIX_DRIVERS_CMSIS_DRIVER_SERIAL_H_

// #include "Driver_USART.h"

// ----------------------------------------------------------------------------
#ifdef __cplusplus

namespace os
{
  namespace cmsis
  {
    namespace driver
    {
      // ----------------------------------------------------------------------

      class Serial
      {

      public:

        // --------------------------------------------------------------------

        typedef void
        (*signal_event_t) (const void* object, uint32_t event);

        // --------------------------------------------------------------------

        Serial ();

        virtual
        ~Serial () = default;

        // --------------------------------------------------------------------

        virtual ARM_DRIVER_VERSION
        get_version (void) = 0;

        virtual ARM_USART_CAPABILITIES
        get_capabilities (void) = 0;

        int32_t
        initialize (signal_event_t cb_event, const void* cb_object = nullptr);

        virtual int32_t
        uninitialize (void) = 0;

        virtual int32_t
        power (ARM_POWER_STATE state) = 0;

        virtual int32_t
        send (const void* data, uint32_t num) = 0;

        virtual int32_t
        receive (void* data, uint32_t num) = 0;

        virtual int32_t
        transfer (const void* data_out, void* data_in, uint32_t num) = 0;

        virtual uint32_t
        get_tx_count (void) = 0;

        virtual uint32_t
        get_rx_count (void) = 0;

        virtual int32_t
        control (uint32_t control, uint32_t arg) = 0;

        virtual ARM_USART_STATUS
        get_status (void) = 0;

        virtual int32_t
        set_modem_control (ARM_USART_MODEM_CONTROL control) = 0;

        virtual ARM_USART_MODEM_STATUS
        get_modem_status (void) = 0;

        void
        signal_event (uint32_t event);

      protected:

        virtual int32_t
        do_initialize (void) = 0;

      protected:

        signal_event_t cb_event_; // Pointer to static function.
        const void* cb_object_; // Pointer to object instance.

      };
    } /* namespace driver */
  } /* namespace cmsis */
} /* namespace os */

#endif /* __cplusplus */

extern "C"
{
  void
  cmsis_driver_serial_signal_event (void* object, uint32_t event);
}

#endif /* POSIX_DRIVERS_CMSIS_DRIVER_SERIAL_H_ */
