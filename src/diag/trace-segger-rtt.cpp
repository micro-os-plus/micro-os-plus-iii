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

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wempty-translation-unit"
#endif

// ----------------------------------------------------------------------------

#if defined(__ARM_EABI__)

// ----------------------------------------------------------------------------

#if defined(TRACE)

#include <cmsis-plus/os-app-config.h>

#if defined(OS_USE_TRACE_SEGGER_RTT)

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/diag/trace.h>

#include <cmsis_device.h>

#include "SEGGER_RTT.h"

// ----------------------------------------------------------------------------

namespace os
{
  namespace trace
  {
    // --------------------------------------------------------------------

    void
    initialize (void)
    {
      SEGGER_RTT_Init ();

      // Clear the SLEEPDEEP.
      // This does not guarantee that the WFI will not prevent
      // the J-Link to read the RTT buffer, but it is the best it
      // can be done at this level.
      SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    }

    // --------------------------------------------------------------------

    ssize_t
    write (const void* buf, std::size_t nbyte)
    {
      if (buf == nullptr || nbyte == 0)
        {
          return 0;
        }

      if (nbyte > BUFFER_SIZE_UP)
        {
          return 0;
        }

      ssize_t ret;

      rtos::interrupts::critical_section ics;
      ret = (ssize_t) SEGGER_RTT_WriteNoLock (0, buf, nbyte);

      return ret;
    }

    void
    flush (void)
    {
      while (_SEGGER_RTT.aUp[0].WrOff != _SEGGER_RTT.aUp[0].RdOff)
        {
          __NOP ();
        }
    }

  } /* namespace trace */
} /* namespace os */

#endif /* defined(OS_USE_TRACE_SEGGER_RTT) */
#endif /* defined(TRACE) */

// ----------------------------------------------------------------------------

#endif /* defined(__ARM_EABI__) */
