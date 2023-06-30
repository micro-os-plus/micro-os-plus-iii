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

#ifndef CMSIS_PLUS_RTOS_OS_TYPES_H_
#define CMSIS_PLUS_RTOS_OS_TYPES_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

// This file is included at the end of os-decls.h.

// Include the application specific definitions.
#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <memory>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

// ----------------------------------------------------------------------------

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

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#if defined(OS_INCLUDE_RTOS_CUSTOM_TYPES)
#include <cmsis-plus/rtos/os-app-types.h>
#endif

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_RTOS_OS_TYPES_H_ */
