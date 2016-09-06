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

#include <cmsis-plus/memory/first-fit-top.h>
#include <cmsis-plus/memory/lifo.h>
#include <cmsis-plus/estd/memory_resource>

// ----------------------------------------------------------------------------

using namespace os;

// ----------------------------------------------------------------------------

extern "C" void
os_startup_initialize_free_store (void* heap_begin, void* heap_end);

#if defined(OS_TYPE_APPLICATION_MEMORY_RESOURCE)
using application_memory_resource = OS_TYPE_APPLICATION_MEMORY_RESOURCE;
#else
//using free_store_memory_resource = os::memory::lifo;
using application_memory_resource = os::memory::first_fit_top;
#endif

#if defined(OS_TYPE_RTOS_MEMORY_RESOURCE)
using rtos_memory_resource = OS_TYPE_RTOS_MEMORY_RESOURCE;
#else
using rtos_memory_resource = os::memory::lifo;
#endif

// Reserve storage for the application memory resource.
static std::aligned_storage<sizeof(application_memory_resource),
    alignof(application_memory_resource)>::type application_free_store;

extern "C" void*
sbrk (ptrdiff_t incr);

// ----------------------------------------------------------------------------

/**
 * @brief Initialise the dynamic allocation arenas.
 * @param heap_begin
 * @param heap_end
 * @par Returns
 *  Nothing.
 *
 * @details
 * This routine is called after the hardware is initialised, before
 * the static constructors, to initialise the application free store
 * and the RTOS dynamic memory (when OS_INTEGER_RTOS_DYNAMIC_MEMORY_SIZE_BYTES
 * is defined).
 *
 * If the RTOS is configured with its own memory, this area is
 * dynamically allocated on the application free store. The RTOS
 * memory resource (by default the one using LIFO) is also
 * dynamically allocated on the application free store. (If it
 * is _free_, why not use it; these areas are permanent anyway).
 *
 * For special applications, it is possible to override this
 * function entirely.
 */
void __attribute__((weak))
os_startup_initialize_free_store (void* heap_begin, void* heap_end)
{
  trace::printf ("%s(%p,%p)\n", __func__, heap_begin, heap_end);

  // Construct the memory resource used for the application free store.
  new (&application_free_store) application_memory_resource
    { heap_begin, heap_end };

  // Configure the memory manager to throw an exception when out of memory.
  reinterpret_cast<rtos::memory::memory_resource*> (&application_free_store)->out_of_memory_handler (
      estd::__throw_bad_alloc);

  // Set application memory manager.
  estd::pmr::set_default_resource (
      reinterpret_cast<estd::pmr::memory_resource*> (&application_free_store));

  // Adjust sbrk() to prevent it overlapping the free store.
  sbrk (static_cast<char*> (heap_end) - static_cast<char*> (sbrk (0)));

#if defined(OS_INTEGER_RTOS_DYNAMIC_MEMORY_SIZE_BYTES)

  // Allocate the RTOS dynamic memory on the application free store.
  void* rtos_arena =
      reinterpret_cast<rtos::memory::memory_resource*> (&application_free_store)->allocate (
      OS_INTEGER_RTOS_DYNAMIC_MEMORY_SIZE_BYTES);

  // Allocate & construct the memory resource used for the RTOS.
  rtos::memory::memory_resource* rtos_dynamic_memory;
  rtos_dynamic_memory = new rtos_memory_resource
    { rtos_arena, OS_INTEGER_RTOS_DYNAMIC_MEMORY_SIZE_BYTES };

  // Configure the memory manager to throw an exception when out of memory.
  rtos_dynamic_memory->out_of_memory_handler (estd::__throw_bad_alloc);

  // Set RTOS system memory manager.
  rtos::memory::set_default_resource (rtos_dynamic_memory);

#else

  // Set RTOS system memory manager.
  rtos::memory::set_default_resource (
      reinterpret_cast<rtos::memory::memory_resource*> (&application_free_store));

#endif /* defined(OS_INTEGER_RTOS_DYNAMIC_MEMORY_SIZE_BYTES) */

}

// ----------------------------------------------------------------------------

#endif /* defined(__ARM_EABI__) */
