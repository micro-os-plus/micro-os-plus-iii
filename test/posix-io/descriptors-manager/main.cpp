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

#include "posix-io/file_descriptors_manager.h"
#include "posix-io/io.h"
#include "posix-io/device-char.h"
#include <cmsis-plus/diag/trace.h>

#include <cerrno>
#include <cassert>
#include <cstdio>
#include <cstring>

// ----------------------------------------------------------------------------

// Mock class, all methods return ENOSYS, as not implemented.

class TestIO : public os::posix::io
{
public:

  virtual int
  do_vopen (const char *path, int oflag, va_list args);

};

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

int
TestIO::do_vopen (const char *path, int oflag, va_list args)
{
  errno = ENOSYS;
  return -1;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

// Size must be 5 for this test.
constexpr std::size_t FD_MANAGER_ARRAY_SIZE = 5;

os::posix::file_descriptors_manager descriptorsManager
  { FD_MANAGER_ARRAY_SIZE };

TestIO test1;
TestIO test2;
TestIO test3;

// ----------------------------------------------------------------------------

int
main (int argc __attribute__((unused)), char* argv[] __attribute__((unused)))
{
  size_t sz = os::posix::file_descriptors_manager::size ();
  // Size must be 5 for this test
  assert (sz == FD_MANAGER_ARRAY_SIZE);

  for (std::size_t i = 0; i < sz; ++i)
    {
      assert (os::posix::file_descriptors_manager::io (i) == nullptr);
    }

  // Check limits.
  assert (os::posix::file_descriptors_manager::valid (-1) == false);
  assert (os::posix::file_descriptors_manager::valid (sz) == false);

  // Allocation should start with 3 (stdin, stdout, stderr preserved).
  int fd1;
  fd1 = os::posix::file_descriptors_manager::alloc (&test1);
  assert (fd1 == 3);

  // Get it back; is it the same?
  assert (os::posix::file_descriptors_manager::io (fd1) == &test1);
  assert (test1.file_descriptor () == fd1);

  // Reallocate opened file, must be busy.
  int fd2;
  fd2 = os::posix::file_descriptors_manager::alloc (&test1);
  assert ((fd2 == -1) && (errno == EBUSY));

  // Free descriptor.
  assert (os::posix::file_descriptors_manager::free (fd1) == 0);
  assert (os::posix::file_descriptors_manager::io (fd1) == nullptr);
  assert (test1.file_descriptor () == os::posix::no_file_descriptor);

  // With clean table, alloc repeatedly to fill the table (size is 5).
  fd1 = os::posix::file_descriptors_manager::alloc (&test1);
  assert (fd1 == 3);
  fd2 = os::posix::file_descriptors_manager::alloc (&test2);
  assert (fd2 == 4);

  // Table full.
  int fd3;
  fd3 = os::posix::file_descriptors_manager::alloc (&test3);
  assert ((fd3 == -1) && (errno == ENFILE));

  // Free outside range.
  assert (
      (os::posix::file_descriptors_manager::free (-1) == -1) && (errno == EBADF));
  assert (
      (os::posix::file_descriptors_manager::free (sz) == -1) && (errno == EBADF));

  // Free last.
  assert (os::posix::file_descriptors_manager::free (sz - 1) == 0);

  // Reallocate last.
  fd3 = os::posix::file_descriptors_manager::alloc (&test3);
  assert (fd3 == ((os::posix::file_descriptor_t) (sz - 1)));

  trace_puts ("'test-descriptors-manager-debug' done.");

  // Success!
  return 0;
}

// ----------------------------------------------------------------------------

