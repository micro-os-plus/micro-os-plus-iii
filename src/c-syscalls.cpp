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
#include "posix-io/IO.h"
#include "posix-io/Device.h"
#include "posix-io/File.h"
#include "posix-io/FileSystem.h"
#include "posix-io/MountManager.h"
#include "posix-io/Directory.h"
#include "posix-io/Socket.h"
#include <cstdarg>
#include <cerrno>

// ----------------------------------------------------------------------------

#if !defined(OS_BOOL_PREFIX_POSIX_SYSCALLS)

// This is the common use case, when the functions are declared in the
// system headers, and the implementation is defined here, and, being
// local, overwrite the newlib definitions.

#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <signal.h>
#include <stdio.h>
#include <sys/times.h>
#include <sys/wait.h>

#if defined(__ARM_EABI__)

// The newlib definitions do not include some functions, like
// those for directory reading, utime, select.
#include "posix-io/dirent.h"
#include "posix-io/utime.h"
#include "posix-io/embed.h"
#else
// ioctl()
#include <stropts.h>
// select()
//#include <sys/select.h>
// utime()
//#include <utime.h>
#endif

#define __posix_accept accept
#define __posix_bind bind
#define __posix_chdir chdir
#define __posix_chmod chmod
#define __posix_chown chown
#define __posix_close close
#define __posix_closedir closedir
#define __posix_connect connect
#define __posix_execve execve
#define __posix_fcntl fcntl
#define __posix_fork fork
#define __posix_fstat fstat
#define __posix_ftruncate ftruncate
#define __posix_fsync fsync
#define __posix_getcwd getcwd
#define __posix_getpeername getpeername
#define __posix_getpid getpid
#define __posix_getsockname getsockname
#define __posix_getsockopt getsockopt
#define __posix_gettimeofday gettimeofday
#define __posix_ioctl ioctl
#define __posix_isatty isatty
#define __posix_kill kill
#define __posix_link link
#define __posix_listen listen
#define __posix_lseek lseek
#define __posix_mkdir mkdir
#define __posix_open open
#define __posix_opendir opendir
#define __posix_raise raise
#define __posix_read read
#define __posix_readdir readdir
//#define __posix_readdir_r readdir_r
#define __posix_readlink readlink
#define __posix_recv recv
#define __posix_recvfrom recvfrom
#define __posix_recvmsg recvmsg
#define __posix_rename rename
#define __posix_rewinddir rewinddir
#define __posix_rmdir rmdir
#define __posix_select select
#define __posix_send send
#define __posix_sendmsg sendmsg
#define __posix_sendto sendto
#define __posix_setsockopt setsockopt
#define __posix_shutdown shutdown
#define __posix_sockatmark sockatmark
#define __posix_socket socket
#define __posix_socketpair socketpair
#define __posix_stat stat
#define __posix_symlink symlink
#define __posix_sync sync
#define __posix_times times
#define __posix_truncate truncate
#define __posix_unlink unlink
#define __posix_utime utime
#define __posix_wait wait
#define __posix_write write
#define __posix_writev writev

#else

// This is the test case, when the functions are declared locally,
// with prefixed names, to avoid clashes with system functions.

#include <dirent.h>
#include <sys/socket.h>

extern "C"
{

  // The standard POSIX IO functions. Prototypes are from:
  // http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html

  // The socket definitions are from:
  // http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_socket.h.html

  int __attribute__((weak))
  __posix_accept (int socket, struct sockaddr* address, socklen_t* address_len);

  int __attribute__((weak))
  __posix_bind (int socket, const struct sockaddr* address,
                socklen_t address_len);

  int __attribute__((weak))
  __posix_chdir (const char* path);

  int __attribute__((weak))
  __posix_chmod (const char* path, mode_t mode);

  int __attribute__((weak))
  __posix_chown (const char* path, uid_t owner, gid_t group);

  int __attribute__((weak))
  __posix_close (int fildes);

  int __attribute__((weak))
  __posix_closedir (DIR* dirp);

  int __attribute__((weak))
  __posix_connect (int socket, const struct sockaddr* address,
                   socklen_t address_len);

  int __attribute__((weak))
  __posix_execve (const char* path, char* const argv[], char* const envp[]);

  int __attribute__((weak))
  __posix_fcntl (int fildes, int cmd, ...);

  pid_t __attribute__((weak))
  __posix_fork (void);

  int __attribute__((weak))
  __posix_fstat (int fildes, struct stat* buf);

  int __attribute__((weak))
  __posix_ftruncate (int fildes, off_t length);

  int __attribute__((weak))
  __posix_fsync (int fildes);

  char*
  __attribute__((weak))
  __posix_getcwd (char* buf, size_t size);

  int __attribute__((weak))
  __posix_getpeername (int socket, struct sockaddr* address,
                       socklen_t* address_len);

  pid_t __attribute__((weak))
  __posix_getpid (void);

  int __attribute__((weak))
  __posix_getsockname (int socket, struct sockaddr* address,
                       socklen_t* address_len);

  int __attribute__((weak))
  __posix_getsockopt (int socket, int level, int option_name,
                      void* option_value, socklen_t* option_len);

  int __attribute__((weak))
  __posix_gettimeofday (struct timeval* ptimeval, void* ptimezone);

  int __attribute__((weak))
  __posix_ioctl (int fildes, int request, ...);

  /**
   * @brief Test for a terminal device.
   *
   * @headerfile <unistd.h>
   *
   * @param [in] fildes Non-negative file descriptor.
   *
   * @return This function shall return 1 if _fildes_ is associated
   * with a terminal; otherwise, it shall return 0 and may set _errno_
   * to indicate the error.
   *
   */
  int __attribute__((weak))
  __posix_isatty (int fildes);

  int __attribute__((weak))
  __posix_kill (pid_t pid, int sig);

  int __attribute__((weak))
  __posix_link (const char* existing, const char* _new);

  int __attribute__((weak))
  __posix_listen (int socket, int backlog);

  off_t __attribute__((weak))
  __posix_lseek (int fildes, off_t offset, int whence);

  int __attribute__((weak))
  __posix_mkdir (const char* path, mode_t mode);

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

  DIR*
  __attribute__((weak))
  __posix_opendir (const char* dirname);

  int __attribute__((weak))
  __posix_raise (int sig);

  ssize_t __attribute__((weak))
  __posix_read (int fildes, void* buf, size_t nbyte);

  struct dirent*
  __attribute__((weak))
  __posix_readdir (DIR* dirp);

#if 0
  int __attribute__((weak))
  __posix_readdir_r (DIR* dirp, struct dirent* entry, struct dirent** result);
#endif

  ssize_t __attribute__((weak))
  __posix_readlink (const char* path, char* buf, size_t bufsize);

  ssize_t __attribute__((weak))
  __posix_recv (int socket, void* buffer, size_t length, int flags);

  ssize_t __attribute__((weak))
  __posix_recvfrom (int socket, void* buffer, size_t length, int flags,
                    struct sockaddr* address, socklen_t* address_len);

  ssize_t __attribute__((weak))
  __posix_recvmsg (int socket, struct msghdr* message, int flags);

  int __attribute__((weak))
  __posix_rename (const char* oldfn, const char* newfn);

  void __attribute__((weak))
  __posix_rewinddir (DIR* dirp);

  int __attribute__((weak))
  __posix_rmdir (const char* path);

  int __attribute__((weak))
  __posix_select (int nfds, fd_set* readfds, fd_set* writefds, fd_set* errorfds,
                  struct timeval* timeout);

  ssize_t __attribute__((weak))
  __posix_send (int socket, const void* buffer, size_t length, int flags);

  ssize_t __attribute__((weak))
  __posix_sendmsg (int socket, const struct msghdr* message, int flags);

  ssize_t __attribute__((weak))
  __posix_sendto (int socket, const void* message, size_t length, int flags,
                  const struct sockaddr* dest_addr, socklen_t dest_len);

  int __attribute__((weak))
  __posix_setsockopt (int socket, int level, int option_name,
                      const void* option_value, socklen_t option_len);

  int __attribute__((weak))
  __posix_shutdown (int socket, int how);

  int __attribute__((weak))
  __posix_sockatmark (int socket);

  int __attribute__((weak))
  __posix_socket (int domain, int type, int protocol);

  int __attribute__((weak))
  __posix_socketpair (int domain, int type, int protocol, int socket_vector[2]);

  int __attribute__((weak))
  __posix_stat (const char* path, struct stat* buf);

  void __attribute__((weak))
  __posix_sync (void);

  int __attribute__((weak))
  __posix_symlink (const char* existing, const char* _new);

  clock_t __attribute__((weak))
  __posix_times (struct tms* buf);

  int __attribute__((weak))
  __posix_truncate (const char* path, off_t length);

  int __attribute__((weak))
  __posix_unlink (const char* name);

  int __attribute__((weak))
  __posix_utime (const char* path, const struct utimbuf* times);

  pid_t __attribute__((weak))
  __posix_wait (int* stat_loc);

  ssize_t __attribute__((weak))
  __posix_write (int fildes, const void* buf, size_t nbyte);

  ssize_t __attribute__((weak))
  __posix_writev (int fildes, const struct iovec* iov, int iovcnt);
}

#endif

// ----------------------------------------------------------------------------

// Notes: Reentrancy and 'errno'.
//
// The standard headers define errno as '*(__errno())';
// If you use a multi-threaded environment, be sure you
// redefine __errno() to return a thread specific pointer.

// ----------------------------------------------------------------------------

// ---- POSIX IO functions ----------------------------------------------------

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
int
__posix_open (const char* path, int oflag, ...)
{
  va_list args;
  va_start(args, oflag);
  auto* const io = os::posix::vopen (path, oflag, args);
  va_end(args);

  if (io == nullptr)
    {
      // Return POSIX style error indicator.
      return -1;
    }

  // Return non-negative POSIX file descriptor.
  return io->getFileDescriptor ();
}

int
__posix_close (int fildes)
{
  // The flow is identical for all POSIX functions: identify the C++
  // object and call the corresponding C++ method.
  auto* const io = os::posix::FileDescriptorsManager::getIo (fildes);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->close ();
}

// ----------------------------------------------------------------------------

ssize_t
__posix_read (int fildes, void* buf, size_t nbyte)
{
  auto* const io = os::posix::FileDescriptorsManager::getIo (fildes);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->read (buf, nbyte);
}

ssize_t
__posix_write (int fildes, const void* buf, size_t nbyte)
{
  auto* const io = os::posix::FileDescriptorsManager::getIo (fildes);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->write (buf, nbyte);
}

ssize_t
__posix_writev (int fildes, const struct iovec* iov, int iovcnt)
{
  auto* const io = os::posix::FileDescriptorsManager::getIo (fildes);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->writev (iov, iovcnt);
}

int
__posix_ioctl (int fildes, int request, ...)
{
  auto* const io = os::posix::FileDescriptorsManager::getIo (fildes);
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

off_t
__posix_lseek (int fildes, off_t offset, int whence)
{
  auto* const io = os::posix::FileDescriptorsManager::getIo (fildes);
  if (io == nullptr)
    {
      errno = EBADF; // Fildes is not an open file descriptor.
      return -1;
    }

  // Works only on files (Does not work on sockets, pipes or FIFOs...)
  if ((io->getType () & os::posix::IO::Type::FILE) == 0)
    {
      errno = ESPIPE; // Not a file.
      return -1;
    }

  return static_cast<os::posix::File*> (io)->lseek (offset, whence);
}

/**
 * @details
 *
 * This function shall test whether _fildes_, an open file descriptor,
 * is associated with a terminal device.
 */
int
__posix_isatty (int fildes)
{
  auto* const io = os::posix::FileDescriptorsManager::getIo (fildes);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->isatty ();
}

int
__posix_fcntl (int fildes, int cmd, ...)
{
  auto* const io = os::posix::FileDescriptorsManager::getIo (fildes);
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

int
__posix_fstat (int fildes, struct stat* buf)
{
  auto* const io = os::posix::FileDescriptorsManager::getIo (fildes);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->fstat (buf);
}

int
__posix_ftruncate (int fildes, off_t length)
{
  auto* const io = os::posix::FileDescriptorsManager::getIo (fildes);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }

  // Works only on files (Does not work on sockets, pipes or FIFOs...)
  if ((io->getType () & os::posix::IO::Type::FILE) == 0)
    {
      errno = EINVAL; // Not a file.
      return -1;
    }

  return static_cast<os::posix::File*> (io)->ftruncate (length);
}

int
__posix_fsync (int fildes)
{
  auto* const io = os::posix::FileDescriptorsManager::getIo (fildes);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }

  // Works only on files (Does not work on sockets, pipes or FIFOs...)
  if ((io->getType () & os::posix::IO::Type::FILE) == 0)
    {
      errno = EINVAL; // Not a file.
      return -1;
    }

  return static_cast<os::posix::File*> (io)->fsync ();
}

// ----------------------------------------------------------------------------
// ----- POSIX File functions -----

int
__posix_chmod (const char* path, mode_t mode)
{
  return os::posix::chmod (path, mode);
}

int
__posix_stat (const char* path, struct stat* buf)
{
  return os::posix::stat (path, buf);
}

int
__posix_truncate (const char* path, off_t length)
{
  return os::posix::truncate (path, length);
}

int
__posix_rename (const char* existing, const char* _new)
{
  return os::posix::rename (existing, _new);
}

int
__posix_unlink (const char* path)
{
  return os::posix::unlink (path);
}

int
__posix_utime (const char* path, const struct utimbuf* times)
{
  return os::posix::utime (path, times);
}

// ----------------------------------------------------------------------------
// ----- POSIX FileSystem functions -----

int
__posix_mkdir (const char* path, mode_t mode)
{
  return os::posix::mkdir (path, mode);
}

int
__posix_rmdir (const char* path)
{
  return os::posix::rmdir (path);
}

void
__posix_sync (void)
{
  return os::posix::sync ();
}

// ----------------------------------------------------------------------------
// ----- Directories functions -----

DIR*
__posix_opendir (const char* dirpath)
{
  return reinterpret_cast<DIR*> (os::posix::opendir (dirpath));
}

struct dirent*
__posix_readdir (DIR* dirp)
{
  auto* const dir = reinterpret_cast<os::posix::Directory*> (dirp);
  if (dir == nullptr)
    {
      errno = ENOENT;
      return nullptr;
    }
  return dir->read ();
}

#if 0
int
__posix_readdir_r (DIR* dirp, struct dirent* entry, struct dirent** result)
  {
    errno = ENOSYS; // Not implemented
    return ((ssize_t) -1);
  }
#endif

void
__posix_rewinddir (DIR* dirp)
{
  auto* const dir = reinterpret_cast<os::posix::Directory*> (dirp);
  if (dir == nullptr)
    {
      errno = ENOENT;
      return;
    }
  return dir->rewind ();
}

int
__posix_closedir (DIR* dirp)
{
  auto* const dir = reinterpret_cast<os::posix::Directory*> (dirp);
  if (dir == nullptr)
    {
      errno = ENOENT;
      return -1;
    }
  return dir->close ();
}

// ----------------------------------------------------------------------------
// Socket functions

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// socket() and socketpair() are the fuctions creating sockets.
// The other are socket specific functions.

// In addition, the following IO functions should work on sockets:
// close(), read(), write(), writev(), ioctl(), fcntl(), select().

int
__posix_socket (int domain, int type, int protocol)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_socketpair (int domain, int type, int protocol, int socket_vector[2])
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_accept (int socket, struct sockaddr* address, socklen_t* address_len)
{
  auto* const io = os::posix::FileDescriptorsManager::getSocket (socket);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  auto* const new_socket = io->accept (address, address_len);
  return new_socket->getFileDescriptor ();
}

int
__posix_bind (int socket, const struct sockaddr* address, socklen_t address_len)
{
  auto* const io = os::posix::FileDescriptorsManager::getSocket (socket);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->bind (address, address_len);
}

int
__posix_connect (int socket, const struct sockaddr* address,
                 socklen_t address_len)
{
  auto* const io = os::posix::FileDescriptorsManager::getSocket (socket);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->connect (address, address_len);
}

int
__posix_getpeername (int socket, struct sockaddr* address,
                     socklen_t* address_len)
{
  auto* const io = os::posix::FileDescriptorsManager::getSocket (socket);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->getpeername (address, address_len);
}

int
__posix_getsockname (int socket, struct sockaddr* address,
                     socklen_t* address_len)
{
  auto* const io = os::posix::FileDescriptorsManager::getSocket (socket);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->getsockname (address, address_len);
}

int
__posix_getsockopt (int socket, int level, int option_name, void* option_value,
                    socklen_t* option_len)
{
  auto* const io = os::posix::FileDescriptorsManager::getSocket (socket);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->getsockopt (level, option_name, option_value, option_len);
}

int
__posix_listen (int socket, int backlog)
{
  auto* const io = os::posix::FileDescriptorsManager::getSocket (socket);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->listen (backlog);
}

ssize_t
__posix_recv (int socket, void* buffer, size_t length, int flags)
{
  auto* const io = os::posix::FileDescriptorsManager::getSocket (socket);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->recv (buffer, length, flags);
}

ssize_t
__posix_recvfrom (int socket, void* buffer, size_t length, int flags,
                  struct sockaddr* address, socklen_t* address_len)
{
  auto* const io = os::posix::FileDescriptorsManager::getSocket (socket);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->recvfrom (buffer, length, flags, address, address_len);
}

ssize_t
__posix_recvmsg (int socket, struct msghdr* message, int flags)
{
  auto* const io = os::posix::FileDescriptorsManager::getSocket (socket);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->recvmsg (message, flags);
}

ssize_t
__posix_send (int socket, const void* buffer, size_t length, int flags)
{
  auto* const io = os::posix::FileDescriptorsManager::getSocket (socket);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->send (buffer, length, flags);
}

ssize_t
__posix_sendmsg (int socket, const struct msghdr* message, int flags)
{
  auto* const io = os::posix::FileDescriptorsManager::getSocket (socket);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->sendmsg (message, flags);
}

ssize_t
__posix_sendto (int socket, const void* message, size_t length, int flags,
                const struct sockaddr* dest_addr, socklen_t dest_len)
{
  auto* const io = os::posix::FileDescriptorsManager::getSocket (socket);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->sendto (message, length, flags, dest_addr, dest_len);
}

int
__posix_setsockopt (int socket, int level, int option_name,
                    const void* option_value, socklen_t option_len)
{
  auto* const io = os::posix::FileDescriptorsManager::getSocket (socket);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->setsockopt (level, option_name, option_value, option_len);
}

int
__posix_shutdown (int socket, int how)
{
  auto* const io = os::posix::FileDescriptorsManager::getSocket (socket);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->shutdown (how);
}

int
__posix_sockatmark (int socket)
{
  auto* const io = os::posix::FileDescriptorsManager::getSocket (socket);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return io->sockatmark ();
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

// These functions are defined here to avoid linker errors in free
// standing applications. They might be called in some error cases
// from library code.
//
// If you detect other functions to be needed, just let us know
// and we'll add them.

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

int
__posix_execve (const char* path, char* const argv[], char* const envp[])
{
  errno = ENOSYS; // Not implemented
  return -1;
}

pid_t
__posix_fork (void)
{
  errno = ENOSYS; // Not implemented
  return ((pid_t) -1);
}

pid_t
__posix_getpid (void)
{
  errno = ENOSYS; // Not implemented
  return ((pid_t) -1);
}

int
__posix_gettimeofday (struct timeval* ptimeval, void* ptimezone)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_kill (pid_t pid, int sig)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_raise (int sig)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_select (int nfds, fd_set* readfds, fd_set* writefds, fd_set* errorfds,
                struct timeval* timeout)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

clock_t
__posix_times (struct tms* buf)
{
  errno = ENOSYS; // Not implemented
  return ((clock_t) -1);
}

pid_t
__posix_wait (int* stat_loc)
{
  errno = ENOSYS; // Not implemented
  return ((pid_t) -1);
}

// File system related - not implemented

int
__posix_chdir (const char* path)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

char*
__posix_getcwd (char* buf, size_t size)
{
  errno = ENOSYS; // Not implemented
  return nullptr;
}

int
__posix_chown (const char* path, uid_t owner, gid_t group)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_link (const char* existing, const char* _new)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_symlink (const char* existing, const char* _new)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

ssize_t
__posix_readlink (const char* path, char* buf, size_t bufsize)
{
  errno = ENOSYS; // Not implemented
  return ((ssize_t) -1);
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

