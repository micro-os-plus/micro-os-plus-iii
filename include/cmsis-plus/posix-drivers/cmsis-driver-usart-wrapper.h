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

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#include <cmsis-plus/posix-drivers/cmsis-driver-serial.h>

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

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_DRIVER_USART_WRAPPER_H_ */
