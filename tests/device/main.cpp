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

#include "posix-io/PosixDevice.h"
#include <cerrno>
#include <cassert>
#include <cstdio>

// -----------------------------------------------------------------------------

// Mock class, all methods return ENOSYS, as not implemented.

class PosixDeviceTest : public os::PosixDevice
{
public:
  PosixDeviceTest (const char* deviceName, uint32_t deviceNumber);

private:

  virtual int
  doOpen (const char *path, int oflag, ...);

private:
  uint32_t fdeviceNumber;
};

PosixDeviceTest::PosixDeviceTest (const char* deviceName, uint32_t deviceNumber) :
    PosixDevice (deviceName)
{
  fdeviceNumber = deviceNumber;
}

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

int
PosixDeviceTest::doOpen (const char *path, int oflag, ...)
{
  errno = ENOSYS;
  return -1;
}

#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif

// -----------------------------------------------------------------------------

// This device will be mapped as "/dev/test"
PosixDeviceTest test ("test", 1);

// -----------------------------------------------------------------------------

int
main (int argc __attribute__((unused)), char* argv[] __attribute__((unused)))
{

  std::printf ("'test-device-debug' done.\n");

  // Success!
  return 0;
}

// -----------------------------------------------------------------------------

