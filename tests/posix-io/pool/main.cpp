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
#pragma GCC diagnostic ignored "-Wunused-parameter"

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

  TestFile* fil = pool.aquire ();
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
      fil = pool.aquire ();
      assert(fil == pool.object (i));
    }

  // One more should return error
  fil = pool.aquire ();
  assert(fil == nullptr);

  trace_puts ("'test-pool-debug' succeeded.\n");

  // Success!
  return 0;
}

// ----------------------------------------------------------------------------

