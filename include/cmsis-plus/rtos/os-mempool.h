/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 * Copyright (c) 2013 ARM LIMITED
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

#ifndef CMSIS_PLUS_RTOS_OS_MEMPOOL_H_
#define CMSIS_PLUS_RTOS_OS_MEMPOOL_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

#include <cmsis-plus/rtos/os-decls.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    namespace mempool
    {

      // ======================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

      /**
       * @brief Memory pool attributes.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class Attributes : public Named_object
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Create memory pool attributes.
         * @param [in] name Null terminated name. If `nullptr`, "-" is assigned.
         */
        Attributes (const char* name);

        /**
         * @cond ignore
         */
        Attributes (const Attributes&) = default;
        Attributes (Attributes&&) = default;
        Attributes&
        operator= (const Attributes&) = default;
        Attributes&
        operator= (Attributes&&) = default;
        /**
         * @endcond
         */

        /**
         * @brief Destroy the memory pool attributes.
         */
        ~Attributes () = default;

        /**
         * @}
         */

      public:

        /**
         * @name Public Member Variables
         * @{
         */

        // Public members, no accessors and mutators required.
        // Warning: must match the type & order of the C file header.
        /**
         * @brief User defined memory pool address.
         */
        void* mp_pool_address;

        /**
         * @brief User defined memory pool size.
         */
        std::size_t mp_pool_size_bytes;

        //
        // TODO: add clock ID.
        //
        // Add more attributes.

        /**
         * @}
         */
      };

      /**
       * @brief Default memory pool initialiser.
       */
      extern const Attributes initializer;

    } /* namespace mempool */

#pragma GCC diagnostic push

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @brief Synchronised **memory pool**.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos
     */
    class Memory_pool : public Named_object
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a memory pool with default settings.
       * @param [in] blocks The maximum number of items in the pool.
       * @param [in] block_size_bytes The size of an item, in bytes.
       */
      Memory_pool (mempool::size_t blocks, mempool::size_t block_size_bytes);

      /**
       * @brief Create a memory pool with custom settings.
       * @param [in] attr Reference to attributes.
       * @param [in] blocks The maximum number of items in the pool.
       * @param [in] block_size_bytes The size of an item, in bytes.
       */
      Memory_pool (const mempool::Attributes& attr, mempool::size_t blocks,
                   mempool::size_t block_size_bytes);

      /**
       * @cond ignore
       */
      Memory_pool (const Memory_pool&) = delete;
      Memory_pool (Memory_pool&&) = delete;
      Memory_pool&
      operator= (const Memory_pool&) = delete;
      Memory_pool&
      operator= (Memory_pool&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the memory pool.
       */
      ~Memory_pool ();

      /**
       * @}
       * @name Operators
       * @{
       */

      /**
       * @brief Compare memory pools.
       * @retval true The given memory pool is the same as this memory pool.
       * @retval false The memory pools are different.
       */
      bool
      operator== (const Memory_pool& rhs) const;

      /**
       * @}
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Allocate a memory block.
       * @par Parameters
       *  None
       * @return Pointer to memory block, or `nullptr` if interrupted.
       */
      void*
      alloc (void);

      /**
       * @brief Allocate a memory block.
       * @par Parameters
       *  None
       * @return Pointer to memory block, or `nullptr` if no memory available.
       */
      void*
      try_alloc (void);

      /**
       * @brief Allocate a memory block with timeout.
       * @param [in] timeout Timeout to wait, in clock units (ticks or seconds).
       * @return Pointer to memory block, or `nullptr` if timeout.
       */
      void*
      timed_alloc (clock::duration_t timeout);

      /**
       * @brief Free the memory block.
       * @par Parameters
       *  None
       * @retval result::ok The memory block was released.
       * @retval EINVAL The block does not belong to the memory pool.
       */
      result_t
      free (void* block);

      /**
       * @brief Get memory pool capacity.
       * @par Parameters
       *  None
       * @return The max number of blocks in the pool.
       */
      std::size_t
      capacity (void) const;

      /**
       * @brief Get blocks count.
       * @par Parameters
       *  None
       * @return The number of blocks used from the queue.
       */
      std::size_t
      count (void) const;

      /**
       * @brief Get block size.
       * @par Parameters
       *  None
       * @return The block size, in bytes.
       */
      std::size_t
      block_size (void) const;

      /**
       * @brief Check if the memory pool is empty.
       * @par Parameters
       *  None
       * @retval true The memory pool has no allocated blocks.
       * @retval false The memory pool has allocated blocks.
       */
      bool
      empty (void) const;

      /**
       * @brief Check if the memory pool is full.
       * @par Parameters
       *  None
       * @retval true All memory blocks are allocated.
       * @retval false There are still memory blocks that can be allocated.
       */
      bool
      full (void) const;

      /**
       * @brief Reset the memory pool.
       * @par Parameters
       *  None
       * @retval result::ok The memory pool was reset.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       */
      result_t
      reset (void);

      /**
       *
       * @par Parameters
       *  None
       */
      void*
      pool (void);

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Functions
       * @{
       */

      /**
       * @brief Internal initialisation.
       * @par Parameters
       *  None
       */
      void
      _init (void);

      /**
       * @brief Internal function used to get the first linked block.
       * @par Parameters
       *  None
       * @return Pointer to block or `nullptr` if no more blocks available.
       */
      void*
      _try_first (void);

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Variables
       * @{
       */

#if !defined(OS_INCLUDE_RTOS_PORT_MEMORY_POOL)
      port::Waiting_threads_list list_;
#endif
      /**
       * @brief Memory address where the pool is located.
       * @details
       * If the pool was dynamically allocated, the `flags_allocated` bit
       * will be set in `flags_` and the destructor will deallocate..
       */
      char* pool_addr_;

#if defined(OS_INCLUDE_RTOS_PORT_MEMORY_POOL)
      friend class port::Memory_pool;
      os_mempool_port_data_t port_;
#endif

      /**
       * @brief The number of blocks in the pool.
       */
      const mempool::size_t blocks_;

      /**
       * @brief The size of a block, in bytes.
       */
      const mempool::size_t block_size_bytes_;

      /**
       * @brief The current number of blocks allocated from the pool.
       */
      volatile mempool::size_t count_;

      // All accesses will be done inside a critical section,
      // the volatile may not be needed, the variable remains stable
      // during the critical section and no loops wait for this variable.
      /**
       * @brief Pointer to the first free block, or nullptr.
       */
      void* volatile first_;

      /**
       * @brief Internal status bits.
       */
      uint8_t flags_;

      /**
       * @brief Internal bits.
       */
      enum
        : uint8_t
          {
            /**
             * @brief Remember to free the allocated memory block.
             */
            flags_allocated = 1
      };

      // Add more internal data.

      /**
       * @}
       */
    };

#pragma GCC diagnostic pop

  } /* namespace rtos */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace rtos
  {
    namespace mempool
    {
      inline
      Attributes::Attributes (const char* name) :
          Named_object (name)
      {
        mp_pool_address = nullptr;
        mp_pool_size_bytes = 0;
      }

    } /* namespace mempool */

    // ========================================================================

    /**
     * @details
     * Identical memory pools should have the same memory address.
     */
    inline bool
    Memory_pool::operator== (const Memory_pool& rhs) const
    {
      return this == &rhs;
    }

    inline std::size_t
    Memory_pool::capacity (void) const
    {
      return blocks_;
    }

    inline std::size_t
    Memory_pool::block_size (void) const
    {
      return block_size_bytes_;
    }

    inline std::size_t
    Memory_pool::count (void) const
    {
      return count_;
    }

    inline bool
    Memory_pool::empty (void) const
    {
      return (count () == 0);
    }

    inline bool
    Memory_pool::full (void) const
    {
      return (count () == capacity ());
    }

    inline void*
    Memory_pool::pool (void)
    {
      return pool_addr_;
    }

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_MEMPOOL_H_ */
