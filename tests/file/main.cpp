/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
 *
 * µOS++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 3.
 *
 * µOS++ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "posix-io/FileDescriptorsManager.h"
#include "posix-io/PosixIo.h"
#include "posix-io/PosixFile.h"
#include "posix-io/PosixFileSystem.h"
#include "posix-io/TPosixPool.h"
#include "posix-io/PosixFileSystemsManager.h"
#include "posix-io/BlockDevice.h"
#include <cerrno>
#include <cassert>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#if defined(OS_INCLUDE_TRACE_PRINTF)
#include "diag/Trace.h"
#endif
#include "utime.h"

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

class TestFile : public os::PosixFile
{
public:

  TestFile ();

  // Methods used for test purposes only.
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
  do_open (const char* path, int oflag, std::va_list args) override;

  virtual int
  do_close (void) override;

  virtual ssize_t
  do_read (void* buf, std::size_t nbyte) override;

  virtual ssize_t
  do_write (const void* buf, std::size_t nbyte) override;

  virtual int
  do_ioctl (int request, std::va_list args) override;

  virtual off_t
  do_lseek (off_t offset, int whence) override;

  virtual int
  do_isatty (void) override;

  virtual int
  do_fcntl (int cmd, va_list args) override;

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
TestFile::do_open (const char* path, int oflag, std::va_list args)
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
  return 0;
}

ssize_t
TestFile::do_write (const void* buf, std::size_t nbyte)
{
  fCmd = Cmds::WRITE;
  fPtr = (void*) buf;
  fNumber = nbyte;
  return 0;
}

int
TestFile::do_ioctl (int request, std::va_list args)
{
  fCmd = Cmds::IOCTL;
  fNumber = request;
  fMode = va_arg(args, int);
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
TestFile::do_fcntl (int cmd, std::va_list args)
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

class TestFileSystem : public os::PosixFileSystem
{
public:

  TestFileSystem (os::PosixPool* filesPool, os::PosixPool* dirsPool);

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

TestFileSystem::TestFileSystem (os::PosixPool* filesPool,
                                os::PosixPool* dirsPool) :
    os::PosixFileSystem (filesPool, dirsPool)
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
class TestBlockDevice : public os::BlockDevice
{
public:
  TestBlockDevice () = default;
};

// ----------------------------------------------------------------------------

using TestFilePool = os::TPosixPool<TestFile>;

constexpr std::size_t FILES_POOL_ARRAY_SIZE = 2;

// Pool of File objects, used in common by all filesystems.
TestFilePool filesPool
  { FILES_POOL_ARRAY_SIZE };

// File systems, all using the same pool.
TestFileSystem root (&filesPool, nullptr);
TestFileSystem babu (&filesPool, nullptr);
TestFileSystem babu2 (&filesPool, nullptr);

// Static manager
os::FileDescriptorsManager dm
  { 5 };

// Static manager
os::PosixFileSystemsManager fsm
  { 2 };

// Block devices, just referenced, no calls forwarded to them.
TestBlockDevice rootDevice;
TestBlockDevice babuDevice;
TestBlockDevice babuDevice2;

// ----------------------------------------------------------------------------

extern "C"
{
  int
  __posix_chmod (const char* path, mode_t mode);

  int
  __posix_stat (const char* path, struct stat* buf);

  int
  __posix_truncate (const char* path, off_t length);

  int
  __posix_rename (const char* existing, const char* _new);

  int
  __posix_unlink (const char* path);

  int
  __posix_utime (const char* path, const struct utimbuf* times);

  // -----

  int
  __posix_mkdir (const char* path, mode_t mode);

  int
  __posix_rmdir (const char* path);

  void
  __posix_sync (void);

  // -----

  int
  __posix_open (const char* path, int oflag, ...);

  int
  __posix_close (int fildes);

  ssize_t
  __posix_read (int fildes, void* buf, size_t nbyte);

  ssize_t
  __posix_write (int fildes, const void* buf, size_t nbyte);

  int
  __posix_ioctl (int fildes, int request, ...);

  off_t
  __posix_lseek (int fildes, off_t offset, int whence);

  int
  __posix_isatty (int fildes);

  int
  __posix_fcntl (int fildes, int cmd, ...);

  int
  __posix_fstat (int fildes, struct stat* buf);

  int
  __posix_ftruncate (int fildes, off_t length);

  int
  __posix_fsync (int fildes);

}

// ----------------------------------------------------------------------------

int
main (int argc __attribute__((unused)), char* argv[] __attribute__((unused)))
{
    {
      // ----- FileSystemsManager -----

      // Check initial size.
      assert(fsm.getSize () == 2);

      // Check if FileSystemsManger is empty.
      for (std::size_t i = 0; i < fsm.getSize (); ++i)
        {
          assert(fsm.getFileSystem (i) == nullptr);
          assert(fsm.getPath (i) == nullptr);
        }
      assert(fsm.getRoot () == nullptr);

      const char* path1 = "/babu/riba";
      const char* path2 = path1;

      // No file system, identify nothing
      assert(fsm.identifyFileSystem (&path2, nullptr) == nullptr);

      // Check if root file system flags are those set by constructor.
      assert(root.getFlags () == 1);

      // Check setRoot(), and mount().
      assert(fsm.setRoot (&root, &rootDevice, 123) == 0);
      assert(fsm.getRoot () == &root);
      assert(root.getBlockDevice () == &rootDevice);

      // Check mount flags.
      assert(root.getFlags () == 123);

      // No file systems mounted, identify root.
      assert(
          os::PosixFileSystemsManager::identifyFileSystem (&path2, nullptr)
              == &root);
      assert(path2 == path1);
    }

    {
      // ----- FileSystemsManager mounts & umounts -----

      errno = -2;
      assert(
          (os::PosixFileSystemsManager::mount (&babu, "/babu/", &babuDevice, 124) == 0) && (errno == 0));
      assert(os::PosixFileSystemsManager::getFileSystem (0) == &babu);
      assert(babu.getBlockDevice () == &babuDevice);

      assert(babu.getFlags () == 124);

      // Check not mounted file, should return root
      const char* path1 = "/baburiba";
      const char* path2 = path1;

      assert(
          os::PosixFileSystemsManager::identifyFileSystem (&path2, nullptr)
              == &root);
      assert(path2 == path1);

      // Check busy error
      errno = -2;
      assert(
          os::PosixFileSystemsManager::mount (&babu, "/babu/", &babuDevice, 124)
              == -1);
      assert(errno == EBUSY);

      path1 = "/babu/riba";
      path2 = path1;

      const char* path3 = "/babu/biba";
      const char* path4 = path3;

      // Check if identified properly
      assert(
          os::PosixFileSystemsManager::identifyFileSystem (&path2, &path4)
              == &babu);

      // Check if path adjusted properly
      assert(path2 == (path1 + std::strlen ("/babu")));
      assert(path4 == (path3 + std::strlen ("/babu")));

      // Check size exceeded
      errno = -2;
      assert(
          (os::PosixFileSystemsManager::mount (&babu2, "/babu2/", &babuDevice2, 124) == 0) && (errno == 0));
      errno = -2;
      assert(
          os::PosixFileSystemsManager::mount (&babu2, "/babu3/", &babuDevice2,
                                              124) == -1);
      assert(errno == ENOENT);

      // Check umounts
      unsigned int cnt = babu.getSyncCount ();
      errno = -2;
      assert(
          (os::PosixFileSystemsManager::umount ("/babu/", 134) == 0) && (errno == 0));
      assert(babu.getFlags () == 134);
      assert(babu.getSyncCount () == cnt + 1);
      assert(babu.getBlockDevice () == nullptr);

      // Check umounts
      cnt = babu2.getSyncCount ();
      errno = -2;
      assert(
          (os::PosixFileSystemsManager::umount ("/babu2/", 144) == 0) && (errno == 0));
      assert(babu2.getFlags () == 144);
      assert(babu2.getSyncCount () == cnt + 1);
      assert(babu2.getBlockDevice () == nullptr);
    }

    {
      // Mount again
      errno = -2;
      assert(
          (os::PosixFileSystemsManager::mount (&babu, "/babu/", &babuDevice, 124) == 0) && (errno == 0));

      // CHMOD
      errno = -2;
      assert((__posix_chmod ("/babu/p1", 321) == 0) && (errno == 0));
      assert(babu.getCmd () == Cmds::CHMOD);
      assert(babu.getNumber () == 321);
      assert(std::strcmp ("/p1", babu.getPath ()) == 0);

      // STAT
      errno = -2;
      struct stat stat_buf;
      assert((__posix_stat ("/babu/p2", &stat_buf) == 0) && (errno == 0));
      assert(babu.getCmd () == Cmds::STAT);
      assert(babu.getPtr () == &stat_buf);
      assert(std::strcmp ("/p2", babu.getPath ()) == 0);

      // TRUNCATE
      errno = -2;
      assert((__posix_truncate ("/babu/p3", 876) == 0) && (errno == 0));
      assert(babu.getCmd () == Cmds::TRUNCATE);
      assert(babu.getNumber () == 876);
      assert(std::strcmp ("/p3", babu.getPath ()) == 0);

      // RENAME
      errno = -2;
      assert(
          (__posix_rename ("/babu/p4", "/babu/p4-new") == 0) && (errno == 0));
      assert(babu.getCmd () == Cmds::RENAME);
      assert(std::strcmp ("/p4", babu.getPath ()) == 0);
      assert(std::strcmp ("/p4-new", (const char* )babu.getPtr ()) == 0);

      // UNLINK
      errno = -2;
      assert((__posix_unlink ("/babu/p5") ==0) && (errno == 0));
      assert(babu.getCmd () == Cmds::UNLINK);
      assert(std::strcmp ("/p5", babu.getPath ()) == 0);

      // UTIME
      errno = -2;
      struct utimbuf times;
      assert((__posix_utime ("/babu/p6", &times) ==0) && (errno == 0));
      assert(babu.getCmd () == Cmds::UTIME);
      assert(babu.getPtr () == &times);
      assert(std::strcmp ("/p6", babu.getPath ()) == 0);

      // MKDIR
      errno = -2;
      assert((__posix_mkdir ("/babu/p7", 654) ==0) && (errno == 0));
      assert(babu.getCmd () == Cmds::MKDIR);
      assert(babu.getNumber () == 654);
      assert(std::strcmp ("/p7", babu.getPath ()) == 0);

      // RMDIR
      errno = -2;
      assert((__posix_rmdir ("/babu/p8") ==0) && (errno == 0));
      assert(babu.getCmd () == Cmds::RMDIR);
      assert(std::strcmp ("/p8", babu.getPath ()) == 0);

      // SYNC
      unsigned int cnt = babu.getSyncCount ();
      errno = -2;
      __posix_sync ();
      assert(errno == 0);
      assert(babu.getCmd () == Cmds::RMDIR);
      assert(babu.getSyncCount () == cnt + 1);

    }

    {
      // Test OPEN
      errno = -2;
      int fd = __posix_open ("/babu/f1", 123, 234);
      assert((fd >= 0) && (errno == 0));

      os::PosixIo* io = os::FileDescriptorsManager::getIo (fd);
      assert(io != nullptr);

      assert(io->getType () == os::PosixIo::Type::FILE);

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
      char buf[3];
      ret = __posix_read (fd, (void*) buf, 321);
      assert((ret == 0) && (errno == 0));
      assert(file->getCmd () == Cmds::READ);
      assert(file->getPtr () == buf);
      assert(file->getNumber () == 321);

      // Test WRITE
      errno = -2;
      ret = __posix_write (fd, (const void*) buf, 432);
      assert((ret == 0) && (errno == 0));
      assert(file->getCmd () == Cmds::WRITE);
      assert(file->getPtr () == buf);
      assert(file->getNumber () == 432);

      // Test IOCTL
      errno = -2;
      ret = __posix_ioctl (fd, 222, 876);
      assert((ret == 0) && (errno == 0));
      assert(file->getCmd () == Cmds::IOCTL);
      assert(file->getNumber () == 222);
      assert(file->getMode () == 876);

      // Test LSEEK
      errno = -2;
      ret = __posix_lseek (fd, 333, 555);
      assert((ret == 0) && (errno == 0));
      assert(file->getCmd () == Cmds::LSEEK);
      assert(file->getNumber () == 333);
      assert(file->getMode () == 555);

      // Test ISATTY
      errno = -2;
      ret = __posix_isatty (fd);
      assert((ret == 0) && (errno == 0));
      assert(file->getCmd () == Cmds::ISATTY);

      // Test FCNTL
      errno = -2;
      ret = __posix_fcntl (fd, 444, 987);
      assert((ret == 0) && (errno == 0));
      assert(file->getCmd () == Cmds::FCNTL);
      assert(file->getNumber () == 444);
      assert(file->getMode () == 987);

      // Test FSTAT
      errno = -2;
      struct stat stat_buf;
      ret = __posix_fstat (fd, &stat_buf);
      assert((ret == 0) && (errno == 0));
      assert(file->getCmd () == Cmds::FSTAT);
      assert(file->getPtr () == &stat_buf);

      // Test FTRUNCATE
      errno = -2;
      ret = __posix_ftruncate (fd, 999);
      assert((ret == 0) && (errno == 0));
      assert(file->getCmd () == Cmds::FTRUNCATE);
      assert(file->getNumber () == 999);

      // Test FSYNC
      errno = -2;
      ret = __posix_fsync (fd);
      assert((ret == 0) && (errno == 0));
      assert(file->getCmd () == Cmds::FSYNC);

      // Test CLOSE
      errno = -2;
      ret = __posix_close (fd);
      assert((ret == 0) && (errno == 0));
      assert(file->getCmd () == Cmds::CLOSE);

      // Must no longer be in the pool
      assert(filesPool.getFlag (0) == false);
    }

  const char* msg = "'test-file-debug' succeeded.\n";
#if defined(OS_INCLUDE_TRACE_PRINTF)
  trace_puts (msg);
#else
  std::puts (msg);
#endif

  // Success!
  return 0;
}

// ----------------------------------------------------------------------------

