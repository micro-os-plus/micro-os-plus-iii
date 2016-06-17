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

