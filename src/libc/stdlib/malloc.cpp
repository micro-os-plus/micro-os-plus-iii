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

#if defined(__ARM_EABI__)

// ----------------------------------------------------------------------------

#include <cmsis-plus/rtos/os.h>

#include <malloc.h>

// ----------------------------------------------------------------------------

using namespace os;

// ----------------------------------------------------------------------------

// These functions are not thread safe.

void*
malloc (size_t bytes)
{
  void* mem = estd::get_default_resource ()->allocate (std::nothrow, bytes);

#if defined(OS_TRACE_LIBC_MALLOC)
  trace::printf ("::%s(%d)=%mem\n", __func__, bytes, mem);
#endif

  return mem;
}

void*
calloc (size_t bytes, size_t elem)
{
  void* mem = estd::get_default_resource ()->allocate (std::nothrow,
                                                       bytes * elem);

#if defined(OS_TRACE_LIBC_MALLOC)
  trace::printf ("::%s(%u,%u)=%mem\bytes", __func__, bytes, elem, mem);
#endif

  if (mem != NULL)
    memset (mem, 0, bytes * elem);

  return mem;
}

void*
realloc (void* ptr, size_t size)
{
  void* mem;

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
free (void* ptr)
{
#if defined(OS_TRACE_LIBC_MALLOC)
  trace::printf ("::%s(%p)\n", __func__, ptr);
#endif

  return estd::get_default_resource ()->deallocate (std::nothrow, ptr, 0);
}

// ----------------------------------------------------------------------------

// Redirect impure functions to the implementation, to avoid including
// weird newlib functions.

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

// ----------------------------------------------------------------------------

// Not really implemented, but present here to avoid including
// weird newlib functions.

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

// ----------------------------------------------------------------------------

#endif /* defined(__ARM_EABI__) */

