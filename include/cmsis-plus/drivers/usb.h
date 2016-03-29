/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 * Copyright (c) 2013-2014 ARM Ltd.
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

/*
 * This file is part of the CMSIS++ proposal, intended as a CMSIS
 * replacement for C++ applications.
 *
 * The code is inspired by ARM CMSIS Driver_USB.h file, v2.00,
 * and tries to remain functionally close to the CMSIS specifications.
 */

#ifndef CMSIS_PLUS_DRIVERS_USB_H_
#define CMSIS_PLUS_DRIVERS_USB_H_

// ----------------------------------------------------------------------------

#ifdef __cplusplus

#include <cstdint>

namespace os
{
  namespace driver
  {
    namespace usb
    {

      // --------------------------------------------------------------------

      using speed_t = uint8_t;

      /**
       * @brief USB Speed.
       */
      enum class Speed
        : speed_t
          {
            //

        ///< Low-speed USB
        low = 0,

        ///< Full-speed USB
        full = 1,

        ///< High-speed USB
        high = 2,
      };

      // --------------------------------------------------------------------

      using endpoint_t = uint8_t;

      constexpr endpoint_t ENDPOINT_NUMBER_MASK = 0x0F;
      constexpr endpoint_t ENDPOINT_DIRECTION_MASK = 0x80;

      // --------------------------------------------------------------------

      using endpoint_type_t = uint8_t;

      /**
       * @brief Endpoint type.
       */
      enum class Endpoint_type
        : endpoint_type_t
          {
            //

        ///< Control Endpoint
        control = 0,

        ///< Isochronous Endpoint
        isochronous = 1,

        ///< Bulk Endpoint
        bulk = 2,

        ///< Interrupt Endpoint
        interrupt = 3
      };

      // --------------------------------------------------------------------
      using packet_size_t = uint16_t;

      constexpr packet_size_t ENDPOINT_MAX_PACKET_SIZE_MASK = 0x07FF;

      // --------------------------------------------------------------------
      using frame_number_t = uint16_t;

      using device_address_t = uint8_t;

      //using ARM_USBH_PIPE_HANDLE = uint32_t;
      using pipe_t = uint32_t;
      using port_t = uint8_t;

      using hub_addr_t = uint8_t;
      using hub_port_t = uint8_t;

      using polling_interval_t = uint8_t;

    } /* namespace usb */
  } /* namespace driver */
} /* namespace os */

#endif /* __cplusplus */

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_DRIVERS_USB_H_ */
