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

#include "posix-io/FileDescriptorsManager.h"
#include "posix-io/PosixIo.h"
#include "posix-io/PosixDevice.h"
#include <cerrno>
#include <cassert>
#include <cstdio>
#include <cstring>

// -----------------------------------------------------------------------------

// Mock class, all methods return ENOSYS, as not implemented.

class TestPosixIo : public os::PosixIo
{
public:

  virtual int
  doOpen (const char *path, int oflag, va_list args);
};

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

int
TestPosixIo::doOpen (const char *path, int oflag, va_list args)
{
  errno = ENOSYS;
  return -1;
}

#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif

// -----------------------------------------------------------------------------

os::FileDescriptorsManager descriptorsManager
  { 5 };

TestPosixIo test1;
TestPosixIo test2;
TestPosixIo test3;

// -----------------------------------------------------------------------------

int
main (int argc __attribute__((unused)), char* argv[] __attribute__((unused)))
{
  size_t sz = os::FileDescriptorsManager::getSize ();
  // Size must be 5 for this test
  assert(sz == 5);

  for (std::size_t i = 0; i < sz; ++i)
    {
      assert(os::FileDescriptorsManager::getPosixIo (i) == nullptr);
    }

  // Check limits
  assert(os::FileDescriptorsManager::checkFileDescriptor (-1) == false);
  assert(os::FileDescriptorsManager::checkFileDescriptor (sz) == false);

  // Allocation should start with 3 (stdin, stdout, stderr preserved)
  int fd1;
  fd1 = os::FileDescriptorsManager::allocFileDescriptor (&test1);
  assert(fd1 == 3);

  // Get it back; is it the same?
  assert(os::FileDescriptorsManager::getPosixIo (fd1) == &test1);
  assert(test1.getFileDescriptor () == fd1);

  // Reallocate opened file, must be busy
  int fd2;
  fd2 = os::FileDescriptorsManager::allocFileDescriptor (&test1);
  assert((fd2 == -1) && (errno == EBUSY));

  // Free descriptor
  assert(os::FileDescriptorsManager::freeFileDescriptor (fd1) == 0);
  assert(os::FileDescriptorsManager::getPosixIo (fd1) == nullptr);
  assert(test1.getFileDescriptor () == os::noFileDescriptor);

  // With clean table, alloc to fill the table (size is 5)
  fd1 = os::FileDescriptorsManager::allocFileDescriptor (&test1);
  assert(fd1 == 3);
  fd2 = os::FileDescriptorsManager::allocFileDescriptor (&test2);
  assert(fd2 == 4);

  // Table full
  int fd3;
  fd3 = os::FileDescriptorsManager::allocFileDescriptor (&test3);
  assert((fd3 == -1) && (errno == ENFILE));

  // Free outside range
  assert(
      (os::FileDescriptorsManager::freeFileDescriptor(-1) == -1) && (errno == EBADF));
  assert(
      (os::FileDescriptorsManager::freeFileDescriptor(sz) == -1) && (errno == EBADF));

  // Free last
  assert(os::FileDescriptorsManager::freeFileDescriptor (sz - 1) == 0);

  // Reallocate last
  fd3 = os::FileDescriptorsManager::allocFileDescriptor (&test3);
  assert(fd3 == ((os::fileDescriptor_t )(sz - 1)));

  std::printf ("'test-descriptors-manager-debug' done.\n");

  // Success!
  return 0;
}

// -----------------------------------------------------------------------------

