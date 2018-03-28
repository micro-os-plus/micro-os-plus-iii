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

#if !defined(OS_USE_SEMIHOSTING_SYSCALLS)

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/posix-io/types.h>
#include <cmsis-plus/posix-io/file-descriptors-manager.h>
#include <cmsis-plus/posix-io/io.h>
#include <cmsis-plus/posix-io/char-device.h>
#include <cmsis-plus/posix-io/tty.h>
#include <cmsis-plus/posix-io/file.h>
#include <cmsis-plus/posix-io/file-system.h>
#include <cmsis-plus/posix-io/directory.h>
#include <cmsis-plus/posix-io/socket.h>

#include <cmsis-plus/posix/sys/uio.h>

#include <cmsis-plus/diag/trace.h>

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

extern "C"
{
  void
  initialise_monitor_handles (void);
}

// ----------------------------------------------------------------------------
// ---- POSIX IO functions ----------------------------------------------------

using namespace os;

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
  auto* const io = posix::vopen (path, oflag, args);
  va_end(args);

  if (io == nullptr)
    {
      // Return POSIX style error indicator.
      return -1;
    }

  // Return non-negative POSIX file descriptor.
  return io->file_descriptor ();
}

int
__posix_close (int fildes)
{
  // The flow is identical for all POSIX functions: identify the C++
  // object and call the corresponding C++ method.
  auto* const io = posix::file_descriptors_manager::io (fildes);
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
  auto* const io = posix::file_descriptors_manager::io (fildes);
  if (io == nullptr)
    {
      // STDIN
      if (fildes == 0)
        {
          return 0; // Default empty input (EOF).
        }
      errno = EBADF;
      return -1;
    }
  return io->read (buf, nbyte);
}

ssize_t
__posix_write (int fildes, const void* buf, size_t nbyte)
{
  auto* const io = posix::file_descriptors_manager::io (fildes);
  if (io == nullptr)
    {
      // STDOUT & STDERR
      if (fildes == 1 || fildes == 2)
        {
          return trace_write (buf, nbyte); // Default output on trace.
        }
      errno = EBADF;
      return -1;
    }
  return io->write (buf, nbyte);
}

ssize_t
__posix_writev (int fildes, const struct iovec* iov, int iovcnt)
{
  auto* const io = posix::file_descriptors_manager::io (fildes);
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
  auto* const io = posix::file_descriptors_manager::io (fildes);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }

  // Works only on STREAMS (CherDevices, in this implementation)
  if ((io->get_type () & posix::io::type::char_device) == 0)
    {
      errno = ENOTTY; // Not a stream.
      return -1;
    }

  va_list args;
  va_start(args, request);
  int ret = (static_cast<posix::char_device*> (io))->vioctl (request, args);
  va_end(args);

  return ret;
}

off_t
__posix_lseek (int fildes, off_t offset, int whence)
{
  auto* const io = posix::file_descriptors_manager::io (fildes);
  if (io == nullptr)
    {
      errno = EBADF; // Fildes is not an open file descriptor.
      return -1;
    }

  // Works only on files (Does not work on sockets, pipes or FIFOs...)
  if ((io->get_type () & posix::io::type::file) == 0)
    {
      errno = ESPIPE; // Not a file.
      return -1;
    }

  return (static_cast<posix::file*> (io))->lseek (offset, whence);
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
  auto* const io = posix::file_descriptors_manager::io (fildes);
  if (io == nullptr)
    {
      if (fildes <= 2)
        {
          return true; // Default TTY for STDIN/OUT/ERR.
        }
      errno = EBADF;
      return -1;
    }
  return io->isatty ();
}

int
__posix_tcdrain (int fildes)
{
  auto* const io = posix::file_descriptors_manager::io (fildes);
  if (io == nullptr)
    {
      errno = EBADF; // Fildes is not an open file descriptor.
      return -1;
    }

  // Works only on tty...)
  if ((io->get_type () & posix::io::type::tty) == 0)
    {
      errno = ESPIPE; // Not a tty.
      return -1;
    }

  return (static_cast<posix::tty*> (io))->tcdrain ();
}

int
__posix_tcgetattr (int fildes, struct termios *termios_p)
{
  auto* const io = posix::file_descriptors_manager::io (fildes);
  if (io == nullptr)
    {
      errno = EBADF; // Fildes is not an open file descriptor.
      return -1;
    }

  // Works only on tty...)
  if ((io->get_type () & posix::io::type::tty) == 0)
    {
      errno = ESPIPE; // Not a tty.
      return -1;
    }

  return (static_cast<posix::tty*> (io))->tcgetattr (termios_p);
}

int
__posix_tcsetattr (int fildes, int optional_actions,
                   const struct termios *termios_p)
{
  auto* const io = posix::file_descriptors_manager::io (fildes);
  if (io == nullptr)
    {
      errno = EBADF; // Fildes is not an open file descriptor.
      return -1;
    }

  // Works only on tty...)
  if ((io->get_type () & posix::io::type::tty) == 0)
    {
      errno = ESPIPE; // Not a tty.
      return -1;
    }

  return (static_cast<posix::tty*> (io))->tcsetattr (optional_actions,
                                                     termios_p);
}

int
__posix_tcflush (int fildes, int queue_selector)
{
  auto* const io = posix::file_descriptors_manager::io (fildes);
  if (io == nullptr)
    {
      errno = EBADF; // Fildes is not an open file descriptor.
      return -1;
    }

  // Works only on tty...)
  if ((io->get_type () & posix::io::type::tty) == 0)
    {
      errno = ESPIPE; // Not a tty.
      return -1;
    }

  return (static_cast<posix::tty*> (io))->tcflush (queue_selector);
}

int
__posix_tcsendbreak (int fildes, int duration)
{
  auto* const io = posix::file_descriptors_manager::io (fildes);
  if (io == nullptr)
    {
      errno = EBADF; // Fildes is not an open file descriptor.
      return -1;
    }

  // Works only on tty...)
  if ((io->get_type () & posix::io::type::tty) == 0)
    {
      errno = ESPIPE; // Not a tty.
      return -1;
    }

  return (static_cast<posix::tty*> (io))->tcsendbreak (duration);
}

int
__posix_fcntl (int fildes, int cmd, ...)
{
  auto* const io = posix::file_descriptors_manager::io (fildes);
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
  auto* const io = posix::file_descriptors_manager::io (fildes);
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
  auto* const io = posix::file_descriptors_manager::io (fildes);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }

  // Works only on files (Does not work on sockets, pipes or FIFOs...)
  if ((io->get_type () & posix::io::type::file) == 0)
    {
      errno = EINVAL; // Not a file.
      return -1;
    }

  return (static_cast<posix::file*> (io))->ftruncate (length);
}

int
__posix_fsync (int fildes)
{
  auto* const io = posix::file_descriptors_manager::io (fildes);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }

  // Works only on files (Does not work on sockets, pipes or FIFOs...)
  if ((io->get_type () & posix::io::type::file) == 0)
    {
      errno = EINVAL; // Not a file.
      return -1;
    }

  return (static_cast<posix::file*> (io))->fsync ();
}

// ----------------------------------------------------------------------------
// ----- POSIX file functions -----

int
__posix_chmod (const char* path, mode_t mode)
{
  return posix::chmod (path, mode);
}

int
__posix_stat (const char* path, struct stat* buf)
{
  return posix::stat (path, buf);
}

int
__posix_truncate (const char* path, off_t length)
{
  return posix::truncate (path, length);
}

int
__posix_rename (const char* existing, const char* _new)
{
  return posix::rename (existing, _new);
}

int
__posix_unlink (const char* path)
{
  return posix::unlink (path);
}

int
__posix_utime (const char* path, const struct utimbuf* times)
{
  return posix::utime (path, times);
}

// ----------------------------------------------------------------------------
// ----- POSIX file_system functions -----

int
__posix_mkdir (const char* path, mode_t mode)
{
  return posix::mkdir (path, mode);
}

int
__posix_rmdir (const char* path)
{
  return posix::rmdir (path);
}

void
__posix_sync (void)
{
  return posix::sync ();
}

// ----------------------------------------------------------------------------
// ----- Directories functions -----

DIR*
__posix_opendir (const char* dirpath)
{
  return reinterpret_cast<DIR*> (posix::opendir (dirpath));
}

struct dirent*
__posix_readdir (DIR* dirp)
{
  auto* const dir = reinterpret_cast<posix::directory*> (dirp);
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
  auto* const dir = reinterpret_cast<posix::directory*> (dirp);
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
  auto* const dir = reinterpret_cast<posix::directory*> (dirp);
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
  auto* const sock = posix::socket (domain, type, protocol);
  if (sock == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  return sock->file_descriptor ();
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
  auto* const io = posix::file_descriptors_manager::socket (socket);
  if (io == nullptr)
    {
      errno = EBADF;
      return -1;
    }
  auto* const new_socket = io->accept (address, address_len);
  return new_socket->file_descriptor ();
}

int
__posix_bind (int socket, const struct sockaddr* address, socklen_t address_len)
{
  auto* const io = posix::file_descriptors_manager::socket (socket);
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
  auto* const io = posix::file_descriptors_manager::socket (socket);
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
  auto* const io = posix::file_descriptors_manager::socket (socket);
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
  auto* const io = posix::file_descriptors_manager::socket (socket);
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
  auto* const io = posix::file_descriptors_manager::socket (socket);
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
  auto* const io = posix::file_descriptors_manager::socket (socket);
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
  auto* const io = posix::file_descriptors_manager::socket (socket);
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
  auto* const io = posix::file_descriptors_manager::socket (socket);
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
  auto* const io = posix::file_descriptors_manager::socket (socket);
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
  auto* const io = posix::file_descriptors_manager::socket (socket);
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
  auto* const io = posix::file_descriptors_manager::socket (socket);
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
  auto* const io = posix::file_descriptors_manager::socket (socket);
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
  auto* const io = posix::file_descriptors_manager::socket (socket);
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
  auto* const io = posix::file_descriptors_manager::socket (socket);
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
  auto* const io = posix::file_descriptors_manager::socket (socket);
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
// If you detect other functions needed, just let us know
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
  ptimeval->tv_sec = static_cast<time_t> (os::rtos::rtclock.now ());
  ptimeval->tv_usec = 0;

  return 0;
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
  return static_cast<clock_t> (-1);
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

clock_t
__posix_clock (void)
{
  errno = ENOSYS; // Not implemented
  return static_cast<clock_t> (-1);
}

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
  return static_cast<pid_t> (-1);
}

pid_t
__posix_getpid (void)
{
  errno = ENOSYS; // Not implemented
  return static_cast<pid_t> (-1);
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
__posix_system (const char *command)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

pid_t
__posix_wait (int* stat_loc)
{
  errno = ENOSYS; // Not implemented
  return static_cast<pid_t> (-1);
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
  return static_cast<ssize_t> (-1);
}

#pragma GCC diagnostic pop

// `initialise_monitor_handles()` is a newlib libgloss function used to prepare
// the stdio files when using semihosting. Better keep the name the same.

void
initialise_monitor_handles (void)
{
  // Default STDIN, STDOUT, STDERR not required, the  __posix_write()
  // implementation defaults STDOUT/ERR to trace_write().
}

// ----------------------------------------------------------------------------

#if defined(__ARM_EABI__) && (__STDC_HOSTED__ != 0)

// The aliases must be in the same compilation unit as the names
// they alias.

#if defined(OS_INCLUDE_NEWLIB_POSIX_FUNCTIONS)

// For special embedded environment that use POSIX system calls
// with the newlib reentrant code, redefine
// some functions with _name(), others directly with name().

#include <cmsis-plus/posix-io/c-syscalls-aliases-newlib.h>

#else

// For regular embedded environment that use POSIX system calls,
// redefine **all** functions without the '__posix_' prefix.

#include <cmsis-plus/posix-io/c-syscalls-aliases-standard.h>

#endif

#endif /* defined(__ARM_EABI__) && (__STDC_HOSTED__ != 0) */

#endif /* !defined(OS_USE_SEMIHOSTING_SYSCALLS) */

// ----------------------------------------------------------------------------

