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

#include <cstddef>
#include <cstdlib>
#include <new>
#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/iso/malloc.h>

namespace
{
  /**
   * @brief The current new handler.
   * @details
   * The initial new_handler is a null pointer, initialised as
   * part of the .bss section.
   */
  std::new_handler __new_handler;
}

namespace std
{

  const nothrow_t nothrow = nothrow_t
    { };

  /**
   * @brief Establishes the function designated by handler as the current new_handler.
   * @param handler
   * @return The previous handler.
   * @details
   * The initial new_handler is a null pointer.
   */
  new_handler
  set_new_handler (new_handler handler) noexcept
  {
    new_handler prev_handler;

    // Use scheduler lock to synchronise access to the handler.
    os::rtos::scheduler::critical_section scs;

    prev_handler = __new_handler;
    __new_handler = handler;

    return prev_handler;
  }

  new_handler
  get_new_handler () noexcept
  {
    new_handler handler;

    // Use scheduler lock to synchronise access to the handler.
    os::rtos::scheduler::critical_section scs;

    handler = __new_handler;

    return handler;
  }

} /* namespace std */

/**
 * @details
 * The allocation function (3.7.4.1) called by a new-expression (5.3.4)
 * to allocate size bytes of storage suitably aligned to represent
 * any object of that size.
 *
 * Return a non-null pointer to suitably aligned storage (3.7.4),
 * or else throw a bad-alloc exception. This requirement is
 * binding on a replacement version of this function.
 *
 * @note A C++ program may define a function with this function signature
 * that displaces the default version defined by the C++ standard library.
 */
void *
__attribute__((weak))
#if defined(__EXCEPTIONS) || defined(__DOXYGEN__)
operator new (std::size_t size) noexcept(false)
#else
operator new (std::size_t size) noexcept
#endif
{    //
  if (size == 0)
    {
      size = 1;
    }

  void* p;

  // Synchronisation primitives already used by estd::malloc,
  // no need to use them again here.
  while ((p = os::estd::malloc (size)) == 0)
    {
      // If malloc() fails and there is a new_handler,
      // call it to try free up memory.
      if (__new_handler)
        {
          __new_handler ();
        }
      else
        {
#if defined(__EXCEPTIONS)
          throw std::bad_alloc ();
#else
          break;
#endif
        }
    }
  return p;
}

/**
 * @details
 * Same as new(size), except that it is called by a placement
 * version of a new-expression when a C++ program prefers a null
 * pointer result as an error indication, instead of a bad_alloc exception.
 *
 * Return a non-null pointer to suitably aligned storage (3.7.4),
 * or else return a null pointer. This nothrow version of operator new
 * returns a pointer obtained as if acquired from the (possibly replaced)
 * ordinary version. This requirement is binding on a replacement
 * version of this function.
 *
 * @note A C++ program may define a function with this function signature
 * that displaces the default version defined by the C++ standard library.
 */
void*
__attribute__((weak))
operator new (std::size_t size, const std::nothrow_t&) noexcept
{
  void* p = 0;
#if defined(__EXCEPTIONS)
  try
    {
      p = ::operator new (size);
    }
  catch (...)
    {
    }
#else
  p = ::operator new (size);
#endif  // __EXCEPTIONS
  return p;
}

/**
 * @details
 * The allocation function (3.7.4.1) called by the array form of a
 * new-expression (5.3.4) to allocate size bytes of storage suitably
 * aligned to represent any array object of that size or smaller.
 *
 * @note A C++ program may define a function with this function signature
 * that displaces the default version defined by the C++ standard library.
 */
void*
__attribute__((weak))
#if defined(__EXCEPTIONS) || defined(__DOXYGEN__)
operator new[] (std::size_t size) noexcept(false)
#else
operator new[] (std::size_t size) noexcept
#endif
{
  return ::operator new (size);
}

/**
 * @details
 * Same as new[](size), except that it is called by a placement
 * version of a new-expression when a C++ program prefers a null
 * pointer result as an error indication, instead of a bad_alloc exception.
 *
 * @note A C++ program may define a function with this function signature
 * that displaces the default version defined by the C++ standard library.
 */
void*
__attribute__((weak))
operator new[] (std::size_t size, const std::nothrow_t&) noexcept
{
  void* p = 0;
#if defined(__EXCEPTIONS)
  try
    {
      p = ::operator new[] (size);
    }
  catch (...)
    {
    }
#else
  p = ::operator new[] (size);
#endif  // __EXCEPTIONS
  return p;
}

/**
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
 */
void
__attribute__((weak))
operator delete (void* ptr) noexcept
{
  if (ptr)
    {
      // Synchronisation primitives used by free()
      os::estd::free (ptr);
    }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wc++14-compat"

void
operator delete (void* ptr, std::size_t size) noexcept;

void
__attribute__((weak))
operator delete (void* ptr, std::size_t size __attribute__((unused))) noexcept
{
  if (ptr)
    {
      // Synchronisation primitives used by free()
      os::estd::free (ptr);
    }
}

#pragma GCC diagnostic push

/**
 * @details
 * The deallocation function (3.7.4.2) called by the implementation
 * to render the value of ptr invalid when the constructor invoked
 * from a nothrow placement version of the new-expression throws
 * an exception.
 *
 * @note A C++ program may define a function with this function signature
 * that displaces the default version defined by the C++ standard library.
 */
void
__attribute__((weak))
operator delete (void* ptr, const std::nothrow_t&) noexcept
{
  ::operator delete (ptr);
}

/**
 * @details
 * The deallocation function (3.7.4.2) called by the array form of
 * a delete-expression to render the value of ptr invalid.
 *
 * @note A C++ program may define a function with this function signature
 * that displaces the default version defined by the C++ standard library.
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
operator delete[] (void* ptr, std::size_t size) noexcept;

void
__attribute__((weak))
operator delete[] (void* ptr, std::size_t size __attribute__((unused))) noexcept
{
  ::operator delete (ptr);
}

#pragma GCC diagnostic push

/**
 * @details
 * The deallocation function (3.7.4.2) called by the implementation to
 * render the value of ptr invalid when the constructor invoked
 * from a nothrow placement version of the array new-expression
 * throws an exception.
 *
 * @note A C++ program may define a function with this function signature
 * that displaces the default version defined by the C++ standard library.
 */
void
__attribute__((weak))
operator delete[] (void* ptr, const std::nothrow_t&) noexcept
{
  ::operator delete[] (ptr);
}
