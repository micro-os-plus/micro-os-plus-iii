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
  namespace estd
  {
    // ========================================================================

    [[noreturn]] void
    __throw_bad_alloc (void)
    {
#if defined(__EXCEPTIONS)
      throw std::bad_alloc ();
#else
      trace::printf ("bad_alloc()\n");
      std::abort ();
#endif
    }

    namespace pmr
    {
      // ----------------------------------------------------------------------

      memory_resource*
      set_default_resource (memory_resource* res) noexcept
      {
        trace::printf ("estd::pmr::%s(%p) \n", __func__, res);

        memory_resource* old = default_resource;
        default_resource = res;

        return old;
      }

    // ------------------------------------------------------------------------
    } /* namespace pmr */
  } /* namespace estd */
} /* namespace os */

// ----------------------------------------------------------------------------
