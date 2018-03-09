/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2018 Liviu Ionescu.
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

#ifndef CMSIS_PLUS_POSIX_IO_DEVICE_BLOCK_PARTITION_H_
#define CMSIS_PLUS_POSIX_IO_DEVICE_BLOCK_PARTITION_H_

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#include <cmsis-plus/posix-io/device-block.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    /**
     * @brief Block device partition class.
     * @headerfile device-block-partition.h <cmsis-plus/posix-io/device-block-partitions.h>
     * @ingroup cmsis-plus-posix-io-base
     */
    class device_block_partition : public device_block
    {
      // ----------------------------------------------------------------------

    public:

      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      device_block_partition (const char* name, device_block& parent,
                              os::rtos::mutex* mutex = nullptr);

      /**
       * @cond ignore
       */

      // The rule of five.
      device_block_partition (const device_block_partition&) = delete;
      device_block_partition (device_block_partition&&) = delete;
      device_block_partition&
      operator= (const device_block_partition&) = delete;
      device_block_partition&
      operator= (device_block_partition&&) = delete;

      /**
       * @endcond
       */

      virtual
      ~device_block_partition ();

      /**
       * @}
       */

      /**
       * @name Public Member Functions
       * @{
       */

    public:

      void
      configure (blknum_t offset, blknum_t nblocks);

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Private Member Functions
       * @{
       */

    protected:

      virtual int
      do_vopen (const char* path, int oflag, std::va_list args) override;

      virtual ssize_t
      do_read_block (void* buf, blknum_t blknum, std::size_t nblocks) override;

      virtual ssize_t
      do_write_block (const void* buf, blknum_t blknum, std::size_t nblocks)
          override;

      virtual int
      do_close (void) override;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    protected:

      /**
       * @cond ignore
       */

      device_block& parent_;

      blknum_t partition_offset_blocks_ = 0;

      /**
       * @endcond
       */
    };

  } /* namespace posix */
} /* namespace os */

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_DEVICE_BLOCK_PARTITION_H_ */
