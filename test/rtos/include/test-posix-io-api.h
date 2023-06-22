/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
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

#if defined(__cplusplus)
}
#endif

#endif /* TEST_POSIX_IO_API_H_ */
