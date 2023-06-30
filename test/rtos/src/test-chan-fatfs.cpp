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

#include <test-chan-fatfs.h>
#include <test-posix-io-api.h>

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/diag/trace.h>

#include <cmsis-plus/posix-io/chan-fatfs-file-system.h>
// #include <cmsis-plus/posix-io/block-device.h>
#include <cmsis-plus/posix-io/file-descriptors-manager.h>

#include "chan-fatfs/diskio.h"     /* Declarations of disk functions */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdexcept>

using namespace os;

static const char* test_name = "Chan FatFS";

void
test_fs (posix::file_system& fs, uint8_t* buff, std::size_t buff_size);

int
test_diskio (posix::block_device& pdrv, /* Physical drive number to be checked (all data on the drive will be lost) */
             std::size_t ncyc, /* Number of test cycles */
             uint8_t* buf, /* Pointer to the working buffer */
             std::size_t sz_buff /* Size of the working buffer in unit of byte */
             );

int
test_chan_fatfs (bool extra __attribute__((unused)))
{
  os::rtos::mutex chmx
    { "ch-mx" };

  for (int i = 0; i < 3; ++i)
    {
      printf ("\n%s - 512/512 fat12, step %d\n", test_name, i);

      using my_block = posix::block_device_implementable<my_block_impl>;

      // >128 & <4085 -> FAT12
      my_block* chbk = new my_block
        { "ch-bk-1", 512u, 512u, 128u };

      static constexpr std::size_t buff_size = FF_MAX_SS + 4;

      uint8_t* buff = new uint8_t[buff_size];
      int res = test_diskio (*chbk, 3, buff, buff_size);
      assert(res == 0);

      posix::chan_fatfs_file_system* fs = new posix::chan_fatfs_file_system
        { "fat-1", *chbk };

      res = fs->device ().open ();
      assert(res != -1);

      // Partition 0, allocation unit 0=auto.
      res = fs->mkfs (FM_FAT | FM_SFD, 0, 0, buff, buff_size);
      assert(res == 0);

      res = fs->device ().close ();
      assert(res == 0);

      test_fs (*fs, buff, buff_size);

      delete fs;
      delete[] buff;
      delete chbk;
    }

    {
      printf ("\n%s - 512/512 fat12 mutex\n", test_name);

      using my_block = posix::block_device_implementable<my_block_impl>;

      // <4085 -> FAT12
      my_block* chbk = new my_block
        { "ch-bk-1m", 512u, 512u, 128u };

      static constexpr std::size_t buff_size = FF_MAX_SS + 4;

      uint8_t* buff = new uint8_t[buff_size];
      int res = test_diskio (*chbk, 3, buff, buff_size);
      assert(res == 0);

      posix::chan_fatfs_file_system_lockable<rtos::mutex>* fs =
          new posix::chan_fatfs_file_system_lockable<rtos::mutex>
            { "fat-1m", *chbk, chmx };

      res = fs->device ().open ();
      assert(res != -1);

      res = fs->mkfs (FM_FAT | FM_SFD, 0, 0, buff, buff_size);
      assert(res == 0);

      res = fs->device ().close ();
      assert(res == 0);

      test_fs (*fs, buff, buff_size);

      delete fs;
      delete[] buff;
      delete chbk;
    }

// The next one are too large to fit the STM32F4DISCO board RAM,
// so can be executed only on the synthetic platform.
#if !defined(OS_IS_CROSS_BUILD)
    {
      printf ("\n%s - 512/512 fat16\n", test_name);

      using my_block = posix::block_device_implementable<my_block_impl>;

      // >4085 && <65535 -> FAT16
      my_block* chbk = new my_block
        { "ch-bk-2", 512u, 512u, 8229u };

      static constexpr std::size_t buff_size = FF_MAX_SS + 4;

      uint8_t* buff = new uint8_t[buff_size];
      int res = test_diskio (*chbk, 3, buff, buff_size);
      assert(res == 0);

      posix::chan_fatfs_file_system* fs = new posix::chan_fatfs_file_system
        { "fat-2", *chbk };

      res = fs->device ().open ();
      assert(res != -1);

      // Partition 0, allocation unit 0=auto.
      res = fs->mkfs (FM_FAT | FM_SFD, 0, 512, buff, buff_size);
      assert(res == 0);

      res = fs->device ().close ();
      assert(res == 0);

      test_fs (*fs, buff, buff_size);

      delete fs;
      delete[] buff;
      delete chbk;
    }

    {
      printf ("\n%s - 512/512 fat32\n", test_name);

      using my_block = posix::block_device_implementable<my_block_impl>;

      // >65535 -> FAT32
      my_block* chbk = new my_block
        { "ch-bk-3", 512u, 512u, 66130u };

      static constexpr std::size_t buff_size = FF_MAX_SS + 4;

      uint8_t* buff = new uint8_t[buff_size];
      int res = test_diskio (*chbk, 3, buff, buff_size);
      assert(res == 0);

      posix::chan_fatfs_file_system* fs = new posix::chan_fatfs_file_system
        { "fat-3", *chbk };

      res = fs->device ().open ();
      assert(res != -1);

      // Partition 0, allocation unit 0=auto.
      res = fs->mkfs (FM_FAT32 | FM_SFD, 0, 0, buff, buff_size);
      assert(res == 0);

      res = fs->device ().close ();
      assert(res == 0);

      test_fs (*fs, buff, buff_size);

      delete fs;
      delete[] buff;
      delete chbk;
    }

    {
      printf ("\n%s - 512/512 exfat\n", test_name);

      using my_block = posix::block_device_implementable<my_block_impl>;

      // >= 1 MB, 2048 blocks; actually 4096 (minimum 4096).
      my_block* chbk = new my_block
        { "ch-bk-4", 512u, 512u, 4096u };

      static constexpr std::size_t buff_size = FF_MAX_SS + 4;

      uint8_t* buff = new uint8_t[buff_size];
      int res = test_diskio (*chbk, 3, buff, buff_size);
      assert(res == 0);

      posix::chan_fatfs_file_system* fs = new posix::chan_fatfs_file_system
        { "fat-4", *chbk };

      res = fs->device ().open ();
      assert(res != -1);

      res = fs->mkfs (FM_EXFAT | FM_SFD, 0, 0, buff, buff_size);
      assert(res == 0);

      res = fs->device ().close ();
      assert(res == 0);

      test_fs (*fs, buff, buff_size);

      delete fs;
      delete[] buff;
      delete chbk;
    }

    {
      printf ("\n%s - 1024/1024 fat\n", test_name);

      using my_block = posix::block_device_implementable<my_block_impl>;

      // >128 & <4085 -> FAT12
      my_block* chbk = new my_block
        { "ch-bk-5", 1024u, 1024u, 128u };

      static constexpr std::size_t buff_size = FF_MAX_SS + 4;

      uint8_t* buff = new uint8_t[buff_size];
      int res = test_diskio (*chbk, 3, buff, buff_size);
      assert(res == 0);

      posix::chan_fatfs_file_system* fs = new posix::chan_fatfs_file_system
        { "fat-5", *chbk };

      res = fs->device ().open ();
      assert(res != -1);

      // Partition 0, allocation unit 0=auto.
      res = fs->mkfs (FM_FAT | FM_SFD, 0, 0, buff, buff_size);
      assert(res == 0);

      res = fs->device ().close ();
      assert(res == 0);

      test_fs (*fs, buff, buff_size);

      delete fs;
      delete[] buff;
      delete chbk;
    }

    {
      printf ("\n%s - 4096/4096 fat\n", test_name);

      using my_block = posix::block_device_implementable<my_block_impl>;

      // >128 & <4085 -> FAT12
      my_block* chbk = new my_block
        { "ch-bk-6", 4096u, 4096u, 128u };

      static constexpr std::size_t buff_size = FF_MAX_SS + 4;

      uint8_t* buff = new uint8_t[buff_size];
      int res = test_diskio (*chbk, 3, buff, buff_size);
      assert(res == 0);

      posix::chan_fatfs_file_system* fs = new posix::chan_fatfs_file_system
        { "fat-6", *chbk };

      res = fs->device ().open ();
      assert(res != -1);

      // Partition 0, allocation unit 0=auto.
      res = fs->mkfs (FM_FAT | FM_SFD, 0, 0, buff, buff_size);
      assert(res == 0);

      res = fs->device ().close ();
      assert(res == 0);

      test_fs (*fs, buff, buff_size);

      delete fs;
      delete[] buff;
      delete chbk;
    }

    {
      printf ("\n%s - 512/4096 fat\n", test_name);

      using my_block = posix::block_device_implementable<my_block_impl>;

      // >128 & <4085 -> FAT12
      my_block* chbk = new my_block
        { "ch-bk-7", 512u, 4096u, 128u };

      static constexpr std::size_t buff_size = FF_MAX_SS + 4;

      uint8_t* buff = new uint8_t[buff_size];
      int res = test_diskio (*chbk, 3, buff, buff_size);
      assert(res == 0);

      posix::chan_fatfs_file_system* fs = new posix::chan_fatfs_file_system
        { "fat-7", *chbk };

      res = fs->device ().open ();
      assert(res != -1);

      // Partition 0, allocation unit 0=auto.
      res = fs->mkfs (FM_FAT | FM_SFD, 0, 0, buff, buff_size);
      assert(res == 0);

      res = fs->device ().close ();
      assert(res == 0);

      test_fs (*fs, buff, buff_size);

      delete fs;
      delete[] buff;
      delete chbk;
    }
#endif

  return 0;
}

// ----------------------------------------------------------------------------

void
test_fs (posix::file_system& fs, uint8_t* buff, std::size_t buff_size)
{

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-macros"
#endif

#define FILE1_NAME "/file1-with-long-name.extension"
#define TEST1_TEXT "baburiba\nhey one\n"

#define DIR1_NAME "/folder1-with-long-name"

#define FILE2_NAME "/file2-with-long-name.extension"
#define TEST2_TEXT "baburiba\nhey one two\n"

#define DIR2_NAME "/folder2-with-long-name"

#define FILE3_NAME "/file3-with-long-name.extension"
#define TEST3_TEXT "baburiba\nhey one two three\n"

#define FILE4_NAME "/file4-with-long-name.extension"
#define TEST4_TEXT "baburiba\nhey one two three four\n"

#define DIR3_NAME "/folder3-with-long-name"

#define FILE5_NAME "/file5-with-long-name.extension"
#define TEST5_TEXT "baburiba\nhey one two three four five\n"

#define DIR4_NAME "/folder4-with-long-name"

#define FILE6_NAME "/file6-with-long-name.extension"
#define TEST6_TEXT "baburiba\nhey one two three four five six\n"

#define MOUNT_NAME "/ram/"

#pragma GCC diagnostic pop

  int res;
  ssize_t sres;

    {
      // Mount as root file system.
      res = fs.mount ();
      assert(res == 0);

      // Must fail, so the sector size is not relevant.
      res = fs.mkfs (FM_FAT | FM_SFD, 0, 512, buff, buff_size);
      assert(res == -1);

        {
          posix::file* f;
          // Write test.
          f = fs.open (FILE1_NAME, O_WRONLY | O_CREAT);
          assert(f != nullptr);

          sres = f->write (TEST1_TEXT, strlen (TEST1_TEXT));
          assert(sres == strlen (TEST1_TEXT));

          res = f->close ();
          assert(res == 0);

          // Read test.
          f = fs.open (FILE1_NAME, O_RDONLY);
          assert(f != nullptr);

          sres = f->read (buff, buff_size);
          assert(sres == strlen (TEST1_TEXT));

          res = f->close ();
          assert(res == 0);

          // Folder creation test.
          fs.mkdir (DIR1_NAME, 0);
          assert(res == 0);

          // Write test with sub-folder.
          f = fs.open (DIR1_NAME FILE2_NAME, O_WRONLY | O_CREAT);
          assert(f != nullptr);

          sres = f->write (TEST2_TEXT, strlen (TEST2_TEXT));
          assert(sres == strlen (TEST2_TEXT));

          res = f->close ();
          assert(res == 0);

#if !(defined(__APPLE__) || defined(__linux__))

          // Fails with clang :-(

          // Read test with sub-folder.
          f = fs.open (DIR1_NAME FILE2_NAME, O_RDONLY);
          assert (f != nullptr);

          sres = f->read (buff, buff_size);
          assert (sres == strlen (TEST2_TEXT));

          res = f->close ();
          assert (res == 0);

#endif

          // --------------------------

          // File system statistics.
          struct statvfs sfs;
          res = fs.statvfs (&sfs);
          assert(res == 0);

          // --------------------------

          posix::directory* d;

          // Opendir test.
          d = fs.opendir ("/");
          assert(d != nullptr);

          struct dirent *dp;
          while (true)
            {
              errno = 0;
              dp = d->read ();

              if (dp == nullptr)
                {
                  break;
                }
              printf ("\"%s\"\n", dp->d_name);
            };

          res = d->close ();
          assert(res == 0);

          d = fs.opendir (DIR1_NAME);
          assert(d != nullptr);

          while (true)
            {
              errno = 0;
              dp = d->read ();

              if (dp == nullptr)
                {
                  break;
                }
              printf ("\"%s\"\n", dp->d_name);
            };

          res = d->close ();
          assert(res == 0);

        }

      // Similar to the above, but using static functions.
        {
          posix::io* f;
          f = posix::open (FILE3_NAME, O_WRONLY | O_CREAT);
          assert(f != nullptr);

          sres = f->write (TEST3_TEXT, strlen (TEST3_TEXT));
          assert(sres == strlen (TEST3_TEXT));

          res = f->close ();
          assert(res == 0);

          f = posix::open (FILE3_NAME, O_RDONLY);
          assert(f != nullptr);

          sres = f->read (buff, buff_size);
          assert(sres == strlen (TEST3_TEXT));

          res = f->close ();
          assert(res == 0);

          posix::mkdir (DIR3_NAME, 0);
          assert(res == 0);

          f = posix::open (DIR3_NAME FILE4_NAME, O_WRONLY | O_CREAT);
          assert(f != nullptr);

          sres = f->write (TEST4_TEXT, strlen (TEST4_TEXT));
          assert(sres == strlen (TEST4_TEXT));

          res = f->close ();
          assert(res == 0);

#if !(defined(__APPLE__) || defined(__linux__))

          // Fails with clang :-(
          f = posix::open (DIR3_NAME FILE4_NAME, O_RDONLY);
          assert (f != nullptr);

          sres = f->read (buff, buff_size);
          assert (sres == strlen (TEST4_TEXT));

          res = f->close ();
          assert (res == 0);

#endif

          // --------------------------

          struct statvfs sfs;
          res = posix::statvfs ("/", &sfs);
          assert(res == 0);

          // --------------------------

          posix::directory* d;

          d = posix::opendir ("/");
          assert(d != nullptr);

          struct dirent *dp;
          while (true)
            {
              errno = 0;
              dp = d->read ();

              if (dp == nullptr)
                {
                  break;
                }
              printf ("\"%s\"\n", dp->d_name);
            };

          res = d->close ();
          assert(res == 0);

          d = posix::opendir (DIR1_NAME);
          assert(d != nullptr);

          while (true)
            {
              errno = 0;
              dp = d->read ();

              if (dp == nullptr)
                {
                  break;
                }
              printf ("\"%s\"\n", dp->d_name);
            };

          res = d->close ();
          assert(res == 0);

          // --------------------------

          res = fs.umount ();
          assert(res == 0);
        }
    }

#if !(defined(__APPLE__) && defined(__clang__))

    {
      // Mount file system below a folder.
      res = fs.mount (MOUNT_NAME);
      assert (res == 0);

        {
          posix::file* f;
          f = fs.open (FILE1_NAME, O_RDONLY);
          assert (f != nullptr);

          sres = f->read (buff, buff_size);
          assert (sres == strlen (TEST1_TEXT));

          res = f->close ();
          assert (res == 0);

#if !(defined(__APPLE__) || defined(__linux__))

          // Fails with clang :-(

          // Read test with sub-folder.
          f = fs.open (DIR1_NAME FILE2_NAME, O_RDONLY);
          assert (f != nullptr);

          sres = f->read (buff, buff_size);
          assert (sres == strlen (TEST2_TEXT));

          res = f->close ();
          assert (res == 0);

#endif

          // --------------------------

          posix::directory* d;

          d = fs.opendir ("/");
          assert (d != nullptr);

          struct dirent *dp;
          while (true)
            {
              errno = 0;
              dp = d->read ();

              if (dp == nullptr)
                {
                  break;
                }
              printf ("\"%s\"\n", dp->d_name);
            };

          res = d->close ();
          assert (res == 0);

          d = fs.opendir (DIR1_NAME);
          assert (d != nullptr);

          while (true)
            {
              errno = 0;
              dp = d->read ();

              if (dp == nullptr)
                {
                  break;
                }
              printf ("\"%s\"\n", dp->d_name);
            };

          res = d->close ();
          assert (res == 0);
        }

      // --------------------------

        {
          posix::io* f;

          f = posix::open (MOUNT_NAME FILE5_NAME, O_WRONLY | O_CREAT);
          assert (f != nullptr);

          sres = f->write (TEST5_TEXT, strlen (TEST5_TEXT));
          assert (sres == strlen (TEST5_TEXT));

          res = f->close ();
          assert (res == 0);

#if !(defined(__APPLE__) || defined(__linux__))

          f = posix::open (MOUNT_NAME FILE5_NAME, O_RDONLY);
          assert (f != nullptr);

          sres = f->read (buff, buff_size);
          assert (sres == strlen (TEST5_TEXT));

          res = f->close ();
          assert (res == 0);

          posix::mkdir (MOUNT_NAME DIR4_NAME, 0);
          assert (res == 0);

          f = posix::open (MOUNT_NAME DIR4_NAME FILE6_NAME, O_WRONLY | O_CREAT);
          assert (f != nullptr);

          sres = f->write (TEST6_TEXT, strlen (TEST6_TEXT));
          assert (sres == strlen (TEST6_TEXT));

          res = f->close ();
          assert (res == 0);

          // --------------------------

          struct statvfs sfs;
          res = posix::statvfs (MOUNT_NAME, &sfs);
          assert (res == 0);

          // --------------------------

          posix::directory* d;

          d = posix::opendir (MOUNT_NAME);
          assert (d != nullptr);

          struct dirent *dp;
          while (true)
            {
              errno = 0;
              dp = d->read ();

              if (dp == nullptr)
                {
                  break;
                }
              printf ("\"%s\"\n", dp->d_name);
            };

          res = d->close ();
          assert (res == 0);

          d = posix::opendir (MOUNT_NAME DIR4_NAME);
          assert (d != nullptr);

          while (true)
            {
              errno = 0;
              dp = d->read ();

              if (dp == nullptr)
                {
                  break;
                }
              printf ("\"%s\"\n", dp->d_name);
            };

          res = d->close ();
          assert (res == 0);

#endif

        }

      // --------------------------

      res = fs.umount ();
      assert (res == 0);
    }
#endif
}

// ----------------------------------------------------------------------------

static DWORD
pn ( /* Pseudo random number generator */
DWORD pns /* 0:Initialize, !0:Read */
)
{
  static DWORD lfsr;
  UINT n;

  if (pns)
    {
      lfsr = pns;
      for (n = 0; n < 32; n++)
        pn (0);
    }
  if (lfsr & 1)
    {
      lfsr >>= 1;
      lfsr ^= 0x80200003;
    }
  else
    {
      lfsr >>= 1;
    }
  return lfsr;
}

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wformat"
#endif

int
test_diskio (posix::block_device& bd, /* Physical drive number to be checked (all data on the drive will be lost) */
             std::size_t ncyc, /* Number of test cycles */
             uint8_t* buf, /* Pointer to the working buffer */
             std::size_t sz_buff /* Size of the working buffer in unit of byte */
             )
{
  UINT n, cc, ns;
  DWORD sz_drv, lba, lba2, sz_eblk, pns = 1;
  WORD sz_sect;
  PDRV pdrv = static_cast<PDRV> (&bd);
  BYTE *pbuff = static_cast<BYTE*> (buf);
  DSTATUS ds;
  DRESULT dr;

  printf ("\ntest_diskio(%p, %u, %p, %u)\n", pdrv, ncyc,
          static_cast<void*> (pbuff), sz_buff);

  assert(sz_buff >= FF_MAX_SS + 4);

  for (cc = 1; cc <= ncyc; cc++)
    {
      printf ("---- Test cycle %u of %u ----\n", cc, ncyc);

      ds = disk_initialize (pdrv);
      assert((ds & STA_NOINIT) == 0);

      sz_drv = 0;
      dr = disk_ioctl (pdrv, GET_SECTOR_COUNT, &sz_drv);
      assert(dr == RES_OK);

      assert(sz_drv >= 12);
      printf (" Number of sectors is %lu\n", sz_drv);

#if FF_MAX_SS != FF_MIN_SS
      sz_sect = 0;
      dr = disk_ioctl (pdrv, GET_SECTOR_SIZE, &sz_sect);
      assert (dr == RES_OK);
      printf (" Size of sector is %u bytes\n", sz_sect);
#else
      sz_sect = FF_MAX_SS;
#endif

      sz_eblk = 0;
      dr = disk_ioctl (pdrv, GET_BLOCK_SIZE, &sz_eblk);
      assert(dr == RES_OK);

      if (dr == RES_OK || sz_eblk >= 2)
        {
          printf (" Size of the erase block is %lu sectors\n", sz_eblk);
        }
      else
        {
          printf (" Size of the erase block is unknown\n");
        }

      /* Single sector write test */
      lba = 0;
      for (n = 0, pn (pns); n < sz_sect; n++)
        pbuff[n] = (BYTE) pn (0);

      dr = disk_write (pdrv, pbuff, lba, 1);
      assert(dr == RES_OK);

      dr = disk_ioctl (pdrv, CTRL_SYNC, nullptr);
      assert(dr == RES_OK);

      memset (pbuff, 0, sz_sect);
      dr = disk_read (pdrv, pbuff, lba, 1);
      assert(dr == RES_OK);

      for (n = 0, pn (pns); n < sz_sect && pbuff[n] == (BYTE) pn (0); n++)
        ;
      assert(n == sz_sect);

      pns++;

      lba = 1;
      ns = static_cast<UINT> (sz_buff / sz_sect);
      if (ns > 4)
        ns = 4;

      for (n = 0, pn (pns); n < (UINT) (sz_sect * ns); n++)
        pbuff[n] = (BYTE) pn (0);

      dr = disk_write (pdrv, pbuff, lba, ns);
      assert(dr == RES_OK);

      dr = disk_ioctl (pdrv, CTRL_SYNC, nullptr);
      assert(dr == RES_OK);

      memset (pbuff, 0, sz_sect * ns);
      dr = disk_read (pdrv, pbuff, lba, ns);
      assert(dr == RES_OK);

      for (n = 0, pn (pns);
          n < (UINT) (sz_sect * ns) && pbuff[n] == (BYTE) pn (0); n++)
        ;
      assert(n == (UINT ) (sz_sect * ns));
      pns++;

      lba = 5;
      for (n = 0, pn (pns); n < sz_sect; n++)
        pbuff[n + 3] = (BYTE) pn (0);

      dr = disk_write (pdrv, pbuff + 3, lba, 1);
      assert(dr == RES_OK);

      dr = disk_ioctl (pdrv, CTRL_SYNC, nullptr);
      assert(dr == RES_OK);

      memset (pbuff + 5, 0, sz_sect);
      dr = disk_read (pdrv, pbuff + 5, lba, 1);
      assert(dr == RES_OK);

      for (n = 0, pn (pns); n < sz_sect && pbuff[n + 5] == (BYTE) pn (0); n++)
        ;
      assert(n == sz_sect);
      pns++;

      if (sz_drv >= 128 + 0x80000000 / (sz_sect / 2))
        {
          lba = 6;
          lba2 = lba + 0x80000000 / (sz_sect / 2);
          for (n = 0, pn (pns); n < (UINT) (sz_sect * 2); n++)
            pbuff[n] = (BYTE) pn (0);

          dr = disk_write (pdrv, pbuff, lba, 1);
          assert(dr == RES_OK);

          dr = disk_write (pdrv, pbuff + sz_sect, lba2, 1);
          assert(dr == RES_OK);

          dr = disk_ioctl (pdrv, CTRL_SYNC, nullptr);
          assert(dr == RES_OK);

          memset (pbuff, 0, sz_sect * 2);
          dr = disk_read (pdrv, pbuff, lba, 1);
          assert(dr == RES_OK);

          dr = disk_read (pdrv, pbuff + sz_sect, lba2, 1);
          assert(dr == RES_OK);

          for (n = 0, pn (pns);
              pbuff[n] == (BYTE) pn (0) && n < (UINT) (sz_sect * 2); n++)
            ;
          assert(n == (UINT ) (sz_sect * 2));
        }
      else
        {
          printf (" Barrier test skipped\n");
        }
      pns++;

      ds = disk_deinitialize (pdrv);
      assert((ds & STA_NOINIT) == 0);
    }

  return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
