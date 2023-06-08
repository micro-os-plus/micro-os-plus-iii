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

#include <cmsis-plus/driver/usb-device.h>
#include <cmsis-plus/driver/usbd-wrapper.h>

#include <cassert>

using namespace os::driver;

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

  virtual return_t
  do_power (Power state) noexcept override;

  virtual return_t
  do_connect (void) noexcept override;

  virtual return_t
  do_disconnect (void) noexcept override;

  virtual usb::device::Status&
  do_get_status (void) noexcept override;

  virtual return_t
  do_wakeup_remote (void) noexcept override;

  virtual return_t
  do_configure_address (usb::device_address_t dev_addr) noexcept override;

  virtual return_t
  do_read_setup_packet (uint8_t* buf) noexcept override;

  virtual usb::frame_number_t
  do_get_frame_number (void) noexcept override;

  virtual return_t
  do_configure_endpoint (usb::endpoint_t ep_addr, usb::Endpoint_type ep_type,
                         usb::packet_size_t ep_max_packet_size)
                             noexcept override;

  virtual return_t
  do_unconfigure_endpoint (usb::endpoint_t ep_addr) noexcept override;

  virtual return_t
  do_stall_endpoint (usb::endpoint_t ep_addr, bool stall) noexcept override;

  virtual return_t
  do_transfer (usb::endpoint_t ep_addr, uint8_t* data, std::size_t num)
      noexcept override;

  virtual std::size_t
  do_get_transfer_count (usb::endpoint_t ep_addr) noexcept override;

  virtual return_t
  do_abort_transfer (usb::endpoint_t ep_addr) noexcept override;

private:

  Version version_
    { 0x0123, 0x0124 };

  usb::device::Capabilities capabilities_
    { true, false, true };
};

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

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

return_t
Test_usb_device::do_power (Power state) noexcept
{
  return RETURN_OK;
}

return_t
Test_usb_device::do_connect (void) noexcept
{
  return RETURN_OK;
}

return_t
Test_usb_device::do_disconnect (void) noexcept
{
  return RETURN_OK;
}

usb::device::Status&
Test_usb_device::do_get_status (void) noexcept
{
  return status_;
}

return_t
Test_usb_device::do_wakeup_remote (void) noexcept
{
  return RETURN_OK;
}

return_t
Test_usb_device::do_configure_address (usb::device_address_t dev_addr) noexcept
{
  return RETURN_OK;
}

return_t
Test_usb_device::do_read_setup_packet (uint8_t* buf) noexcept
{
  return RETURN_OK;
}

usb::frame_number_t
Test_usb_device::do_get_frame_number (void) noexcept
{
  return 0;
}

return_t
Test_usb_device::do_configure_endpoint (usb::endpoint_t ep_addr,
                                        usb::Endpoint_type ep_type,
                                        usb::packet_size_t ep_max_packet_size) noexcept
{
  return RETURN_OK;
}

return_t
Test_usb_device::do_unconfigure_endpoint (usb::endpoint_t ep_addr) noexcept
{
  return RETURN_OK;
}

return_t
Test_usb_device::do_stall_endpoint (usb::endpoint_t ep_addr, bool stall) noexcept
{
  return RETURN_OK;
}

return_t
Test_usb_device::do_transfer (usb::endpoint_t ep_addr, uint8_t* data,
                              std::size_t num) noexcept
{
  return RETURN_OK;
}

std::size_t
Test_usb_device::do_get_transfer_count (usb::endpoint_t ep_addr) noexcept
{
  return 0;
}

return_t
Test_usb_device::do_abort_transfer (usb::endpoint_t ep_addr) noexcept
{
  return RETURN_OK;
}

#pragma GCC diagnostic pop

static Test_usb_device device;

void
test_usbd (void);

extern ARM_DRIVER_USBD test_usbd_driver;

static Usbd_wrapper wrap
  { &test_usbd_driver, nullptr, nullptr };

void
test_usbd (void)
{
  Version version;
  version = device.get_version ();
  assert(version.get_api () == 0x0123);
  assert(version.get_drv () == 0x0124);

  usb::device::Capabilities capa;
  capa = device.get_capabilities ();
  assert(capa.vbus_detection);
  assert(!capa.event_vbus_on);
  assert(capa.event_vbus_off);

  version = wrap.get_version ();
  assert(version.get_api () == 0x1234);
  assert(version.get_drv () == 0x5678);

  capa = wrap.get_capabilities ();
  assert(capa.vbus_detection);
  assert(capa.event_vbus_on);
  assert(!capa.event_vbus_off);

  usb::device::Status status = wrap.get_status ();

  assert(!status.is_vbus_on ());
  assert(status.get_speed () == 2);
  assert(status.is_active ());

  assert(wrap.power (Power::full) == 11);
  assert(wrap.power (Power::off) == 22);
  assert(wrap.power (Power::low) == 33);

  assert(wrap.connect () == 4);
  assert(wrap.disconnect () == 5);
  assert(wrap.wakeup_remote () == 6);
  assert(wrap.configure_address (31) == 31);

  uint8_t buf[10];
  assert(wrap.read_setup_packet (buf) == 7);

  assert(wrap.configure_endpoint (2, usb::Endpoint_type::control, 1) == 2);
  assert(wrap.unconfigure_endpoint (3) == 3);
  assert(wrap.stall_endpoint (4, true) == 4);

  assert(wrap.transfer (5, buf, sizeof(buf)) == 5);
  assert(wrap.transfer (5, buf, 0) == RETURN_OK);

  assert(wrap.get_transfer_count (6) == 6);

  assert(wrap.abort_transfer (7) == 7);

  assert(wrap.get_frame_number () == 8);

}
