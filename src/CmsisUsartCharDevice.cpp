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
 *
 * Contributors:
 *   - Lix Paulian
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
        ARM_USART_SignalEvent_t callBack) :
        CharDevice (deviceName)
    {
      fDriver = driver;
      fEventCallBack = callBack;

      fRxSem = nullptr;
      fTxSem = nullptr;

      fCntIn = 0;
      fCntOut = 0;
    }

    CmsisUsartCharDevice::~CmsisUsartCharDevice ()
    {
      fRxSem = nullptr;
      fTxSem = nullptr;
    }

    // ------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    int
    CmsisUsartCharDevice::do_vopen (const char* path, int oflag,
                                    std::va_list args)
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

          if ((fRxSem == nullptr) || (fTxSem == nullptr))
            {
              result = ARM_DRIVER_ERROR;
              break;
            }

          // These are present here for some obscure reasons related to inits.
          // Might not be the best solution.
          osSemaphoreWait (fRxSem, 1);
          osSemaphoreWait (fTxSem, 1);

          fCntIn = 0;
          fCntOut = 0;

          result = fDriver->Initialize (fEventCallBack);
          if (result != ARM_DRIVER_OK)
            break;

          result = fDriver->PowerControl (ARM_POWER_FULL);
          if (result != ARM_DRIVER_OK)
            break;

          // Default configuration: 8 bits, no parity, 1 stop bit,
          // no flow control, 115200 bps.
          result = fDriver->Control (ARM_USART_MODE_ASYNCHRONOUS |
          ARM_USART_DATA_BITS_8 |
          ARM_USART_PARITY_NONE |
          ARM_USART_STOP_BITS_1 |
          ARM_USART_FLOW_CONTROL_NONE,
                                     115200);
          if (result != ARM_DRIVER_OK)
            break;

          // Enable TX output.
          result = fDriver->Control (ARM_USART_CONTROL_TX, 1);
          if (result != ARM_DRIVER_OK)
            break;

          // Enable RX input.
          result = fDriver->Control (ARM_USART_CONTROL_RX, 1);
          if (result != ARM_DRIVER_OK)
            break;

          result = fDriver->Receive (fBuffer, BUFF_LEN);
          if (result != ARM_DRIVER_OK)
            break;
        }
      while (false);

      if (result != ARM_DRIVER_OK)
        {
          fDriver->PowerControl (ARM_POWER_OFF);
          fDriver->Uninitialize ();

          errno = ENOSR;
          return -1;
        }

      // Return POSIX OK.
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

      // Disable USART and I/O pins used.
      fDriver->Control (ARM_USART_CONTROL_TX, 0);
      fDriver->Control (ARM_USART_CONTROL_RX, 0);
      fDriver->PowerControl (ARM_POWER_OFF);
      fDriver->Uninitialize ();

      // Return POSIX OK.
      return 0;
    }

    ssize_t
    CmsisUsartCharDevice::do_read (void* buf, std::size_t nbyte)
    {
      ssize_t count = 0;
      uint8_t *ubuf = (uint8_t *) buf;
      ARM_USART_STATUS status;

      while (fCntOut == ((fCntIn = fDriver->GetRxCount ())))
        {
          osSemaphoreWait (fRxSem, osWaitForever);  // wait for an event

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"
          status = fDriver->GetStatus ();
#pragma GCC diagnostic pop
          if (status.rx_framing_error)  // other error checking should be added here
            {
              errno = EIO;
              return -1;
            }
        }

      for (; fCntIn != fCntOut; count++, nbyte--)
        {
          if (!nbyte)
            break;      // caller's buffer full
          *ubuf++ = fBuffer[fCntOut++];
          if (fCntOut >= BUFF_LEN)
            fCntOut = 0;
        }

      // Actual number of chars received in buffer.
      return count;
    }

    ssize_t
    CmsisUsartCharDevice::do_write (const void* buf, std::size_t nbyte)
    {
      ssize_t count;

      ARM_USART_STATUS status;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"
      status = fDriver->GetStatus ();
#pragma GCC diagnostic pop
      if (status.tx_busy)
        {
          osSemaphoreWait (fTxSem, osWaitForever);
        }

      if ((fDriver->Send (buf, nbyte)) == ARM_DRIVER_OK)
        {
          osSemaphoreWait (fTxSem, osWaitForever);
          count = fDriver->GetTxCount ();
        }
      else
        {
          count = -1;
          errno = EIO;
        }

      // Actual number of chars transmitted from buffer.
      return count;
    }

#if 0
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

    int
    CmsisUsartCharDevice::do_vfcntl (int cmd, std::va_list args)
      {
        errno = ENOSYS; // Not implemented
        return -1;
      }
#endif

    // ------------------------------------------------------------------------

    // This function is called by the CMSIS driver in an interrupt context.

    void
    CmsisUsartCharDevice::eventCallBack (uint32_t event)
    {
      if ((event
          & (ARM_USART_EVENT_RECEIVE_COMPLETE | ARM_USART_EVENT_RX_FRAMING_ERROR
              | ARM_USART_EVENT_RX_TIMEOUT)))
        {
          if (event & ARM_USART_EVENT_RECEIVE_COMPLETE)
            fDriver->Receive (fBuffer, BUFF_LEN); // re-initialize to buffer start
          else
            osSemaphoreRelease (fRxSem);
        }
      if (event & ARM_USART_EVENT_TX_COMPLETE)
        {
          osSemaphoreRelease (fTxSem);
        }
    }

#pragma GCC diagnostic pop

  } /* namespace posix */
} /* namespace os */
