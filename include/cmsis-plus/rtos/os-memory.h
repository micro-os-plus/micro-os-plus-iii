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

#include <cstddef>
#include <cerrno>
#include <cassert>
#include <limits>
#include <memory>

#include <cmsis-plus/iso/system_error>

// ----------------------------------------------------------------------------

// These definitions refer only to the RTOS allocators.
// The application should use the similar ones from the
// os::estd:: namespace.

namespace os
{
  namespace rtos
  {
    namespace memory
    {
      // ======================================================================

      template<typename T>
        class new_delete_allocator
        {
        public:

          typedef T value_type;

          new_delete_allocator () noexcept = default;
          new_delete_allocator (new_delete_allocator const & a) = default;

          template<typename U>
            new_delete_allocator (new_delete_allocator<U> const & other)
                noexcept;

          new_delete_allocator&
          operator= (new_delete_allocator const & a) = default;

          value_type*
          allocate (std::size_t size);

          void
          deallocate (value_type* p, std::size_t bytes) noexcept;

          std::size_t
          max_size (void) const noexcept;
        };

      // ======================================================================

      class memory_resource;

      bool
      operator== (const memory_resource& a, const memory_resource& b) noexcept;
      bool
      operator!= (const memory_resource& a, const memory_resource& b) noexcept;

      template<typename T>
        class polymorphic_allocator;

      template<typename T1, typename T2>
        bool
        operator== (const polymorphic_allocator<T1>& a,
                    const polymorphic_allocator<T2>& b) noexcept;

      template<typename T1, typename T2>
        bool
        operator!= (const polymorphic_allocator<T1>& a,
                    const polymorphic_allocator<T2>& b) noexcept;

      // Global memory resources
      memory_resource*
      new_delete_resource (void) noexcept;

      memory_resource*
      null_memory_resource (void) noexcept;

      // The default memory resource
      memory_resource*
      set_default_resource (memory_resource* r) noexcept;

      memory_resource*
      get_default_resource (void) noexcept;

      // ======================================================================

      class memory_resource
      {

      public:

        static constexpr std::size_t max_align = alignof(std::max_align_t);

        virtual
        ~memory_resource ();

        void*
        allocate (std::size_t bytes, std::size_t alignment = max_align);

        void
        deallocate (void* p, std::size_t bytes, std::size_t alignment =
                        max_align);

        bool
        is_equal (memory_resource const & other) const noexcept;

      protected:

        virtual void*
        do_allocate (std::size_t bytes, std::size_t alignment) = 0;

        virtual void
        do_deallocate (void* p, std::size_t bytes, std::size_t alignment) = 0;

        virtual bool
        do_is_equal (memory_resource const &r) const noexcept = 0;
      };

      // ======================================================================

      template<typename T>
        class polymorphic_allocator
        {
        public:

          typedef T value_type;

          polymorphic_allocator () noexcept;

          polymorphic_allocator (memory_resource* r) noexcept;

          polymorphic_allocator (polymorphic_allocator const & a) = default;

          template<typename U>
            polymorphic_allocator (polymorphic_allocator<U> const & other)
                noexcept;

          polymorphic_allocator&
          operator= (polymorphic_allocator const & a) = default;

          value_type*
          allocate (std::size_t n);

          void
          deallocate (value_type* p, std::size_t n) noexcept;

          std::size_t
          max_size (void) const noexcept;

          polymorphic_allocator
          select_on_container_copy_construction (void) const noexcept;

          memory_resource*
          resource (void) const noexcept;

        private:

          memory_resource* res_;
        };

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

      // ======================================================================

      template<typename T>
        template<typename U>
          inline
          new_delete_allocator<T>::new_delete_allocator (
              new_delete_allocator<U> const & other __attribute__((unused))) noexcept
          {
            ;
          }

      template<typename T>
        inline typename new_delete_allocator<T>::value_type*
        new_delete_allocator<T>::allocate (std::size_t n)
        {
          return static_cast<value_type*> (::operator new (
              n * sizeof(value_type)));
        }

      template<typename T>
        inline void
        new_delete_allocator<T>::deallocate (
            value_type* p, std::size_t n __attribute__((unused))) noexcept
        {
          ::operator delete (p);
        }

      // ======================================================================

      inline void*
      memory_resource::allocate (std::size_t bytes, std::size_t align)
      {
        return do_allocate (bytes, align);
      }

      inline void
      memory_resource::deallocate (void* p, std::size_t bytes,
                                   std::size_t align)
      {
        do_deallocate (p, bytes, align);
      }

      inline bool
      memory_resource::is_equal (memory_resource const & other) const noexcept
      {
        return do_is_equal (other);
      }

      // ======================================================================

      template<typename T>
        polymorphic_allocator<T>::polymorphic_allocator () noexcept :
        res_(get_default_resource())
          {
            ;
          }

      template<typename T>
        inline
        polymorphic_allocator<T>::polymorphic_allocator (memory_resource* r) noexcept :
        res_(r)
          {
            ;
          }

      template<typename T>
        template<typename U>
          inline
          polymorphic_allocator<T>::polymorphic_allocator (
              polymorphic_allocator<U> const & other) noexcept :
          res_(other.resource())
            {
              ;
            }

      template<typename T>
        inline typename polymorphic_allocator<T>::value_type*
        polymorphic_allocator<T>::allocate (std::size_t n)
        {
          if (n > max_size ())
            {
              estd::__throw_system_error (
                  EINVAL, "polymorphic_allocator<T>::allocate(std::size_t n)"
                  " 'n' exceeds maximum supported size");
            }

          return static_cast<value_type*> (res_->allocate (
              n * sizeof(value_type), alignof(value_type)));
        }

      template<typename T>
        inline void
        polymorphic_allocator<T>::deallocate (value_type* p, std::size_t n) noexcept
        {
          assert(n <= max_size ());
          res_->deallocate (p, n * sizeof(value_type), alignof(value_type));
        }

      template<typename T>
        inline std::size_t
        polymorphic_allocator<T>::max_size (void) const noexcept
        {
          return std::numeric_limits<std::size_t>::max () / sizeof(value_type);
        }

      template<typename T>
        inline polymorphic_allocator<T>
        polymorphic_allocator<T>::select_on_container_copy_construction (
            void) const noexcept
        {
          return polymorphic_allocator ();
        }

      template<typename T>
        inline memory_resource*
        polymorphic_allocator<T>::resource (void) const noexcept
        {
          return res_;
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

      template<typename T, typename U>
        inline bool
        operator== (polymorphic_allocator<T> const & lhs,
                    polymorphic_allocator<U> const & rhs) noexcept
        {
          return *lhs.resource () == *rhs.resource ();
        }

      template<typename T, typename U>
        inline bool
        operator!= (polymorphic_allocator<T> const & lhs,
                    polymorphic_allocator<U> const & rhs) noexcept
        {
          return !(lhs == rhs);
        }

    } /* namespace memory */
  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_MEMORY_H_ */
