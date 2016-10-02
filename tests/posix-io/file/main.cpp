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

#include "posix-io/file_descriptors_manager.h"
#include "posix-io/io.h"
#include "posix-io/file.h"
#include "posix-io/file-system.h"
#include "posix-io/TPool.h"
#include "posix-io/mount_manager.h"
#include "posix-io/device_block.h"
#include <cmsis-plus/diag/trace.h>

#include <cerrno>
#include <cassert>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include "utime.h"
#include <unistd.h>
#include <sys/stat.h>

#if defined(__ARM_EABI__)

extern "C"
{
  void
  sync (void);

  int
  open (const char* path, int oflag, ...);

  ssize_t
  writev (int fildes, const struct iovec* iov, int iovcnt);

  int
  fcntl (int fildes, int cmd, ...);
}

#include "posix-io/redefinitions.h"

#endif

// ----------------------------------------------------------------------------

enum class Cmds
  : unsigned int
    { UNKNOWN,
  NOTSET,
  SYNC,
  CHMOD,
  STAT,
  TRUNCATE,
  RENAME,
  UNLINK,
  UTIME,
  MKDIR,
  RMDIR,
  OPEN,
  CLOSE,
  READ,
  WRITE,
  WRITEV,
  IOCTL,
  LSEEK,
  ISATTY,
  FCNTL,
  FSTAT,
  FTRUNCATE,
  FSYNC
};

// Test class, all methods store the input in local variables,
// to be checked later.

class TestFile : public os::posix::file
{
public:

  TestFile ();

  // Methods used for test purposes only.

  void
  clear (void);

  Cmds
  getCmd (void);

  int
  getMode (void);

  const char*
  getPath (void);

  unsigned int
  getNumber (void);

  void*
  getPtr (void);

protected:

  // Implementations.

  virtual int
  do_vopen (const char* path, int oflag, std::va_list args) override;

  virtual int
  do_close (void) override;

  virtual ssize_t
  do_read (void* buf, std::size_t nbyte) override;

  virtual ssize_t
  do_write (const void* buf, std::size_t nbyte) override;

  virtual ssize_t
  do_writev (const struct iovec* iov, int iovcnt) override;

  virtual off_t
  do_lseek (off_t offset, int whence) override;

  virtual int
  do_isatty (void) override;

  virtual int
  do_vfcntl (int cmd, va_list args) override;

  virtual int
  do_fstat (struct stat* buf) override;

  virtual int
  do_ftruncate (off_t length) override;

  virtual int
  do_fsync (void) override;

private:

  uint32_t fSomething;
  const char* fPath;
  int fMode;
  unsigned int fNumber;
  void* fPtr;
  Cmds fCmd;

};

TestFile::TestFile ()
{
  clear ();
}

void
TestFile::clear (void)
{
  fCmd = Cmds::NOTSET;
  fPath = nullptr;
  fMode = -1;
  fSomething = 1;
  fNumber = 1;
  fPtr = nullptr;
}

inline Cmds
TestFile::getCmd (void)
{
  return fCmd;
}

inline unsigned int
TestFile::getNumber (void)
{
  return fNumber;
}

inline int
TestFile::getMode (void)
{
  return fMode;
}

inline const char*
TestFile::getPath (void)
{
  return fPath;
}

inline void*
TestFile::getPtr (void)
{
  return fPtr;
}

int
TestFile::do_vopen (const char* path, int oflag, std::va_list args)
{
  fPath = path;

  fNumber = oflag;

  fMode = va_arg(args, int);
  return 0;
}

int
TestFile::do_close (void)
{
  fCmd = Cmds::CLOSE;
  return 0; // Always return success
}

ssize_t
TestFile::do_read (void *buf, std::size_t nbyte)
{
  fCmd = Cmds::READ;
  fPtr = buf;
  fNumber = nbyte;
  return nbyte / 2;
}

ssize_t
TestFile::do_write (const void* buf, std::size_t nbyte)
{
  fCmd = Cmds::WRITE;
  fPtr = (void*) buf;
  fNumber = nbyte;
  return nbyte / 2;
}

ssize_t
TestFile::do_writev (const struct iovec* iov, int iovcnt)
{
  fCmd = Cmds::WRITEV;
  fPtr = (void*) iov;
  fNumber = iovcnt;
  return 0;
}

off_t
TestFile::do_lseek (off_t offset, int whence)
{
  fCmd = Cmds::LSEEK;
  fNumber = offset;
  fMode = whence;
  return 0;
}

int
TestFile::do_isatty (void)
{
  fCmd = Cmds::ISATTY;
  return 0;
}

int
TestFile::do_vfcntl (int cmd, std::va_list args)
{
  fCmd = Cmds::FCNTL;
  fNumber = cmd;
  fMode = va_arg(args, int);
  return 0;
}

int
TestFile::do_fstat (struct stat* buf)
{
  fCmd = Cmds::FSTAT;
  fPtr = buf;
  return 0;
}

int
TestFile::do_ftruncate (off_t length)
{
  fCmd = Cmds::FTRUNCATE;
  fNumber = length;
  return 0;
}

int
TestFile::do_fsync (void)
{
  fCmd = Cmds::FSYNC;
  return 0;
}

// ----------------------------------------------------------------------------

class TestFileSystem : public os::posix::file_system
{
public:

  TestFileSystem (os::posix::pool* filesPool, os::posix::pool* dirsPool);

  // Methods used for test purposes only.
  unsigned int
  getFlags (void);

  Cmds
  getCmd (void);

  unsigned int
  getSyncCount (void);

  unsigned int
  getNumber (void);

  const char*
  getPath (void);

  void*
  getPtr (void);

protected:

  // Implementations.

  virtual int
  do_mount (unsigned int flags) override;

  virtual int
  do_unmount (unsigned int flags) override;

  virtual void
  do_sync (void) override;

  virtual int
  do_chmod (const char* path, mode_t mode) override;

  virtual int
  do_stat (const char* path, struct stat* buf) override;

  virtual int
  do_truncate (const char* path, off_t length) override;

  virtual int
  do_rename (const char* existing, const char* _new) override;

  virtual int
  do_unlink (const char* path) override;

  virtual int
  do_utime (const char* path, const struct utimbuf* times) override;

  virtual int
  do_mkdir (const char* path, mode_t mode) override;

  virtual int
  do_rmdir (const char* path) override;

private:

  unsigned int fMountFlags;
  Cmds fCmd;
  unsigned int fSyncCount;
  const char* fPath;
  unsigned int fNumber;
  void* fPtr;

};

TestFileSystem::TestFileSystem (os::posix::pool* filesPool,
                                os::posix::pool* dirsPool) :
    os::posix::file_system (filesPool, dirsPool)
{
  fMountFlags = 1;
  fCmd = Cmds::NOTSET;
  fSyncCount = 1;
  fPath = nullptr;
  fNumber = 0;
  fPtr = nullptr;
}

inline unsigned int
TestFileSystem::getFlags (void)
{
  return fMountFlags;
}

inline Cmds
TestFileSystem::getCmd (void)
{
  return fCmd;
}

inline unsigned int
TestFileSystem::getSyncCount (void)
{
  return fSyncCount;
}

inline unsigned int
TestFileSystem::getNumber (void)
{
  return fNumber;
}

inline const char*
TestFileSystem::getPath (void)
{
  return fPath;
}

inline void*
TestFileSystem::getPtr (void)
{
  return fPtr;
}

// ----------------------------------------------------------------------------

int
TestFileSystem::do_mount (unsigned int flags)
{
  fMountFlags = flags;

  return 0;
}

int
TestFileSystem::do_unmount (unsigned int flags)
{
  fMountFlags = flags;

  return 0;
}

void
TestFileSystem::do_sync (void)
{
  fSyncCount++;
}

int
TestFileSystem::do_chmod (const char* path, mode_t mode)
{
  fCmd = Cmds::CHMOD;
  fPath = path;
  fNumber = mode;
  return 0;
}

int
TestFileSystem::do_stat (const char* path, struct stat* buf)
{
  fCmd = Cmds::STAT;
  fPath = path;
  fPtr = buf;
  return 0;
}

int
TestFileSystem::do_truncate (const char* path, off_t length)
{
  fCmd = Cmds::TRUNCATE;
  fPath = path;
  fNumber = length;
  return 0;
}

int
TestFileSystem::do_rename (const char* existing, const char* _new)
{
  fCmd = Cmds::RENAME;
  fPath = existing;
  fPtr = (void*) _new;
  return 0;
}

int
TestFileSystem::do_unlink (const char* path)
{
  fCmd = Cmds::UNLINK;
  fPath = path;
  return 0;
}

int
TestFileSystem::do_utime (const char* path, const struct utimbuf* times)
{
  fCmd = Cmds::UTIME;
  fPath = path;
  fPtr = (void*) times;
  return 0;
}

int
TestFileSystem::do_mkdir (const char* path, mode_t mode)
{
  fCmd = Cmds::MKDIR;
  fPath = path;
  fNumber = mode;
  return 0;
}

int
TestFileSystem::do_rmdir (const char* path)
{
  fCmd = Cmds::RMDIR;
  fPath = path;
  return 0;
}

// ----------------------------------------------------------------------------

// Required only as a reference, no functionality needed.
class TestBlockDevice : public os::posix::device_block
{
public:
  TestBlockDevice () = default;
};

// ----------------------------------------------------------------------------

using TestFilePool = os::posix::TPool<TestFile>;

constexpr std::size_t FILES_POOL_ARRAY_SIZE = 2;

// pool of file objects, used in common by all filesystems.
TestFilePool filesPool
  { FILES_POOL_ARRAY_SIZE };

// file systems, all using the same pool.
TestFileSystem root_fs
  { &filesPool, nullptr };
TestFileSystem fs1
  { &filesPool, nullptr };
TestFileSystem fs2
  { &filesPool, nullptr };

// Static manager
os::posix::file_descriptors_manager dm
  { 5 };

// Static manager
os::posix::mount_manager mm
  { 2 };

// Block devices, just referenced, no calls forwarded to them.
TestBlockDevice root_dev;
TestBlockDevice dev1;
TestBlockDevice dev2;

// ----------------------------------------------------------------------------

int
main (int argc __attribute__((unused)), char* argv[] __attribute__((unused)))
{
    {
      // ----- mount_manager -----

      // Check initial size.
      assert(mm.getSize () == 2);

      // Check if FileSystemsManger is empty.
      for (std::size_t i = 0; i < mm.getSize (); ++i)
        {
          assert(mm.getFileSystem (i) == nullptr);
          assert(mm.getPath (i) == nullptr);
        }
      assert(mm.getRoot () == nullptr);

      const char* path1 = "/babu/riba";
      const char* path2 = path1;

      // No file system, identify nothing
      assert(
          os::posix::mount_manager::identifyFileSystem (&path2, nullptr)
              == nullptr);

      // Check if root_fs file system flags are those set by constructor.
      assert(root_fs.getFlags () == 1);

      // Check setRoot(), and mount().
      assert(os::posix::mount_manager::setRoot (&root_fs, &root_dev, 123) == 0);
      assert(os::posix::mount_manager::getRoot () == &root_fs);
      assert(root_fs.getBlockDevice () == &root_dev);

      // Check mount flags.
      assert(root_fs.getFlags () == 123);

      // No file systems mounted, identify root.
      assert(
          os::posix::mount_manager::identifyFileSystem (&path2, nullptr)
              == &root_fs);
      assert(path2 == path1);
    }

    {
      // ----- mount_manager mounts & umounts -----

      errno = -2;
      assert(
          (os::posix::mount_manager::mount (&fs1, "/fs1/", &dev1, 124) == 0) && (errno == 0));
      assert(os::posix::mount_manager::getFileSystem (0) == &fs1);
      assert(fs1.getBlockDevice () == &dev1);

      assert(fs1.getFlags () == 124);

      // Check not mounted file, should return root
      const char* path1 = "/baburiba";
      const char* path2 = path1;

      assert(
          os::posix::mount_manager::identifyFileSystem (&path2, nullptr)
              == &root_fs);
      assert(path2 == path1);

      // Check busy error
      errno = -2;
      assert(os::posix::mount_manager::mount (&fs1, "/fs1/", &dev1, 124) == -1);
      assert(errno == EBUSY);

      path1 = "/fs1/babu";
      path2 = path1;

      const char* path3 = "/fs1/riba";
      const char* path4 = path3;

      // Check if identified properly
      assert(
          os::posix::mount_manager::identifyFileSystem (&path2, &path4) == &fs1);

      // Check if path adjusted properly
      assert(path2 == (path1 + std::strlen ("/fs1")));
      assert(path4 == (path3 + std::strlen ("/fs1")));

      // Check size exceeded
      errno = -2;
      assert(
          (os::posix::mount_manager::mount (&fs2, "/fs2/", &dev2, 124) == 0) && (errno == 0));
      errno = -2;
      assert(os::posix::mount_manager::mount (&fs2, "/fs3/", &dev2, 124) == -1);
      assert(errno == ENOENT);

      // Check umounts
      unsigned int cnt = fs1.getSyncCount ();
      errno = -2;
      assert(
          (os::posix::mount_manager::umount ("/fs1/", 134) == 0) && (errno == 0));
      assert(fs1.getFlags () == 134);
      assert(fs1.getSyncCount () == cnt + 1);
      assert(fs1.getBlockDevice () == nullptr);

      // Check umounts
      cnt = fs2.getSyncCount ();
      errno = -2;
      assert(
          (os::posix::mount_manager::umount ("/fs2/", 144) == 0) && (errno == 0));
      assert(fs2.getFlags () == 144);
      assert(fs2.getSyncCount () == cnt + 1);
      assert(fs2.getBlockDevice () == nullptr);
    }

    {
      // Mount again
      errno = -2;
      assert(
          (os::posix::mount_manager::mount (&fs1, "/fs1/", &dev1, 124) == 0) && (errno == 0));
    }

    {
      // C API

      // CHMOD
      errno = -2;
      assert((__posix_chmod ("/fs1/p1", 321) == 0) && (errno == 0));
      assert(fs1.getCmd () == Cmds::CHMOD);
      assert(fs1.getNumber () == 321);
      assert(std::strcmp ("/p1", fs1.getPath ()) == 0);

      // STAT
      errno = -2;
      struct stat stat_buf;
      assert((__posix_stat ("/fs1/p2", &stat_buf) == 0) && (errno == 0));
      assert(fs1.getCmd () == Cmds::STAT);
      assert(fs1.getPtr () == &stat_buf);
      assert(std::strcmp ("/p2", fs1.getPath ()) == 0);

      // TRUNCATE
      errno = -2;
      assert((__posix_truncate ("/fs1/p3", 876) == 0) && (errno == 0));
      assert(fs1.getCmd () == Cmds::TRUNCATE);
      assert(fs1.getNumber () == 876);
      assert(std::strcmp ("/p3", fs1.getPath ()) == 0);

      // RENAME
      errno = -2;
      assert((__posix_rename ("/fs1/p4", "/fs1/p4-new") == 0) && (errno == 0));
      assert(fs1.getCmd () == Cmds::RENAME);
      assert(std::strcmp ("/p4", fs1.getPath ()) == 0);
      assert(std::strcmp ("/p4-new", (const char* )fs1.getPtr ()) == 0);

      // UNLINK
      errno = -2;
      assert((__posix_unlink ("/fs1/p5") ==0) && (errno == 0));
      assert(fs1.getCmd () == Cmds::UNLINK);
      assert(std::strcmp ("/p5", fs1.getPath ()) == 0);

      // UTIME
      errno = -2;
      struct utimbuf times;
      assert((__posix_utime ("/fs1/p6", &times) ==0) && (errno == 0));
      assert(fs1.getCmd () == Cmds::UTIME);
      assert(fs1.getPtr () == &times);
      assert(std::strcmp ("/p6", fs1.getPath ()) == 0);

      // MKDIR
      errno = -2;
      assert((__posix_mkdir ("/fs1/p7", 654) ==0) && (errno == 0));
      assert(fs1.getCmd () == Cmds::MKDIR);
      assert(fs1.getNumber () == 654);
      assert(std::strcmp ("/p7", fs1.getPath ()) == 0);

      // RMDIR
      errno = -2;
      assert((__posix_rmdir ("/fs1/p8") ==0) && (errno == 0));
      assert(fs1.getCmd () == Cmds::RMDIR);
      assert(std::strcmp ("/p8", fs1.getPath ()) == 0);

      // SYNC
      unsigned int cnt = fs1.getSyncCount ();
      errno = -2;
      __posix_sync ();
      assert(errno == 0);
      assert(fs1.getCmd () == Cmds::RMDIR);
      assert(fs1.getSyncCount () == cnt + 1);
    }

    {
      // C++ API

      // CHMOD
      errno = -2;
      assert((os::posix::chmod ("/fs1/p1", 321) == 0) && (errno == 0));
      assert(fs1.getCmd () == Cmds::CHMOD);
      assert(fs1.getNumber () == 321);
      assert(std::strcmp ("/p1", fs1.getPath ()) == 0);

      // STAT
      errno = -2;
      struct stat stat_buf;
      assert((os::posix::stat ("/fs1/p2", &stat_buf) == 0) && (errno == 0));
      assert(fs1.getCmd () == Cmds::STAT);
      assert(fs1.getPtr () == &stat_buf);
      assert(std::strcmp ("/p2", fs1.getPath ()) == 0);

      // TRUNCATE
      errno = -2;
      assert((os::posix::truncate ("/fs1/p3", 876) == 0) && (errno == 0));
      assert(fs1.getCmd () == Cmds::TRUNCATE);
      assert(fs1.getNumber () == 876);
      assert(std::strcmp ("/p3", fs1.getPath ()) == 0);

      // RENAME
      errno = -2;
      assert(
          (os::posix::rename ("/fs1/p4", "/fs1/p4-new") == 0) && (errno == 0));
      assert(fs1.getCmd () == Cmds::RENAME);
      assert(std::strcmp ("/p4", fs1.getPath ()) == 0);
      assert(std::strcmp ("/p4-new", (const char* )fs1.getPtr ()) == 0);

      // UNLINK
      errno = -2;
      assert((os::posix::unlink ("/fs1/p5") ==0) && (errno == 0));
      assert(fs1.getCmd () == Cmds::UNLINK);
      assert(std::strcmp ("/p5", fs1.getPath ()) == 0);

      // UTIME
      errno = -2;
      struct utimbuf times;
      assert((os::posix::utime ("/fs1/p6", &times) ==0) && (errno == 0));
      assert(fs1.getCmd () == Cmds::UTIME);
      assert(fs1.getPtr () == &times);
      assert(std::strcmp ("/p6", fs1.getPath ()) == 0);

      // MKDIR
      errno = -2;
      assert((os::posix::mkdir ("/fs1/p7", 654) ==0) && (errno == 0));
      assert(fs1.getCmd () == Cmds::MKDIR);
      assert(fs1.getNumber () == 654);
      assert(std::strcmp ("/p7", fs1.getPath ()) == 0);

      // RMDIR
      errno = -2;
      assert((os::posix::rmdir ("/fs1/p8") ==0) && (errno == 0));
      assert(fs1.getCmd () == Cmds::RMDIR);
      assert(std::strcmp ("/p8", fs1.getPath ()) == 0);

      // SYNC
      unsigned int cnt = fs1.getSyncCount ();
      errno = -2;
      os::posix::sync ();
      assert(errno == 0);
      assert(fs1.getCmd () == Cmds::RMDIR);
      assert(fs1.getSyncCount () == cnt + 1);
    }

    {
      // C API

      // Test OPEN
      errno = -2;
      int fd = __posix_open ("/fs1/f1", 123, 234);
      assert((fd >= 0) && (errno == 0));

      os::posix::io* io = os::posix::file_descriptors_manager::getIo (fd);
      assert(io != nullptr);

      assert(io->getType () == os::posix::io::Type::FILE);

      TestFile* file = static_cast<TestFile*> (io);
      // Must be the first used slot in the pool.
      assert(filesPool.getObject (0) == file);
      assert(filesPool.getFlag (0) == true);

      // Check params passing.
      assert(std::strcmp ("/f1", file->getPath ()) == 0);
      assert(file->getNumber () == 123);
      assert(file->getMode () == 234);

      int ret;

      // Test READ
      errno = -2;
      file->clear ();
      char buf[3];
      ret = __posix_read (fd, (void*) buf, 320);
      assert((ret == (320/2)) && (errno == 0));
      assert(file->getCmd () == Cmds::READ);
      assert(file->getPtr () == buf);
      assert(file->getNumber () == 320);

      // Test WRITE
      errno = -2;
      file->clear ();
      ret = __posix_write (fd, (const void*) buf, 432);
      assert((ret == (432/2)) && (errno == 0));
      assert(file->getCmd () == Cmds::WRITE);
      assert(file->getPtr () == buf);
      assert(file->getNumber () == 432);

      // Test WRITEV
      errno = -2;
      file->clear ();
      ret = __posix_writev (fd, (const struct iovec*) buf, 234);
      assert((ret == 0) && (errno == 0));
      assert(file->getCmd () == Cmds::WRITEV);
      assert(file->getPtr () == buf);
      assert(file->getNumber () == 234);

      // Test LSEEK
      errno = -2;
      file->clear ();
      ret = __posix_lseek (fd, 333, 555);
      assert((ret == 0) && (errno == 0));
      assert(file->getCmd () == Cmds::LSEEK);
      assert(file->getNumber () == 333);
      assert(file->getMode () == 555);

      // Test ISATTY
      errno = -2;
      file->clear ();
      ret = __posix_isatty (fd);
      assert((ret == 0) && (errno == 0));
      assert(file->getCmd () == Cmds::ISATTY);

      // Test FCNTL
      errno = -2;
      file->clear ();
      ret = __posix_fcntl (fd, 444, 987);
      assert((ret == 0) && (errno == 0));
      assert(file->getCmd () == Cmds::FCNTL);
      assert(file->getNumber () == 444);
      assert(file->getMode () == 987);

      // Test FSTAT
      errno = -2;
      file->clear ();
      struct stat stat_buf;
      ret = __posix_fstat (fd, &stat_buf);
      assert((ret == 0) && (errno == 0));
      assert(file->getCmd () == Cmds::FSTAT);
      assert(file->getPtr () == &stat_buf);

      // Test FTRUNCATE
      errno = -2;
      file->clear ();
      ret = __posix_ftruncate (fd, 999);
      assert((ret == 0) && (errno == 0));
      assert(file->getCmd () == Cmds::FTRUNCATE);
      assert(file->getNumber () == 999);

      // Test FSYNC
      errno = -2;
      file->clear ();
      ret = __posix_fsync (fd);
      assert((ret == 0) && (errno == 0));
      assert(file->getCmd () == Cmds::FSYNC);

      // Test CLOSE
      errno = -2;
      file->clear ();
      ret = __posix_close (fd);
      assert((ret == 0) && (errno == 0));
      assert(file->getCmd () == Cmds::CLOSE);

      // Must no longer be in the pool
      assert(filesPool.getFlag (0) == false);
    }

    {
      // C++ API

      // Test OPEN via namespace generic call.
      errno = -2;
      os::posix::io* io = os::posix::open ("/fs1/f0", 124, 235);
      assert((io != nullptr) && (errno == 0));

      assert(io->getType () == os::posix::io::Type::FILE);

      TestFile* tfile = static_cast<TestFile*> (io);

      // Must be the first used slot in the pool.
      assert(filesPool.getObject (0) == tfile);
      assert(filesPool.getFlag (0) == true);

      // Check params passing.
      assert(std::strcmp ("/f0", tfile->getPath ()) == 0);
      assert(tfile->getNumber () == 124);
      assert(tfile->getMode () == 235);

      // Test CLOSE
      errno = -2;
      int ret = io->close ();
      assert((ret == 0) && (errno == 0));
      assert(tfile->getCmd () == Cmds::CLOSE);

      // Must no longer be in the pool
      assert(filesPool.getFlag (0) == false);
    }

    {
      // Test OPEN via class call.
      errno = -2;
      os::posix::file* file = os::posix::file::open ("/fs1/f1", 123, 234);
      assert((file != nullptr) && (errno == 0));

      assert(file->getType () == os::posix::io::Type::FILE);

      TestFile* tfile = static_cast<TestFile*> (file);
      // Must be the first used slot in the pool.
      assert(filesPool.getObject (0) == tfile);
      assert(filesPool.getFlag (0) == true);

      // Check params passing.
      assert(std::strcmp ("/f1", tfile->getPath ()) == 0);
      assert(tfile->getNumber () == 123);
      assert(tfile->getMode () == 234);

      int ret;

      // Test READ
      errno = -2;
      char buf[3];
      ret = file->read ((void*) buf, 320);
      assert((ret == (320/2)) && (errno == 0));
      assert(tfile->getCmd () == Cmds::READ);
      assert(tfile->getPtr () == buf);
      assert(tfile->getNumber () == 320);

      // Test WRITE
      errno = -2;
      ret = file->write ((const void*) buf, 432);
      assert((ret == (432/2)) && (errno == 0));
      assert(tfile->getCmd () == Cmds::WRITE);
      assert(tfile->getPtr () == buf);
      assert(tfile->getNumber () == 432);

      // Test WRITEV
      errno = -2;
      ret = file->writev ((const struct iovec*) buf, 234);
      assert((ret == 0) && (errno == 0));
      assert(tfile->getCmd () == Cmds::WRITEV);
      assert(tfile->getPtr () == buf);
      assert(tfile->getNumber () == 234);

      // Test LSEEK
      errno = -2;
      ret = file->lseek (333, 555);
      assert((ret == 0) && (errno == 0));
      assert(tfile->getCmd () == Cmds::LSEEK);
      assert(tfile->getNumber () == 333);
      assert(tfile->getMode () == 555);

      // Test ISATTY
      errno = -2;
      ret = file->isatty ();
      assert((ret == 0) && (errno == 0));
      assert(tfile->getCmd () == Cmds::ISATTY);

      // Test FCNTL
      errno = -2;
      ret = file->fcntl (444, 987);
      assert((ret == 0) && (errno == 0));
      assert(tfile->getCmd () == Cmds::FCNTL);
      assert(tfile->getNumber () == 444);
      assert(tfile->getMode () == 987);

      // Test FSTAT
      errno = -2;
      struct stat stat_buf;
      ret = file->fstat (&stat_buf);
      assert((ret == 0) && (errno == 0));
      assert(tfile->getCmd () == Cmds::FSTAT);
      assert(tfile->getPtr () == &stat_buf);

      // Test FTRUNCATE
      errno = -2;
      ret = file->ftruncate (999);
      assert((ret == 0) && (errno == 0));
      assert(tfile->getCmd () == Cmds::FTRUNCATE);
      assert(tfile->getNumber () == 999);

      // Test FSYNC
      errno = -2;
      ret = file->fsync ();
      assert((ret == 0) && (errno == 0));
      assert(tfile->getCmd () == Cmds::FSYNC);

      // Test CLOSE
      errno = -2;
      ret = file->close ();
      assert((ret == 0) && (errno == 0));
      assert(tfile->getCmd () == Cmds::CLOSE);

      // Must no longer be in the pool
      assert(filesPool.getFlag (0) == false);
    }

  trace_puts ("'test-file-debug' succeeded.");

  // Success!
  return 0;
}

// ----------------------------------------------------------------------------

