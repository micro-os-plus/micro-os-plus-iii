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

#if defined(__ARM_EABI__)

// ----------------------------------------------------------------------------

#include <cmsis-plus/os-app-config.h>
#include <cmsis-plus/rtos/os-hooks.h>

#if defined(OS_USE_SEMIHOSTING_SYSCALLS)

#include <cmsis-plus/arm/semihosting.h>
#include <cmsis-plus/diag/trace.h>

#include <cmsis-plus/posix-io/types.h>

#include <cmsis-plus/posix/dirent.h>
#include <cmsis-plus/posix/sys/socket.h>

#include "cmsis_device.h"

#include <cstring>

#include <cstdint>
#include <cstdarg>
#include <cerrno>

#include <sys/fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <ctype.h>
#include <sys/wait.h>
#include <unistd.h>

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
// In a multi-threaded environment, __errno() must return a
// thread specific pointer.

// Documentation:
// http://infocenter.arm.com/help/topic/com.arm.doc.dui0205g/DUI0205.pdf

// ----------------------------------------------------------------------------

// Struct used to keep track of the file position, just so we
// can implement fseek(fh,x,SEEK_CUR).
struct fdent
{
  int handle;
  int pos;
};

/*
 *  User file descriptors (fd) are integer indexes into
 * the openfiles[] array. Error checking is done by using
 * findslot().
 *
 * This openfiles array is manipulated directly by only
 * these 5 functions:
 *
 * findslot() - Translate entry.
 * newslot() - Find empty entry.
 * initilise_monitor_handles() - Initialize entries.
 * _swiopen() - Initialize entry.
 * _close() - Handle stdout == stderr case.
 *
 * Every other function must use findslot().
 */

#if !defined(OS_INTEGER_SEMIHOSTING_MAX_OPEN_FILES)
#define OS_INTEGER_SEMIHOSTING_MAX_OPEN_FILES (20)
#endif

static struct fdent openfiles[OS_INTEGER_SEMIHOSTING_MAX_OPEN_FILES];

// ----------------------------------------------------------------------------
// Support functions.

// Return a pointer to the structure associated with
// the user file descriptor fd.
static struct fdent*
__semihosting_findslot (int fd)
{
  // User file descriptor is out of range.
  if ((unsigned int) fd >= OS_INTEGER_SEMIHOSTING_MAX_OPEN_FILES)
    {
      return nullptr;
    }

  // User file descriptor is open?
  if (openfiles[fd].handle == -1)
    {
      return nullptr;
    }

  // Valid.
  return &openfiles[fd];
}

// Return the next lowest numbered free file
// structure, or -1 if we can't find one.
static int
__semihosting_newslot (void)
{
  int i;
  for (i = 0; i < OS_INTEGER_SEMIHOSTING_MAX_OPEN_FILES; i++)
    {
      if (openfiles[i].handle == -1)
        {
          break;
        }
    }

  if (i == OS_INTEGER_SEMIHOSTING_MAX_OPEN_FILES)
    {
      return -1;
    }

  return i;
}

static int
__semihosting_get_errno (void)
{
  return call_host (SEMIHOSTING_SYS_ERRNO, nullptr);
}

// Set errno and return result.
static int
__semihosting_error (int result)
{
  errno = __semihosting_get_errno ();
  return result;
}

// Check the return and set errno appropriately.
static int
__semihosting_checkerror (int result)
{
  if (result == -1)
    {
      return __semihosting_error (-1);
    }

  return result;
}

/* fd, is a user file descriptor. */
static int
__semihosting_lseek (int fd, int ptr, int dir)
{
  struct fdent *pfd;

  /* Valid file descriptor? */
  pfd = __semihosting_findslot (fd);
  if (pfd == NULL)
    {
      errno = EBADF;
      return -1;
    }

  /* Valid whence? */
  if ((dir != SEEK_CUR) && (dir != SEEK_SET) && (dir != SEEK_END))
    {
      errno = EINVAL;
      return -1;
    }

  /* Convert SEEK_CUR to SEEK_SET */
  if (dir == SEEK_CUR)
    {
      ptr = pfd->pos + ptr;
      /* The resulting file offset would be negative. */
      if (ptr < 0)
        {
          errno = EINVAL;
          if ((pfd->pos > 0) && (ptr > 0))
            {
              errno = EOVERFLOW;
            }
          return -1;
        }
      dir = SEEK_SET;
    }

  int block[2];
  int res;

  if (dir == SEEK_END)
    {
      block[0] = pfd->handle;
      res = __semihosting_checkerror (call_host (SEMIHOSTING_SYS_FLEN, block));
      if (res == -1)
        {
          return -1;
        }
      ptr += res;
    }

  /* This code only does absolute seeks.  */
  block[0] = pfd->handle;
  block[1] = ptr;
  res = __semihosting_checkerror (call_host (SEMIHOSTING_SYS_SEEK, block));

  /* At this point ptr is the current file position. */
  if (res >= 0)
    {
      pfd->pos = ptr;
      return ptr;
    }
  else
    {
      return -1;
    }
}

static int
__semihosting_stat (int fd, struct stat* st)
{
  struct fdent *pfd;
  pfd = __semihosting_findslot (fd);
  if (pfd == NULL)
    {
      errno = EBADF;
      return -1;
    }

  /* Always assume a character device,
   with 1024 byte blocks. */
  st->st_mode |= S_IFCHR;
  st->st_blksize = 1024;

  int res;
  res = __semihosting_checkerror (
      call_host (SEMIHOSTING_SYS_FLEN, &pfd->handle));
  if (res == -1)
    {
      return -1;
    }

  /* Return the file size. */
  st->st_size = res;
  return 0;
}

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

  int fd = __semihosting_newslot ();
  if (fd == -1)
    {
      errno = EMFILE;
      return -1;
    }

  /* It is an error to open a file that already exists. */
  if ((oflag & O_CREAT) && (oflag & O_EXCL))
    {
      struct stat st;
      int res;
      res = __posix_stat (path, &st);
      if (res != -1)
        {
          errno = EEXIST;
          return -1;
        }
    }

  int aflags = 0;
  /* The flags are Unix-style, so we need to convert them. */
#ifdef O_BINARY
  if (oflag & O_BINARY)
    {
      aflags |= 1;
    }
#endif

  /* In O_RDONLY we expect aflags == 0. */

  if (oflag & O_RDWR)
    {
      aflags |= 2;
    }

  if ((oflag & O_CREAT) || (oflag & O_TRUNC) || (oflag & O_WRONLY))
    {
      aflags |= 4;
    }

  if (oflag & O_APPEND)
    {
      /* Can't ask for w AND a; means just 'a'.  */
      aflags &= ~4;
      aflags |= 8;
    }

  uint32_t block[3];
  block[0] = (uint32_t) path;
  block[2] = std::strlen (path);
  block[1] = (uint32_t) aflags;

  int fh = call_host (SEMIHOSTING_SYS_OPEN, block);

  /* Return a user file descriptor or an error. */
  if (fh >= 0)
    {
      openfiles[fd].handle = fh;
      openfiles[fd].pos = 0;
      return fd;
    }
  else
    {
      return __semihosting_error (fh);
    }
}

int
__posix_close (int fildes)
{
  struct fdent *pfd;
  pfd = __semihosting_findslot (fildes);
  if (pfd == NULL)
    {
      errno = EBADF;
      return -1;
    }

  // Handle stderr == stdout.
  if ((fildes == 1 || fildes == 2)
      && (openfiles[1].handle == openfiles[2].handle))
    {
      pfd->handle = -1;
      return 0;
    }

  int block[1];
  block[0] = pfd->handle;

  // Attempt to close the handle.
  int res;
  res = __semihosting_checkerror (call_host (SEMIHOSTING_SYS_CLOSE, block));

  // Reclaim handle?
  if (res == 0)
    {
      pfd->handle = -1;
    }

  return res;
}

// ----------------------------------------------------------------------------

// fd, is a valid user file handle.
// Translates the return of SYS_READ into
// bytes read.

ssize_t
__posix_read (int fildes, void* buf, size_t nbyte)
{
  struct fdent *pfd;
  pfd = __semihosting_findslot (fildes);
  if (pfd == NULL)
    {
      errno = EBADF;
      return -1;
    }

  int block[3];
  block[0] = pfd->handle;
  block[1] = (int) buf;
  block[2] = nbyte;

  int res;
  // Returns the number of bytes *not* written.
  res = __semihosting_checkerror (call_host (SEMIHOSTING_SYS_READ, block));
  if (res == -1)
    {
      return res;
    }

  pfd->pos += nbyte - res;

  /* res == nbyte is not an error,
   at least if we want feof() to work.  */
  return nbyte - res;
}

ssize_t
__posix_write (int fildes, const void* buf, size_t nbyte)
{
  struct fdent *pfd;
  pfd = __semihosting_findslot (fildes);
  if (pfd == NULL)
    {
      errno = EBADF;
      return -1;
    }

  int block[3];

  block[0] = pfd->handle;
  block[1] = (int) buf;
  block[2] = nbyte;

  // Returns the number of bytes *not* written.
  int res;
  res = __semihosting_checkerror (call_host (SEMIHOSTING_SYS_WRITE, block));
  /* Clearly an error. */
  if (res < 0)
    {
      return -1;
    }

  pfd->pos += nbyte - res;

  // Did we write 0 bytes?
  // Retrieve errno for just in case.
  if ((nbyte - res) == 0)
    {
      return __semihosting_error (0);
    }

  return (nbyte - res);
}

off_t
__posix_lseek (int fildes, off_t offset, int whence)
{
  return __semihosting_lseek (fildes, offset, whence);
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
  struct fdent *pfd;
  pfd = __semihosting_findslot (fildes);
  if (pfd == NULL)
    {
      errno = EBADF;
      return 0;
    }

  int tty;
  tty = call_host (SEMIHOSTING_SYS_ISTTY, &pfd->handle);

  if (tty == 1)
    {
      return 1;
    }

  errno = __semihosting_get_errno ();
  return 0;
}

int
__posix_fstat (int fildes, struct stat* buf)
{
  memset (buf, 0, sizeof(*buf));
  return __semihosting_stat (fildes, buf);
}

// ----------------------------------------------------------------------------
// ----- POSIX File functions -----

int
__posix_stat (const char* path, struct stat* buf)
{
  int fd;
  memset (buf, 0, sizeof(*buf));
  // The best we can do is try to open the file read only.
  // If it exists, then we can guess a few things about it.
  if ((fd = __posix_open (path, O_RDONLY)) == -1)
    {
      return -1;
    }
  buf->st_mode |= S_IFREG | S_IREAD;
  int res = __semihosting_stat (fd, buf);
  // Not interested in the error.
  __posix_close (fd);
  return res;
}

int
__posix_rename (const char* existing, const char* _new)
{
  uint32_t block[4];
  block[0] = (uint32_t) existing;
  block[1] = std::strlen (existing);
  block[2] = (uint32_t) _new;
  block[3] = std::strlen (_new);
  return
      __semihosting_checkerror (call_host (SEMIHOSTING_SYS_RENAME, block)) ?
          -1 : 0;
}

int
__posix_unlink (const char* path)
{
  uint32_t block[2];
  block[0] = (uint32_t) path;
  block[1] = strlen (path);

  int res;
  res = call_host (SEMIHOSTING_SYS_REMOVE, block);
  if (res == -1)
    {
      return __semihosting_error (res);
    }
  return 0;
}

int
__posix_system (const char *command)
{
  // Hmmm.  The ARM debug interface specification doesn't say whether
  // SYS_SYSTEM does the right thing with a null argument, or assign any
  // meaning to its return value.  Try to do something reasonable....
  if (command == nullptr)
    {
      return 1; // maybe there is a shell available? we can hope. :-P
    }

  uint32_t block[2];
  block[0] = (uint32_t) command;
  block[1] = strlen (command);
  int e = __semihosting_checkerror (call_host (SEMIHOSTING_SYS_SYSTEM, block));
  if ((e >= 0) && (e < 256))
    {
      // We have to convert e, an exit status to the encoded status of
      // the command.  To avoid hard coding the exit status, we simply
      // loop until we find the right position.
      int exit_code;

      for (exit_code = e; (e != 0) && (WEXITSTATUS (e) != exit_code); e <<= 1)
        {
          continue;
        }
    }
  return e;
}

int
__posix_gettimeofday (struct timeval* ptimeval, void* ptimezone)
{
  struct timezone* tzp = (struct timezone*) ptimezone;
  if (ptimeval)
    {
      // Ask the host for the seconds since the Unix epoch.
      ptimeval->tv_sec = call_host (SEMIHOSTING_SYS_TIME, NULL);
      ptimeval->tv_usec = 0;
    }

  // Return fixed data for the time zone.
  if (tzp)
    {
      tzp->tz_minuteswest = 0;
      tzp->tz_dsttime = 0;
    }

  return 0;
}

// Return a clock that ticks at 100Hz.
clock_t
__posix_clock (void)
{
  clock_t timeval;
  timeval = (clock_t) call_host (SEMIHOSTING_SYS_CLOCK, NULL);

  return timeval;
}

clock_t
__posix_times (struct tms* buf)
{
  clock_t timeval = __posix_clock ();
  if (buf)
    {
      buf->tms_utime = timeval; // user time
      buf->tms_stime = 0; // system time
      buf->tms_cutime = 0; // user time, children
      buf->tms_cstime = 0; // system time, children
    }

  return timeval;
}

char*
__posix_getcwd (char* buf, size_t size)
{
  // no cwd available via semihosting, so we use the temporary folder
  strncpy (buf, "/tmp", size);
  return buf;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// ----------------------------------------------------------------------------
// ----- POSIX FileSystem functions -----

// Required by Google Tests
int
__posix_mkdir (const char* path, mode_t mode)
{
#if 0
  // always return true
  return 0;
#else
  errno = ENOSYS;
  return -1;
#endif
}

int
__posix_rmdir (const char* path)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

void
__posix_sync (void)
{
  errno = ENOSYS; // Not implemented
}

// ----------------------------------------------------------------------------
// ----- Directories functions -----

DIR*
__posix_opendir (const char* dirpath)
{
  errno = ENOSYS; // Not implemented
  return nullptr;
}

struct dirent*
__posix_readdir (DIR* dirp)
{
  errno = ENOSYS; // Not implemented
  return nullptr;
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
  errno = ENOSYS; // Not implemented
}

int
__posix_closedir (DIR* dirp)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

#pragma GCC diagnostic pop

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
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_bind (int socket, const struct sockaddr* address, socklen_t address_len)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_connect (int socket, const struct sockaddr* address,
                 socklen_t address_len)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_getpeername (int socket, struct sockaddr* address,
                     socklen_t* address_len)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_getsockname (int socket, struct sockaddr* address,
                     socklen_t* address_len)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_getsockopt (int socket, int level, int option_name, void* option_value,
                    socklen_t* option_len)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_listen (int socket, int backlog)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

ssize_t
__posix_recv (int socket, void* buffer, size_t length, int flags)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

ssize_t
__posix_recvfrom (int socket, void* buffer, size_t length, int flags,
                  struct sockaddr* address, socklen_t* address_len)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

ssize_t
__posix_recvmsg (int socket, struct msghdr* message, int flags)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

ssize_t
__posix_send (int socket, const void* buffer, size_t length, int flags)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

ssize_t
__posix_sendmsg (int socket, const struct msghdr* message, int flags)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

ssize_t
__posix_sendto (int socket, const void* message, size_t length, int flags,
                const struct sockaddr* dest_addr, socklen_t dest_len)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_setsockopt (int socket, int level, int option_name,
                    const void* option_value, socklen_t option_len)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_shutdown (int socket, int how)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_sockatmark (int socket)
{
  errno = ENOSYS; // Not implemented
  return -1;
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
__posix_select (int nfds, fd_set* readfds, fd_set* writefds, fd_set* errorfds,
                struct timeval* timeout)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_chdir (const char* path)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

// ----------------------------------------------------------------------------
// Not available via semihosting.

ssize_t
__posix_writev (int fildes, const struct iovec* iov, int iovcnt)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_ioctl (int fildes, int request, ...)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_fcntl (int fildes, int cmd, ...)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_ftruncate (int fildes, off_t length)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_fsync (int fildes)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_chmod (const char* path, mode_t mode)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_truncate (const char* path, off_t length)
{
  errno = ENOSYS; // Not implemented
  return -1;
}

int
__posix_utime (const char* path, const struct utimbuf* times)
{
  errno = ENOSYS; // Not implemented
  return -1;
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
  return 1;
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

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

extern "C"
{
  void
  initialise_monitor_handles (void);
}

// ----------------------------------------------------------------------------

void
__attribute__ ((noreturn,weak))
os_terminate (int code __attribute__((unused)))
{
  /* There is only one SWI for both _exit and _kill. For _exit, call
   the SWI with the second argument set to -1, an invalid value for
   signum, so that the SWI handler can distinguish the two calls.
   Note: The RDI implementation of _kill throws away both its
   arguments.  */
  report_exception (
      code == 0 ? ADP_Stopped_ApplicationExit : ADP_Stopped_RunTimeError);
  /* NOTREACHED */
}

// ----------------------------------------------------------------------------

// This is the semihosting implementation for the routine to
// process args.
// The entire command line is received from the host
// and parsed into strings.

#define ARGS_BUF_ARRAY_SIZE 80
#define ARGV_BUF_ARRAY_SIZE 10

typedef struct command_line_block_s
{
  char* command_line;
  int size;
} command_line_block_t;

void
os_startup_initialize_args (int* p_argc, char*** p_argv)
{
  // Array of chars to receive the command line from the host.
  static char args_buf[ARGS_BUF_ARRAY_SIZE];

  // Array of pointers to store the final argv pointers (pointing
  // in the above array).
  static char* argv_buf[ARGV_BUF_ARRAY_SIZE];

  int argc = 0;
  int is_in_argument = 0;

  command_line_block_t cmd_block;
  cmd_block.command_line = args_buf;
  cmd_block.size = sizeof(args_buf) - 1;

  int ret = call_host (SEMIHOSTING_SYS_GET_CMDLINE, &cmd_block);
  if (ret == 0)
    {
      // In case the host send more than we can chew, limit the
      // string to our buffer.
      args_buf[ARGS_BUF_ARRAY_SIZE - 1] = '\0';

      // The command line is a null terminated string.
      char* p = cmd_block.command_line;

      int delim = '\0';
      int ch;

      while ((ch = *p) != '\0')
        {
          if (is_in_argument == 0)
            {
              if (!isblank (ch))
                {
                  if (argc
                      >= (int) ((sizeof(argv_buf) / sizeof(argv_buf[0])) - 1))
                    break;

                  if (ch == '"' || ch == '\'')
                    {
                      // Remember the delimiter to search for the
                      // corresponding terminator.
                      delim = ch;
                      ++p; // skip the delimiter.
                      ch = *p;
                    }
                  // Remember the arg beginning address.
                  argv_buf[argc++] = p;
                  is_in_argument = 1;
                }
            }
          else if (delim != '\0')
            {
              if ((ch == delim))
                {
                  delim = '\0';
                  *p = '\0';
                  is_in_argument = 0;
                }
            }
          else if (isblank (ch))
            {
              delim = '\0';
              *p = '\0';
              is_in_argument = 0;
            }
          ++p;
        }
    }

  if (argc == 0)
    {
      // No args found in string, return a single empty name.
      args_buf[0] = '\0';
      argv_buf[0] = &args_buf[0];
      ++argc;
    }

  // Must end the array with a null pointer.
  argv_buf[argc] = NULL;

  *p_argc = argc;
  *p_argv = &argv_buf[0];

  // temporary here
  initialise_monitor_handles ();

  return;
}

static int monitor_stdin;
static int monitor_stdout;
static int monitor_stderr;

// ----------------------------------------------------------------------------

void
initialise_monitor_handles (void)
{
  // Open the standard file descriptors by opening the special
  // teletype device, ":tt", read-only to obtain a descriptor for
  // standard input and write-only to obtain a descriptor for standard
  // output. Finally, open ":tt" in append mode to obtain a descriptor
  // for standard error. Since this is a write mode, most kernels will
  // probably return the same value as for standard output, but the
  // kernel can differentiate the two using the mode flag and return a
  // different descriptor for standard error.

  int volatile block[3];

  block[0] = (int) ":tt";
  block[2] = 3; // length of filename
  block[1] = 0; // mode "r"
  monitor_stdin = call_host (SEMIHOSTING_SYS_OPEN, (void*) block);

  block[0] = (int) ":tt";
  block[2] = 3; // length of filename
  block[1] = 4; // mode "w"
  monitor_stdout = call_host (SEMIHOSTING_SYS_OPEN, (void*) block);

  block[0] = (int) ":tt";
  block[2] = 3; // length of filename
  block[1] = 8; // mode "a"
  monitor_stderr = call_host (SEMIHOSTING_SYS_OPEN, (void*) block);

  // If we failed to open stderr, redirect to stdout.
  if (monitor_stderr == -1)
    {
      monitor_stderr = monitor_stdout;
    }

  for (int i = 0; i < OS_INTEGER_SEMIHOSTING_MAX_OPEN_FILES; i++)
    {
      openfiles[i].handle = -1;
    }

  openfiles[0].handle = monitor_stdin;
  openfiles[0].pos = 0;
  openfiles[1].handle = monitor_stdout;
  openfiles[1].pos = 0;
  openfiles[2].handle = monitor_stderr;
  openfiles[2].pos = 0;
}

// ----------------------------------------------------------------------------

#if (__STDC_HOSTED__ != 0)

// The aliases must be in the same compilation unit as the names
// they alias.

#if defined(OS_INCLUDE_NEWLIB_POSIX_FUNCTIONS)

// For special embedded environment that use POSIX system calls
// with the newlib reentrant code, redefine
// some functions with _name(), others directly with name().

#include <cmsis-plus/posix-io/newlib-aliases.h>

#else

// For regular embedded environment that use POSIX system calls,
// redefine **all** functions without the '__posix_' prefix.

#include <cmsis-plus/posix-io/standard-aliases.h>

#endif

#endif /* (__STDC_HOSTED__ != 0) */
#endif /* defined(OS_USE_SEMIHOSTING_SYSCALLS) */

// ----------------------------------------------------------------------------

#endif /* defined(__ARM_EABI__) */

