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

#ifndef CMSIS_PLUS_RTOS_OS_MEMORY_H_
#define CMSIS_PLUS_RTOS_OS_MEMORY_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

#include <cmsis-plus/estd/system_error>

#include <limits>
#include <new>
#include <cerrno>

// ----------------------------------------------------------------------------

// These definitions refer only to the RTOS allocators.
// The application should use the similar ones from the
// os::estd:: namespace.

namespace os
{
  namespace rtos
  {
    namespace scheduler
    {
      class critical_section;
    }

    class null_locker;

    namespace memory
    {
      // ----------------------------------------------------------------------

      constexpr std::size_t
      max (std::size_t a, std::size_t b)
      {
        return a >= b ? a : b;
      }

      class memory_resource;

      // ----------------------------------------------------------------------

      /**
       * @addtogroup cmsis-plus-rtos-memres
       * @{
       */

      /**
       * @name RTOS System Memory Functions
       * @{
       */

      /**
       * @brief Get the address of a memory manager based on POSIX `malloc()`.
       * @par Parameters
       *  None.
       * @return Pointer to a memory manager object instance.
       */
      memory_resource*
      malloc_resource (void) noexcept;

      /**
       * @brief Set the default RTOS system memory manager.
       * @param res Pointer to new memory manager object instance.
       * @return Pointer to previous memory manager object instance.
       */
      memory_resource*
      set_default_resource (memory_resource* res) noexcept;

      /**
       * @brief Get the default RTOS system memory manager.
       * @par Parameters
       *  None.
       * @return Pointer to a memory manager object instance.
       */
      memory_resource*
      get_default_resource (void) noexcept;

      /**
       * @}
       */

      // ======================================================================
      /**
       * @brief Type of out of memory handler.
       */
      using out_of_memory_handler_t = void (*)(void);

      /**
       * @brief Memory resource manager.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @details
       * This class is based on the standard memory manager, with
       * several extensions, to control the throw behaviour and to
       * add statistics.
       */
      class memory_resource
      {

      public:

        /**
         * @brief The largest alignment for the platform. Also default
         * when supplied alignment is not supported.
         */
        static constexpr std::size_t max_align = alignof(std::max_align_t);

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Destruct the memory resource object instance.
         */

        virtual
        ~memory_resource ();

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
         * @param bytes Number of bytes to allocate.
         * @param alignment Alignment constraint (power of 2).
         * @return Pointer to newly allocated block, or `nullptr`.
         */
        void*
        allocate (std::size_t bytes, std::size_t alignment = max_align);

        /**
         * @brief Deallocate the previously allocated memory block.
         * @param addr Address of the block to free.
         * @param bytes Number of bytes to deallocate (may be 0 if unknown).
         * @param alignment Alignment constraint (power of 2).
         * @par Returns
         *  Nothing.
         */
        void
        deallocate (void* addr, std::size_t bytes, std::size_t alignment =
                        max_align) noexcept;

        /**
         * @brief Compare for equality with another `memory_resource`.
         * @param other Reference to another `memory_resource`.
         * @retval true The `memory_resource` objects are equal.
         * @retval false The `memory_resource` objects are not equal.
         */
        bool
        is_equal (memory_resource const & other) const noexcept;

        /**
         * @brief Reset the memory manager to the initial state.
         * @par Parameters
         *  None.
         * @par Returns
         *  Nothing.
         */
        void
        reset (void) noexcept;

        /**
         * @brief Coalesce free blocks.
         * @par Parameters
         *  None.
         * @retval true if the operation freed more memory.
         * @retval false if the operation was ineffective.
         */
        bool
        coalesce (void) noexcept;

        /**
         * @brief Get the largest value that can be passed to `allocate()`.
         * @par Parameters
         *  None.
         * @return Number of bytes or 0 if unknown.
         */
        std::size_t
        max_size (void) const noexcept;

        /**
         * @brief Set the out of memory handler.
         * @param handler Pointer to new handler.
         * @return Pointer to old handler.
         */
        out_of_memory_handler_t
        out_of_memory_handler (out_of_memory_handler_t handler);

        /**
         * @brief Get the out of memory handler.
         * @par Parameters
         *  None.
         * @return Pointer to existing handler.
         */
        out_of_memory_handler_t
        out_of_memory_handler (void);

        /**
         * @brief Get the total size of managed memory.
         * @return Number of bytes.
         */
        std::size_t
        total_bytes (void);

        /**
         * @brief Get the current size of all allocated chunks.
         * @par Parameters
         *  None.
         * @return Number of bytes.
         */
        std::size_t
        allocated_bytes (void);

        /**
         * @brief Get the maximum allocated size.
         * @par Parameters
         *  None.
         * @return Number of bytes.
         */
        std::size_t
        max_allocated_bytes (void);

        /**
         * @brief Get the current size of all free chunks.
         * @par Parameters
         *  None.
         * @return Number of bytes.
         */
        std::size_t
        free_bytes (void);

        /**
         * @brief Get the current number of allocated chunks.
         * @par Parameters
         *  None.
         * @return Number of chunks.
         */
        std::size_t
        allocated_chunks (void);

        /**
         * @brief Get the current number of free chunks.
         * @par Parameters
         *  None.
         * @return Number of chunks.
         */
        std::size_t
        free_chunks (void);

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
         * @param bytes Number of bytes to allocate.
         * @param alignment Alignment constraint (power of 2).
         * @return Pointer to newly allocated block, or `nullptr`.
         */
        virtual void*
        do_allocate (std::size_t bytes, std::size_t alignment) = 0;

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
            noexcept = 0;

        /**
         * @brief Implementation of the equality comparator.
         * @param other Reference to another `memory_resource`.
         * @retval true The `memory_resource` objects are equal.
         * @retval false The `memory_resource` objects are not equal.
         */
        virtual bool
        do_is_equal (memory_resource const &other) const noexcept;

        /**
         * @brief Implementation of the function to get max size.
         * @par Parameters
         *  None.
         * @return Integer with size in bytes, or 0 if unknown.
         */
        virtual std::size_t
        do_max_size (void) const noexcept;

        /**
         * @brief Implementation of the function to reset the memory manager.
         * @par Parameters
         *  None.
         * @par Returns
         *  Nothing.
         */
        virtual void
        do_reset (void) noexcept;

        /**
         * @brief Implementation of the function to coalesce free blocks.
         * @par Parameters
         *  None.
         * @retval true if the operation resulted in larger blocks.
         * @retval false if the operation was ineffective.
         */
        virtual bool
        do_coalesce (void) noexcept;

        /**
         * @brief Helper function to align size values.
         * @param size Unaligned size.
         * @param align Alignment requirement (power of 2).
         * @return Aligned size.
         */
        static constexpr std::size_t
        align (std::size_t size, std::size_t align) noexcept;

        /**
         * @}
         */

      protected:

        /**
         * @cond ignore
         */

        out_of_memory_handler_t out_of_memory_handler_ = nullptr;

        std::size_t total_bytes_ = 0;
        std::size_t allocated_bytes_ = 0;
        std::size_t free_bytes_ = 0;
        std::size_t allocated_chunks_ = 0;
        std::size_t free_chunks_ = 0;
        std::size_t max_allocated_bytes_ = 0;

        /**
         * @endcond
         */

      };

      /**
       * @name Operators
       * @{
       */

      /**
       * @brief Compare the `memory_resource` instances for equality.
       * @param lhs First instance to compare.
       * @param rhs Second instance to compare.
       * @retval true The two object `memory_resource` instances are equal.
       * @retval false The two object `memory_resource` instances are not equal.
       */
      bool
      operator== (const memory_resource& lhs, const memory_resource& rhs)
          noexcept;

      /**
       * @brief Compare the `memory_resource` instances for inequality.
       * @param lhs First instance to compare.
       * @param rhs Second instance to compare.
       * @retval true The two object `memory_resource` instances are not equal.
       * @retval false The two object `memory_resource` instances are equal.
       */
      bool
      operator!= (const memory_resource& lhs, const memory_resource& rhs)
          noexcept;

      /**
       * @}
       */

      // ======================================================================
      /**
       * @brief Standard allocator based on the RTOS system default memory
       * manager.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @details
       * This class template is used as the default allocator for
       * system classes. It gets memory from the system default memory
       * manager `os::rtos::memory::get_default_resource()`.
       *
       * @note As default allocator, this class must be stateless,
       *  i.e. have no member variables.
       */
      template<typename T>
        class default_resource_allocator
        {
        public:

          /**
           * @brief Type of elements to be allocated.
           */
          using value_type = T;

          /**
           * @name Constructors & Destructor
           * @{
           */

          /**
           * @brief Default constructor. Construct a default resource
           * allocator object instance.
           */
          default_resource_allocator () noexcept = default;

          /**
           * @brief Copy constructor.
           * @param a Reference to existing allocator.
           */
          default_resource_allocator (default_resource_allocator const & a) = default;

          /**
           * @brief Copy constructor template.
           * @param other
           */
          template<typename U>
            default_resource_allocator (
                default_resource_allocator<U> const & other) noexcept;

          /**
           * @brief Move constructor.
           * @param a Reference to existing allocator.
           */
          default_resource_allocator (default_resource_allocator && a) = default;

          /**
           * @brief Copy assignment operator.
           * @param a Reference to existing allocator.
           * @return
           */
          default_resource_allocator&
          operator= (default_resource_allocator const & a) = default;

          /**
           * @brief Move assignment operator.
           * @param a Reference to existing allocator.
           * @return
           */
          default_resource_allocator&
          operator= (default_resource_allocator && a) = default;

          /**
           * @brief Destruct the default resource allocator object instance.
           */
          ~default_resource_allocator () = default;

          /**
           * @}
           */

        public:

          /**
           * @name Public Member Functions
           * @{
           */

          /**
           * @brief Allocate a number of memory blocks of type `value_type`.
           * @param elements Number of elements of type `value_type`.
           * @return Pointer to newly allocated memory blocks.
           */
          value_type*
          allocate (std::size_t elements);

          /**
           * @brief Deallocate the number of memory blocks of type `value_type`.
           * @param addr Pointer to previously allocated memory blocks.
           * @param elements Number of elements of type `value_type`.
           * @par Returns
           *  Nothing
           */
          void
          deallocate (value_type* addr, std::size_t elements) noexcept;

          /**
           * @brief The number of elements that can be passed to `allocate()`.
           * @return Number of elements of type `value_type`.
           */
          std::size_t
          max_size (void) const noexcept;

          /**
           * @}
           */
        };

      /**
       * @}
       */

      // ======================================================================
      /**
       * @cond ignore
       */

      template<typename L>
        class lock_guard;

      using D_T = memory_resource* (void);

      // Experimental, to be finalised.
      template<typename T, typename L = null_locker, D_T D =
          get_default_resource>
        class polymorphic_synchronized_allocator
        {
        public:

          using value_type = T;
          using locker_type = L;

          polymorphic_synchronized_allocator () noexcept;

          polymorphic_synchronized_allocator (memory_resource* r) noexcept;

          polymorphic_synchronized_allocator (
              polymorphic_synchronized_allocator const & a) = default;

          template<typename U>
            polymorphic_synchronized_allocator (
                polymorphic_synchronized_allocator<U, locker_type> const & other)
                    noexcept;

          polymorphic_synchronized_allocator&
          operator= (polymorphic_synchronized_allocator const & a) = default;

          value_type*
          allocate (std::size_t elements);

          void
          deallocate (value_type* p, std::size_t elements) noexcept;

          std::size_t
          max_size (void) const noexcept;

          polymorphic_synchronized_allocator
          select_on_container_copy_construction (void) const noexcept;

          memory_resource*
          resource (void) const noexcept;

        private:

          memory_resource* res_;
        };

      template<typename T1, typename T2, typename L, D_T D>
        bool
        operator== (const polymorphic_synchronized_allocator<T1, L, D>& lhs,
                    const polymorphic_synchronized_allocator<T2, L, D>& rhs)
                        noexcept;

      template<typename T1, typename T2, typename L, D_T D>
        bool
        operator!= (const polymorphic_synchronized_allocator<T1, L, D>& lhs,
                    const polymorphic_synchronized_allocator<T2, L, D>& rhs)
                        noexcept;

    /**
     * @endcond
     */

    // ------------------------------------------------------------------------
    } /* namespace memory */
  } /* namespace rtos */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace rtos
  {
    namespace memory
    {
      // ----------------------------------------------------------------------

      extern memory_resource* default_resource;

      // ----------------------------------------------------------------------

      /**
       * @details
       * If not set explicitly by the user, this function
       * will return an instance of `malloc_memory_resource`
       * on bare metal platforms and of
       * `malloc_memory_resource` on POSIX platforms.
       */
      inline memory_resource*
      get_default_resource (void) noexcept
      {
        return default_resource;
      }

      // ======================================================================

      /**
       * @details
       * Allocate storage with a size of at least `bytes` bytes. The
       * returned storage is aligned to the specified alignment if
       * such alignment is supported, and to `alignof(std::max_align_t)`
       * otherwise.
       *
       * If the storage of the requested size and alignment cannot be
       * obtained:
       * - if the out of memory handler is not set, return `nullptr`;
       * - if the out of memory handler is set, call it and retry.
       *
       * Equivalent to `return do_allocate(bytes, alignment);`.
       *
       * @par Exceptions
       *   The code itself throws nothing, but if the out of memory
       *   handler is set, it may throw a `bad_alloc()` exception.
       *
       * @see do_allocate();
       */
      inline void*
      memory_resource::allocate (std::size_t bytes, std::size_t alignment)
      {
        return do_allocate (bytes, alignment);
      }

      /**
       * @details
       * Deallocate the storage pointed to by `addr`.
       * The address shall have been returned
       * by a prior call to `allocate()` on a memory_resource
       * that compares equal to *this, and the storage it points to shall
       * not yet have been deallocated.
       *
       * Equivalent to `return do_deallocate(p, bytes, alignment);`.
       *
       * @par Exceptions
       *   Throws nothing.
       *
       * @see do_deallocate();
       */
      inline void
      memory_resource::deallocate (void* addr, std::size_t bytes,
                                   std::size_t alignment) noexcept
      {
        do_deallocate (addr, bytes, alignment);
      }

      /**
       * @details
       * Compare `*this` for equality with other. Two `memory_resources`
       * compare equal if and only if memory allocated from one
       * `memory_resource` can be deallocated from the other and vice versa.
       *
       * @par Exceptions
       *   Throws nothing.
       *
       * @see do_is_equal();
       */
      inline bool
      memory_resource::is_equal (memory_resource const & other) const noexcept
      {
        return do_is_equal (other);
      }

      /**
       * @details
       *
       * @see do_max_size();
       */
      inline std::size_t
      memory_resource::max_size (void) const noexcept
      {
        return do_max_size ();
      }

      /**
       * @details
       *
       * @see do_reset();
       */
      inline void
      memory_resource::reset (void) noexcept
      {
        do_reset ();
      }

      /**
       * @details
       * In case the memory manager does not coalesce during deallocation,
       * traverse the list of free blocks and coalesce.
       *
       * Return `true` if the operation was successful and at least
       * one larger block resulted.
       *
       * @see do_coalesce();
       */
      inline bool
      memory_resource::coalesce (void) noexcept
      {
        return do_coalesce ();
      }

      /**
       * @details
       *
       * @par Standard compliance
       *   Extension to standard.
       */
      inline constexpr std::size_t
      memory_resource::align (std::size_t size, std::size_t align) noexcept
      {
        return ((size) + (align) - 1L) & ~((align) - 1L);
      }

      /**
       * @details
       *
       * @par Standard compliance
       *   Extension to standard.
       */
      inline out_of_memory_handler_t
      memory_resource::out_of_memory_handler (out_of_memory_handler_t handler)
      {
        trace::printf ("%s(%p) @%p\n", __func__, handler, this);

        out_of_memory_handler_t tmp = out_of_memory_handler_;
        out_of_memory_handler_ = handler;

        return tmp;
      }

      /**
       * @details
       *
       * @par Standard compliance
       *   Extension to standard.
       */
      inline out_of_memory_handler_t
      memory_resource::out_of_memory_handler (void)
      {
        return out_of_memory_handler_;
      }

      inline std::size_t
      memory_resource::total_bytes (void)
      {
        return total_bytes_;
      }

      inline std::size_t
      memory_resource::allocated_bytes (void)
      {
        return allocated_bytes_;
      }

      inline std::size_t
      memory_resource::max_allocated_bytes (void)
      {
        return max_allocated_bytes_;
      }

      inline std::size_t
      memory_resource::free_bytes (void)
      {
        return free_bytes_;
      }

      inline std::size_t
      memory_resource::allocated_chunks (void)
      {
        return allocated_chunks_;
      }

      inline std::size_t
      memory_resource::free_chunks (void)
      {
        return free_chunks_;
      }

      // ======================================================================

      inline bool
      operator== (memory_resource const & lhs, memory_resource const & rhs) noexcept
      {
        return &lhs == &rhs || lhs.is_equal (rhs);
      }

      inline bool
      operator!= (memory_resource const & lhs, memory_resource const & rhs) noexcept
      {
        return !(lhs == rhs);
      }

      // ======================================================================

      template<typename T>
        template<typename U>
          inline
          default_resource_allocator<T>::default_resource_allocator (
              default_resource_allocator<U> const & other __attribute__((unused))) noexcept
          {
            ;
          }

      template<typename T>
        inline typename default_resource_allocator<T>::value_type*
        default_resource_allocator<T>::allocate (std::size_t elements)
        {
          scheduler::critical_section scs;

          return static_cast<value_type*> (get_default_resource ()->allocate (
              elements * sizeof(value_type)));
        }

      template<typename T>
        inline void
        default_resource_allocator<T>::deallocate (value_type* addr,
                                                   std::size_t elements) noexcept
        {
          scheduler::critical_section scs;

          get_default_resource ()->deallocate (addr,
                                               elements * sizeof(value_type));
        }

      template<typename T>
        inline std::size_t
        default_resource_allocator<T>::max_size (void) const noexcept
        {
          return get_default_resource ()->max_size () / sizeof(value_type);
        }

      // ======================================================================

      /**
       * @cond ignore
       */

      template<typename T, typename U, typename L>
        inline bool
        operator== (polymorphic_synchronized_allocator<T, L> const & lhs,
                    polymorphic_synchronized_allocator<U, L> const & rhs) noexcept
        {
          return *lhs.resource () == *rhs.resource ();
        }

      template<typename T, typename U, typename L>
        inline bool
        operator!= (polymorphic_synchronized_allocator<T, L> const & lhs,
                    polymorphic_synchronized_allocator<U, L> const & rhs) noexcept
        {
          return !(lhs == rhs);
        }

      // ======================================================================

      template<typename T, typename L, D_T D>
        inline
        polymorphic_synchronized_allocator<T, L, D>::polymorphic_synchronized_allocator () noexcept :
        res_(D())
          {
            trace::printf ("%s() @%p %p\n", __func__, this, res_);
          }

      template<typename T, typename L, D_T D>
        inline
        polymorphic_synchronized_allocator<T, L, D>::polymorphic_synchronized_allocator (
            memory_resource* r) noexcept :
        res_(r)
          {
            trace::printf ("%s(%p) @%p\n", __func__, r, this);
          }

      template<typename T, typename L, D_T D>
        template<typename U>
          inline
          polymorphic_synchronized_allocator<T, L, D>::polymorphic_synchronized_allocator (
              polymorphic_synchronized_allocator<U, locker_type> const & other) noexcept :
          res_(other.resource())
            {
              ;
            }

      template<typename T, typename L, D_T D>
        typename polymorphic_synchronized_allocator<T, L, D>::value_type*
        polymorphic_synchronized_allocator<T, L, D>::allocate (
            std::size_t elements)
        {
          trace::printf ("%s(%u) @%p\n", __func__, elements, this);
          if (elements > max_size ())
            {
              estd::__throw_system_error (
                  EINVAL,
                  "polymorphic_synchronized_allocator<T>::allocate(size_t n)"
                  " 'n' exceeds maximum supported size");
            }

          locker_type lk;
          lock_guard<locker_type> ulk
            { lk };

          return static_cast<value_type*> (res_->allocate (
              elements * sizeof(value_type), alignof(value_type)));
        }

      template<typename T, typename L, D_T D>
        void
        polymorphic_synchronized_allocator<T, L, D>::deallocate (
            value_type * addr, std::size_t elements) noexcept
        {
          assert (elements <= max_size ());
          trace::printf ("%s(%p,%u) @%p\n", __func__, addr, elements, this);

          locker_type lk;
          lock_guard<locker_type> ulk
            { lk };

          res_->deallocate (addr, elements * sizeof(value_type),
                            alignof(value_type));
        }

      template<typename T, typename L, D_T D>
        inline std::size_t
        polymorphic_synchronized_allocator<T, L, D>::max_size (void) const noexcept
        {
          return res_->max_size () / sizeof(T);
        }

      template<typename T, typename L, D_T D>
        inline polymorphic_synchronized_allocator<T, L, D>
        polymorphic_synchronized_allocator<T, L, D>::select_on_container_copy_construction (
            void) const noexcept
        {
          return polymorphic_synchronized_allocator ();
        }

      template<typename T, typename L, D_T D>
        inline memory_resource*
        polymorphic_synchronized_allocator<T, L, D>::resource (void) const noexcept
        {
          return res_;
        }

    /**
     * @endcond
     */

    // ------------------------------------------------------------------------
    } /* namespace memory */
  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_MEMORY_H_ */
