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

#include "posix-drivers/CmsisUsartCharDevice.h"

#include "Driver_USART.h"

#include <cerrno>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    CmsisUsartCharDevice::CmsisUsartCharDevice (
        const char* deviceName, ARM_DRIVER_USART* driver,
        ARM_USART_SignalEvent_t eventCallBack) :
        CharDevice (deviceName)
    {
      fDriver = driver;
      fEventCallBack = eventCallBack;

      fRxSem = nullptr;
      fTxSem = nullptr;
    }

    CmsisUsartCharDevice::~CmsisUsartCharDevice ()
    {
      ;
    }

    // ------------------------------------------------------------------------

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

    int
    CmsisUsartCharDevice::do_vopen (const char* path, int oflag, va_list args)
    {
      if (fRxSem != nullptr)
        {
          errno = EEXIST; // Already opened
          return -1;
        }

      int32_t result;

      do
        {
          fRxSem = osSemaphoreCreate (osSemaphore(fRxSem), 1);
          fTxSem = osSemaphoreCreate (osSemaphore(fTxSem), 1);

          if (fRxSem == NULL || fTxSem == NULL)
            {
              result = ARM_DRIVER_ERROR;
              break;
            }

          osSemaphoreWait (fRxSem, 1);
          osSemaphoreWait (fTxSem, 1);

          if ((result = fDriver->Initialize (fEventCallBack)))
            break;

          if ((result = fDriver->PowerControl (ARM_POWER_FULL)))
            break;

          // default configuration: 8 bits, no parity, 1 stop bit,
          // no flow control, 115200 bps
          if ((result = fDriver->Control (ARM_USART_MODE_ASYNCHRONOUS |
          ARM_USART_DATA_BITS_8 |
          ARM_USART_PARITY_NONE |
          ARM_USART_STOP_BITS_1 |
          ARM_USART_FLOW_CONTROL_NONE,
                                          115200)))
            break;

          /* enable TX output */
          if ((result = fDriver->Control (ARM_USART_CONTROL_TX, 1)))
            break;

          /* enable RX input */
          if ((result = fDriver->Control (ARM_USART_CONTROL_RX, 1)))
            break;

        }
      while (0);

      if (result != ARM_DRIVER_OK)
        {
          errno = ENOSR;
          return -1;
        }
      return 0;
    }

    int
    CmsisUsartCharDevice::do_close (void)
    {
      if (fRxSem == nullptr)
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      osSemaphoreDelete (fRxSem);
      fRxSem = nullptr;

      osSemaphoreDelete (fTxSem);
      fTxSem = nullptr;

      /* disable USART and I/O pins used */
      fDriver->Control (ARM_USART_CONTROL_TX, 0);
      fDriver->Control (ARM_USART_CONTROL_RX, 0);
      fDriver->PowerControl (ARM_POWER_OFF);
      fDriver->Uninitialize ();

      return 0;
    }

    ssize_t
    CmsisUsartCharDevice::do_read (void* buf, std::size_t nbyte)
    {
      ARM_USART_STATUS status;
      ssize_t count = -1;

      if ((fDriver->Receive ((void *) buf, nbyte)) == ARM_DRIVER_OK)
        {
          osSemaphoreWait (fRxSem, osWaitForever);
          status = fDriver->GetStatus ();
          if (!status.rx_framing_error)
            count = fDriver->GetRxCount ();
        }
      fDriver->Control (ARM_USART_ABORT_RECEIVE, 1);

      if (count == -1)
        {
          errno = EIO;
        }
      return count;
    }

    ssize_t
    CmsisUsartCharDevice::do_write (const void* buf, std::size_t nbyte)
    {
      ARM_USART_STATUS status;
      ssize_t count;

      status = fDriver->GetStatus ();
      if (status.tx_busy)
        osSemaphoreWait (fTxSem, osWaitForever);

      if ((fDriver->Send ((void *) buf, nbyte)) == ARM_DRIVER_OK)
        {
          osSemaphoreWait (fTxSem, osWaitForever);
          count = fDriver->GetTxCount ();
        }
      else
        {
          count = -1;
          errno = EIO;
        }
      return count;
    }

    ssize_t
    CmsisUsartCharDevice::do_writev (const struct iovec* iov, int iovcnt)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    CmsisUsartCharDevice::do_vioctl (int request, std::va_list args)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

#if 0
    int
    CmsisUsartCharDevice::do_vfcntl (int cmd, std::va_list args)
      {
        errno = ENOSYS; // Not implemented
        return -1;
      }
#endif

    void
    CmsisUsartCharDevice::eventCallBack (uint32_t event)
    {
      if ((event
          & (ARM_USART_EVENT_RECEIVE_COMPLETE | ARM_USART_EVENT_RX_FRAMING_ERROR))
          || ((event & ARM_USART_EVENT_RX_TIMEOUT) && fDriver->GetRxCount ()))
        osSemaphoreRelease (fRxSem);
      else if (event & ARM_USART_EVENT_TX_COMPLETE)
        osSemaphoreRelease (fTxSem);
    }

#pragma GCC diagnostic pop

  } /* namespace posix */
} /* namespace os */
