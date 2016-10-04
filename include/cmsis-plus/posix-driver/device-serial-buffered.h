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

#ifndef CMSIS_PLUS_POSIX_DRIVER_DEVICE_SERIAL_BUFFERED_H_
#define CMSIS_PLUS_POSIX_DRIVER_DEVICE_SERIAL_BUFFERED_H_

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#include <cmsis-plus/rtos/os.h>

#include <cmsis-plus/posix-io/device-char.h>
#include <cmsis-plus/posix-driver/circular-buffer.h>
#include <cmsis-plus/driver/serial.h>

// ----------------------------------------------------------------------------

// TODO: (multiline)
// - add flow control on both send & receive
// - cancel pending reads/writes at close (partly done)
// - add error processing

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @brief Buffered serial driver class template.
     * @headerfile circular-buffer.h <cmsis-plus/posix-driver/circular-buffer.h>
     * @ingroup cmsis-plus-posix-io-driver
     */
    template<typename CS>
      class device_serial_buffered : public os::posix::device_char
      {
        using critical_section = CS;

        // ----------------------------------------------------------------------

        /**
         * @name Constructors & Destructor
         * @{
         */

      public:

        device_serial_buffered (const char* device_name,
                                os::driver::Serial* driver,
                                os::posix::circular_buffer_bytes* rx_buf,
                                os::posix::circular_buffer_bytes* tx_buf);

        /**
         * @cond ignore
         */

        // The rule of five.
        device_serial_buffered (const device_serial_buffered&) = delete;
        device_serial_buffered (device_serial_buffered&&) = delete;
        device_serial_buffered&
        operator= (const device_serial_buffered&) = delete;
        device_serial_buffered&
        operator= (device_serial_buffered&&) = delete;

        /**
         * @endcond
         */

        virtual
        ~device_serial_buffered ();

        /**
         * @}
         */

        // --------------------------------------------------------------------
        /**
         * @name Public Static Member Functions
         * @{
         */

      public:

        // Static function called by the CMSIS driver in an
        // interrupt context.

        static void
        signal_event (device_serial_buffered* object, uint32_t event);

        /**
         * @}
         */

        // --------------------------------------------------------------------
        /**
         * @name Private Member Functions
         * @{
         */

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

        /**
         * @}
         */

        // --------------------------------------------------------------------
      private:

        /**
         * @cond ignore
         */

        // Pointer to actual CMSIS-like serial driver (usart or usb cdc acm)
        os::driver::Serial* driver_ = nullptr;

        os::rtos::semaphore_binary open_sem_
          { "open", 0 };
        os::rtos::semaphore_binary rx_sem_
          { "rx", 0 };
        os::rtos::semaphore_binary tx_sem_
          { "tx", 0 };

        os::posix::circular_buffer_bytes* rx_buf_ = nullptr;
        os::posix::circular_buffer_bytes* tx_buf_ = nullptr;

        std::size_t rx_count_ = 0; //
        bool volatile tx_busy_ = false;
        bool volatile is_connected_ = false;
        bool volatile is_opened_ = false;
        // Padding!

        /**
         * @endcond
         */

      };

#pragma GCC diagnostic pop

  } /* namespace posix */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    template<typename CS>
      device_serial_buffered<CS>::device_serial_buffered (
          const char* device_name, //
          os::driver::Serial* driver, os::posix::circular_buffer_bytes* rx_buf,
          os::posix::circular_buffer_bytes* tx_buf) :
          //
          device_char (device_name), // Construct parent.
          driver_ (driver), //
          rx_buf_ (rx_buf), //
          tx_buf_ (tx_buf) //
      {
        trace::printf ("%s(\"%s\",%p,%p,%p) %p\n", __func__, device_name,
                       driver, rx_buf, tx_buf, this);

        assert (rx_buf != nullptr);

        // Do not check the same for tx_buf, it may be null.

        driver_->register_callback (
            reinterpret_cast<os::driver::signal_event_t> (signal_event), this);
      }

    template<typename CS>
      device_serial_buffered<CS>::~device_serial_buffered ()
      {
        trace::printf ("%s() %p\n", __func__, this);

        driver_ = nullptr;
        is_connected_ = false;
        is_opened_ = false;
      }

    // ------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    template<typename CS>
      int
      device_serial_buffered<CS>::do_vopen (const char* path, int oflag,
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
                    // ----- Enter critical section ---------------------------
                    critical_section cs;

                    status = driver_->get_modem_status ();
                    // ----- Exit critical section ----------------------------
                  }
                if (status.is_dcd_active ())
                  {
                    break;
                  }
                open_sem_.wait ();
              }
          }

        uint8_t* pbuf;
        std::size_t nbyte = rx_buf_->back_contiguous_buffer (&pbuf);

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

    template<typename CS>
      bool
      device_serial_buffered<CS>::do_is_opened (void)
      {
        return is_opened_;
      }

    template<typename CS>
      bool
      device_serial_buffered<CS>::do_is_connected (void)
      {
        return is_connected_;
      }

    template<typename CS>
      int
      device_serial_buffered<CS>::do_close (void)
      {

        if (is_connected_)
          {
            // Wait for write to complete.
            // TODO: what if flow control prevents this?
            if (tx_buf_ != nullptr)
              {
                for (;;)
                  {
                    if (tx_buf_->empty ())
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

    template<typename CS>
      ssize_t
      device_serial_buffered<CS>::do_read (void* buf, std::size_t nbyte)
      {
        // TODO: implement cases when 0 must be returned
        // (disconnects, timeouts).
        while (true)
          {
            std::size_t count;
              {
                // ----- Enter critical section -------------------------------
                critical_section cs;

                count = rx_buf_->pop_front (static_cast<uint8_t*> (buf), nbyte);
                // ----- Exit critical section --------------------------------
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

    template<typename CS>
      ssize_t
      device_serial_buffered<CS>::do_write (const void* buf, std::size_t nbyte)
      {
        std::size_t count;

        if (tx_buf_ != nullptr)
          {
            count = 0;
              {
                // ----- Enter critical section -------------------------------
                critical_section cs;

                if (tx_buf_->below_high_water_mark ())
                  {
                    // If there is more space in the buffer, try to fill it.
                    count = tx_buf_->push_back (
                        static_cast<const uint8_t*> (buf), nbyte);
                  }
                // ----- Exit critical section --------------------------------
              }
            while (true)
              {
                os::driver::serial::Status status;
                  {
                    // ----- Enter critical section ---------------------------
                    critical_section cs;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"
                    status = driver_->get_status ();
#pragma GCC diagnostic pop

                    // ----- Exit critical section ----------------------------
                  }
                if (!status.tx_busy)
                  {
                    uint8_t* pbuf;
                    std::size_t nb;
                      {
                        // ----- Enter critical section -----------------------
                        critical_section cs; // -----

                        nb = tx_buf_->front_contiguous_buffer (&pbuf);
                        // ----- Exit critical section ------------------------
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
//                    critical_section cs; // -----
//
//                    isBelowHWM = tx_buf_->below_high_water_mark ();
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
                    // ----- Enter critical section ---------------------------
                    critical_section cs;

                    std::size_t n;
                    // If there is more space in the buffer, try to fill it.
                    n = tx_buf_->push_back (
                        static_cast<const uint8_t*> (buf) + count,
                        nbyte - count);
                    count += n;
                    // ----- Exit critical section ----------------------------
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
    template<typename CS>
    ssize_t
    device_serial_buffered<CS>::do_writev (const struct iovec* iov, int iovcnt)
      {
        errno = ENOSYS; // Not implemented
        return -1;
      }

    template<typename CS>
    int
    device_serial_buffered<CS>::do_vioctl (int request, std::va_list args)
      {
        errno = ENOSYS; // Not implemented
        return -1;
      }

    template<typename CS>
    int
    device_serial_buffered<CS>::do_vfcntl (int cmd, std::va_list args)
      {
        errno = ENOSYS; // Not implemented
        return -1;
      }
#endif

    // ------------------------------------------------------------------------

    template<typename CS>
      void
      device_serial_buffered<CS>::signal_event (device_serial_buffered* object,
                                                uint32_t event)
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
            std::size_t adjust = object->rx_buf_->advance_back (count);
            assert (count == adjust);

            if (event & os::driver::serial::Event::receive_complete)
              {
                uint8_t* pbuf;
                std::size_t nbyte = object->rx_buf_->back_contiguous_buffer (
                    &pbuf);
                if (nbyte == 0)
                  {
                    // Overwrite the last byte, but keep the driver in
                    // receive mode continuously.
                    object->rx_buf_->retreat_back ();
                    nbyte = object->rx_buf_->back_contiguous_buffer (&pbuf);
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
                std::size_t adjust = object->tx_buf_->advance_front (count);
                assert (count == adjust);

                uint8_t* pbuf;
                std::size_t nbyte = object->tx_buf_->front_contiguous_buffer (
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
                if (object->tx_buf_->below_low_water_mark ())
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

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_DRIVER_DEVICE_SERIAL_BUFFERED_H_ */
