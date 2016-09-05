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

#ifndef CMSIS_PLUS_MEMORY_NULL_H_
#define CMSIS_PLUS_MEMORY_NULL_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

#include <cmsis-plus/rtos/os.h>

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
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos-memres
     * @brief
     * An internal memory manager that throws a `bad_alloc()`
     * exception when trying to allocate.
     *
     * It is the default memory manager when running on bare metal
     * platforms, to prevent unwanted dynamic allocations,
     * unless another setting is done during startup.
     */
    class null_memory_resource : public rtos::memory::memory_resource
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Default constructor. Construct a memory manager object instance.
       */
      null_memory_resource () = default;

      /**
       * @cond ignore
       */

      // The rule of five.
      null_memory_resource (const null_memory_resource&) = delete;
      null_memory_resource (null_memory_resource&&) = delete;
      null_memory_resource&
      operator= (const null_memory_resource&) = delete;
      null_memory_resource&
      operator= (null_memory_resource&&) = delete;

      /**
       * @endcond
       */

      /**
       * @brief Destruct the memory manager object instance.
       */
      ~null_memory_resource () = default;

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

#pragma GCC diagnostic push
// Needed because 'alignment' is used only in trace calls.
#pragma GCC diagnostic ignored "-Wunused-parameter"

    inline void*
    null_memory_resource::do_allocate (std::size_t bytes, std::size_t alignment)
    {
      estd::__throw_bad_alloc ();
    }

    inline void
    null_memory_resource::do_deallocate (void* addr, std::size_t bytes,
                                         std::size_t alignment) noexcept
    {
      ;
    }

#pragma GCC diagnostic pop

  // ==========================================================================
  } /* namespace memory */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_MEMORY_NULL_H_ */
