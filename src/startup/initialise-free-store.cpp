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

#include <cmsis-plus/rtos/os-hooks.h>
#include <cmsis-plus/memory/first-fit-top.h>
#include <cmsis-plus/memory/lifo.h>
#include <cmsis-plus/memory/block-pool.h>
#include <cmsis-plus/estd/memory_resource>

// ----------------------------------------------------------------------------

using namespace os;

// ----------------------------------------------------------------------------

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

extern "C" void*
sbrk (ptrdiff_t incr);

// ----------------------------------------------------------------------------

#if !defined(OS_EXCLUDE_DYNAMIC_MEMORY_ALLOCATIONS)

// Reserve storage for the application memory resource.
static std::aligned_storage<sizeof(application_memory_resource),
    alignof(application_memory_resource)>::type application_free_store;

#endif /* !defined(OS_EXCLUDE_DYNAMIC_MEMORY_ALLOCATIONS) */

/**
 * @details
 * This routine is called after the hardware is initialised, before
 * the static constructors, to initialise the application free store
 * and the RTOS dynamic memory (when
 * `OS_INTEGER_RTOS_DYNAMIC_MEMORY_SIZE_BYTES` is defined).
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
os_startup_initialize_free_store (void* heap_address,
                                  std::size_t heap_size_bytes)
{
  trace::printf ("%s(%p,%u)\n", __func__, heap_address, heap_size_bytes);

#if !defined(OS_EXCLUDE_DYNAMIC_MEMORY_ALLOCATIONS)

  // Construct the memory resource used for the application free store.
  new (&application_free_store) application_memory_resource
    { "app", heap_address, heap_size_bytes };

  // Configure the memory manager to throw an exception when out of memory.
  reinterpret_cast<rtos::memory::memory_resource*> (&application_free_store)->out_of_memory_handler (
      os_rtos_application_out_of_memory_hook);

  // Set the application free store memory manager.
  estd::pmr::set_default_resource (
      reinterpret_cast<estd::pmr::memory_resource*> (&application_free_store));

  // Adjust sbrk() to prevent it overlapping the free store.
  sbrk (
      static_cast<char*> (static_cast<char*> (heap_address) + heap_size_bytes)
          - static_cast<char*> (sbrk (0)));

#if defined(OS_INTEGER_RTOS_DYNAMIC_MEMORY_SIZE_BYTES)

    {
      // Allocate the RTOS dynamic memory on the application free store.
      void* rtos_arena =
          reinterpret_cast<rtos::memory::memory_resource*> (&application_free_store)->allocate (
          OS_INTEGER_RTOS_DYNAMIC_MEMORY_SIZE_BYTES);

      // Allocate & construct the memory resource used for the RTOS.
      rtos::memory::memory_resource* mr = new rtos_memory_resource
        { "sys", rtos_arena, OS_INTEGER_RTOS_DYNAMIC_MEMORY_SIZE_BYTES };

      // Configure the memory manager to throw an exception when out of memory.
      mr->out_of_memory_handler (os_rtos_system_out_of_memory_hook);

      // Set RTOS system memory manager.
      rtos::memory::set_default_resource (mr);
    }

#else

  // The RTOS system memory manager is identical with the application one.
  rtos::memory::set_default_resource (
      reinterpret_cast<rtos::memory::memory_resource*> (&application_free_store));

#endif /* defined(OS_INTEGER_RTOS_DYNAMIC_MEMORY_SIZE_BYTES) */

#if defined(OS_INTEGER_RTOS_ALLOC_THREAD_POOL_SIZE)

    {
      static_assert(OS_INTEGER_RTOS_ALLOC_THREAD_POOL_SIZE > 1,
          "Mutex pool size must be >1.");
      rtos::memory::memory_resource* mr =
          new os::memory::block_pool_typed_inclusive<rtos::thread,
              OS_INTEGER_RTOS_ALLOC_THREAD_POOL_SIZE> ("pool-th");

      // Configure the memory manager to throw an exception when out of memory.
      mr->out_of_memory_handler (os_rtos_system_out_of_memory_hook);

      rtos::memory::set_resource_typed<rtos::thread> (mr);
    }

#endif /* defined(OS_INTEGER_RTOS_ALLOC_THREAD_POOL_SIZE) */

#if defined(OS_INTEGER_RTOS_ALLOC_CONDITION_VARIABLE_POOL_SIZE)

    {
      static_assert(OS_INTEGER_RTOS_ALLOC_CONDITION_VARIABLE_POOL_SIZE > 1,
          "Mutex pool size must be >1.");
      rtos::memory::memory_resource* mr =
          new os::memory::block_pool_typed_inclusive<rtos::condition_variable,
              OS_INTEGER_RTOS_ALLOC_CONDITION_VARIABLE_POOL_SIZE> ("pool-cv");

      // Configure the memory manager to throw an exception when out of memory.
      mr->out_of_memory_handler (os_rtos_system_out_of_memory_hook);

      rtos::memory::set_resource_typed<rtos::condition_variable> (mr);
    }

#endif /* defined(OS_INTEGER_RTOS_ALLOC_CONDITION_VARIABLE_POOL_SIZE) */

#if defined(OS_INTEGER_RTOS_ALLOC_EVENT_FLAGS_POOL_SIZE)

    {
      static_assert(OS_INTEGER_RTOS_ALLOC_EVENT_FLAGS_POOL_SIZE > 1,
          "Mutex pool size must be >1.");
      rtos::memory::memory_resource* mr =
          new os::memory::block_pool_typed_inclusive<rtos::event_flags,
              OS_INTEGER_RTOS_ALLOC_EVENT_FLAGS_POOL_SIZE> ("pool-ef");

      // Configure the memory manager to throw an exception when out of memory.
      mr->out_of_memory_handler (os_rtos_system_out_of_memory_hook);

      rtos::memory::set_resource_typed<rtos::event_flags> (mr);
    }

#endif /* defined(OS_INTEGER_RTOS_ALLOC_EVENT_FLAGS_POOL_SIZE) */

#if defined(OS_INTEGER_RTOS_ALLOC_MEMORY_POOL_POOL_SIZE)

    {
      static_assert(OS_INTEGER_RTOS_ALLOC_MEMORY_POOL_POOL_SIZE > 1,
          "Mutex pool size must be >1.");
      rtos::memory::memory_resource* mr =
          new os::memory::block_pool_typed_inclusive<rtos::memory_pool,
              OS_INTEGER_RTOS_ALLOC_MEMORY_POOL_POOL_SIZE> ("pool-mp");

      // Configure the memory manager to throw an exception when out of memory.
      mr->out_of_memory_handler (os_rtos_system_out_of_memory_hook);

      rtos::memory::set_resource_typed<rtos::memory_pool> (mr);
    }

#endif /* defined(OS_INTEGER_RTOS_ALLOC_MEMORY_POOL_POOL_SIZE) */

#if defined(OS_INTEGER_RTOS_ALLOC_MESSAGE_QUEUE_POOL_SIZE)

    {
      static_assert(OS_INTEGER_RTOS_ALLOC_MESSAGE_QUEUE_POOL_SIZE > 1,
          "Mutex pool size must be >1.");
      rtos::memory::memory_resource* mr =
          new os::memory::block_pool_typed_inclusive<rtos::message_queue,
              OS_INTEGER_RTOS_ALLOC_MESSAGE_QUEUE_POOL_SIZE> ("pool-mq");

      // Configure the memory manager to throw an exception when out of memory.
      mr->out_of_memory_handler (os_rtos_system_out_of_memory_hook);

      rtos::memory::set_resource_typed<rtos::message_queue> (mr);
    }

#endif /* defined(OS_INTEGER_RTOS_ALLOC_MESSAGE_QUEUE_POOL_SIZE) */

#if defined(OS_INTEGER_RTOS_ALLOC_MUTEX_POOL_SIZE)

    {
      static_assert(OS_INTEGER_RTOS_ALLOC_MUTEX_POOL_SIZE > 1,
          "Mutex pool size must be >1.");
      rtos::memory::memory_resource* mr =
          new os::memory::block_pool_typed_inclusive<rtos::mutex,
              OS_INTEGER_RTOS_ALLOC_MUTEX_POOL_SIZE> ("pool-mx");

      // Configure the memory manager to throw an exception when out of memory.
      mr->out_of_memory_handler (os_rtos_system_out_of_memory_hook);

      rtos::memory::set_resource_typed<rtos::mutex> (mr);
    }

#endif /* defined(OS_INTEGER_RTOS_ALLOC_MUTEX_POOL_SIZE) */

#if defined(OS_INTEGER_RTOS_ALLOC_SEMAPHORE_POOL_SIZE)

    {
      static_assert(OS_INTEGER_RTOS_ALLOC_SEMAPHORE_POOL_SIZE > 1,
          "Semaphore pool size must be >1.");
      rtos::memory::memory_resource* mr =
          new os::memory::block_pool_typed_inclusive<rtos::semaphore,
              OS_INTEGER_RTOS_ALLOC_MUTEX_POOL_SIZE> ("pool-sp");

      // Configure the memory manager to throw an exception when out of memory.
      mr->out_of_memory_handler (os_rtos_system_out_of_memory_hook);

      rtos::memory::set_resource_typed<rtos::semaphore> (mr);
    }

#endif /* defined(OS_INTEGER_RTOS_ALLOC_SEMAPHORE_POOL_SIZE) */

#if defined(OS_INTEGER_RTOS_ALLOC_TIMER_POOL_SIZE)

    {
      static_assert(OS_INTEGER_RTOS_ALLOC_TIMER_POOL_SIZE > 1,
          "Mutex pool size must be >1.");
      rtos::memory::memory_resource* mr =
          new os::memory::block_pool_typed_inclusive<rtos::timer,
              OS_INTEGER_RTOS_ALLOC_TIMER_POOL_SIZE> ("pool-tm");

      // Configure the memory manager to throw an exception when out of memory.
      mr->out_of_memory_handler (os_rtos_system_out_of_memory_hook);

      rtos::memory::set_resource_typed<rtos::timer> (mr);
    }

#endif /* defined(OS_INTEGER_RTOS_ALLOC_TIMER_POOL_SIZE) */

#endif /* !defined(OS_EXCLUDE_DYNAMIC_MEMORY_ALLOCATIONS) */
}

/**
 * @details
 * This function is called when the application memory manager detects
 * an out of memory condition.
 *
 * This function is usually used to gracefully reset the device.
 *
 * However, for special memory managers, which do not coalesce
 * automatically, it might be possible to first try to coalesce.
 * If this succeeds, this call can return, and the allocation will be
 * resumed.
 *
 * @note Since most allocations are done in critical sections,
 * this function is very likely to be called with the scheduler locked.
 */
void __attribute__((weak))
os_rtos_application_out_of_memory_hook (void)
{
  estd::__throw_bad_alloc ();
}

#if defined(OS_INTEGER_RTOS_DYNAMIC_MEMORY_SIZE_BYTES)

/**
 * @details
 * This function is called when the RTOS system memory manager detects
 * an out of memory condition.
 *
 * This function is usually used to gracefully reset the device.
 *
 * However, for special memory managers, which do not coalesce
 * automatically, it might be possible to first try to coalesce.
 * If this succeeds, this call can return, and the allocation will be
 * resumed.
 *
 * @note Since most allocations are done in critical sections,
 * this function is very likely to be called with the scheduler locked.
 */
void __attribute__((weak))
os_rtos_system_out_of_memory_hook (void)
{
  estd::__throw_bad_alloc ();
}

#endif

// ----------------------------------------------------------------------------

#endif /* defined(__ARM_EABI__) */
