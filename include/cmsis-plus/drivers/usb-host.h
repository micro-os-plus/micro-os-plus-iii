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
 * The code is inspired by ARM CMSIS Driver_USBH.h file, v2.01,
 * and tries to remain functionally close to the CMSIS specifications.
 */

#ifndef CMSIS_PLUS_DRIVERS_USB_HOST_H_
#define CMSIS_PLUS_DRIVERS_USB_HOST_H_

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
      namespace host
      {
        // ==================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

        /**
         * @brief USB host driver capabilities.
         */
        class Capabilities
        {
        public:

          // For compatibility with ARM CMSIS, these bits should be
          // exactly in this order.

          ///< Root HUB available Ports Mask
          uint32_t port_mask :15;

          ///< Automatic SPLIT packet handling
          bool auto_split :1;

          ///< Signal Connect event
          bool event_connect :1;

          ///< Signal Disconnect event
          bool event_disconnect :1;

          ///< Signal Overcurrent event
          bool event_overcurrent :1;
        };

#pragma GCC diagnostic pop

        // ==================================================================
        // ----- USB Host Status -----

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

        /**
         * @brief USB host state
         */
        class Status
        {
        public:

          bool
          is_connected (void) const noexcept;

          bool
          is_overcurrent (void) const noexcept;

          speed_t
          get_speed (void) const noexcept;

          // ----------------------------------------------------------------

        public:

          // For compatibility with ARM CMSIS, these bits should be
          // exactly in this order.

          ///< USB Host Port connected flag
          bool connected :1;

          ///< USB Host Port overcurrent flag
          bool overcurrent :1;

          ///< USB Host Port speed setting (ARM_USB_SPEED_xxx)
          speed_t speed :2;
        };

#pragma GCC diagnostic pop

        // ==================================================================
        // ----- USB Host Port Events -----

        /**
         * @brief USB Host Port Events
         */
        enum Port_event
          : event_t
            {
              //

          ///< USB Device Connected to Port
          connect = (1UL << 0),

          ///< USB Device Disconnected from Port
          disconnect = (1UL << 1),

          ///< USB Device caused Overcurrent
          overcurrent = (1UL << 2),

          ///< USB Reset completed
          reset = (1UL << 3),

          ///< USB Suspend occurred
          suspend = (1UL << 4),

          ///< USB Resume occurred
          resume = (1UL << 5),

          ///< USB Device activated Remote Wakeup
          remote_hangup = (1UL << 6)
        };

        // ==================================================================
        // ----- USB Host Pipe Event -----

        /**
         * @brief USB Host Pipe Event
         */
        enum Pipe_event
          : event_t
            {
              //

          ///< Transfer completed
          transfer_complete = (1UL << 0),

          ///< NAK Handshake received
          handshake_nak = (1UL << 1),

          ///< NYET Handshake received
          handshake_nyet = (1UL << 2),

          ///< MDATA Handshake received
          handshake_mdata = (1UL << 3),

          ///< STALL Handshake received
          handshake_stall = (1UL << 4),

          ///< ERR Handshake received
          handshake_err = (1UL << 5),

          ///< Bus Error detected
          bus_err = (1UL << 6)
        };

        // ------------------------------------------------------------------

        typedef void
        (*signal_port_event_t) (const void* object, port_t port, event_t event);

        typedef void
        (*signal_pipe_event_t) (const void* object, pipe_t pipe, event_t event);

      } /* namespace host */

      // ====================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

      class Host : public Base
      {

      public:

        // ------------------------------------------------------------------

        Host () noexcept;

        Host (const Host&) = delete;

        Host (Host&&) = delete;

        Host&
        operator= (const Host&) = delete;

        Host&
        operator= (Host&&) = delete;

        virtual
        ~Host () noexcept = default;

        // ------------------------------------------------------------------

        /**
         * @brief       Register port event callback.
         * @param [in] cb_func  Pointer to callback function.
         * @param [in] cb_object Pointer to object to be passed to the function.
         */
        void
        register_port_callback (host::signal_port_event_t cb_func,
                                const void* cb_object = nullptr) noexcept;

        void
        register_pipe_callback (host::signal_pipe_event_t cb_func,
                                const void* cb_object = nullptr) noexcept;

        // ------------------------------------------------------------------

        const host::Capabilities&
        get_capabilities (void) noexcept;

        return_t
        power_port_vbus (port_t port, bool vbus) noexcept;

        return_t
        reset_port (port_t port) noexcept;

        return_t
        suspend_port (port_t port) noexcept;

        return_t
        resume_port (port_t port) noexcept;

        host::Status&
        get_port_status (port_t port) noexcept;

        pipe_t
        create_pipe (device_address_t dev_addr, speed_t dev_speed,
                     hub_addr_t hub_addr, hub_port_t hub_port,
                     endpoint_t ep_addr, endpoint_type_t ep_type,
                     packet_size_t ep_max_packet_size,
                     polling_interval_t ep_interval) noexcept;

        return_t
        modify_pipe (pipe_t pipe, device_address_t dev_addr, speed_t dev_speed,
                     hub_addr_t hub_addr, hub_port_t hub_port,
                     packet_size_t ep_max_packet_size) noexcept;

        return_t
        delete_pipe (pipe_t pipe) noexcept;

        return_t
        reset_pipe (pipe_t pipe) noexcept;

        return_t
        transfer (pipe_t pipe, uint32_t packet, uint8_t* data, std::size_t num)
            noexcept;

        std::size_t
        get_transfer_count (pipe_t pipe) noexcept;

        return_t
        abort_transfer (pipe_t pipe) noexcept;

        uint16_t
        get_frame_number (void) noexcept;

        void
        signal_port_event (port_t port, event_t event) noexcept;

        void
        signal_pipe_event (pipe_t pipe, event_t event) noexcept;

      protected:

        virtual const host::Capabilities&
        do_get_capabilities (void) noexcept = 0;

        virtual return_t
        do_power_port_vbus (port_t port, bool vbus) noexcept = 0;

        virtual return_t
        do_reset_port (port_t port) noexcept = 0;

        virtual return_t
        do_suspend_port (port_t port) noexcept = 0;

        virtual return_t
        do_resume_port (port_t port) noexcept = 0;

        virtual host::Status&
        do_get_port_status (port_t port) noexcept = 0;

        virtual pipe_t
        do_create_pipe (device_address_t dev_addr, speed_t dev_speed,
                        hub_addr_t hub_addr, hub_port_t hub_port,
                        endpoint_t ep_addr, endpoint_type_t ep_type,
                        packet_size_t ep_max_packet_size,
                        polling_interval_t ep_interval) noexcept = 0;

        virtual return_t
        do_modify_pipe (pipe_t pipe, device_address_t dev_addr,
                        speed_t dev_speed, hub_addr_t hub_addr,
                        hub_port_t hub_port, packet_size_t ep_max_packet_size)
                            noexcept = 0;

        virtual return_t
        do_delete_pipe (pipe_t pipe) noexcept = 0;

        virtual return_t
        do_reset_pipe (pipe_t pipe) noexcept = 0;

        virtual return_t
        do_transfer (pipe_t pipe, uint32_t packet, uint8_t* data,
                     std::size_t num) noexcept = 0;

        virtual std::size_t
        do_get_transfer_count (pipe_t pipe) noexcept = 0;

        virtual return_t
        do_abort_transfer (pipe_t pipe) noexcept = 0;

        virtual uint16_t
        do_get_frame_number (void) noexcept = 0;

      private:

        /// Pointer to static function that implements the port callback.
        host::signal_port_event_t cb_port_func_;

        /// Pointer to object instance associated with the port callback.
        const void* cb_port_object_;

        /// Pointer to static function that implements the pipe callback.
        host::signal_pipe_event_t cb_pipe_func_;

        /// Pointer to object instance associated with the pipe callback.
        const void* cb_pipe_object_;

      protected:

        host::Status status_;
      };

#pragma GCC diagnostic pop

      // --------------------------------------------------------------------
      // ----- Definitions -----

      namespace host
      {
        // ------------------------------------------------------------------

        inline bool
        Status::is_connected (void) const noexcept
        {
          return connected;
        }

        inline bool
        Status::is_overcurrent (void) const noexcept
        {
          return overcurrent;
        }

        inline speed_t
        Status::get_speed (void) const noexcept
        {
          return speed;
        }

      } /* namespace host */

      // --------------------------------------------------------------------

      inline const host::Capabilities&
      Host::get_capabilities (void) noexcept
      {
        return do_get_capabilities ();
      }

      inline return_t
      Host::power_port_vbus (port_t port, bool vbus) noexcept
      {
        return do_power_port_vbus (port, vbus);
      }

      inline return_t
      Host::reset_port (port_t port) noexcept
      {
        return do_reset_port (port);
      }

      inline return_t
      Host::suspend_port (port_t port) noexcept
      {
        return do_suspend_port (port);
      }

      inline return_t
      Host::resume_port (port_t port) noexcept
      {
        return do_resume_port (port);
      }

      inline host::Status&
      Host::get_port_status (port_t port) noexcept
      {
        return do_get_port_status (port);
      }

      inline pipe_t
      Host::create_pipe (device_address_t dev_addr, speed_t dev_speed,
                         hub_addr_t hub_addr, hub_port_t hub_port,
                         endpoint_t ep_addr, endpoint_type_t ep_type,
                         packet_size_t ep_max_packet_size,
                         polling_interval_t ep_interval) noexcept
      {
        return do_create_pipe (dev_addr, dev_speed, hub_addr, hub_port, ep_addr,
                               ep_type, ep_max_packet_size, ep_interval);
      }

      inline return_t
      Host::modify_pipe (pipe_t pipe, device_address_t dev_addr,
                         speed_t dev_speed, hub_addr_t hub_addr,
                         hub_port_t hub_port, packet_size_t ep_max_packet_size) noexcept
      {
        return do_modify_pipe (pipe, dev_addr, dev_speed, hub_addr, hub_port,
                               ep_max_packet_size);
      }

      inline return_t
      Host::delete_pipe (pipe_t pipe) noexcept
      {
        return do_delete_pipe (pipe);
      }

      inline return_t
      Host::reset_pipe (pipe_t pipe) noexcept
      {
        return do_reset_pipe (pipe);
      }

      inline std::size_t
      Host::get_transfer_count (pipe_t pipe) noexcept
      {
        return do_get_transfer_count (pipe);
      }

      inline return_t
      Host::abort_transfer (pipe_t pipe) noexcept
      {
        return do_abort_transfer (pipe);
      }

      inline uint16_t
      Host::get_frame_number (void) noexcept
      {
        return do_get_frame_number ();
      }

    } /* namespace usb */
  } /* namespace driver */
} /* namespace os */

#endif /* __cplusplus */

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_DRIVERS_USB_HOST_H_ */
