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
#include "posix-io/PosixDevice.h"
#include <cstdarg>
#include <cerrno>

// ----------------------------------------------------------------------------

#if !defined(OS_BOOL_PREFIX_POSIX_SYSCALLS)

#define __posix_chown chown
#define __posix_close close
#define __posix_execve execve
#define __posix_fork fcntl
#define __posix_fork fork
#define __posix_fstat fstat
#define __posix_ftruncate ftruncate
#define __posix_fsync fsync
#define __posix_getpid getpid
#define __posix_gettimeofday gettimeofday
#define __posix_ioctl ioctl
#define __posix_isatty isatty
#define __posix_kill kill
#define __posix_link link
#define __posix_lseek lseek
#define __posix_open open
#define __posix_raise raise
#define __posix_read read
#define __posix_readlink readlink
#define __posix_stat stat
#define __posix_symlink symlink
#define __posix_times times
#define __posix_unlink unlink
#define __posix_write write

#endif

// ----------------------------------------------------------------------------

extern "C"
{

  // The standard POSIX IO functions.
  //
  // Original names are prefixed to avoid clashes during tests,
  // but can be redefined to standard names using the above macro.

  int __attribute__((weak))
  __posix_chown (const char* path, uid_t owner, gid_t group);

  int __attribute__((weak))
  __posix_close (int fildes);

  int __attribute__((weak))
  __posix_execve (const char *path, char * const argv[], char * const envp[]);

  int __attribute__((weak))
  __posix_fcntl (int fildes, int cmd, ...);

  pid_t __attribute__((weak))
  __posix_fork (void);

  int __attribute__((weak))
  __posix_fstat (int fildes, struct stat* buf);

  int __attribute__((weak))
  __posix_ftruncate (int fildes, off_t length);

  pid_t __attribute__((weak))
  __posix_fsync (void);

  pid_t __attribute__((weak))
  __posix_getpid (void);

  int __attribute__((weak))
  __posix_gettimeofday (struct timeval* ptimeval, void* ptimezone);

  int __attribute__((weak))
  __posix_ioctl (int fildes, int request, ...);

  int __attribute__((weak))
  __posix_isatty (int fildes);

  int __attribute__((weak))
  __posix_kill (pid_t pid, int sig);

  int __attribute__((weak))
  __posix_link (const char* existing, const char* _new);

  off_t __attribute__((weak))
  __posix_lseek (int fildes, off_t offset, int whence);

  /**
   * @brief Open file relative to directory file descriptor.
   *
   * @headerfile <fcntl.h>
   *
   * @param [in] path The _path_ argument points to a pathname naming the file.
   * @param [in] oflag Values for _oflag_ are constructed by a bitwise-inclusive
   * OR of flags from the following list, defined in `<fcntl.h>`.
   *
   * @return Upon successful completion, this function shall open the file and
   * return a non-negative integer representing the lowest numbered unused file
   * descriptor. Otherwise, this function shall return -1 and set _errno_
   * to indicate the error. If -1 is returned, no files shall be created
   * or modified.
   */
  int __attribute__((weak))
  __posix_open (const char* path, int oflag, ...);

  int __attribute__((weak))
  __posix_raise (int sig);

  ssize_t __attribute__((weak))
  __posix_read (int fildes, void* buf, size_t nbyte);

  ssize_t __attribute__((weak))
  __posix_readlink (const char* path, char* buf, size_t bufsize);

  int __attribute__((weak))
  __posix_stat (const char* path, struct stat* buf);

  int __attribute__((weak))
  __posix_symlink (const char* path1, const char* path2);

  clock_t __attribute__((weak))
  __posix_times (struct tms* buf);

  int __attribute__((weak))
  __posix_unlink (const char* name);

  pid_t __attribute__((weak))
  __posix_wait (int* stat_loc);

  ssize_t __attribute__((weak))
  __posix_write (int fildes, const void* buf, size_t nbyte);

}

// ----------------------------------------------------------------------------

// Reentrancy notes and 'errno'.
//
// These functions do not handle reentrancy, so do not
// use non reentrant functions like strtok(), but replace them
// with their new versions like strtok_r().
//
// 'errno' is not a problem, since the standard headers
// define it as '*(__errno())'; if you use a multi-threaded
// environment, be sure you redefine __errno() to return a
// thread specific pointer.

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * The `open()` function shall establish the connection between a file and a
 * file descriptor. It shall create an open file description that refers
 * to a file and a file descriptor that refers to that open file
 * description. The file descriptor is used by other I/O functions to
 * refer to that file. The _path_ argument points to a pathname naming
 * the file.
 */
int __attribute__((weak))
__posix_open (const char *path, int oflag, ...)
{
  va_list args;
  va_start(args, oflag);
  os::PosixIo* io = os::PosixIo::vopen (path, oflag, args);
  va_end(args);

  if (io == nullptr)
    {
      // Return POSIX style error indicator.
      return -1;
    }

  // Return non-negative POSIX file descriptor.
  return io->getFileDescriptor ();
}

int __attribute__((weak))
__posix_close (int fildes)
{
  // The flow is identical for all POSIX functions: identify the C++
  // object and call the corresponding C++ method.
  os::PosixIo* io = os::FileDescriptorsManager::getObject (fildes);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->close ();
}

// ----------------------------------------------------------------------------

ssize_t __attribute__((weak))
__posix_read (int fildes, void *buf, size_t nbyte)
{
  os::PosixIo* io = os::FileDescriptorsManager::getObject (fildes);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->read (buf, nbyte);
}

ssize_t __attribute__((weak))
__posix_write (int fildes, const void *buf, size_t nbyte)
{
  os::PosixIo* io = os::FileDescriptorsManager::getObject (fildes);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->write (buf, nbyte);
}

int __attribute__((weak))
__posix_ioctl (int fildes, unsigned long request, ...)
{
  os::PosixIo* io = os::FileDescriptorsManager::getObject (fildes);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }

  va_list args;
  va_start(args, request);
  int ret = io->vioctl (request, args);
  va_end(args);

  return ret;
}

off_t __attribute__((weak))
__posix_lseek (int fildes, off_t offset, int whence)
{
  os::PosixIo* io = os::FileDescriptorsManager::getObject (fildes);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->lseek (offset, whence);
}

int __attribute__((weak))
__posix_isatty (int fildes)
{
  os::PosixIo* io = os::FileDescriptorsManager::getObject (fildes);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->isatty ();
}

int __attribute__((weak))
__posix_fcntl (int fildes, int cmd, ...)
{
  os::PosixIo* io = os::FileDescriptorsManager::getObject (fildes);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }

  va_list args;
  va_start(args, cmd);
  int ret = io->vfcntl (cmd, args);
  va_end(args);

  return ret;
}

int __attribute__((weak))
__posix_fstat (int fildes, struct stat* buf)
{
  os::PosixIo* io = os::FileDescriptorsManager::getObject (fildes);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->fstat (buf);
}

int __attribute__((weak))
__posix_ftruncate (int fildes, off_t length)
{
  os::PosixIo* io = os::FileDescriptorsManager::getObject (fildes);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->ftruncate (length);
}

int __attribute__((weak))
__posix_fsync (int fildes)
{
  os::PosixIo* io = os::FileDescriptorsManager::getObject (fildes);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->fsync ();
}

// ----------------------------------------------------------------------------

// These functions are defined here to avoid linker errors in freestanding
// applications. They might be called in some error cases from library
// code.
//
// If you detect other functions to be needed, just let us know
// and we'll add them.

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

int __attribute__((weak))
__posix_chown (const char* path, uid_t owner, gid_t group)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int __attribute__((weak))
__posix_execve (const char *path, char * const argv[], char * const envp[])
{
  errno = ENOSYS; // Not implemented
  return -1;
}

pid_t __attribute__((weak))
__posix_fork (void)
{
  errno = ENOSYS; // Not implemented
  return ((pid_t) -1);
}

pid_t __attribute__((weak))
__posix_getpid (void)
{
  errno = ENOSYS; // Not implemented
  return ((pid_t) -1);
}

int __attribute__((weak))
__posix_gettimeofday (struct timeval* ptimeval, void* ptimezone)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int __attribute__((weak))
__posix_kill (pid_t pid, int sig)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int __attribute__((weak))
__posix_link (const char* existing, const char* _new)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int __attribute__((weak))
__posix_raise (int sig)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

ssize_t __attribute__((weak))
__posix_readlink (const char* path, char* buf, size_t bufsize)
{
  errno = ENOSYS; // Not implemented
  return ((ssize_t) -1);
}

int __attribute__((weak))
__posix_stat (const char* path, struct stat* buf)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int __attribute__((weak))
__posix_symlink (const char* path1, const char* path2)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

clock_t __attribute__((weak))
__posix_times (struct tms* buf)
{
  errno = ENOSYS; // Not implemented
  return ((clock_t) -1);
}

int __attribute__((weak))
__posix_unlink (const char* name)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

pid_t __attribute__((weak))
__posix_wait (int* stat_loc)
{
  errno = ENOSYS; // Not implemented
  return ((pid_t) -1);
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

