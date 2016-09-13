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

#include <cmsis-plus/memory/block-pool.h>
#include <cmsis-plus/rtos/os.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace memory
  {

    // ========================================================================

    /**
     * @details
     */
    block_pool::~block_pool ()
    {
      trace::printf ("%s() @%p %s\n", __func__, this, this->name ());
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    /**
     * @details
     */
    void*
    block_pool::do_allocate (std::size_t bytes, std::size_t alignment)
    {
      assert(bytes <= block_size_bytes_);

      if (first_ == nullptr)
        {
          return nullptr;
        }

      void* p = static_cast<void*> (first_);
      first_ = *(static_cast<void**> (first_));
      ++count_;

      // Update statistics.
      // What is subtracted from free is added to allocated.
      internal_increase_allocated_statistics (block_size_bytes_);

#if defined(OS_TRACE_LIBCPP_MEMORY_RESOURCE)
      trace::printf ("%s(%u,%u)=%p,%u @%p %s\n", __func__, bytes, alignment, p,
                     block_size_bytes_, this, name ());
#endif

      return p;
    }

    /**
     * @details
     */
    void
    block_pool::do_deallocate (void* addr, std::size_t bytes,
                               std::size_t alignment) noexcept
    {
#if defined(OS_TRACE_LIBCPP_MEMORY_RESOURCE)
      trace::printf ("%s(%p,%u,%u) @%p %s\n", __func__, addr, bytes, alignment,
                     this, name ());
#endif

      if ((addr < pool_addr_)
          || (addr
              >= (static_cast<char*> (pool_addr_) + blocks_ * block_size_bytes_)))
        {
          assert(false);
          return;
        }

      // Perform a push_front() on the single linked LIFO list,
      // i.e. add the block to the beginning of the list.

      // Link previous list to this block; may be null, but it does
      // not matter.
      *(static_cast<void**> (addr)) = first_;

      // Now this block is the first in the free list..
      first_ = addr;

      --count_;

      // Update statistics.
      // What is subtracted from allocated is added to free.
      internal_decrease_allocated_statistics (block_size_bytes_);
    }

#pragma GCC diagnostic push

    /**
     * @details
     */
    std::size_t
    block_pool::do_max_size (void) const noexcept
    {
      return block_size_bytes_ * blocks_;
    }

    /**
     * @details
     */
    void
    block_pool::do_reset (void) noexcept
    {
#if defined(OS_TRACE_LIBCPP_MEMORY_RESOURCE)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif
      internal_reset_ ();
    }

    /**
     * @details
     */
    void
    block_pool::internal_construct_ (std::size_t blocks,
                                     std::size_t block_size_bytes, void* addr,
                                     std::size_t bytes) noexcept
    {
      blocks_ = blocks;

      block_size_bytes_ = rtos::memory::align_size (block_size_bytes,
                                                    alignof(void*));
      assert(block_size_bytes_ >= sizeof(void*));

      assert(addr != nullptr);
      pool_addr_ = addr;

      std::size_t align_sz = bytes;

      void* res;
      // Possibly adjust the last two parameters.
      res = std::align (alignof(void*), blocks * block_size_bytes_, pool_addr_,
                        align_sz);

      // std::align() will fail if it cannot fit the adjusted block size.
      if (res != nullptr)
        {
          assert(res != nullptr);
        }

      // The extra assert is made redundant by std::align().
      // assert(blocks * block_size_bytes_ <= align_sz);

      total_bytes_ = blocks_ * block_size_bytes_;

      internal_reset_ ();
    }

    /**
     * @details
     */
    void
    block_pool::internal_reset_ (void) noexcept
    {
      // Construct a linked list of blocks. Store the pointer at
      // the beginning of each block. Each block
      // will hold the address of the next free block, or nullptr at the end.
      char* p = static_cast<char*> (pool_addr_);
      for (std::size_t i = 1; i < blocks_; ++i)
        {
          // Compute the address of the next block;
          char* pn = p + block_size_bytes_;

          // Make this block point to the next one.
          *(static_cast<void**> (static_cast<void*> (p))) = pn;
          // Advance pointer
          p = pn;
        }

      // Mark end of list.
      *(static_cast<void**> (static_cast<void*> (p))) = nullptr;

      first_ = pool_addr_; // Pointer to first block.

      count_ = 0; // No allocated blocks.

      allocated_bytes_ = 0;
      max_allocated_bytes_ = 0;
      free_bytes_ = total_bytes_;
      allocated_chunks_ = 0;
      free_chunks_ = blocks_;

    }

  // --------------------------------------------------------------------------
  } /* namespace memory */
} /* namespace os */

// ----------------------------------------------------------------------------
