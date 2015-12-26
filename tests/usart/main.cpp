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
#include "posix-io/FileDescriptorsManager.h"
#include "posix-io/CharDevicesRegistry.h"
#include "Driver_USART.h"

// Static manager
os::posix::FileDescriptorsManager descriptorsManager
  { 5 };

// Static manager
os::posix::CharDevicesRegistry devicesRegistry
  { 1 };

ARM_DRIVER_USART driver
  { };

extern "C" void
eventCallBack (uint32_t event);

// This device will be mapped as "/dev/usart"
os::posix::CmsisUsartCharDevice usart
  { "usart", &driver, eventCallBack };

void
eventCallBack (uint32_t event)
{
  // Forward event to driver instance.
  usart.eventCallBack (event);
}

int
main (int argc __attribute__((unused)), char* argv[] __attribute__((unused)))
{
  os::posix::CharDevicesRegistry::add (&usart);

  int fd = __posix_open ("/dev/usart", 0);

  __posix_close (fd);
}

