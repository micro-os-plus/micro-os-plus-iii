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

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/estd/thread>

// ----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace estd
  {
  // ==========================================================================

#include "thread-cpp.h"

  // ==========================================================================
  } /* namespace estd */
} /* namespace os */

#if defined(OS_HAS_STD_THREADS) && defined(OS_INCLUDE_STD_THREADS)

namespace std
{
// ============================================================================

#include "thread-cpp.h"

// ============================================================================
} /* namespace std */

#endif

// ----------------------------------------------------------------------------
