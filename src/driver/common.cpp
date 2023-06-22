/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016-2023 Liviu Ionescu. All rights reserved.
 * Copyright (c) 2013-2014 ARM Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

/*
 * The code is inspired by ARM CMSIS Driver_USART.h file, v2.02,
 * and tries to remain functionally close to the CMSIS specifications.
 */

#include <cmsis-plus/driver/common.h>
#include <cassert>

// ----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace driver
  {
    // ----------------------------------------------------------------------

    Base::~Base () noexcept
    {
    }

  // ----------------------------------------------------------------------
  } /* namespace driver */
} /* namespace os */

// ----------------------------------------------------------------------------
