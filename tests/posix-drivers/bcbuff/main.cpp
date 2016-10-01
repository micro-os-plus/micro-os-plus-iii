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

#include <cassert>
#include <cstring>

// ----------------------------------------------------------------------------

int
main (int argc __attribute__((unused)), char* argv[] __attribute__((unused)))
{
  uint8_t buff[5];
  os::dev::ByteCircularBuffer cb
    { buff, 5 };

  // Empty buffer.
  assert(cb.size () == 5);
  assert(cb.length () == 0);
  assert(cb.isEmpty ());
  assert(!cb.isFull ());

  // Low water marks.
  assert(cb.isBelowLowWaterMark ());
  assert(!cb.isAboveLowWaterMark ());

  // No more pops.
  uint8_t ch[6];
  assert(cb.popFront (&ch[0]) == 0);
  assert(cb.popFront (ch, 5) == 0);
  assert(cb.advanceFront (2) == 0);

  uint8_t* pb;
  assert(cb.getFrontContiguousBuffer (&pb) == 0);
  pb = nullptr;
  assert(cb.getBackContiguousBuffer (&pb) == 5);
  assert(pb == &buff[0]);

  // Full buffer.
  assert(cb.pushBack ((uint8_t* )"012345", 5) == 5);
  assert(cb.isFull ());
  assert(!cb.isEmpty ());

  // No more pushes
  assert(cb.pushBack ('?') == 0);
  assert(cb.pushBack ((uint8_t* )"012345", 5) == 0);
  assert(cb.advanceBack (2) == 0);

  // High water marks.
  assert(cb.isAboveHighWaterMark ());
  assert(!cb.isBelowHighWaterMark ());

  // Array operator.
  assert(cb[2] == '2');

  // Clear.
  cb.clear ();
  assert(cb.isEmpty ());

  //  0 1 2 3 4
  // | |x|x| | |
  // +-+-+-+-+-+
  //    f   b

  assert(cb.pushBack ((uint8_t* )"abc", 3) == 3);
  assert(cb.popFront (&ch[0]) == 1);
  assert(ch[0] == 'a');

  assert(cb.length () == 2);

  assert(!cb.isBelowLowWaterMark ());
  assert(cb.isAboveLowWaterMark ());

  assert(!cb.isAboveHighWaterMark ());
  assert(cb.isBelowHighWaterMark ());

  pb = nullptr;
  assert(cb.getFrontContiguousBuffer (&pb) == 2);
  assert(pb == &buff[1]);

  pb = nullptr;
  assert(cb.getBackContiguousBuffer (&pb) == 2);
  assert(pb == &buff[3]);

  //  0 1 2 3 4
  // | |x|x|x| |
  // +-+-+-+-+-+
  //    f     b

  assert(cb.pushBack ('d') == 1);

  pb = nullptr;
  assert(cb.getFrontContiguousBuffer (&pb) == 3);
  assert(pb == &buff[1]);

  pb = nullptr;
  assert(cb.getBackContiguousBuffer (&pb) == 1);
  assert(pb == &buff[4]);

  //  0 1 2 3 4
  // | | | |x| |
  // +-+-+-+-+-+
  //        f b

  assert(cb.popFront (&ch[0]) == 1);
  assert(ch[0] == 'b');

  assert(cb.popFront (&ch[0]) == 1);
  assert(ch[0] == 'c');

  pb = nullptr;
  assert(cb.getFrontContiguousBuffer (&pb) == 1);
  assert(pb == &buff[3]);

  pb = nullptr;
  assert(cb.getBackContiguousBuffer (&pb) == 1);
  assert(pb == &buff[4]);

  //  0 1 2 3 4
  // | | | |x|x|
  // +-+-+-+-+-+
  //  b     f

  assert(cb.pushBack ('e') == 1);

  pb = nullptr;
  assert(cb.getFrontContiguousBuffer (&pb) == 2);
  assert(pb == &buff[3]);

  pb = nullptr;
  assert(cb.getBackContiguousBuffer (&pb) == 3);
  assert(pb == &buff[0]);

  //  0 1 2 3 4
  // | | | |x|x|
  // +-+-+-+-+-+
  //  b     f

  assert(cb.pushBack ('f') == 1);

  pb = nullptr;
  assert(cb.getFrontContiguousBuffer (&pb) == 2);
  assert(pb == &buff[3]);

  pb = nullptr;
  assert(cb.getBackContiguousBuffer (&pb) == 2);
  assert(pb == &buff[1]);

  // pushBack/popFront buffer
  cb.clear ();
  assert(cb.pushBack ((uint8_t* )"xy", 1) == 1);
  assert(cb.pushBack ((uint8_t* )"yz", 2) == 2);
  assert(cb.pushBack ((uint8_t* )"defghi", 5) == 2);

  cb.clear ();
  assert(cb.pushBack ((uint8_t* )"xy", 1) == 1);
  assert(cb.pushBack ((uint8_t* )"yz", 2) == 2);
  assert(cb.advanceFront (2) == 2);
  assert(cb.pushBack ((uint8_t* )"defghi", 6) == 4);

  std::memset (ch, '?', sizeof(ch));
  assert(cb.popFront (ch, 1) == 1);
  assert(ch[0] == 'z');
  assert(ch[1] == '?');
  assert(cb.popFront (ch, 6) == 4);
  assert(ch[0] == 'd');
  assert(ch[3] == 'g');
  assert(ch[4] == '?');

  // cb.dump();
  os::trace::puts ("'test-bcbuff-debug' succeeded.");
  return 0;
}

