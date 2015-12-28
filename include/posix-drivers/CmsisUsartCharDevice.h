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

#ifndef POSIX_DRIVERS_CMSIS_USART_CHAR_DEVICE_H_
#define POSIX_DRIVERS_CMSIS_USART_CHAR_DEVICE_H_

// ----------------------------------------------------------------------------

#include "posix-io/CharDevice.h"

#include "cmsis_os.h"

#include <cstdarg>
#include <cstdlib>

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

    };

  } /* namespace posix */
} /* namespace os */

#endif /* POSIX_DRIVERS_CMSIS_USART_CHAR_DEVICE_H_ */
