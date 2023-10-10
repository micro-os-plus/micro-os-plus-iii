/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2018-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <test-posix-io-api.h>

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/diag/trace.h>

#include <cmsis-plus/posix-io/char-device.h>
#include <cmsis-plus/posix-io/block-device.h>
#include <cmsis-plus/posix-io/block-device-partition.h>
#include <cmsis-plus/posix-io/file-descriptors-manager.h>

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdexcept>

// ----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

// ----------------------------------------------------------------------------

using namespace os;

// ----------------------------------------------------------------------------


void*
my_memcpy (void *dst, const void *src, std::size_t n);

void*
my_memcpy (void *dst, const void *src, std::size_t n)
{
  // os::trace::printf ("%s(%p, %p, %u)\n", __func__, dst, src,
  //                    static_cast<std::size_t> (n));
  return memcpy (dst, src, n);
}

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wpadded"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpadded"
#endif

class my_char_impl : public posix::char_device_impl
{
public:

  my_char_impl (uint8_t* buf, size_t sz);
  my_char_impl (uint8_t* buf, size_t sz, int extra);

  // The rule of five.
  my_char_impl (const my_char_impl&) = delete;
  my_char_impl (my_char_impl&&) = delete;
  my_char_impl&
  operator= (const my_char_impl&) = delete;
  my_char_impl&
  operator= (my_char_impl&&) = delete;

  virtual
  ~my_char_impl () override;

  virtual bool
  do_is_opened (void) override;

  virtual int
  do_vopen (const char* path, int oflag, std::va_list args) override;

  virtual ssize_t
  do_read (void* buf, std::size_t nbyte) override;

  virtual ssize_t
  do_write (const void* buf, std::size_t nbyte) override;

  virtual off_t
  do_lseek (off_t offset, int whence) override;

  virtual int
  do_vioctl (int request, std::va_list args) override;

  virtual void
  do_sync (void) override;

  virtual int
  do_close (void) override;

};

#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

my_char_impl::my_char_impl (uint8_t* buf, size_t sz)
{
  os::trace::printf ("%s()=@%p 1\n", __func__, this);
}

my_char_impl::my_char_impl (uint8_t* buf, size_t sz, int extra)
{
  os::trace::printf ("%s()=@%p 2\n", __func__, this);
}

#pragma GCC diagnostic pop

my_char_impl::~my_char_impl ()
{
  os::trace::printf ("%s() @%p\n", __func__, this);
}

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

int
my_char_impl::do_vopen (const char* path, int oflag, std::va_list args)
{
  return -1;
}

bool
my_char_impl::do_is_opened (void)
{
  return true;
}

ssize_t
my_char_impl::do_read (void* buf, std::size_t nbyte)
{
  return -1;
}

ssize_t
my_char_impl::do_write (const void* buf, std::size_t nbyte)
{
  return -1;
}

off_t
my_char_impl::do_lseek (off_t offset, int whence)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
my_char_impl::do_vioctl (int request, std::va_list args)
{
  return -1;
}

void
my_char_impl::do_sync (void)
{
}

int
my_char_impl::do_close (void)
{
  return -1;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

my_block_impl::my_block_impl (std::size_t bsize, std::size_t esize,
                              std::size_t nblocks)
{
  trace::printf ("my_block_impl::%s()=@%p\n", __func__, this);

  num_blocks_ = nblocks;
  // Align.
  block_logical_size_bytes_ = (bsize + sizeof(elem_t) - 1)
      & (~(sizeof(elem_t) - 1));
  block_physical_size_bytes_ = (esize + sizeof(elem_t) - 1)
      & (~(sizeof(elem_t) - 1));

  arena_ = new elem_t[nblocks * bsize / sizeof(elem_t)];
  memset (static_cast<void*> (arena_), 0xFF, nblocks * bsize / sizeof(elem_t));
}

my_block_impl::~my_block_impl ()
{
  trace::printf ("my_block_impl::%s() @%p\n", __func__, this);

  delete[] arena_;
}

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

int
my_block_impl::do_vopen (const char* path, int oflag, std::va_list args)
{
  return 0;
}

ssize_t
my_block_impl::do_read_block (void* buf, posix::block_device::blknum_t blknum,
                              std::size_t nblocks)
{
#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
  my_memcpy (buf, &arena_[blknum * block_logical_size_bytes_ / sizeof(elem_t)],
             nblocks * block_logical_size_bytes_);
#pragma GCC diagnostic pop
  return static_cast<ssize_t> (nblocks);
}

ssize_t
my_block_impl::do_write_block (const void* buf,
                               posix::block_device::blknum_t blknum,
                               std::size_t nblocks)
{
#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
  my_memcpy (&arena_[blknum * block_logical_size_bytes_ / sizeof(elem_t)], buf,
             nblocks * block_logical_size_bytes_);
#pragma GCC diagnostic pop
  return static_cast<ssize_t> (nblocks);
}

int
my_block_impl::do_vioctl (int request, std::va_list args)
{
  return -1;
}

void
my_block_impl::do_sync (void)
{
}

int
my_block_impl::do_close (void)
{
  return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wweak-template-vtables"
#endif

// Explicit template instantiation.
template class posix::char_device_implementable<my_char_impl>;
using my_char = posix::char_device_implementable<my_char_impl>;

static uint8_t cbuf[4];

static my_char mc
  { "mc", cbuf, sizeof(cbuf) };

static my_char mc2
  { "mc2", cbuf, sizeof(cbuf), 7 };

// ----------------------------------------------------------------------------

// Explicit template instantiation.
template class posix::block_device_implementable<my_block_impl>;

// Explicit template instantiation.
template class posix::block_device_lockable<my_block_impl, os::rtos::mutex>;
using my_block = posix::block_device_lockable<my_block_impl, os::rtos::mutex>;

static os::rtos::mutex mx1
  { "mx1" };

// /dev/mb
static my_block mb
  { "mb", mx1, 512u, 512u, 2u + 3u };

// Explicit template instantiation.
template class posix::block_device_partition_implementable<>;
using my_partition1 = posix::block_device_partition_implementable<>;

// /dev/mb1
static my_partition1 p1
  { "mb-p1", mb };

// Explicit template instantiation.
template class posix::block_device_partition_lockable<
    posix::block_device_partition_impl, rtos::mutex>;
using my_partition2 = posix::block_device_partition_lockable<posix::block_device_partition_impl, rtos::mutex>;

static rtos::mutex mx2
  { "mx2" };

// /dev/mb2
// The mutex is not really needed, but it is used to test the template.
static my_partition2 p2
  { "mb-p2", mb, mx2 };

// ----------

// Used to allocate the C file descriptors.
static posix::file_descriptors_manager fdm
  { 5 };

#pragma GCC diagnostic pop

// ----------

static const char* test_name = "Test POSIX I/O";

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

int
test_posix_io_api (bool extra __attribute__((unused)))
{
#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
  static uint8_t* buff;
  buff = new uint8_t[512];
#pragma GCC diagnostic pop

  ssize_t res;

  std::size_t bsz = 0;

  printf ("\n%s - Block device partitions - C++ API\n", test_name);
    {
      posix::block_device::blknum_t bks = 0;

      // The number of blocks is known only after open().
      res = mb.open ();
      assert(res >= 0);

      bks = mb.blocks ();
      bsz = mb.block_logical_size_bytes ();
      assert(bsz > 0);

      res = mb.close ();
      assert(res >= 0);

      static constexpr std::size_t nr = 3;

      // p1 is the large one.
      p1.configure (0, bks - nr);
      // p2 is small, nr blocks, at the end.
      p2.configure (bks - nr, nr);
    }

  printf ("\n%s - Block device locked - C++ API\n", test_name);
    {
      res = p2.open ();
      assert(res >= 0);

      for (std::size_t i = 0; i < p2.blocks (); ++i)
        {
          res = p2.read_block (buff, i);
          assert(res >= 0);
          buff[0] = static_cast<uint8_t> (i);
          buff[bsz - 1] = static_cast<uint8_t> (i);
          res = p2.write_block (buff, i);
          assert(res >= 0);
        }

      for (std::size_t i = 0; i < p2.blocks (); ++i)
        {
          memset (buff, 0xFF, bsz);
          res = p2.read_block (buff, i);
          assert(res >= 0);
          assert(buff[0] == i);
          assert(buff[bsz - 1] == i);
        }

      res = p2.read_block (buff, p2.blocks ());
      assert(res == -1);

      res = p2.write_block (buff, p2.blocks ());
      assert(res == -1);

      p2.close ();
    }

  printf ("\n%s - Block device unlocked - C++ API\n", test_name);
    {
      res = p1.open ();
      assert(res >= 0);

      res = p1.close ();
      assert(res >= 0);
    }

  printf ("\n%s - Block device - intermixed opens - C++ API\n", test_name);
    {
      int res1 = p1.open ();
      assert(res1 >= 0);
      int res2 = p2.open ();
      assert(res2 >= 0);
      res1 = p1.close ();
      assert(res1 >= 0);

      buff[0] = 0xFF;
      res = p2.read_block (buff, 0);
      assert(res >= 0);
      assert(buff[0] == 0);

      res2 = p2.close ();
      assert(res2 >= 0);
    }

#if defined(OS_IS_CROSS_BUILD) && !defined(OS_USE_SEMIHOSTING_SYSCALLS)

  printf ("\n%s - Block device - C API\n", test_name);
    {
      int fd = open ("/dev/mb2", 0);

      buff[0] = 0xFF;
      res = read (fd, buff, bsz);
      assert (res >= 0);
      assert (buff[0] == 0);

      // Should fail due to wrong size.
      res = read (fd, buff, bsz - 1);
      assert (res == -1);

      res = close (fd);
      assert (res >= 0);
    }

#endif

  delete buff;

  return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
