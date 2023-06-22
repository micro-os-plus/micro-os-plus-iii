/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
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
