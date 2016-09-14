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

#ifndef CMSIS_PLUS_RTOS_OS_TYPES_H_
#define CMSIS_PLUS_RTOS_OS_TYPES_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

// This file is included at the end of os-decls.h.

// Include the application specific definitions.
#include <cmsis-plus/os-app-config.h>
#include <memory>

namespace os
{
  namespace rtos
  {
    namespace memory
    {

#if !defined(OS_INCLUDE_RTOS_CUSTOM_ALLOCATOR) || defined(__DOXYGEN__)

      /**
       * @brief Type of allocator used by the system objects.
       *  Must be stateless.
       * @ingroup cmsis-plus-rtos-memres
       * @tparam T Type of elements to be allocated.
       */
      template<typename T>
        using allocator = allocator_stateless_default_resource<T>;

#endif

    } /* namespace memory */
  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#if defined(OS_INCLUDE_RTOS_CUSTOM_TYPES)
#include <cmsis-plus/rtos/os-app-types.h>
#endif

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_TYPES_H_ */
