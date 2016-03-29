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

#include <cmsis-plus/drivers/usb-host.h>
#include <cmsis-plus/drivers/usbh-wrapper.h>

#include <cassert>

using namespace os::driver;

class Test_usb_host : public usb::Host
{
public:

  Test_usb_host () = default;

  virtual
  ~Test_usb_host () = default;

protected:

  virtual const Version&
  do_get_version (void) noexcept override;

  const usb::host::Capabilities&
  do_get_capabilities (void) noexcept override;

  virtual return_t
  do_power (Power state) noexcept override;

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

private:

  Version version_
    { 0x0123, 0x0124 };

  usb::host::Capabilities capabilities_
    { 7, true, false, false, true };
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

const Version&
Test_usb_host::do_get_version (void) noexcept
{
  return version_;
}

const usb::host::Capabilities&
Test_usb_host::do_get_capabilities (void) noexcept
{
  return capabilities_;
}

return_t
Test_usb_host::do_power (Power state) noexcept
{
  return RETURN_OK;
}

return_t
Test_usb_host::do_power_port_vbus (usb::port_t port, bool vbus) noexcept
{
  return RETURN_OK;
}

return_t
Test_usb_host::do_reset_port (usb::port_t port) noexcept
{
  return RETURN_OK;
}

return_t
Test_usb_host::do_suspend_port (usb::port_t port) noexcept
{
  return RETURN_OK;
}

return_t
Test_usb_host::do_resume_port (usb::port_t port) noexcept
{
  return RETURN_OK;
}

usb::host::Status&
Test_usb_host::do_get_port_status (usb::port_t port) noexcept
{
  return status_;
}

usb::pipe_t
Test_usb_host::do_create_pipe (usb::device_address_t dev_addr,
                               usb::speed_t dev_speed, usb::hub_addr_t hub_addr,
                               usb::hub_port_t hub_port,
                               usb::endpoint_t ep_addr,
                               usb::endpoint_type_t ep_type,
                               usb::packet_size_t ep_max_packet_size,
                               usb::polling_interval_t ep_interval) noexcept
{
  return 0;
}

return_t
Test_usb_host::do_modify_pipe (usb::pipe_t pipe, usb::device_address_t dev_addr,
                               usb::speed_t dev_speed, usb::hub_addr_t hub_addr,
                               usb::hub_port_t hub_port,
                               usb::packet_size_t ep_max_packet_size) noexcept
{
  return RETURN_OK;
}

return_t
Test_usb_host::do_delete_pipe (usb::pipe_t pipe) noexcept
{
  return RETURN_OK;
}

return_t
Test_usb_host::do_reset_pipe (usb::pipe_t pipe) noexcept
{
  return RETURN_OK;
}

return_t
Test_usb_host::do_transfer (usb::pipe_t pipe, uint32_t packet, uint8_t* data,
                            std::size_t num) noexcept
{
  return RETURN_OK;
}

std::size_t
Test_usb_host::do_get_transfer_count (usb::pipe_t pipe) noexcept
{
  return 0;
}

return_t
Test_usb_host::do_abort_transfer (usb::pipe_t pipe) noexcept
{
  return RETURN_OK;
}

uint16_t
Test_usb_host::do_get_frame_number (void) noexcept
{
  return 0;
}

#pragma GCC diagnostic pop

static Test_usb_host device;

void
test_usbh (void);

extern ARM_DRIVER_USBH test_usbh_driver;

static Usbh_wrapper wrap
  { &test_usbh_driver, nullptr, nullptr };

void
test_usbh (void)
{
  Version version;
  version = device.get_version ();
  assert(version.get_api () == 0x0123);
  assert(version.get_drv () == 0x0124);

  usb::host::Capabilities capa;
  capa = device.get_capabilities ();
  assert(capa.port_mask == 7);
  assert(capa.auto_split);
  assert(!capa.event_connect);
  assert(!capa.event_disconnect);
  assert(capa.event_overcurrent);

  version = wrap.get_version ();
  assert(version.get_api () == 0x1234);
  assert(version.get_drv () == 0x5678);

  capa = wrap.get_capabilities ();
  assert(capa.port_mask == 77);
  assert(capa.auto_split);
  assert(capa.event_connect);
  assert(!capa.event_disconnect);
  assert(!capa.event_overcurrent);

  usb::host::Status status = wrap.get_port_status (1);

  assert(!status.is_connected ());
  assert(status.is_overcurrent ());
  assert(status.get_speed () == 2);

  assert(wrap.power (Power::full) == 11);
  assert(wrap.power (Power::off) == 22);
  assert(wrap.power (Power::low) == 33);

  assert(wrap.power_port_vbus (6, true) == 6 + 1);
  assert(wrap.reset_port (7) == 7 + 2);
  assert(wrap.suspend_port (7) == 7 + 3);
  assert(wrap.resume_port (7) == 7 + 4);
  assert(wrap.create_pipe (7, 1, 2, 3, 4, 5, 6, 7) == 7 + 5);
  assert(wrap.modify_pipe (7, 1, 2, 3, 4, 5) == 7 + 6);
  assert(wrap.delete_pipe (7) == 7 + 7);
  assert(wrap.reset_pipe (7) == 7 + 8);

  uint8_t buf[10];
  assert(wrap.transfer (7, 1, buf, 10) == 7 + 9);

  assert(wrap.get_transfer_count (7) == 7 + 10);
  assert(wrap.abort_transfer (7) == 7 + 11);
  assert(wrap.get_frame_number () == 9);

}
