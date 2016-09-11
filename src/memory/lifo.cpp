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

#include <cmsis-plus/memory/lifo.h>
#include <memory>

// ----------------------------------------------------------------------------

namespace os
{
  namespace memory
  {

    // ========================================================================

    /**
     * @details
     */
    lifo::~lifo ()
    {
      trace::printf ("%s() @%p %s\n", __func__, this, this->name ());
    }

#pragma GCC diagnostic push
// Needed because 'alignment' is used only in trace calls.
#pragma GCC diagnostic ignored "-Wunused-parameter"

    /**
     * @details
     * The allocator is deterministic and fast, it always uses
     * the top part of the first block (memory is allocated
     * top-down). If this block is not large enough, the
     * allocation fails; the free list is never traversed, since
     * this is no longer deterministic.
     *
     * Deallocating in reverse order restores the first block
     * to its original glory. A slight reorder of deallocations
     * is allowed, but memory is not reused until the previous
     * adjacent block is freed, and so on recursively, increasing
     * back the size of the first block.
     *
     * With the given top-down allocation policy, recent block have always
     * lower addresses.
     *
     * Similarly, a block is reused only after all more recently
     * allocated blocks are freed.
     *
     * @par Exceptions
     *   Throws nothing itself, but the out of memory handler may
     *   throw `bad_alloc()`.
     */
    void*
    lifo::do_allocate (std::size_t bytes, std::size_t alignment)
    {
      // TODO: consider `alignment` if > block_align.

      std::size_t alloc_size = rtos::memory::align_size (bytes, chunk_align);
      alloc_size += block_padding;
      alloc_size += chunk_offset;

      alloc_size = os::rtos::memory::max (alloc_size, block_minchunk);

      chunk_t* chunk = nullptr;

      while (true)
        {
          // Allocate only from the first block and only if it is
          // the really first in the arena; this prevents fragmentation.
          if (free_list_ == reinterpret_cast<chunk_t*> (arena_addr_))
            {
              chunk = free_list_;

              int rem = static_cast<int> (chunk->size - alloc_size);
              if (rem >= 0)
                {
                  if ((static_cast<std::size_t> (rem)) >= block_minchunk)
                    {
                      // If the chunk is larger than needed
                      // (at least one more chunk is available);
                      // break it into two chunks and return the top one.

                      // Shrink bottom chunk to remaining size.
                      chunk->size = static_cast<std::size_t> (rem);

                      // Compute where top chunk starts.
                      chunk =
                          reinterpret_cast<chunk_t *> (reinterpret_cast<char *> (chunk)
                              + rem);
                      chunk->size = alloc_size;

                      // Splitting one chunk creates one more chunk.
                      ++free_chunks_;
                    }
                  else
                    {
                      // Found a chunk that is exactly the size or slightly
                      // larger than the requested size; return this chunk.

                      // Always at the list head.
                      // The next chunk becomes the first list element.
                      free_list_ = chunk->next;

                      // If this was the last chunk, the free list is empty.
                    }
                }
            }

          if (chunk != nullptr)
            {
              break;
            }

          if (out_of_memory_handler_ == nullptr)
            {
#if defined(OS_TRACE_LIBCPP_MEMORY_RESOURCE)
              trace::printf ("%s(%u,%u)=0 @%p %s\n", __func__, bytes, alignment,
                             this, this->name ());
#endif

              return nullptr;
            }

#if defined(OS_TRACE_LIBCPP_MEMORY_RESOURCE)
          trace::printf ("%s(%u,%u) @%p %s out of memory\n", __func__, bytes,
                         alignment, this, this->name ());
#endif
          out_of_memory_handler_ ();

          // If the handler returned, assume it freed some memory
          // and try again to allocate.
        }

      // Update statistics.
      // What is subtracted from free is added to allocated.
      internal_increase_allocated_statistics (chunk->size);

      // Compute pointer to payload area.
      char* payload = reinterpret_cast<char *> (chunk) + chunk_offset;

      // Align it to block_align.
      void* aligned_payload = payload;
      std::size_t aligned_size = chunk->size - chunk_offset;

      void* res;
      res = std::align (block_align, bytes, aligned_payload, aligned_size);
      if (res != nullptr)
        {
          assert(res != nullptr);
        }

      // Compute the possible alignment offset.
      std::ptrdiff_t offset = static_cast<char *> (aligned_payload) - payload;
      if (offset)
        {
          // If non-zero, store it in the gap left by alignment in the
          // chunk header.

          chunk_t* adj_chunk =
              reinterpret_cast<chunk_t *> (static_cast<char *> (aligned_payload)
                  - chunk_offset);
          adj_chunk->size = static_cast<std::size_t> (-offset);
        }

#if defined(OS_TRACE_LIBCPP_MEMORY_RESOURCE)
      trace::printf ("%s(%u,%u)=%p,%u @%p %s\n", __func__, bytes, alignment,
                     aligned_payload, alloc_size, this, name ());
#endif

      return aligned_payload;
    }

#pragma GCC diagnostic pop

  // --------------------------------------------------------------------------
  } /* namespace memory */
} /* namespace os */

// ----------------------------------------------------------------------------
