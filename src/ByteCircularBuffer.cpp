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

#include "posix-drivers/ByteCircularBuffer.h"
#include <cmsis-plus/diag/trace.h>

#include <cstring>
#include <cassert>

// ----------------------------------------------------------------------------

namespace os
{
  namespace dev
  {
    // ------------------------------------------------------------------------

    ByteCircularBuffer::ByteCircularBuffer (const uint8_t* buf,
                                            std::size_t siz,
                                            std::size_t highWaterMark,
                                            std::size_t lowWaterMark) :
        fBuf (buf), //
        fSize (siz), //
        fHighWaterMark (highWaterMark <= fSize ? highWaterMark : siz), //
        fLowWaterMark (lowWaterMark)
    {
      assert(fLowWaterMark <= fHighWaterMark);

      clear ();
    }

    ByteCircularBuffer::ByteCircularBuffer (const uint8_t* buf,
                                            std::size_t siz) :
        ByteCircularBuffer (buf, siz, siz, 0)
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

      std::size_t len = count;
      if (count > (fSize - fLen))
        {
          len = fSize - fLen;
        }

      if (len == 0)
        {
          return 0;
        }

      std::size_t sizeToEnd = (std::size_t) (fSize - (fBack - fBuf));
      if (len <= sizeToEnd)
        {
          std::memcpy (fBack, buf, len);
          fBack += len;
          if ((std::size_t) (fBack - fBuf) >= fSize)
            {
              // Wrap.
              fBack = const_cast<uint8_t* volatile > (fBuf);
            }
          fLen += len;
        }
      else
        {
          std::memcpy (fBack, buf, sizeToEnd);
          fBack = const_cast<uint8_t* volatile > (fBuf);
          std::memcpy (fBack, buf + sizeToEnd, len - sizeToEnd);
          fBack += (len - sizeToEnd);
          fLen += len;
        }
      return len;
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
    ByteCircularBuffer::popFront (uint8_t* buf, std::size_t siz)
    {
      assert(buf != nullptr);

      std::size_t len = siz;
      if (len > fLen)
        {
          len = fLen;
        }

      std::size_t sizeToEnd = fSize - (std::size_t) (fFront - fBuf);
      if (len <= sizeToEnd)
        {
          std::memcpy (buf, fFront, len);
          fFront += len;
          if ((std::size_t) (fFront - fBuf) >= fSize)
            {
              fFront = const_cast<uint8_t* volatile > (fBuf);
            }
          fLen -= len;
        }
      else
        {
          std::memcpy (buf, fFront, sizeToEnd);
          fFront = const_cast<uint8_t* volatile > (fBuf);
          std::memcpy (buf + sizeToEnd, fFront, len - sizeToEnd);
          fFront += (len - sizeToEnd);
          fLen -= len;
        }
      return len;
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
      std::size_t len = sizeToEnd;
      if (len > fLen)
        {
          len = fLen;
        }

      return len;
    }

    std::size_t
    ByteCircularBuffer::getBackContiguousBuffer (uint8_t** ppbuf)
    {
      assert(ppbuf != nullptr);
      *ppbuf = fBack;

      std::size_t sizeToEnd = fSize - (std::size_t) (fBack - fBuf);
      std::size_t len = sizeToEnd;
      if (len > (fSize - fLen))
        {
          len = fSize - fLen;
        }

      return len;
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
