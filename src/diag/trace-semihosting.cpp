/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
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

#if defined(OS_USE_TRACE_SEMIHOSTING_DEBUG) || defined(OS_USE_TRACE_SEMIHOSTING_STDOUT)

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
      // ----------------------------------------------------------------------

      void
      initialize (void)
      {
        // For semihosting, no inits are required.
      }

      // ----------------------------------------------------------------------

      // Semihosting is another output channel that can be used for the trace
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

      // ----------------------------------------------------------------------

#if defined(OS_USE_TRACE_SEMIHOSTING_DEBUG)

#if !defined(OS_INTEGER_TRACE_SEMIHOSTING_BUFF_ARRAY_SIZE)
#define OS_INTEGER_TRACE_SEMIHOSTING_BUFF_ARRAY_SIZE  (16)
#endif

      ssize_t
      write (const void* buf, std::size_t nbyte)
      {
        if (buf == nullptr || nbyte == 0)
          {
            return 0;
          }

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
            char tmp[OS_INTEGER_TRACE_SEMIHOSTING_BUFF_ARRAY_SIZE];
            size_t togo = nbyte;
            while (togo > 0)
              {
                std::size_t n = ((togo < sizeof(tmp)) ? togo : sizeof(tmp));
                std::size_t i = 0;
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

#elif defined(OS_USE_TRACE_SEMIHOSTING_STDOUT)

    ssize_t
    write (const void* buf, std::size_t nbyte)
      {
      if (buf == nullptr || nbyte == 0)
        {
          return 0;
        }

      static int handle; // STATIC!

        void* block[3];
        int ret;

        if (handle == 0)
          {
            // On the very first call get the file handle from the host.

            // Special filename for stdin/out/err.
            block[0] = (void*) ":tt";
            block[1] = (void*) 4;// mode "w"
            // Length of ":tt", except null terminator.
            block[2] = (void*) (sizeof(":tt") - 1);

            ret = call_host (SEMIHOSTING_SYS_OPEN, (void*) block);
            if (ret == -1)
              {
                return -1;
              }

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
          {
            return -1;
          }

        // The compliant way of returning errors.
        if (ret == (int) nbyte)
          {
            return -1;
          }

        // Return the number of bytes written.
        return (ssize_t) (nbyte) - (ssize_t) ret;
      }

#endif /* defined(OS_USE_TRACE_SEMIHOSTING_STDOUT) */

  } /* namespace trace */
} /* namespace os */

#endif /* defined(OS_USE_TRACE_SEMIHOSTING_DEBUG) || defined(OS_USE_TRACE_SEMIHOSTING_STDOUT) */
#endif /* defined(TRACE) */

// ----------------------------------------------------------------------------

#endif /* defined(__ARM_EABI__) */

// ----------------------------------------------------------------------------

