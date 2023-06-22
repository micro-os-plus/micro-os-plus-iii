/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#ifndef CMSIS_PLUS_POSIX_DRIVER_CIRCULAR_BUFFER_H_
#define CMSIS_PLUS_POSIX_DRIVER_CIRCULAR_BUFFER_H_

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#include <cstdint>
#include <cstddef>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    /**
     * @brief Circular buffer class template.
     * @headerfile circular-buffer.h <cmsis-plus/posix-driver/circular-buffer.h>
     * @ingroup cmsis-plus-posix-io-utils
     */
    template<typename T>
      class circular_buffer
      {
        // ----------------------------------------------------------------------

      public:

        /**
         * @brief Standard type definition.
         */
        using value_type = T;

        /**
         * @name Constructors & Destructor
         * @{
         */

      public:

        circular_buffer (const value_type* buf, std::size_t size,
                         std::size_t high_water_mark,
                         std::size_t low_water_mark = 0);

        circular_buffer (const value_type* buf, std::size_t size);

        /**
         * @cond ignore
         */

        // The rule of five.
        circular_buffer (const circular_buffer&) = delete;
        circular_buffer (circular_buffer&&) = delete;
        circular_buffer&
        operator= (const circular_buffer&) = delete;
        circular_buffer&
        operator= (circular_buffer&&) = delete;

        /**
         * @endcond
         */

        ~circular_buffer ();

        /**
         * @}
         */

        // --------------------------------------------------------------------
        /**
         * @name Public Member Functions
         * @{
         */

      public:

        void
        clear (void);

        const value_type&
        operator[] (std::size_t idx) const;

        // Insert bytes to the back of the buffer.
        std::size_t
        push_back (value_type v);

        std::size_t
        push_back (const value_type* buf, std::size_t count);

        std::size_t
        advance_back (std::size_t count);

        void
        retreat_back (void);

        // Retrieve bytes from the front of the buffer.
        std::size_t
        pop_front (value_type* buf);

        std::size_t
        pop_front (value_type* buf, std::size_t size);

        std::size_t
        advance_front (std::size_t count);

        // Get the address of the largest contiguous buffer in the front, and
        // length; might be only partial, if buffer wraps.
        std::size_t
        front_contiguous_buffer (value_type** ppbuf);

        // Get the address of the largest contiguous buffer in the back, and
        // length; might be only partial, if buffer wraps.
        std::size_t
        back_contiguous_buffer (value_type** ppbuf);

        bool
        empty (void) const;

        bool
        full (void) const;

        bool
        above_high_water_mark (void) const;

        bool
        below_high_water_mark (void) const;

        bool
        above_low_water_mark (void) const;

        bool
        below_low_water_mark (void) const;

        std::size_t
        length (void) const;

        std::size_t
        size (void) const;

        void
        dump (void);

        /**
         * @}
         */

        // --------------------------------------------------------------------
      private:

        /**
         * @cond ignore
         */

        const value_type* const buf_;
        std::size_t const size_;
        std::size_t const high_water_mark_;
        std::size_t const low_water_mark_;

        // The following are volatile because they can be updated on
        // different threads or even on interrupts.

        // Actual length: [0 - size].
        std::size_t volatile len_;

        // Next free position to push, at the back.
        value_type* volatile back_;

        // First used position to pop, at the front.
        value_type* volatile front_;

        /**
         * @endcond
         */

      };
    // ========================================================================

    /**
     * @brief Circular buffer of bytes.
     * @headerfile circular-buffer.h <cmsis-plus/posix-driver/circular-buffer.h>
     * @ingroup cmsis-plus-posix-io-utils
     */
    using circular_buffer_bytes = circular_buffer<uint8_t>;

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace posix
  {

    template<typename T>
      circular_buffer<T>::circular_buffer (const value_type* buf,
                                           std::size_t siz,
                                           std::size_t high_water_mark,
                                           std::size_t low_water_mark) :
          buf_ (buf), //
          size_ (siz), //
          high_water_mark_ (high_water_mark <= size_ ? high_water_mark : siz), //
          low_water_mark_ (low_water_mark)
      {
        assert (low_water_mark_ <= high_water_mark_);

        clear ();
      }

    template<typename T>
      circular_buffer<T>::circular_buffer (const value_type* buf,
                                           std::size_t siz) :
          circular_buffer
            { buf, siz, siz, 0 }
      {
        trace::printf ("%s(%p,%u) %p\n", __func__, buf, siz, this);
      }

    template<typename T>
      circular_buffer<T>::~circular_buffer ()
      {
        trace::printf ("%s() %p\n", __func__, this);
      }

    // ------------------------------------------------------------------------

    template<typename T>
      void
      circular_buffer<T>::clear (void)
      {
        back_ = front_ = const_cast<value_type* volatile > (buf_);
        len_ = 0;
#if defined(DEBUG)
        std::memset (static_cast<void*> (const_cast<value_type*> (buf_)), '?',
                     size_ * sizeof(value_type));
#endif
      }

    template<typename T>
      inline const typename circular_buffer<T>::value_type&
      circular_buffer<T>::operator[] (std::size_t idx) const
      {
        return buf_[idx];
      }

    template<typename T>
      inline bool
      circular_buffer<T>::empty (void) const
      {
        return (len_ == 0);
      }

    template<typename T>
      inline bool
      circular_buffer<T>::full (void) const
      {
        return (len_ >= size_);
      }

    template<typename T>
      inline bool
      circular_buffer<T>::above_high_water_mark (void) const
      {
        // Allow for water mark to be size.
        return (len_ >= high_water_mark_);
      }

    template<typename T>
      inline bool
      circular_buffer<T>::below_low_water_mark (void) const
      {
        // Allow for water mark to be 0.
        return (len_ <= low_water_mark_);
      }

    template<typename T>
      inline bool
      circular_buffer<T>::below_high_water_mark (void) const
      {
        return !above_high_water_mark ();
      }

    template<typename T>
      inline bool
      circular_buffer<T>::above_low_water_mark (void) const
      {
        return !below_low_water_mark ();
      }

    template<typename T>
      inline std::size_t
      circular_buffer<T>::length (void) const
      {
        return len_;
      }

    template<typename T>
      inline std::size_t
      circular_buffer<T>::size (void) const
      {
        return size_;
      }

    template<typename T>
      std::size_t
      circular_buffer<T>::push_back (value_type v)
      {
        if (full ())
          {
            return 0;
          }

        // Add to back.
        *back_++ = v;
        if (static_cast<std::size_t> (back_ - buf_) >= size_)
          {
            // Wrap.
            back_ = const_cast<value_type* volatile > (buf_);
          }
        len_++;
        return 1;
      }

    // Return the actual number of bytes, if not enough space for all.
    template<typename T>
      std::size_t
      circular_buffer<T>::push_back (const value_type* buf, std::size_t count)
      {
        assert (buf != nullptr);

        std::size_t len = count;
        if (count > (size_ - len_))
          {
            len = size_ - len_;
          }

        if (len == 0)
          {
            return 0;
          }

        std::size_t sizeToEnd = static_cast<std::size_t> (size_
            - static_cast<std::size_t> (back_ - buf_));
        if (len <= sizeToEnd)
          {
            std::memcpy (back_, buf, len);
            back_ += len;
            if (static_cast<std::size_t> (back_ - buf_) >= size_)
              {
                // Wrap.
                back_ = const_cast<value_type* volatile > (buf_);
              }
            len_ += len;
          }
        else
          {
            std::memcpy (back_, buf, sizeToEnd);
            back_ = const_cast<value_type* volatile > (buf_);
            std::memcpy (back_, buf + sizeToEnd, len - sizeToEnd);
            back_ += (len - sizeToEnd);
            len_ += len;
          }
        return len;
      }

    template<typename T>
      std::size_t
      circular_buffer<T>::advance_back (std::size_t count)
      {
        std::size_t adjust = count;
        if (count > (size_ - len_))
          {
            adjust = size_ - len_;
          }

        if (adjust == 0)
          {
            return 0;
          }

        back_ += adjust;
        if (back_ >= (buf_ + size_))
          {
            // Wrap.
            back_ -= size_;
          }
        len_ += adjust;

        return adjust;
      }

    template<typename T>
      void
      circular_buffer<T>::retreat_back (void)
      {
        if (back_ == buf_)
          {
            back_ = static_cast<value_type*> (const_cast<value_type*> (buf_)
                + size_ - 1);
          }
        else
          {
            back_ -= 1;
          }
        len_--;
      }

    template<typename T>
      std::size_t
      circular_buffer<T>::pop_front (value_type* buf)
      {
        assert (buf != nullptr);

        value_type c;
        if (len_ == 0)
          {
            return 0;
          }
        else
          {
            c = *front_++;
            if (static_cast<std::size_t> (front_ - buf_) >= size_)
              {
                front_ = const_cast<value_type* volatile > (buf_);
              }
            len_--;
            *buf = c;
            return 1;
          }
      }

    template<typename T>
      std::size_t
      circular_buffer<T>::pop_front (value_type* buf, std::size_t siz)
      {
        assert (buf != nullptr);

        std::size_t len = siz;
        if (len > len_)
          {
            len = len_;
          }

        std::size_t sizeToEnd = size_
            - static_cast<std::size_t> (front_ - buf_);
        if (len <= sizeToEnd)
          {
            std::memcpy (buf, front_, len);
            front_ += len;
            if (static_cast<std::size_t> (front_ - buf_) >= size_)
              {
                front_ = const_cast<value_type* volatile > (buf_);
              }
            len_ -= len;
          }
        else
          {
            std::memcpy (buf, front_, sizeToEnd);
            front_ = const_cast<value_type* volatile > (buf_);
            std::memcpy (buf + sizeToEnd, front_, len - sizeToEnd);
            front_ += (len - sizeToEnd);
            len_ -= len;
          }
        return len;
      }

    template<typename T>
      std::size_t
      circular_buffer<T>::advance_front (std::size_t count)
      {
        if (count == 0)
          {
            return 0;
          }

        std::size_t adjust = count;
        if (adjust > len_)
          {
            adjust = len_;
          }

        front_ += adjust;
        if (front_ >= (buf_ + size_))
          {
            // Wrap.
            front_ -= size_;
          }
        len_ -= adjust;

        return adjust;
      }

    template<typename T>
      std::size_t
      circular_buffer<T>::front_contiguous_buffer (value_type** ppbuf)
      {
        assert (ppbuf != nullptr);

        *ppbuf = front_;

        std::size_t sizeToEnd = size_
            - static_cast<std::size_t> (front_ - buf_);
        std::size_t len = sizeToEnd;
        if (len > len_)
          {
            len = len_;
          }

        return len;
      }

    template<typename T>
      std::size_t
      circular_buffer<T>::back_contiguous_buffer (value_type** ppbuf)
      {
        assert (ppbuf != nullptr);

        *ppbuf = back_;

        std::size_t sizeToEnd = size_ - static_cast<std::size_t> (back_ - buf_);
        std::size_t len = sizeToEnd;
        if (len > (size_ - len_))
          {
            len = size_ - len_;
          }

        return len;
      }

    template<typename T>
      void
      circular_buffer<T>::dump (void)
      {
        os::trace::printf ("%s @%p {buf=%p, size=%d, len=%d, hwm=%d, lwn=%d}\n",
                           __PRETTY_FUNCTION__, buf_, size_, len_,
                           high_water_mark_, low_water_mark_);
      }

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_POSIX_DRIVER_CIRCULAR_BUFFER_H_ */
