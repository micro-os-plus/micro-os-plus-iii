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

#include <cassert>
#include <new>
#include <memory>

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/rtos/port/os-inlines.h>

// Better be the last, to undef putchar()
#include <cmsis-plus/diag/trace.h>

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
     *  Current functionality inspired by ARM CMSIS, with extensions.
     */

    /**
     * This constructor shall initialise the memory pool object
     * with the given number of blocks and default settings.
     * The effect shall be equivalent to creating a memory pool object
     * referring to the attributes in `mempool::initializer`.
     * Upon successful initialisation, the state of the memory
     * pool shall become initialised, with all blocks available.
     *
     * Only the memory pool itself may be used for allocations.
     * It is not allowed to make copies of
     * condition variable objects.
     *
     * For default memory pool objects, the storage is dynamically allocated.
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
     * This constructor shall initialise the memory pool object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the memory pool attributes shall not be affected.
     * Upon successful initialisation, the state of the memory pool
     * variable shall become initialised.
     *
     * Only the memory pool itself may be used for allocations.
     * It is not allowed to make copies of
     * condition variable objects.
     *
     * In cases where default memory pool attributes are
     * appropriate, the variable `mempool::initializer` can be used to
     * initialise condition variables.
     * The effect shall be equivalent to creating a memory pool object with
     * the simple constructor.
     *
     * If the attributes define a storage area, it is used, otherwise
     * it is dynamically allocated.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Memory_pool::Memory_pool (const mempool::Attributes& attr,
                              mempool::size_t blocks,
                              mempool::size_t block_size_bytes) :
        Named_object
          { attr.name () }, //
#if !defined(OS_INCLUDE_RTOS_PORT_MEMORY_POOL)
            clock_ (attr.clock != nullptr ? *attr.clock : systick_clock),
#endif
            blocks_ (blocks), //
            // Adjust block size to multiple of pointer.
            // Blocks must be large enough to store a pointer, used
            // to construct the list of free blocks.
            block_size_bytes_ (
                (static_cast<mempool::size_t> (block_size_bytes
                    + __SIZEOF_POINTER__ - 1))
                    & (static_cast<mempool::size_t> (~(__SIZEOF_POINTER__ - 1))))
    {
      os_assert_throw(!scheduler::in_handler_mode (), EPERM);
      assert(blocks_ > 0);

      // Blocks must be pointer aligned.
#if 0
      pool_addr_ = (char*) attr.mp_pool_address;
      pool_addr_ += (__SIZEOF_POINTER__ - 1);
      pool_addr_ &= ~(__SIZEOF_POINTER__ - 1);
#else
      void* p = attr.mp_pool_address;
      std::size_t sz = attr.mp_pool_size_bytes;
      pool_addr_ = static_cast<char*> (std::align (__SIZEOF_POINTER__,
                                                   blocks_ * block_size_bytes_,
                                                   p, sz));
#endif

#if defined(OS_TRACE_RTOS_MEMPOOL)
      trace::printf ("%s() @%p %s %d %d\n", __func__, this, name (), blocks_,
                     block_size_bytes_);
#endif

      if (pool_addr_ == nullptr)
        {
          // TODO: use custom allocator
          allocated_pool_addr_ = new (std::nothrow) char[blocks_
              * block_size_bytes_];
          pool_addr_ = allocated_pool_addr_;
          // TODO: should we care with alignment?
        }
      else
        {
          allocated_pool_addr_ = nullptr;

          os_assert_throw(
              attr.mp_pool_size_bytes
                  >= (static_cast<std::size_t> (blocks_ * block_size_bytes_)),
              ENOMEM);
        }

      os_assert_throw(pool_addr_ != nullptr, ENOMEM);

      _init ();
    }

    /**
     * @details
     * This destructor shall destroy the memory pool object; the object
     * becomes, in effect, uninitialised. An implementation may cause
     * the destructor to set the object to an invalid value.
     *
     * It shall be safe to destroy an initialised memory pool object
     * upon which no threads are currently blocked. Attempting to
     * destroy a memory pool object upon which other threads are
     * currently blocked results in undefined behaviour.
     *
     * If the storage for the memory pool was dynamically allocated,
     * it is deallocated.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Memory_pool::~Memory_pool ()
    {
#if defined(OS_TRACE_RTOS_MEMPOOL)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

      assert(list_.empty ());

      delete[] allocated_pool_addr_;
    }

    /*
     * Construct the linked list of blocks and initialise the
     * internal pointers and counters.
     */
    void
    Memory_pool::_init (void)
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
          void* p = static_cast<void*> (first_);
          first_ = *(static_cast<void**> (first_));
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

#if defined(OS_TRACE_RTOS_MEMPOOL)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

      void* p;

      // Extra test before entering the loop, with its inherent weight.
      // Trade size for speed.
        {
          interrupts::Critical_section ics; // ----- Critical section -----

          p = _try_first ();
          if (p != nullptr)
            {
              return p;
            }
        }

      Thread& crt_thread = this_thread::thread ();

      // Prepare a list node pointing to the current thread.
      // Do not worry for being on stack, it is temporarily linked to the
      // list and guaranteed to be removed before this function returns.
      Waiting_thread_node node
        { crt_thread };

      for (;;)
        {
            {
              interrupts::Critical_section ics; // ----- Critical section -----

              p = _try_first ();
              if (p != nullptr)
                {
                  return p;
                }

              // Add this thread to the memory pool waiting list.
              scheduler::_link_node (list_, node);
            }

          port::scheduler::reschedule ();

          // Remove the thread from the memory pool waiting list,
          // if not already removed by free().
          scheduler::_unlink_node (node);

          if (this_thread::thread ().interrupted ())
            {
              return nullptr;
            }
        }

      /* NOTREACHED */
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
#if defined(OS_TRACE_RTOS_MEMPOOL)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

      interrupts::Critical_section ics; // ----- Critical section -----

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
    Memory_pool::timed_alloc (clock::duration_t timeout)
    {
      os_assert_throw(!scheduler::in_handler_mode (), EPERM);

#if defined(OS_TRACE_RTOS_MEMPOOL)
      trace::printf ("%s(%d) @%p %s\n", __func__, timeout, this, name ());
#endif

      void* p;

      // Extra test before entering the loop, with its inherent weight.
      // Trade size for speed.
        {
          interrupts::Critical_section ics; // ----- Critical section -----

          p = _try_first ();
          if (p != nullptr)
            {
              return p;
            }
        }

      Thread& crt_thread = this_thread::thread ();

      // Prepare a list node pointing to the current thread.
      // Do not worry for being on stack, it is temporarily linked to the
      // list and guaranteed to be removed before this function returns.
      Waiting_thread_node node
        { crt_thread };

      Clock_timestamps_list& clock_list = clock_.steady_list ();
      clock::timestamp_t timeout_timestamp = clock_.steady_now () + timeout;

      // Prepare a timeout node pointing to the current thread.
      Timeout_thread_node timeout_node
        { timeout_timestamp, crt_thread };

      for (;;)
        {
            {
              interrupts::Critical_section ics; // ----- Critical section -----

              p = _try_first ();
              if (p != nullptr)
                {
                  return p;
                }

              // Add this thread to the memory pool waiting list,
              // and the clock timeout list.
              scheduler::_link_node (list_, node, clock_list, timeout_node);
            }

          port::scheduler::reschedule ();

          // Remove the thread from the memory pool waiting list,
          // if not already removed by free() and from the clock
          // timeout list, if not already removed by the timer.
          scheduler::_unlink_node (node, timeout_node);

          if (this_thread::thread ().interrupted ())
            {
              return nullptr;
            }

          if (clock_.steady_now () >= timeout_timestamp)
            {
              return nullptr;
            }
        }

      /* NOTREACHED */
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
#if defined(OS_TRACE_RTOS_MEMPOOL)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

      // Validate pointer.
      if ((block < pool_addr_)
          || (block
              >= (static_cast<char*> (pool_addr_) + blocks_ * block_size_bytes_)))
        {
          return EINVAL;
        }

        {
          interrupts::Critical_section ics; // ----- Critical section -----

          // Perform a push_front() on the single linked LIFO list,
          // i.e. add the block to the beginning of the list.

          // Link previous list to this block; may be null, but it does
          // not matter.
          *(static_cast<void**> (block)) = first_;

          // Now this block is the first one.
          first_ = block;

          --count_;
        }

      // Wake-up one thread, if any.
      list_.resume_one ();

      if (!scheduler::in_handler_mode ())
        {
          port::this_thread::yield ();
        }

      return result::ok;
    }

    /**
     * @details
     * Reset the memory pool to the initial state, with all blocks free.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Memory_pool::reset (void)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

#if defined(OS_TRACE_RTOS_MEMPOOL)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

        {
          interrupts::Critical_section ics; // ----- Critical section -----

          _init ();
        }

        {
          interrupts::Critical_section ics; // ----- Critical section -----

          // Wake-up all threads, if any.
          list_.resume_all ();

          list_.clear ();
        }

      return result::ok;
    }

  // --------------------------------------------------------------------------

  } /* namespace rtos */
} /* namespace os */
