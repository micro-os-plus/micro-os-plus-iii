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

#include <cmsis-plus/memory/lifo.h>
#include <memory>

// ----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace memory
  {

    // ========================================================================

    lifo::~lifo ()
    {
      trace::printf ("lifo::%s() @%p %s\n", __func__, this, this->name ());
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
      std::size_t block_padding = calc_block_padding (alignment);
      std::size_t alloc_size = rtos::memory::align_size (bytes, chunk_align);
      alloc_size += block_padding;
      alloc_size += chunk_offset;

      std::size_t block_minchunk = calc_block_minchunk (block_padding);
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
                      // Compute where top chunk starts.
                      chunk =
                          reinterpret_cast<chunk_t *> (reinterpret_cast<char *> (chunk)
                              + rem);
#pragma GCC diagnostic pop
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
              trace::printf ("lifo::%s(%u,%u)=0 @%p %s\n", __func__, bytes,
                             alignment, this, this->name ());
#endif

              return nullptr;
            }

#if defined(OS_TRACE_LIBCPP_MEMORY_RESOURCE)
          trace::printf ("lifo::%s(%u,%u) @%p %s out of memory\n", __func__,
                         bytes, alignment, this, this->name ());
#endif
          out_of_memory_handler_ ();

          // If the handler returned, assume it freed some memory
          // and try again to allocate.
        }

      void* aligned_payload = internal_align_ (chunk, bytes, alignment);

#if defined(OS_TRACE_LIBCPP_MEMORY_RESOURCE)
      trace::printf ("lifo::%s(%u,%u)=%p,%u @%p %s\n", __func__, bytes,
                     alignment, aligned_payload, alloc_size, this, name ());
#endif

      return aligned_payload;
    }

#pragma GCC diagnostic pop

  // --------------------------------------------------------------------------
  } /* namespace memory */
} /* namespace os */

// ----------------------------------------------------------------------------
