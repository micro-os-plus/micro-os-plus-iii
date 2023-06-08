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

#include <cmsis-plus/driver/usbd-wrapper.h>
#include <cmsis-plus/diag/trace.h>
#include <Driver_USBD.h>

#include <cassert>
#include <utility>
#include <cstdio>

// ----------------------------------------------------------------------------

namespace os
{
  namespace driver
  {
    // ----------------------------------------------------------------------

    usbd_wrapper::usbd_wrapper (
        ARM_DRIVER_USBD* driver, ARM_USBD_SignalDeviceEvent_t c_cb_device_func,
        ARM_USBD_SignalEndpointEvent_t c_cb_endpoint_func) noexcept :
    driver_ (driver),
    c_cb_device_func_ (c_cb_device_func),
    c_cb_endpoint_func_ (c_cb_endpoint_func)
      {
        trace::printf("%s() %p\n", __func__, this);
      }

    usbd_wrapper::~usbd_wrapper () noexcept
    {
      trace::printf ("%s() %p\n", __func__, this);

      driver_ = nullptr;
    }

    // ----------------------------------------------------------------------

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Waggregate-return"
#endif
#pragma GCC diagnostic ignored "-Wstrict-aliasing"

    const Version&
    usbd_wrapper::do_get_version (void) noexcept
    {
      // Overwrite the C++ instance. Assume same layout.
      *(reinterpret_cast<ARM_DRIVER_VERSION*> (&version_)) =
          driver_->GetVersion ();
      return version_;
    }

    const usb::device::Capabilities&
    usbd_wrapper::do_get_capabilities (void) noexcept
    {
      // Overwrite the C++ instance. Assume same layout.
      *(reinterpret_cast<ARM_USBD_CAPABILITIES*> (&capa_)) =
          driver_->GetCapabilities ();
      return capa_;
    }

    usb::device::Status&
    usbd_wrapper::do_get_status (void) noexcept
    {
      // Overwrite the C++ instance. Assume same layout.
      *(reinterpret_cast<ARM_USBD_STATE*> (&status_)) =
          driver_->DeviceGetState ();
      return status_;
    }

#pragma GCC diagnostic pop

    return_t
    usbd_wrapper::do_power (Power state) noexcept
    {
      assert (driver_ != nullptr);

      return_t status;

      if (state == Power::full)
        {
          status = driver_->Initialize (c_cb_device_func_, c_cb_endpoint_func_);
          if (status != ARM_DRIVER_OK)
            {
              return status;
            }
        }

      status = driver_->PowerControl (static_cast<ARM_POWER_STATE> (state));

      if (state == Power::off)
        {
          driver_->Uninitialize ();
        }

      return status;
    }

    return_t
    usbd_wrapper::do_connect (void) noexcept
    {
      return driver_->DeviceConnect ();
    }

    return_t
    usbd_wrapper::do_disconnect (void) noexcept
    {
      return driver_->DeviceDisconnect ();
    }

    return_t
    usbd_wrapper::do_wakeup_remote (void) noexcept
    {
      return driver_->DeviceRemoteWakeup ();
    }

    return_t
    usbd_wrapper::do_configure_address (usb::device_address_t dev_addr) noexcept
    {
      return driver_->DeviceSetAddress (dev_addr);
    }

    return_t
    usbd_wrapper::do_read_setup_packet (uint8_t* buf) noexcept
    {
      return driver_->ReadSetupPacket (buf);
    }

    usb::frame_number_t
    usbd_wrapper::do_get_frame_number (void) noexcept
    {
      return driver_->GetFrameNumber ();
    }

    return_t
    usbd_wrapper::do_configure_endpoint (usb::endpoint_t ep_addr,
                                         usb::Endpoint_type ep_type,
                                         usb::packet_size_t ep_max_packet_size) noexcept
    {
      return driver_->EndpointConfigure (ep_addr,
                                         static_cast<uint8_t> (ep_type),
                                         ep_max_packet_size);
    }

    return_t
    usbd_wrapper::do_unconfigure_endpoint (usb::endpoint_t ep_addr) noexcept
    {
      return driver_->EndpointUnconfigure (ep_addr);
    }

    return_t
    usbd_wrapper::do_stall_endpoint (usb::endpoint_t ep_addr, bool stall) noexcept
    {
      return driver_->EndpointStall (ep_addr, stall);
    }

    return_t
    usbd_wrapper::do_transfer (usb::endpoint_t ep_addr, uint8_t* data,
                               std::size_t num) noexcept
    {
      return driver_->EndpointTransfer (ep_addr, data,
                                        static_cast<uint32_t> (num));
    }

    std::size_t
    usbd_wrapper::do_get_transfer_count (usb::endpoint_t ep_addr) noexcept
    {
      return driver_->EndpointTransferGetResult (ep_addr);
    }

    return_t
    usbd_wrapper::do_abort_transfer (usb::endpoint_t ep_addr) noexcept
    {
      return driver_->EndpointTransferAbort (ep_addr);
    }

  } /* namespace driver */
} /* namespace os */

// ----------------------------------------------------------------------------
