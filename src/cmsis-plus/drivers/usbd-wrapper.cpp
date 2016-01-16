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

#include <cmsis-plus/drivers/usbd-wrapper.h>

#include <Driver_USBD.h>

#include <cassert>
#include <utility>
#include <cstdio>

// ----------------------------------------------------------------------------

namespace os
{
  namespace cmsis
  {
    namespace driver
    {
      // ----------------------------------------------------------------------

      Usbd_wrapper::Usbd_wrapper (
          ARM_DRIVER_USBD* driver,
          ARM_USBD_SignalDeviceEvent_t c_cb_device_func,
          ARM_USBD_SignalEndpointEvent_t c_cb_endpoint_func) noexcept :
      driver_ (driver),
      c_cb_device_func_ (c_cb_device_func),
      c_cb_endpoint_func_ (c_cb_endpoint_func)
        {
          ;
        }

      Usbd_wrapper::~Usbd_wrapper () noexcept
      {
        driver_ = nullptr;
      }

      // ----------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"
#pragma GCC diagnostic ignored "-Wstrict-aliasing"

      const Version&
      Usbd_wrapper::do_get_version (void) noexcept
      {
        // Overwrite the C++ instance. Assume same layout.
        *((ARM_DRIVER_VERSION*) (&version_)) = driver_->GetVersion ();
        return version_;
      }

      const usb::device::Capabilities&
      Usbd_wrapper::do_get_capabilities (void) noexcept
      {
        // Overwrite the C++ instance. Assume same layout.
        *((ARM_USBD_CAPABILITIES*) (&capa_)) = driver_->GetCapabilities ();
        return capa_;
      }

      usb::device::Status&
      Usbd_wrapper::do_get_status (void) noexcept
      {
        // Overwrite the C++ instance. Assume same layout.
        *((ARM_USBD_STATE*) (&status_)) = driver_->DeviceGetState ();
        return status_;
      }

#pragma GCC diagnostic pop

      return_t
      Usbd_wrapper::do_power (Power state) noexcept
      {
        assert(driver_ != nullptr);

        return_t status;

        if (state == Power::full)
          {
            status = driver_->Initialize (c_cb_device_func_,
                                          c_cb_endpoint_func_);
            if (status != ARM_DRIVER_OK)
              {
                return status;
              }
          }

        status = driver_->PowerControl ((ARM_POWER_STATE) state);

        if (state == Power::off)
          {
            driver_->Uninitialize ();
          }

        return status;
      }

      return_t
      Usbd_wrapper::do_connect (void) noexcept
      {
        return driver_->DeviceConnect ();
      }

      return_t
      Usbd_wrapper::do_disconnect (void) noexcept
      {
        return driver_->DeviceDisconnect ();
      }

      return_t
      Usbd_wrapper::do_wakeup_remote (void) noexcept
      {
        return driver_->DeviceRemoteWakeup ();
      }

      return_t
      Usbd_wrapper::do_configure_address (usb::device_address_t dev_addr) noexcept
      {
        return driver_->DeviceSetAddress (dev_addr);
      }

      return_t
      Usbd_wrapper::do_read_setup_packet (uint8_t* buf) noexcept
      {
        return driver_->ReadSetupPacket (buf);
      }

      usb::frame_number_t
      Usbd_wrapper::do_get_frame_number (void) noexcept
      {
        return driver_->GetFrameNumber ();
      }

      return_t
      Usbd_wrapper::do_configure_endpoint (
          usb::endpoint_t ep_addr, usb::Endpoint_type ep_type,
          usb::packet_size_t ep_max_packet_size) noexcept
      {
        return driver_->EndpointConfigure (ep_addr, (uint8_t) ep_type,
                                           ep_max_packet_size);
      }

      return_t
      Usbd_wrapper::do_unconfigure_endpoint (usb::endpoint_t ep_addr) noexcept
      {
        return driver_->EndpointUnconfigure (ep_addr);
      }

      return_t
      Usbd_wrapper::do_stall_endpoint (usb::endpoint_t ep_addr, bool stall) noexcept
      {
        return driver_->EndpointStall (ep_addr, stall);
      }

      return_t
      Usbd_wrapper::do_transfer (usb::endpoint_t ep_addr, uint8_t* data,
                                 std::size_t num) noexcept
      {
        return driver_->EndpointTransfer (ep_addr, data, num);
      }

      std::size_t
      Usbd_wrapper::do_get_transfer_count (usb::endpoint_t ep_addr) noexcept
      {
        return driver_->EndpointTransferGetResult (ep_addr);
      }

      return_t
      Usbd_wrapper::do_abort_transfer (usb::endpoint_t ep_addr) noexcept
      {
        return driver_->EndpointTransferAbort (ep_addr);
      }

    } /* namespace driver */
  } /* namespace cmsis */
} /* namespace os */
