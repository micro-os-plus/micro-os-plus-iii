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

// ----------------------------------------------------------------------------

using namespace os;

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

      // Used when running on the synthetic POSIX platform.
      class malloc_memory_resource : public memory_resource
      {
      public:

        ~malloc_memory_resource () = default;

      protected:

        virtual void*
        do_allocate (std::size_t bytes,
                     std::size_t alignment __attribute__((unused))) override;

        virtual void
        do_deallocate (void* p, std::size_t bytes __attribute__((unused)),
                       std::size_t alignment __attribute__((unused))) override;
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
          void* p, std::size_t bytes __attribute__((unused)),
          std::size_t alignment __attribute__((unused)))
      {
        // Ignore size and alignment for now.
        std::free (p);
      }

      // ----------------------------------------------------------------------

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

      static memory::malloc_memory_resource malloc_res;

#pragma GCC diagnostic pop

    } /* namespace memory */
  } /* namespace rtos */

  namespace estd
  {
    estd::memory_resource* default_resource __attribute__((weak))
    = &rtos::memory::malloc_res;
  }

  namespace rtos
  {
    namespace memory
    {

      // ----------------------------------------------------------------------

      // The default RTOS memory resource.
      memory_resource* default_resource __attribute__((weak)) = &malloc_res;

      memory_resource*
      set_default_resource (memory_resource* r) noexcept
      {
        trace::printf ("rtos::memory::%s(%p) \n", __func__, r);

        memory_resource* old = default_resource;
        default_resource = r;

        return old;
      }

      memory_resource*
      malloc_resource (void) noexcept
      {
        return &malloc_res;
      }

    // ------------------------------------------------------------------------

    } /* namespace memory */
  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------
