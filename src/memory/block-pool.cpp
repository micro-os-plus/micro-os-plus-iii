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

#include <cmsis-plus/memory/block-pool.h>
#include <cmsis-plus/rtos/os.h>

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

    block_pool::~block_pool ()
    {
      trace::printf ("%s() @%p %s\n", __func__, this, this->name ());
    }

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

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

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wvolatile"
#endif
      ++count_;
#pragma GCC diagnostic pop

      // Update statistics.
      // What is subtracted from free is added to allocated.
      internal_increase_allocated_statistics (block_size_bytes_);

#if defined(OS_TRACE_LIBCPP_MEMORY_RESOURCE)
      trace::printf ("%s(%u,%u)=%p,%u @%p %s\n", __func__, bytes, alignment, p,
                     block_size_bytes_, this, name ());
#endif

      return p;
    }

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

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wvolatile"
#endif
      --count_;
#pragma GCC diagnostic pop

      // Update statistics.
      // What is subtracted from allocated is added to free.
      internal_decrease_allocated_statistics (block_size_bytes_);
    }

#pragma GCC diagnostic push

    std::size_t
    block_pool::do_max_size (void) const noexcept
    {
      return block_size_bytes_ * blocks_;
    }

    void
    block_pool::do_reset (void) noexcept
    {
#if defined(OS_TRACE_LIBCPP_MEMORY_RESOURCE)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif
      internal_reset_ ();
    }

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
