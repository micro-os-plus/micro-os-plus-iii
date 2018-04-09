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

using namespace os;

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

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
#pragma GCC diagnostic ignored "-Wunused-parameter"

my_char_impl::my_char_impl (uint8_t* buf, size_t sz)
{
  os::trace::printf ("%s()=@%p 1\n", __func__, this);
}

my_char_impl::my_char_impl (uint8_t* buf, size_t sz,
                            int extra)
{
  os::trace::printf ("%s()=@%p 2\n", __func__, this);
}

#pragma GCC diagnostic pop

my_char_impl::~my_char_impl ()
{
  os::trace::printf ("%s() @%p\n", __func__, this);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

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
  ;
}

int
my_char_impl::do_close (void)
{
  return -1;
}

#pragma GCC diagnostic pop

class my_block_impl : public posix::block_device_impl
{
public:

  my_block_impl (void);

  // The rule of five.
  my_block_impl (const my_block_impl&) = delete;
  my_block_impl (my_block_impl&&) = delete;
  my_block_impl&
  operator= (const my_block_impl&) = delete;
  my_block_impl&
  operator= (my_block_impl&&) = delete;

  virtual
  ~my_block_impl () override;

  virtual bool
  do_is_opened (void) override;

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
  static constexpr std::size_t BSZ = 512;
  static constexpr std::size_t SZ = 128 + 3;
  uint32_t arena_[SZ][(BSZ / sizeof(uint32_t))];
};

my_block_impl::my_block_impl (void)
{
  trace::printf ("my_block_impl::%s()=@%p\n", __func__, this);

  memset (arena_, 0xFF, sizeof(arena_));
}

my_block_impl::~my_block_impl ()
{
  trace::printf ("my_block_impl::%s() @%p\n", __func__, this);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

bool
my_block_impl::do_is_opened (void)
{
  return true;
}

int
my_block_impl::do_vopen (const char* path, int oflag, std::va_list args)
{

  num_blocks_ = SZ;
  block_logical_size_bytes_ = BSZ;
  block_physical_size_bytes_ = BSZ;

  return 0;
}

ssize_t
my_block_impl::do_read_block (void* buf, posix::block_device::blknum_t blknum,
                              std::size_t nblocks)
{
  memcpy (buf, &arena_[blknum], nblocks * BSZ);
  return static_cast<ssize_t>(nblocks);
}

ssize_t
my_block_impl::do_write_block (const void* buf,
                               posix::block_device::blknum_t blknum,
                               std::size_t nblocks)
{
  memcpy (&arena_[blknum], buf, nblocks * BSZ);
  return static_cast<ssize_t>(nblocks);
}

int
my_block_impl::do_vioctl (int request, std::va_list args)
{
  return -1;
}

void
my_block_impl::do_sync (void)
{
  ;
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
template class posix::block_device_lockable<my_block_impl, os::rtos::mutex>;
using my_block = posix::block_device_lockable<my_block_impl, os::rtos::mutex>;

static os::rtos::mutex mx1
  { "mx1" };

// /dev/mb
static my_block mb
  { "mb", mx1 };


// Explicit template instantiation.
template class posix::block_device_partition_implementable<>;
using my_partition1 = posix::block_device_partition_implementable<>;

// /dev/mb1
static my_partition1 p1
  { "mb1", mb };

// Explicit template instantiation.
template class posix::block_device_partition_lockable<
    posix::block_device_partition_impl, rtos::mutex>;
using my_partition2 = posix::block_device_partition_lockable<posix::block_device_partition_impl, rtos::mutex>;

static rtos::mutex mx2
  { "mx2" };

// /dev/mb2
// The mutex is not really needed, but it is used to test the template.
static my_partition2 p2
  { "mb2", mb, mx2 };

// ----------

// Used to allocate the C file descriptors.
static posix::file_descriptors_manager fdm
  { 5 };

#pragma GCC diagnostic pop


// ----------

static uint8_t buff[512 * 4];

static const char* test_name = "Test POSIX I/O";

#pragma GCC diagnostic push
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

int
test_posix_io_api (bool extra __attribute__((unused)))
{

  ssize_t res;

  std::size_t bsz = 0;

  printf ("\n%s - Block device partitions - C++ API.\n", test_name);
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

  printf ("\n%s - Block device locked - C++ API.\n", test_name);
    {
      res = p2.open ();
      assert(res >= 0);

      for (std::size_t i = 0; i < p2.blocks (); ++i)
        {
          res = p2.read_block (buff, i);
          assert(res >= 0);
          buff[0] = static_cast<uint8_t> (i);
          res = p2.write_block (buff, i);
          assert(res >= 0);
        }

      for (std::size_t i = 0; i < p2.blocks (); ++i)
        {
          buff[0] = 0xFF;
          res = p2.read_block (buff, i);
          assert(res >= 0);
          assert(buff[0] == i);
        }

      res = p2.read_block (buff, p2.blocks ());
      assert(res == -1);

      res = p2.write_block (buff, p2.blocks ());
      assert(res == -1);

      p2.close ();
    }

  printf ("\n%s - Block device unlocked - C++ API.\n", test_name);
    {
      res = p1.open ();
      assert(res >= 0);

      res = p1.close ();
      assert(res >= 0);
    }

  printf ("\n%s - Block device - intermixed opens - C++ API.\n", test_name);
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

  printf ("\n%s - Block device - C API.\n", test_name);
    {
      int fd = open ("/dev/mb2", 0);

      buff[0] = 0xFF;
      res = read (fd, buff, bsz);
      assert(res >= 0);
      assert(buff[0] == 0);

      // Should fail due to wrong size.
      res = read (fd, buff, bsz - 1);
      assert(res == -1);

      res = close (fd);
      assert(res >= 0);
    }

#endif

  return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

