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

#ifndef POSIX_IO_SYS_UIO_H_
#define POSIX_IO_SYS_UIO_H_

#if !defined(__ARM_EABI__)
#include <sys/uio.h>
#else

#include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

  struct iovec
  {
    void* iov_base;  // Base address of a memory region for input or output.
    size_t iov_len;   // The size of the memory pointed to by iov_base.
  };

  ssize_t
  writev (int fildes, const struct iovec* iov, int iovcnt);

#ifdef __cplusplus
}
#endif

#endif /* __ARM_EABI__ */

#endif /* POSIX_IO_SYS_UIO_H_ */
