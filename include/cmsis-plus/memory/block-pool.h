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

#ifndef CMSIS_PLUS_MEMORY_BLOCK_POOL_H_
#define CMSIS_PLUS_MEMORY_BLOCK_POOL_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

#include <cmsis-plus/rtos/os.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace memory
  {

    // ========================================================================

    /**
     * @brief Memory resource managing a pool of same size blocks,
     *  using an existing arena.
     * @ingroup cmsis-plus-rtos-memres
     * @headerfile block-pool.h <cmsis-plus/memory/block-pool.h>
     *
     * @details
     * This class is a deterministic, non-fragmenting memory
     * manager, that allocates identical size blocks from a pool.
     *
     * This memory manager is ideal for allocation of system objects.
     *
     * The only drawback is that the maximum number of objects must be
     * known before the first allocations, but usually this is
     * not a problem.
     */
    class block_pool : public rtos::memory::memory_resource
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct a memory resource object instance.
       * @param [in] blocks The maximum number of items in the pool.
       * @param [in] block_size_bytes The size of an item, in bytes.
       * @param [in] addr Begin of allocator arena.
       * @param [in] bytes Size of allocator arena, in bytes.
       */
      block_pool (std::size_t blocks, std::size_t block_size_bytes, void* addr,
                  std::size_t bytes);

      /**
       * @brief Construct a named memory resource object instance.
       * @param name Pointer to name.
       * @param [in] blocks The maximum number of items in the pool.
       * @param [in] block_size_bytes The size of an item, in bytes.
       * @param [in] addr Begin of allocator arena.
       * @param [in] bytes Size of allocator arena, in bytes.
       */
      block_pool (const char* name, std::size_t blocks,
                  std::size_t block_size_bytes, void* addr, std::size_t bytes);

    protected:

      /**
       * @brief Default constructor. Construct a memory resource
       *  object instance.
       */
      block_pool (const char* name);

    public:

      /**
       * @cond ignore
       */

      // The rule of five.
      block_pool (const block_pool&) = delete;
      block_pool (block_pool&&) = delete;
      block_pool&
      operator= (const block_pool&) = delete;
      block_pool&
      operator= (block_pool&&) = delete;

      /**
       * @endcond
       */

      /**
       * @brief Destruct the memory resource object instance.
       */
      virtual
      ~block_pool ();

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Functions
       * @{
       */

      /**
       * @brief Internal function to construct the memory resource object instance.
       * @param [in] blocks The maximum number of items in the pool.
       * @param [in] block_size_bytes The size of an item, in bytes.
       * @param [in] addr Begin of allocator arena.
       * @param [in] bytes Size of allocator arena, in bytes.
       * @par Returns
       *  Nothing.
       */
      void
      internal_construct_ (std::size_t blocks, std::size_t block_size_bytes,
                           void* addr, std::size_t bytes) noexcept;

      /**
       * @brief Internal function to reset the memory resource object.
       * @par Parameters
       *  None.
       * @par Returns
       *  Nothing.
       */
      void
      internal_reset_ (void) noexcept;

      /**
       * @brief Implementation of the memory allocator.
       * @param [in] bytes Number of bytes to allocate.
       * @param [in] alignment Alignment constraint (power of 2).
       * @return Pointer to newly allocated block, or `nullptr`.
       */
      virtual void*
      do_allocate (std::size_t bytes, std::size_t alignment) override;

      /**
       * @brief Implementation of the memory deallocator.
       * @param [in] addr Address of a previously allocated block to free.
       * @param [in] bytes Number of bytes to deallocate (may be 0 if unknown).
       * @param [in] alignment Alignment constraint (power of 2).
       * @par Returns
       *  Nothing.
       */
      virtual void
      do_deallocate (void* addr, std::size_t bytes, std::size_t alignment)
          noexcept override;

      /**
       * @brief Implementation of the function to get max size.
       * @par Parameters
       *  None.
       * @return Integer with size in bytes, or 0 if unknown.
       */
      virtual std::size_t
      do_max_size (void) const noexcept override;

      /**
       * @brief Implementation of the function to reset the memory manager.
       * @par Parameters
       *  None.
       * @par Returns
       *  Nothing.
       */
      virtual void
      do_reset (void) noexcept override;

      /**
       * @}
       */

    protected:

      /**
       * @cond ignore
       */

      /**
       * @brief The static address where the pool is stored.
       * (from `attr.mp_pool_address`).
       */
      void* pool_addr_ = nullptr;

      /**
       * @brief Pointer to the first free block, or nullptr.
       */
      void* volatile first_ = nullptr;

      /**
       * @brief The number of blocks in the pool.
       */
      std::size_t blocks_ = 0;

      /**
       * @brief The size of a block, in bytes.
       */
      std::size_t block_size_bytes_ = 0;

      /**
       * @brief The current number of blocks allocated from the pool.
       */
      volatile std::size_t count_ = 0;

      /**
       * @endcond
       */

    };

    // ========================================================================

    /**
     * @brief Memory resource managing an internal pool.
     *  of same size blocks of type T.
     * @ingroup cmsis-plus-rtos-memres
     * @headerfile block-pool.h <cmsis-plus/memory/block-pool.h>
     *
     * @details
     * This class template is a convenience class that includes
     * an array of objects to be used as the pool.
     *
     * The common use case it to define statically allocated block pools.
     */
    template<typename T, std::size_t N>
      class block_pool_typed_inclusive : public block_pool
      {
      public:

        /**
         * @brief Standard allocator type definition.
         */
        using value_type = T;

        static_assert(sizeof(value_type) >= sizeof(void*),
            "Template type T must be large enough to store a pointer.");

        /**
         * @brief Local constant based on template definition.
         */
        static const std::size_t blocks = N;

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a memory resource object instance.
         * @par Parameters
         *  None.
         */
        block_pool_typed_inclusive (void);

        /**
         * @brief Construct a named memory resource object instance.
         * @param [in] name Pointer to name.
         */
        block_pool_typed_inclusive (const char* name);

      public:

        /**
         * @cond ignore
         */

        // The rule of five.
        block_pool_typed_inclusive (const block_pool_typed_inclusive&) = delete;
        block_pool_typed_inclusive (block_pool_typed_inclusive&&) = delete;
        block_pool_typed_inclusive&
        operator= (const block_pool_typed_inclusive&) = delete;
        block_pool_typed_inclusive&
        operator= (block_pool_typed_inclusive&&) = delete;

        /**
         * @endcond
         */

        /**
         * @brief Destruct the memory resource object instance.
         */
        virtual
        ~block_pool_typed_inclusive ();

        /**
         * @}
         */

      protected:

        /**
         * @cond ignore
         */

        /**
         * @brief The allocation arena is an array of objects.
         */
        typename std::aligned_storage<sizeof(value_type), alignof(value_type)>::type arena_[blocks];

        /**
         * @endcond
         */

      };

    // ========================================================================

    /**
     * @brief Memory resource managing a dynamically allocated pool.
     *  of same size blocks of type T.
     * @ingroup cmsis-plus-rtos-memres
     * @headerfile block-pool.h <cmsis-plus/memory/block-pool.h>
     *
     * @details
     * This class template is as a convenience class that
     * allocates an array of objects to be used for the pool.
     *
     * The common use case it to define dynamically allocated block pools.
     *
     * @note The allocator must be parametrised with the same block type.
     */
    template<typename T, typename A = os::rtos::memory::allocator<T>>
      class block_pool_typed_allocated : public block_pool
      {
      public:

        /**
         * @brief Standard allocator type definition.
         */
        using value_type = T;

        static_assert(sizeof(value_type) >= sizeof(void*),
            "Template type T must be large enough to store a pointer.");

        /**
         * @brief Standard allocator type definition.
         */
        using allocator_type = A;

        /**
         * @brief Standard allocator traits definition.
         */
        using allocator_traits = std::allocator_traits<A>;

        // It is recommended to have the same type, but at least the types
        // should have the same size.
        static_assert(sizeof(value_type) == sizeof(typename allocator_traits::value_type),
            "The allocator must be parametrised with a type of same size.");

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a memory resource object instance.
         * @param [in] blocks The maximum number of items in the pool.
         * @param [in] allocator Reference to allocator. Default a
         * local temporary instance.
         */
        block_pool_typed_allocated (std::size_t blocks,
                                    const allocator_type& allocator =
                                        allocator_type ());

        /**
         * @brief Construct a named memory resource object instance.
         * @param [in] name Pointer to name.
         * @param [in] blocks The maximum number of items in the pool.
         * @param [in] allocator Reference to allocator. Default a
         * local temporary instance.
         */
        block_pool_typed_allocated (const char* name, std::size_t blocks,
                                    const allocator_type& allocator =
                                        allocator_type ());

      public:

        /**
         * @cond ignore
         */

        // The rule of five.
        block_pool_typed_allocated (const block_pool_typed_allocated&) = delete;
        block_pool_typed_allocated (block_pool_typed_allocated&&) = delete;
        block_pool_typed_allocated&
        operator= (const block_pool_typed_allocated&) = delete;
        block_pool_typed_allocated&
        operator= (block_pool_typed_allocated&&) = delete;

        /**
         * @endcond
         */

        /**
         * @brief Destruct the memory resource object instance.
         */
        virtual
        ~block_pool_typed_allocated ();

        /**
         * @}
         */

      protected:

        /**
         * @cond ignore
         */

        /**
         * @brief Pointer to allocator.
         * @details
         * The allocator is remembered because deallocation
         * must be performed during destruction. A more automated
         * solution using a unique_ptr<> would require more RAM
         * and is considered not justified.
         */
        allocator_type* allocator_ = nullptr;

        /**
         * @endcond
         */

      };

  // -------------------------------------------------------------------------
  } /* namespace memory */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace memory
  {

    // ========================================================================

    inline
    block_pool::block_pool (const char* name) :
        rtos::memory::memory_resource
          { name }
    {
      ;
    }

    inline
    block_pool::block_pool (std::size_t blocks, std::size_t block_size_bytes,
                            void* addr, std::size_t bytes) :
        block_pool
          { nullptr, blocks, block_size_bytes, addr, bytes }
    {
      ;
    }

    inline
    block_pool::block_pool (const char* name, std::size_t blocks,
                            std::size_t block_size_bytes, void* addr,
                            std::size_t bytes) :
        rtos::memory::memory_resource
          { name }
    {
      trace::printf ("%s(%u,%u,%p,%u) @%p %s\n", __func__, blocks,
                     block_size_bytes, addr, bytes, this, this->name ());

      internal_construct_ (blocks, block_size_bytes, addr, bytes);
    }

    // ========================================================================

    template<typename T, std::size_t N>
      inline
      block_pool_typed_inclusive<T, N>::block_pool_typed_inclusive () :
          block_pool_typed_inclusive (nullptr)
      {
        ;
      }

    template<typename T, std::size_t N>
      inline
      block_pool_typed_inclusive<T, N>::block_pool_typed_inclusive (
          const char* name) :
          block_pool
            { name }
      {
        trace::printf ("%s() @%p %s\n", __func__, this, this->name ());

        internal_construct_ (blocks, sizeof(value_type), &arena_[0],
                             sizeof(arena_));
      }

    template<typename T, std::size_t N>
      block_pool_typed_inclusive<T, N>::~block_pool_typed_inclusive ()
      {
        trace::printf ("%s() @%p %s\n", __func__, this, this->name ());
      }

    // ========================================================================

    template<typename T, typename A>
      inline
      block_pool_typed_allocated<T, A>::block_pool_typed_allocated (
          std::size_t blocks, const allocator_type& allocator) :
          block_pool_typed_allocated (nullptr, blocks, allocator)
      {
        ;
      }

    template<typename T, typename A>
      block_pool_typed_allocated<T, A>::block_pool_typed_allocated (
          const char* name, std::size_t blocks, const allocator_type& allocator) :
          block_pool
            { name }
      {
        trace::printf ("%s(%u,%p) @%p %s\n", __func__, blocks, &allocator, this,
                       this->name ());

        // Remember the allocator, it'll be used by the destructor.
        allocator_ =
            static_cast<allocator_type*> (&const_cast<allocator_type&> (allocator));

        void* addr = allocator_->allocate (blocks);
        if (addr == nullptr)
          {
            estd::__throw_bad_alloc ();
          }

        internal_construct_ (blocks, sizeof(value_type), addr,
                             blocks * sizeof(value_type));
      }

    template<typename T, typename A>
      block_pool_typed_allocated<T, A>::~block_pool_typed_allocated ()
      {
        trace::printf ("%s() @%p %s\n", __func__, this, this->name ());

        // Skip in case a derived class did the deallocation.
        if (allocator_ != nullptr)
          {
            allocator_->deallocate (
                static_cast<typename allocator_traits::pointer> (pool_addr_),
                blocks_);

            // Prevent another deallocation.
            allocator_ = nullptr;
          }
      }

  // --------------------------------------------------------------------------

  } /* namespace memory */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_MEMORY_BLOCK_POOL_H_ */
