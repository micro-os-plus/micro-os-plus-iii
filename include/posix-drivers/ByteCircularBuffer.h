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

#ifndef POSIX_DRIVERS_BYTE_CIRCULAR_BUFFER_H_
#define POSIX_DRIVERS_BYTE_CIRCULAR_BUFFER_H_

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

#endif /* POSIX_DRIVERS_BYTE_CIRCULAR_BUFFER_H_ */
