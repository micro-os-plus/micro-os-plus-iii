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

#if defined(__ARM_EABI__)

// ----------------------------------------------------------------------------

#include <cmsis-plus/memory/newlib-nano-malloc.h>

// ----------------------------------------------------------------------------

using namespace os;

// ----------------------------------------------------------------------------

extern "C" void
os_startup_initialize_free_store (void* heap_begin, void* heap_end);

using free_store_memory_resource = os::memory::newlib_nano_malloc;

static std::aligned_storage<sizeof(free_store_memory_resource),
    alignof(free_store_memory_resource)>::type free_store;

// ----------------------------------------------------------------------------

void __attribute__((weak))
os_startup_initialize_free_store (void* heap_begin, void* heap_end)
{
  trace::printf ("Free store (heap): %p-%p\n", heap_begin, heap_end);

  new (&free_store) free_store_memory_resource
    { heap_begin, heap_end };

  // Set application memory manager.
  estd::set_default_resource (
      reinterpret_cast<estd::memory_resource*> (&free_store));

  // Set RTOS memory manager.
  rtos::memory::set_default_resource (
      reinterpret_cast<rtos::memory::memory_resource*> (&free_store));
}

// ----------------------------------------------------------------------------

#endif /* defined(__ARM_EABI__) */
