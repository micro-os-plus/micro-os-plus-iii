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

/*
 * [Partly inspired from the LLVM libcxx sources].
 * Copyright (c) 2009-2013 by the contributors listed in
 * 'LLVM libcxx Credits.txt'. See 'LLVM libcxx License.txt' for details.
 *
 * References are to ISO/IEC 14882:2011(E) Third edition (2011-09-01).
 */

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/estd/memory_resource>
#include <cmsis-plus/memory/malloc.h>
#include <cmsis-plus/memory/null.h>

// ----------------------------------------------------------------------------

using namespace os;

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    namespace memory
    {
      // ======================================================================

      /**
       * @cond ignore
       */

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

      // The memory resources must not be destructed, since some
      // static objects will want to deallocate memory they manage.

      static std::aligned_storage<sizeof(os::memory::malloc_memory_resource),
          alignof(os::memory::malloc_memory_resource)>::type malloc_res;

      static std::aligned_storage<sizeof(os::memory::null_memory_resource),
          alignof(os::memory::null_memory_resource)>::type null_res;

      static std::aligned_storage<
          sizeof(os::memory::new_delete_memory_resource),
          alignof(os::memory::new_delete_memory_resource)>::type new_delete_res;

      static void
      __attribute__((constructor))
      __init (void)
      {
        new (&malloc_res) os::memory::malloc_memory_resource ("malloc");
        new (&null_res) os::memory::null_memory_resource ();
        new (&new_delete_res) os::memory::new_delete_memory_resource ();
      }

      // allocator_pool<mutex> allocator_mutex_instance;

#pragma GCC diagnostic pop

      /**
       * @endcond
       */

      /**
       * @cond ignore
       */

      // The default RTOS system memory resource.
#if defined(__APPLE__) || defined(__linux__)

      memory_resource* default_resource __attribute__((weak))
      = reinterpret_cast<memory_resource*> (&malloc_res);

      // ----------------------------------------------------------------------

      memory_resource* resource_thread __attribute__((weak))
      = reinterpret_cast<memory_resource*> (&malloc_res);

      memory_resource* resource_condition_variable __attribute__((weak))
      = reinterpret_cast<memory_resource*> (&malloc_res);

      memory_resource* resource_event_flags __attribute__((weak))
      = reinterpret_cast<memory_resource*> (&malloc_res);

      memory_resource* resource_memory_pool __attribute__((weak))
      = reinterpret_cast<memory_resource*> (&malloc_res);

      memory_resource* resource_message_queue __attribute__((weak))
      = reinterpret_cast<memory_resource*> (&malloc_res);

      memory_resource* resource_mutex __attribute__((weak))
      = reinterpret_cast<memory_resource*> (&malloc_res);

      memory_resource* resource_semaphore __attribute__((weak))
      = reinterpret_cast<memory_resource*> (&malloc_res);

      memory_resource* resource_timer __attribute__((weak))
      = reinterpret_cast<memory_resource*> (&malloc_res);

#else

      memory_resource* default_resource __attribute__((weak))
      = reinterpret_cast<memory_resource*> (&null_res);

      // ----------------------------------------------------------------------

      memory_resource* resource_thread __attribute__((weak))
      = reinterpret_cast<memory_resource*> (&null_res);

      memory_resource* resource_condition_variable __attribute__((weak))
      = reinterpret_cast<memory_resource*> (&null_res);

      memory_resource* resource_event_flags __attribute__((weak))
      = reinterpret_cast<memory_resource*> (&null_res);

      memory_resource* resource_memory_pool __attribute__((weak))
      = reinterpret_cast<memory_resource*> (&null_res);

      memory_resource* resource_message_queue __attribute__((weak))
      = reinterpret_cast<memory_resource*> (&null_res);

      memory_resource* resource_mutex __attribute__((weak))
      = reinterpret_cast<memory_resource*> (&null_res);

      memory_resource* resource_semaphore __attribute__((weak))
      = reinterpret_cast<memory_resource*> (&null_res);

      memory_resource* resource_timer __attribute__((weak))
      = reinterpret_cast<memory_resource*> (&null_res);

#endif

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------
      /**
       * @details
       *
       * @see malloc_memory_resource
       */
      memory_resource*
      malloc_resource (void) noexcept
      {
        return reinterpret_cast<memory_resource*> (&malloc_res);
      }

      // ----------------------------------------------------------------------

      /**
       * @details
       * On bare metal applications, this function is called
       * from `os_startup_initialize_free_store()`, during the
       * system startup, with a memory manager handling the free
       * RAM.
       *
       * @warning This function is not thread safe.
       */
      memory_resource*
      set_default_resource (memory_resource* res) noexcept
      {
        trace::printf ("rtos::memory::%s(%p) \n", __func__, res);

        memory_resource* old = default_resource;
        default_resource = res;

        return old;
      }

      // ----------------------------------------------------------------------

      /**
       * @details
       * On bare metal applications, this function is called
       * from `os_startup_initialize_free_store()`, during the
       * system startup, with a memory manager specific to this object type.
       */
      template<>
        memory_resource*
        set_resource_typed<thread> (memory_resource* res) noexcept
        {
          trace::printf ("rtos::memory::%s(%p) \n", __func__, res);

          memory_resource* old = resource_thread;
          resource_thread = res;

          return old;
        }

      /**
       * @details
       * On bare metal applications, this function is called
       * from `os_startup_initialize_free_store()`, during the
       * system startup, with a memory manager specific to this object type.
       */
      template<>
        memory_resource*
        set_resource_typed<condition_variable> (memory_resource* res) noexcept
        {
          trace::printf ("rtos::memory::%s(%p) \n", __func__, res);

          memory_resource* old = resource_condition_variable;
          resource_condition_variable = res;

          return old;
        }

      /**
       * @details
       * On bare metal applications, this function is called
       * from `os_startup_initialize_free_store()`, during the
       * system startup, with a memory manager specific to this object type.
       */
      template<>
        memory_resource*
        set_resource_typed<event_flags> (memory_resource* res) noexcept
        {
          trace::printf ("rtos::memory::%s(%p) \n", __func__, res);

          memory_resource* old = resource_event_flags;
          resource_event_flags = res;

          return old;
        }

      /**
       * @details
       * On bare metal applications, this function is called
       * from `os_startup_initialize_free_store()`, during the
       * system startup, with a memory manager specific to this object type.
       */
      template<>
        memory_resource*
        set_resource_typed<memory_pool> (memory_resource* res) noexcept
        {
          trace::printf ("rtos::memory::%s(%p) \n", __func__, res);

          memory_resource* old = resource_memory_pool;
          resource_memory_pool = res;

          return old;
        }

      /**
       * @details
       * On bare metal applications, this function is called
       * from `os_startup_initialize_free_store()`, during the
       * system startup, with a memory manager specific to this object type.
       */
      template<>
        memory_resource*
        set_resource_typed<message_queue> (memory_resource* res) noexcept
        {
          trace::printf ("rtos::memory::%s(%p) \n", __func__, res);

          memory_resource* old = resource_message_queue;
          resource_message_queue = res;

          return old;
        }

      /**
       * @details
       * On bare metal applications, this function is called
       * from `os_startup_initialize_free_store()`, during the
       * system startup, with a memory manager specific to this object type.
       */
      template<>
        memory_resource*
        set_resource_typed<mutex> (memory_resource* res) noexcept
        {
          trace::printf ("rtos::memory::%s(%p) \n", __func__, res);

          memory_resource* old = resource_mutex;
          resource_mutex = res;

          return old;
        }

      /**
       * @details
       * On bare metal applications, this function is called
       * from `os_startup_initialize_free_store()`, during the
       * system startup, with a memory manager specific to this object type.
       */
      template<>
        memory_resource*
        set_resource_typed<semaphore> (memory_resource* res) noexcept
        {
          trace::printf ("rtos::memory::%s(%p) \n", __func__, res);

          memory_resource* old = resource_semaphore;
          resource_semaphore = res;

          return old;
        }

      /**
       * @details
       * On bare metal applications, this function is called
       * from `os_startup_initialize_free_store()`, during the
       * system startup, with a memory manager specific to this object type.
       */
      template<>
        memory_resource*
        set_resource_typed<timer> (memory_resource* res) noexcept
        {
          trace::printf ("rtos::memory::%s(%p) \n", __func__, res);

          memory_resource* old = resource_timer;
          resource_timer = res;

          return old;
        }

      // ======================================================================

      /**
       * @details
       */
      memory_resource::~memory_resource ()
      {
        ;
      }

      /**
       * @fn memory_resource::do_allocate()
       * @details
       * Allocates storage with a size of at least bytes bytes.
       * The returned storage is aligned to the specified alignment
       * if such alignment is supported, and to `alignof(std::max_align_t)`
       * otherwise.
       *
       * The function should return a `nullptr`
       * when no more memory is available.
       *
       * @par Exceptions
       *   The code itself should throw nothing, but if the out of memory
       *   handler is set, it may throw a `bad_alloc()` exception.
       *
       */

      /**
       * @fn memory_resource::do_deallocate()
       * @details
       * Deallocates the storage pointed to by `addr`.
       * The address must have been returned by a prior call to
       * `allocate(bytes, alignment)` on a memory_resource that
       * compares equal to `*this`, and the storage it points to
       * must not yet have been deallocated, otherwise the behaviour
       * is undefined.
       *
       * @par Exceptions
       *   Should throw nothing itself.
       */

      /**
       * @details
       * Compares `*this` for equality with `other`.
       * Two memory_resources compare equal if and only if memory
       * allocated from one memory_resource can be deallocated from
       * the other and vice versa.
       *
       * The most-derived type of other may not match the most derived
       * type of `*this`. A derived class implementation therefore must
       * typically check whether the most derived types of `*this` and
       * other match using dynamic_cast, and immediately return false
       * if the cast fails.
       *
       * @par Exceptions
       *   Throws nothing.
       */
      bool
      memory_resource::do_is_equal (memory_resource const &other) const noexcept
      {
        return &other == this;
      }

      /**
       * @details
       * The default implementation of this virtual function returns
       * zero, meaning the size is not known.
       *
       * Override this function to return the actual size.
       *
       * @par Standard compliance
       *   Extension to standard.
       */
      std::size_t
      memory_resource::do_max_size (void) const noexcept
      {
        return 0;
      }

      /**
       * @details
       * The default implementation of this virtual function
       * does nothing.
       *
       * Override this function to perform the action.
       *
       * @par Standard compliance
       *   Extension to standard.
       */
      void
      memory_resource::do_reset (void) noexcept
      {
        return;
      }

      /**
       * @details
       * The default implementation of this virtual function returns
       * false, meaning the operation was ineffective.
       *
       * Override this function to perform the action.
       *
       * @par Standard compliance
       *   Extension to standard.
       */
      bool
      memory_resource::do_coalesce (void) noexcept
      {
        return false;
      }

      void
      memory_resource::internal_increase_allocated_statistics (
          std::size_t bytes) noexcept
      {
        // Update statistics.
        // What is subtracted from free is added to allocated.
        allocated_bytes_ += bytes;
        if (allocated_bytes_ > max_allocated_bytes_)
          {
            max_allocated_bytes_ = allocated_bytes_;
          }
        free_bytes_ -= bytes;
        ++allocated_chunks_;
        --free_chunks_;
      }

      void
      memory_resource::internal_decrease_allocated_statistics (
          std::size_t bytes) noexcept
      {
        // Update statistics.
        // What is subtracted from allocated is added to free.
        allocated_bytes_ -= bytes;
        free_bytes_ += bytes;
        --allocated_chunks_;
        ++free_chunks_;

      }

    // ------------------------------------------------------------------------

    } /* namespace memory */
  } /* namespace rtos */
} /* namespace os */

namespace os
{
  namespace estd
  {
    namespace pmr
    {
      /**
       * @cond ignore
       */

      memory_resource*
      new_delete_resource (void) noexcept
      {
        return reinterpret_cast<memory_resource*> (&rtos::memory::new_delete_res);
      }

      memory_resource*
      null_memory_resource (void) noexcept
      {
        return reinterpret_cast<memory_resource*> (&rtos::memory::null_res);
      }

#if defined(__APPLE__) || defined(__linux__)
      memory_resource* default_resource __attribute__((weak))
      = reinterpret_cast<memory_resource*> (&rtos::memory::malloc_res);
#else
      memory_resource* default_resource __attribute__((weak))
      = reinterpret_cast<memory_resource*> (&rtos::memory::null_res);
#endif

      /**
       * @endcond
       */
      ;
    // Avoid formatter bug
    } /* namespace pmr */
  } /* namespace estd */
} /* namespace os */

// ----------------------------------------------------------------------------
