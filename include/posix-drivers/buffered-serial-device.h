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

#ifndef POSIX_DRIVERS_BUFFERED_SERIAL_DEVICE_H_
#define POSIX_DRIVERS_BUFFERED_SERIAL_DEVICE_H_

// ----------------------------------------------------------------------------

#include "posix-io/CharDevice.h"
#include "posix-drivers/ByteCircularBuffer.h"
#include "cmsis-plus/drivers/serial.h"

#include "diag/trace.h"

#include "cmsis_os.h"

#include <cstdarg>
#include <cstdlib>
#include <cerrno>
#include <cassert>

// ----------------------------------------------------------------------------

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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    template<typename Cs_T>
      class Buffered_serial_device : public os::posix::CharDevice
      {
        using Critical_section = Cs_T;

      public:

        Buffered_serial_device (const char* device_name,
                                os::cmsis::driver::Serial* driver,
                                os::dev::ByteCircularBuffer* rx_buf,
                                os::dev::ByteCircularBuffer* tx_buf);

        virtual
        ~Buffered_serial_device ();

        // --------------------------------------------------------------------

        // Static function called by the CMSIS driver in an
        // interrupt context.

        static void
        signal_event (Buffered_serial_device* object, uint32_t event);

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

        // Pointer to actual CMSIS-like serial driver (usart or usb cdc acm)
        os::cmsis::driver::Serial* driver_;

        osSemaphoreId rx_sem_; //
        osSemaphoreDef(rx_sem_);

        osSemaphoreId tx_sem_; //
        osSemaphoreDef(tx_sem_);

        os::dev::ByteCircularBuffer* rx_buf_;
        os::dev::ByteCircularBuffer* tx_buf_;

        std::size_t rx_count_; //
        bool volatile tx_busy_;
        // Padding!

      };

#pragma GCC diagnostic pop

    // ------------------------------------------------------------------------

    template<typename Cs_T>
      Buffered_serial_device<Cs_T>::Buffered_serial_device (
          const char* deviceName, //
          os::cmsis::driver::Serial* driver,
          os::dev::ByteCircularBuffer* rx_buf,
          os::dev::ByteCircularBuffer* tx_buf) :
          //
          CharDevice (deviceName), // Construct parent.
          //
          driver_ (driver), //
          rx_sem_ (nullptr), //
          tx_sem_ (nullptr), //
          rx_buf_ (rx_buf), //
          tx_buf_ (tx_buf), //
          rx_count_ (0), //
          tx_busy_ (false) //
      {
        assert(rx_buf != nullptr);

        // Do not check the same for tx_buf, it may be null.

        driver_->register_callback (
            reinterpret_cast<os::cmsis::driver::signal_event_t> (signal_event),
            this);
      }

    template<typename Cs_T>
      Buffered_serial_device<Cs_T>::~Buffered_serial_device ()
      {
        rx_sem_ = nullptr;
        tx_sem_ = nullptr;

      }

    // ------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    template<typename Cs_T>
      int
      Buffered_serial_device<Cs_T>::do_vopen (const char* path, int oflag,
                                              std::va_list args)
      {
        if (rx_sem_ != nullptr)
          {
            errno = EEXIST; // Already opened
            return -1;
          }

        int32_t result;

        do
          {
            // Start disabled, the first wait will block.
            rx_sem_ = osSemaphoreCreate (osSemaphore(rx_sem_), 0);
            tx_sem_ = osSemaphoreCreate (osSemaphore(tx_sem_), 0);

            if ((rx_sem_ == nullptr) || (tx_sem_ == nullptr))
              {
                result = os::cmsis::driver::ERROR;
                break;
              }

            // Clear buffers.
            rx_buf_->clear ();
            if (tx_buf_ != nullptr)
              {
                tx_buf_->clear ();
              }

            // Default configuration: 8 bits, no parity, 1 stop bit,
            // no flow control, 115200 bps.
            result = driver_->configure (
                os::cmsis::driver::serial::MODE_ASYNCHRONOUS
                    | os::cmsis::driver::serial::DATA_BITS_8
                    | os::cmsis::driver::serial::PARITY_NONE
                    | os::cmsis::driver::serial::STOP_BITS_1
                    | os::cmsis::driver::serial::FLOW_CONTROL_NONE,
                115200);
            // assert(result == os::cmsis::driver::RETURN_OK);
            if (result != os::cmsis::driver::RETURN_OK)
              break;

            // Enable TX output.
            result = driver_->control (os::cmsis::driver::serial::ENABLE_TX);
            if (result != os::cmsis::driver::RETURN_OK)
              break;

            // Enable RX input.
            result = driver_->control (os::cmsis::driver::serial::ENABLE_RX);
            if (result != os::cmsis::driver::RETURN_OK)
              break;

            uint8_t* pbuf;
            std::size_t nbyte = rx_buf_->getBackContiguousBuffer (&pbuf);

            result = driver_->receive (pbuf, nbyte);
            if (result != os::cmsis::driver::RETURN_OK)
              break;
          }
        while (false); // Actually NOT a loop, just a sequence of ifs!

        if (result != os::cmsis::driver::RETURN_OK)
          {
            errno = ENOSR;
            return -1;
          }

        // Return POSIX OK.
        return 0;
      }

    template<typename Cs_T>
      bool
      Buffered_serial_device<Cs_T>::doIsOpened (void)
      {
        return (rx_sem_ != nullptr);
      }

    template<typename Cs_T>
      int
      Buffered_serial_device<Cs_T>::do_close (void)
      {
        // Abort pending reads.
        os::cmsis::driver::return_t ret;
        ret = driver_->control (os::cmsis::driver::serial::ABORT_RECEIVE);
        assert(ret == os::cmsis::driver::RETURN_OK);

        // TODO: should we wait for write to complete? what if flow
        // control prevents this?
        // Abort pending writes.
        // driver_->control (os::cmsis::driver::serial::ABORT_SEND);

        // Disable transmitter and receiver.
        ret = driver_->control (os::cmsis::driver::serial::DISABLE_TX);
        assert(ret == os::cmsis::driver::RETURN_OK);

        ret = driver_->control (os::cmsis::driver::serial::DISABLE_RX);
        assert(ret == os::cmsis::driver::RETURN_OK);
        ret = driver_->control (os::cmsis::driver::serial::DISABLE_BREAK);
        assert(ret == os::cmsis::driver::RETURN_OK);

        osSemaphoreDelete (rx_sem_);
        rx_sem_ = nullptr;

        osSemaphoreDelete (tx_sem_);
        tx_sem_ = nullptr;

        // Return POSIX OK.
        return 0;
      }

    template<typename Cs_T>
      ssize_t
      Buffered_serial_device<Cs_T>::do_read (void* buf, std::size_t nbyte)
      {
        // TODO: implement cases when 0 must be returned
        // (disconnects, timeouts).
        while (true)
          {
            std::size_t count;
              {
                Critical_section cs; // -----

                count = rx_buf_->popFront (static_cast<uint8_t*> (buf), nbyte);
              }
            if (count > 0)
              {
                // Actual number of chars received in buffer.
                return count;
              }

            // Block and wait for bytes to arrive.
            osSemaphoreWait (rx_sem_, osWaitForever);
          }
      }

    template<typename Cs_T>
      ssize_t
      Buffered_serial_device<Cs_T>::do_write (const void* buf,
                                              std::size_t nbyte)
      {
        std::size_t count;

        if (tx_buf_ != nullptr)
          {
            count = 0;
              {
                Critical_section cs; // -----

                if (tx_buf_->isBelowHighWaterMark ())
                  {
                    // If there is more space in the buffer, try to fill it.
                    count = tx_buf_->pushBack (
                        static_cast<const uint8_t*> (buf), nbyte);
                  }
              }
            while (true)
              {
                os::cmsis::driver::serial::Status status;
                  {
                    Critical_section cs; // -----

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"
                    status = driver_->get_status ();
#pragma GCC diagnostic pop

                  }
                // We use a local tx busy flag because the ARM driver's flag
                // may become not-busy between transmissions.
                if (!status.tx_busy)
//                if (!tx_busy_)
                  {
                    uint8_t* pbuf;
                    std::size_t nb;
                      {
                        Critical_section cs; // -----

                        nb = tx_buf_->getFrontContiguousBuffer (&pbuf);
                      }
                    if (nb > 0)
                      {
                        if (driver_->send (pbuf, nb)
                            != os::cmsis::driver::RETURN_OK)
                          {
                            errno = EIO;
                            return -1;
                          }
//                        tx_busy_ = true;
                      }
                  }

//                bool isBelowHWM;
//                  {
//                    Critical_section cs; // -----
//
//                    isBelowHWM = tx_buf_->isBelowHighWaterMark ();
//                  }
                if (count == nbyte)
                  {
                    return nbyte;
                  }

                // Block and wait for buffer to be freed.
                osSemaphoreWait (tx_sem_, osWaitForever);

                if (count < nbyte)
                  {
                    Critical_section cs;  // -----

                    std::size_t n;
                    // If there is more space in the buffer, try to fill it.
                    n = tx_buf_->pushBack (
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
            os::cmsis::driver::serial::Status status;
            status = driver_->get_status ();
            if (status.is_tx_busy ())
              {
                osSemaphoreWait (tx_sem_, osWaitForever);
              }

            if ((driver_->send (buf, nbyte)) == os::cmsis::driver::RETURN_OK)
              {
                osSemaphoreWait (tx_sem_, osWaitForever);
                count = driver_->get_tx_count ();
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
    template<typename Cs_T>
    ssize_t
    Buffered_serial_device<Cs_T>::do_writev (const struct iovec* iov, int iovcnt)
      {
        errno = ENOSYS; // Not implemented
        return -1;
      }

    template<typename Cs_T>
    int
    Buffered_serial_device<Cs_T>::do_vioctl (int request, std::va_list args)
      {
        errno = ENOSYS; // Not implemented
        return -1;
      }

    template<typename Cs_T>
    int
    Buffered_serial_device<Cs_T>::do_vfcntl (int cmd, std::va_list args)
      {
        errno = ENOSYS; // Not implemented
        return -1;
      }
#endif

    // ------------------------------------------------------------------------

    template<typename Cs_T>
      void
      Buffered_serial_device<Cs_T>::signal_event (
          Buffered_serial_device* object, uint32_t event)
      {
        if (object->rx_sem_ == nullptr)
          {
            // After close(), ignore interrupts.
            return;
          }
        if ((event
            & (os::cmsis::driver::serial::Event::receive_complete
                | os::cmsis::driver::serial::Event::rx_framing_error
                | os::cmsis::driver::serial::Event::rx_timeout)))
          {
            // TODO: process errors and timeouts
            std::size_t tmpCount = object->driver_->get_rx_count ();
            std::size_t count = tmpCount - object->rx_count_;
            object->rx_count_ = tmpCount;
            std::size_t adjust = object->rx_buf_->advanceBack (count);
            assert(count == adjust);

            if (event & os::cmsis::driver::serial::Event::receive_complete)
              {
                uint8_t* pbuf;
                std::size_t nbyte = object->rx_buf_->getBackContiguousBuffer (
                    &pbuf);
                if (nbyte == 0)
                  {
                    // Overwrite the last byte, but keep the driver in
                    // receive mode continuously.
                    object->rx_buf_->retreatBack ();
                    nbyte = object->rx_buf_->getBackContiguousBuffer (&pbuf);
                  }
                assert(nbyte > 0);

                // Read as much as we can.
                int32_t status;
                status = object->driver_->receive (pbuf, nbyte);
                // TODO: implement error processing.
                assert(status == os::cmsis::driver::RETURN_OK);

                object->rx_count_ = 0;
              }
            if (count > 0)
              {
                // Immediately wake up, do not wait to reach any water mark.
                osSemaphoreRelease (object->rx_sem_);
              }
          }
        if (event & os::cmsis::driver::serial::Event::tx_complete)
          {
            if (object->tx_buf_ != nullptr)
              {
                std::size_t count = object->driver_->get_tx_count ();
                std::size_t adjust = object->tx_buf_->advanceFront (count);
                assert(count == adjust);

                uint8_t* pbuf;
                std::size_t nbyte = object->tx_buf_->getFrontContiguousBuffer (
                    &pbuf);
                if (nbyte > 0)
                  {
                    int32_t status;
                    status = object->driver_->send (pbuf, nbyte);
                    // TODO: implement error processing
                    assert(status == os::cmsis::driver::RETURN_OK);
                  }
                else
                  {
                    object->tx_busy_ = false;
                  }
                if (object->tx_buf_->isBelowLowWaterMark ())
                  {
                    // Wake up thread, to come and send more bytes.
                    osSemaphoreRelease (object->tx_sem_);
                  }
              }
            else
              {
                // No buffer, wake up the thread to return from write().
                osSemaphoreRelease (object->tx_sem_);
              }
          }
      }

#pragma GCC diagnostic pop

  } /* namespace dev */
} /* namespace os */

#endif /* POSIX_DRIVERS_BUFFERED_SERIAL_DEVICE_H_ */
