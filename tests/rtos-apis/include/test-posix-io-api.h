/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
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

#ifndef TEST_POSIX_IO_API_H_
#define TEST_POSIX_IO_API_H_

#include <cmsis-plus/posix-io/block-device.h>

#if defined(__cplusplus)
extern "C"
{
#endif

  int
  test_posix_io_api (bool extra);

#if defined(__cplusplus)
}
#endif

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

  class my_block_impl : public os::posix::block_device_impl
  {
  public:

    my_block_impl (std::size_t bsize, std::size_t esize, std::size_t nblocks);

    // The rule of five.
    my_block_impl (const my_block_impl&) = delete;
    my_block_impl (my_block_impl&&) = delete;
    my_block_impl&
    operator= (const my_block_impl&) = delete;
    my_block_impl&
    operator= (my_block_impl&&) = delete;

    virtual
    ~my_block_impl () override;

    virtual int
    do_vopen (const char* path, int oflag, std::va_list args) override;

    virtual ssize_t
    do_read_block (void* buf, blknum_t blknum, std::size_t nblocks) override;

    virtual ssize_t
    do_write_block (const void* buf, blknum_t blknum, std::size_t nblocks)
        override;

    virtual int
    do_vioctl (int request, std::va_list args) override;

    virtual void
    do_sync (void) override;

    virtual int
    do_close (void) override;

  private:
    using elem_t = void*;
    elem_t* arena_;
  };

#pragma GCC diagnostic pop


#endif /* TEST_POSIX_IO_API_H_ */
