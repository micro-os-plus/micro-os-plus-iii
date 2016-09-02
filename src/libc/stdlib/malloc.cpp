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

#if defined(__ARM_EABI__) || defined(__DOXYGEN__)

// ----------------------------------------------------------------------------

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/estd/memory_resource>

#include <malloc.h>

// ----------------------------------------------------------------------------

using namespace os;

// ----------------------------------------------------------------------------

/**
 * @addtogroup cmsis-plus-rtos-c-memres
 * @{
 */

/**
 * @name Standard functions
 * @{
 */


// These library functions were modified to use the application
// memory resource and to be thread safe.
/**
 * @brief Allocate a memory block (non-initialised).
 * @headerfile stdlib.h <stdlib.h>
 * @param bytes Number of bytes to allocate.
 * @return A pointer to the allocated memory or null and `ENOMEM`.
 *
 * @details
 * The `malloc()` function shall allocate unused space for an object
 * whose size is _bytes_ and whose value is unspecified.
 *
 * The order and contiguity of storage allocated by successive calls
 * to `malloc()` is unspecified. The pointer returned if the allocation
 * succeeds shall be suitably aligned so that it may be assigned to a
 * pointer to any type of object and then used to access such an object
 * in the space allocated (until the space is explicitly freed or
 * reallocated). Each such allocation shall yield a pointer to an
 * object disjoint from any other object. The pointer returned points
 * to the start (lowest byte address) of the allocated space. If the
 * space cannot be allocated, a null pointer shall be returned. If
 * the size of the space requested is 0, the behaviour is
 * implementation-defined: the value returned shall be either a
 * null pointer or a unique pointer.
 *
 * Upon successful completion with size not equal to 0, `malloc()`
 * shall return a pointer to the allocated space. If size is 0,
 * either a null pointer or a unique pointer that can be successfully
 * passed to `free()` shall be returned. Otherwise, it shall return a
 * null pointer and set `errno` to indicate the error.
 *
 * @note In CMSIS++ this function uses a scheduler critical section
 * and is thread safe.
 *
 * @par POSIX compatibility
 *  Inspired by [`malloc()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/malloc.html)
 *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
void*
malloc (size_t bytes)
{
  assert(!rtos::interrupts::in_handler_mode ());

  void* mem;
    {
      // ----- Begin of critical section --------------------------------------
      rtos::scheduler::critical_section scs;

      errno = 0;
      mem = estd::pmr::get_default_resource ()->allocate (bytes);
      if (mem == nullptr)
        {
          errno = ENOMEM;
        }

#if defined(OS_TRACE_LIBC_MALLOC)
      trace::printf ("::%s(%d)=%p\n", __func__, bytes, mem);
#endif
      // ----- End of critical section ----------------------------------------
    }

  return mem;
}

/**
 * @brief Allocate an array of memory blocks (initialised to zero).
 * @headerfile stdlib.h <stdlib.h>
 * @param nelem Number of elements.
 * @param elbytes Element size in bytes.
 * @return A pointer to the allocated memory or null and `ENOMEM`.
 *
 * @details
 * The `calloc()` function shall allocate unused space for an
 * array of _nelem_ elements each of whose size in bytes is _elbytes_.
 * The space shall be initialised to all bits 0.
 *
 * The order and contiguity of storage allocated by successive calls
 * to `calloc()` is unspecified. The pointer returned if the allocation
 * succeeds shall be suitably aligned so that it may be assigned to a
 * pointer to any type of object and then used to access such an object
 * or an array of such objects in the space allocated (until the space
 * is explicitly freed or reallocated). Each such allocation shall
 * yield a pointer to an object disjoint from any other object. The
 * pointer returned shall point to the start (lowest byte address)
 * of the allocated space. If the space cannot be allocated, a null
 * pointer shall be returned. If the size of the space requested is
 * 0, the behaviour is implementation-defined: the value returned shall
 * be either a null pointer or a unique pointer.
 *
 * Upon successful completion with both _nelem_ and _elbytes_ non-zero,
 * `calloc()` shall return a pointer to the allocated space. If either
 * _nelem_ or _elbytes_ is 0, then either a null pointer or a unique
 * pointer value that can be successfully passed to `free()` shall be
 * returned. Otherwise, it shall return a null pointer and set `errno`
 * to indicate the error.
 *
 * @note In CMSIS++ this function uses a scheduler critical section
 * and is thread safe.
 *
 * @par POSIX compatibility
 *  Inspired by [`calloc()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/calloc.html)
 *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
void*
calloc (size_t nelem, size_t elbytes)
{
  assert(!rtos::interrupts::in_handler_mode ());

  errno = 0;
  if (nelem == 0 || elbytes == 0)
    {
      return nullptr;
    }

  void* mem;
    {
      // ----- Begin of critical section --------------------------------------
      rtos::scheduler::critical_section scs;

      mem = estd::pmr::get_default_resource ()->allocate (nelem * elbytes);

#if defined(OS_TRACE_LIBC_MALLOC)
      trace::printf ("::%s(%u,%u)=%p\n", __func__, nelem, elbytes, mem);
#endif
      // ----- End of critical section ----------------------------------------
    }

  if (mem != nullptr)
    {
      memset (mem, 0, nelem * elbytes);
    }
  else
    {
      errno = ENOMEM;
    }

  return mem;
}

/**
 * @brief Reallocate the memory block (non-initialised).
 * @headerfile stdlib.h <stdlib.h>
 * @param ptr Pointer to previously allocated block.
 * @param bytes Number of bytes to re-allocate.
 * @return A pointer to the allocated memory or null and `ENOMEM`.
 *
 * @details
 * The `realloc()` function shall deallocate the old object pointed
 * to by _ptr_ and return a pointer to a new object that has the size
 * specified by `bytes`. The contents of the new object shall be the
 * same as that of the old object prior to deallocation, up to the
 * lesser of the new and old sizes. Any bytes in the new object beyond
 * the size of the old object have indeterminate values. If the size
 * of the space requested is zero, the behaviour shall be
 * implementation-defined: either a null pointer is returned, or
 * the behavior shall be as if the size were some non-zero value,
 * except that the returned pointer shall not be used to access an
 * object. If the space cannot be allocated, the object shall remain
 * unchanged.
 *
 * If _ptr_ is a null pointer, `realloc()` shall be equivalent to
 * `malloc()` for the specified size.
 *
 * If _ptr_ does not match a pointer returned earlier by `calloc()`,
 * `malloc()`, or `realloc()` or if the space has previously been
 * deallocated by a call to `free()` or `realloc()`, the behaviour
 * is undefined.
 *
 * The order and contiguity of storage allocated by successive
 * calls to `realloc()` is unspecified. The pointer returned if the
 * allocation succeeds shall be suitably aligned so that it may be
 * assigned to a pointer to any type of object and then used to
 * access such an object in the space allocated (until the space
 * is explicitly freed or reallocated). Each such allocation shall
 * yield a pointer to an object disjoint from any other object. The
 * pointer returned shall point to the start (lowest byte address)
 * of the allocated space. If the space cannot be allocated, a null
 * pointer shall be returned.
 *
 * Upon successful completion, `realloc()` shall return a pointer
 * to the (possibly moved) allocated space. If size is 0, either:
 * - A null pointer shall be returned and `errno` set to an
 * implementation-defined value.
 * - A unique pointer that can be successfully passed to `free()`
 * shall be returned, and the memory object pointed to by _ptr_
 * shall be freed. The application shall ensure that the pointer
 * is not used to access an object.
 *
 * If there is not enough available memory, `realloc()` shall
 * return a null pointer and set `errno` to `ENOMEM`. If `realloc()`
 * returns a null pointer and `errno` has been set to `ENOMEM`,
 * the memory referenced by _ptr_ shall not be changed.
 *
 * @note In CMSIS++ this function uses a scheduler critical section
 * and is thread safe.
 *
 * @par POSIX compatibility
 *  Inspired by [`realloc()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/realloc.html)
 *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
void*
realloc (void* ptr, size_t bytes)
{
  assert(!rtos::interrupts::in_handler_mode ());

  void* mem;

    {
      // ----- Begin of critical section --------------------------------------
      rtos::scheduler::critical_section scs;

      errno = 0;
      if (ptr == nullptr)
        {
          mem = estd::pmr::get_default_resource ()->allocate (bytes);
#if defined(OS_TRACE_LIBC_MALLOC)
          trace::printf ("::%s(%p,%u)=%p\n", __func__, ptr, bytes, mem);
#endif
          if (mem == nullptr)
            {
              errno = ENOMEM;
            }
          return mem;
        }

      if (bytes == 0)
        {
          estd::pmr::get_default_resource ()->deallocate (ptr, 0);
#if defined(OS_TRACE_LIBC_MALLOC)
          trace::printf ("::%s(%p,%u)=0\n", __func__, ptr, bytes);
#endif
          return nullptr;
        }

#if 0
      /* TODO: There is chance to shrink the chunk if newly requested
       * size is much small */
      if (nano_malloc_usable_size (RCALL ptr) >= bytes)
      return ptr;
#endif

      mem = estd::pmr::get_default_resource ()->allocate (bytes);
      if (mem != nullptr)
        {
          memcpy (mem, ptr, bytes);
          estd::pmr::get_default_resource ()->deallocate (ptr, 0);
        }
      else
        {
          errno = ENOMEM;
        }

#if defined(OS_TRACE_LIBC_MALLOC)
      trace::printf ("::%s(%p,%u)=%p", __func__, ptr, bytes, mem);
#endif
      // ----- End of critical section ----------------------------------------
    }

  return mem;
}

/**
 * @brief Free the allocated memory block.
 * @headerfile stdlib.h <stdlib.h>
 * @param ptr Pointer to previously allocated block.
 * @par Returns
 *  Nothing.
 *
 * @details
 * The `free()` function shall cause the space pointed to by _ptr_
 * to be deallocated; that is, made available for further allocation.
 * If _ptr_ is a null pointer, no action shall occur. Otherwise, if
 * the argument does not match a pointer earlier returned by a
 * function in POSIX.1-2008 that allocates memory as if by `malloc()`,
 * or if the space has been deallocated by a call to `free()` or
 * `realloc()`, the behaviour is undefined.
 *
 * Any use of a pointer that refers to freed space results in
 * undefined behaviour.
 *
 * The `free()` function shall not return a value.
 *
 * @note In CMSIS++ this function uses a scheduler critical section
 * and is thread safe.
 *
 * @par POSIX compatibility
 *  Inspired by [`free()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/free.html)
 *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
void
free (void* ptr)
{
  assert(!rtos::interrupts::in_handler_mode ());

  if (ptr == nullptr)
    {
      return;
    }

  // ----- Begin of critical section ------------------------------------------
  rtos::scheduler::critical_section scs;

#if defined(OS_TRACE_LIBC_MALLOC)
  trace::printf ("::%s(%p)\n", __func__, ptr);
#endif

  // Size unknown, pass 0.
  estd::pmr::get_default_resource ()->deallocate (ptr, 0);
  // ----- End of critical section --------------------------------------------
}

/**
 * @}
 */

/**
 * @}
 */

// ----------------------------------------------------------------------------
// Redirect impure functions to the implementation, to avoid including
// weird newlib functions.
/**
 * @cond ignore
 */

void*
_malloc_r (struct _reent* impure __attribute__((unused)), size_t size)
{
  return malloc (size);
}

void*
_calloc_r (struct _reent* impure __attribute__((unused)), size_t n, size_t elem)
{
  return calloc (n, elem);
}

void
_free_r (struct _reent* impure __attribute__((unused)), void* ptr)
{
  free (ptr);
}

void*
_realloc_r (struct _reent* impure __attribute__((unused)), void* ptr,
            size_t size)
{
  return realloc (ptr, size);
}

/**
 * @endcond
 */

// ----------------------------------------------------------------------------
// Not really implemented, but present here to avoid including
// weird newlib functions.
/**
 * @cond ignore
 */

struct mallinfo
_mallinfo_r (struct _reent* impure __attribute__((unused)))
{
  abort ();
}

void
_malloc_stats_r (struct _reent* impure __attribute__((unused)))
{
  abort ();
}

size_t
_malloc_usable_size_r (struct _reent* reent __attribute__((unused)),
                       void* ptr __attribute__((unused)))
{
  abort ();
}

int
_mallopt_r (struct _reent* impure __attribute__((unused)),
            int parameter_number __attribute__((unused)),
            int parameter_value __attribute__((unused)))
{
  abort ();
}

void*
_memalign_r (struct _reent* impure __attribute__((unused)),
             size_t align __attribute__((unused)),
             size_t s __attribute__((unused)))
{
  abort ();
}

void*
_pvalloc_r (struct _reent* impure __attribute__((unused)),
            size_t s __attribute__((unused)))
{
  abort ();
}

void*
_valloc_r (struct _reent* impure __attribute__((unused)),
           size_t s __attribute__((unused)))
{
  abort ();
}

/**
 * @endcond
 */

// ----------------------------------------------------------------------------
#endif /* defined(__ARM_EABI__) */

