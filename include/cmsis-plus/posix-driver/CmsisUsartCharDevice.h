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

#ifndef CMSIS_PLUS_POSIX_DRIVER_CMSIS_USART_CHAR_DEVICE_H_
#define CMSIS_PLUS_POSIX_DRIVER_CMSIS_USART_CHAR_DEVICE_H_

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#include <cmsis-plus/posix-io/CharDevice.h>

#include "cmsis-plus/legacy/cmsis_os.h"

#include <cstdarg>
#include <cstdlib>

#define BUFF_LEN 200

// ----------------------------------------------------------------------------

// Forward definitions to avoid "Driver_USART.h"
extern "C"
{
  typedef struct _ARM_DRIVER_USART const ARM_DRIVER_USART;
  typedef void
  (*ARM_USART_SignalEvent_t) (uint32_t event);
}

// ----------------------------------------------------------------------------

// The CMSIS Drivers use static callbacks, that are not able to pass
// back a data pointer, so this should be implemented as a truly static
// function that forwards the event to the instance.

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class CmsisUsartCharDevice : public CharDevice
    {
    public:

      CmsisUsartCharDevice (const char* deviceName, ARM_DRIVER_USART* driver,
                            ARM_USART_SignalEvent_t eventCallBack);

      virtual
      ~CmsisUsartCharDevice ();

      // ----------------------------------------------------------------------

      void
      eventCallBack (uint32_t event);

      // ----------------------------------------------------------------------

    protected:

      virtual int
      do_vopen (const char* path, int oflag, std::va_list args) override;

      virtual int
      do_close (void) override;

      virtual ssize_t
      do_read (void* buf, std::size_t nbyte) override;

      virtual ssize_t
      do_write (const void* buf, std::size_t nbyte) override;

#if 0
      virtual ssize_t
      do_writev (const struct iovec* iov, int iovcnt) override;

      virtual int
      do_vioctl (int request, std::va_list args) override;

      virtual int
      do_vfcntl (int cmd, va_list args) override;
#endif

      // ----------------------------------------------------------------------

    private:

      ARM_DRIVER_USART* fDriver; // Pointer to actual CMSIS driver
      ARM_USART_SignalEvent_t fEventCallBack;

      osSemaphoreId fRxSem; //
      osSemaphoreDef(fRxSem);

      osSemaphoreId fTxSem; //
      osSemaphoreDef(fTxSem);

      uint8_t fBuffer[BUFF_LEN];
      std::size_t fCntIn, fCntOut;

    };

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_DRIVER_CMSIS_USART_CHAR_DEVICE_H_ */
