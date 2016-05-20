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

/*
 * The initial CMSIS++ RTOS API was inspired by CMSIS RTOS API v1.x,
 * Copyright (c) 2013 ARM LIMITED.
 */

#ifndef CMSIS_PLUS_RTOS_OS_MEMPOOL_H_
#define CMSIS_PLUS_RTOS_OS_MEMPOOL_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

#include <cmsis-plus/rtos/os-decls.h>
#include <cmsis-plus/rtos/os-memory.h>

// Better be the last, to undef putchar()
#include <cmsis-plus/diag/trace.h>

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
      class Attributes : public Clocked_attributes
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
        constexpr
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
        void* mp_pool_address = nullptr;

        /**
         * @brief User defined memory pool size.
         */
        std::size_t mp_pool_size_bytes = 0;

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

      // Storage for a message queue. Each message is stored in an element
      // extended to a multiple of pointers. The lists are kept in two arrays
      // of indices and the priorities in a separate array.
      template<typename T, std::size_t blocks, std::size_t block_size_bytes>
        class Arena
        {
        public:
          T pool[(blocks * block_size_bytes + sizeof(T) - 1) / sizeof(T)];
        };

      template<typename T>
        constexpr std::size_t
        compute_allocated_size_bytes (std::size_t blocks,
                                      std::size_t block_size_bytes)
        {
          // Align each block
          return (blocks
              * ((block_size_bytes + (sizeof(T) - 1)) & ~(sizeof(T) - 1)));
        }

    } /* namespace mempool */

#pragma GCC diagnostic push

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @brief Synchronised **memory pool**, using the
     * default RTOS allocator.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos
     */
    class Memory_pool : public Named_object
    {
    public:

      using Allocator = memory::allocator<stack::allocation_element_t>;

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a memory pool with default settings.
       * @param [in] blocks The maximum number of items in the pool.
       * @param [in] block_size_bytes The size of an item, in bytes.
       * @param [in] allocator Reference to allocator. Default a local temporary instance.
       */
      Memory_pool (mempool::size_t blocks, mempool::size_t block_size_bytes,
                   const Allocator& allocator = Allocator ());

      /**
       * @brief Create a named memory pool with default settings.
       * @param [in] name Pointer to name.
       * @param [in] blocks The maximum number of items in the pool.
       * @param [in] block_size_bytes The size of an item, in bytes.
       * @param [in] allocator Reference to allocator. Default a local temporary instance.
       */
      Memory_pool (const char* name, mempool::size_t blocks,
                   mempool::size_t block_size_bytes,
                   const Allocator& allocator = Allocator ());

      /**
       * @brief Create a memory pool with custom settings.
       * @param [in] attr Reference to attributes.
       * @param [in] blocks The maximum number of items in the pool.
       * @param [in] block_size_bytes The size of an item, in bytes.
       * @param [in] allocator Reference to allocator. Default a local temporary instance.
       */
      Memory_pool (const mempool::Attributes& attr, mempool::size_t blocks,
                   mempool::size_t block_size_bytes,
                   const Allocator& allocator = Allocator ());
      /**
       * @brief Create a named memory pool with custom settings.
       * @param [in] name Pointer to name.
       * @param [in] attr Reference to attributes.
       * @param [in] blocks The maximum number of items in the pool.
       * @param [in] block_size_bytes The size of an item, in bytes.
       * @param [in] allocator Reference to allocator. Default a local temporary instance.
       */
      Memory_pool (const char* name, const mempool::Attributes& attr,
                   mempool::size_t blocks, mempool::size_t block_size_bytes,
                   const Allocator& allocator = Allocator ());

      /**
       * @cond ignore
       */

    protected:

      // Internal constructors, used from templates.
      Memory_pool ();
      Memory_pool (const char* name);
      Memory_pool (const char* given_name, const char* attr_name);

    public:

      Memory_pool (const Memory_pool&) = delete;
      Memory_pool (Memory_pool&&) = delete;
      Memory_pool&
      operator= (const Memory_pool&) = delete;
      Memory_pool&
      operator= (Memory_pool&&) = delete;
      /**
       * @endcond
       */

    public:

      /**
       * @brief Destroy the memory pool.
       */
      virtual
      ~Memory_pool ();

      /**
       * @}
       */

      /*
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
       */

    public:

      /**
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
       * @brief Internal function used during memory pool construction.
       * @param [in] attr Reference to attributes.
       * @param [in] blocks The maximum number of items in the pool.
       * @param [in] block_size_bytes The size of an item, in bytes.
       * @param [in] queue_address Pointer to queue storage.
       * @param [in] queue_size_bytes Size of queue storage.
       */
      void
      _construct (const mempool::Attributes& attr, mempool::size_t blocks,
                  mempool::size_t block_size_bytes, void* pool_address,
                  std::size_t pool_size_bytes);

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
      /**
       * @brief List of threads waiting to alloc.
       */
      Waiting_threads_list list_;
      /**
       * @brief Pointer to clock to be used for timeouts.
       */
      Clock* clock_ = nullptr;
#endif
      /**
       * @brief The static address where the pool is stored
       * (from `attr.mp_pool_address`).
       */
      void* pool_addr_ = nullptr;
      /**
       * @brief The dynamic address if the pool was allocated
       * (and must be deallocated)
       */
      void* allocated_pool_addr_ = nullptr;

      /**
       * @brief Pointer to allocator.
       */
      const void* allocator_ = nullptr;

#if defined(OS_INCLUDE_RTOS_PORT_MEMORY_POOL)
      friend class port::Memory_pool;
      os_mempool_port_data_t port_;
#endif

      /**
       * @brief Total size of the statically allocated pool storage
       * (from `attr.mp_pool_size_bytes`).
       */
      std::size_t pool_size_bytes_ = 0;
      /**
       * @brief Total size of the dynamically allocated pool storage.
       */
      std::size_t allocated_pool_size_elements_ = 0;

      /**
       * @brief The number of blocks in the pool.
       */
      mempool::size_t blocks_ = 0;

      /**
       * @brief The size of a block, in bytes.
       */
      mempool::size_t block_size_bytes_ = 0;

      /**
       * @brief The current number of blocks allocated from the pool.
       */
      volatile mempool::size_t count_ = 0;

      // All accesses will be done inside a critical section,
      // the volatile may not be needed, the variable remains stable
      // during the critical section and no loops wait for this variable.
      /**
       * @brief Pointer to the first free block, or nullptr.
       */
      void* volatile first_ = nullptr;

      /**
       * @}
       */
    };

    // ========================================================================

    /**
     * @brief Template of a synchronised **memory pool** with allocator.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos
     */
    template<typename Allocator = memory::allocator<void*>>
      class Memory_pool_allocated : public Memory_pool
      {
      public:

        using allocator_type = Allocator;

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Create a memory pool with default settings.
         * @param [in] blocks The maximum number of items in the pool.
         * @param [in] block_size_bytes The size of an item, in bytes.
         * @param [in] allocator Reference to allocator. Default a local temporary instance.
         */
        Memory_pool_allocated (mempool::size_t blocks,
                               mempool::size_t block_size_bytes,
                               const Allocator& allocator = Allocator ());

        /**
         * @brief Create a named memory pool with default settings.
         * @param [in] name Pointer to name.
         * @param [in] blocks The maximum number of items in the pool.
         * @param [in] block_size_bytes The size of an item, in bytes.
         * @param [in] allocator Reference to allocator. Default a local temporary instance.
         */
        Memory_pool_allocated (const char* name, mempool::size_t blocks,
                               mempool::size_t block_size_bytes,
                               const Allocator& allocator = Allocator ());

        /**
         * @brief Create a memory pool with custom settings.
         * @param [in] attr Reference to attributes.
         * @param [in] blocks The maximum number of items in the pool.
         * @param [in] block_size_bytes The size of an item, in bytes.
         * @param [in] allocator Reference to allocator. Default a local temporary instance.
         */
        Memory_pool_allocated (const mempool::Attributes& attr,
                               mempool::size_t blocks,
                               mempool::size_t block_size_bytes,
                               const Allocator& allocator = Allocator ());

        /**
         * @brief Create a named memory pool with custom settings.
         * @param [in] name Pointer to name.
         * @param [in] attr Reference to attributes.
         * @param [in] blocks The maximum number of items in the pool.
         * @param [in] block_size_bytes The size of an item, in bytes.
         * @param [in] allocator Reference to allocator. Default a local temporary instance.
         */
        Memory_pool_allocated (const char* name,
                               const mempool::Attributes& attr,
                               mempool::size_t blocks,
                               mempool::size_t block_size_bytes,
                               const Allocator& allocator = Allocator ());

        /**
         * @cond ignore
         */
        Memory_pool_allocated (const Memory_pool_allocated&) = delete;
        Memory_pool_allocated (Memory_pool_allocated&&) = delete;
        Memory_pool_allocated&
        operator= (const Memory_pool_allocated&) = delete;
        Memory_pool_allocated&
        operator= (Memory_pool_allocated&&) = delete;
        /**
         * @endcond
         */

        /**
         * @brief Destroy the memory pool.
         */
        virtual
        ~Memory_pool_allocated ();

        /**
         * @}
         */

      };

    // ========================================================================

    /**
     * @brief Template of a synchronised **memory pool** with
     * block type and allocator.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos
     */
    template<typename T, typename Allocator = memory::allocator<void*>>
      class Memory_pool_typed : public Memory_pool_allocated<Allocator>
      {
      public:

        using value_type = T;
        using allocator_type = Allocator;

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Create a memory pool with default settings.
         * @param [in] blocks The maximum number of items in the pool.
         * @param [in] allocator Reference to allocator. Default a local temporary instance.
         */
        Memory_pool_typed (mempool::size_t blocks, const Allocator& allocator =
                               Allocator ());

        /**
         * @brief Create a named memory pool with default settings.
         * @param [in] name Pointer to name.
         * @param [in] blocks The maximum number of items in the pool.
         * @param [in] allocator Reference to allocator. Default a local temporary instance.
         */
        Memory_pool_typed (const char* name, mempool::size_t blocks,
                           const Allocator& allocator = Allocator ());

        /**
         * @brief Create a memory pool with custom settings.
         * @param [in] attr Reference to attributes.
         * @param [in] blocks The maximum number of items in the pool.
         * @param [in] allocator Reference to allocator. Default a local temporary instance.
         */
        Memory_pool_typed (const mempool::Attributes& attr,
                           mempool::size_t blocks, const Allocator& allocator =
                               Allocator ());

        /**
         * @brief Create a named memory pool with custom settings.
         * @param [in] name Pointer to name.
         * @param [in] attr Reference to attributes.
         * @param [in] blocks The maximum number of items in the pool.
         * @param [in] allocator Reference to allocator. Default a local temporary instance.
         */
        Memory_pool_typed (const char* name, const mempool::Attributes& attr,
                           mempool::size_t blocks, const Allocator& allocator =
                               Allocator ());

        /**
         * @cond ignore
         */
        Memory_pool_typed (const Memory_pool_typed&) = delete;
        Memory_pool_typed (Memory_pool_typed&&) = delete;
        Memory_pool_typed&
        operator= (const Memory_pool_typed&) = delete;
        Memory_pool_typed&
        operator= (Memory_pool_typed&&) = delete;
        /**
         * @endcond
         */

        /**
         * @brief Destroy the memory pool.
         */
        virtual
        ~Memory_pool_typed ();

        /**
         * @}
         */

      public:

        /**
         * @name Public Member Functions
         * @{
         */

        /**
         * @brief Allocate a memory block.
         * @par Parameters
         *  None
         * @return Pointer to memory block, or `nullptr` if interrupted.
         */
        value_type*
        alloc (void);

        /**
         * @brief Allocate a memory block.
         * @par Parameters
         *  None
         * @return Pointer to memory block, or `nullptr` if no memory available.
         */
        value_type*
        try_alloc (void);

        /**
         * @brief Allocate a memory block with timeout.
         * @param [in] timeout Timeout to wait, in clock units (ticks or seconds).
         * @return Pointer to memory block, or `nullptr` if timeout.
         */
        value_type*
        timed_alloc (clock::duration_t timeout);

        /**
         * @brief Free the memory block.
         * @par Parameters
         *  None
         * @retval result::ok The memory block was released.
         * @retval EINVAL The block does not belong to the memory pool.
         */
        result_t
        free (value_type* block);

        /**
         * @}
         */

      };

    // ========================================================================

    /**
     * @brief Template of a synchronised **memory pool** with
     * block type and local storage.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos
     */
    template<typename T, std::size_t N>
      class Memory_pool_static : public Memory_pool
      {
      public:

        using value_type = T;
        static const std::size_t blocks = N;

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Create a memory pool with default settings.
         */
        Memory_pool_static ();

        /**
         * @brief Create a named memory pool with default settings.
         * @param [in] name Pointer to name.
         */
        Memory_pool_static (const char* name);

        /**
         * @brief Create a memory pool with custom settings.
         * @param [in] attr Reference to attributes.
         */
        Memory_pool_static (const mempool::Attributes& attr);

        /**
         * @brief Create a named memory pool with custom settings.
         * @param [in] name Pointer to name.
         * @param [in] attr Reference to attributes.
         */
        Memory_pool_static (const char* name, const mempool::Attributes& attr);

        /**
         * @cond ignore
         */
        Memory_pool_static (const Memory_pool_static&) = delete;
        Memory_pool_static (Memory_pool_static&&) = delete;
        Memory_pool_static&
        operator= (const Memory_pool_static&) = delete;
        Memory_pool_static&
        operator= (Memory_pool_static&&) = delete;
        /**
         * @endcond
         */

        /**
         * @brief Destroy the memory pool.
         */
        virtual
        ~Memory_pool_static ();

        /**
         * @}
         */

      protected:

        /**
         * @name Private Member Variables
         * @{
         */

        /**
         * @brief Local storage for the pool.
         * @details
         * The local storage is large enough to include `blocks`
         * blocks of type `T`.
         * For performance reasons, the individual components are
         * aligned as pointers.
         */
        mempool::Arena<void*, blocks, sizeof(T)> arena_;

        /**
         * @}
         */

      public:

        /**
         * @name Public Member Functions
         * @{
         */

        /**
         * @brief Allocate a memory block.
         * @par Parameters
         *  None
         * @return Pointer to memory block, or `nullptr` if interrupted.
         */
        value_type*
        alloc (void);

        /**
         * @brief Allocate a memory block.
         * @par Parameters
         *  None
         * @return Pointer to memory block, or `nullptr` if no memory available.
         */
        value_type*
        try_alloc (void);

        /**
         * @brief Allocate a memory block with timeout.
         * @param [in] timeout Timeout to wait, in clock units (ticks or seconds).
         * @return Pointer to memory block, or `nullptr` if timeout.
         */
        value_type*
        timed_alloc (clock::duration_t timeout);

        /**
         * @brief Free the memory block.
         * @par Parameters
         *  None
         * @retval result::ok The memory block was released.
         * @retval EINVAL The block does not belong to the memory pool.
         */
        result_t
        free (value_type* block);

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
      constexpr
      Attributes::Attributes (const char* name) :
          Clocked_attributes
            { name }
      {
        ;
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

    // ========================================================================

    /**
     * This constructor shall initialise a memory pool object
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
     * For default memory pool objects, the storage is dynamically
     * allocated using the RTOS specific allocator
     * (`rtos::memory::allocator`).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */

    template<typename Allocator>
      inline
      Memory_pool_allocated<Allocator>::Memory_pool_allocated (
          mempool::size_t blocks, mempool::size_t block_size_bytes,
          const Allocator& allocator) :
          Memory_pool_allocated
            { nullptr, blocks, block_size_bytes, allocator }
      {
        ;
      }

    /**
     * This constructor shall initialise a named memory pool object
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
     * For default memory pool objects, the storage is dynamically
     * allocated using the RTOS specific allocator
     * (`rtos::memory::allocator`).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */

    template<typename Allocator>
      Memory_pool_allocated<Allocator>::Memory_pool_allocated (
          const char* name, mempool::size_t blocks,
          mempool::size_t block_size_bytes, const Allocator& allocator) :
          Memory_pool
            { name }
      {
#if defined(OS_TRACE_RTOS_MEMPOOL)
        trace::printf ("%s() @%p %s %d %d\n", __func__, this, this->name (),
                       blocks, block_size_bytes);
#endif

        allocator_ = &allocator;

        allocated_pool_size_elements_ = (mempool::compute_allocated_size_bytes<
            typename Allocator::value_type> (blocks, block_size_bytes)
            + sizeof(typename Allocator::value_type) - 1)
            / sizeof(typename Allocator::value_type);

        allocated_pool_addr_ = const_cast<Allocator&> (allocator).allocate (
            allocated_pool_size_elements_);

        _construct (
            mempool::initializer,
            blocks,
            block_size_bytes,
            allocated_pool_addr_,
            allocated_pool_size_elements_
                * sizeof(typename Allocator::value_type));
      }

    /**
     * This constructor shall initialise a memory pool object
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
     * If the attributes define a storage area (via `mp_pool_address` and
     * `mp_pool_size_bytes`), that storage is used, otherwise
     * the storage is dynamically allocated using the RTOS specific allocator
     * (`rtos::memory::allocator`).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename Allocator>
      inline
      Memory_pool_allocated<Allocator>::Memory_pool_allocated (
          const mempool::Attributes& attr, mempool::size_t blocks,
          mempool::size_t block_size_bytes, const Allocator& allocator) :
          Memory_pool_allocated
            { nullptr, attr, blocks, block_size_bytes, allocator }
      {
        ;
      }

    /**
     * This constructor shall initialise a named memory pool object
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
     * If the attributes define a storage area (via `mp_pool_address` and
     * `mp_pool_size_bytes`), that storage is used, otherwise
     * the storage is dynamically allocated using the RTOS specific allocator
     * (`rtos::memory::allocator`).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename Allocator>
      Memory_pool_allocated<Allocator>::Memory_pool_allocated (
          const char* name, const mempool::Attributes& attr,
          mempool::size_t blocks, mempool::size_t block_size_bytes,
          const Allocator& allocator) :
          Memory_pool
            { name, attr.name () }
      {
#if defined(OS_TRACE_RTOS_MEMPOOL)
        trace::printf ("%s() @%p %s %d %d\n", __func__, this, this->name (),
                       blocks, block_size_bytes);
#endif
        if (attr.mp_pool_address != nullptr)
          {
            // Do not use any allocator at all.
            _construct (attr, blocks, block_size_bytes, nullptr, 0);
          }
        else
          {
            allocator_ = &allocator;

            // If no user storage was provided via attributes,
            // allocate it dynamically via the allocator.
            allocated_pool_size_elements_ =
                (mempool::compute_allocated_size_bytes<
                    typename Allocator::value_type> (blocks, block_size_bytes)
                    + sizeof(typename Allocator::value_type) - 1)
                    / sizeof(typename Allocator::value_type);

            allocated_pool_addr_ = const_cast<Allocator&> (allocator).allocate (
                allocated_pool_size_elements_);

            _construct (
                attr,
                blocks,
                block_size_bytes,
                allocated_pool_addr_,
                allocated_pool_size_elements_
                    * sizeof(typename Allocator::value_type));
          }
      }

    /**
     * @details
     * This destructor shall destroy a memory pool object; the object
     * becomes, in effect, uninitialised. An implementation may cause
     * the destructor to set the object to an invalid value.
     *
     * It shall be safe to destroy an initialised memory pool object
     * upon which no threads are currently blocked. Attempting to
     * destroy a memory pool object upon which other threads are
     * currently blocked results in undefined behaviour.
     *
     * If the storage for the memory pool was dynamically allocated,
     * it is deallocated using the same allocator.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename Allocator>
      Memory_pool_allocated<Allocator>::~Memory_pool_allocated ()
      {
#if defined(OS_TRACE_RTOS_MEMPOOL)
        trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif
        typedef typename std::allocator_traits<Allocator>::pointer pointer;

        if (allocated_pool_addr_ != nullptr)
          {
            static_cast<Allocator*> (const_cast<void*> (allocator_))->deallocate (
                static_cast<pointer> (allocated_pool_addr_),
                allocated_pool_size_elements_);

            allocated_pool_addr_ = nullptr;
          }
      }

    // ========================================================================

    /**
     * This constructor shall initialise a memory pool object
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
     * For default memory pool objects, the storage is dynamically
     * allocated using the RTOS specific allocator
     * (`rtos::memory::allocator`).
     *
     * Implemented as a wrapper over the parent constructor, automatically
     * passing the block size.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename T, typename Allocator>
      inline
      Memory_pool_typed<T, Allocator>::Memory_pool_typed (
          mempool::size_t blocks, const Allocator& allocator) :
          Memory_pool_allocated<Allocator> (blocks, sizeof(T), allocator)
      {
        ;
      }

    /**
     * This constructor shall initialise a named memory pool object
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
     * For default memory pool objects, the storage is dynamically
     * allocated using the RTOS specific allocator
     * (`rtos::memory::allocator`).
     *
     * Implemented as a wrapper over the parent constructor, automatically
     * passing the block size.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename T, typename Allocator>
      inline
      Memory_pool_typed<T, Allocator>::Memory_pool_typed (
          const char* name, mempool::size_t blocks, const Allocator& allocator) :
          Memory_pool_allocated<Allocator> (name, blocks, sizeof(T), allocator)
      {
        ;
      }

    /**
     * This constructor shall initialise a memory pool object
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
     * If the attributes define a storage area (via `mp_pool_address` and
     * `mp_pool_size_bytes`), that storage is used, otherwise
     * the storage is dynamically allocated using the RTOS specific allocator
     * (`rtos::memory::allocator`).
     *
     * Implemented as a wrapper over the parent constructor, automatically
     * passing the message size.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename T, typename Allocator>
      inline
      Memory_pool_typed<T, Allocator>::Memory_pool_typed (
          const mempool::Attributes& attr, mempool::size_t blocks,
          const Allocator& allocator) :
          Memory_pool_allocated<Allocator> (attr, blocks, sizeof(T), allocator)
      {
        ;
      }

    /**
     * This constructor shall initialise a named memory pool object
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
     * If the attributes define a storage area (via `mp_pool_address` and
     * `mp_pool_size_bytes`), that storage is used, otherwise
     * the storage is dynamically allocated using the RTOS specific allocator
     * (`rtos::memory::allocator`).
     *
     * Implemented as a wrapper over the parent constructor, automatically
     * passing the message size.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename T, typename Allocator>
      inline
      Memory_pool_typed<T, Allocator>::Memory_pool_typed (
          const char* name, const mempool::Attributes& attr,
          mempool::size_t blocks, const Allocator& allocator) :
          Memory_pool_allocated<Allocator> (name, attr, blocks, sizeof(T),
                                            allocator)
      {
        ;
      }

    /**
     * @details
     * This destructor shall destroy a memory pool object; the object
     * becomes, in effect, uninitialised. An implementation may cause
     * the destructor to set the object to an invalid value.
     *
     * It shall be safe to destroy an initialised memory pool object
     * upon which no threads are currently blocked. Attempting to
     * destroy a memory pool object upon which other threads are
     * currently blocked results in undefined behaviour.
     *
     * If the storage for the memory pool was dynamically allocated,
     * it is deallocated using the same allocator.
     *
     * Implemented as a wrapper over the parent destructor.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename T, typename Allocator>
      Memory_pool_typed<T, Allocator>::~Memory_pool_typed ()
      {
        ;
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the cast.
     *
     * @see Memory_pool::alloc().
     */
    template<typename T, typename Allocator>
      inline typename Memory_pool_typed<T, Allocator>::value_type*
      Memory_pool_typed<T, Allocator>::alloc (void)
      {
        return static_cast<value_type*> (Memory_pool_allocated<Allocator>::alloc ());
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the cast.
     *
     * @see Memory_pool::try_alloc().
     */
    template<typename T, typename Allocator>
      inline typename Memory_pool_typed<T, Allocator>::value_type*
      Memory_pool_typed<T, Allocator>::try_alloc (void)
      {
        return static_cast<value_type*> (Memory_pool_allocated<Allocator>::try_alloc ());
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the cast.
     *
     * @see Memory_pool::timed_alloc().
     */
    template<typename T, typename Allocator>
      inline typename Memory_pool_typed<T, Allocator>::value_type*
      Memory_pool_typed<T, Allocator>::timed_alloc (clock::duration_t timeout)
      {
        return static_cast<value_type*> (Memory_pool_allocated<Allocator>::timed_alloc (
            timeout));
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the cast.
     *
     * @see Memory_pool::free().
     */
    template<typename T, typename Allocator>
      inline result_t
      Memory_pool_typed<T, Allocator>::free (value_type* block)
      {
        return Memory_pool_allocated<Allocator>::free (block);
      }

    // ========================================================================

    /**
     * This constructor shall initialise a memory pool object
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
     * The storage shall be statically allocated inside the
     * memory pool object instance.
     *
     * Implemented as a wrapper over the parent constructor, automatically
     * passing the block size and the storage details.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename T, std::size_t N>
      Memory_pool_static<T, N>::Memory_pool_static ()
      {
        _construct (mempool::initializer, blocks, sizeof(T), &arena_,
                    sizeof(arena_));
      }

    /**
     * This constructor shall initialise a named memory pool object
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
     * The storage shall be statically allocated inside the
     * memory pool object instance.
     *
     * Implemented as a wrapper over the parent constructor, automatically
     * passing the block size and the storage details.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename T, std::size_t N>
      Memory_pool_static<T, N>::Memory_pool_static (const char* name) :
          Memory_pool
            { name }
      {
        _construct (mempool::initializer, blocks, sizeof(T), &arena_,
                    sizeof(arena_));
      }

    /**
     * This constructor shall initialise a memory pool object
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
     * The storage shall be statically allocated inside the
     * memory pool object instance.
     *
     * Implemented as a wrapper over the parent constructor, automatically
     * passing the message size and the storage details.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename T, std::size_t N>
      Memory_pool_static<T, N>::Memory_pool_static (
          const mempool::Attributes& attr) :
          Memory_pool
            { attr.name () }
      {
        _construct (attr, blocks, sizeof(T), &arena_, sizeof(arena_));
      }

    /**
     * This constructor shall initialise a named memory pool object
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
     * The storage shall be statically allocated inside the
     * memory pool object instance.
     *
     * Implemented as a wrapper over the parent constructor, automatically
     * passing the message size and the storage details.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename T, std::size_t N>
      Memory_pool_static<T, N>::Memory_pool_static (
          const char* name, const mempool::Attributes& attr) :
          Memory_pool
            { name, attr.name () }
      {
        _construct (attr, blocks, sizeof(T), &arena_, sizeof(arena_));
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
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename T, std::size_t N>
      Memory_pool_static<T, N>::~Memory_pool_static ()
      {
        ;
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the cast.
     *
     * @see Memory_pool::alloc().
     */
    template<typename T, std::size_t N>
      inline typename Memory_pool_static<T, N>::value_type*
      Memory_pool_static<T, N>::alloc (void)
      {
        return static_cast<value_type*> (Memory_pool::alloc ());
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the cast.
     *
     * @see Memory_pool::try_alloc().
     */
    template<typename T, std::size_t N>
      inline typename Memory_pool_static<T, N>::value_type*
      Memory_pool_static<T, N>::try_alloc (void)
      {
        return static_cast<value_type*> (Memory_pool::try_alloc ());
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the cast.
     *
     * @see Memory_pool::timed_alloc().
     */
    template<typename T, std::size_t N>
      inline typename Memory_pool_static<T, N>::value_type*
      Memory_pool_static<T, N>::timed_alloc (clock::duration_t timeout)
      {
        return static_cast<value_type*> (Memory_pool::timed_alloc (timeout));
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the cast.
     *
     * @see Memory_pool::free().
     */
    template<typename T, std::size_t N>
      inline result_t
      Memory_pool_static<T, N>::free (value_type* block)
      {
        return Memory_pool::free (block);
      }

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_MEMPOOL_H_ */
