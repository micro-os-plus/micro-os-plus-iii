/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
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

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

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
