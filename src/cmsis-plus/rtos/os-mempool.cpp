/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 *
 * µOS++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 3.
 *
 * µOS++ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/diag/trace.h>

#include <cmsis-plus/rtos/port/os-decls.h>
#include <cmsis-plus/rtos/port/os-inlines.h>

// ----------------------------------------------------------------------------

#include <cassert>
#include <cerrno>
#include <cstdlib>

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {

    // ------------------------------------------------------------------------

    namespace mempool
    {
      const Attributes initializer
        { nullptr };

    } /* namespace mempool */

    // ------------------------------------------------------------------------

    /**
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Memory_pool::Memory_pool (mempool::size_t blocks,
                              mempool::size_t block_size_bytes) :
        Memory_pool
          { mempool::initializer, blocks, block_size_bytes }
    {
      ;
    }

    /**
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Memory_pool::Memory_pool (const mempool::Attributes& attr,
                              mempool::size_t blocks,
                              mempool::size_t block_size_bytes) :
        Named_object
          { attr.name () }, //
        blocks_ (blocks), //
        block_size_bytes_ (block_size_bytes)

    {
      os_assert_throw(!scheduler::in_handler_mode (), EPERM);

      pool_addr_ = (char*) attr.mp_pool_address;

      assert(blocks_ > 0);
      // Blocks must be large enough to store the index, used
      // to construct the list of free blocks.
      assert(block_size_bytes_ >= sizeof(std::ptrdiff_t));

      flags_ = 0;

      trace::printf ("%s() @%p %s %d %d\n", __func__, this, name (), blocks_,
                     block_size_bytes_);

      if (pool_addr_ == nullptr)
        {
          pool_addr_ = new (std::nothrow) char[blocks_ * block_size_bytes_];
          flags_ |= flags_allocated;
        }

      os_assert_throw(pool_addr_ != nullptr, ENOMEM);

      // Construct a linked list of blocks. Store the pointer at
      // the beginning of each block. Each block
      // will hold the address of the next free block, or nullptr at the end.
      char* p = pool_addr_;
      for (std::size_t i = 1; i < blocks_; ++i)
        {
          *(void**) p = (p + block_size_bytes_);
          p += block_size_bytes_;
        }
      *(void**) p = nullptr;

      first_ = pool_addr_; // Pointer to first block.
      last_ = p; // Pointer to last block.

      count_ = 0; // No allocated blocks.
    }

    /**
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Memory_pool::~Memory_pool ()
    {
      trace::printf ("%s() @%p %s\n", __func__, this, name ());

      if (flags_ | flags_allocated)
        {
          delete[] (pool_addr_);
        }
    }

    /*
     * Internal function used to return the first block in the
     * free list.
     */
    void*
    Memory_pool::try_first (void)
    {
      if (first_ != nullptr)
        {
          void* p = (void*) first_;
          first_ = *(void**) first_;
          ++count_;
          return p;
        }
      return nullptr;
    }

    /**
     * @details
     * Allocate a fixed size memory block from the memory pool.
     *
     * It uses a critical section to protect simultaneous access from
     * other threads or interrupts.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    void*
    Memory_pool::alloc (void)
    {
      os_assert_throw(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s() @%p %s\n", __func__, this, name ());

      bool queued = false;
      for (;;)
        {
            {
              Critical_section_irq cs; // ----- Critical section -----

              void* p = try_first ();
              if (p != nullptr)
                {
                  return p;
                }

              if (!queued)
                {
                  // Add this thread to the waiting list.
                  // Will be removed by free().
                  list_.add (&this_thread::thread ());
                  queued = true;
                }
            }
          this_thread::suspend ();

          if (this_thread::thread ().interrupted ())
            {
              return nullptr;
            }
        }

      /* NOTREACHED */
      return nullptr;
    }

    /**
     * @details
     * Allocate a fixed size memory block from the memory pool.
     *
     * It uses a critical section to protect simultaneous access from
     * other threads or interrupts.
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    void*
    Memory_pool::try_alloc (void)
    {
      trace::printf ("%s() @%p %s\n", __func__, this, name ());

      return try_first ();
    }

    /**
     * @details
     * Allocate a fixed size memory block from the memory pool.
     *
     * It uses a critical section to protect simultaneous access from
     * other threads or interrupts.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    void*
    Memory_pool::timed_alloc (systicks_t ticks)
    {
      os_assert_throw(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s(%d) @%p %s\n", __func__, ticks, this, name ());

      bool queued = false;

      if (ticks == 0)
        {
          ticks = 1;
        }

      Systick_clock::rep start = Systick_clock::now ();
      for (;;)
        {
          Systick_clock::sleep_rep slept_ticks;
            {
              Critical_section_irq cs; // ----- Critical section -----

              void* p = try_first ();
              if (p != nullptr)
                {
                  return p;
                }

              Systick_clock::rep now = Systick_clock::now ();
              slept_ticks = (Systick_clock::sleep_rep) (now - start);
              if (slept_ticks >= ticks)
                {
                  if (queued)
                    {
                      list_.remove (&this_thread::thread ());
                    }
                  return nullptr;
                }

              if (!queued)
                {
                  // Add this thread to the waiting list.
                  // Will be removed by free() or if timeout occurs.
                  list_.add (&this_thread::thread ());
                  queued = true;
                }
            }

          Systick_clock::sleep_for (ticks - slept_ticks);

          if (this_thread::thread ().interrupted ())
            {
              return nullptr;
            }
        }

      /* NOTREACHED */
      return nullptr;
    }

    /**
     * @details
     * Return a memory block previously allocated by `alloc()` or
     * `calloc()` back to the memory pool.
     *
     * It uses a critical section to protect simultaneous access from
     * other threads or interrupts.
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    result_t
    Memory_pool::free (void* block)
    {
      trace::printf ("%s() @%p %s\n", __func__, this, name ());

      // Validate pointer.
      if ((block < pool_addr_)
          || (block >= (pool_addr_ + blocks_ * block_size_bytes_)))
        {
          return EINVAL;
        }

      Critical_section_irq cs; // ----- Critical section -----

      // Perform a push_back() on the single linked FIFO list,
      // i.e. add the block to the end of the list.
      if (last_ != nullptr)
        {
          // Link the block after the current last.
          *(void**) last_ = block;
        }

      // Mark block as end of list.
      *(void**) block = nullptr;

      // Now this block is the last one.
      last_ = block;

      // If the list was empty (all blocks were allocated, so no first),
      // this block will be both the first and the last.
      if (first_ == nullptr)
        {
          first_ = block;
        }

      --count_;

      if (!list_.empty ())
        {
          // Wake-up one thread, if any.
          list_.top_prio_task ()->wakeup ();
        }

      return result::ok;
    }

    /**
     * @details
     * Reset the memory pool to initial state, all block are free.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Memory_pool::reset (void)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s() @%p %s\n", __func__, this, name ());

      // TODO
      return result::ok;
    }

  // --------------------------------------------------------------------------

  } /* namespace rtos */
} /* namespace os */
