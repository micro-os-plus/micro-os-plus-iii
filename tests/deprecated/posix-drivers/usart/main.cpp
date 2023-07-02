/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#include "posix-drivers/CmsisUsartdevice_char.h"
#include "posix-drivers/TBufferedCmsisUsartdevice_char.h"
#include "posix-io/file-descriptors-manager.h"
#include "posix-io/device_charsRegistry.h"
#include "Driver_USART.h"

// Static manager
os::posix::file_descriptors_manager descriptorsManager
  { 5 };

// Static manager
os::posix::device_char_registry devicesRegistry
  { 2 };

// ----------------------------------------------------------------------------

ARM_DRIVER_USART driver1
  { };

extern "C" void
eventCallBack1 (uint32_t event);

// This device will be mapped as "/dev/usart"
os::posix::CmsisUsartdevice_char usart1
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

using BufferedCmsisUsartdevice_char =
os::posix::TBufferedCmsisUsartdevice_char<TestCriticalSection>;

ARM_DRIVER_USART driver2
  { };

uint8_t usart2_rx_buffer[200];
os::posix::ByteCircularBuffer usart2_rx_circular_buffer
  { usart2_rx_buffer, sizeof(usart2_rx_buffer) };

uint8_t usart2_tx_buffer[200];
os::posix::ByteCircularBuffer usart2_tx_circular_buffer
  { usart2_tx_buffer, sizeof(usart2_tx_buffer) };

extern "C" void
eventCallBack2 (uint32_t event);

// This device will be mapped as "/dev/usart2"
BufferedCmsisUsartdevice_char usart2
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
  os::posix::device_charsRegistry::add (&usart1);
  os::posix::device_charsRegistry::add (&usart2);

  int fd1 = __posix_open ("/dev/usart1", 0);
  __posix_close (fd1);

  int fd2 = __posix_open ("/dev/usart2", 0);
  __posix_close (fd2);

}

