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
#if defined(OS_USE_TRACE_SEMIHOSTING_STDOUT)

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

    ssize_t
    write (const void* buf, std::size_t nbyte)
    {
      static int handle; // STATIC!

      void* block[3];
      int ret;

      if (handle == 0)
        {
          // On the very first call get the file handle from the host.

          // Special filename for stdin/out/err.
          block[0] = (void*) ":tt";
          block[1] = (void*) 4; // mode "w"
          // Length of ":tt", except null terminator.
          block[2] = (void*) (sizeof(":tt") - 1);

          ret = call_host (SEMIHOSTING_SYS_OPEN, (void*) block);
          if (ret == -1)
            return -1;

          handle = ret;
        }

      block[0] = (void*) handle;
      block[1] = (void*) buf;
      block[2] = (void*) nbyte;
      // Send character array to host file/device.
      ret = call_host (SEMIHOSTING_SYS_WRITE, (void*) block);
      // This call returns the number of bytes NOT written (0 if all ok).

      // -1 is not a legal value, but SEGGER seems to return it
      if (ret == -1)
        return -1;

      // The compliant way of returning errors.
      if (ret == (int) nbyte)
        return -1;

      // Return the number of bytes written.
      return (ssize_t) (nbyte) - (ssize_t) ret;
    }

  } /* namespace trace */
} /* namespace os */

#endif /* defined(OS_USE_TRACE_SEMIHOSTING_STDOUT) */
#endif /* defined(TRACE) */

// ----------------------------------------------------------------------------

