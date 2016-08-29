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

#if defined(__ARM_EABI__)

// ----------------------------------------------------------------------------

#include <cmsis-plus/rtos/os.h>
//#include <cmsis-plus/diag/trace.h>
#include <cmsis-plus/estd/malloc.h>
#include <cmsis-plus/estd/memory_resource>

#include <cstdlib>
#include <malloc.h>
#include <string.h>

using namespace os::rtos;

// ----------------------------------------------------------------------------

namespace os
{
  namespace estd
  {
    /**
     * @details
     * The malloc() function allocates size bytes of memory and returns a
     * pointer to the allocated memory. Currently it calls
     * the C function.
     *
     * @note Synchronisation is provided by using a scheduler lock.
     */
    void*
    malloc (std::size_t size) noexcept
    {
      void* p;
        {
          // ----- Begin of critical section ----------------------------------
          scheduler::critical_section cs;

#if 0
          p = ::malloc (size);
#else
          p = get_default_resource ()->allocate (size, 1);
#endif

#if defined(OS_TRACE_LIBC_MALLOC)
          trace::printf ("estd::%s(%d)=%p\n", __func__, size, p);
#endif
          // ----- End of critical section ------------------------------------
        }

      return p;
    }

    /**
     * @details
     * The free() function deallocates the memory allocation pointed
     * to by ptr.
     *
     * If ptr is a NULL pointer, no operation is performed.
     *
     * Currently it calls the C function.
     *
     * @note Synchronisation is provided by using a scheduler lock.
     */
    void
    free (void *ptr) noexcept
    {
        {
          // ----- Begin of critical section ----------------------------------
          scheduler::critical_section cs;

#if defined(OS_TRACE_LIBC_MALLOC)
          trace::printf ("estd::%s(%p)\n", __func__, ptr);
#endif

#if 0
          ::free (ptr);
#else
          get_default_resource ()->deallocate (ptr, 0, 1);
#endif
          // ----- End of critical section ------------------------------------
        }
    }
  } /* namespace estd */
} /* namespace os */

#if 0
extern "C"
  {
    void*
    malloc (size_t size);

    void
    free (void *ptr);
  }
#endif

void*
malloc (size_t size)
{
  return os::estd::malloc (size);
}

void *
calloc (size_t n, size_t elem)
{
  void * mem = os::estd::malloc (n * elem);
  if (mem != NULL)
    memset (mem, 0, n * elem);

  return mem;
}

void *
realloc (void * ptr, size_t size)
{
  void * mem;

  if (ptr == NULL)
    return malloc (size);

  if (size == 0)
    {
      free (ptr);
      return NULL;
    }

#if 0
  /* TODO: There is chance to shrink the chunk if newly requested
   * size is much small */
  if (nano_malloc_usable_size (RCALL ptr) >= size)
  return ptr;
#endif

  mem = malloc (size);
  if (mem != NULL)
    {
      memcpy (mem, ptr, size);
      free (ptr);
    }
  return mem;
}

void
free (void *ptr)
{
  return os::estd::free (ptr);
}

// ----------------------------------------------------------------------------

typedef size_t malloc_size_t;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"

void*
_malloc_r (struct _reent * impure, size_t size)
{
  return os::estd::malloc (size);
}

void *
_calloc_r (struct _reent * impure, size_t n, size_t elem)
{
  return calloc (n, elem);
}

void
_cfree_r (void* impure __attribute__((unused)), void* ptr)
{
  free (ptr);
}

void
_free_r (struct _reent * impure __attribute__((unused)), void* ptr)
{
  free (ptr);
}

struct mallinfo
_mallinfo_r (void* impure)
{
  abort ();
}

void
_malloc_stats_r (void* impure __attribute__((unused)))
{
  abort ();
}

malloc_size_t
_malloc_usable_size_r (void* reent __attribute__((unused)), void * ptr)
{
  abort ();
}

int
_mallopt_r (void* impure __attribute__((unused)),
            int parameter_number __attribute__((unused)),
            int parameter_value __attribute__((unused)))
{
  abort ();
}

void*
_memalign_r (void* impure __attribute__((unused)), size_t align, size_t s)
{
  abort ();
}

void*
_pvalloc_r (void* impure __attribute__((unused)), size_t s)
{
  abort ();
}

void*
_realloc_r (struct _reent * impure __attribute__((unused)), void* ptr,
            size_t size)
{
  return realloc(ptr, size);
}

void*
_valloc_r (void* impure __attribute__((unused)), size_t s)
{
  abort ();
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif /* defined(__ARM_EABI__) */


