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
#include "posix-io/PosixFile.h"
#include "posix-io/PosixDevice.h"
#include "posix-io/PosixDevicesRegistry.h"
#include "posix-io/TPosixPool.h"
#include <cerrno>
#include <cassert>
#include <cstdio>
#include <cstdarg>
#if defined(OS_INCLUDE_TRACE_PRINTF)
#include "diag/Trace.h"
#endif

// -----------------------------------------------------------------------------

// Test class, all methods return ENOSYS, as not implemented, except open().

class TestPosixFile : public os::PosixFile
{
public:

  TestPosixFile ();

  int
  do_open (const char* path, int oflag, std::va_list args);

private:

  uint32_t fSomething;

};

TestPosixFile::TestPosixFile ()
{
  fSomething = 1;
}

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

int
TestPosixFile::do_open (const char* path, int oflag, std::va_list args)
{
  va_arg(args, int);

  return 0;
}

#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif

// -----------------------------------------------------------------------------

using TestFilePool = os::TPosixPool<TestPosixFile>;

constexpr std::size_t POOL_ARRAY_SIZE = 2;

TestFilePool pool
  { POOL_ARRAY_SIZE };

// -----------------------------------------------------------------------------

int
main (int argc __attribute__((unused)), char* argv[] __attribute__((unused)))
{
  assert(pool.getSize () == POOL_ARRAY_SIZE);
  for (std::size_t i = 0; i < pool.getSize (); ++i)
    {
      assert(pool.getObject (i) != nullptr);
      assert(pool.getFlag (i) == false);
    }

  TestPosixFile* fil = pool.aquire ();
  assert(pool.getFlag (0) == true);
  assert(fil == pool.getObject (0));

  // Release something not in array
  assert(pool.release (nullptr) == false);
  assert(pool.release (fil) == true);

  // Check if released
  assert(pool.getFlag (0) == false);

  // Check full pool.
  for (std::size_t i = 0; i < pool.getSize (); ++i)
    {
      fil = pool.aquire ();
      assert(fil == pool.getObject (i));
    }

  // One more should return error
  fil = pool.aquire ();
  assert(fil == nullptr);

  const char* msg = "'test-pool-debug' succeeded.\n";
#if defined(OS_INCLUDE_TRACE_PRINTF)
  trace_puts (msg);
#else
  std::puts (msg);
#endif

  // Success!
  return 0;
}

// -----------------------------------------------------------------------------

