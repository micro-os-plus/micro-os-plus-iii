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

#ifndef POSIX_PTHREAD_H_
#define POSIX_PTHREAD_H_

#if !defined(__ARM_EABI__)
#include <pthread.h>
#else

#include <sys/types.h>
#include <sched.h>

#ifdef __cplusplus
extern "C"
{
#endif

// ----------------------------------------------------------------------------
// Standard definitions from:
// http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html
// ----------------------------------------------------------------------------

// Thread attributes
#define PTHREAD_CREATE_JOINABLE 1
#define PTHREAD_CREATE_DETACHED 2

#define PTHREAD_INHERIT_SCHED 1
#define PTHREAD_EXPLICIT_SCHED  2

#define PTHREAD_CANCEL_ASYNCHRONOUS 0x00  /* Cancel occurs immediately */
#define PTHREAD_CANCEL_ENABLE 0x01 /* Cancel takes place at next cancellation point */
#define PTHREAD_CANCEL_DEFERRED 0x02 /* Cancel waits until cancellation point */
#define PTHREAD_CANCEL_DISABLE 0x00 /* Cancel postponed */

// Value returned from pthread_join() when a thread is cancelled
#define PTHREAD_CANCELED ((void *) 1)

#define PTHREAD_SCOPE_SYSTEM  1
#define PTHREAD_SCOPE_PROCESS 2

#define PTHREAD_PROCESS_SHARED  1
#define PTHREAD_PROCESS_PRIVATE 2

// Mutex attributes
#define PTHREAD_MUTEX_NORMAL 0
#define PTHREAD_MUTEX_ERRORCHECK 1
#define PTHREAD_MUTEX_RECURSIVE 2

#define PTHREAD_MUTEX_ROBUST 1
#define PTHREAD_MUTEX_STALLED 0

#define PTHREAD_MUTEX_DEFAULT PTHREAD_MUTEX_NORMAL

#define PTHREAD_PRIO_INHERIT  1
#define PTHREAD_PRIO_NONE 0
#define PTHREAD_PRIO_PROTECT  2

#define PTHREAD_ONCE_INIT  {_PTHREAD_ONCE_SIG_init, {0}}
#define PTHREAD_COND_INITIALIZER {_PTHREAD_COND_SIG_init, {0}}
#define PTHREAD_RWLOCK_INITIALIZER {_PTHREAD_RWLOCK_SIG_init, {0}}
#define PTHREAD_MUTEX_INITIALIZER {_PTHREAD_MUTEX_SIG_init, {0}}

#define PTHREAD_BARRIER_SERIAL_THREAD

  // --------------------------------------------------------------------------
  // Types
  using pthread_attr_t = struct
    {};
  using pthread_t = void*;
  using pthread_cond_t = struct
    {};
  using pthread_condattr_t = struct
    {};
  using pthread_mutex_t = struct
    {};
  using pthread_key_t = struct
    {};
  using pthread_mutexattr_t = struct
    {};
  using pthread_once_t = struct
    {};
  using pthread_rwlock_t = struct
    {};
  using pthread_rwlockattr_t = struct
    {};

  using pthread_barrier_t = struct
    {};
  using pthread_barrierattr_t = struct
    {};
  using pthread_spinlock_t = struct
    {};

  // ==========================================================================
  // Pthread attributes

  int
  pthread_attr_init (pthread_attr_t* attr);
  int
  pthread_attr_destroy (pthread_attr_t* attr);

  int
  pthread_attr_getdetachstate (const pthread_attr_t* attr, int* detachstate);
  int
  pthread_attr_setdetachstate (pthread_attr_t* attr, int detachstate);
  int
  pthread_attr_getguardsize (const pthread_attr_t* attr, size_t* guardsize);
  int
  pthread_attr_setguardsize (pthread_attr_t* attr, size_t guardsize);
  int
  pthread_attr_getinheritsched (const pthread_attr_t* attr,
                                int* inheritedsched);
  int
  pthread_attr_setinheritsched (pthread_attr_t* attr, int inheritedsched);
  int
  pthread_attr_getschedparam (const pthread_attr_t* attr,
                              struct sched_param* param);
  int
  pthread_attr_setschedparam (pthread_attr_t* attr,
                              const struct sched_param* param);
  int
  pthread_attr_getschedpolicy (const pthread_attr_t* attr, int* policy);
  int
  pthread_attr_setschedpolicy (pthread_attr_t* attr, int policy);
  int
  pthread_attr_getscope (const pthread_attr_t* attr, int* contentionscope);
  int
  pthread_attr_setscope (pthread_attr_t* attr, int contentionscope);
  int
  pthread_attr_getstackaddr (const pthread_attr_t* attr, void** stackaddr);
  int
  pthread_attr_setstackaddr (pthread_attr_t* attr, void* stackaddr);
  int
  pthread_attr_getstacksize (const pthread_attr_t* attr, size_t* stacksize);
  int
  pthread_attr_setstacksize (pthread_attr_t* attr, size_t stacksize);

  // Ptread
  int
  pthread_create (pthread_t* tid, const pthread_attr_t* attr, void *
  (*start) (void *),
                  void* arg);
  void
  pthread_exit (void* value_ptr);

  int
  pthread_cancel (pthread_t thread);
  int
  pthread_detach (pthread_t thread);
  int
  pthread_equal (pthread_t t1, pthread_t t2);
  int
  pthread_join (pthread_t thread, void** value_ptr);

  int
  pthread_setschedprio (pthread_t thread, int prio);

  int
  pthread_getschedparam (pthread_t, int*, struct sched_param*);
  int
  pthread_setschedparam (pthread_t, int, const struct sched_param*);

  // Keys
  int
  pthread_key_create (pthread_key_t* key, void
  (*destructor) (void *));

  int
  pthread_key_delete (pthread_key_t key);

  void*
  pthread_getspecific (pthread_key_t key);

  int
  pthread_setspecific (pthread_key_t key, const void* value);

  // Once
  int
  pthread_once (pthread_once_t*, void
  (*) (void));

  // Misc
  pthread_t
  pthread_self (void);

  int
  pthread_getconcurrency (void);

  int
  pthread_setconcurrency (int level);

  void
  pthread_cleanup_push (void
  (*routine) (void*),
                        void *arg);

  void
  pthread_cleanup_pop (int execute);

  int
  pthread_setcancelstate (int state, int* oldstate);

  int
  pthread_setcanceltype (int type, int* oldtype);

  void
  pthread_testcancel (void);

  int
  pthread_getcpuclockid (pthread_t thread, clockid_t*);

  // ==========================================================================
  // Mutex attributes
  int
  pthread_mutexattr_init (pthread_mutexattr_t* attr);
  int
  pthread_mutexattr_destroy (pthread_mutexattr_t* attr);

  int
  pthread_mutexattr_getprioceiling (const pthread_mutexattr_t* attr,
                                    int* prioceiling);
  int
  pthread_mutexattr_setprioceiling (pthread_mutexattr_t* attr, int prioceiling);
  int
  pthread_mutexattr_getprotocol (const pthread_mutexattr_t* attr,
                                 int* protocol);
  int
  pthread_mutexattr_setprotocol (pthread_mutexattr_t* attr, int protocol);
  int
  pthread_mutexattr_getpshared (const pthread_mutexattr_t* attr, int* pshared);
  int
  pthread_mutexattr_setpshared (pthread_mutexattr_t* attr, int pshared);
  int
  pthread_mutexattr_getrobust (const pthread_mutexattr_t*restrict,
                               int *restrict);
  int
  pthread_mutexattr_setrobust (pthread_mutexattr_t*, int);
  int
  pthread_mutexattr_gettype (const pthread_mutexattr_t* attr, int*);
  int
  pthread_mutexattr_settype (pthread_mutexattr_t* attr, int);

  // Mutex
  int
  pthread_mutex_init (pthread_mutex_t* mutex, const pthread_mutexattr_t* attr);
  int
  pthread_mutex_destroy (pthread_mutex_t* mutex);

  int
  pthread_mutex_lock (pthread_mutex_t* mutex);
  int
  pthread_mutex_trylock (pthread_mutex_t* mutex);
  int
  pthread_mutex_timedlock (pthread_mutex_t* mutex,
                           const struct timespec* abstime);
  int
  pthread_mutex_unlock (pthread_mutex_t* mutex);

  int
  pthread_mutex_getprioceiling (const pthread_mutex_t* mutex, int* prioceiling);
  int
  pthread_mutex_setprioceiling (pthread_mutex_t* mutex, int prioceiling,
                                int* old_prioceiling);

  // ==========================================================================
  // Condition variable attributes
  int
  pthread_condattr_init (pthread_condattr_t* attr);
  int
  pthread_condattr_destroy (pthread_condattr_t* attr);

  int
  pthread_condattr_getclock (const pthread_condattr_t* attr, clockid_t* clock);
  int
  pthread_condattr_setclock (pthread_condattr_t* attr, clockid_t clock);

  int
  pthread_condattr_getpshared (const pthread_condattr_t* attr, int* pshared);
  int
  pthread_condattr_setpshared (pthread_condattr_t* attr, int pshared);

  // Condition variable
  int
  pthread_cond_init (pthread_cond_t* cond, const pthread_condattr_t* attr);
  int
  pthread_cond_destroy (pthread_cond_t* cond);

  int
  pthread_cond_broadcast (pthread_cond_t* cond);
  int
  pthread_cond_signal (pthread_cond_t* cond);
  int
  pthread_cond_timedwait (pthread_cond_t* cond, pthread_mutex_t* mutex,
                          const struct timespec* abstime);
  int
  pthread_cond_wait (pthread_cond_t* cond, pthread_mutex_t* mutex);

#if NOT_IMPLEMENTED

// ==========================================================================
// RW lock attributes
int
pthread_rwlockattr_init (pthread_rwlockattr_t* attr);
int
pthread_rwlockattr_destroy (pthread_rwlockattr_t* attr);

int
pthread_rwlockattr_getpshared (const pthread_rwlockattr_t* attr,
    int* pshared);
int
pthread_rwlockattr_setpshared (pthread_rwlockattr_t* attr, int pshared);

// RW lock
int
pthread_rwlock_init (pthread_rwlock_t* rwlock,
    const pthread_rwlockattr_t* attr);
int
pthread_rwlock_destroy (pthread_rwlock_t* rwlock);

int
pthread_rwlock_rdlock (pthread_rwlock_t* rwlock);
int
pthread_rwlock_wrlock (pthread_rwlock_t* rwlock);
int
pthread_rwlock_tryrdlock (pthread_rwlock_t* rwlock);
int
pthread_rwlock_trywrlock (pthread_rwlock_t* rwlock);
int
pthread_rwlock_timedrdlock (pthread_rwlock_t*restrict,
    const struct timespec *restrict);
int
pthread_rwlock_timedwrlock (pthread_rwlock_t*restrict,
    const struct timespec *restrict);
int
pthread_rwlock_unlock (pthread_rwlock_t* rwlock);

// ==========================================================================
// Barrier attributes

int
pthread_barrierattr_init (pthread_barrierattr_t* attr);
int
pthread_barrierattr_destroy (pthread_barrierattr_t* attr);

int
pthread_barrierattr_getpshared (const pthread_barrierattr_t* attr,
    int* pshared);
int
pthread_barrierattr_setpshared (pthread_barrierattr_t* attr, int pshared);

// ==========================================================================
// Barrier

int
pthread_barrier_init (pthread_barrier_t* barrier,
    const pthread_barrierattr_t*, unsigned);
int
pthread_barrier_destroy (pthread_barrier_t* barrier);
int
pthread_barrier_wait (pthread_barrier_t* barrier);

// ==========================================================================
// Spin lock

int
pthread_spin_init (pthread_spinlock_t* spin, int);
int
pthread_spin_destroy (pthread_spinlock_t* spin);
int
pthread_spin_lock (pthread_spinlock_t* spin);
int
pthread_spin_trylock (pthread_spinlock_t* spin);
int
pthread_spin_unlock (pthread_spinlock_t* spin);

// ==========================================================================

int pthread_atfork(void (*)(void), void (*)(void),
    void(*)(void));

#endif

// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* __ARM_EABI__ */

#endif /* POSIX_PTHREAD_H_ */
