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

#ifndef CMSIS_PLUS_MEMORY_LIFO_H_
#define CMSIS_PLUS_MEMORY_LIFO_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

#include <cmsis-plus/memory/first-fit-top.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace memory
  {

    // ========================================================================

    /**
     * @brief Memory resource implementing the LIFO
     * allocation/deallocation policies.
     * @ingroup cmsis-plus-rtos-memres
     * @headerfile lifo.h <cmsis-plus/memory/lifo.h>
     */
    class lifo : public os::memory::first_fit_top
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct a memory resource object instance.
       * @param addr_begin Begin of allocator arena.
       * @param addr_end End of allocator arena.
       */
      lifo (void* addr_begin, void* addr_end);

      /**
       * @brief Construct a named memory resource object instance.
       * @param name Pointer to name.
       * @param addr_begin Begin of allocator arena.
       * @param addr_end End of allocator arena.
       */
      lifo (const char* name, void* addr_begin, void* addr_end);

      /**
       * @brief Construct a memory resource object instance.
       * @param addr Begin of allocator arena.
       * @param bytes Size of allocator arena, in bytes.
       */
      lifo (void* addr, std::size_t bytes);

      /**
       * @brief Construct a named memory resource object instance.
       * @param name Pointer to name.
       * @param addr Begin of allocator arena.
       * @param bytes Size of allocator arena, in bytes.
       */
      lifo (const char* name, void* addr, std::size_t bytes);

      /**
       * @cond ignore
       */

      // The rule of five.
      lifo (const lifo&) = delete;
      lifo (lifo&&) = delete;
      lifo&
      operator= (const lifo&) = delete;
      lifo&
      operator= (lifo&&) = delete;

      /**
       * @endcond
       */

      /**
       * @brief Destruct the memory resource object instance.
       */
      virtual
      ~lifo ();

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
       * @}
       */

    };

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
    lifo::lifo (const char* name, void* addr, std::size_t bytes) :
        first_fit_top
          { name, addr, bytes }
    {
      trace::printf ("%s(%p,%u) @%p %s\n", __func__, addr, bytes, this,
                     this->name ());
    }

    inline
    lifo::lifo (const char* name, void* addr_begin, void* addr_end) :
        first_fit_top
          { name, addr_begin, addr_end }
    {
      assert(addr_begin < addr_end);

      trace::printf ("%s(%p,%p) @%p %s\n", __func__, addr_begin, addr_end, this,
                     this->name ());
    }

    inline
    lifo::lifo (void* addr_begin, void* addr_end) :
        lifo
          { nullptr, addr_begin, addr_end }
    {
      ;
    }

    inline
    lifo::lifo (void* addr, std::size_t bytes) :
        first_fit_top
          { nullptr, addr, bytes }
    {
      ;
    }

  // --------------------------------------------------------------------------

  } /* namespace memory */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_MEMORY_LIFO_H_ */
