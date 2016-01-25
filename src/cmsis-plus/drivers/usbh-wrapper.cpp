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

#include <cmsis-plus/drivers/usbh-wrapper.h>

#include <Driver_USBH.h>

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

      Usbh_wrapper::Usbh_wrapper (ARM_DRIVER_USBH* driver,
                                  ARM_USBH_SignalPortEvent_t c_cb_port_func,
                                  ARM_USBH_SignalPipeEvent_t c_cb_pipe_func) noexcept :
      driver_ (driver),
      c_cb_port_func_ (c_cb_port_func),
      c_cb_pipe_func_ (c_cb_pipe_func)
        {
          ;
        }

      Usbh_wrapper::~Usbh_wrapper () noexcept
      {
        driver_ = nullptr;
      }

      // ----------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"
#pragma GCC diagnostic ignored "-Wstrict-aliasing"

      const Version&
      Usbh_wrapper::do_get_version (void) noexcept
      {
        // Overwrite the C++ instance. Assume same layout.
        *((ARM_DRIVER_VERSION*) (&version_)) = driver_->GetVersion ();
        return version_;
      }

      const usb::host::Capabilities&
      Usbh_wrapper::do_get_capabilities (void) noexcept
      {
        // Overwrite the C++ instance. Assume same layout.
        *((ARM_USBH_CAPABILITIES*) (&capa_)) = driver_->GetCapabilities ();
        return capa_;
      }

      usb::host::Status&
      Usbh_wrapper::do_get_port_status (usb::port_t port) noexcept
      {
        // Overwrite the C++ instance. Assume same layout.
        *((ARM_USBH_PORT_STATE*) (&status_)) = driver_->PortGetState (port);
        return status_;
      }

#pragma GCC diagnostic pop

      return_t
      Usbh_wrapper::do_power (Power state) noexcept
      {
        assert(driver_ != nullptr);

        return_t status;

        if (state == Power::full)
          {
            status = driver_->Initialize (c_cb_port_func_, c_cb_pipe_func_);
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
      Usbh_wrapper::do_power_port_vbus (usb::port_t port, bool vbus) noexcept
      {
        return driver_->PortVbusOnOff (port, vbus);
      }

      return_t
      Usbh_wrapper::do_reset_port (usb::port_t port) noexcept
      {
        return driver_->PortReset (port);
      }

      return_t
      Usbh_wrapper::do_suspend_port (usb::port_t port) noexcept
      {
        return driver_->PortSuspend (port);
      }

      return_t
      Usbh_wrapper::do_resume_port (usb::port_t port) noexcept
      {
        return driver_->PortResume (port);
      }

      usb::pipe_t
      Usbh_wrapper::do_create_pipe (usb::device_address_t dev_addr,
                                    usb::speed_t dev_speed,
                                    usb::hub_addr_t hub_addr,
                                    usb::hub_port_t hub_port,
                                    usb::endpoint_t ep_addr,
                                    usb::endpoint_type_t ep_type,
                                    usb::packet_size_t ep_max_packet_size,
                                    usb::polling_interval_t ep_interval) noexcept
      {
        return driver_->PipeCreate (dev_addr, dev_speed, hub_addr, hub_port,
                                    ep_addr, ep_type, ep_max_packet_size,
                                    ep_interval);
      }

      return_t
      Usbh_wrapper::do_modify_pipe (usb::pipe_t pipe,
                                    usb::device_address_t dev_addr,
                                    usb::speed_t dev_speed,
                                    usb::hub_addr_t hub_addr,
                                    usb::hub_port_t hub_port,
                                    usb::packet_size_t ep_max_packet_size) noexcept
      {
        return driver_->PipeModify (pipe, dev_addr, dev_speed, hub_addr,
                                    hub_port, ep_max_packet_size);
      }

      return_t
      Usbh_wrapper::do_delete_pipe (usb::pipe_t pipe) noexcept
      {
        return driver_->PipeDelete (pipe);
      }

      return_t
      Usbh_wrapper::do_reset_pipe (usb::pipe_t pipe) noexcept
      {
        return driver_->PipeReset (pipe);
      }

      return_t
      Usbh_wrapper::do_transfer (usb::pipe_t pipe, uint32_t packet,
                                 uint8_t* data, std::size_t num) noexcept
      {
        return driver_->PipeTransfer (pipe, packet, data, (uint32_t)num);
      }

      std::size_t
      Usbh_wrapper::do_get_transfer_count (usb::pipe_t pipe) noexcept
      {
        return driver_->PipeTransferGetResult (pipe);
      }

      return_t
      Usbh_wrapper::do_abort_transfer (usb::pipe_t pipe) noexcept
      {
        return driver_->PipeTransferAbort (pipe);
      }

      usb::frame_number_t
      Usbh_wrapper::do_get_frame_number (void) noexcept
      {
        return driver_->GetFrameNumber ();
      }

    } /* namespace driver */
  } /* namespace cmsis */
} /* namespace os */

// ----------------------------------------------------------------------------
