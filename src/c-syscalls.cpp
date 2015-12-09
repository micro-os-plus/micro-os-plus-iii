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

#if defined(OS_REDEFINE_POSIX_SYSCALLS)

#define chown __posix_chown
#define close __posix_close
#define execve __posix_execve
#define fcntl __posix_fork
#define fork __posix_fork
#define fstat __posix_fstat
#define getpid __posix_getpid
#define gettimeofday __posix_gettimeofday
#define ioctl __posix_ioctl
#define isatty __posix_isatty
#define kill __posix_kill
#define link __posix_link
#define lseek __posix_lseek
#define open __posix_open
#define raise __posix_raise
#define read __posix_read
#define readlink __posix_readlink
#define stat __posix_stat
#define symlink __posix_symlink
#define times __posix_times
#define unlink __posix_unlink
#define write __posix_write

#endif

// ----------------------------------------------------------------------------

extern "C"
{

  // Redefine the standard POSIX functions.
  // Original names are prefixed to avoid clashes during tests.

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
// defines it as '*(__errno())'; if you use a multi-threaded
// environment, be sure you redefine __errno() to return a
// thread specific pointer.

// ----------------------------------------------------------------------------

int __attribute__((weak))
__posix_open (const char *path, int oflag, ...)
{
  if (path == nullptr)
    {
      errno = EBADF;
      return -1;
    }

  os::PosixIo* io = os::PosixDevice::identifyPosixDevice (path);
  if (io != nullptr)
    {
      int ret;

      va_list args;
      va_start(args, oflag);
      ret = io->open (path, oflag, args);
      va_end(args);

      return ret;
    }

  // TODO: process files from file systems.

  errno = EBADF;
  return -1;
}

ssize_t __attribute__((weak))
__posix_read (int fildes, void *buf, size_t nbyte)
{
  os::PosixIo* io = os::FileDescriptorsManager::getPosixIo (fildes);
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
  os::PosixIo* io = os::FileDescriptorsManager::getPosixIo (fildes);
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
  os::PosixIo* io = os::FileDescriptorsManager::getPosixIo (fildes);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }

  int ret;

  va_list args;
  va_start(args, request);
  ret = io->ioctl (request, args);
  va_end(args);

  return ret;
}

int __attribute__((weak))
__posix_close (int fildes)
{
  os::PosixIo* io = os::FileDescriptorsManager::getPosixIo (fildes);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->close ();
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

int __attribute__((weak))
__posix_fcntl (int fildes, int cmd, ...)
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

int __attribute__((weak))
__posix_fstat (int fildes, struct stat* buf)
{
  errno = ENOSYS; // Not implemented
  return -1;
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
__posix_isatty (int fildes)
{
  errno = ENOSYS; // Not implemented
  return 0; // Not a TTY
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

off_t __attribute__((weak))
__posix_lseek (int fildes, off_t offset, int whence)
{
  errno = ENOSYS; // Not implemented
  return ((off_t) -1);
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

