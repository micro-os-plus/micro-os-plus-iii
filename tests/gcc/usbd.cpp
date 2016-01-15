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

#include <cmsis-plus/drivers/usb-device.h>
#include <cassert>

using namespace os::cmsis::driver;

class Test_usb_device : public usb::Device
{
public:

  Test_usb_device () = default;

  virtual
  ~Test_usb_device () = default;

protected:

  virtual const Version&
  do_get_version (void) noexcept override;

  const usb::device::Capabilities&
  do_get_capabilities (void) noexcept override;

  virtual status_t
  do_power (Power state) noexcept override;

  virtual status_t
  do_connect (void) noexcept override;

  virtual status_t
  do_disconnect (void) noexcept override;

  virtual usb::device::Status&
  do_get_status (void) noexcept override;

  virtual status_t
  do_wakeup_remote (void) noexcept override;

  virtual status_t
  do_configure_address (usb::device::device_address_t dev_addr)
      noexcept override;

  virtual status_t
  do_read_setup_packet (uint8_t* buf) noexcept override;

  virtual usb::device::frame_number_t
  do_get_frame_number (void) noexcept override;

  virtual status_t
  do_configure_endpoint (usb::endpoint_t ep_addr, usb::endpoint_type_t ep_type,
                         usb::packet_size_t ep_max_packet_size)
                             noexcept override;

  virtual status_t
  do_unconfigure_endpoint (usb::endpoint_t ep_addr) noexcept override;

  virtual status_t
  do_stall_endpoint (usb::endpoint_t ep_addr, bool stall) noexcept override;

  virtual status_t
  do_transfer (usb::endpoint_t ep_addr, uint8_t* data, std::size_t num)
      noexcept override;

  virtual std::size_t
  do_get_transfer_count (usb::endpoint_t ep_addr) noexcept override;

  virtual status_t
  do_abort_transfer (usb::endpoint_t ep_addr) noexcept override;

private:

  Version version_
    { 0x0123, 0x0124 };

  usb::device::Capabilities capabilities_
    { true, false, true };
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

const Version&
Test_usb_device::do_get_version (void) noexcept
{
  return version_;
}

const usb::device::Capabilities&
Test_usb_device::do_get_capabilities (void) noexcept
{
  return capabilities_;
}

status_t
Test_usb_device::do_power (Power state) noexcept
{
  return STATUS_OK;
}

status_t
Test_usb_device::do_connect (void) noexcept
{
  return STATUS_OK;
}

status_t
Test_usb_device::do_disconnect (void) noexcept
{
  return STATUS_OK;
}

usb::device::Status&
Test_usb_device::do_get_status (void) noexcept
{
  return status_;
}

status_t
Test_usb_device::do_wakeup_remote (void) noexcept
{
  return STATUS_OK;
}

status_t
Test_usb_device::do_configure_address (usb::device::device_address_t dev_addr) noexcept
{
  return STATUS_OK;
}

status_t
Test_usb_device::do_read_setup_packet (uint8_t* buf) noexcept
{
  return STATUS_OK;
}

usb::device::frame_number_t
Test_usb_device::do_get_frame_number (void) noexcept
{
  return 0;
}

status_t
Test_usb_device::do_configure_endpoint (usb::endpoint_t ep_addr,
                                        usb::endpoint_type_t ep_type,
                                        usb::packet_size_t ep_max_packet_size) noexcept
{
  return STATUS_OK;
}

status_t
Test_usb_device::do_unconfigure_endpoint (usb::endpoint_t ep_addr) noexcept
{
  return STATUS_OK;
}

status_t
Test_usb_device::do_stall_endpoint (usb::endpoint_t ep_addr, bool stall) noexcept
{
  return STATUS_OK;
}

status_t
Test_usb_device::do_transfer (usb::endpoint_t ep_addr, uint8_t* data,
                              std::size_t num) noexcept
{
  return STATUS_OK;
}

std::size_t
Test_usb_device::do_get_transfer_count (usb::endpoint_t ep_addr) noexcept
{
  return 0;
}

status_t
Test_usb_device::do_abort_transfer (usb::endpoint_t ep_addr) noexcept
{
  return STATUS_OK;
}

#pragma GCC diagnostic pop

Test_usb_device usbd_test;

void
test_usbd (void);

void
test_usbd (void)
{
  Version version;
  version = usbd_test.get_version ();
  assert(version.get_api () == 0x0123);
  assert(version.get_drv () == 0x0124);

  usb::device::Capabilities capa;
  capa = usbd_test.get_capabilities ();
  assert(capa.vbus_detection);
  assert(!capa.event_vbus_on);
  assert(capa.event_vbus_off);

}
