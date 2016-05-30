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

#ifndef POSIX_IO_TYPES_H_
#define POSIX_IO_TYPES_H_

// ----------------------------------------------------------------------------

#include <sys/types.h>

#include <cmsis-plus/posix/dirent.h>
#include <cmsis-plus/posix/sys/socket.h>

// ----------------------------------------------------------------------------

#ifdef __cplusplus

namespace os
{
  namespace posix
  {

    typedef int fileDescriptor_t;

    constexpr fileDescriptor_t noFileDescriptor = -1;

  } /* namespace posix */
} /* namespace os */

#endif /* __cplusplus */

#ifdef __cplusplus
extern "C"
{
#endif

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

  clock_t __attribute__((weak))
  __posix_clock (void);

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

  int __attribute__((weak))
  __posix_readdir_r (DIR* dirp, struct dirent* entry, struct dirent** result);

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

  int __attribute__((weak))
  __posix_system(const char *command);

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

  // --------------------------------------------------------------------------

  void
  os_initialize_args (int* p_argc, char*** p_argv);

#ifdef __cplusplus
}
#endif

// ----------------------------------------------------------------------------

#endif /* POSIX_IO_TYPES_H_ */
