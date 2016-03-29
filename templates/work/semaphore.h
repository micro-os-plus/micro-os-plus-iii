/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
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
