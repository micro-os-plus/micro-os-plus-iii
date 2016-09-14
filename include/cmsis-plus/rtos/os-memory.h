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
  namespace estd
  {
    [[noreturn]] void
    __throw_bad_alloc (void);

    template<typename L>
      class lock_guard;

  }

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

      /**
       * @brief Helper function to align size values.
       * @param size Unaligned size.
       * @param align Alignment requirement (power of 2).
       * @return Aligned size.
       */
      constexpr std::size_t
      align_size (std::size_t size, std::size_t align) noexcept
      {
        return ((size) + (align) - 1L) & ~((align) - 1L);
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
       * @brief Memory resource manager (abstract class).
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       *
       * @details
       * This class is based on the standard C++17 memory manager, with
       * several extensions, to control the throw behaviour and to
       * add statistics.
       */
      class memory_resource : public internal::object_named
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
         * @brief Default constructor. Construct a memory resource
         *  object instance.
         */
        memory_resource () = default;

        /**
         * @brief Construct a named memory resource object instance.
         * @param name Pointer to name.
         */
        memory_resource (const char* name);

        /**
         * @cond ignore
         */

        // The rule of five.
        memory_resource (const memory_resource&) = delete;
        memory_resource (memory_resource&&) = delete;
        memory_resource&
        operator= (const memory_resource&) = delete;
        memory_resource&
        operator= (memory_resource&&) = delete;

        /**
         * @endcond
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
         * @brief Get the number of allocations.
         * @par Parameters
         *  None.
         * @return Number of allocations.
         */
        std::size_t
        allocations (void);

        /**
         * @brief Get the number of deallocations.
         * @par Parameters
         *  None.
         * @return Number of deallocations
         */
        std::size_t
        deallocations (void);

        /**
         * @brief Print a long message with usage statistics.
         * @par Parameters
         *  None.
         * @par Returns
         *  Nothing.
         */
        void
        trace_print_statistics (void);

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
         * @brief Update statistics after allocation.
         * @param [in] bytes Number of allocated bytes.
         * @par Returns
         *  Nothing.
         */
        void
        internal_increase_allocated_statistics (std::size_t bytes) noexcept;

        /**
         * @brief Update statistics after deallocation.
         * @param [in] bytes Number of deallocated bytes.
         * @par Returns
         *  Nothing.
         */
        void
        internal_decrease_allocated_statistics (std::size_t bytes) noexcept;

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
        std::size_t allocations_ = 0;
        std::size_t deallocations_ = 0;

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
       * @tparam T Type of elements to be allocated.
       *
       * @details
       * This class template is used as the default allocator for
       * system classes. It gets memory from the system default memory
       * manager `os::rtos::memory::get_default_resource()`.
       *
       * @note As default allocator, this class must be stateless,
       *  i.e. have no member variables.
       */
      template<typename T>
        class allocator_stateless_default_resource
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
          allocator_stateless_default_resource () noexcept = default;

          /**
           * @brief Copy constructor.
           * @param other Reference to existing allocator.
           */
          allocator_stateless_default_resource (
              allocator_stateless_default_resource const & other) = default;

          /**
           * @brief Copy constructor template.
           * @param other Reference to allocator.
           */
          template<typename U>
            allocator_stateless_default_resource (
                allocator_stateless_default_resource<U> const & other) noexcept;

          /**
           * @brief Move constructor.
           * @param other Reference to existing allocator.
           */
          allocator_stateless_default_resource (
              allocator_stateless_default_resource && other) = default;

          /**
           * @brief Destruct the default resource allocator object instance.
           */
          ~allocator_stateless_default_resource () = default;

          /**
           * @}
           */

          /**
           * @name Operators
           * @{
           */

          /**
           * @brief Copy assignment operator.
           * @param other Reference to existing allocator.
           * @return Reference to allocator.
           */
          allocator_stateless_default_resource&
          operator= (allocator_stateless_default_resource const & other) = default;

          /**
           * @brief Move assignment operator.
           * @param other Reference to existing allocator.
           * @return Reference to allocator.
           */
          allocator_stateless_default_resource&
          operator= (allocator_stateless_default_resource && other) = default;

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
           *  Nothing.
           */
          void
          deallocate (value_type* addr, std::size_t elements) noexcept;

          /**
           * @brief The maximum number of elements that can be
           *  passed to `allocate()`.
           * @return Number of elements of type `value_type`.
           */
          std::size_t
          max_size (void) const noexcept;

          /**
           * @}
           */

        protected:

          // This class should have no member variables, to meet the
          // default allocator stateless requirements.
        };

      // ======================================================================
      /**
       * @cond ignore
       */

      template<typename L>
        class lock_guard;

      using F = memory_resource* (void);

      /**
       * @endcond
       */

      /**
       * @brief Allocator using memory resources.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @tparam T Type of elements to be allocated.
       * @tparam L Type of lockable object.
       * @tparam get_resource Function to get the default resource.
       */
      template<typename T, typename L, F get_resource>
        class allocator_stateless_polymorphic_synchronized
        {
        public:

          using value_type = T;
          using locker_type = L;

          /**
           * @name Constructors & Destructor
           * @{
           */

          /**
           * @brief Default constructor. Construct a default
           * allocator object instance.
           */
          allocator_stateless_polymorphic_synchronized () noexcept;

          /**
           * @brief Copy constructor.
           * @param other Reference to existing allocator.
           */
          allocator_stateless_polymorphic_synchronized (
              allocator_stateless_polymorphic_synchronized const & other) = default;

          /**
           * @brief Copy constructor template.
           * @param other Reference to allocator.
           */
          template<typename U>
            allocator_stateless_polymorphic_synchronized (
                allocator_stateless_polymorphic_synchronized<U, L, get_resource> const & other)
                    noexcept;

          /**
           * @brief Move constructor.
           * @param other Reference to existing allocator.
           */
          allocator_stateless_polymorphic_synchronized (
              allocator_stateless_polymorphic_synchronized && other) = default;

          /**
           * @brief Destruct the default allocator object instance.
           */
          ~allocator_stateless_polymorphic_synchronized () = default;

          /**
           * @}
           */

          /**
           * @name Operators
           * @{
           */

          /**
           * @brief Copy assignment operator.
           * @param other Reference to existing allocator.
           * @return Reference to allocator.
           */
          allocator_stateless_polymorphic_synchronized&
          operator= (allocator_stateless_polymorphic_synchronized const & other) = default;

          /**
           * @brief Move assignment operator.
           * @param other Reference to existing allocator.
           * @return Reference to allocator.
           */
          allocator_stateless_polymorphic_synchronized&
          operator= (allocator_stateless_polymorphic_synchronized && other) = default;

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
           *  Nothing.
           */
          void
          deallocate (value_type* addr, std::size_t elements) noexcept;

          /**
           * @brief The maximum number of elements that can be
           *  passed to `allocate()`.
           * @return Number of elements of type `value_type`.
           */
          std::size_t
          max_size (void) const noexcept;

#if 0
          allocator_stateless_polymorphic_synchronized
          select_on_container_copy_construction (void) const noexcept;

          memory_resource*
          resource (void) const noexcept;
#endif

          /**
           * @}
           */

        private:

          // This class should have no member variables, to meet the
          // default allocator stateless requirements.
        };

      template<typename T1, typename T2, typename L, F get_resource>
        bool
        operator== (
            const allocator_stateless_polymorphic_synchronized<T1, L,
                get_resource>& lhs,
            const allocator_stateless_polymorphic_synchronized<T2, L,
                get_resource>& rhs) noexcept;

      template<typename T1, typename T2, typename L, F get_resource>
        bool
        operator!= (
            const allocator_stateless_polymorphic_synchronized<T1, L,
                get_resource>& lhs,
            const allocator_stateless_polymorphic_synchronized<T2, L,
                get_resource>& rhs) noexcept;

      /**
       * @brief Allocator deleter.
       * @tparam A Allocator type.
       *
       * @details
       * This class acts as a function to be used as deleter
       * by smart pointers, like `unique_ptr<>`.
       *
       * It refers to the allocator to destruct and deallocate the object.
       */
      template<typename A>
        class allocator_deleter
        {
        public:

          /**
           * @brief Standard allocator type definition.
           */
          using allocator_type = A;

          /**
           * @brief Standard allocator traits definition.
           */
          using allocator_traits = std::allocator_traits<A>;

          using pointer = typename allocator_traits::pointer;

          /**
           * @name Constructors & Destructor
           * @{
           */

          /**
           * @brief Default constructor.
           */
          allocator_deleter ();

          /**
           * @brief Copy constructor.
           * @param other Reference to allocator.
           */
          allocator_deleter (const allocator_type& other);

          /**
           * @brief Move constructor.
           * @param other Reference to existing allocator.
           */
          allocator_deleter (allocator_deleter&& other) = default;

          /**
           * @brief Destruct the allocator deleter.
           */
          ~allocator_deleter () = default;

          /**
           * @}
           */

          /**
           * @name Operators
           * @{
           */

          /**
           * @brief Copy assignment operator.
           * @param other Reference to existing allocator.
           * @return Reference to allocator.
           */
          allocator_deleter&
          operator= (const allocator_deleter& other) = default;

          /**
           * @brief Move assignment operator.
           * @param other Reference to existing allocator.
           * @return Reference to allocator.
           */
          allocator_deleter&
          operator= (allocator_deleter&& other) = default;

          /**
           * @brief Function operator.
           * @param addr Pointer to memory to deallocate.
           */
          void
          operator() (pointer addr) const;

          /**
           * @}
           */

        protected:

          /**
           * @cond ignore
           */

          allocator_type a_;

          /**
           * @endcond
           */
        };

      /**
       * @brief Function template to allocate a unique pointer.
       * @tparam T Type of object to be allocated.
       * @tparam A Allocator type.
       * @tparam Args Variable arguments list.
       *
       * @param allocator Reference to allocator.
       * @param args Arguments used to construct the object of type T.
       * @return A standard unique pointer with deleter.
       */
      template<typename T, typename A, typename ... Args>
        auto
        allocate_unique (const A& allocator, Args&&... args);

      // ----------------------------------------------------------------------

      /**
       * @name Type Specific Resource Managers
       * @{
       */

      /**
       * @brief Function template to set a memory resource.
       * @param res Pointer to memory resource.
       * @return Pointer to previous memory resource.
       */
      template<typename T>
        memory_resource*
        set_resource_typed (memory_resource* res) noexcept;

      /**
       * @brief Function template to get a memory resource.
       * @return Pointer to current memory resource.
       */
      template<typename T>
        memory_resource*
        get_resource_typed (void) noexcept;

      // ----------------------------------------------------------------------

      template<>
        memory_resource*
        set_resource_typed<thread> (memory_resource* res) noexcept;

      template<>
        memory_resource*
        get_resource_typed<thread> (void) noexcept;

      // ----------------------------------------------------------------------

      template<>
        memory_resource*
        set_resource_typed<condition_variable> (memory_resource* res) noexcept;

      template<>
        memory_resource*
        get_resource_typed<condition_variable> (void) noexcept;

      // ----------------------------------------------------------------------

      template<>
        memory_resource*
        set_resource_typed<event_flags> (memory_resource* res) noexcept;

      template<>
        memory_resource*
        get_resource_typed<event_flags> (void) noexcept;

      // ----------------------------------------------------------------------

      template<>
        memory_resource*
        set_resource_typed<memory_pool> (memory_resource* res) noexcept;

      template<>
        memory_resource*
        get_resource_typed<memory_pool> (void) noexcept;

      // ----------------------------------------------------------------------

      template<>
        memory_resource*
        set_resource_typed<message_queue> (memory_resource* res) noexcept;

      template<>
        memory_resource*
        get_resource_typed<message_queue> (void) noexcept;

      // ----------------------------------------------------------------------

      template<>
        memory_resource*
        set_resource_typed<mutex> (memory_resource* res) noexcept;

      template<>
        memory_resource*
        get_resource_typed<mutex> (void) noexcept;

      // ----------------------------------------------------------------------

      template<>
        memory_resource*
        set_resource_typed<semaphore> (memory_resource* res) noexcept;

      template<>
        memory_resource*
        get_resource_typed<semaphore> (void) noexcept;

      // ----------------------------------------------------------------------

      template<>
        memory_resource*
        set_resource_typed<timer> (memory_resource* res) noexcept;

      template<>
        memory_resource*
        get_resource_typed<timer> (void) noexcept;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @brief Type of an allocator for objects of type T.
       * @tparam T type of object.
       *
       * @details
       * The allocator uses scheduler critical sections to be thread safe,
       * and the default memory resource associated with the given type.
       */
      template<typename T, typename U = T>
        using allocator_typed = allocator_stateless_polymorphic_synchronized<T, scheduler::lockable, get_resource_typed<U>>;

      /**
       * @brief Type of a RTOS unique pointer to objects of type T.
       * @tparam T type of object.
       *
       * @details
       * The type is based on the standard unique pointer, but with the
       * specific RTOS deleter.
       */
      template<typename T, typename U = T>
        using unique_ptr = std::unique_ptr<T, allocator_deleter<allocator_typed<T, U>>>;

    /**
     * @}
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

      /**
       * @cond ignore
       */

      extern memory_resource* default_resource;

      extern memory_resource* resource_thread;
      extern memory_resource* resource_condition_variable;
      extern memory_resource* resource_event_flags;
      extern memory_resource* resource_memory_pool;
      extern memory_resource* resource_message_queue;
      extern memory_resource* resource_mutex;
      extern memory_resource* resource_semaphore;
      extern memory_resource* resource_timer;

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------
      /**
       * @details
       * If not set explicitly by the user, this function
       * will return an instance of `null_memory_resource`
       * on bare metal platforms and of
       * `malloc_memory_resource` on POSIX platforms.
       */
      inline memory_resource*
      get_default_resource (void) noexcept
      {
        return default_resource;
      }

      /**
       * @details
       * If not set explicitly by the user, this function
       * will return an instance of `null_memory_resource`
       * on bare metal platforms and of
       * `malloc_memory_resource` on POSIX platforms.
       */
      template<>
        inline memory_resource*
        get_resource_typed<thread> (void) noexcept
        {
          return resource_thread;
        }

      /**
       * @details
       * If not set explicitly by the user, this function
       * will return an instance of `null_memory_resource`
       * on bare metal platforms and of
       * `malloc_memory_resource` on POSIX platforms.
       */
      template<>
        inline memory_resource*
        get_resource_typed<condition_variable> (void) noexcept
        {
          return resource_condition_variable;
        }

      /**
       * @details
       * If not set explicitly by the user, this function
       * will return an instance of `null_memory_resource`
       * on bare metal platforms and of
       * `malloc_memory_resource` on POSIX platforms.
       */
      template<>
        inline memory_resource*
        get_resource_typed<event_flags> (void) noexcept
        {
          return resource_event_flags;
        }

      /**
       * @details
       * If not set explicitly by the user, this function
       * will return an instance of `null_memory_resource`
       * on bare metal platforms and of
       * `malloc_memory_resource` on POSIX platforms.
       */
      template<>
        inline memory_resource*
        get_resource_typed<memory_pool> (void) noexcept
        {
          return resource_memory_pool;
        }

      /**
       * @details
       * If not set explicitly by the user, this function
       * will return an instance of `null_memory_resource`
       * on bare metal platforms and of
       * `malloc_memory_resource` on POSIX platforms.
       */
      template<>
        inline memory_resource*
        get_resource_typed<message_queue> (void) noexcept
        {
          return resource_message_queue;
        }

      /**
       * @details
       * If not set explicitly by the user, this function
       * will return an instance of `null_memory_resource`
       * on bare metal platforms and of
       * `malloc_memory_resource` on POSIX platforms.
       */
      template<>
        inline memory_resource*
        get_resource_typed<mutex> (void) noexcept
        {
          return resource_mutex;
        }

      /**
       * @details
       * If not set explicitly by the user, this function
       * will return an instance of `null_memory_resource`
       * on bare metal platforms and of
       * `malloc_memory_resource` on POSIX platforms.
       */
      template<>
        inline memory_resource*
        get_resource_typed<semaphore> (void) noexcept
        {
          return resource_semaphore;
        }

      /**
       * @details
       * If not set explicitly by the user, this function
       * will return an instance of `null_memory_resource`
       * on bare metal platforms and of
       * `malloc_memory_resource` on POSIX platforms.
       */
      template<>
        inline memory_resource*
        get_resource_typed<timer> (void) noexcept
        {
          return resource_timer;
        }

      // ======================================================================

      /**
       * @details
       */
      inline
      memory_resource::memory_resource (const char* name) :
          object_named
            { name }
      {
        ;
      }

      /**
       * @details
       * Allocate storage with a size of at least _bytes_ bytes. The
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
        ++allocations_;
        return do_allocate (bytes, alignment);
      }

      /**
       * @details
       * Deallocate the storage pointed to by _addr_.
       * The address shall have been returned
       * by a prior call to `allocate()` on a memory_resource
       * that compares equal to `*this`, and the storage it points to shall
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
        ++deallocations_;
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
      inline out_of_memory_handler_t
      memory_resource::out_of_memory_handler (out_of_memory_handler_t handler)
      {
        trace::printf ("%s(%p) @%p %s\n", __func__, handler, this, name ());

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

      inline std::size_t
      memory_resource::allocations (void)
      {
        return allocations_;
      }

      inline std::size_t
      memory_resource::deallocations (void)
      {
        return deallocations_;
      }

      inline void
      memory_resource::trace_print_statistics (void)
      {
#if defined(TRACE)
        trace::printf ("Memory '%s' @%p: \n"
                       "\ttotal: %u bytes, \n"
                       "\tallocated: %u bytes in %u chunk(s), \n"
                       "\tfree: %u bytes in %u chunk(s), \n"
                       "\tmax: %u bytes, \n"
                       "\tcalls: %u allocs, %u deallocs\n",
                       name (), this, total_bytes (), allocated_bytes (),
                       allocated_chunks (), free_bytes (), free_chunks (),
                       max_allocated_bytes (), allocations (),
                       deallocations ());
#endif /* defined(TRACE) */
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
          allocator_stateless_default_resource<T>::allocator_stateless_default_resource (
              allocator_stateless_default_resource<U> const & other __attribute__((unused))) noexcept
          {
            ;
          }

      template<typename T>
        inline typename allocator_stateless_default_resource<T>::value_type*
        allocator_stateless_default_resource<T>::allocate (std::size_t elements)
        {
          scheduler::critical_section scs;

          return static_cast<value_type*> (get_default_resource ()->allocate (
              elements * sizeof(value_type)));
        }

      template<typename T>
        inline void
        allocator_stateless_default_resource<T>::deallocate (
            value_type* addr, std::size_t elements) noexcept
        {
          scheduler::critical_section scs;

          get_default_resource ()->deallocate (addr,
                                               elements * sizeof(value_type));
        }

      template<typename T>
        inline std::size_t
        allocator_stateless_default_resource<T>::max_size (void) const noexcept
        {
          return get_default_resource ()->max_size () / sizeof(value_type);
        }

      // ======================================================================

      template<typename T, typename U, typename L, F get_resource>
        inline bool
        operator== (
            allocator_stateless_polymorphic_synchronized<T, L, get_resource> const & lhs,
            allocator_stateless_polymorphic_synchronized<U, L, get_resource> const & rhs) noexcept
        {
          return *lhs.resource () == *rhs.resource ();
        }

      template<typename T, typename U, typename L, F get_resource>
        inline bool
        operator!= (
            allocator_stateless_polymorphic_synchronized<T, L, get_resource> const & lhs,
            allocator_stateless_polymorphic_synchronized<U, L, get_resource> const & rhs) noexcept
        {
          return !(lhs == rhs);
        }

      // ======================================================================

      template<typename T, typename L, F get_resource>
        inline
        allocator_stateless_polymorphic_synchronized<T, L, get_resource>::allocator_stateless_polymorphic_synchronized () noexcept
        {
          trace::printf ("%s() @%p %p\n", __func__, this, get_resource ());
        }

      template<typename T, typename L, F get_resource>
        template<typename U>
          inline
          allocator_stateless_polymorphic_synchronized<T, L, get_resource>::allocator_stateless_polymorphic_synchronized (
              allocator_stateless_polymorphic_synchronized<U, L, get_resource> const & other __attribute__((unused))) noexcept
          {
            ;
          }

      template<typename T, typename L, F get_resource>
        typename allocator_stateless_polymorphic_synchronized<T, L, get_resource>::value_type*
        allocator_stateless_polymorphic_synchronized<T, L, get_resource>::allocate (
            std::size_t elements)
        {
          trace::printf ("%s(%u) @%p\n", __func__, elements, this);

#if 0
          std::size_t ms = max_size ();
          if ((ms > 0) && (elements > max_size ()))
            {
              estd::__throw_system_error (
                  EINVAL,
                  "allocator_stateless_polymorphic_synchronized<T>::allocate(size_t n)"
                  " 'n' exceeds maximum supported size");
            }
#endif

          locker_type lk;
          estd::lock_guard<locker_type> ulk
            { lk };

          return static_cast<value_type*> (get_resource ()->allocate (
              elements * sizeof(value_type), alignof(value_type)));
        }

      template<typename T, typename L, F get_resource>
        void
        allocator_stateless_polymorphic_synchronized<T, L, get_resource>::deallocate (
            value_type* addr, std::size_t elements) noexcept
        {
          trace::printf ("%s(%p,%u) @%p\n", __func__, addr, elements, this);

#if 0
          std::size_t ms = max_size ();
          if (ms > 0)
            {
              assert (elements <= max_size ());
            }
#endif

          locker_type lk;
          estd::lock_guard<locker_type> ulk
            { lk };

          get_resource ()->deallocate (addr, elements * sizeof(value_type),
                                       alignof(value_type));
        }

      template<typename T, typename L, F get_resource>
        inline std::size_t
        allocator_stateless_polymorphic_synchronized<T, L, get_resource>::max_size (
            void) const noexcept
        {
          return get_resource ()->max_size () / sizeof(T);
        }

#if 0
      template<typename T, typename L, F get_resource>
      inline allocator_stateless_polymorphic_synchronized<T, L, get_resource>
      allocator_stateless_polymorphic_synchronized<T, L, get_resource>::select_on_container_copy_construction (
          void) const noexcept
        {
          return allocator_stateless_polymorphic_synchronized ();
        }

      template<typename T, typename L, F get_resource>
      inline memory_resource*
      allocator_stateless_polymorphic_synchronized<T, L, get_resource>::resource (
          void) const noexcept
        {
          return get_resource ();
        }
#endif

      // ======================================================================

      template<typename A>
        inline
        allocator_deleter<A>::allocator_deleter ()
        {
          ;
        }

      template<typename A>
        inline
        allocator_deleter<A>::allocator_deleter (const allocator_type& other) :
            a_
              { other }
        {
          ;
        }

      /**
       * @brief Function operator to delete an allocated object.
       * @param addr Pointer to object.
       *
       * @details
       * Use `allocator_traits` to destroy and deallocate the object.
       */
      template<typename A>
        inline void
        allocator_deleter<A>::operator() (pointer addr) const
        {
          // Local allocator, without it many errors are issued.
          // TODO: understand exactly why.
          allocator_type alloc
            { a_ };

          // Call the object destructor.
          allocator_traits::destroy (alloc, std::addressof (*addr));

          // Deallocate the object, using the same allocator
          // used to allocate the object.
          allocator_traits::deallocate (alloc, addr, 1);
        }

      // ======================================================================

      /**
       * @details
       * This function is a factory of unique pointers.
       * It is inspired by `std::allocate_shared<>`.
       *
       * Objects are allocated using the given allocator, and
       * deallocated using a custom deleter associated to the allocator.
       *
       * The returned unique pointers always have the deleter associated,
       * so the object size is two pointers.
       */
      template<typename T, typename A, typename ... Args>
        auto
        allocate_unique (const A& allocator, Args&&... args)
        {
          /**
           * @brief Standard allocator type definition.
           */
          using allocator_type = A;

          /**
           * @brief Standard allocator traits definition.
           */
          using allocator_traits = std::allocator_traits<A>;

          static_assert(std::is_same<typename allocator_traits::value_type, std::remove_cv_t<T>>::value
              || std::is_base_of<typename allocator_traits::value_type, std::remove_cv_t<T>>::value,
              "Allocator must be of same type or derived.");

          static_assert(sizeof(T) <= sizeof(typename allocator_traits::value_type),
              "Derived type must not be larger.");

          allocator_type alloc
            { allocator };

          // Allocate space for 1 object instance of type T.
          auto p = allocator_traits::allocate (alloc, 1);

#if defined(__EXCEPTIONS)

          try
            {
              // Use placement new to construct the object.
              allocator_traits::construct (alloc, std::addressof (*p),
                                           std::forward<Args>(args)...);

              // Figure out the deleter type.
              using D = allocator_deleter<A>;

              // Make the unique pointer with the object and the deleter.
              return std::unique_ptr<T, D> (p, D (alloc));
            }
          catch (...)
            {
              allocator_traits::deallocate (alloc, p, 1);
              throw;
            }

#else

          // Use placement new to construct the object.
          allocator_traits::construct (alloc, std::addressof (*p),
              std::forward<Args>(args)...);

          // Figure out the deleter type.
          using D = allocator_deleter<A>;

          // Make the unique pointer with the object and the deleter.
          return std::unique_ptr<T, D> (p, D (alloc));

#endif /* defined(__EXCEPTIONS) */
        }

    // ------------------------------------------------------------------------
    } /* namespace memory */
  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_MEMORY_H_ */
