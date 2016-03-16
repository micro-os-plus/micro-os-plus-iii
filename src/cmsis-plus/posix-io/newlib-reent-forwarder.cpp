/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
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

#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>

/*
 * Some newlib functions call directly the '_r' functions.
 * When 'impure' mechanism is not used, these functions should
 * reach the actual posix implementation, ignoring the pointer.
 */
extern "C"
{

  int
  __posix_close (int fildes);

  int
  __posix_fstat (int fildes, struct stat *buf);

  pid_t
  __posix_getpid (void);

  int
  __posix_isatty (int fildes);

  off_t
  __posix_lseek (int fildes, off_t offset, int whence);

  ssize_t
  __posix_read (int fildes, void *buf, size_t nbyte);

  ssize_t
  __posix_write (int fildes, const void *buf, size_t nbyte);

  // ----

  int
  _close_r (void* ptr, int fildes);

  int
  _fstat_r (void* ptr, int fildes, struct stat *buf);

  pid_t
  _getpid_r (void* ptr);

  int
  _isatty_r (void* ptr, int fildes);

  off_t
  _lseek_r (void* ptr, int fildes, off_t offset, int whence);

  ssize_t
  _read_r (void* ptr, int fildes, void *buf, size_t nbyte);

  ssize_t
  _write_r (void* ptr, int fildes, const void *buf, size_t nbyte);

}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

pid_t
_getpid_r (void* ptr)
{
  return __posix_getpid ();
}

ssize_t
_write_r (void* ptr, int fildes, const void *buf, size_t nbyte)
{
  return __posix_write (fildes, buf, nbyte);
}

int
_close_r (void* ptr, int fildes)
{
  return __posix_close (fildes);
}

int
_fstat_r (void* ptr, int fildes, struct stat *buf)
{
  return __posix_fstat (fildes, buf);
}

int
_isatty_r (void* ptr, int fildes)
{
  return __posix_isatty (fildes);
}

off_t
_lseek_r (void* ptr, int fildes, off_t offset, int whence)
{
  return __posix_lseek (fildes, offset, whence);
}

ssize_t
_read_r (void* ptr, int fildes, void *buf, size_t nbyte)
{
  return __posix_read (fildes, buf, nbyte);
}

#pragma GCC diagnostic pop
