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

#include "posix-io/file_descriptors_manager.h"
#include "posix-io/io.h"
#include "posix-io/device-char.h"
#include "posix-io/device_charsRegistry.h"
#include <cmsis-plus/diag/trace.h>

#include "posix/stropts.h"

#include <cerrno>
#include <cassert>
#include <cstdio>
#include <cstdarg>
#include <fcntl.h>

#if defined(__ARM_EABI__)
#include "posix-io/redefinitions.h"
#endif

// ----------------------------------------------------------------------------

enum class Cmds
  : unsigned int
    { UNKNOWN, NOTSET, OPEN, CLOSE, READ, WRITE, WRITEV, IOCTL
};

// Test class, all methods return ENOSYS, as not implemented, except open().

class TestDevice : public os::posix::device_char
{
public:

  TestDevice (const char* deviceName, uint32_t deviceNumber);

  void
  clear (void);

  Cmds
  getCmd (void);

  int
  getMode (void);

  unsigned int
  getNumber (void);

protected:

  virtual int
  do_vopen (const char* path, int oflag, va_list args) override;

  virtual int
  do_vioctl (int request, std::va_list args) override;

private:

  uint32_t fDeviceNumber;
  uint32_t fMode;
  unsigned int fNumber;
  Cmds fCmd;

};

TestDevice::TestDevice (const char* deviceName, uint32_t deviceNumber) :
    device_char (deviceName)
{
  fDeviceNumber = deviceNumber;

  clear ();
}

void
TestDevice::clear (void)
{
  fCmd = Cmds::NOTSET;
  fMode = 0;
  fNumber = 1;
}

inline int
TestDevice::getMode (void)
{
  return fMode;
}

inline Cmds
TestDevice::getCmd (void)
{
  return fCmd;
}

inline unsigned int
TestDevice::getNumber (void)
{
  return fNumber;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

int
TestDevice::do_vioctl (int request, std::va_list args)
{
  fCmd = Cmds::IOCTL;
  fNumber = request;
  fMode = va_arg(args, int);
  return 0;
}

int
TestDevice::do_vopen (const char* path, int oflag, va_list args)
{
  fMode = va_arg(args, int);

  return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#define DESCRIPTORS_ARRAY_SIZE (5)
os::posix::file_descriptors_manager descriptorsManager
  { DESCRIPTORS_ARRAY_SIZE };

#define DEVICES_ARRAY_SIZE (3)
os::posix::device_charsRegistry devicesRegistry
  { DEVICES_ARRAY_SIZE };

// This device will be mapped as "/dev/test"
TestDevice test
  { "test", 1 };

// ----------------------------------------------------------------------------

int
main (int argc __attribute__((unused)), char* argv[] __attribute__((unused)))
{
  std::size_t sz = os::posix::device_charsRegistry::size ();
  assert (sz == DEVICES_ARRAY_SIZE);

  // Check if initial status is empty.
  for (std::size_t i = 0; i < sz; ++i)
    {
      assert (os::posix::device_charsRegistry::device (i) == nullptr);
    }

  // Register device
  os::posix::device_charsRegistry::add (&test);

  // Check if first device is registered.
  assert (os::posix::device_charsRegistry::device (0) == &test);

    {
      // Test C++ API

      os::posix::io* io;
      io = os::posix::open ("/dev/test", 0, 123);
      assert ((io != nullptr) && (errno == 0));

      int fd;
      fd = io->file_descriptor ();

      // Get it back; is it the same?
      assert (os::posix::file_descriptors_manager::io (fd) == &test);

      // Check passing variadic mode.
      assert (test.getMode () == 123);

      // Test IOCTL
      errno = -2;
      int ret = test.ioctl (222, 876);
      assert ((ret == 0) && (errno == 0));
      assert (test.getCmd () == Cmds::IOCTL);
      assert (test.getNumber () == 222);
      assert (test.getMode () == 876);

      // Close and free descriptor.
      ret = io->close ();
      assert ((ret == 0) && (errno == 0));

      // Check if descriptor freed.
      assert (os::posix::file_descriptors_manager::io (fd) == nullptr);
      assert (test.file_descriptor () == os::posix::no_file_descriptor);
    }

    {
      // Test C API

      int fd = __posix_open ("/dev/test", 0, 234);
      assert ((fd >= 3) && (errno == 0));

      // Get it back; is it the same?
      assert (os::posix::file_descriptors_manager::io (fd) == &test);
      assert (test.file_descriptor () == fd);

      assert (test.get_type () == os::posix::io::type::device);

      // Check passing variadic mode.
      assert (test.getMode () == 234);

      // Test IOCTL
      errno = -2;
      int ret = __posix_ioctl (fd, 222, 876);
      assert ((ret == 0) && (errno == 0));
      assert (test.getCmd () == Cmds::IOCTL);
      assert (test.getNumber () == 222);
      assert (test.getMode () == 876);

      // Close and free descriptor
      ret = __posix_close (fd);
      assert ((ret == 0) && (errno == 0));

      // Check if descriptor freed.
      assert (os::posix::file_descriptors_manager::io (fd) == nullptr);
      assert (test.file_descriptor () == os::posix::no_file_descriptor);
    }

  trace_puts ("'test-device-debug' succeeded.");

  // Success!
  return 0;
}

// ----------------------------------------------------------------------------

