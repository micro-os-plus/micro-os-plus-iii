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

#include "posix-drivers/ByteCircularBuffer.h"
#include "diag/trace.h"

#include <cstring>
#include <cassert>

// ----------------------------------------------------------------------------

namespace os
{
  namespace dev
  {
    // ------------------------------------------------------------------------

    ByteCircularBuffer::ByteCircularBuffer (const uint8_t* buf,
                                            std::size_t size,
                                            std::size_t highWaterMark,
                                            std::size_t lowWaterMark) :
        fBuf (buf), //
        fSize (size), //
        fHighWaterMark (highWaterMark <= fSize ? highWaterMark : size), //
        fLowWaterMark (lowWaterMark)
    {
      assert(fLowWaterMark <= fHighWaterMark);

      clear ();
    }

    ByteCircularBuffer::ByteCircularBuffer (const uint8_t* buf,
                                            std::size_t size) :
        ByteCircularBuffer (buf, size, size, 0)
    {
      ;
    }

    // ------------------------------------------------------------------------

    void
    ByteCircularBuffer::clear (void)
    {
      fBack = fFront = const_cast<uint8_t* volatile > (fBuf);
      fLen = 0;
#if defined(DEBUG)
      std::memset ((void*) fBuf, '?', fSize);
#endif
    }

    std::size_t
    ByteCircularBuffer::pushBack (uint8_t c)
    {
      if (isFull ())
        {
          return 0;
        }

      // Add to back.
      *fBack++ = c;
      if ((std::size_t) (fBack - fBuf) >= fSize)
        {
          // Wrap.
          fBack = const_cast<uint8_t* volatile > (fBuf);
        }
      fLen++;
      return 1;
    }

    // Return the actual number of bytes, if not enough space for all.
    std::size_t
    ByteCircularBuffer::pushBack (const uint8_t* buf, std::size_t count)
    {
      assert(buf != nullptr);

      std::size_t length = count;
      if (count > (fSize - fLen))
        {
          length = fSize - fLen;
        }

      if (length == 0)
        {
          return 0;
        }

      std::size_t sizeToEnd = (std::size_t) (fSize - (fBack - fBuf));
      if (length <= sizeToEnd)
        {
          std::memcpy (fBack, buf, length);
          fBack += length;
          if ((std::size_t) (fBack - fBuf) >= fSize)
            {
              // Wrap.
              fBack = const_cast<uint8_t* volatile > (fBuf);
            }
          fLen += length;
        }
      else
        {
          std::memcpy (fBack, buf, sizeToEnd);
          fBack = const_cast<uint8_t* volatile > (fBuf);
          std::memcpy (fBack, buf + sizeToEnd, length - sizeToEnd);
          fBack += (length - sizeToEnd);
          fLen += length;
        }
      return length;
    }

    std::size_t
    ByteCircularBuffer::advanceBack (std::size_t count)
    {
      std::size_t adjust = count;
      if (count > (fSize - fLen))
        {
          adjust = fSize - fLen;
        }

      if (adjust == 0)
        {
          return 0;
        }

      fBack += adjust;
      if (fBack >= (fBuf + fSize))
        {
          // Wrap.
          fBack -= fSize;
        }
      fLen += adjust;

      return adjust;
    }

    void
    ByteCircularBuffer::retreatBack (void)
    {
      if (fBack == fBuf)
        {
          fBack = (uint8_t*) (fBuf + fSize - 1);
        }
      else
        {
          fBack -= 1;
        }
      fLen--;
    }

    std::size_t
    ByteCircularBuffer::popFront (uint8_t* buf)
    {
      assert(buf != nullptr);

      uint8_t c;
      if (fLen == 0)
        {
          return 0;
        }
      else
        {
          c = *fFront++;
          if ((std::size_t) (fFront - fBuf) >= fSize)
            {
              fFront = const_cast<uint8_t* volatile > (fBuf);
            }
          fLen--;
          *buf = c;
          return 1;
        }
    }

    std::size_t
    ByteCircularBuffer::popFront (uint8_t* buf, std::size_t size)
    {
      assert(buf != nullptr);

      std::size_t length = size;
      if (length > fLen)
        {
          length = fLen;
        }

      std::size_t sizeToEnd = fSize - (std::size_t) (fFront - fBuf);
      if (length <= sizeToEnd)
        {
          std::memcpy (buf, fFront, length);
          fFront += length;
          if ((std::size_t) (fFront - fBuf) >= fSize)
            {
              fFront = const_cast<uint8_t* volatile > (fBuf);
            }
          fLen -= length;
        }
      else
        {
          std::memcpy (buf, fFront, sizeToEnd);
          fFront = const_cast<uint8_t* volatile > (fBuf);
          std::memcpy (buf + sizeToEnd, fFront, length - sizeToEnd);
          fFront += (length - sizeToEnd);
          fLen -= length;
        }
      return length;
    }

    std::size_t
    ByteCircularBuffer::advanceFront (std::size_t count)
    {
      if (count == 0)
        {
          return 0;
        }

      std::size_t adjust = count;
      if (adjust > fLen)
        {
          adjust = fLen;
        }

      fFront += adjust;
      if (fFront >= (fBuf + fSize))
        {
          // Wrap.
          fFront -= fSize;
        }
      fLen -= adjust;

      return adjust;
    }

    std::size_t
    ByteCircularBuffer::getFrontContiguousBuffer (uint8_t** ppbuf)
    {
      assert(ppbuf != nullptr);
      *ppbuf = fFront;

      std::size_t sizeToEnd = fSize - (std::size_t) (fFront - fBuf);
      std::size_t length = sizeToEnd;
      if (length > fLen)
        {
          length = fLen;
        }

      return length;
    }

    std::size_t
    ByteCircularBuffer::getBackContiguousBuffer (uint8_t** ppbuf)
    {
      assert(ppbuf != nullptr);
      *ppbuf = fBack;

      std::size_t sizeToEnd = fSize - (std::size_t) (fBack - fBuf);
      std::size_t length = sizeToEnd;
      if (length > (fSize - fLen))
        {
          length = fSize - fLen;
        }

      return length;
    }

    void
    ByteCircularBuffer::dump (void)
    {
      os::trace::printf ("%s @%p {buf=%p, size=%d, len=%d, hwm=%d, lwn=%d}\n",
                         __PRETTY_FUNCTION__, fBuf, fSize, fLen, fHighWaterMark,
                         fLowWaterMark);
    }

  }
/* namespace dev */
} /* namespace os */
