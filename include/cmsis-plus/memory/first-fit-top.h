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

#ifndef CMSIS_PLUS_MEMORY_FIRST_FIT_TOP_H_
#define CMSIS_PLUS_MEMORY_FIRST_FIT_TOP_H_

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
     * @brief Memory resource implementing the first fit, top-down
     *  allocation policies, using an existing arena.
     * @ingroup cmsis-plus-rtos-memres
     * @headerfile first-fit-top.h <cmsis-plus/memory/first-fit-top.h>
     *
     * @details
     * This memory manager was inspired by the **newlib nano**
     * implementation of `malloc()` & `free()`.
     *
     * Neither allocation nor deallocation are deterministic, but are
     * reasonably fast.
     */
    class first_fit_top : public rtos::memory::memory_resource
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
      first_fit_top (void* addr, std::size_t bytes);

      /**
       * @brief Construct a named memory resource object instance.
       * @param [in] name Pointer to name.
       * @param [in] addr Begin of allocator arena.
       * @param [in] bytes Size of allocator arena, in bytes.
       */
      first_fit_top (const char* name, void* addr, std::size_t bytes);

    protected:

      /**
       * @brief Default constructor. Construct a memory resource
       *  object instance.
       */
      first_fit_top () = default;

      /**
       * @brief Construct a named memory resource object instance.
       * @param [in] name
       */
      first_fit_top (const char* name);

    public:

      /**
       * @cond ignore
       */

      // The rule of five.
      first_fit_top (const first_fit_top&) = delete;
      first_fit_top (first_fit_top&&) = delete;
      first_fit_top&
      operator= (const first_fit_top&) = delete;
      first_fit_top&
      operator= (first_fit_top&&) = delete;

      /**
       * @endcond
       */

      /**
       * @brief Destruct the memory resource object instance.
       */
      virtual
      ~first_fit_top ();

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Functions
       * @{
       */

      /**
       * @brief Internal function to construct the memory resource.
       * @param [in] addr Begin of allocator arena.
       * @param [in] bytes Size of allocator arena, in bytes.
       * @par Returns
       *  Nothing.
       */
      void
      internal_construct_ (void* addr, std::size_t bytes);

      /**
       * @brief Internal function to reset the memory resource.
       * @par Parameters
       *  None.
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

      typedef struct chunk_s
      {
        // The actual chunk size, in bytes;
        // exactly after it comes the next chunk.
        std::size_t size;
        // When chunk is in the free list, pointer to next chunk, .
        // At this address, or slightly further if alignment was
        // required, starts the payload.
        struct chunk_s* next;
      } chunk_t;

#pragma GCC diagnostic pop

      // Offset of payload inside the chunk.
      static constexpr std::size_t chunk_offset = offsetof(chunk_t, next);
      static constexpr std::size_t chunk_align = sizeof(void*);

      static constexpr std::size_t block_align = alignof(std::max_align_t);
      static constexpr std::size_t block_minsize = sizeof(void *);
      static constexpr std::size_t block_maxsize = 1024 * 1024;
      static constexpr std::size_t block_padding = os::rtos::memory::max (
          block_align, chunk_align) - chunk_align;
      static constexpr std::size_t block_minchunk = chunk_offset + block_padding
          + block_minsize;

      void* arena_addr_ = nullptr;
      // No need for arena_size_bytes_, use total_bytes_.

      chunk_t* free_list_ = nullptr;

      /**
       * @endcond
       */

    };

    // ========================================================================

    /**
     * @brief Memory resource implementing the first fit, top-down
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
      class first_fit_top_inclusive : public first_fit_top
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
        first_fit_top_inclusive (void);

        /**
         * @brief Construct a named memory resource object instance.
         * @param [in] name Pointer to name.
         */
        first_fit_top_inclusive (const char* name);

      public:

        /**
         * @cond ignore
         */

        // The rule of five.
        first_fit_top_inclusive (const first_fit_top_inclusive&) = delete;
        first_fit_top_inclusive (first_fit_top_inclusive&&) = delete;
        first_fit_top_inclusive&
        operator= (const first_fit_top_inclusive&) = delete;
        first_fit_top_inclusive&
        operator= (first_fit_top_inclusive&&) = delete;

        /**
         * @endcond
         */

        /**
         * @brief Destruct the memory resource object instance.
         */
        virtual
        ~first_fit_top_inclusive ();

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
     * @brief Memory resource implementing the first fit, top-down
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
      class first_fit_top_allocated : public first_fit_top
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
        first_fit_top_allocated (std::size_t bytes,
                                 const allocator_type& allocator =
                                     allocator_type ());

        /**
         * @brief Construct a named memory resource object instance.
         * @param [in] name Pointer to name.
         * @param [in] bytes The size of the allocation arena.
         * @param [in] allocator Reference to allocator. Default a
         * local temporary instance.
         */
        first_fit_top_allocated (const char* name, std::size_t bytes,
                                 const allocator_type& allocator =
                                     allocator_type ());

      public:

        /**
         * @cond ignore
         */

        // The rule of five.
        first_fit_top_allocated (const first_fit_top_allocated&) = delete;
        first_fit_top_allocated (first_fit_top_allocated&&) = delete;
        first_fit_top_allocated&
        operator= (const first_fit_top_allocated&) = delete;
        first_fit_top_allocated&
        operator= (first_fit_top_allocated&&) = delete;

        /**
         * @endcond
         */

        /**
         * @brief Destruct the memory resource object instance.
         */
        virtual
        ~first_fit_top_allocated ();

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

  // --------------------------------------------------------------------------
  } /* namespace memory */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace memory
  {

    // ========================================================================

    inline
    first_fit_top::first_fit_top (const char* name) :
        rtos::memory::memory_resource
          { name }
    {
      ;
    }

    inline
    first_fit_top::first_fit_top (void* addr, std::size_t bytes) :
        first_fit_top
          { nullptr, addr, bytes }
    {
      ;
    }

    inline
    first_fit_top::first_fit_top (const char* name, void* addr,
                                  std::size_t bytes) :
        rtos::memory::memory_resource
          { name }
    {
      trace::printf ("%s(%p,%u) @%p %s\n", __func__, addr, bytes, this,
                     this->name ());

      internal_construct_ (addr, bytes);
    }

    // ========================================================================

    template<std::size_t N>
      inline
      first_fit_top_inclusive<N>::first_fit_top_inclusive () :
          first_fit_top_inclusive (nullptr)
      {
        ;
      }

    template<std::size_t N>
      inline
      first_fit_top_inclusive<N>::first_fit_top_inclusive (const char* name) :
          first_fit_top
            { name }
      {
        trace::printf ("%s() @%p %s\n", __func__, this, this->name ());

        internal_construct_ (&arena_[0], bytes);
      }

    template<std::size_t N>
      first_fit_top_inclusive<N>::~first_fit_top_inclusive ()
      {
        trace::printf ("%s() @%p %s\n", __func__, this, this->name ());
      }

    // ========================================================================

    template<typename A>
      inline
      first_fit_top_allocated<A>::first_fit_top_allocated (
          std::size_t bytes, const allocator_type& allocator) :
          first_fit_top_allocated (nullptr, bytes, allocator)
      {
        ;
      }

    template<typename A>
      first_fit_top_allocated<A>::first_fit_top_allocated (
          const char* name, std::size_t bytes, const allocator_type& allocator) :
          first_fit_top
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
      first_fit_top_allocated<A>::~first_fit_top_allocated ()
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

#endif /* CMSIS_PLUS_MEMORY_FIRST_FIT_TOP_H_ */
