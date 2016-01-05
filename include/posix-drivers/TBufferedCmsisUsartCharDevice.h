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

#ifndef POSIX_DRIVERS_TBUFFERED_CMSIS_USART_CHAR_DEVICE_H_
#define POSIX_DRIVERS_TBUFFERED_CMSIS_USART_CHAR_DEVICE_H_

// ----------------------------------------------------------------------------

#include "posix-io/CharDevice.h"
#include "posix-drivers/ByteCircularBuffer.h"

#include "diag/trace.h"

#include "cmsis_os.h"
#include "Driver_USART.h"

#include <cstdarg>
#include <cstdlib>
#include <cerrno>
#include <cassert>

// ----------------------------------------------------------------------------

// Note:
// The CMSIS Drivers use static callbacks, that are not able to pass
// back a data pointer, so this should be implemented as a truly static
// function that forwards the event to the instance.

// TODO: (multiline)
// - add flow control on both send & receive
// - add link control (connected/disconnected)
// - cancel pending reads/writes at close
// - add error processing

namespace os
{
  namespace dev
  {
    // ------------------------------------------------------------------------

    template<typename CriticalSection_T>
      class TBufferedCmsisUsartCharDevice : public os::posix::CharDevice
      {
        using CriticalSection = CriticalSection_T;

      public:

        TBufferedCmsisUsartCharDevice (const char* deviceName,
                                       ARM_DRIVER_USART* driver,
                                       ARM_USART_SignalEvent_t eventCallBack,
                                       os::dev::ByteCircularBuffer* rxBuf,
                                       os::dev::ByteCircularBuffer* txBuf =
                                           nullptr);

        virtual
        ~TBufferedCmsisUsartCharDevice ();

        // --------------------------------------------------------------------

        void
        eventCallBack (uint32_t event);

        // --------------------------------------------------------------------

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

        virtual bool
        doIsOpened (void) override;

        // --------------------------------------------------------------------

      private:

        ARM_DRIVER_USART* fDriver; // Pointer to actual CMSIS driver
        ARM_USART_SignalEvent_t fEventCallBack;

        osSemaphoreId fRxSem; //
        osSemaphoreDef(fRxSem);

        osSemaphoreId fTxSem; //
        osSemaphoreDef(fTxSem);

        os::dev::ByteCircularBuffer* fRxBuf;
        os::dev::ByteCircularBuffer* fTxBuf;

        std::size_t fRxCount;bool volatile fTxBusy;

      };

    // ------------------------------------------------------------------------

    template<typename CriticalSection_T>
      TBufferedCmsisUsartCharDevice<CriticalSection_T>::TBufferedCmsisUsartCharDevice (
          const char* deviceName, ARM_DRIVER_USART* driver,
          ARM_USART_SignalEvent_t eventCallBack,
          os::dev::ByteCircularBuffer* rxBuf,
          os::dev::ByteCircularBuffer* txBuf) :
          //
          CharDevice (deviceName), // Construct parent.
          //
          fDriver (driver), //
          fEventCallBack (eventCallBack), //
          fRxSem (nullptr), //
          fTxSem (nullptr), //
          fRxBuf (rxBuf), //
          fTxBuf (txBuf), //
          fRxCount (0), //
          fTxBusy (false) //
      {
        assert(rxBuf != nullptr);
        // txBuf may be null.
      }

    template<typename CriticalSection_T>
      TBufferedCmsisUsartCharDevice<CriticalSection_T>::~TBufferedCmsisUsartCharDevice ()
      {
        fRxSem = nullptr;
        fTxSem = nullptr;
      }

    // ------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    template<typename CriticalSection_T>
      int
      TBufferedCmsisUsartCharDevice<CriticalSection_T>::do_vopen (
          const char* path, int oflag, std::va_list args)
      {
        if (fRxSem != nullptr)
          {
            errno = EEXIST; // Already opened
            return -1;
          }

        int32_t result;

        do
          {
            // Start disabled, the first wait will block.
            fRxSem = osSemaphoreCreate (osSemaphore(fRxSem), 0);
            fTxSem = osSemaphoreCreate (osSemaphore(fTxSem), 0);

            if ((fRxSem == nullptr) || (fTxSem == nullptr))
              {
                result = ARM_DRIVER_ERROR;
                break;
              }

            // Clear buffers.
            fRxBuf->clear ();
            if (fTxBuf != nullptr)
              {
                fTxBuf->clear ();
              }

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

            uint8_t* pbuf;
            std::size_t nbyte = fRxBuf->getBackContiguousBuffer (&pbuf);

            result = fDriver->Receive (pbuf, nbyte);
            if (result != ARM_DRIVER_OK)
              break;
          }
        while (false); // Actually NOT a loop, just a sequence of ifs!

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

    template<typename CriticalSection_T>
      bool
      TBufferedCmsisUsartCharDevice<CriticalSection_T>::doIsOpened (void)
      {
        return (fRxSem != nullptr);
      }

    template<typename CriticalSection_T>
      int
      TBufferedCmsisUsartCharDevice<CriticalSection_T>::do_close (void)
      {
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

    template<typename CriticalSection_T>
      ssize_t
      TBufferedCmsisUsartCharDevice<CriticalSection_T>::do_read (
          void* buf, std::size_t nbyte)
      {
        // TODO: implement cases when 0 must be returned
        // (disconnects, timeouts).
        while (true)
          {
            std::size_t count;
              {
                CriticalSection cs; // -----

                count = fRxBuf->popFront (static_cast<uint8_t*> (buf), nbyte);
              }
            if (count > 0)
              {
                // Actual number of chars received in buffer.
                return count;
              }

            // Block and wait for bytes to arrive.
            osSemaphoreWait (fRxSem, osWaitForever);
          }
      }

    template<typename CriticalSection_T>
      ssize_t
      TBufferedCmsisUsartCharDevice<CriticalSection_T>::do_write (
          const void* buf, std::size_t nbyte)
      {
        std::size_t count;

        if (fTxBuf != nullptr)
          {
            count = 0;
              {
                CriticalSection cs; // -----

                if (fTxBuf->isBelowHighWaterMark ())
                  {
                    // If there is more space in the buffer, try to fill it.
                    count = fTxBuf->pushBack (static_cast<const uint8_t*> (buf),
                                              nbyte);
                  }
              }
            while (true)
              {
                ARM_USART_STATUS status;
                  {
                    CriticalSection cs; // -----

                    status = fDriver->GetStatus ();
                  }
                // We use our own tx busy flag because the ARM driver's flag
                // may become free between transmissions.
                if (!fTxBusy)
                  {
                    uint8_t* pbuf;
                    std::size_t nb;
                      {
                        CriticalSection cs; // -----

                        nb = fTxBuf->getFrontContiguousBuffer (&pbuf);
                      }
                    if (nb > 0)
                      {
                        if (fDriver->Send (pbuf, nb) != ARM_DRIVER_OK)
                          {
                            errno = EIO;
                            return -1;
                          }
                        fTxBusy = true;
                      }
                  }

                bool isBelowHWM;
                  {
                    CriticalSection cs; // -----

                    isBelowHWM = fTxBuf->isBelowHighWaterMark ();
                  }
                if (count == nbyte)
                  {
                    return nbyte;
                  }

                // Block and wait for buffer to be freed.
                osSemaphoreWait (fTxSem, osWaitForever);

                if (count < nbyte)
                  {
                    CriticalSection cs;  // -----

                    std::size_t n;
                    // If there is more space in the buffer, try to fill it.
                    n = fTxBuf->pushBack (
                        static_cast<const uint8_t*> (buf) + count,
                        nbyte - count);
                    count += n;
                  }
              }
          }
        else
          {
            // Do not use a transmit buffer, send directly from the user buffer.
            // Wait while transmitting.
            ARM_USART_STATUS status;
            status = fDriver->GetStatus ();
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
          }
        // Actual number of bytes transmitted from buffer.
        return count;
      }

#if 0
    template<typename CriticalSection_T>
    ssize_t
    TBufferedCmsisUsartCharDevice<CriticalSection_T>::do_writev (const struct iovec* iov, int iovcnt)
      {
        errno = ENOSYS; // Not implemented
        return -1;
      }

    template<typename CriticalSection_T>
    int
    TBufferedCmsisUsartCharDevice<CriticalSection_T>::do_vioctl (int request, std::va_list args)
      {
        errno = ENOSYS; // Not implemented
        return -1;
      }

    template<typename CriticalSection_T>
    int
    TBufferedCmsisUsartCharDevice<CriticalSection_T>::do_vfcntl (int cmd, std::va_list args)
      {
        errno = ENOSYS; // Not implemented
        return -1;
      }
#endif

    // ------------------------------------------------------------------------

    // This function is called by the CMSIS driver in an interrupt context.

    template<typename CriticalSection_T>
      void
      TBufferedCmsisUsartCharDevice<CriticalSection_T>::eventCallBack (
          uint32_t event)
      {
        if ((event
            & (ARM_USART_EVENT_RECEIVE_COMPLETE
                | ARM_USART_EVENT_RX_FRAMING_ERROR | ARM_USART_EVENT_RX_TIMEOUT)))
          {
            // TODO: process errors and timeouts
            std::size_t tmpCount = fDriver->GetRxCount ();
            std::size_t count = tmpCount - fRxCount;
            fRxCount = tmpCount;
            std::size_t adjust = fRxBuf->advanceBack (count);
            assert(count == adjust);

            if (event & ARM_USART_EVENT_RECEIVE_COMPLETE)
              {
                uint8_t* pbuf;
                std::size_t nbyte = fRxBuf->getBackContiguousBuffer (&pbuf);
                if (nbyte == 0)
                  {
                    // Overwrite the last byte, but keep the driver in
                    // receive mode continuously.
                    fRxBuf->retreatBack ();
                    nbyte = fRxBuf->getBackContiguousBuffer (&pbuf);
                  }
                assert(nbyte > 0);

                // Read as much as we can.
                int32_t status;
                status = fDriver->Receive (pbuf, nbyte);
                // TODO: implement error processing.
                assert(status == ARM_DRIVER_OK);

                fRxCount = 0;
              }
            if (count > 0)
              {
                // Immediately wake up, do not wait to reach any water mark.
                osSemaphoreRelease (fRxSem);
              }
          }
        if (event & ARM_USART_EVENT_TX_COMPLETE)
          {
            if (fTxBuf != nullptr)
              {
                std::size_t count = fDriver->GetTxCount ();
                std::size_t adjust = fTxBuf->advanceFront (count);
                assert(count == adjust);

                uint8_t* pbuf;
                std::size_t nbyte = fTxBuf->getFrontContiguousBuffer (&pbuf);
                if (nbyte > 0)
                  {
                    int32_t status;
                    status = fDriver->Send (pbuf, nbyte);
                    // TODO: implement error processing
                    assert(status == ARM_DRIVER_OK);
                  }
                else
                  {
                    fTxBusy = false;
                  }
                if (fTxBuf->isBelowLowWaterMark ())
                  {
                    // Wake up thread, to come and send more bytes.
                    osSemaphoreRelease (fTxSem);
                  }
              }
            else
              {
                // No buffer, wake up the thread to return from write().
                osSemaphoreRelease (fTxSem);
              }
          }
      }

#pragma GCC diagnostic pop

  } /* namespace dev */
} /* namespace os */

#endif /* POSIX_DRIVERS_TBUFFERED_CMSIS_USART_CHAR_DEVICE_H_ */
