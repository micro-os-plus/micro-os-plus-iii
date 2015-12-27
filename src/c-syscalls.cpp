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

#include "posix-io/types.h"
#include "posix-io/FileDescriptorsManager.h"
#include "posix-io/IO.h"
#include "posix-io/CharDevice.h"
#include "posix-io/File.h"
#include "posix-io/FileSystem.h"
#include "posix-io/MountManager.h"
#include "posix-io/Directory.h"
#include "posix-io/Socket.h"

#include "posix/sys/uio.h"

#include <cstdarg>
#include <cerrno>

// ----------------------------------------------------------------------------

// Notes: Function prefix.
//
// To facilitate testing on POSIX platforms, and also to allow
// integration on custom platforms, all function names are prefixed
// with '__posix_'.
// For embedded environments it is possible to also add aliases to the
// standard, non-prefixed names, by adding the following preprocessor
// definition: OS_INCLUDE_STANDARD_POSIX_FUNCTIONS

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

  // Works only on STREAMS (CherDevices, in this implementation)
  if ((io->getType () & os::posix::IO::Type::DEVICE) == 0)
    {
      errno = ENOTTY; // Not a stream.
      return -1;
    }

  va_list args;
  va_start(args, request);
  int ret = static_cast<os::posix::CharDevice*> (io)->vioctl (request, args);
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
  auto* const sock = os::posix::socket (domain, type, protocol);
  if (sock == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return sock->getFileDescriptor ();
}

#if 0
int
__posix_socketpair (int domain, int type, int protocol, int socket_vector[2])
  {
    errno = ENOSYS; // Not implemented
    return -1;
  }
#endif

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

// ----------------------------------------------------------------------------
// Not yet implemented.

int __attribute__((weak))
__posix_readdir_r (DIR* dirp, struct dirent* entry, struct dirent** result)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int __attribute__((weak))
__posix_socketpair (int domain, int type, int protocol, int socket_vector[2])
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_gettimeofday (struct timeval* ptimeval, void* ptimezone)
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

// ----------------------------------------------------------------------------
// Unavailable in non-Unix embedded environments.

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

pid_t
__posix_wait (int* stat_loc)
{
  errno = ENOSYS; // Not implemented
  return ((pid_t) -1);
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

// ----------------------------------------------------------------------------

// This is the standard default implementation for the routine to
// process args. It returns a single empty arg.
//
// For semihosting applications, this is redefined to get the real
// args from the debugger.
//
// You can redefine it to fetch some args in a non-volatile memory.

void __attribute__((weak))
__initialize_args (int* p_argc, char*** p_argv)
{
  // By the time we reach this, the data and bss should have been initialised.

  // The strings pointed to by the argv array shall be modifiable by the
  // program, and retain their last-stored values between program startup
  // and program termination. (static, no const)
  static char name[] = "";

  // The string pointed to by argv[0] represents the program name;
  // argv[0][0] shall be the null character if the program name is not
  // available from the host environment. argv[argc] shall be a null pointer.
  // (static, no const)
  static char* argv[2] =
    { name, NULL };

  *p_argc = 1;
  *p_argv = &argv[0];
  return;
}

// ----------------------------------------------------------------------------

#pragma GCC diagnostic pop

#if defined(OS_INCLUDE_STANDARD_POSIX_FUNCTIONS)

// For embedded environment that use POSIX system calls, redefine
// all functions without the '__posix_' prefix.

extern "C"
  {

    int __attribute__((weak, alias ("__posix_accept")))
    accept (int socket, struct sockaddr* address, socklen_t* address_len);

    int __attribute__((weak, alias ("__posix_bind")))
    bind (int socket, const struct sockaddr* address, socklen_t address_len);

    int __attribute__((weak, alias ("__posix_chdir")))
    chdir (const char* path);

    int __attribute__((weak, alias ("__posix_chmod")))
    chmod (const char* path, mode_t mode);

    int __attribute__((weak, alias ("__posix_chown")))
    chown (const char* path, uid_t owner, gid_t group);

    int __attribute__((weak, alias ("__posix_close")))
    close (int fildes);

    int __attribute__((weak, alias ("__posix_closedir")))
    closedir (DIR* dirp);

    int __attribute__((weak, alias ("__posix_connect")))
    connect (int socket, const struct sockaddr* address, socklen_t address_len);

    int __attribute__((weak, alias ("__posix_execve")))
    execve (const char* path, char* const argv[], char* const envp[]);

    int __attribute__((weak, alias ("__posix_fcntl")))
    fcntl (int fildes, int cmd, ...);

    pid_t __attribute__((weak, alias ("__posix_fork")))
    fork (void);

    int __attribute__((weak, alias ("__posix_fstat")))
    fstat (int fildes, struct stat* buf);

    int __attribute__((weak, alias ("__posix_ftruncate")))
    ftruncate (int fildes, off_t length);

    int __attribute__((weak, alias ("__posix_fsync")))
    fsync (int fildes);

    char*
    __attribute__((weak, alias ("__posix_getcwd")))
    getcwd (char* buf, size_t size);

    int __attribute__((weak, alias ("__posix_getpeername")))
    getpeername (int socket, struct sockaddr* address, socklen_t* address_len);

    pid_t __attribute__((weak, alias ("__posix_getpid")))
    getpid (void);

    int __attribute__((weak, alias ("__posix_getsockname")))
    getsockname (int socket, struct sockaddr* address, socklen_t* address_len);

    int __attribute__((weak, alias ("__posix_getsockopt")))
    getsockopt (int socket, int level, int option_name, void* option_value,
        socklen_t* option_len);

    int __attribute__((weak, alias ("__posix_gettimeofday")))
    gettimeofday (struct timeval* ptimeval, void* ptimezone);

    int __attribute__((weak, alias ("__posix_ioctl")))
    ioctl (int fildes, int request, ...);

    int __attribute__((weak, alias ("__posix_isatty")))
    isatty (int fildes);

    int __attribute__((weak, alias ("__posix_kill")))
    kill (pid_t pid, int sig);

    int __attribute__((weak, alias ("__posix_link")))
    link (const char* existing, const char* _new);

    int __attribute__((weak, alias ("__posix_listen")))
    listen (int socket, int backlog);

    off_t __attribute__((weak, alias ("__posix_lseek")))
    lseek (int fildes, off_t offset, int whence);

    int __attribute__((weak, alias ("__posix_mkdir")))
    mkdir (const char* path, mode_t mode);

    int __attribute__((weak, alias ("__posix_open")))
    open (const char* path, int oflag, ...);

    DIR*
    __attribute__((weak, alias ("__posix_opendir")))
    opendir (const char* dirname);

    int __attribute__((weak, alias ("__posix_raise")))
    raise (int sig);

    ssize_t __attribute__((weak, alias ("__posix_read")))
    read (int fildes, void* buf, size_t nbyte);

    struct dirent*
    __attribute__((weak, alias ("__posix_readdir")))
    readdir (DIR* dirp);

    int __attribute__((weak, alias ("__posix_readdir_r")))
    readdir_r (DIR* dirp, struct dirent* entry, struct dirent** result);

    ssize_t __attribute__((weak, alias ("__posix_readlink")))
    readlink (const char* path, char* buf, size_t bufsize);

    ssize_t __attribute__((weak, alias ("__posix_recv")))
    recv (int socket, void* buffer, size_t length, int flags);

    ssize_t __attribute__((weak, alias ("__posix_recvfrom")))
    recvfrom (int socket, void* buffer, size_t length, int flags,
        struct sockaddr* address, socklen_t* address_len);

    ssize_t __attribute__((weak, alias ("__posix_recvmsg")))
    recvmsg (int socket, struct msghdr* message, int flags);

    int __attribute__((weak, alias ("__posix_rename")))
    rename (const char* oldfn, const char* newfn);

    void __attribute__((weak, alias ("__posix_rewinddir")))
    rewinddir (DIR* dirp);

    int __attribute__((weak, alias ("__posix_rmdir")))
    rmdir (const char* path);

    int __attribute__((weak, alias ("__posix_select")))
    select (int nfds, fd_set* readfds, fd_set* writefds, fd_set* errorfds,
        struct timeval* timeout);

    ssize_t __attribute__((weak, alias ("__posix_send")))
    send (int socket, const void* buffer, size_t length, int flags);

    ssize_t __attribute__((weak, alias ("__posix_sendmsg")))
    sendmsg (int socket, const struct msghdr* message, int flags);

    ssize_t __attribute__((weak, alias ("__posix_sendto")))
    sendto (int socket, const void* message, size_t length, int flags,
        const struct sockaddr* dest_addr, socklen_t dest_len);

    int __attribute__((weak, alias ("__posix_setsockopt")))
    setsockopt (int socket, int level, int option_name, const void* option_value,
        socklen_t option_len);

    int __attribute__((weak, alias ("__posix_shutdown")))
    shutdown (int socket, int how);

    int __attribute__((weak, alias ("__posix_sockatmark")))
    sockatmark (int socket);

    int __attribute__((weak, alias ("__posix_socket")))
    socket (int domain, int type, int protocol);

#if 0
    int __attribute__((weak, alias ("__posix_socketpair")))
    socketpair (int domain, int type, int protocol, int socket_vector[2]);
#endif

    int __attribute__((weak, alias ("__posix_stat")))
    stat (const char* path, struct stat* buf);

    void __attribute__((weak, alias ("__posix_sync")))
    sync (void);

    int __attribute__((weak, alias ("__posix_symlink")))
    symlink (const char* existing, const char* _new);

    clock_t __attribute__((weak, alias ("__posix_times")))
    times (struct tms* buf);

    int __attribute__((weak, alias ("__posix_truncate")))
    truncate (const char* path, off_t length);

    int __attribute__((weak, alias ("__posix_unlink")))
    unlink (const char* name);

    int __attribute__((weak, alias ("__posix_utime")))
    utime (const char* path, const struct utimbuf* times);

    pid_t __attribute__((weak, alias ("__posix_wait")))
    wait (int* stat_loc);

    ssize_t __attribute__((weak, alias ("__posix_write")))
    write (int fildes, const void* buf, size_t nbyte);

    ssize_t __attribute__((weak, alias ("__posix_writev")))
    writev (int fildes, const struct iovec* iov, int iovcnt);

  }

#elif defined(OS_INCLUDE_NEWLIB_POSIX_FUNCTIONS)

// For embedded environment that use POSIX system calls, redefine
// some functions with _name(), some directly with name().

extern "C"
  {

    int __attribute__((weak, alias ("__posix_accept")))
    accept (int socket, struct sockaddr* address, socklen_t* address_len);

    int __attribute__((weak, alias ("__posix_bind")))
    bind (int socket, const struct sockaddr* address, socklen_t address_len);

    int __attribute__((weak, alias ("__posix_chdir")))
    chdir (const char* path);

    int __attribute__((weak, alias ("__posix_chmod")))
    chmod (const char* path, mode_t mode);

    int __attribute__((weak, alias ("__posix_chown")))
    _chown (const char* path, uid_t owner, gid_t group);

    int __attribute__((weak, alias ("__posix_close")))
    _close (int fildes);

    int __attribute__((weak, alias ("__posix_closedir")))
    closedir (DIR* dirp);

    int __attribute__((weak, alias ("__posix_connect")))
    connect (int socket, const struct sockaddr* address, socklen_t address_len);

    int __attribute__((weak, alias ("__posix_execve")))
    _execve (const char* path, char* const argv[], char* const envp[]);

    int __attribute__((weak, alias ("__posix_fcntl")))
    fcntl (int fildes, int cmd, ...);

    pid_t __attribute__((weak, alias ("__posix_fork")))
    _fork (void);

    int __attribute__((weak, alias ("__posix_fstat")))
    _fstat (int fildes, struct stat* buf);

    int __attribute__((weak, alias ("__posix_ftruncate")))
    ftruncate (int fildes, off_t length);

    int __attribute__((weak, alias ("__posix_fsync")))
    fsync (int fildes);

    char*
    __attribute__((weak, alias ("__posix_getcwd")))
    getcwd (char* buf, size_t size);

    int __attribute__((weak, alias ("__posix_getpeername")))
    getpeername (int socket, struct sockaddr* address, socklen_t* address_len);

    pid_t __attribute__((weak, alias ("__posix_getpid")))
    _getpid (void);

    int __attribute__((weak, alias ("__posix_getsockname")))
    getsockname (int socket, struct sockaddr* address, socklen_t* address_len);

    int __attribute__((weak, alias ("__posix_getsockopt")))
    getsockopt (int socket, int level, int option_name, void* option_value,
        socklen_t* option_len);

    int __attribute__((weak, alias ("__posix_gettimeofday")))
    _gettimeofday (struct timeval* ptimeval, void* ptimezone);

    int __attribute__((weak, alias ("__posix_ioctl")))
    ioctl (int fildes, int request, ...);

    int __attribute__((weak, alias ("__posix_isatty")))
    _isatty (int fildes);

    int __attribute__((weak, alias ("__posix_kill")))
    _kill (pid_t pid, int sig);

    int __attribute__((weak, alias ("__posix_link")))
    _link (const char* existing, const char* _new);

    int __attribute__((weak, alias ("__posix_listen")))
    listen (int socket, int backlog);

    off_t __attribute__((weak, alias ("__posix_lseek")))
    _lseek (int fildes, off_t offset, int whence);

    int __attribute__((weak, alias ("__posix_mkdir")))
    mkdir (const char* path, mode_t mode);

    int __attribute__((weak, alias ("__posix_open")))
    _open (const char* path, int oflag, ...);

    DIR*
    __attribute__((weak, alias ("__posix_opendir")))
    opendir (const char* dirname);

    int __attribute__((weak, alias ("__posix_raise")))
    raise (int sig);

    ssize_t __attribute__((weak, alias ("__posix_read")))
    _read (int fildes, void* buf, size_t nbyte);

    struct dirent*
    __attribute__((weak, alias ("__posix_readdir")))
    readdir (DIR* dirp);

    int __attribute__((weak, alias ("__posix_readdir_r")))
    readdir_r (DIR* dirp, struct dirent* entry, struct dirent** result);

    ssize_t __attribute__((weak, alias ("__posix_readlink")))
    _readlink (const char* path, char* buf, size_t bufsize);

    ssize_t __attribute__((weak, alias ("__posix_recv")))
    recv (int socket, void* buffer, size_t length, int flags);

    ssize_t __attribute__((weak, alias ("__posix_recvfrom")))
    recvfrom (int socket, void* buffer, size_t length, int flags,
        struct sockaddr* address, socklen_t* address_len);

    ssize_t __attribute__((weak, alias ("__posix_recvmsg")))
    recvmsg (int socket, struct msghdr* message, int flags);

    int __attribute__((weak, alias ("__posix_rename")))
    rename (const char* oldfn, const char* newfn);

    void __attribute__((weak, alias ("__posix_rewinddir")))
    rewinddir (DIR* dirp);

    int __attribute__((weak, alias ("__posix_rmdir")))
    rmdir (const char* path);

    int __attribute__((weak, alias ("__posix_select")))
    select (int nfds, fd_set* readfds, fd_set* writefds, fd_set* errorfds,
        struct timeval* timeout);

    ssize_t __attribute__((weak, alias ("__posix_send")))
    send (int socket, const void* buffer, size_t length, int flags);

    ssize_t __attribute__((weak, alias ("__posix_sendmsg")))
    sendmsg (int socket, const struct msghdr* message, int flags);

    ssize_t __attribute__((weak, alias ("__posix_sendto")))
    sendto (int socket, const void* message, size_t length, int flags,
        const struct sockaddr* dest_addr, socklen_t dest_len);

    int __attribute__((weak, alias ("__posix_setsockopt")))
    setsockopt (int socket, int level, int option_name, const void* option_value,
        socklen_t option_len);

    int __attribute__((weak, alias ("__posix_shutdown")))
    shutdown (int socket, int how);

    int __attribute__((weak, alias ("__posix_sockatmark")))
    sockatmark (int socket);

    int __attribute__((weak, alias ("__posix_socket")))
    socket (int domain, int type, int protocol);

#if 0
    int __attribute__((weak, alias ("__posix_socketpair")))
    socketpair (int domain, int type, int protocol, int socket_vector[2]);
#endif

    int __attribute__((weak, alias ("__posix_stat")))
    _stat (const char* path, struct stat* buf);

    void __attribute__((weak, alias ("__posix_sync")))
    sync (void);

    int __attribute__((weak, alias ("__posix_symlink")))
    _symlink (const char* existing, const char* _new);

    clock_t __attribute__((weak, alias ("__posix_times")))
    _times (struct tms* buf);

    int __attribute__((weak, alias ("__posix_truncate")))
    truncate (const char* path, off_t length);

    int __attribute__((weak, alias ("__posix_unlink")))
    _unlink (const char* name);

    int __attribute__((weak, alias ("__posix_utime")))
    utime (const char* path, const struct utimbuf* times);

    pid_t __attribute__((weak, alias ("__posix_wait")))
    _wait (int* stat_loc);

    _ssize_t __attribute__((weak, alias ("__posix_write")))
    _write (int fildes, const void* buf, size_t nbyte);

    ssize_t __attribute__((weak, alias ("__posix_writev")))
    writev (int fildes, const struct iovec* iov, int iovcnt);

  }

#endif /* !defined(OS_INCLUDE_STANDARD_POSIX_FUNCTIONS) */

#if defined(OS_INCLUDE_NEWLIB_REENT_FUNCTIONS)

extern "C"
  {

    // Many newlib functions call the reentrant versions right away,
    // and these call the _name() implementations. To avoid this, the
    // shortcut is to simply skip the reentrant code (ignore the pointer)
    // and directly call the posix implementation.

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wunused-parameter"

    int __attribute__((weak))
    _close_r (void* ptr, int fildes)
      {
        return __posix_close (fildes);
      }

    int __attribute__((weak))
    _execve_r (void* ptr, const char* path, char* const argv[],
        char* const envp[])
      {
        return __posix_execve (path, argv, envp);
      }

    int __attribute__((weak))
    _fcntl_r (void* ptr, int fildes, int cmd, int arg)
      {
        return __posix_fcntl (fildes, cmd, arg);
      }

    pid_t __attribute__((weak))
    _fork_r (void* ptr)
      {
        return __posix_fork ();
      }

    int __attribute__((weak))
    _fstat_r (void* ptr, int fildes, struct stat* buf)
      {
        return __posix_fstat (fildes, buf);
      }

    int __attribute__((weak))
    _gettimeofday_r (void* ptr, struct timeval* ptimeval, void* ptimezone)
      {
        return __posix_gettimeofday (ptimeval, ptimezone);
      }

    int __attribute__((weak))
    _isatty_r (void* ptr, int fildes)
      {
        return __posix_isatty (fildes);
      }

    int __attribute__((weak))
    _kill_r (void* ptr, pid_t pid, int sig)
      {
        return __posix_kill (pid, sig);
      }

    int __attribute__((weak))
    _link_r (void* ptr, const char* existing, const char* _new)
      {
        return __posix_link (existing, _new);
      }

    off_t __attribute__((weak))
    _lseek_r (void* ptr, int fildes, off_t offset, int whence)
      {
        return __posix_lseek (fildes, offset, whence);
      }

    int __attribute__((weak))
    _mkdir_r (void* ptr, const char* path, mode_t mode)
      {
        return __posix_mkdir (path, mode);
      }

    int __attribute__((weak))
    _open_r (void* ptr, const char* path, int oflag, int mode)
      {
        return __posix_open (path, oflag, mode);
      }

    ssize_t __attribute__((weak))
    _read_r (void* ptr, int fildes, void* buf, size_t nbyte)
      {
        return __posix_read (fildes, buf, nbyte);
      }

    int __attribute__((weak))
    _rename_r (void* ptr, const char* oldfn, const char* newfn)
      {
        return __posix_rename (oldfn, newfn);
      }

    int __attribute__((weak))
    _stat_r (void* ptr, const char* path, struct stat* buf)
      {
        return __posix_stat (path, buf);
      }

    clock_t __attribute__((weak))
    _times_r (void* ptr, struct tms* buf)
      {
        return __posix_times (buf);
      }

    int __attribute__((weak))
    _unlink_r (void* ptr, const char* name)
      {
        return __posix_unlink (name);
      }

    pid_t __attribute__((weak))
    _wait_r (void* ptr, int* stat_loc)
      {
        return __posix_wait (stat_loc);
      }

    ssize_t __attribute__((weak))
    _write_r (void* ptr, int fildes, const void* buf, size_t nbyte)
      {
        return __posix_write (fildes, buf, nbyte);
      }

#pragma GCC diagnostic pop

  }

#endif // defined(OS_INCLUDE_NEWLIB_REENT_FUNCTIONS)

// ----------------------------------------------------------------------------

