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

#ifndef CMSIS_PLUS_POSIX_DRIVERS_BYTE_CIRCULAR_BUFFER_H_
#define CMSIS_PLUS_POSIX_DRIVERS_BYTE_CIRCULAR_BUFFER_H_

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#include <cstdint>
#include <cstddef>

// ----------------------------------------------------------------------------

namespace os
{
  namespace dev
  {
    // ------------------------------------------------------------------------

    class ByteCircularBuffer
    {
    public:

      ByteCircularBuffer (const uint8_t* buf, std::size_t size,
                          std::size_t highWaterMark, std::size_t lowWaterMark =
                              0);

      ByteCircularBuffer (const uint8_t* buf, std::size_t size);

      // ----------------------------------------------------------------------

      void
      clear (void);

      const uint8_t&
      operator[] (std::size_t idx) const;

      // Insert bytes to the back of the buffer.
      std::size_t
      pushBack (uint8_t c);

      std::size_t
      pushBack (const uint8_t* buf, std::size_t count);

      std::size_t
      advanceBack (std::size_t count);

      void
      retreatBack (void);

      // Retrieve bytes from the front of the buffer.
      std::size_t
      popFront (uint8_t* buf);

      std::size_t
      popFront (uint8_t* buf, std::size_t size);

      std::size_t
      advanceFront (std::size_t count);

      // Get the address of the largest contiguous buffer in the front, and
      // length; might be only partial, if buffer wraps.
      std::size_t
      getFrontContiguousBuffer (uint8_t** ppbuf);

      // Get the address of the largest contiguous buffer in the back, and
      // length; might be only partial, if buffer wraps.
      std::size_t
      getBackContiguousBuffer (uint8_t** ppbuf);

      bool
      isEmpty (void) const;

      bool
      isFull (void) const;

      bool
      isAboveHighWaterMark (void) const;

      bool
      isBelowHighWaterMark (void) const;

      bool
      isAboveLowWaterMark (void) const;

      bool
      isBelowLowWaterMark (void) const;

      std::size_t
      length (void) const;

      std::size_t
      size (void) const;

      void
      dump (void);

      // ----------------------------------------------------------------------

    private:

      const uint8_t* const fBuf;
      std::size_t const fSize;
      std::size_t const fHighWaterMark;
      std::size_t const fLowWaterMark;

      // The following are volatile because they can be updated on
      // different threads or even on interrupts.

      // Actual length: [0 - size].
      std::size_t volatile fLen;

      // Next free position to push, at the back.
      uint8_t* volatile fBack;

      // First used position to pop, at the front.
      uint8_t* volatile fFront;
    };

    // ------------------------------------------------------------------------

    inline const uint8_t&
    ByteCircularBuffer::operator[] (std::size_t idx) const
    {
      return fBuf[idx];
    }

    inline bool
    ByteCircularBuffer::isEmpty (void) const
    {
      return (fLen == 0);
    }

    inline bool
    ByteCircularBuffer::isFull (void) const
    {
      return (fLen >= fSize);
    }

    inline bool
    ByteCircularBuffer::isAboveHighWaterMark (void) const
    {
      // Allow for water mark to be size.
      return (fLen >= fHighWaterMark);
    }

    inline bool
    ByteCircularBuffer::isBelowLowWaterMark (void) const
    {
      // Allow for water mark to be 0.
      return (fLen <= fLowWaterMark);
    }

    inline bool
    ByteCircularBuffer::isBelowHighWaterMark (void) const
    {
      return !isAboveHighWaterMark ();
    }

    inline bool
    ByteCircularBuffer::isAboveLowWaterMark (void) const
    {
      return !isBelowLowWaterMark ();
    }

    inline std::size_t
    ByteCircularBuffer::length (void) const
    {
      return fLen;
    }

    inline std::size_t
    ByteCircularBuffer::size (void) const
    {
      return fSize;
    }

  } /* namespace dev */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_DRIVERS_BYTE_CIRCULAR_BUFFER_H_ */
