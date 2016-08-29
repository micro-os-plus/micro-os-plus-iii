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

#include <cmsis-plus/estd/memory_resource>
#include <cmsis-plus/estd/system_error>

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

    namespace memory
    {
      // ----------------------------------------------------------------------

      constexpr std::size_t
      max (std::size_t a, std::size_t b)
      {
        return a >= b ? a : b;
      }

      using memory_resource = estd::memory_resource;

      template<typename L>
        class lock_guard;

      using D_T = memory_resource* (void);

      template<typename T, typename L, D_T D>
        class polymorphic_synchronized_allocator;

      template<typename T1, typename T2, typename L, D_T D>
        bool
        operator== (const polymorphic_synchronized_allocator<T1, L, D>& a,
                    const polymorphic_synchronized_allocator<T2, L, D>& b)
                        noexcept;

      template<typename T1, typename T2, typename L, D_T D>
        bool
        operator!= (const polymorphic_synchronized_allocator<T1, L, D>& a,
                    const polymorphic_synchronized_allocator<T2, L, D>& b)
                        noexcept;

      // ----------------------------------------------------------------------

      /**
       * @brief Get the address of the malloc memory resource.
       * @return Pointer to memory resource.
       */
      memory_resource*
      malloc_resource () noexcept;

      /**
       * @brief Set the default RTOS memory resource.
       * @param r Pointer to memory resource.
       * @return Pointer to previous memory resource.
       */
      memory_resource*
      set_default_resource (memory_resource* r) noexcept;

      /**
       * @brief Get the default RTOS memory resource.
       * @return Pointer to memory resource.
       */
      memory_resource*
      get_default_resource (void) noexcept;

      // ======================================================================

      /**
       * @brief Null locker.
       * @headerfile os.h <cmsis-plus/estd/memory_resource>
       */
      class null_locker
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a null lockable object instance.
         * @par Parameters
         *  None
         */

        constexpr
        null_locker ();

        /**
         * @cond ignore
         */

        null_locker (const null_locker&) = delete;
        null_locker (null_locker&&) = delete;
        null_locker&
        operator= (const null_locker&) = delete;
        null_locker&
        operator= (null_locker&&) = delete;

        /**
         * @endcond
         */

        /**
         * @brief Destruct the null lockable object instance.
         */
        ~null_locker ();

        /**
         * @}
         */

      public:

        /**
         * @name Public Member Functions
         * @{
         */

        /**
         * @brief Lock the scheduler.
         * @par Parameters
         *  None
         * @return  Nothing.
         */
        void
        lock (void);

        /**
         * @brief Unlock the scheduler.
         * @par Parameters
         *  None
         * @return  Nothing.
         */
        void
        unlock (void);

        /**
         * @}
         */

      };

      // ======================================================================

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
          allocate (std::size_t size);

          void
          deallocate (value_type* p, std::size_t bytes) noexcept;

          std::size_t
          max_size (void) const noexcept;

          polymorphic_synchronized_allocator
          select_on_container_copy_construction (void) const noexcept;

          memory_resource*
          resource (void) const noexcept;

        private:

          memory_resource* res_;
        };

      // ======================================================================

      template<typename T>
        class default_resource_allocator
        {
        public:

          typedef T value_type;

          default_resource_allocator () noexcept = default;
          default_resource_allocator (default_resource_allocator const & a) = default;

          template<typename U>
            default_resource_allocator (
                default_resource_allocator<U> const & other) noexcept;

          default_resource_allocator&
          operator= (default_resource_allocator const & a) = default;

          value_type*
          allocate (std::size_t size);

          void
          deallocate (value_type* p, std::size_t bytes) noexcept;

          std::size_t
          max_size (void) const noexcept;
        };

    // ----------------------------------------------------------------------

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

      inline memory_resource*
      get_default_resource (void) noexcept
      {
        return default_resource;
      }

      // ========================================================================

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

      inline
      null_locker::~null_locker ()
      {
        ;
      }

      inline void
      null_locker::lock (void)
      {
        ;
      }

      inline void
      null_locker::unlock (void)
      {
        ;
      }
      // ======================================================================

      template<typename T, typename L, D_T D>
        polymorphic_synchronized_allocator<T, L, D>::polymorphic_synchronized_allocator () noexcept :
        res_(D())
          {
            trace::printf ("%s() @%p %p\n", __func__, this, res_);
            ;
          }

      template<typename T, typename L, D_T D>
        inline
        polymorphic_synchronized_allocator<T, L, D>::polymorphic_synchronized_allocator (
            memory_resource* r) noexcept :
        res_(r)
          {
            trace::printf ("%s(%p) @%p\n", __func__, r, this);
            ;
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
        inline typename polymorphic_synchronized_allocator<T, L, D>::value_type*
        polymorphic_synchronized_allocator<T, L, D>::allocate (
            std::size_t bytes)
        {
          trace::printf ("%s(%u) @%p\n", __func__, bytes, this);
          if (bytes > max_size ())
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
              bytes * sizeof(value_type), alignof(value_type)));
        }

      template<typename T, typename L, D_T D>
        inline void
        polymorphic_synchronized_allocator<T, L, D>::deallocate (
            value_type * p, std::size_t bytes) noexcept
        {
          assert(bytes <= max_size ());
          trace::printf ("%s(%p,%u) @%p\n", __func__, p, bytes, this);

          locker_type lk;
          lock_guard<locker_type> ulk
            { lk };

          res_->deallocate (p, bytes * sizeof(value_type), alignof(value_type));
        }

      template<typename T, typename L, D_T D>
        inline std::size_t
        polymorphic_synchronized_allocator<T, L, D>::max_size (void) const noexcept
        {
          return std::numeric_limits<std::size_t>::max () / sizeof(value_type);
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
        default_resource_allocator<T>::allocate (std::size_t n)
        {
          scheduler::critical_section scs;

          return static_cast<value_type*> (get_default_resource ()->allocate (
              n * sizeof(value_type)));
        }

      template<typename T>
        inline void
        default_resource_allocator<T>::deallocate (
            value_type* p, std::size_t n __attribute__((unused))) noexcept
        {
          scheduler::critical_section scs;

          get_default_resource ()->deallocate (std::nothrow, p, 0);
        }

    // ------------------------------------------------------------------------

    } /* namespace memory */
  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_MEMORY_H_ */
