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

#ifndef CMSIS_PLUS_POSIX_DRIVERS_BUFFERED_SERIAL_DEVICE_H_
#define CMSIS_PLUS_POSIX_DRIVERS_BUFFERED_SERIAL_DEVICE_H_

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#include <cmsis-plus/rtos/os.h>

#include <cmsis-plus/posix-io/CharDevice.h>
#include <cmsis-plus/posix-drivers/ByteCircularBuffer.h>
#include <cmsis-plus/drivers/serial.h>

// ----------------------------------------------------------------------------

// TODO: (multiline)
// - add flow control on both send & receive
// - cancel pending reads/writes at close (partly done)
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
                                os::driver::Serial* driver,
                                os::dev::ByteCircularBuffer* rx_buf,
                                os::dev::ByteCircularBuffer* tx_buf);

        // Prevent copy, move, assign
        Buffered_serial_device (const Buffered_serial_device&) = delete;

        Buffered_serial_device (Buffered_serial_device&&) = delete;

        Buffered_serial_device&
        operator= (const Buffered_serial_device&) = delete;

        Buffered_serial_device&
        operator= (Buffered_serial_device&&) = delete;

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
        do_is_opened (void) override;

        virtual bool
        do_is_connected (void) override;

        // --------------------------------------------------------------------

      private:

        // Pointer to actual CMSIS-like serial driver (usart or usb cdc acm)
        os::driver::Serial* driver_ = nullptr;

        os::rtos::semaphore_binary open_sem_
          { "open", 0 };
        os::rtos::semaphore_binary rx_sem_
          { "rx", 0 };
        os::rtos::semaphore_binary tx_sem_
          { "tx", 0 };

        os::dev::ByteCircularBuffer* rx_buf_ = nullptr;
        os::dev::ByteCircularBuffer* tx_buf_ = nullptr;

        std::size_t rx_count_ = 0; //
        bool volatile tx_busy_ = false;
        bool volatile is_connected_ = false;
        bool volatile is_opened_ = false;
        // Padding!

      };

#pragma GCC diagnostic pop

    // ------------------------------------------------------------------------

    template<typename Cs_T>
      Buffered_serial_device<Cs_T>::Buffered_serial_device (
          const char* deviceName, //
          os::driver::Serial* driver, os::dev::ByteCircularBuffer* rx_buf,
          os::dev::ByteCircularBuffer* tx_buf) :
          //
          CharDevice (deviceName), // Construct parent.
          driver_ (driver), //
          rx_buf_ (rx_buf), //
          tx_buf_ (tx_buf) //
      {
        assert (rx_buf != nullptr);

        // Do not check the same for tx_buf, it may be null.

        driver_->register_callback (
            reinterpret_cast<os::driver::signal_event_t> (signal_event), this);
      }

    template<typename Cs_T>
      Buffered_serial_device<Cs_T>::~Buffered_serial_device ()
      {
        driver_ = nullptr;
        is_connected_ = false;
        is_opened_ = false;
      }

    // ------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    template<typename Cs_T>
      int
      Buffered_serial_device<Cs_T>::do_vopen (const char* path, int oflag,
                                              std::va_list args)
      {
        if (is_opened_)
          {
            errno = EEXIST; // Already opened
            return -1;
          }

        int32_t result;

        do
          {
            // Reset semaphores, in case we come here after close.
            open_sem_.reset ();
            rx_sem_.reset ();
            tx_sem_.reset ();

            is_opened_ = true;

            // Clear buffers.
            rx_buf_->clear ();
            rx_count_ = 0;

            if (tx_buf_ != nullptr)
              {
                tx_buf_->clear ();
              }

            // Default configuration: 8 bits, no parity, 1 stop bit,
            // no flow control, 115200 bps.
            result = driver_->configure (
                os::driver::serial::MODE_ASYNCHRONOUS
                    | os::driver::serial::DATA_BITS_8
                    | os::driver::serial::PARITY_NONE
                    | os::driver::serial::STOP_BITS_1
                    | os::driver::serial::FLOW_CONTROL_NONE,
                115200);
            // assert(result == os::driver::RETURN_OK);
            if (result != os::driver::RETURN_OK)
              break;

            // Enable TX output.
            result = driver_->control (os::driver::serial::Control::enable_tx);
            if (result != os::driver::RETURN_OK)
              break;

            // Enable RX input.
            result = driver_->control (os::driver::serial::Control::enable_rx);
            if (result != os::driver::RETURN_OK)
              break;

          }
        while (false); // Actually NOT a loop, just a sequence of ifs!

        if (result != os::driver::RETURN_OK)
          {
            errno = EIO;
            return -1;
          }

        os::driver::serial::Capabilities capa;
        capa = driver_->get_capabilities ();
        if (capa.dcd)
          {
            os::driver::serial::Modem_status status;
            for (;;)
              {
                  {
                    Critical_section cs; // -----

                    status = driver_->get_modem_status ();
                  }
                if (status.is_dcd_active ())
                  {
                    break;
                  }
                open_sem_.wait ();
              }
          }

        uint8_t* pbuf;
        std::size_t nbyte = rx_buf_->getBackContiguousBuffer (&pbuf);

        result = driver_->receive (pbuf, nbyte);
        if (result != os::driver::RETURN_OK)
          {
            errno = EIO;
            return -1;
          }

        is_connected_ = true;

        // Return POSIX idea of OK.
        return 0;
      }

    template<typename Cs_T>
      bool
      Buffered_serial_device<Cs_T>::do_is_opened (void)
      {
        return is_opened_;
      }

    template<typename Cs_T>
      bool
      Buffered_serial_device<Cs_T>::do_is_connected (void)
      {
        return is_connected_;
      }

    template<typename Cs_T>
      int
      Buffered_serial_device<Cs_T>::do_close (void)
      {

        if (is_connected_)
          {
            // Wait for write to complete.
            // TODO: what if flow control prevents this?
            if (tx_buf_ != nullptr)
              {
                for (;;)
                  {
                    if (tx_buf_->isEmpty ())
                      {
                        break;
                      }
                    tx_sem_.wait ();
                  }
              }
          }

        // Abort pending reads.
        os::driver::return_t ret;
        ret = driver_->control (os::driver::serial::Control::abort_receive);
        assert (ret == os::driver::RETURN_OK);

        // Abort pending writes.
        ret = driver_->control (os::driver::serial::Control::abort_send);
        assert (ret == os::driver::RETURN_OK);

        // Disable transmitter and receiver.
        ret = driver_->control (os::driver::serial::Control::disable_tx);
        assert (ret == os::driver::RETURN_OK);

        ret = driver_->control (os::driver::serial::Control::disable_rx);
        assert (ret == os::driver::RETURN_OK);
        ret = driver_->control (os::driver::serial::Control::disable_break);
        assert (ret == os::driver::RETURN_OK);

        is_opened_ = false;
        is_connected_ = false;

        // Return POSIX idea of OK.
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
            if (!is_connected_)
              {
                errno = EIO;
                return -1;
              }
            // Block and wait for bytes to arrive.
            rx_sem_.wait ();
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
                os::driver::serial::Status status;
                  {
                    Critical_section cs; // -----

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"
                    status = driver_->get_status ();
#pragma GCC diagnostic pop

                  }
                if (!status.tx_busy)
                  {
                    uint8_t* pbuf;
                    std::size_t nb;
                      {
                        Critical_section cs; // -----

                        nb = tx_buf_->getFrontContiguousBuffer (&pbuf);
                      }
                    if (nb > 0)
                      {
                        if (driver_->send (pbuf, nb) != os::driver::RETURN_OK)
                          {
                            errno = EIO;
                            return -1;
                          }
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

                if (!is_connected_)
                  {
                    if (count > 0)
                      {
                        return count;
                      }

                    errno = EIO;
                    return -1;
                  }

                // Block and wait for buffer to be freed.
                tx_sem_.wait ();

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
            os::driver::serial::Status status;
            for (;;)
              {
                if (!is_connected_)
                  {
                    errno = EIO;
                    return -1;
                  }

                status = driver_->get_status ();
                if (!status.is_tx_busy ())
                  {
                    break;
                  }
                tx_sem_.wait ();
              }

            if ((driver_->send (buf, nbyte)) == os::driver::RETURN_OK)
              {
                for (;;)
                  {
                    if (!is_connected_)
                      {
                        errno = EIO;
                        return -1;
                      }

                    status = driver_->get_status ();
                    if (!status.is_tx_busy ())
                      {
                        break;
                      }
                    tx_sem_.wait ();
                  }
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
        if (!object->is_opened_)
          {
            // After close(), ignore interrupts.
            return;
          }
        if ((event
            & (os::driver::serial::Event::receive_complete
                | os::driver::serial::Event::rx_framing_error
                | os::driver::serial::Event::rx_timeout)))
          {
            // TODO: process errors and timeouts
            std::size_t tmpCount = object->driver_->get_rx_count ();
            std::size_t count = tmpCount - object->rx_count_;
            object->rx_count_ = tmpCount;
            std::size_t adjust = object->rx_buf_->advanceBack (count);
            assert (count == adjust);

            if (event & os::driver::serial::Event::receive_complete)
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
                assert (nbyte > 0);

                // Read as much as we can.
                int32_t status;
                status = object->driver_->receive (pbuf, nbyte);
                // TODO: implement error processing.
                assert (status == os::driver::RETURN_OK);

                object->rx_count_ = 0;
              }
            if (count > 0)
              {
                // Immediately wake up, do not wait to reach any water mark.
                object->rx_sem_.post ();
              }
          }
        if (event & os::driver::serial::Event::tx_complete)
          {
            if (object->tx_buf_ != nullptr)
              {
                std::size_t count = object->driver_->get_tx_count ();
                std::size_t adjust = object->tx_buf_->advanceFront (count);
                assert (count == adjust);

                uint8_t* pbuf;
                std::size_t nbyte = object->tx_buf_->getFrontContiguousBuffer (
                    &pbuf);
                if (nbyte > 0)
                  {
                    int32_t status;
                    status = object->driver_->send (pbuf, nbyte);
                    // TODO: implement error processing
                    assert (status == os::driver::RETURN_OK);
                  }
                else
                  {
                    object->tx_busy_ = false;
                  }
                if (object->tx_buf_->isBelowLowWaterMark ())
                  {
                    // Wake up thread, to come and send more bytes.
                    object->tx_sem_.post ();
                  }
              }
            else
              {
                // No buffer, wake up the thread to return from write().
                object->tx_sem_.post ();
              }
          }
        if (event & os::driver::serial::Event::dcd)
          {
            os::driver::serial::Modem_status status;
            status = object->driver_->get_modem_status ();

            bool is_dcd_active = status.is_dcd_active ();
            object->is_connected_ = is_dcd_active;
            if (is_dcd_active)
              {
                // Connected, wake-up open().
                object->open_sem_.post ();
              }
            else
              {
                // Disconnected, cancel read.
                object->rx_sem_.post ();

                // Cancel write.
                object->tx_sem_.post ();
              }
          }
        if (event & os::driver::serial::Event::cts)
          {
            // TODO: add flow control
          }
        if (event & os::driver::serial::Event::dsr)
          {
            // TODO: add flow control
          }
      }

#pragma GCC diagnostic pop

  } /* namespace dev */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_DRIVERS_BUFFERED_SERIAL_DEVICE_H_ */
