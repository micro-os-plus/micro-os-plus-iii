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

#include <cmsis-plus/diag/trace.h>
#include <cmsis-plus/estd/memory_resource>
#include <new>
#include <cstdlib>

// ----------------------------------------------------------------------------

using namespace os::estd;

namespace
{
  // ==========================================================================

  // Standard
  class new_delete_memory_resource : public memory_resource
  {
  public:

    ~new_delete_memory_resource () = default;

  protected:

    virtual void*
    do_allocate (size_t bytes, size_t alignment __attribute__((unused))) override
    {
      // Ignore alignment for now.
      return ::operator new (bytes);
    }

    virtual void
    do_deallocate (void* p, size_t bytes __attribute__((unused)),
                   size_t alignment __attribute__((unused))) override
    {
      // Ignore size and alignment for now.
      ::operator delete (p);
    }
  };

  // ==========================================================================

  // Standard
  class null_memory_resource : public memory_resource
  {
  public:

    ~null_memory_resource () = default;

  protected:

    virtual void*
    do_allocate (std::size_t bytes __attribute__((unused)),
                 std::size_t alignment __attribute__((unused))) override
    {
      __throw_bad_alloc (ENOSYS, "No dynamic allocations allowed.");
    }

    virtual void
    do_deallocate (void* p __attribute__((unused)),
                   std::size_t bytes __attribute__((unused)),
                   std::size_t alignment __attribute__((unused))) override
    {
      ;
    }
  };

  // --------------------------------------------------------------------------

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

  static new_delete_memory_resource new_delete_res;
  static null_memory_resource null_res;

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
}

namespace os
{
  namespace estd
  {
    // ========================================================================

    void
    __throw_bad_alloc (int ev
#if defined(__EXCEPTIONS)
                       __attribute__((unused))
#endif
                       ,
                       const char* what_arg
#if defined(__EXCEPTIONS)
                       __attribute__((unused))
#endif
                       )
    {
#if defined(__EXCEPTIONS)
      throw std::bad_alloc ();
#else
      trace::printf ("bad_alloc(), %d, %s\n", ev, what_arg);
      std::abort ();
#endif
    }

    // ------------------------------------------------------------------------

    memory_resource*
    new_delete_resource (void) noexcept
    {
      return &new_delete_res;
    }

    memory_resource*
    null_memory_resource (void) noexcept
    {
      return &null_res;
    }

    // ------------------------------------------------------------------------

    // The default memory resource is the LIBC malloc().
    // The actual definition is located in rtos/os-memory.cpp.
    // memory_resource* default_resource __attribute__((weak)) = &malloc_res;

    memory_resource*
    set_default_resource (memory_resource* r) noexcept
    {
      trace::printf ("estd::%s(%p) \n", __func__, r);

      memory_resource* old = default_resource;
      default_resource = r;

      return old;
    }

    // ========================================================================

    memory_resource::~memory_resource ()
    {
      ;
    }

    bool
    memory_resource::do_is_equal (memory_resource const &other) const noexcept
    {
      return &other == this;
    }

  // ------------------------------------------------------------------------
  } /* namespace estd */
} /* namespace os */

// ----------------------------------------------------------------------------
