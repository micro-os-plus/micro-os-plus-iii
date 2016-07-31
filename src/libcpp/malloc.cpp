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

#include <cmsis-plus/iso/malloc.h>
#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/diag/trace.h>

#include <cstdlib>

using namespace os::rtos;

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

          p = ::malloc (size);

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
          return ::free (ptr);
          // ----- End of critical section ------------------------------------
        }
    }
  } /* namespace estd */
} /* namespace os */
