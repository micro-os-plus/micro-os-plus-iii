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

#ifndef CMSIS_PLUS_MEMORY_LIFO_H_
#define CMSIS_PLUS_MEMORY_LIFO_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

#include <cmsis-plus/memory/first-fit-top.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace memory
  {

    // ========================================================================

    /**
     * @brief Memory resource implementing the LIFO
     *  allocation/deallocation policies, using an existing arena.
     * @ingroup cmsis-plus-rtos-memres
     * @headerfile lifo.h <cmsis-plus/memory/lifo.h>
     *
     * @details
     * This memory manager is a variant of `first_fit_top` that
     * guarantees a deterministic, fragmentation free, allocation.
     *
     * Deallocation is guaranteed to be deterministic only when
     * deallocating the last allocated block (strict LIFO policy).
     *
     * However this class does not enforce the strict LIFO policy,
     * deallocating older block is allowed, but generally it is
     * no longer deterministic, since a traversal of the free list
     * is required. On the other side, with block always allocated
     * from top to down, and the free list ordered, deallocating
     * older block is predictable if the age of the block
     * is known, in other words deallocating the last allocated block
     * inserts it after the head block, deallocating the second last
     * allocated block inserts it before the second element in the list,
     * and generally deallocating the n-th last allocated block
     * inserts it somewhere up to the n-th element in the free list,
     * possibly sooner, so the behaviour is somehow deterministic.
     *
     * This memory manager is ideal for one-time allocations of
     * objects during startup, objects to be kept alive for the
     * entire life span of the application. It is also ideal
     * for pools of objects, that are later created and destroyed
     * on an as-needed basis.
     */
    class lifo : public os::memory::first_fit_top
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct a memory resource object instance.
       * @param [in] addr Begin of allocator arena.
       * @param [in] bytes Size of allocator arena, in bytes.
       */
      lifo (void* addr, std::size_t bytes);

      /**
       * @brief Construct a named memory resource object instance.
       * @param [in] name Pointer to name.
       * @param [in] addr Begin of allocator arena.
       * @param [in] bytes Size of allocator arena, in bytes.
       */
      lifo (const char* name, void* addr, std::size_t bytes);

    protected:

      /**
       * @brief Default constructor. Construct a memory resource
       *  object instance.
       */
      lifo () = default;

      /**
       * @brief Construct a named memory resource object instance.
       * @param [in] name
       */
      lifo (const char* name);

    public:

      /**
       * @cond ignore
       */

      // The rule of five.
      lifo (const lifo&) = delete;
      lifo (lifo&&) = delete;
      lifo&
      operator= (const lifo&) = delete;
      lifo&
      operator= (lifo&&) = delete;

      /**
       * @endcond
       */

      /**
       * @brief Destruct the memory resource object instance.
       */
      virtual
      ~lifo ();

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Functions
       * @{
       */

      /**
       * @brief Implementation of the memory allocator.
       * @param [in] bytes Number of bytes to allocate.
       * @param [in] alignment Alignment constraint (power of 2).
       * @return Pointer to newly allocated block, or `nullptr`.
       */
      virtual void*
      do_allocate (std::size_t bytes, std::size_t alignment) override;

      /**
       * @}
       */

    };

    // ========================================================================

    /**
     * @brief Memory resource implementing the LIFO
     *  allocation policies, using an internal arena.
     * @ingroup cmsis-plus-rtos-memres
     * @headerfile first-fit-top.h <cmsis-plus/memory/first-fit-top.h>
     *
     * @details
     * This class template is a convenience class that includes
     * an array of chars to be used as the allocation arena.
     *
     * The common use case it to define statically allocated memory managers.
     */
    template<std::size_t N>
      class lifo_inclusive : public lifo
      {
      public:

        /**
         * @brief Local constant based on template definition.
         */
        static const std::size_t bytes = N;

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a memory resource object instance.
         * @par Parameters
         *  None.
         */
        lifo_inclusive (void);

        /**
         * @brief Construct a named memory resource object instance.
         * @param [in] name Pointer to name.
         */
        lifo_inclusive (const char* name);

      public:

        /**
         * @cond ignore
         */

        // The rule of five.
        lifo_inclusive (const lifo_inclusive&) = delete;
        lifo_inclusive (lifo_inclusive&&) = delete;
        lifo_inclusive&
        operator= (const lifo_inclusive&) = delete;
        lifo_inclusive&
        operator= (lifo_inclusive&&) = delete;

        /**
         * @endcond
         */

        /**
         * @brief Destruct the memory resource object instance.
         */
        virtual
        ~lifo_inclusive ();

        /**
         * @}
         */

      protected:

        /**
         * @cond ignore
         */

        /**
         * @brief The allocation arena is an array of bytes.
         */
        char arena_[bytes];

        /**
         * @endcond
         */

      };

    // ========================================================================

    /**
     * @brief Memory resource implementing the LIFO
     *  allocation policies, using a dynamically allocated arena.
     * @ingroup cmsis-plus-rtos-memres
     * @headerfile first-fit-top.h <cmsis-plus/memory/first-fit-top.h>
     *
     * @details
     * This class template is a convenience class that allocates
     * an array of chars to be used as the allocation arena.
     *
     * The common use case it to define dynamically allocated memory managers.
     */
    template<typename A = os::rtos::memory::allocator<char>>
      class lifo_allocated : public lifo
      {
      public:

        /**
         * @brief Standard allocator type definition.
         */
        using value_type = char;

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
         * @param [in] bytes The size of the allocation arena.
         * @param [in] allocator Reference to allocator. Default a
         * local temporary instance.
         */
        lifo_allocated (std::size_t bytes, const allocator_type& allocator =
                            allocator_type ());

        /**
         * @brief Construct a named memory resource object instance.
         * @param [in] name Pointer to name.
         * @param [in] bytes The size of the allocation arena.
         * @param [in] allocator Reference to allocator. Default a
         * local temporary instance.
         */
        lifo_allocated (const char* name, std::size_t bytes,
                        const allocator_type& allocator = allocator_type ());

      public:

        /**
         * @cond ignore
         */

        // The rule of five.
        lifo_allocated (const lifo_allocated&) = delete;
        lifo_allocated (lifo_allocated&&) = delete;
        lifo_allocated&
        operator= (const lifo_allocated&) = delete;
        lifo_allocated&
        operator= (lifo_allocated&&) = delete;

        /**
         * @endcond
         */

        /**
         * @brief Destruct the memory resource object instance.
         */
        virtual
        ~lifo_allocated ();

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
    lifo::lifo (void* addr, std::size_t bytes) :
        first_fit_top
          { nullptr, addr, bytes }
    {
      trace::printf ("%s(%p,%u) @%p %s\n", __func__, addr, bytes, this,
                     this->name ());
    }

    inline
    lifo::lifo (const char* name, void* addr, std::size_t bytes) :
        first_fit_top
          { name, addr, bytes }
    {
      trace::printf ("%s(%p,%u) @%p %s\n", __func__, addr, bytes, this,
                     this->name ());
    }

    // ========================================================================

    template<std::size_t N>
      inline
      lifo_inclusive<N>::lifo_inclusive () :
          lifo_inclusive (nullptr)
      {
        ;
      }

    template<std::size_t N>
      inline
      lifo_inclusive<N>::lifo_inclusive (const char* name) :
          lifo
            { name }
      {
        trace::printf ("%s() @%p %s\n", __func__, this, this->name ());

        internal_construct_ (&arena_[0], bytes);
      }

    template<std::size_t N>
      lifo_inclusive<N>::~lifo_inclusive ()
      {
        trace::printf ("%s() @%p %s\n", __func__, this, this->name ());
      }

    // ========================================================================

    template<typename A>
      inline
      lifo_allocated<A>::lifo_allocated (std::size_t bytes,
                                         const allocator_type& allocator) :
          lifo_allocated (nullptr, bytes, allocator)
      {
        ;
      }

    template<typename A>
      lifo_allocated<A>::lifo_allocated (const char* name, std::size_t bytes,
                                         const allocator_type& allocator) :
          lifo
            { name }
      {
        trace::printf ("%s(%u) @%p %s\n", __func__, bytes, this, this->name ());

        // Remember the allocator, it'll be used by the destructor.
        allocator_ =
            static_cast<allocator_type*> (&const_cast<allocator_type&> (allocator));

        void* addr = allocator_->allocate (bytes);
        if (addr == nullptr)
          {
            estd::__throw_bad_alloc ();
          }

        internal_construct_ (addr, bytes);
      }

    template<typename A>
      lifo_allocated<A>::~lifo_allocated ()
      {
        trace::printf ("%s() @%p %s\n", __func__, this, this->name ());

        // Skip in case a derived class did the deallocation.
        if (allocator_ != nullptr)
          {
            allocator_->deallocate (
                static_cast<typename allocator_traits::pointer> (arena_addr_),
                total_bytes_);

            // Prevent another deallocation.
            allocator_ = nullptr;
          }
      }

  // --------------------------------------------------------------------------

  } /* namespace memory */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_MEMORY_LIFO_H_ */
