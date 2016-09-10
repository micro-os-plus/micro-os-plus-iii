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
    template<typename T>
      class allocator;

    // ========================================================================

    /**
     * @brief Memory resource managing a pool of same size blocks.
     * @ingroup cmsis-plus-rtos-memres
     * @headerfile block-pool.h <cmsis-plus/memory/block-pool.h>
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

      /**
       * @cond ignore
       */

    protected:

      /**
       * @brief Default constructor. Construct a memory resource
       *  object instance.
       */
      block_pool (const char* name);

    public:

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
       *
       */
      void
      internal_reset_ (void) noexcept;

      /**
       * @brief Implementation of the memory allocator.
       * @param bytes Number of bytes to allocate.
       * @param alignment Alignment constraint (power of 2).
       * @return Pointer to newly allocated block, or `nullptr`.
       */
      virtual void*
      do_allocate (std::size_t bytes, std::size_t alignment) override;

      /**
       * @brief Implementation of the memory deallocator.
       * @param addr Address of a previously allocated block to free.
       * @param bytes Number of bytes to deallocate (may be 0 if unknown).
       * @param alignment Alignment constraint (power of 2).
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
       * @brief Calculator for pool storage requirements.
       * @param blocks Number of blocks.
       * @param block_size_bytes Size of block.
       * @return Total required storage in bytes, including internal alignment.
       */
      template<typename T>
        constexpr std::size_t
        compute_allocated_size_bytes (std::size_t blocks,
                                      std::size_t block_size_bytes)
        {
          // Align each block
          return (blocks
              * ((block_size_bytes + (sizeof(T) - 1)) & ~(sizeof(T) - 1)));
        }

      /**
       * @cond ignore
       */

      /**
       * @brief The static address where the pool is stored
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
     * @brief Memory resource managing a pool of same size blocks of type T.
     * @ingroup cmsis-plus-rtos-memres
     * @headerfile block-pool.h <cmsis-plus/memory/block-pool.h>
     */
    template<typename T, typename A = os::rtos::memory::allocator<T>>
      class block_pool_typed : public block_pool
      {
      public:

        /**
         * @brief Standard allocator type definition.
         */
        using value_type = T;

        /**
         * @brief Standard allocator type definition.
         */
        using allocator_type = A;

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
        block_pool_typed (std::size_t blocks, const allocator_type& allocator =
                              allocator_type ());

        /**
         * @brief Construct a named memory resource object instance.
         * @param name Pointer to name.
         * @param [in] blocks The maximum number of items in the pool.
         * @param [in] allocator Reference to allocator. Default a
         * local temporary instance.
         */
        block_pool_typed (const char* name, std::size_t blocks,
                          const allocator_type& allocator = allocator_type ());

        /**
         * @cond ignore
         */

      public:

        // The rule of five.
        block_pool_typed (const block_pool_typed&) = delete;
        block_pool_typed (block_pool_typed&&) = delete;
        block_pool_typed&
        operator= (const block_pool_typed&) = delete;
        block_pool_typed&
        operator= (block_pool_typed&&) = delete;

        /**
         * @endcond
         */

        /**
         * @brief Destruct the memory resource object instance.
         */
        virtual
        ~block_pool_typed ();

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
         * A non-null allocator requires deallocation during destruction.
         */
        allocator_type* allocator_ = nullptr;

        /**
         * @brief Total size of the dynamically allocated storage,
         *  in allocation elements.
         */
        std::size_t allocated_elements_ = 0;

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

    // ========================================================================

    template<typename T, typename A>
      inline
      block_pool_typed<T, A>::block_pool_typed (std::size_t blocks,
                                                const allocator_type& allocator) :
          block_pool_typed (nullptr, blocks, allocator)
      {
        ;
      }

    template<typename T, typename A>
      block_pool_typed<T, A>::block_pool_typed (const char* name,
                                                std::size_t blocks,
                                                const allocator_type& allocator) :
          block_pool
            { name }
      {
        trace::printf ("%s(%u,%p) @%p %s\n", __func__, blocks, &allocator, this,
                       this->name ());

        // A non-null allocator will require deallocation during destruction.
        allocator_ =
            static_cast<allocator_type*> (&const_cast<allocator_type&> (allocator));

        // Allocate pool dynamically via the allocator.
        allocated_elements_ = (compute_allocated_size_bytes<
            typename allocator_type::value_type> (blocks, sizeof(value_type))
            + sizeof(typename allocator_type::value_type) - 1)
            / sizeof(typename allocator_type::value_type);

        blocks_ = blocks;
        block_size_bytes_ = sizeof(value_type);

        pool_addr_ = allocator_->allocate (allocated_elements_);

        internal_reset_ ();
      }

    template<typename T, typename A>
      block_pool_typed<T, A>::~block_pool_typed ()
      {
        trace::printf ("%s() @%p %s\n", __func__, this, this->name ());

        typedef typename std::allocator_traits<allocator_type>::pointer pointer;

        if (allocator_ != nullptr)
          {
            allocator_->deallocate (static_cast<pointer> (pool_addr_),
                                    allocated_elements_);

            // After deallocation, clear the pointer to prevent a new
            // deallocation in parent destructor.
            allocator_ = nullptr;
          }
      }

  // --------------------------------------------------------------------------

  } /* namespace memory */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_MEMORY_BLOCK_POOL_H_ */
