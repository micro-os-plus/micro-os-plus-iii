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

#ifndef CMSIS_PLUS_MEMORY_MALLOC_H_
#define CMSIS_PLUS_MEMORY_MALLOC_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/rtos/os.h>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace memory
  {

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif

    // ========================================================================

    /**
     * @headerfile os.h <cmsis-plus/memory/malloc.h>
     * @ingroup cmsis-plus-rtos-memres
     * @brief
     * A memory manager that allocates memory via
     * the system `std::malloc()` and deallocates via `std::free()`.
     *
     * It is the default memory manager when running on synthetic
     * POSIX platforms.
     *
     * @warning This memory manager is not thread safe.
     */
    class malloc_memory_resource : public rtos::memory::memory_resource
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Default constructor. Construct a memory manager object instance.
       */
      malloc_memory_resource ();

      /**
       * @brief Construct a named memory manager object instance.
       */
      malloc_memory_resource (const char* name);

      /**
       * @cond ignore
       */

      malloc_memory_resource (const malloc_memory_resource&) = delete;
      malloc_memory_resource (malloc_memory_resource&&) = delete;
      malloc_memory_resource&
      operator= (const malloc_memory_resource&) = delete;
      malloc_memory_resource&
      operator= (malloc_memory_resource&&) = delete;

      /**
       * @endcond
       */

      /**
       * @brief Destruct the memory manager object instance.
       */
      ~malloc_memory_resource () override;

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Functions
       * @{
       */

      /**
       * @brief Implementation of the memory allocator.
       * @param bytes Number of bytes to allocate.
       * @param alignment Alignment constraint (power of 2).
       * @return Pointer to newly allocated block, or `nullptr`.
       */
      virtual void*
      do_allocate (std::size_t bytes, std::size_t alignment) override;

      /**
       * @brief Implementation of the memory deallocator.
       * @param addr Address of a previously allocated block to free.
       * @param bytes Number of bytes to deallocate (may be 0 if unknown).
       * @param alignment Alignment constraint (power of 2).
       * @par Returns
       *  Nothing.
       */
      virtual void
      do_deallocate (void* addr, std::size_t bytes, std::size_t alignment)
          noexcept override;

      /**
       * @}
       */
    };

    // ======================================================================

    /**
     * @headerfile os.h <cmsis-plus/memory/malloc.h>
     * @ingroup cmsis-plus-rtos-memres
     * @brief
     * A memory manager that allocates memory via
     * the system `operator new` and deallocates via `operator delete`.
     *
     * This is a memory resource defined by the ISO C++ standard.
     *
     * @note This memory manager is thread safe.
     */
    class new_delete_memory_resource : public rtos::memory::memory_resource
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Destruct the memory manager object instance.
       */
      ~new_delete_memory_resource () override = default;

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Functions
       * @{
       */

      /**
       * @brief Implementation of the memory allocator.
       * @param bytes Number of bytes to allocate.
       * @param alignment Alignment constraint (power of 2).
       * @return Pointer to newly allocated block, or `nullptr`.
       */
      virtual void*
      do_allocate (size_t bytes, size_t alignment) override;

      /**
       * @brief Implementation of the memory deallocator.
       * @param addr Address of a previously allocated block to free.
       * @param bytes Number of bytes to deallocate (may be 0 if unknown).
       * @param alignment Alignment constraint (power of 2).
       * @par Returns
       *  Nothing.
       */
      virtual void
      do_deallocate (void* addr, size_t bytes, size_t alignment)
          noexcept override;

      /**
       * @}
       */

    };

#pragma GCC diagnostic pop

  // -------------------------------------------------------------------------
  } /* namespace memory */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace memory
  {

    // ========================================================================

    inline
    malloc_memory_resource::malloc_memory_resource ()
    {
      trace::printf ("%s() @%p %s\n", __func__, this, this->name ());
    }

    inline
    malloc_memory_resource::malloc_memory_resource (const char* name) :
        rtos::memory::memory_resource
          { name }
    {
      trace::printf ("%s() @%p %s\n", __func__, this, this->name ());
    }

    inline
    malloc_memory_resource::~malloc_memory_resource ()
    {
      trace::printf ("%s() @%p %s\n", __func__, this, this->name ());
    }

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
// Needed because 'alignment' is used only in trace calls.
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

    inline void*
    malloc_memory_resource::do_allocate (std::size_t bytes,
                                         std::size_t alignment)
    {
      // Ignore alignment for now.
      void* mem = std::malloc (bytes);
#if defined(OS_TRACE_LIBCPP_MEMORY_RESOURCE)
      trace::printf ("%s(%u,%u)=%p @%p %s\n", __func__, bytes, alignment, mem,
                     this, name ());
#endif

      return mem;
    }

    inline void
    malloc_memory_resource::do_deallocate (void* addr, std::size_t bytes,
                                           std::size_t alignment) noexcept
    {
#if defined(OS_TRACE_LIBCPP_MEMORY_RESOURCE)
      trace::printf ("%s(%p,%u,%u) @%p %s\n", __func__, addr, bytes, alignment,
                     this, name ());
#endif
      // Ignore size and alignment for now.
      std::free (addr);
    }

#pragma GCC diagnostic pop

    // ========================================================================

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
// Needed because 'alignment' is used only in trace calls.
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

    inline void*
    new_delete_memory_resource::do_allocate (size_t bytes, size_t alignment)
    {
      // Ignore alignment for now.
      void* mem = ::operator new (bytes);
#if defined(OS_TRACE_LIBCPP_MEMORY_RESOURCE)
      trace::printf ("%s(%u,%u)=%p @%p %s\n", __func__, bytes, alignment, mem,
                     this, name ());
#endif
      allocated_chunks_++;
      return mem;
    }

    inline void
    new_delete_memory_resource::do_deallocate (void* addr, size_t bytes,
                                               size_t alignment) noexcept
    {
#if defined(OS_TRACE_LIBCPP_MEMORY_RESOURCE)
      trace::printf ("%s(%p,%u,%u) @%p %s\n", __func__, addr, bytes, alignment,
                     this, name ());
#endif
      // Ignore size and alignment for now.
      ::operator delete (addr);
      allocated_chunks_--;
    }

#pragma GCC diagnostic pop

  // ==========================================================================
  } /* namespace memory */
} /* namespace os */

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_MEMORY_MALLOC_H_ */
