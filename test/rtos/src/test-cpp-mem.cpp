/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#include <cmsis-plus/rtos/os.h>
#include <test-cpp-mem.h>
#include <cmsis-plus/estd/memory_resource>

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
