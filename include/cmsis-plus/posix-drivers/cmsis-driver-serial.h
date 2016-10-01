/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
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

#ifndef CMSIS_PLUS_POSIX_DRIVERS_CMSIS_DRIVER_SERIAL_H_
#define CMSIS_PLUS_POSIX_DRIVERS_CMSIS_DRIVER_SERIAL_H_

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#include "Driver_USART.h"
//#include <cstdint>

// ----------------------------------------------------------------------------

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
        ~Serial ();

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

extern "C"
{
  void
  cmsis_driver_serial_signal_event (void* object, uint32_t event);
}

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_DRIVERS_CMSIS_DRIVER_SERIAL_H_ */
