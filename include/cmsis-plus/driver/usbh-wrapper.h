/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#ifndef CMSIS_PLUS_DRIVER_USBH_WRAPPER_H_
#define CMSIS_PLUS_DRIVER_USBH_WRAPPER_H_

#include <cmsis-plus/driver/usb-host.h>

// ----------------------------------------------------------------------------

extern "C"
{
  // Avoid to include <Driver_USBH.h>
  typedef uint32_t ARM_USBH_PIPE_HANDLE;

  typedef void
  (*ARM_USBH_SignalPortEvent_t) (uint8_t port, uint32_t event); ///< Pointer to port event callback
  typedef void
  (*ARM_USBH_SignalPipeEvent_t) (ARM_USBH_PIPE_HANDLE pipe_hndl,
                                 uint32_t event); ///< Pointer to pipe event callback.

  typedef struct _ARM_DRIVER_USBH const ARM_DRIVER_USBH;
}

namespace os
{
  namespace driver
  {
    // ======================================================================

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wpadded"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpadded"
#endif

    // This wrapper makes a CMSIS USBH Keil driver behave like a
    // µOS++ Serial driver.

    class usbh_wrapper : public usb::Host
    {
    public:

      // --------------------------------------------------------------------

      usbh_wrapper (ARM_DRIVER_USBH* driver,
                    ARM_USBH_SignalPortEvent_t c_cb_port_func,
                    ARM_USBH_SignalPipeEvent_t c_cb_pipe_func) noexcept;

      usbh_wrapper (const usbh_wrapper&) = delete;

      usbh_wrapper (usbh_wrapper&&) = delete;

      usbh_wrapper&
      operator= (const usbh_wrapper&) = delete;

      usbh_wrapper&
      operator= (usbh_wrapper&&) = delete;

      virtual
      ~usbh_wrapper () noexcept;

      // --------------------------------------------------------------------

    protected:

      virtual const Version&
      do_get_version (void) noexcept override;

      virtual return_t
      do_power (Power state) noexcept override;

      virtual const usb::host::Capabilities&
      do_get_capabilities (void) noexcept override;

      virtual return_t
      do_power_port_vbus (usb::port_t port, bool vbus) noexcept override;

      virtual return_t
      do_reset_port (usb::port_t port) noexcept override;

      virtual return_t
      do_suspend_port (usb::port_t port) noexcept override;

      virtual return_t
      do_resume_port (usb::port_t port) noexcept override;

      virtual usb::host::Status&
      do_get_port_status (usb::port_t port) noexcept override;

      virtual usb::pipe_t
      do_create_pipe (usb::device_address_t dev_addr, usb::speed_t dev_speed,
                      usb::hub_addr_t hub_addr, usb::hub_port_t hub_port,
                      usb::endpoint_t ep_addr, usb::endpoint_type_t ep_type,
                      usb::packet_size_t ep_max_packet_size,
                      usb::polling_interval_t ep_interval) noexcept override;

      virtual return_t
      do_modify_pipe (usb::pipe_t pipe, usb::device_address_t dev_addr,
                      usb::speed_t dev_speed, usb::hub_addr_t hub_addr,
                      usb::hub_port_t hub_port,
                      usb::packet_size_t ep_max_packet_size) noexcept override;

      virtual return_t
      do_delete_pipe (usb::pipe_t pipe) noexcept override;

      virtual return_t
      do_reset_pipe (usb::pipe_t pipe) noexcept override;

      virtual return_t
      do_transfer (usb::pipe_t pipe, uint32_t packet, uint8_t* data,
                   std::size_t num) noexcept override;

      virtual std::size_t
      do_get_transfer_count (usb::pipe_t pipe) noexcept override;

      virtual return_t
      do_abort_transfer (usb::pipe_t pipe) noexcept override;

      virtual uint16_t
      do_get_frame_number (void) noexcept override;

      // --------------------------------------------------------------------

    private:

      /// Pointer to CMSIS USBD Keil driver.
      ARM_DRIVER_USBH* driver_;

      /// Pointer to non-reentrant callback. Must be stored because
      /// Initialize() is now delayed just before PowerControl(FULL).
      ARM_USBH_SignalPortEvent_t c_cb_port_func_;
      ARM_USBH_SignalPipeEvent_t c_cb_pipe_func_;

      // Attempts to somehow use && failed, since the Keil driver
      // functions return temporary objects. So the only portable
      // solution was to copy these objects here and return
      // references to these objects.
      // (Not particularly proud of this solution, but could not find
      // a better one.)

      Version version_
        { 0, 0 };
      usb::host::Capabilities capa_;
      usb::host::Status status_;

    };

#pragma GCC diagnostic pop

  } /* namespace driver */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_DRIVER_USBH_WRAPPER_H_ */
