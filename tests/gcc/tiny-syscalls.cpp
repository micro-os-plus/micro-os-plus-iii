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

#include <cstdint>
#include <unistd.h>
#include <sys/stat.h>

extern "C"
{
  void __attribute__((weak))
  _start (void);

  void *
  _sbrk (int incr);

  void
  _exit (int code);

  int
  _kill (pid_t pid, int sig);

  pid_t
  _getpid (void);

  ssize_t
  _write (int fildes, const void *buf, size_t nbyte);

  int
  _close (int fildes);

  int
  _fstat (int fildes, struct stat *buf);

  int
  _isatty (int fildes);

  off_t
  _lseek (int fildes, off_t offset, int whence);

  ssize_t
  _read (int fildes, void *buf, size_t nbyte);

  extern int main (int, const char **);

}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

void
_exit (int code)
{
  for (;;)
    ;
}

void *
_sbrk (int incr)
{
  return nullptr;
}

int
_kill (pid_t pid, int sig)
{
  return 0;
}

pid_t
_getpid (void)
{
  return 0;
}

ssize_t
_write (int fildes, const void *buf, size_t nbyte)
{
  return 0;
}

int
_close (int fildes)
{
  return 0;
}

int
_fstat (int fildes, struct stat *buf)
{
  return 0;
}

int
_isatty (int fildes)
{
  return 0;
}

off_t
_lseek (int fildes, off_t offset, int whence)
{
  return 0;
}

ssize_t
_read (int fildes, void *buf, size_t nbyte)
{
  return 0;
}

void
_start (void)
{
  main (1, nullptr);
}

#pragma GCC diagnostic pop
