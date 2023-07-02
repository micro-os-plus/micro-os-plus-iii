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

#include "posix-io/file_descriptors_manager.h"
#include "posix-io/io.h"
#include "posix-io/directory.h"
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

#if defined(__ARM_EABI__)
#include "posix-io/redefinitions.h"
#endif

// ----------------------------------------------------------------------------

enum class Cmds
  : unsigned int
    { UNKNOWN, NOTSET, OPEN, CLOSE, READ, REWIND
};

class TestFile : public os::posix::file
{
public:

  TestFile ();

  // Methods used for test purposes only.
  Cmds
  getCmd (void);

  int
  getMode (void);

  const char*
  path (void);

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

private:

  const char* fPath;
  int fMode;
  unsigned int fNumber;
  Cmds fCmd;

};

TestFile::TestFile ()
{
  fCmd = Cmds::NOTSET;
  fPath = nullptr;
  fMode = -1;
  fNumber = 1;
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
TestFile::path (void)
{
  return fPath;
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

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wpadded"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpadded"
#endif

// Test class, all methods store the input in local variables,
// to be checked later.

class TestDir : public os::posix::directory
{
public:

  TestDir ();

  // Methods used for test purposes only.
  Cmds
  getCmd (void);

  const char*
  path (void);

protected:

  // Implementations.

  virtual directory*
  do_vopen (const char* dirname) override;

  virtual struct dirent*
  do_read (void) override;

  virtual void
  do_rewind (void) override;

  virtual int
  do_close (void) override;

private:

  const char* fPath;
  Cmds fCmd;
  unsigned int fCount;

};

#pragma GCC diagnostic pop

TestDir::TestDir ()
{
  fCmd = Cmds::NOTSET;
  fPath = nullptr;
  fCount = 0;
}

inline Cmds
TestDir::getCmd (void)
{
  return fCmd;
}

inline const char*
TestDir::path (void)
{
  return fPath;
}

os::posix::directory*
TestDir::do_vopen (const char* dirname)
{
  fPath = dirname;
  fCount = 1;

  return this;
}

int
TestDir::do_close (void)
{
  fCmd = Cmds::CLOSE;
  return 0; // Always return success
}

struct dirent*
TestDir::do_read ()
{
  fCmd = Cmds::READ;
  sprintf (dir_entry ()->d_name, "file%d", fCount);

  fCount++;
  return dir_entry ();
}

void
TestDir::do_rewind (void)
{
  fCmd = Cmds::REWIND;
  fCount = 1;

  return;
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

protected:

  // Implementations.

  virtual int
  do_mount (unsigned int flags) override;

  virtual int
  do_unmount (unsigned int flags) override;

  virtual void
  do_sync (void) override;

private:

  unsigned int fMountFlags;
  Cmds fCmd;
  unsigned int fSyncCount;
};

TestFileSystem::TestFileSystem (os::posix::pool* filesPool,
                                os::posix::pool* dirsPool) :
    os::posix::file_system (filesPool, dirsPool)
{
  fMountFlags = 1;
  fCmd = Cmds::NOTSET;
  fSyncCount = 1;
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

// ----------------------------------------------------------------------------

// Required only as a reference, no functionality needed.
class TestBlockDevice : public os::posix::device_block
{
public:
  TestBlockDevice () = default;
};

// ----------------------------------------------------------------------------

using TestFilePool = os::posix::pool_typed<TestFile>;

constexpr std::size_t FILES_POOL_ARRAY_SIZE = 2;

// pool of file objects, used in common by all filesystems.
TestFilePool filesPool
  { FILES_POOL_ARRAY_SIZE };

using TestDirPool = os::posix::pool_typed<TestDir>;

constexpr std::size_t DIRS_POOL_ARRAY_SIZE = 2;

// pool of Dir objects, used in common by all filesystems.
TestDirPool dirsPool
  { DIRS_POOL_ARRAY_SIZE };

// file systems, all using the same pool.
TestFileSystem root (&filesPool, &dirsPool);
TestFileSystem babu (&filesPool, &dirsPool);

// Static manager
os::posix::file_descriptors_manager dm
  { 5 };

// Static manager
os::posix::mount_manager fsm
  { 2 };

// Block devices, just referenced, no calls forwarded to them.
TestBlockDevice rootDevice;
TestBlockDevice babuDevice;
TestBlockDevice babuDevice2;

// ----------------------------------------------------------------------------

int
main (int argc __attribute__((unused)), char* argv[] __attribute__((unused)))
{
    {
      // Mount
      errno = -2;
      assert(
          (os::posix::mount_manager::mount (&babu, "/babu/", &babuDevice, 124) == 0) && (errno == 0));
    }

    {
      // Test OPEN
      errno = -2;
      DIR* pdir = __posix_opendir ("/babu/d1");
      assert((pdir != nullptr) && (errno == 0));

      TestDir* dir = reinterpret_cast<TestDir*> (pdir);
      // Must be the first used slot in the pool.
      assert(dirsPool.object (0) == dir);
      assert(dirsPool.in_use (0) == true);

      // Check params passing.
      assert(std::strcmp ("/d1", dir->path ()) == 0);

      // Test READ
      errno = -2;
      struct dirent* de = __posix_readdir (pdir);
      assert((de != nullptr) && (errno == 0));
      assert(dir->getCmd () == Cmds::READ);

      // Test REWIND
      errno = -2;
      __posix_rewinddir (pdir);
      assert((errno == 0));
      assert(dir->getCmd () == Cmds::REWIND);

      // Test CLOSE
      errno = -2;
      int ret = __posix_closedir (pdir);
      assert((ret == 0) && (errno == 0));
      assert(dir->getCmd () == Cmds::CLOSE);

      // Must no longer be in the pool
      assert(dirsPool.in_use (0) == false);
    }

    {
      // C API

      // Test OPEN
      errno = -2;
      DIR* pdir = __posix_opendir ("/babu/d1");
      assert((pdir != nullptr) && (errno == 0));

      TestDir* dir = reinterpret_cast<TestDir*> (pdir);
      // Must be the first used slot in the pool.
      assert(dirsPool.object (0) == dir);
      assert(dirsPool.in_use (0) == true);

      // Check params passing.
      assert(std::strcmp ("/d1", dir->path ()) == 0);

      // Test READ
      errno = -2;
      struct dirent* de = __posix_readdir (pdir);
      assert((de != nullptr) && (errno == 0));
      assert(dir->getCmd () == Cmds::READ);

      // Test REWIND
      errno = -2;
      __posix_rewinddir (pdir);
      assert((errno == 0));
      assert(dir->getCmd () == Cmds::REWIND);

      // Test CLOSE
      errno = -2;
      int ret = __posix_closedir (pdir);
      assert((ret == 0) && (errno == 0));
      assert(dir->getCmd () == Cmds::CLOSE);

      // Must no longer be in the pool
      assert(dirsPool.in_use (0) == false);
    }

    {
      // C++ API

      // Test OPEN
      errno = -2;
      os::posix::directory* dir = os::posix::opendir ("/babu/d2");
      assert((dir != nullptr) && (errno == 0));

      // Must be the first used slot in the pool.
      assert(dirsPool.object (0) == dir);
      assert(dirsPool.in_use (0) == true);

      TestDir* tdir = static_cast<TestDir*> (dir);

      // Check params passing.
      assert(std::strcmp ("/d2", tdir->path ()) == 0);

      // Test READ
      errno = -2;
      struct dirent* de = dir->read ();
      assert((de != nullptr) && (errno == 0));
      assert(tdir->getCmd () == Cmds::READ);
      assert(std::strcmp (dir->name (), "file1") == 0);

      // Second READ
      de = dir->read ();
      assert((de != nullptr) && (errno == 0));
      assert(tdir->getCmd () == Cmds::READ);
      assert(std::strcmp (dir->name (), "file2") == 0);

      // Test REWIND
      errno = -2;
      dir->rewind ();
      assert((errno == 0));
      assert(tdir->getCmd () == Cmds::REWIND);

      // Test READ after REWIND
      de = dir->read ();
      assert((de != nullptr) && (errno == 0));
      assert(tdir->getCmd () == Cmds::READ);
      assert(std::strcmp (dir->name (), "file1") == 0);

      // Test CLOSE
      errno = -2;
      int ret = dir->close ();
      assert((ret == 0) && (errno == 0));
      assert(tdir->getCmd () == Cmds::CLOSE);

      // Must no longer be in the pool
      assert(dirsPool.in_use (0) == false);
    }

  trace_puts ("'test-directory-debug' succeeded.");

  // Success!
  return 0;
}

// ----------------------------------------------------------------------------

