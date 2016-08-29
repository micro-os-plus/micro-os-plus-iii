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

#include <cmsis-plus/memory/newlib-nano-malloc.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace memory
  {

    // ========================================================================

    /**
     * @class newlib_nano_malloc
     * @details
     * This memory manager was inspired by the **newlib nano**
     * implementation of `malloc()` & `free()`.
     *
     * `allocate()` tries to be fast and grasps the first block
     * large enough, possibly splitting large blocks and increasing
     * fragmentation. If the block is only slightly larger
     * (the remaining space is not large enough for a minimum chunk)
     * the block is not split, but left partly unused.
     *
     * When large blocks are split, the top sub-block is returned;
     * in other words, memory is allocated bottom-down. This speeds
     * up deallocation for blocks allocated recently.
     *
     * The free list is kept ordered by addresses, which means
     * `deallocate()` will need to traverse part of it, the older
     * the chunk, the longer the traversal.
     *
     * Neither allocation nor deallocation are deterministic, but are
     * reasonably fast.
     */

    newlib_nano_malloc::newlib_nano_malloc (void* addr, std::size_t size)
    {
      assert(size > block_minchunk);

#if defined(OS_TRACE_LIBCPP_MEMORY_RESOURCE)
      trace::printf ("%s(%p,%u) @%p \n", __func__, addr, size, this);
#endif

      addr_ = addr;
      size_ = size;

      void* align_addr = addr;
      std::size_t align_sz = size;

      // Align address for first chunk.
      void* res;
      res = std::align (chunk_align, block_minchunk, align_addr, align_sz);
      if (res != nullptr)
        {
          assert(res != nullptr);
        }

      // Fill it the first chunk.
      chunk_t* chunk = reinterpret_cast<chunk_t*> (align_addr);
      chunk->size = align_sz;

      // Remember first chunk as list head.
      free_list_ = chunk;
    }

    newlib_nano_malloc::~newlib_nano_malloc ()
    {
#if defined(OS_TRACE_LIBCPP_MEMORY_RESOURCE)
      trace::printf ("%s() @%p \n", __func__, this);
#endif
    }

    void*
    newlib_nano_malloc::do_allocate (std::size_t bytes, std::size_t alignment)
    {
#if defined(OS_TRACE_LIBCPP_MEMORY_RESOURCE)
      trace::printf ("%s(%u,%u)\n", __func__, bytes, alignment);
#endif

      bool do_not_throw = false;
      if ((alignment & no_throw) != 0)
        {
          do_not_throw = true;
          alignment &= ~no_throw;
        }

      // TODO: consider `alignment` if > block_align.

      std::size_t alloc_size = do_align (bytes, chunk_align);
      alloc_size += block_padding;
      alloc_size += chunk_offset;

      alloc_size = os::rtos::memory::max (alloc_size, block_minchunk);

      chunk_t* prev_chunk = free_list_;
      chunk_t* chunk = prev_chunk;

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
                  chunk =
                      reinterpret_cast<chunk_t *> (reinterpret_cast<char *> (chunk)
                          + rem);
                  chunk->size = alloc_size;
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

      if (chunk == nullptr)
        {
          if (do_not_throw)
            {
              return nullptr;
            }
          else
            {
              estd::__throw_bad_alloc (ENOMEM, "No more free space.");
            }
        }

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
      trace::printf ("%s(%u,%u)=%p,%u\n", __func__, bytes, alignment,
          aligned_payload, alloc_size);
#endif

      return aligned_payload;
    }

    void
    newlib_nano_malloc::do_deallocate (void* addr, std::size_t bytes,
                                       std::size_t alignment)
    {
#if defined(OS_TRACE_LIBCPP_MEMORY_RESOURCE)
      trace::printf ("%s(%p,%u,%u)\n", __func__, addr, bytes,
          alignment);
#endif

      bool do_not_throw = false;
      if ((alignment & no_throw) != 0)
        {
          do_not_throw = true;
          alignment &= ~no_throw;
        }

      // The address must be inside the arena; no exceptions.
      if ((addr < addr_) || (addr > (static_cast<char*> (addr_) + size_)))
        {
          if (do_not_throw)
            {
              return;
            }
          else
            {
              estd::__throw_bad_alloc (EINVAL, "Address out of arena.");
            }
        }

      // Compute the chunk address from the user address.
      chunk_t* chunk = reinterpret_cast<chunk_t *> (static_cast<char *> (addr)
          - chunk_offset);

      // If the block was aligned, the offset appears as size; adjust back.
      if (static_cast<std::ptrdiff_t> (chunk->size) < 0)
        {
          chunk = reinterpret_cast<chunk_t *> (reinterpret_cast<char *> (chunk)
              + static_cast<std::ptrdiff_t> (chunk->size));
        }

      if (bytes)
        {
          // If size is known, validate.
          // (when called from free(), the size is not known).
          if (bytes + chunk_offset > chunk->size)
            {
              if (do_not_throw)
                {
                  return;
                }
              else
                {
                  estd::__throw_bad_alloc (EINVAL, "Size larger than chunk.");
                }
            }
        }

      // If the free list is empty, create it with the current chunk, alone.
      if (free_list_ == nullptr)
        {
          // Mark the end of the list with a null pointer.
          chunk->next = nullptr;

          // The chunk becomes the first list element.
          free_list_ = chunk;
          return;
        }

      // The free list exists; is the chunk before the list head?
      if (chunk < free_list_)
        {
          // Is the chunk *right* before the list head?
          if (reinterpret_cast<char *> (chunk) + chunk->size
              == reinterpret_cast<char *> (free_list_))
            {
              // Coalesce chunk to the list head.
              chunk->size += free_list_->size;
              chunk->next = free_list_->next;
            }
          else
            {
              // Insert before the list head.
              chunk->next = free_list_;
            }
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

      if (reinterpret_cast<char *> (prev_chunk) + prev_chunk->size
          == reinterpret_cast<char *> (chunk))
        {
          // Chunk to be freed is adjacent to a free chunk before it.
          prev_chunk->size += chunk->size;
          // If the merged chunk is also adjacent to the chunk after it,
          // merge again.
          if (reinterpret_cast<char *> (prev_chunk) + prev_chunk->size
              == reinterpret_cast<char *> (next_chunk))
            {
              prev_chunk->size += next_chunk->size;
              prev_chunk->next = next_chunk->next;
            }
        }
      else if (reinterpret_cast<char *> (prev_chunk) + prev_chunk->size
          > reinterpret_cast<char *> (chunk))
        {
          // Already freed.
          return;
        }
      else if (reinterpret_cast<char *> (chunk) + chunk->size
          == reinterpret_cast<char *> (next_chunk))
        {
          // The chunk to be freed is adjacent to a free chunk after it.
          chunk->size += next_chunk->size;
          chunk->next = next_chunk->next;
          prev_chunk->next = chunk;
        }
      else
        {
          // Not adjacent to any chunk. Just insert it.
          // The result is a new fragment. Not great...
          chunk->next = next_chunk;
          prev_chunk->next = chunk;
        }
    }

  // --------------------------------------------------------------------------

  } /* namespace memory */
} /* namespace os */

// ----------------------------------------------------------------------------
