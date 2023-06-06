/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
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

#include <cmsis-plus/rtos/os.h>
#include <test-cpp-mem.h>
#include <cmsis-plus/estd/memory_resource>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

#if 0
#if 0
extern unsigned int _Heap_Begin;
extern unsigned int _Heap_Limit;

os::estd::memory::newlib_nano mr
  { &_Heap_Begin, (&_Heap_Limit - &_Heap_Begin) * sizeof(unsigned int)};
#else

char arena[1000];

os::rtos::memory::newlib_nano_malloc mr
  { arena, sizeof(arena) };

#endif

os::estd::polymorphic_allocator<char, os::rtos::scheduler::lockable> a
  { &mr };

#pragma GCC diagnostic pop
#endif

int
test_cpp_mem (void)
{
#if 0
  char* p1 = a.allocate (19);

  char* p2 = a.allocate (18);

  char* p3 = a.allocate (17);

  char* p4 = a.allocate (18);

  a.deallocate (p2, 20);

  a.deallocate (p3, 20);

  char* p5 = a.allocate (19);

  a.deallocate (p1, 20);

  a.deallocate (p4, 20);

  a.deallocate (p5, 20);
#endif
  return 0;
}
// ----------------------------------------------------------------------------
