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

#include "posix-io/FileDescriptorsManager.h"
#include "posix-io/io.h"
#include "posix-io/file.h"
#include "posix-io/TPool.h"
#include <cmsis-plus/diag/trace.h>

#include <cerrno>
#include <cassert>
#include <cstdio>
#include <cstdarg>

// ----------------------------------------------------------------------------

// Test class, all methods return ENOSYS, as not implemented, except open().

class TestFile : public os::posix::file
{
public:

  TestFile ();

  int
  do_vopen (const char* path, int oflag, std::va_list args);

private:

  uint32_t fSomething;

};

TestFile::TestFile ()
{
  fSomething = 1;
}

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

int
TestFile::do_vopen (const char* path, int oflag, std::va_list args)
{
  va_arg(args, int);

  return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

using TestFilePool = os::posix::pool_typed<TestFile>;

constexpr std::size_t POOL_ARRAY_SIZE = 2;

TestFilePool pool
  { POOL_ARRAY_SIZE };

// ----------------------------------------------------------------------------

int
main (int argc __attribute__((unused)), char* argv[] __attribute__((unused)))
{
  assert(pool.size () == POOL_ARRAY_SIZE);
  for (std::size_t i = 0; i < pool.size (); ++i)
    {
      assert(pool.object (i) != nullptr);
      assert(pool.in_use (i) == false);
    }

  TestFile* fil = pool.acquire ();
  assert(pool.in_use (0) == true);
  assert(fil == pool.object (0));

  // Release something not in array
  assert(pool.release (nullptr) == false);
  assert(pool.release (fil) == true);

  // Check if released
  assert(pool.in_use (0) == false);

  // Check full pool.
  for (std::size_t i = 0; i < pool.size (); ++i)
    {
      fil = pool.acquire ();
      assert(fil == pool.object (i));
    }

  // One more should return error
  fil = pool.acquire ();
  assert(fil == nullptr);

  trace_puts ("'test-pool-debug' succeeded\n");

  // Success!
  return 0;
}

// ----------------------------------------------------------------------------

