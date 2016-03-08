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
#include <new>

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {

    // ------------------------------------------------------------------------

    /**
     * @details
     * The os::rtos::mempool namespace groups memory pool attributes
     * and initialisers.
     */
    namespace mempool
    {
      /**
       * @class Attributes
       * @details
       * Allow to assign a name and custom attributes (like a static
       * address) to the memory pool.
       *
       * To simplify access, the member variables are public and do not
       * require accessors or mutators.
       *
       * @par Example
       *
       * Define an array of structures and
       * pass its address and size via the attributes.
       *
       * @code{.cpp}
       * // Define the type of one pool block.
       * typedef struct {
       *   uint32_t length;
       *   uint32_t width;
       *   uint32_t height;
       *   uint32_t weight;
       * } properties_t;
       *
       * // Define the pool size.
       * constexpr uint32_t pool_size = 10;
       *
       * // Allocate static storage for the pool.
       * properties_t pool[pool_size];
       *
       * void
       * func(void)
       * {
       *    // Do something
       *
       *    // Define pool attributes.
       *    mempool::Attributes attr { "properties" };
       *    attr.mp_pool_address = pool;
       *    attr.mp_pool_size_bytes = sizeof(pool);
       *
       *    // Create the pool object.
       *    Memory_pool mp { attr, pool_size, sizeof(properties_t) };
       *
       *    // Do something else.
       * }
       * @endcode
       *
       * @par POSIX compatibility
       *  No POSIX similar functionality identified, but inspired by POSIX
       *  attributes used in [<pthread.h>](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
       *  (IEEE Std 1003.1, 2013 Edition).
       */

      /**
       * @var void* Attributes::mp_pool_address
       * @details
       * Set this variable to a user defined memory area large enough
       * to store the memory pool. Usually this is a statically
       * allocated array of structures.
       *
       * The default value is `nullptr`, which means there is no
       * user defined memory pool.
       */

      /**
       * @var size_t Attributes::mp_pool_size_bytes
       * @details
       * The memory pool size must match exactly the allocated size. It is
       * used for validation; when the memory pool is initialised,
       * this size must be large enough to accommodate the desired
       * memory pool.
       *
       * If the @ref mp_pool_address is `nullptr`, this variable is not
       * checked, but it is recommended to leave it zero.
       */

      const Attributes initializer
        { nullptr };

    } /* namespace mempool */

    // ------------------------------------------------------------------------

    /**
     * @class Memory_pool
     * @details
     * Manage a pool of same size blocks. Fast and deterministic allocation
     * and dealocation behaviour, suitable for use even in ISRs.
     *
     * @par Example
     *
     * @code{.cpp}
     * // Define the type of one pool block.
     * typedef struct {
     *   uint32_t length;
     *   uint32_t width;
     *   uint32_t height;
     *   uint32_t weight;
     * } properties_t;
     *
     * // Define the pool size.
     * constexpr uint32_t pool_size = 10;
     *
     * // Create the pool object.
     * Memory_pool mp { pool_size, sizeof(properties_t) };
     *
     * void
     * func(void)
     * {
     *   // Do something
     *
     *   void* buf;
     *
     *   // Get one block from pool.
     *   buf = mp.alloc();
     *
     *   // ... use the buffer
     *
     *   // Free the buffer.
     *   mp.free(buf);
     *
     *   // Do something else.
     * }
     * @endcode
     *
     * @note There is no equivalent of calloc(); to initialise to
     * zero a memory block, use:
     * @code{.cpp}
     * block = mp.alloc();
     * memset (block, 0, mp.block_size ());
     * @endcode
     *
     * @par POSIX compatibility
     *  No POSIX similar functionality identified.
     */

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
      else
        {
          os_assert_throw(
              attr.mp_pool_size_bytes
                  >= ((std::size_t )(blocks_ * block_size_bytes_)),
              ENOMEM);
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

      count_ = 0; // No allocated blocks.
    }

    /**
     * @details
     * If the pool was dynamically allocated, it is automatically freed.
     *
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
    Memory_pool::_try_first (void)
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
     * Allocate a fixed size memory block from the memory pool. If there
     * are no free blocks, the current task is added to the
     * waiting list and will wait until one block is freed.
     *
     * The function uses a critical section to protect simultaneous
     * access from
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
              interrupts::Critical_section cs; // ----- Critical section -----

              void* p = _try_first ();
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
     * Try to allocate a fixed size memory block from
     * the memory pool, if available, return it, otherwise return
     * `nullptr`.
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

      return _try_first ();
    }

    /**
     * @details
     * Allocate a fixed size memory block from the memory pool. If there
     * are no free blocks, the current task is added to the
     * waiting list and will wait until one block is freed or the
     * timeout expired.
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
              interrupts::Critical_section cs; // ----- Critical section -----

              void* p = _try_first ();
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

          Systick_clock::wait (ticks - slept_ticks);

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
     * Return a memory block previously allocated by `alloc()`
     * back to the memory pool.
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

      interrupts::Critical_section cs; // ----- Critical section -----

      // Perform a push_front() on the single linked LIFO list,
      // i.e. add the block to the beginning of the list.

      // Link previous list to this block; may be null, but it does
      // not matter.
      *(void**) block = first_;

      // Now this block is the first one.
      first_ = block;

      --count_;

      if (!list_.empty ())
        {
          // Wake-up one thread, if any.
          list_.wakeup_one ();
        }

      return result::ok;
    }

    /**
     * @details
     * Reset the memory pool to the initial state, with all block free.
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
