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
 * [Partly inspired from the LLVM libcxx sources].
 * Copyright (c) 2009-2013 by the contributors listed in
 * 'LLVM libcxx Credits.txt'. See 'LLVM libcxx License.txt' for details.
 *
 * References are to ISO/IEC 14882:2011(E) Third edition (2011-09-01).
 */

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/estd/memory_resource>

// ----------------------------------------------------------------------------

using namespace os;

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    namespace memory
    {
      // ======================================================================

      /**
       * @addtogroup cmsis-plus-rtos-memres
       * @{
       */

      /**
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @brief
       * An internal memory manager that allocates memory via
       * the system `std::malloc()` and deallocates via `std::free()`.
       *
       * It is the default memory manager when running on synthetic
       * POSIX platforms.
       *
       * @warning This memory manager is not thread safe.
       */
      class malloc_memory_resource : public rtos::memory::memory_resource
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a memory manager object instance.
         */
        malloc_memory_resource () = default;

        /**
         * @cond ignore
         */

        malloc_memory_resource (const malloc_memory_resource&) = delete;
        malloc_memory_resource (malloc_memory_resource&&) = delete;
        malloc_memory_resource&
        operator= (const malloc_memory_resource&) = delete;
        malloc_memory_resource&
        operator= (malloc_memory_resource&&) = delete;

        /**
         * @endcond
         */

        /**
         * @brief Destruct the memory manager object instance.
         */
        ~malloc_memory_resource () = default;

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
        do_allocate (std::size_t bytes,
                     std::size_t alignment __attribute__((unused))) override;

        /**
         * @brief Implementation of the memory deallocator.
         * @param addr Address of a previously allocated block to free.
         * @param bytes Number of bytes to deallocate (may be 0 if unknown).
         * @param alignment Alignment constraint (power of 2).
         * @par Returns
         *  Nothing.
         */
        virtual void
        do_deallocate (void* addr, std::size_t bytes __attribute__((unused)),
                       std::size_t alignment __attribute__((unused)))
                           noexcept override;

        /**
         * @}
         */
      };

      void*
      malloc_memory_resource::do_allocate (
          std::size_t bytes, std::size_t alignment __attribute__((unused)))
      {
        // Ignore alignment for now.
        return std::malloc (bytes);
      }

      void
      malloc_memory_resource::do_deallocate (
          void* addr, std::size_t bytes __attribute__((unused)),
          std::size_t alignment __attribute__((unused))) noexcept
      {
        // Ignore size and alignment for now.
        std::free (addr);
      }

      // ======================================================================

      /**
       * @cond ignore
       */

      // Standard
      class new_delete_memory_resource : public rtos::memory::memory_resource
      {
      public:

        ~new_delete_memory_resource () = default;

      protected:

        virtual void*
        do_allocate (size_t bytes, size_t alignment __attribute__((unused)))
            override;

        virtual void
        do_deallocate (void* addr, size_t bytes __attribute__((unused)),
                       size_t alignment __attribute__((unused)))
                           noexcept override;
      };

      void*
      new_delete_memory_resource::do_allocate (
          size_t bytes, size_t alignment __attribute__((unused)))
      {
        // Ignore alignment for now.
        return ::operator new (bytes);
      }

      void
      new_delete_memory_resource::do_deallocate (
          void* addr, size_t bytes __attribute__((unused)),
          size_t alignment __attribute__((unused))) noexcept
      {
        // Ignore size and alignment for now.
        ::operator delete (addr);
      }

      /**
       * @endcond
       */

      // ======================================================================
      /**
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @brief
       * An internal memory manager that throws a `bad_alloc()`
       * exception when trying to allocate.
       *
       * It is the default memory manager when running on bare metal
       * platforms, to prevent unwanted dynamic allocations,
       * unless another setting is done during startup.
       */
      class null_memory_resource : public rtos::memory::memory_resource
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a memory manager object instance.
         */
        null_memory_resource () = default;

        /**
         * @cond ignore
         */

        // The rule of five.
        null_memory_resource (const null_memory_resource&) = delete;
        null_memory_resource (null_memory_resource&&) = delete;
        null_memory_resource&
        operator= (const null_memory_resource&) = delete;
        null_memory_resource&
        operator= (null_memory_resource&&) = delete;

        /**
         * @endcond
         */

        /**
         * @brief Destruct the memory manager object instance.
         */
        ~null_memory_resource () = default;

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
        do_allocate (std::size_t bytes __attribute__((unused)),
                     std::size_t alignment __attribute__((unused))) override;

        /**
         * @brief Implementation of the memory deallocator.
         * @param addr Address of a previously allocated block to free.
         * @param bytes Number of bytes to deallocate (may be 0 if unknown).
         * @param alignment Alignment constraint (power of 2).
         * @par Returns
         *  Nothing.
         */
        virtual void
        do_deallocate (void* addr __attribute__((unused)),
                       std::size_t bytes __attribute__((unused)),
                       std::size_t alignment __attribute__((unused)))
                           noexcept override;
        /**
         * @}
         */
      };

      void*
      null_memory_resource::do_allocate (
          std::size_t bytes __attribute__((unused)),
          std::size_t alignment __attribute__((unused)))
      {
        estd::__throw_bad_alloc ();
      }

      void
      null_memory_resource::do_deallocate (
          void* addr __attribute__((unused)),
          std::size_t bytes __attribute__((unused)),
          std::size_t alignment __attribute__((unused))) noexcept
      {
        ;
      }

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @cond ignore
       */

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

      static malloc_memory_resource malloc_res;
      static new_delete_memory_resource new_delete_res;
      static null_memory_resource null_res;

#pragma GCC diagnostic pop

      /**
       * @endcond
       */

      /**
       * @cond ignore
       */

      // The default RTOS system memory resource.
#if defined(__APPLE__) || defined(__linux__)
      memory_resource* default_resource __attribute__((weak)) = &malloc_res;
#else
      memory_resource* default_resource __attribute__((weak)) = &null_res;
#endif

      /**
       * @endcond
       */

      /**
       * @details
       * On bare metal applications, this function is called
       * from `os_startup_initialize_free_store()`, during the
       * system startup, with a memory manager handling the free
       * RAM.
       *
       * @warning This function is not thread safe.
       */
      memory_resource*
      set_default_resource (memory_resource* res) noexcept
      {
        trace::printf ("rtos::memory::%s(%p) \n", __func__, res);

        memory_resource* old = default_resource;
        default_resource = res;

        return old;
      }

      /**
       * @details
       *
       * @see malloc_memory_resource
       */
      memory_resource*
      malloc_resource (void) noexcept
      {
        return &malloc_res;
      }

      // ======================================================================

      /**
       * @details
       */
      memory_resource::~memory_resource ()
      {
        ;
      }

      /**
       * @fn memory_resource::do_allocate()
       * @details
       * Allocates storage with a size of at least bytes bytes.
       * The returned storage is aligned to the specified alignment
       * if such alignment is supported, and to `alignof(std::max_align_t)`
       * otherwise.
       *
       * The function should return a `nullptr`
       * when no more memory is available.
       *
       * @par Exceptions
       *   The code itself should throw nothing, but if the out of memory
       *   handler is set, it may throw a `bad_alloc()` exception.
       *
       */

      /**
       * @fn memory_resource::do_deallocate()
       * @details
       * Deallocates the storage pointed to by `addr`.
       * The address must have been returned by a prior call to
       * `allocate(bytes, alignment)` on a memory_resource that
       * compares equal to `*this`, and the storage it points to
       * must not yet have been deallocated, otherwise the behaviour
       * is undefined.
       *
       * @par Exceptions
       *   Should throw nothing itself.
       */

      /**
       * @details
       * Compares `*this` for equality with `other`.
       * Two memory_resources compare equal if and only if memory
       * allocated from one memory_resource can be deallocated from
       * the other and vice versa.
       *
       * The most-derived type of other may not match the most derived
       * type of `*this`. A derived class implementation therefore must
       * typically check whether the most derived types of `*this` and
       * other match using dynamic_cast, and immediately return false
       * if the cast fails.
       *
       * @par Exceptions
       *   Throws nothing.
       */
      bool
      memory_resource::do_is_equal (memory_resource const &other) const noexcept
      {
        return &other == this;
      }

      /**
       * @details
       * The default implementation of this virtual function returns
       * zero, meaning the size is not known.
       *
       * Override this function to return the actual size.
       *
       * @par Standard compliance
       *   Extension to standard.
       */
      std::size_t
      memory_resource::do_max_size (void) const noexcept
      {
        return 0;
      }

    // ------------------------------------------------------------------------

    } /* namespace memory */
  } /* namespace rtos */
} /* namespace os */

namespace os
{
  namespace estd
  {
    namespace pmr
    {
      /**
       * @cond ignore
       */

      memory_resource*
      new_delete_resource (void) noexcept
      {
        return &rtos::memory::new_delete_res;
      }

      memory_resource*
      null_memory_resource (void) noexcept
      {
        return &rtos::memory::null_res;
      }

#if defined(__APPLE__) || defined(__linux__)
      memory_resource* default_resource __attribute__((weak))
          = &rtos::memory::malloc_res;
#else
    memory_resource* default_resource __attribute__((weak)) = &rtos::memory::null_res;
#endif

    /**
     * @endcond
     */
  }
/* namespace pmr */
} /* namespace estd */
} /* namespace os */

// ----------------------------------------------------------------------------
