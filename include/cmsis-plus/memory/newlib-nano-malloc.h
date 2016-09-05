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

#ifndef CMSIS_PLUS_MEMORY_NEWLIB_NANO_MALLOC_H_
#define CMSIS_PLUS_MEMORY_NEWLIB_NANO_MALLOC_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

#include <cmsis-plus/rtos/os.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace memory
  {

    // ========================================================================

    /**
     * @brief Memory resource implementing the newlib nano allocation/deallocation policies.
     * @ingroup cmsis-plus-rtos-memres
     * @headerfile newlib-nano-malloc.h <cmsis-plus/memory/newlib-nano-malloc.h>
     */
    class newlib_nano_malloc : public rtos::memory::memory_resource
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
      newlib_nano_malloc (void* addr_begin, void* addr_end);

      /**
       * @brief Construct a memory resource object instance.
       * @param addr Begin of allocator arena.
       * @param bytes Size of allocator arena, in bytes.
       */
      newlib_nano_malloc (void* addr, std::size_t bytes);

      /**
       * @cond ignore
       */

      // The rule of five.
      newlib_nano_malloc (const newlib_nano_malloc&) = delete;
      newlib_nano_malloc (newlib_nano_malloc&&) = delete;
      newlib_nano_malloc&
      operator= (const newlib_nano_malloc&) = delete;
      newlib_nano_malloc&
      operator= (newlib_nano_malloc&&) = delete;

      /**
       * @endcond
       */

      /**
       * @brief Destruct the memory resource object instance.
       */
      virtual
      ~newlib_nano_malloc ();

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
       * @brief Implementation of the function to get max size.
       * @par Parameters
       *  None
       * @return Integer with size in bytes, or 0 if unknown.
       */
      virtual std::size_t
      do_max_size (void) const noexcept override;

      /**
       * @brief Implementation of the function to reset the memory manager.
       * @par Parameters
       *  None
       * @par Returns
       *  Nothing.
       */
      virtual void
      do_reset (void) noexcept override;

      /**
       * @}
       */

    protected:

      /**
       * @cond ignore
       */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

      typedef struct chunk_s
      {
        // The actual chunk size, in bytes;
        // exactly after it comes the next chunk.
        std::size_t size;
        // When chunk is in the free list, pointer to next chunk, .
        // At this address, or slightly further if alignment was
        // required, starts the payload.
        struct chunk_s* next;
      } chunk_t;

#pragma GCC diagnostic pop

      // Offset of payload inside the chunk.
      static constexpr std::size_t chunk_offset = offsetof(chunk_t, next);
      static constexpr std::size_t chunk_align = sizeof(void*);

      static constexpr std::size_t block_align = alignof(std::max_align_t);
      static constexpr std::size_t block_minsize = sizeof(void *);
      static constexpr std::size_t block_maxsize = 1024 * 1024;
      static constexpr std::size_t block_padding = os::rtos::memory::max (
          block_align, chunk_align) - chunk_align;
      static constexpr std::size_t block_minchunk = chunk_offset + block_padding
          + block_minsize;

      void* addr_;

      chunk_t* free_list_;

      /**
       * @endcond
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
    newlib_nano_malloc::newlib_nano_malloc (void* addr_begin, void* addr_end) :
        newlib_nano_malloc
          { addr_begin, static_cast<std::size_t> (static_cast<char*> (addr_end)
              - static_cast<char*> (addr_begin)) }
    {
      assert(addr_begin < addr_end);
    }

  // --------------------------------------------------------------------------

  } /* namespace memory */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_MEMORY_NEWLIB_NANO_MALLOC_H_ */
