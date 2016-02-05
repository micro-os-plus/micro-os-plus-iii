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

#ifndef POSIX_SEMAPHORE_H_
#define POSIX_SEMAPHORE_H_

#if !defined(__ARM_EABI__)
#include <semaphore.h>
#else

#ifdef __cplusplus
extern "C"
{
#endif

// ----------------------------------------------------------------------------
// Standard definitions from:
// http://pubs.opengroup.org/onlinepubs/009695399/basedefs/semaphore.h.html
// ----------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Types
  using sem_t = struct
    {};

  sem_t *
  sem_open (const char* name, int oflag, ...);
  int
  sem_close (sem_t* sem);
  int
  sem_unlink (const char* name);

  int
  sem_init (sem_t* sem, int pshared, unsigned value);
  int
  sem_destroy (sem_t* sem);

  int
  sem_getvalue (sem_t* sem, int* sval);

  int
  sem_post (sem_t* sem);
  int
  sem_wait (sem_t* sem);
  int
  sem_trywait (sem_t* sem);
  int
  sem_timedwait (sem_t* sem, const struct timespec* abstime);

// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* __ARM_EABI__ */

#endif /* POSIX_SEMAPHORE_H_ */
