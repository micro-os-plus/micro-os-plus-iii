/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
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

#include "posix-drivers/CmsisUsartCharDevice.h"
#include "posix-drivers/TBufferedCmsisUsartCharDevice.h"
#include "posix-io/FileDescriptorsManager.h"
#include "posix-io/CharDevicesRegistry.h"
#include "Driver_USART.h"

// Static manager
os::posix::FileDescriptorsManager descriptorsManager
  { 5 };

// Static manager
os::posix::CharDevicesRegistry devicesRegistry
  { 2 };

// ----------------------------------------------------------------------------

ARM_DRIVER_USART driver1
  { };

extern "C" void
eventCallBack1 (uint32_t event);

// This device will be mapped as "/dev/usart"
os::posix::CmsisUsartCharDevice usart1
  { "usart1", &driver1, eventCallBack1 };

void
eventCallBack1 (uint32_t event)
{
  // Forward event to driver instance.
  usart1.eventCallBack (event);
}

// ----------------------------------------------------------------------------

class TestCriticalSection
{
public:
  inline
  TestCriticalSection ()
  {
  }

  inline
  ~TestCriticalSection ()
  {
  }
};

using BufferedCmsisUsartCharDevice =
os::dev::TBufferedCmsisUsartCharDevice<TestCriticalSection>;

ARM_DRIVER_USART driver2
  { };

uint8_t usart2_rx_buffer[200];
os::dev::ByteCircularBuffer usart2_rx_circular_buffer
  { usart2_rx_buffer, sizeof(usart2_rx_buffer) };

uint8_t usart2_tx_buffer[200];
os::dev::ByteCircularBuffer usart2_tx_circular_buffer
  { usart2_tx_buffer, sizeof(usart2_tx_buffer) };

extern "C" void
eventCallBack2 (uint32_t event);

// This device will be mapped as "/dev/usart2"
BufferedCmsisUsartCharDevice usart2
  { "usart2", &driver2, eventCallBack2, &usart2_rx_circular_buffer,
      &usart2_tx_circular_buffer };

void
eventCallBack2 (uint32_t event)
{
  // Forward event to driver instance.
  usart1.eventCallBack (event);
}

// ----------------------------------------------------------------------------

int
main (int argc __attribute__((unused)), char* argv[] __attribute__((unused)))
{
  os::posix::CharDevicesRegistry::add (&usart1);
  os::posix::CharDevicesRegistry::add (&usart2);

  int fd1 = __posix_open ("/dev/usart1", 0);
  __posix_close (fd1);

  int fd2 = __posix_open ("/dev/usart2", 0);
  __posix_close (fd2);

}

