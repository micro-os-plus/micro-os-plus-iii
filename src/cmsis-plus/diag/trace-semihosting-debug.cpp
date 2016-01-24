/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
 *
 * µOS++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 3.
 *
 * µOS++ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#if defined(TRACE)
#if defined(OS_USE_TRACE_SEMIHOSTING_DEBUG)

#include <cmsis-plus/diag/trace.h>

// ----------------------------------------------------------------------------

#if defined(OS_DEBUG_SEMIHOSTING_FAULTS)
#error "Cannot debug semihosting using semihosting trace; use OS_USE_TRACE_ITM"
#endif

#include <cmsis-plus/arm/semihosting.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace trace
  {
    // ------------------------------------------------------------------------

    void
    initialize (void)
    {
      // For semihosting, no inits are required.
    }

    // ------------------------------------------------------------------------

    // Semihosting is the other output channel that can be used for the trace
    // messages. It comes in two flavours: STDOUT and DEBUG. The STDOUT channel
    // is the equivalent of the stdout in POSIX and in most cases it is
    // forwarded to the GDB server stdout stream. The debug channel is a
    // separate channel. STDOUT is buffered, so nothing is displayed until
    // a \n; DEBUG is not buffered, but can be slow.
    //
    // Choosing between semihosting stdout and debug depends on the capabilities
    // of your GDB server, and also on specific needs. It is recommended to test
    // DEBUG first, and if too slow, try STDOUT.
    //
    // The JLink GDB server fully support semihosting, and both configurations
    // are available; to activate it, use "monitor semihosting enable" or check
    // the corresponding button in the JLink Debugging plug-in.
    // In OpenOCD, support for semihosting can be enabled using
    // "monitor arm semihosting enable".
    //
    // Note: Applications built with semihosting output active normally cannot
    // be executed without the debugger connected and active, since they use
    // BKPT to communicate with the host. However, with a carefully written
    // HardFault_Handler, the semihosting BKPT calls can be processed, making
    // possible to run semihosting applications as standalone, without being
    // terminated with hardware faults.

    // ------------------------------------------------------------------------

#if !defined(OS_INTEGER_TRACE_SEMIHOSTING_TMP_ARRAY_SIZE)
#define OS_INTEGER_TRACE_SEMIHOSTING_TMP_ARRAY_SIZE  (16)
#endif

    ssize_t
    write (const void* buf, std::size_t nbyte)
    {
      const char* cbuf = (const char*) buf;

      // Since the single character debug channel is quite slow, try to
      // optimise and send a null terminated string, if possible.
      if (cbuf[nbyte] == '\0')
        {
          // send string
          call_host (SEMIHOSTING_SYS_WRITE0, (void*) cbuf);
        }
      else
        {
          // If not, use a local buffer to speed things up.
          // For re-entrance, this bugger must be allocated on the stack,
          // so be cautious with the size.
          char tmp[OS_INTEGER_TRACE_SEMIHOSTING_TMP_ARRAY_SIZE];
          size_t togo = nbyte;
          while (togo > 0)
            {
              unsigned int n = ((togo < sizeof(tmp)) ? togo : sizeof(tmp));
              unsigned int i = 0;
              for (; i < n; ++i, ++cbuf)
                {
                  tmp[i] = *cbuf;
                }
              tmp[i] = '\0';

              call_host (SEMIHOSTING_SYS_WRITE0, (void*) tmp);

              togo -= n;
            }
        }

      // All bytes written.
      return (ssize_t) nbyte;
    }

  } /* namespace trace */
} /* namespace os */

#endif /* defined(OS_USE_TRACE_SEMIHOSTING_DEBUG) */
#endif /* defined(TRACE) */

// ----------------------------------------------------------------------------

