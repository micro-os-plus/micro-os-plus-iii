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

#ifndef POSIX_IO_NEWLIB_ALIASES_H_
#define POSIX_IO_NEWLIB_ALIASES_H_

#ifdef __cplusplus
extern "C"
{
#endif

  // For embedded environment that use POSIX system calls, redefine
  // some functions with _name(), some directly with name().

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

  clock_t __attribute__((weak, alias ("__posix_clock")))
  _clock (void);

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

  int __attribute__((weak, alias ("__posix_system")))
  system (const char *command);

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

  ssize_t __attribute__((weak, alias ("__posix_write")))
  _write (int fildes, const void* buf, size_t nbyte);

  ssize_t __attribute__((weak, alias ("__posix_writev")))
  writev (int fildes, const struct iovec* iov, int iovcnt);

#ifdef __cplusplus
}
#endif

#endif /* POSIX_IO_NEWLIB_ALIASES_H_ */

// ----------------------------------------------------------------------------

