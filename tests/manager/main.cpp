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

#include <posix-io/FileDescriptorsManager.h>
#include "posix-io/PosixIo.h"
#include <cerrno>
#include <cassert>

// -----------------------------------------------------------------------------

// Mock class, all methods return ENOSYS, as not implemented.

class PosixTest : public PosixIo
{
public:
  PosixTest ();
  virtual
  ~PosixTest ();

  virtual ssize_t
  read (void *buf, size_t nbyte);

  virtual ssize_t
  write (const void *buf, size_t nbyte);

  virtual int
  ioctl (unsigned long request, ...);

  virtual int
  closeImplementation (void);
};

PosixTest::PosixTest ()
{

}

PosixTest::~PosixTest ()
{

}

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

ssize_t
PosixTest::read (void *buf, size_t nbyte)
{
  return ENOSYS;
}

ssize_t
PosixTest::write (const void *buf, size_t nbyte)
{
  return ENOSYS;
}

int
PosixTest::ioctl (unsigned long request, ...)
{
  return ENOSYS;
}

#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif

int
PosixTest::closeImplementation (void)
{
  return ENOSYS;
}

// -----------------------------------------------------------------------------

PosixTest test;
PosixTest test_2;
PosixTest test_3;

// -----------------------------------------------------------------------------

int
main (int argc __attribute__((unused)), char* argv[] __attribute__((unused)))
{
  size_t sz = FileDescriptorsManager::getSize ();
  // Size must be 5 for this test
  assert(sz == 5);

  // Check limits
  assert(FileDescriptorsManager::checkFileDescriptor (-1) == false);
  assert(FileDescriptorsManager::checkFileDescriptor (sz) == false);

  // Allocation should start with 3 (stdin, stdout, stderr preserved)
  fileDescriptor_t fd;
  fd = FileDescriptorsManager::allocFileDescriptor (&test);
  assert(fd == 3);

  // Get it back; is it the same?
  assert(FileDescriptorsManager::getPosixIo (fd) == &test);
  assert(test.getFileDescriptor () == fd);

  // Reallocate opened file, must be busy
  fileDescriptor_t fd_2;
  fd_2 = FileDescriptorsManager::allocFileDescriptor (&test);
  assert(fd_2 == EBUSY);

  // Free descriptor
  assert(FileDescriptorsManager::freeFileDescriptor (fd) == 0);
  assert(FileDescriptorsManager::getPosixIo (fd) == nullptr);
  assert(test.getFileDescriptor () == noFileDescriptor);

  // With clean table, alloc to fill the table (size is 5)
  fd = FileDescriptorsManager::allocFileDescriptor (&test);
  assert(fd == 3);
  fd_2 = FileDescriptorsManager::allocFileDescriptor (&test_2);
  assert(fd_2 == 4);

  // Table full
  fileDescriptor_t fd_3;
  fd_3 = FileDescriptorsManager::allocFileDescriptor (&test_3);
  assert(fd_3 == ENFILE);

  // Free outside range
  assert(FileDescriptorsManager::freeFileDescriptor(-1) == EBADF);
  assert(FileDescriptorsManager::freeFileDescriptor(sz) == EBADF);

  // Free last
  assert(FileDescriptorsManager::freeFileDescriptor (sz - 1) == 0);

  // Reallocate last
  fd_3 = FileDescriptorsManager::allocFileDescriptor (&test_3);
  assert(fd_3 == ((fileDescriptor_t )(sz - 1)));

  // Success!
  return 0;
}

// -----------------------------------------------------------------------------

