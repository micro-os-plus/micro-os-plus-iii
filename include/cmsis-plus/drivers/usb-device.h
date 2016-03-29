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
 * The code is inspired by ARM CMSIS Driver_USBD.h file, v2.01,
 * and tries to remain functionally close to the CMSIS specifications.
 */

#ifndef CMSIS_PLUS_DRIVERS_USB_DEVICE_H_
#define CMSIS_PLUS_DRIVERS_USB_DEVICE_H_

// ----------------------------------------------------------------------------

#ifdef __cplusplus

#include <cmsis-plus/drivers/common.h>
#include <cmsis-plus/drivers/usb.h>

#include <cstdint>
#include <cstddef>

namespace os
{
  namespace driver
  {
    namespace usb
    {
      namespace device
      {
        // ==================================================================

        /**
         * @brief USB device driver capabilities.
         */
        class Capabilities
        {
        public:

          // For compatibility with ARM CMSIS, these bits should be
          // exactly in this order.

          ///< VBUS detection
          bool vbus_detection :1;

          ///< Signal VBUS On event
          bool event_vbus_on :1;

          ///< Signal VBUS Off event
          bool event_vbus_off :1;

        };

        // ==================================================================
        // ----- USB Device Status -----

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

        /**
         * @brief USB device state
         */
        class Status
        {
        public:

          bool
          is_vbus_on (void) const noexcept;

          speed_t
          get_speed (void) const noexcept;

          bool
          is_active (void) const noexcept;

          // ----------------------------------------------------------------

        public:

          // For compatibility with ARM CMSIS, these bits should be
          // exactly in this order.

          ///< USB Device VBUS flag
          bool vbus :1;

          ///< USB Device speed setting (ARM_USB_SPEED_xxx)
          speed_t speed :2;

          ///< USB Device active flag
          bool active :1;
        };

#pragma GCC diagnostic pop

        // ==================================================================
        // ----- USB Device Events -----

        /**
         * @brief USB Device Events
         */
        enum Device_event
          : event_t
            {
              //

          ///< USB Device VBUS On
          vbus_on = (1UL << 0),

          ///< USB Device VBUS Off
          vbus_off = (1UL << 1),

          ///< USB Reset occurred
          reset = (1UL << 2),

          ///< USB switch to High Speed occurred
          high_speed = (1UL << 3),

          ///< USB Suspend occurred
          suspend = (1UL << 4),

          ///< USB Resume occurred
          resume = (1UL << 5)
        };

        // ==================================================================
        // ----- USB Device Endpoint Events -----

        /**
         * @brief USB Device Endpoint Events
         */
        enum Endpoint_event
          : event_t
            {
              //

          ///< SETUP Packet
          setup = (1UL << 0),

          ///< OUT Packet(s)
          out = (1UL << 1),

          ///< IN Packet(s)
          in = (1UL << 2),
        };

        // ------------------------------------------------------------------

        typedef void
        (*signal_device_event_t) (const void* object, event_t event);

        typedef void
        (*signal_endpoint_event_t) (const void* object, endpoint_t ep_addr,
                                    event_t event);

      } /* namespace device */

      // ====================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

      class Device : public Base
      {

      public:

        // ------------------------------------------------------------------

        Device () noexcept;

        Device (const Device&) = delete;

        Device (Device&&) = delete;

        Device&
        operator= (const Device&) = delete;

        Device&
        operator= (Device&&) = delete;

        virtual
        ~Device () noexcept = default;

        // ------------------------------------------------------------------

        /**
         * @brief       Register device event callback.
         * @param [in]   cb_func  Pointer to function.
         * @param [in] cb_object Pointer to object passed to the function.
         * @return      Execution status.
         */
        void
        register_device_callback (device::signal_device_event_t cb_func,
                                  const void* cb_object = nullptr) noexcept;

        void
        register_endpoint_callback (device::signal_endpoint_event_t cb_func,
                                    const void* cb_object = nullptr) noexcept;

        // ------------------------------------------------------------------

        /**
         * @brief       Get driver capabilities
         * @return      Capabilities.
         */
        const device::Capabilities&
        get_capabilities (void) noexcept;

        /**
         * @brief       Connect USB Device.
         * @return      Execution status.
         */
        return_t
        connect (void) noexcept;

        /**
         * @brief       Disconnect USB Device.
         * @return      Execution status.
         */
        return_t
        disconnect (void) noexcept;

        /**
         * @brief       Get current USB Device Status.
         * @return      Device State.
         */
        device::Status&
        get_status (void) noexcept;

        /**
         * @brief       Trigger USB Remote Wakeup.
         * @return      Execution status.
         */
        return_t
        wakeup_remote (void) noexcept;

        /**
         * @brief       Set USB Device Address.
         * @param [in]   dev_addr  Device Address
         * @return      Execution status.
         */
        return_t
        configure_address (device_address_t dev_addr) noexcept;

        /**
         * @brief       Read setup packet received over Control Endpoint.
         * @param [out]  buf  Pointer to buffer for setup packet
         * @return      Execution status.
         */
        return_t
        read_setup_packet (uint8_t* buf) noexcept;

        /**
         * @brief       Get current USB Frame Number.
         * @return      Frame Number
         */
        frame_number_t
        get_frame_number (void) noexcept;

        /**
         * @brief       Configure USB Endpoint.
         * @param [in]   ep_addr  Endpoint Address
         *                - ep_addr.0..3: Address
         *                - ep_addr.7:    Direction
         * @param [in]   ep_type  Endpoint Type (ARM_USB_ENDPOINT_xxx)
         * @param [in]   ep_max_packet_size Endpoint Maximum Packet Size
         * @return      Execution status.
         */
        return_t
        configure_endpoint (endpoint_t ep_addr, Endpoint_type ep_type,
                            packet_size_t ep_max_packet_size) noexcept;

        /**
         * @brief       Unconfigure USB Endpoint.
         * @param [in]   ep_addr  Endpoint Address
         *                - ep_addr.0..3: Address
         *                - ep_addr.7:    Direction
         * @return      Execution status.
         */
        return_t
        unconfigure_endpoint (endpoint_t ep_addr) noexcept;

        /**
         * @brief       Set/Clear Stall for USB Endpoint.
         * @param [in]   ep_addr  Endpoint Address
         *                - ep_addr.0..3: Address
         *                - ep_addr.7:    Direction
         * @param [in]   stall  Operation
         *                - \b false Clear
         *                - \b true Set
         * @return      Execution status.
         */
        return_t
        stall_endpoint (endpoint_t ep_addr, bool stall) noexcept;

        /**
         * @brief       Read data from or Write data to USB Endpoint.
         * @param [in]   ep_addr  Endpoint Address
         *                - ep_addr.0..3: Address
         *                - ep_addr.7:    Direction
         * @param [out]  data Pointer to buffer for data to read or with data to write
         * @param [in]   num  Number of data bytes to transfer
         * @return      Execution status.
         */
        return_t
        transfer (endpoint_t ep_addr, uint8_t* data, std::size_t num) noexcept;

        /**
         * @brief       Get result of USB Endpoint transfer.
         * @param [in]   ep_addr  Endpoint Address
         *                - ep_addr.0..3: Address
         *                - ep_addr.7:    Direction
         * @return      number of successfully transferred data bytes
         */
        std::size_t
        get_transfer_count (endpoint_t ep_addr) noexcept;

        /**
         * @brief       Abort current USB Endpoint transfer.
         * @param [in]   ep_addr  Endpoint Address
         *                - ep_addr.0..3: Address
         *                - ep_addr.7:    Direction
         * @return      Execution status.
         */
        return_t
        abort_transfer (endpoint_t ep_addr) noexcept;

        /**
         * @brief       Signal device events.
         * @param [in]  event
         * @return      none
         */
        void
        signal_device_event (event_t event) noexcept;

        /**
         * @brief       Signal USB Endpoint Event.
         * @param[in]   ep_addr  Endpoint Address
         *                - ep_addr.0..3: Address
         *                - ep_addr.7:    Direction
         * @param [in]  event
         * @return      none
         */
        void
        signal_endpoint_event (endpoint_t ep_addr, event_t event) noexcept;

      protected:

        virtual const device::Capabilities&
        do_get_capabilities (void) noexcept = 0;

        virtual return_t
        do_connect (void) noexcept = 0;

        virtual return_t
        do_disconnect (void) noexcept = 0;

        virtual device::Status&
        do_get_status (void) noexcept = 0;

        virtual return_t
        do_wakeup_remote (void) noexcept = 0;

        virtual return_t
        do_configure_address (device_address_t dev_addr) noexcept = 0;

        virtual return_t
        do_read_setup_packet (uint8_t* buf) noexcept = 0;

        virtual frame_number_t
        do_get_frame_number (void) noexcept = 0;

        virtual return_t
        do_configure_endpoint (endpoint_t ep_addr, Endpoint_type ep_type,
                               packet_size_t ep_max_packet_size) noexcept = 0;

        virtual return_t
        do_unconfigure_endpoint (endpoint_t ep_addr) noexcept = 0;

        virtual return_t
        do_stall_endpoint (endpoint_t ep_addr, bool stall) noexcept = 0;

        virtual return_t
        do_transfer (endpoint_t ep_addr, uint8_t* data, std::size_t num)
            noexcept = 0;

        virtual std::size_t
        do_get_transfer_count (endpoint_t ep_addr) noexcept = 0;

        virtual return_t
        do_abort_transfer (endpoint_t ep_addr) noexcept = 0;

      private:

        /// Pointer to static function that implements the device callback.
        device::signal_device_event_t cb_device_func_;

        /// Pointer to object instance associated with the device callback.
        const void* cb_device_object_;

        /// Pointer to static function that implements the endpoint callback.
        device::signal_endpoint_event_t cb_endpoint_func_;

        /// Pointer to object instance associated with the endpoint callback.
        const void* cb_endpoint_object_;

      protected:

        device::Status status_;
      };

#pragma GCC diagnostic pop

      // ----------------------------------------------------------------------
      // ----- Definitions -----

      namespace device
      {
        // --------------------------------------------------------------------

        inline bool
        Status::is_vbus_on (void) const noexcept
        {
          return vbus;
        }

        inline speed_t
        Status::get_speed (void) const noexcept
        {
          return speed;
        }

        inline bool
        Status::is_active (void) const noexcept
        {
          return active;
        }

      } /* namespace device */

      // --------------------------------------------------------------------

      inline const device::Capabilities&
      Device::get_capabilities (void) noexcept
      {
        return do_get_capabilities ();
      }

      inline device::Status&
      Device::get_status (void) noexcept
      {
        return do_get_status ();
      }

      inline return_t
      Device::connect (void) noexcept
      {
        return do_connect ();
      }

      inline return_t
      Device::disconnect (void) noexcept
      {
        return do_disconnect ();
      }

      inline return_t
      Device::wakeup_remote (void) noexcept
      {
        return do_wakeup_remote ();
      }

      inline return_t
      Device::configure_address (uint8_t dev_addr) noexcept
      {
        return do_configure_address (dev_addr);
      }

      inline frame_number_t
      Device::get_frame_number (void) noexcept
      {
        return do_get_frame_number ();
      }

      inline return_t
      Device::configure_endpoint (endpoint_t ep_addr, Endpoint_type ep_type,
                                  packet_size_t ep_max_packet_size) noexcept
      {
        return do_configure_endpoint (ep_addr, ep_type, ep_max_packet_size);
      }

      inline return_t
      Device::unconfigure_endpoint (endpoint_t ep_addr) noexcept
      {
        return do_unconfigure_endpoint (ep_addr);
      }

      inline return_t
      Device::stall_endpoint (endpoint_t ep_addr, bool stall) noexcept
      {
        return do_stall_endpoint (ep_addr, stall);
      }

      inline std::size_t
      Device::get_transfer_count (endpoint_t ep_addr) noexcept
      {
        return do_get_transfer_count (ep_addr);
      }

      inline return_t
      Device::abort_transfer (endpoint_t ep_addr) noexcept
      {
        return do_abort_transfer (ep_addr);
      }

    } /* namespace usb */
  } /* namespace driver */
} /* namespace os */

#endif /* __cplusplus */

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_DRIVERS_USB_DEVICE_H_ */
