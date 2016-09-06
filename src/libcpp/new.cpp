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
 * This file is part of the CMSIS++ proposal, intended as a CMSIS
 * replacement for C++ applications.
 *
 * [Partly inspired from the LLVM libcxx sources].
 * Copyright (c) 2009-2013 by the contributors listed in
 * 'LLVM libcxx Credits.txt'. See 'LLVM libcxx License.txt' for details.
 *
 * References are to ISO/IEC 14882:2011(E) Third edition (2011-09-01)
 */

/**
 * @file
 * @brief Global synchronised new/delete definitions.
 */

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/estd/memory_resource>

// ----------------------------------------------------------------------------

using namespace os;

// ----------------------------------------------------------------------------

namespace
{
  /**
   * @brief The current new handler.
   *
   * @details
   * The initial `new_handler` is a null pointer, initialised as
   * part of the .bss section.
   */
  std::new_handler new_handler_;
}

/**
 * @addtogroup cmsis-plus-rtos-memres
 * @{
 */

namespace std
{
  // Constant to be used as parameter to differentiate
  // the `noexcept` functions.
  const nothrow_t nothrow = nothrow_t
    { };

  /**
   * @brief Establishes the function designated by handler
   * as the current `new_handler`.
   * @param handler Pointer to user function.
   * @return The previous handler.
   *
   * @details
   * This handler is invoked when the standard operator new()
   * detect an out of memory condition, to give a chance to the
   * application process it properly. If the application
   * can arrange for more memory to be used for allocation,
   * this function should return and the allocation process is retried.
   * If not, this function should gracefully shut down and restart.
   *
   * The initial `new_handler` is a null pointer.
   */
  new_handler
  set_new_handler (new_handler handler) noexcept
  {
    trace::printf ("std::%s(%p) \n", __func__, handler);

    new_handler prev_handler;

    prev_handler = new_handler_;
    new_handler_ = handler;

    return prev_handler;
  }

  /**
   * @brief Get the current handler.
   * @par Parameters
   *  None.
   * @return Pointer to user function, or `nullptr` if not set.
   *
   * @details
   * The initial `new_handler` is a null pointer.
   */
  new_handler
  get_new_handler () noexcept
  {
    return new_handler_;
  }

} /* namespace std */

// ----------------------------------------------------------------------------

/**
 * @name Standard operators
 * @{
 */

/**
 * @brief Allocate space for a new object instance.
 * @param bytes Number of bytes to allocate.
 * @return Pointer to allocated object.
 *
 * @details
 * The allocation function (3.7.4.1) called by a new-expression (5.3.4)
 * to allocate a storage of size _bytes_ suitably aligned to represent
 * any object of that size.
 *
 * Return a non-null pointer to suitably aligned storage (3.7.4),
 * or else throw a `bad-alloc` exception. This requirement is
 * binding on a replacement version of this function.
 *
 * @note A C++ program may define a function with this function signature
 * that displaces the default version defined by the C++ standard library.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
void *
__attribute__((weak))
operator new (std::size_t bytes)
{
  assert(!rtos::interrupts::in_handler_mode ());
  if (bytes == 0)
    {
      bytes = 1;
    }

  // ----- Begin of critical section ------------------------------------------
  rtos::scheduler::critical_section scs;

  while (true)
    {
      void* mem = estd::pmr::get_default_resource ()->allocate (bytes);

      if (mem != nullptr)
        {
#if defined(OS_TRACE_LIBCPP_OPERATOR_NEW)
          trace::printf ("::%s(%d)=%p\n", __func__, bytes, mem);
#endif
          return mem;
        }

      // If allocate() fails and there is a new_handler,
      // call it to try free up memory.
      if (new_handler_)
        {
          new_handler_ ();
        }
      else
        {
          estd::__throw_bad_alloc ();
        }
    }

  // ----- End of critical section --------------------------------------------
}

/**
 * @brief Allocate space for a new object instance (nothrow).
 * @param bytes Number of bytes to allocate.
 * @param nothrow
 * @return Pointer to allocated object.
 *
 * @details
 * Same as `new(bytes)`, except that it is called
 * when a C++ program prefers a null
 * pointer result as an error indication, instead of a `bad_alloc` exception.
 *
 * Return a non-null pointer to suitably aligned storage (3.7.4),
 * or else return a null pointer. This `nothrow` version of `operator new`
 * returns a pointer obtained as if acquired from the (possibly replaced)
 * ordinary version. This requirement is binding on a replacement
 * version of this function.
 *
 * @note A C++ program may define a function with this function signature
 * that displaces the default version defined by the C++ standard library.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
void*
__attribute__((weak))
operator new (std::size_t bytes,
              const std::nothrow_t& nothrow __attribute__((unused))) noexcept
{
  assert(!rtos::interrupts::in_handler_mode ());

  if (bytes == 0)
    {
      bytes = 1;
    }

  // ----- Begin of critical section ------------------------------------------
  rtos::scheduler::critical_section scs;

  while (true)
    {
      void* mem = estd::pmr::get_default_resource ()->allocate (bytes);

      if (mem != nullptr)
        {
#if defined(OS_TRACE_LIBCPP_OPERATOR_NEW)
          trace::printf ("::%s(%d)=%p\n", __func__, bytes, mem);
#endif
          return mem;
        }

      // If allocate() fails and there is a new_handler,
      // call it to try free up memory.
      if (new_handler_)
        {
          new_handler_ ();
        }
      else
        {
          break; // return nullptr
        }
    }

  // ----- End of critical section --------------------------------------------

  return nullptr;
}

/**
 * @brief Allocate space for an array of new object instances.
 * @param bytes Number of bytes to allocate.
 * @return Pointer to allocated object.
 *
 * @details
 * The allocation function (3.7.4.1) called by the array form of a
 * new-expression (5.3.4) to allocate size bytes of storage suitably
 * aligned to represent any array object of that size or smaller.
 *
 * @note A C++ program may define a function with this function signature
 * that displaces the default version defined by the C++ standard library.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
void*
__attribute__((weak))
operator new[] (std::size_t bytes)
{
  return ::operator new (bytes);
}

/**
 * @brief Allocate space for an array of new object instances (nothrow).
 * @param bytes Number of bytes to allocate.
 * @param nothrow
 * @return Pointer to allocated object.
 *
 * @details
 * Same as new[](size), except that it is called by
 * a C++ program that prefers a null
 * pointer result as an error indication, instead of a `bad_alloc` exception.
 *
 * @note A C++ program may define a function with this function signature
 * that displaces the default version defined by the C++ standard library.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
void*
__attribute__((weak))
operator new[] (std::size_t bytes,
                const std::nothrow_t& nothrow __attribute__((unused))) noexcept
{
  return ::operator new (bytes, std::nothrow);
}

// ----------------------------------------------------------------------------

/**
 * @brief Deallocate the dynamically allocated object instance.
 * @param ptr Pointer to object.
 * @par Returns
 *  Nothing.
 *
 * @details
 * The deallocation function (3.7.4.2) called by a delete-expression
 * to render the value of ptr invalid.
 *
 * ptr shall be a null pointer or its value shall be a value returned by
 * an earlier call to the (possibly replaced) operator new(os::std::size_t)
 * or operator new(os::std::size_t,const std::nothrow_t&) which has not
 * been invalidated by an intervening call to operator delete(void*).
 *
 * If ptr is null, does nothing. Otherwise, reclaims the storage
 * allocated by the earlier call to operator new.
 *
 * @note A C++ program may define a function with this function signature
 * that displaces the default version defined by the C++ standard library.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
void
__attribute__((weak))
operator delete (void* ptr) noexcept
{
#if defined(OS_TRACE_LIBCPP_OPERATOR_NEW)
  trace::printf ("::%s(%p)\n", __func__, ptr);
#endif

  assert(!rtos::interrupts::in_handler_mode ());

  if (ptr)
    {
      // ----- Begin of critical section --------------------------------------
      rtos::scheduler::critical_section scs;

      // The unknown size is passed as 0.
      estd::pmr::get_default_resource ()->deallocate (ptr, 0);
      // ----- End of critical section ----------------------------------------
    }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wc++14-compat"

void
operator delete (void* ptr, std::size_t bytes) noexcept;

/**
 * @brief Deallocate the dynamically allocated object instance.
 * @param ptr Pointer to object.
 * @param bytes Number of bytes to deallocate.
 * @par Returns
 *  Nothing.
 *
 * @details
 * The deallocation function (3.7.4.2) called by a delete-expression
 * to render the value of _ptr_ invalid.
 *
 * _ptr_ shall be a null pointer or its value shall be a value returned by
 * an earlier call to the (possibly replaced) operator new()
 * which has not
 * been invalidated by an intervening call to operator delete(void*).
 *
 * If _ptr_ is null, does nothing. Otherwise, reclaims the storage
 * allocated by the earlier call to operator new.
 *
 * @note A C++ program may define a function with this function signature
 * that displaces the default version defined by the C++ standard library.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
void
__attribute__((weak))
operator delete (void* ptr, std::size_t bytes) noexcept
{
#if defined(OS_TRACE_LIBCPP_OPERATOR_NEW)
  trace::printf ("::%s(%p,%u)\n", __func__, ptr, bytes);
#endif

  assert(!rtos::interrupts::in_handler_mode ());

  if (ptr)
    {
      // ----- Begin of critical section --------------------------------------
      rtos::scheduler::critical_section scs;

      estd::pmr::get_default_resource ()->deallocate (ptr, bytes);
      // ----- End of critical section ----------------------------------------
    }
}

#pragma GCC diagnostic pop

/**
 * @brief Deallocate the dynamically allocated object instance (nothrow).
 * @param ptr Pointer to object.
 * @param nothrow
 * @par Returns
 *  Nothing.
 *
 * @details
 * The deallocation function (3.7.4.2) called by the implementation
 * to render the value of ptr invalid when the constructor invoked
 * from a nothrow placement version of the new-expression throws
 * an exception.
 *
 * @note A C++ program may define a function with this function signature
 * that displaces the default version defined by the C++ standard library.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
void
__attribute__((weak))
operator delete (void* ptr,
                 const std::nothrow_t& nothrow __attribute__((unused))) noexcept
{
#if defined(OS_TRACE_LIBCPP_OPERATOR_NEW)
  trace::printf ("::%s(%p)\n", __func__, ptr);
#endif

  assert(!rtos::interrupts::in_handler_mode ());

  if (ptr)
    {
      // ----- Begin of critical section --------------------------------------
      rtos::scheduler::critical_section scs;

      estd::pmr::get_default_resource ()->deallocate (ptr, 0);
      // ----- End of critical section ----------------------------------------
    }
}

/**
 * @brief Deallocate the dynamically allocated array of object.
 * @param ptr Pointer to array of objects.
 * @par Returns
 *  Nothing.
 *
 * @details
 * The deallocation function (3.7.4.2) called by the array form of
 * a delete-expression to render the value of ptr invalid.
 *
 * If _ptr_ is null, does nothing. Otherwise, reclaims the storage
 * allocated by the earlier call to operator new.
 *
 * @note A C++ program may define a function with this function signature
 * that displaces the default version defined by the C++ standard library.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
void
__attribute__((weak))
operator delete[] (void* ptr) noexcept
{
  ::operator delete (ptr);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wc++14-compat"

void
operator delete[] (void* ptr, std::size_t bytes) noexcept;

/**
 * @brief Deallocate the dynamically allocated array of object.
 * @param ptr Pointer to array of objects.
 * @param bytes Number of bytes to deallocate.
 * @par Returns
 *  Nothing.
 *
 * @details
 * The deallocation function (3.7.4.2) called by the array form of
 * a delete-expression to render the value of ptr invalid.
 *
 * If _ptr_ is null, does nothing. Otherwise, reclaims the storage
 * allocated by the earlier call to operator new.
 *
 * @note A C++ program may define a function with this function signature
 * that displaces the default version defined by the C++ standard library.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
void
__attribute__((weak))
operator delete[] (void* ptr, std::size_t bytes) noexcept
{
  ::operator delete (ptr, bytes);
}

#pragma GCC diagnostic pop

/**
 * @brief Deallocate the dynamically allocated array of object (nothrow).
 * @param ptr Pointer to array of objects.
 * @param nothrow
 * @par Returns
 *  Nothing.
 *
 * @details
 * The deallocation function (3.7.4.2) called by the implementation to
 * render the value of ptr invalid when the constructor invoked
 * from a nothrow placement version of the array new-expression
 * throws an exception.
 *
 * If _ptr_ is null, does nothing. Otherwise, reclaims the storage
 * allocated by the earlier call to operator new.
 *
 * @note A C++ program may define a function with this function signature
 * that displaces the default version defined by the C++ standard library.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
void
__attribute__((weak))
operator delete[] (void* ptr, const std::nothrow_t& nothrow) noexcept
{
  ::operator delete (ptr, nothrow);
}

/**
 * @}
 */

/**
 * @}
 */

// ----------------------------------------------------------------------------
