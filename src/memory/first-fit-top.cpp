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

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/memory/first-fit-top.h>
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

    first_fit_top::~first_fit_top ()
    {
      trace::printf ("first_fit_top::%s() @%p %s\n", __func__, this, name ());
    }

    void
    first_fit_top::internal_construct_ (void* addr, std::size_t bytes)
    {
      assert(bytes > chunk_minsize);

      arena_addr_ = addr;
      total_bytes_ = bytes;

      // Align address for first chunk.
      void* res;
      // Possibly adjust the last two parameters.
      res = std::align (chunk_align, chunk_minsize, arena_addr_, total_bytes_);
      // std::align() will fail if it cannot fit the min chunk.
      if (res == nullptr)
        {
          assert(res != nullptr);
        }
      assert((total_bytes_ % chunk_align) == 0);

      internal_reset_ ();
    }

    void
    first_fit_top::internal_reset_ (void) noexcept
    {
      // Fill it with the first chunk.
      chunk_t* chunk = reinterpret_cast<chunk_t*> (arena_addr_);
      // Entire arena is a big free chunk.
      chunk->size = total_bytes_;
      // Mark the end of the list with a null pointer.
      chunk->next = nullptr;

      allocated_bytes_ = 0;
      max_allocated_bytes_ = 0;
      free_bytes_ = total_bytes_;
      allocated_chunks_ = 0;
      free_chunks_ = 1;

      // Remember first chunk as list head.
      free_list_ = chunk;
    }

    void
    first_fit_top::do_reset (void) noexcept
    {
#if defined(OS_TRACE_LIBCPP_MEMORY_RESOURCE)
      trace::printf ("first_fit_top::%s() @%p %s\n", __func__, this, name ());
#endif

      internal_reset_ ();
    }

#pragma GCC diagnostic push
// Needed because 'alignment' is used only in trace calls.
#pragma GCC diagnostic ignored "-Wunused-parameter"

    /**
     * @details
     * The allocator tries to be fast and grasps the first block
     * large enough, possibly splitting large blocks and increasing
     * fragmentation. If the block is only slightly larger
     * (the remaining space is not large enough for a minimum chunk)
     * the block is not split, but left partly unused.
     *
     * When large blocks are split, the top sub-block is returned;
     * in other words, memory is allocated top-down. This speeds
     * up deallocation for blocks allocated recently.
     *
     * @par Exceptions
     *   Throws nothing by itself, but the out of memory handler may
     *   throw `bad_alloc()`.
     */
    void*
    first_fit_top::do_allocate (std::size_t bytes, std::size_t alignment)
    {
      std::size_t block_padding = calc_block_padding (alignment);
      std::size_t alloc_size = rtos::memory::align_size (bytes, chunk_align);
      alloc_size += block_padding;
      alloc_size += chunk_offset;

      std::size_t block_minchunk = calc_block_minchunk (block_padding);
      alloc_size = os::rtos::memory::max (alloc_size, block_minchunk);

      chunk_t* chunk;

      while (true)
        {
          chunk_t* prev_chunk = free_list_;
          chunk = prev_chunk;

          while (chunk)
            {
              int rem = static_cast<int> (chunk->size - alloc_size);
              if (rem >= 0)
                {
                  if ((static_cast<std::size_t> (rem)) >= block_minchunk)
                    {
                      // Found a chunk that is much larger than required size
                      // (at least one more chunk is available);
                      // break it into two chunks and return the second one.

                      chunk->size = static_cast<std::size_t> (rem);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
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

                      if (prev_chunk == chunk)
                        {
                          // This implies p==r==free_list, i.e. the list head.
                          // The next chunk becomes the first list element.
                          free_list_ = chunk->next;

                          // If this was the last chunk, the free list is empty.
                        }
                      else
                        {
                          // Normal case. Remove it from the free_list.
                          prev_chunk->next = chunk->next;
                        }
                    }
                  break;
                }
              prev_chunk = chunk;
              chunk = chunk->next;
            }

          if (chunk != nullptr)
            {
              break;
            }

          if (out_of_memory_handler_ == nullptr)
            {
#if defined(OS_TRACE_LIBCPP_MEMORY_RESOURCE)
              trace::printf ("first_fit_top::%s(%u,%u)=0 @%p %s\n", __func__,
                             bytes, alignment, this, name ());
#endif

              return nullptr;
            }

#if defined(OS_TRACE_LIBCPP_MEMORY_RESOURCE)
          trace::printf ("first_fit_top::%s(%u,%u) @%p %s out of memory\n",
                         __func__, bytes, alignment, this, name ());
#endif
          out_of_memory_handler_ ();

          // If the handler returned, assume it freed some memory
          // and try again to allocate.
        }

      void* aligned_payload = internal_align_ (chunk, bytes, alignment);

#if defined(OS_TRACE_LIBCPP_MEMORY_RESOURCE)
      trace::printf ("first_fit_top::%s(%u,%u)=%p,%u @%p %s\n", __func__, bytes,
                     alignment, aligned_payload, alloc_size, this, name ());
#endif

      return aligned_payload;
    }

    /**
     * @details
     * Deallocation is not guaranteed to be deterministic, but if
     * done in strict reverse allocation order, it becomes deterministic,
     * otherwise a traversal of the free list is required, the older the
     * block, the more nodes to traverse (the free list is kept in
     * ascending addresses order).
     *
     * If the block is already in the free list, issue a trace message,
     * but otherwise ignore the condition.
     *
     * @par Exceptions
     *   Throws nothing.
     */
    void
    first_fit_top::do_deallocate (void* addr, std::size_t bytes,
                                  std::size_t alignment) noexcept
    {
#if defined(OS_TRACE_LIBCPP_MEMORY_RESOURCE)
      trace::printf ("first_fit_top::%s(%p,%u,%u) @%p %s\n", __func__, addr,
                     bytes, alignment, this, name ());
#endif

      // The address must be inside the arena; no exceptions.
#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
      if ((addr < arena_addr_)
          || (addr > (static_cast<char*> (arena_addr_) + total_bytes_)))
        {
          assert(false);
          return;
        }
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
      // Compute the chunk address from the user address.
      chunk_t* chunk = reinterpret_cast<chunk_t *> (static_cast<char *> (addr)
          - chunk_offset);

      // If the block was aligned, the offset appears as size; adjust back.
      if (static_cast<std::ptrdiff_t> (chunk->size) < 0)
        {
          chunk = reinterpret_cast<chunk_t *> (reinterpret_cast<char *> (chunk)
              + static_cast<std::ptrdiff_t> (chunk->size));
        }
#pragma GCC diagnostic pop

      if (bytes)
        {
          // If size is known, validate.
          // (when called from free(), the size is not known).
          if (bytes + chunk_offset > chunk->size)
            {
              assert(false);
              return;
            }
        }

      // Update statistics.
      // What is subtracted from allocated is added to free.
      internal_decrease_allocated_statistics (chunk->size);

      // If the free list is empty, create it with the current chunk, alone.
      if (free_list_ == nullptr)
        {
          // Mark the end of the list with a null pointer.
          chunk->next = nullptr;

          // The chunk becomes the first list element.
          free_list_ = chunk;
          assert(free_chunks_ == 1);

          return;
        }

      // The free list exists; is the chunk before the list head?
      if (chunk < free_list_)
        {
          // Is the chunk *right* before the list head?
#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
          if (reinterpret_cast<char *> (chunk) + chunk->size
              == reinterpret_cast<char *> (free_list_))
            {
              // Coalesce chunk to the list head.
              chunk->size += free_list_->size;
              chunk->next = free_list_->next; // May be nullptr

              // Coalescing means one less chunk.
              --free_chunks_;
            }
          else
            {
              // Insert before the list head.
              chunk->next = free_list_;
            }
#pragma GCC diagnostic pop
          // The chunk becomes the new list head.
          free_list_ = chunk;

          return;
        }

      // Walk the free list to find the place to insert,
      // (the list must be ordered by addresses).
      // Warning: not deterministic!

      chunk_t* next_chunk = free_list_;
      chunk_t* prev_chunk;
      do
        {
          prev_chunk = next_chunk;
          next_chunk = next_chunk->next;
        }
      while (next_chunk != nullptr && next_chunk <= chunk);

      // Now prev_chunk <= chunk and either next_chunk == nullptr or
      // next_chunk > chunk.
      // Try to merge with chunks immediately before/after it.

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
      if (reinterpret_cast<char *> (prev_chunk) + prev_chunk->size
          == reinterpret_cast<char *> (chunk))
        {
          // Chunk to be freed is adjacent to a free chunk before it.
          prev_chunk->size += chunk->size;

          // Coalescing means one less chunk.
          --free_chunks_;

          // If the merged chunk is also adjacent to the chunk after it,
          // merge again. Does not match if next_chunk == nullptr.
          if (reinterpret_cast<char *> (prev_chunk) + prev_chunk->size
              == reinterpret_cast<char *> (next_chunk))
            {
              prev_chunk->size += next_chunk->size;
              prev_chunk->next = next_chunk->next;

              // Coalescing means one less chunk.
              --free_chunks_;
            }
        }
      else if (reinterpret_cast<char *> (prev_chunk) + prev_chunk->size
          > reinterpret_cast<char *> (chunk))
        {
          // Already freed.

          // Revert statistics.
          // What is subtracted from free is added to allocated.
          allocated_bytes_ += chunk->size;
          free_bytes_ -= chunk->size;
          ++allocated_chunks_;
          --free_chunks_;

          trace::printf ("first_fit_top::%s(%p,%u,%u) @%p %s already freed\n",
                         __func__, addr, bytes, alignment, this, name ());

          return;
        }
      // Does not match if next_chunk == nullptr.
      else if (reinterpret_cast<char *> (chunk) + chunk->size
          == reinterpret_cast<char *> (next_chunk))
        {
          // The chunk to be freed is adjacent to a free chunk after it.
          chunk->size += next_chunk->size;
          chunk->next = next_chunk->next; // May be nullptr.
          prev_chunk->next = chunk;

          // Coalescing means one less chunk.
          --free_chunks_;
        }
      else
        {
          // Not adjacent to any chunk. Just insert it.
          // The result is a new fragment. Not great...
          chunk->next = next_chunk; // May be nullptr.
          prev_chunk->next = chunk;
        }
    }
#pragma GCC diagnostic pop

    std::size_t
    first_fit_top::do_max_size (void) const noexcept
    {
      return total_bytes_;
    }

    void*
    first_fit_top::internal_align_ (chunk_t* chunk, std::size_t bytes,
                             std::size_t alignment)
    {
      // Update statistics.
      // The value subtracted from free is added to allocated.
      internal_increase_allocated_statistics (chunk->size);

      // Compute pointer to payload area.
#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
      char* payload = reinterpret_cast<char *> (chunk) + chunk_offset;
#pragma GCC diagnostic pop

      // Align it to user provided alignment.
      void* aligned_payload = payload;
      std::size_t aligned_size = chunk->size - chunk_offset;

      void* res;
      res = std::align (alignment, bytes, aligned_payload, aligned_size);
      if (res == nullptr)
        {
          assert(res != nullptr);
        }

      // Compute the possible alignment offset.
      std::ptrdiff_t offset = static_cast<char *> (aligned_payload) - payload;
      if (offset)
        {
          // If non-zero, store it in the gap left by alignment in the
          // chunk header.

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
          chunk_t* adj_chunk =
              reinterpret_cast<chunk_t *> (static_cast<char *> (aligned_payload)
                  - chunk_offset);
          adj_chunk->size = static_cast<std::size_t> (-offset);
#pragma GCC diagnostic pop
        }

      assert(
          (reinterpret_cast<uintptr_t> (aligned_payload) & (alignment - 1))
              == 0);

      return aligned_payload;
    }

#pragma GCC diagnostic pop

  // --------------------------------------------------------------------------
  } /* namespace memory */
} /* namespace os */

// ----------------------------------------------------------------------------
