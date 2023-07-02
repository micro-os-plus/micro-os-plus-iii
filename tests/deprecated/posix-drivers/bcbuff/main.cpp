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

#include "posix-drivers/ByteCircularBuffer.h"
#include <cmsis-plus/diag/trace.h>

#include <cassert>
#include <cstring>

// ----------------------------------------------------------------------------

int
main (int argc __attribute__((unused)), char* argv[] __attribute__((unused)))
{
  uint8_t buff[5];
  os::posix::ByteCircularBuffer cb
    { buff, 5 };

  // Empty buffer.
  assert(cb.size () == 5);
  assert(cb.length () == 0);
  assert(cb.empty ());
  assert(!cb.full ());

  // Low water marks.
  assert(cb.below_low_water_mark ());
  assert(!cb.above_low_water_mark ());

  // No more pops.
  uint8_t ch[6];
  assert(cb.pop_front (&ch[0]) == 0);
  assert(cb.pop_front (ch, 5) == 0);
  assert(cb.advance_front (2) == 0);

  uint8_t* pb;
  assert(cb.front_contiguous_buffer (&pb) == 0);
  pb = nullptr;
  assert(cb.back_contiguous_buffer (&pb) == 5);
  assert(pb == &buff[0]);

  // Full buffer.
  assert(cb.push_back ((uint8_t* )"012345", 5) == 5);
  assert(cb.full ());
  assert(!cb.empty ());

  // No more pushes
  assert(cb.push_back ('?') == 0);
  assert(cb.push_back ((uint8_t* )"012345", 5) == 0);
  assert(cb.advance_back (2) == 0);

  // High water marks.
  assert(cb.above_high_water_mark ());
  assert(!cb.below_high_water_mark ());

  // Array operator.
  assert(cb[2] == '2');

  // Clear.
  cb.clear ();
  assert(cb.empty ());

  //  0 1 2 3 4
  // | |x|x| | |
  // +-+-+-+-+-+
  //    f   b

  assert(cb.push_back ((uint8_t* )"abc", 3) == 3);
  assert(cb.pop_front (&ch[0]) == 1);
  assert(ch[0] == 'a');

  assert(cb.length () == 2);

  assert(!cb.below_low_water_mark ());
  assert(cb.above_low_water_mark ());

  assert(!cb.above_high_water_mark ());
  assert(cb.below_high_water_mark ());

  pb = nullptr;
  assert(cb.front_contiguous_buffer (&pb) == 2);
  assert(pb == &buff[1]);

  pb = nullptr;
  assert(cb.back_contiguous_buffer (&pb) == 2);
  assert(pb == &buff[3]);

  //  0 1 2 3 4
  // | |x|x|x| |
  // +-+-+-+-+-+
  //    f     b

  assert(cb.push_back ('d') == 1);

  pb = nullptr;
  assert(cb.front_contiguous_buffer (&pb) == 3);
  assert(pb == &buff[1]);

  pb = nullptr;
  assert(cb.back_contiguous_buffer (&pb) == 1);
  assert(pb == &buff[4]);

  //  0 1 2 3 4
  // | | | |x| |
  // +-+-+-+-+-+
  //        f b

  assert(cb.pop_front (&ch[0]) == 1);
  assert(ch[0] == 'b');

  assert(cb.pop_front (&ch[0]) == 1);
  assert(ch[0] == 'c');

  pb = nullptr;
  assert(cb.front_contiguous_buffer (&pb) == 1);
  assert(pb == &buff[3]);

  pb = nullptr;
  assert(cb.back_contiguous_buffer (&pb) == 1);
  assert(pb == &buff[4]);

  //  0 1 2 3 4
  // | | | |x|x|
  // +-+-+-+-+-+
  //  b     f

  assert(cb.push_back ('e') == 1);

  pb = nullptr;
  assert(cb.front_contiguous_buffer (&pb) == 2);
  assert(pb == &buff[3]);

  pb = nullptr;
  assert(cb.back_contiguous_buffer (&pb) == 3);
  assert(pb == &buff[0]);

  //  0 1 2 3 4
  // | | | |x|x|
  // +-+-+-+-+-+
  //  b     f

  assert(cb.push_back ('f') == 1);

  pb = nullptr;
  assert(cb.front_contiguous_buffer (&pb) == 2);
  assert(pb == &buff[3]);

  pb = nullptr;
  assert(cb.back_contiguous_buffer (&pb) == 2);
  assert(pb == &buff[1]);

  // push_back/pop_front buffer
  cb.clear ();
  assert(cb.push_back ((uint8_t* )"xy", 1) == 1);
  assert(cb.push_back ((uint8_t* )"yz", 2) == 2);
  assert(cb.push_back ((uint8_t* )"defghi", 5) == 2);

  cb.clear ();
  assert(cb.push_back ((uint8_t* )"xy", 1) == 1);
  assert(cb.push_back ((uint8_t* )"yz", 2) == 2);
  assert(cb.advance_front (2) == 2);
  assert(cb.push_back ((uint8_t* )"defghi", 6) == 4);

  std::memset (ch, '?', sizeof(ch));
  assert(cb.pop_front (ch, 1) == 1);
  assert(ch[0] == 'z');
  assert(ch[1] == '?');
  assert(cb.pop_front (ch, 6) == 4);
  assert(ch[0] == 'd');
  assert(ch[3] == 'g');
  assert(ch[4] == '?');

  // cb.dump();
  os::trace::puts ("'test-bcbuff-debug' succeeded.");
  return 0;
}

