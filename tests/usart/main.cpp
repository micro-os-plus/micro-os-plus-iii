/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
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
    // Code to enter critical section
    // fStatus=ceva();
  }

  inline
  ~TestCriticalSection ()
  {
    // Code to exit critical section
    // altceva(fStatus);
  }

private:
  // uint32_t fStatus;
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

