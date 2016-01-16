/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 * Copyright (c) 2013-2014 ARM Ltd.
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
  namespace cmsis
  {
    namespace driver
    {
      namespace usb
      {

        // --------------------------------------------------------------------

        using speed_t = uint32_t;

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
  } /* namespace cmsis */
} /* namespace os */

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_DRIVERS_USB_H_ */
