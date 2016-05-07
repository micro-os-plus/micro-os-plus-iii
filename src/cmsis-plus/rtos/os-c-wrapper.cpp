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

/*
 * This file is part of the CMSIS++ proposal, intended as a CMSIS
 * replacement for C++ applications.
 *
 * The code provides an implementation of the C API for the CMSIS++
 * and legacy CMSIS, using the CMSIC++ RTOS definitions.
 */

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/rtos/os-c-api.h>
#include <cmsis-plus/diag/trace.h>

// Note: The legacy header is included later, to avoid polluting the namespace.

#include <cassert>
#include <cerrno>
#include <cstring>
#include <new>

// #include <cstdio>

// ----------------------------------------------------------------------------

using namespace os::rtos;

// ----------------------------------------------------------------------------

// Validate C structs sizes (should match the C++ objects sizes).
// TODO: validate individual members (size & offset).

static_assert(sizeof(Thread) == sizeof(os_thread_t), "adjust os_thread_t size");
static_assert(sizeof(thread::Attributes) == sizeof(os_thread_attr_t), "adjust os_thread_attr_t size");

static_assert(sizeof(Timer) == sizeof(os_timer_t), "adjust size of os_timer_t");
static_assert(sizeof(timer::Attributes) == sizeof(os_timer_attr_t), "adjust size of os_timer_attr_t");

static_assert(sizeof(Mutex) == sizeof(os_mutex_t), "adjust size of os_mutex_t");
static_assert(sizeof(mutex::Attributes) == sizeof(os_mutex_attr_t), "adjust size of os_mutex_attr_t");

static_assert(sizeof(Condition_variable) == sizeof(os_condvar_t), "adjust size of os_condvar_t");
static_assert(sizeof(condvar::Attributes) == sizeof(os_condvar_attr_t), "adjust size of os_condvar_attr_t");

static_assert(sizeof(Semaphore) == sizeof(os_semaphore_t), "adjust size of os_semaphore_t");
static_assert(sizeof(semaphore::Attributes) == sizeof(os_semaphore_attr_t), "adjust size of os_semaphore_attr_t");

static_assert(sizeof(Memory_pool) == sizeof(os_mempool_t), "adjust size of os_mempool_t");
static_assert(sizeof(mempool::Attributes) == sizeof(os_mempool_attr_t), "adjust size of os_mempool_attr_t");

static_assert(sizeof(Message_queue) == sizeof(os_mqueue_t), "adjust size of os_mqueue_t");
static_assert(sizeof(mqueue::Attributes) == sizeof(os_mqueue_attr_t), "adjust size of os_mqueue_attr_t");

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wenum-compare"

static_assert(os_priority_idle == thread::priority::idle, "adjust os_priority_idle");
static_assert(os_priority_low == thread::priority::low, "adjust os_priority_low");
static_assert(os_priority_below_normal == thread::priority::below_normal, "adjust os_priority_below_normal");
static_assert(os_priority_normal == thread::priority::normal, "adjust os_priority_normal");
static_assert(os_priority_above_normal == thread::priority::above_normal, "adjust os_priority_above_normal");
static_assert(os_priority_high == thread::priority::high, "adjust os_priority_high");
static_assert(os_priority_realtime == thread::priority::realtime, "adjust os_priority_realtime");
static_assert(os_priority_error == thread::priority::error, "adjust os_priority_error");

#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"

// ----------------------------------------------------------------------------

os_result_t
os_sched_initialize (void)
{
  return (os_result_t) scheduler::initialize ();
}

void
os_sched_start (void)
{
  scheduler::start ();
}

bool
os_sched_is_started (void)
{
  return scheduler::started ();
}

os_sched_status_t
os_sched_lock (os_sched_status_t status)
{
  return scheduler::lock (status);
}

void
os_sched_unlock (os_sched_status_t status)
{
  scheduler::unlock (status);
}

bool
os_sched_is_locked (void)
{
  return scheduler::locked ();
}

// ----------------------------------------------------------------------------

os_irq_status_t
os_irq_critical_enter (void)
{
  return interrupts::Critical_section::enter ();
}

void
os_irq_critical_exit (os_irq_status_t status)
{
  interrupts::Critical_section::exit (status);
}

// ----------------------------------------------------------------------------

os_irq_status_t
os_irq_uncritical_enter (void)
{
  return interrupts::Uncritical_section::enter ();
}

void
os_irq_uncritical_exit (os_irq_status_t status)
{
  interrupts::Uncritical_section::exit (status);
}

// ----------------------------------------------------------------------------

void
os_this_thread_wait (void)
{
  return this_thread::wait ();
}

void
os_this_thread_exit (void* exit_ptr)
{
  this_thread::exit (exit_ptr);
}

os_result_t
os_this_thread_sig_wait (os_thread_sigset_t mask, os_thread_sigset_t* oflags,
                         os_flags_mode_t mode)
{
  return (os_result_t) this_thread::sig_wait (mask, oflags, mode);
}

os_result_t
os_this_thread_try_sig_wait (os_thread_sigset_t mask,
                             os_thread_sigset_t* oflags, os_flags_mode_t mode)
{
  return (os_result_t) this_thread::try_sig_wait (mask, oflags, mode);
}

os_result_t
os_this_thread_timed_sig_wait (os_thread_sigset_t mask,
                               os_clock_duration_t timeout,
                               os_thread_sigset_t* oflags, os_flags_mode_t mode)
{
  return (os_result_t) this_thread::timed_sig_wait (mask, timeout, oflags, mode);
}

// ----------------------------------------------------------------------------

void
os_thread_attr_init (os_thread_attr_t* attr, const char* name)
{
  new (attr) thread::Attributes (name);
}

void
os_thread_create (os_thread_t* thread, const os_thread_attr_t* attr,
                  os_thread_func_t func, const os_thread_func_args_t args)
{
  new (thread) Thread ((thread::Attributes&) *attr, (thread::func_t) func,
                       (thread::func_args_t) args);
}

void
os_thread_destroy (os_thread_t* thread)
{
  (reinterpret_cast<Thread&> (*thread)).~Thread ();
}

os_result_t
os_thread_join (os_thread_t* thread, void** exit_ptr)
{
  return (os_result_t) (reinterpret_cast<Thread&> (*thread)).join (exit_ptr);
}

os_thread_prio_t
os_thread_get_prio (os_thread_t* thread)
{
  return (os_thread_prio_t) (reinterpret_cast<Thread&> (*thread)).sched_prio ();
}

os_result_t
os_thread_set_prio (os_thread_t* thread, os_thread_prio_t prio)
{
  return (os_result_t) (reinterpret_cast<Thread&> (*thread)).sched_prio (prio);
}

void
os_thread_resume (os_thread_t* thread)
{
  return (reinterpret_cast<Thread&> (*thread)).resume ();
}

os_thread_user_storage_t*
os_thread_get_user_storage (os_thread_t* thread)
{
  return (reinterpret_cast<Thread&> (*thread)).user_storage ();
}

os_result_t
os_thread_sig_raise (os_thread_t* thread, os_thread_sigset_t mask,
                     os_thread_sigset_t* oflags)
{
  return (os_result_t) (reinterpret_cast<Thread&> (*thread)).sig_raise (mask,
                                                                        oflags);
}

os_result_t
os_thread_sig_clear (os_thread_t* thread, os_thread_sigset_t mask,
                     os_thread_sigset_t* oflags)
{
  return (os_result_t) (reinterpret_cast<Thread&> (*thread)).sig_clear (mask,
                                                                        oflags);
}

os_thread_sigset_t
os_thread_sig_get (os_thread_t* thread, os_thread_sigset_t mask,
                   os_flags_mode_t mode)
{
  return (os_thread_sigset_t) (reinterpret_cast<Thread&> (*thread)).sig_get (
      mask, mode);
}

// ----------------------------------------------------------------------------

os_clock_timestamp_t
os_systick_clock_now (void)
{
  return (os_clock_timestamp_t) systick_clock.now ();
}

os_clock_timestamp_t
os_systick_clock_now_details (os_systick_clock_current_t* details)
{
  return (os_clock_timestamp_t) systick_clock.now (
      (Systick_clock::current_t*) details);
}

os_result_t
os_systick_clock_sleep_for (os_clock_duration_t timeout)
{
  return (os_result_t) systick_clock.sleep_for (timeout);
}

os_result_t
os_systick_clock_wait (os_clock_duration_t timeout)
{
  return (os_result_t) systick_clock.wait_for (timeout);
}

// os_systick_sleep_rep_t

os_clock_timestamp_t
os_realtime_clock_now (void)
{
  return (os_clock_timestamp_t) realtime_clock.now ();
}

os_result_t
os_realtime_clock_sleep_for (os_clock_duration_t secs)
{
  return (os_result_t) realtime_clock.sleep_for (secs);
}

// ----------------------------------------------------------------------------

void
os_timer_attr_init (os_timer_attr_t* attr, const char* name)
{
  new (attr) timer::Attributes (name);
}

void
os_timer_create (os_timer_t* timer, const os_timer_attr_t* attr,
                 os_timer_func_t func, os_timer_func_args_t args)
{
  new (timer) Timer ((timer::Attributes&) *attr, (timer::func_t) func,
                     (timer::func_args_t) args);
}

void
os_timer_destroy (os_timer_t* timer)
{
  (reinterpret_cast<Timer&> (*timer)).~Timer ();
}

os_result_t
os_timer_start (os_timer_t* timer, os_clock_duration_t timeout)
{
  return (os_result_t) (reinterpret_cast<Timer&> (*timer)).start (timeout);
}

os_result_t
os_timer_stop (os_timer_t* timer)
{
  return (os_result_t) (reinterpret_cast<Timer&> (*timer)).stop ();
}

// ----------------------------------------------------------------------------

void
os_mutex_attr_init (os_mutex_attr_t* attr, const char* name)
{
  new (attr) mutex::Attributes (name);
}

void
os_mutex_create (os_mutex_t* mutex, const os_mutex_attr_t* attr)
{
  new (mutex) Mutex ((mutex::Attributes&) *attr);
}

void
os_mutex_destroy (os_mutex_t* mutex)
{
  (reinterpret_cast<Mutex&> (*mutex)).~Mutex ();
}

os_result_t
os_mutex_lock (os_mutex_t* mutex)
{
  return (os_result_t) (reinterpret_cast<Mutex&> (*mutex)).lock ();
}

os_result_t
os_mutex_try_lock (os_mutex_t* mutex)
{
  return (os_result_t) (reinterpret_cast<Mutex&> (*mutex)).try_lock ();
}

os_result_t
os_mutex_timed_lock (os_mutex_t* mutex, os_clock_duration_t timeout)
{
  return (os_result_t) (reinterpret_cast<Mutex&> (*mutex)).timed_lock (timeout);
}

os_result_t
os_mutex_unlock (os_mutex_t* mutex)
{
  return (os_result_t) (reinterpret_cast<Mutex&> (*mutex)).unlock ();
}

os_thread_prio_t
os_mutex_get_prio_ceiling (os_mutex_t* mutex)
{
  return (os_thread_prio_t) (reinterpret_cast<Mutex&> (*mutex)).prio_ceiling ();
}

os_result_t
os_mutex_set_prio_ceiling (os_mutex_t* mutex, os_thread_prio_t prio_ceiling,
                           os_thread_prio_t* old_prio_ceiling)
{
  return (os_result_t) (reinterpret_cast<Mutex&> (*mutex)).prio_ceiling (
      prio_ceiling, old_prio_ceiling);
}

os_result_t
os_mutex_mark_consistent (os_mutex_t* mutex)
{
  return (os_result_t) (reinterpret_cast<Mutex&> (*mutex)).consistent ();
}

os_thread_t*
os_mutex_get_owner (os_mutex_t* mutex)
{
  return (os_thread_t*) (reinterpret_cast<Mutex&> (*mutex)).owner ();
}

os_result_t
os_mutex_reset (os_mutex_t* mutex)
{
  return (os_result_t) (reinterpret_cast<Mutex&> (*mutex)).reset ();
}

// ----------------------------------------------------------------------------

void
os_condvar_attr_init (os_condvar_attr_t* attr, const char* name)
{
  new (attr) condvar::Attributes (name);
}

void
os_condvar_create (os_condvar_t* condvar, os_condvar_attr_t* attr)
{
  new (condvar) Condition_variable ((condvar::Attributes&) *attr);
}

void
os_condvar_destroy (os_condvar_t* condvar)
{
  (reinterpret_cast<Condition_variable&> (*condvar)).~Condition_variable ();
}

os_result_t
os_condvar_signal (os_condvar_t* condvar)
{
  return (os_result_t) (reinterpret_cast<Condition_variable&> (*condvar)).signal ();
}

os_result_t
os_condvar_broadcast (os_condvar_t* condvar)
{
  return (os_result_t) (reinterpret_cast<Condition_variable&> (*condvar)).broadcast ();
}

os_result_t
os_condvar_wait (os_condvar_t* condvar, os_mutex_t* mutex)
{
  return (os_result_t) (reinterpret_cast<Condition_variable&> (*condvar)).wait (
      (Mutex&) *mutex);
}

os_result_t
os_condvar_timed_wait (os_condvar_t* condvar, os_mutex_t* mutex,
                       os_clock_duration_t timeout)
{
  return (os_result_t) (reinterpret_cast<Condition_variable&> (*condvar)).timed_wait (
      (Mutex&) *mutex, timeout);
}

// ----------------------------------------------------------------------------

void
os_semaphore_attr_init (os_semaphore_attr_t* attr, const char* name)
{
  new (attr) semaphore::Attributes (name);
}

void
os_semaphore_create (os_semaphore_t* semaphore, os_semaphore_attr_t* attr)
{
  new (semaphore) Semaphore ((semaphore::Attributes&) *attr);
}

void
os_semaphore_destroy (os_semaphore_t* semaphore)
{
  (reinterpret_cast<Semaphore&> (*semaphore)).~Semaphore ();
}

os_result_t
os_semaphore_post (os_semaphore_t* semaphore)
{
  return (os_result_t) (reinterpret_cast<Semaphore&> (*semaphore)).post ();
}

os_result_t
os_semaphore_wait (os_semaphore_t* semaphore)
{
  return (os_result_t) (reinterpret_cast<Semaphore&> (*semaphore)).wait ();
}

os_result_t
os_semaphore_try_wait (os_semaphore_t* semaphore)
{
  return (os_result_t) (reinterpret_cast<Semaphore&> (*semaphore)).try_wait ();
}

os_result_t
os_semaphore_timed_wait (os_semaphore_t* semaphore, os_clock_duration_t timeout)
{
  return (os_result_t) (reinterpret_cast<Semaphore&> (*semaphore)).timed_wait (
      timeout);
}

os_semaphore_count_t
os_semaphore_get_value (os_semaphore_t* semaphore)
{
  return (os_semaphore_count_t) (reinterpret_cast<Semaphore&> (*semaphore)).value ();
}

os_result_t
os_semaphore_reset (os_semaphore_t* semaphore)
{
  return (os_result_t) (reinterpret_cast<Semaphore&> (*semaphore)).reset ();
}

os_semaphore_count_t
os_semaphore_get_initial_value (os_semaphore_t* semaphore)
{
  return (os_semaphore_count_t) (reinterpret_cast<Semaphore&> (*semaphore)).initial_value ();
}

os_semaphore_count_t
os_semaphore_get_max_value (os_semaphore_t* semaphore)
{
  return (os_semaphore_count_t) (reinterpret_cast<Semaphore&> (*semaphore)).max_value ();
}

// ----------------------------------------------------------------------------

void
os_mempool_attr_init (os_mempool_attr_t* attr, const char* name)
{
  new (attr) mempool::Attributes (name);
}

void
os_mempool_create (os_mempool_t* mempool, os_mempool_attr_t* attr,
                   os_mempool_size_t blocks, os_mempool_size_t block_size_bytes)
{
  new (mempool) Memory_pool ((mempool::Attributes&) *attr, blocks,
                             block_size_bytes);
}

void
os_mempool_destroy (os_mempool_t* mempool)
{
  (reinterpret_cast<Memory_pool&> (*mempool)).~Memory_pool ();
}

void*
os_mempool_alloc (os_mempool_t* mempool)
{
  return (reinterpret_cast<Memory_pool&> (*mempool)).alloc ();
}

void*
os_mempool_try_alloc (os_mempool_t* mempool)
{
  return (reinterpret_cast<Memory_pool&> (*mempool)).try_alloc ();
}

void*
os_mempool_timed_alloc (os_mempool_t* mempool, os_clock_duration_t timeout)
{
  return (reinterpret_cast<Memory_pool&> (*mempool)).timed_alloc (timeout);
}

os_result_t
os_mempool_free (os_mempool_t* mempool, void* block)
{
  return (os_result_t) (reinterpret_cast<Memory_pool&> (*mempool)).free (block);
}

size_t
os_mempool_get_capacity (os_mempool_t* mempool)
{
  return (reinterpret_cast<Memory_pool&> (*mempool)).capacity ();
}

size_t
os_mempool_get_count (os_mempool_t* mempool)
{
  return (reinterpret_cast<Memory_pool&> (*mempool)).count ();
}

size_t
os_mempool_get_block_size (os_mempool_t* mempool)
{
  return (reinterpret_cast<Memory_pool&> (*mempool)).block_size ();
}

bool
os_mempool_is_empty (os_mempool_t* mempool)
{
  return (reinterpret_cast<Memory_pool&> (*mempool)).empty ();
}

bool
os_mempool_is_full (os_mempool_t* mempool)
{
  return (reinterpret_cast<Memory_pool&> (*mempool)).full ();
}

os_result_t
os_mempool_reset (os_mempool_t* mempool)
{
  return (os_result_t) (reinterpret_cast<Memory_pool&> (*mempool)).reset ();
}

void*
os_mempool_get_pool (os_mempool_t* mempool)
{
  return (void*) (reinterpret_cast<Memory_pool&> (*mempool)).pool ();
}

// --------------------------------------------------------------------------

void
os_mqueue_attr_init (os_mqueue_attr_t* attr, const char* name)
{
  new (attr) mqueue::Attributes (name);
}

void
os_mqueue_create (os_mqueue_t* mqueue, os_mqueue_attr_t* attr,
                  os_mqueue_size_t msgs, os_mqueue_size_t msg_size_bytes)
{
  new (mqueue) Message_queue ((mqueue::Attributes&) *attr, msgs,
                              msg_size_bytes);
}

void
os_mqueue_destroy (os_mqueue_t* mqueue)
{
  (reinterpret_cast<Message_queue&> (*mqueue)).~Message_queue ();
}

os_result_t
os_mqueue_send (os_mqueue_t* mqueue, const char* msg, size_t nbytes,
                os_mqueue_prio_t mprio)
{
  return (os_result_t) (reinterpret_cast<Message_queue&> (*mqueue)).send (
      msg, nbytes, mprio);
}

os_result_t
os_mqueue_try_send (os_mqueue_t* mqueue, const char* msg, size_t nbytes,
                    os_mqueue_prio_t mprio)
{
  return (os_result_t) (reinterpret_cast<Message_queue&> (*mqueue)).try_send (
      msg, nbytes, mprio);
}

os_result_t
os_mqueue_timed_send (os_mqueue_t* mqueue, const char* msg, size_t nbytes,
                      os_mqueue_prio_t mprio, os_clock_duration_t timeout)
{
  return (os_result_t) (reinterpret_cast<Message_queue&> (*mqueue)).timed_send (
      msg, nbytes, mprio, timeout);
}

os_result_t
os_mqueue_receive (os_mqueue_t* mqueue, char* msg, size_t nbytes,
                   os_mqueue_prio_t* mprio)
{
  return (os_result_t) (reinterpret_cast<Message_queue&> (*mqueue)).receive (
      msg, nbytes, mprio);
}

os_result_t
os_mqueue_try_receive (os_mqueue_t* mqueue, char* msg, size_t nbytes,
                       os_mqueue_prio_t* mprio)
{
  return (os_result_t) (reinterpret_cast<Message_queue&> (*mqueue)).try_receive (
      msg, nbytes, mprio);
}

os_result_t
os_mqueue_timed_receive (os_mqueue_t* mqueue, char* msg, size_t nbytes,
                         os_mqueue_prio_t* mprio, os_clock_duration_t timeout)
{
  return (os_result_t) (reinterpret_cast<Message_queue&> (*mqueue)).timed_receive (
      msg, nbytes, mprio, timeout);
}

size_t
os_mqueue_get_length (os_mqueue_t* mqueue)
{
  return (reinterpret_cast<Message_queue&> (*mqueue)).length ();
}

size_t
os_mqueue_get_capacity (os_mqueue_t* mqueue)
{
  return (reinterpret_cast<Message_queue&> (*mqueue)).capacity ();
}

size_t
os_mqueue_get_msg_size (os_mqueue_t* mqueue)
{
  return (reinterpret_cast<Message_queue&> (*mqueue)).msg_size ();
}

bool
os_mqueue_is_empty (os_mqueue_t* mqueue)
{
  return (reinterpret_cast<Message_queue&> (*mqueue)).empty ();
}

bool
os_mqueue_is_full (os_mqueue_t* mqueue)
{
  return (reinterpret_cast<Message_queue&> (*mqueue)).full ();
}

os_result_t
os_mqueue_reset (os_mqueue_t* mqueue)
{
  return (os_result_t) (reinterpret_cast<Message_queue&> (*mqueue)).reset ();
}

// --------------------------------------------------------------------------

void
os_evflags_attr_init (os_evflags_attr_t* attr, const char* name)
{
  new (attr) evflags::Attributes (name);
}

void
os_evflags_create (os_evflags_t* evflags, os_evflags_attr_t* attr)
{
  new (evflags) Event_flags ((evflags::Attributes&) *attr);
}

void
os_evflags_destroy (os_evflags_t* evflags)
{
  (reinterpret_cast<Event_flags&> (*evflags)).~Event_flags ();
}

os_result_t
os_evflags_wait (os_evflags_t* evflags, os_flags_mask_t mask,
                 os_flags_mask_t* oflags, os_flags_mode_t mode)
{
  return (os_result_t) (reinterpret_cast<Event_flags&> (*evflags)).wait (mask,
                                                                         oflags,
                                                                         mode);
}

os_result_t
os_evflags_try_wait (os_evflags_t* evflags, os_flags_mask_t mask,
                     os_flags_mask_t* oflags, os_flags_mode_t mode)
{
  return (os_result_t) (reinterpret_cast<Event_flags&> (*evflags)).try_wait (
      mask, oflags, mode);
}

os_result_t
os_evflags_timed_wait (os_evflags_t* evflags, os_flags_mask_t mask,
                       os_clock_duration_t timeout, os_flags_mask_t* oflags,
                       os_flags_mode_t mode)
{
  return (os_result_t) (reinterpret_cast<Event_flags&> (*evflags)).timed_wait (
      mask, timeout, oflags, mode);
}

os_result_t
os_evflags_raise (os_evflags_t* evflags, os_flags_mask_t mask,
                  os_flags_mask_t* oflags)
{
  return (os_result_t) (reinterpret_cast<Event_flags&> (*evflags)).raise (
      mask, oflags);
}

os_result_t
os_evflags_clear (os_evflags_t* evflags, os_flags_mask_t mask,
                  os_flags_mask_t* oflags)
{
  return (os_result_t) (reinterpret_cast<Event_flags&> (*evflags)).clear (
      mask, oflags);
}

os_flags_mask_t
os_evflags_get (os_evflags_t* evflags, os_flags_mask_t mask,
                os_flags_mode_t mode)
{
  return (os_flags_mask_t) (reinterpret_cast<Event_flags&> (*evflags)).get (
      mask, mode);
}

bool
os_evflags_get_waiting (os_evflags_t* evflags)
{
  return (reinterpret_cast<Event_flags&> (*evflags)).waiting ();
}

// ****************************************************************************
// ***** Legacy CMSIS RTOS implementation *****

#include <cmsis-plus/legacy/cmsis_os.h>

// ----------------------------------------------------------------------------
//  ==== Kernel Control Functions ====

/**
 * @details
 * Initialise of the RTOS Kernel to allow peripheral setup and creation
 * of other RTOS objects with the functions:
 * - osThreadCreate : Create a thread function.
 * - osTimerCreate : Define attributes of the timer callback function.
 * - osMutexCreate : Define and initialise a mutex.
 * - osSemaphoreCreate : Define and initialise a semaphore.
 * - osPoolCreate : Define and initialise a fix-size memory pool.
 * - osMessageCreate : Define and initialise a message queue.
 * - osMailCreate : Define and initialise a mail queue with fix-size memory blocks.
 *
 * The RTOS scheduler does not start thread switching until the function
 * osKernelStart is called.
 *
 * @note In case that the RTOS Kernel starts thread execution with the
 * function main the function osKernelInitialize stops thread
 * switching. This allows you to setup the system to a defined
 * state before thread switching is resumed with osKernelStart.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osKernelInitialize (void)
{
  if (scheduler::in_handler_mode ())
    {
      return osErrorISR;
    }

  scheduler::initialize ();
  return osOK;
}

/**
 * @details
 * Start the RTOS Kernel and begin thread switching.
 *
 * @note When the CMSIS-RTOS starts thread execution with the
 * function main this function resumes thread switching.
 * The main thread will continue executing after osKernelStart.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osKernelStart (void)
{
  scheduler::start ();
  // In CMSIS++ this does not return.
}

/**
 * @details
 * Identifies if the RTOS scheduler is started. For systems with the
 * option to start the main function as a thread this allows
 * you to identify that the RTOS scheduler is already running.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
int32_t
osKernelRunning (void)
{
  return scheduler::started () ? 1 : 0;
}

#if (defined (osFeature_SysTick)  &&  (osFeature_SysTick != 0))

/**
 * @details
 * Get the value of the Kernel SysTick timer for time comparison.
 * The value is a rolling 32-bit counter that is typically composed
 * of the kernel system interrupt timer value and an counter that
 * counts these interrupts.
 *
 * This function allows the implementation of timeout checks.
 * These are for example required when checking for a busy status
 * in a device or peripheral initialisation routine.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
uint32_t
osKernelSysTick (void)
{
  Systick_clock::current_t crt;

  // Get the current SysTick timestamp, with full details, down to
  // cpu cycles.
  systick_clock.now (&crt);

  // Convert ticks to cycles.
  return static_cast<uint32_t> (crt.ticks) * crt.divisor + crt.cycles;
}

#endif    // System Timer available

// ----------------------------------------------------------------------------
//  ==== Thread Management ====

/**
 * @details
 * Find a free slot in the array of thread definitions and initialise
 * the thread.
 *
 * Start the thread function by adding it to the Active Threads list
 * and set it to state READY. The thread function receives the argument
 * pointer as function argument when the function is started. When the
 * priority of the created thread function is higher than the current
 * RUNNING thread, the created thread function starts instantly and
 * becomes the new RUNNING thread.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osThreadId
osThreadCreate (const osThreadDef_t* thread_def, void* args)
{
  if (scheduler::in_handler_mode ())
    {
      return nullptr;
    }

  if (thread_def == nullptr)
    {
      return nullptr;
    }

  thread::Attributes attr
    { thread_def->name };
  attr.th_priority = thread_def->tpriority;
  attr.th_stack_size_bytes = thread_def->stacksize;

  // Creating thread with invalid priority should fail (validator requirement).
  if (thread_def->tpriority >= osPriorityError)
    {
      return nullptr;
    }

  // Find a free slot in the tread definitions array.
  for (uint32_t i = 0; i < thread_def->instances; ++i)
    {
      Thread* th = (Thread*) &thread_def->data[i];
      if (th->sched_state () == thread::state::undefined
          || th->sched_state () == thread::state::destroyed)
        {
          if (attr.th_stack_size_bytes > 0)
            {
              attr.th_stack_address = &thread_def->stack[(i)
                  * ((thread_def->stacksize + sizeof(uint64_t) - 1)
                      / sizeof(uint64_t))];
            }
          new (th) Thread (attr, (thread::func_t) thread_def->pthread, args);

          // No need to yield here, already done by constructor.
          return reinterpret_cast<osThreadId> (th);
        }
    }
  return nullptr;
}

/**
 * @details
 * Get the thread ID of the current running thread.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osThreadId
osThreadGetId (void)
{
  if (scheduler::in_handler_mode ())
    {
      return nullptr;
    }

  return reinterpret_cast<osThreadId> (&this_thread::thread ());
}

/**
 * @details
 * Remove the thread function from the active thread list. If the
 * thread is currently RUNNING the execution will stop.
 *
 * @note In case that osThreadTerminate terminates the currently
 * running task, the function never returns and other threads
 * that are in the READY state are started.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osThreadTerminate (osThreadId thread_id)
{
  if (scheduler::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (thread_id == nullptr)
    {
      return osErrorParameter;
    }

  thread::state_t state =
      (reinterpret_cast<Thread&> (*thread_id)).sched_state ();
  if (state == thread::state::undefined)
    {
      return osErrorResource;
    }

  if ((reinterpret_cast<Thread*> (thread_id)) == &this_thread::thread ())
    {
      this_thread::exit ();
      /* NOTREACHED */
    }
  else
    {
      (reinterpret_cast<Thread&> (*thread_id)).kill ();
    }

  return osOK;
}

/**
 * @details
 * Pass control to the next thread that is in state READY.
 * If there is no other thread in the state READY, the current
 * thread continues execution and no thread switching occurs.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osThreadYield (void)
{
  if (scheduler::in_handler_mode ())
    {
      return osErrorISR;
    }

  this_thread::yield ();

  return osOK;
}

/**
 * @details
 * Change the priority of a running thread.
 * There is no need for an extra yield after it, if the priorities
 * require, there is an implicit reschedule.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osThreadSetPriority (osThreadId thread_id, osPriority priority)
{
  if (scheduler::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (thread_id == nullptr)
    {
      return osErrorParameter;
    }

  thread::state_t state =
      (reinterpret_cast<Thread&> (*thread_id)).sched_state ();
  if (state == thread::state::undefined || state >= thread::state::destroyed)
    {
      return osErrorResource;
    }

  if (priority < osPriorityIdle || priority >= osPriorityError)
    {
      return osErrorValue;
    }

  // Call C++ mutator.
  thread::priority_t prio = static_cast<thread::priority_t> (priority);
  result_t res = ((reinterpret_cast<Thread&> (*thread_id)).sched_prio (prio));

  // A mandatory yield is needed here, must be done
  // by the implementation.

  if (res == result::ok)
    {
      return osOK;
    }
  else if (res == EINVAL)
    {
      return osErrorValue;
    }
  else
    {
      return osErrorOS;
    }
}

/**
 * @details
 * Get the priority of an active thread. In case of a failure the value
 * osPriorityError is returned.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osPriority
osThreadGetPriority (osThreadId thread_id)
{
  if (scheduler::in_handler_mode ())
    {
      return osPriorityError;
    }

  if (thread_id == nullptr)
    {
      return osPriorityError;
    }

  // Call C++ accessor.
  thread::priority_t prio =
      (reinterpret_cast<Thread&> (*thread_id)).sched_prio ();
  return static_cast<osPriority> (prio);
}

// ----------------------------------------------------------------------------
//  ==== Generic Wait Functions ====

/**
 * @details
 * Wait for a specified time period in _millisec_.
 *
 * The millisec value specifies the number of timer ticks and is therefore
 * an upper bound. The exact time delay depends on the actual time elapsed
 * since the last timer tick.
 *
 * For a value of 1, the system waits until the next timer tick occurs.
 * That means that the actual time delay may be up to one timer tick less.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osDelay (uint32_t millisec)
{
  if (scheduler::in_handler_mode ())
    {
      return osErrorISR;
    }

  result_t res = systick_clock.sleep_for (
      Systick_clock::ticks_cast (millisec * 1000u));

  if (res == ETIMEDOUT)
    {
      return osEventTimeout;
    }
  else
    {
      return osErrorOS;
    }
}

#if (defined (osFeature_Wait)  &&  (osFeature_Wait != 0))

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

/**
 * @details
 * Wait for any event of the type signal, message, mail for a specified
 * time period in millisec. While the system waits, the thread that is
 * calling this function is put into the state WAITING. When millisec
 * is set to osWaitForever, the function will wait for an infinite time
 * until an event occurs.
 *
 * The osWait function puts a thread into the state WAITING and waits
 * for any of the following events:
 *
 * - a signal sent to that thread explicitly
 * - a message from a message object that is registered to that thread
 * - a mail from a mail object that is registered to that thread
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osEvent
osWait (uint32_t millisec)
{
  osEvent event;

  if (scheduler::in_handler_mode ())
    {
      event.status = osErrorISR;
      return event;
    }

  result_t res = systick_clock.wait_for (
      Systick_clock::ticks_cast (millisec * 1000u));

  // TODO: return events
  if (res == ETIMEDOUT)
    {
      event.status = osEventTimeout;
    }
  else
    {
      event.status = osErrorOS;
    }

  return event;
}

#pragma GCC diagnostic pop

#endif  // Generic Wait available

// ----------------------------------------------------------------------------
//  ==== Timer Management Functions ====

/**
 * @details
 * Create a one-shot or periodic timer and associate it with a callback
 * function argument. The timer is initially stopped and must be started with
 * `osTimerStart()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osTimerId
osTimerCreate (const osTimerDef_t* timer_def, os_timer_type type, void* args)
{
  if (scheduler::in_handler_mode ())
    {
      return nullptr;
    }

  if (timer_def == nullptr)
    {
      return nullptr;
    }

  timer::Attributes attr
    { timer_def->name };
  attr.tm_type = (timer::type_t) type;

  return reinterpret_cast<osTimerId> (new ((void*) timer_def->data) Timer (
      attr, (timer::func_t) timer_def->ptimer, (timer::func_args_t) args));
}

/**
 * @details
 * Start or restart the timer.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osTimerStart (osTimerId timer_id, uint32_t millisec)
{
  if (scheduler::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (timer_id == nullptr)
    {
      return osErrorParameter;
    }

  result_t res = (reinterpret_cast<Timer&> (*timer_id)).start (
      Systick_clock::ticks_cast (millisec * 1000u));

  if (res == result::ok)
    {
      return osOK;
    }
  else
    {
      return osErrorOS;
    }
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osTimerStop (osTimerId timer_id)
{
  if (scheduler::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (timer_id == nullptr)
    {
      return osErrorParameter;
    }

  result_t res = (reinterpret_cast<Timer&> (*timer_id)).stop ();
  if (res == result::ok)
    {
      return osOK;
    }
  else if (res == EAGAIN)
    {
      return osErrorResource;
    }
  else
    {
      return osErrorOS;
    }
}

/**
 * @details
 * Delete the timer object that was created by @ref osTimerCreate.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osTimerDelete (osTimerId timer_id)
{
  if (scheduler::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (timer_id == nullptr)
    {
      return osErrorParameter;
    }

  (reinterpret_cast<Timer&> (*timer_id)).~Timer ();
  return osOK;
}

// ----------------------------------------------------------------------------
//  ==== Signal Management ====

/**
 * @details
 * Set the signal flags of an active thread.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
int32_t
osSignalSet (osThreadId thread_id, int32_t signals)
{
  if (thread_id == nullptr)
    {
      return (int32_t) 0x80000000;
    }

  if (signals == (int32_t) 0x80000000)
    {
      return (int32_t) 0x80000000;
    }

  thread::sigset_t osig;
  ((Thread*) (thread_id))->sig_raise ((thread::sigset_t) signals, &osig);
  return (int32_t) osig;
}

/**
 * @details
 * Clear the signal flags of an active thread.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
int32_t
osSignalClear (osThreadId thread_id, int32_t signals)
{
  if (thread_id == nullptr)
    {
      return (int32_t) 0x80000000;
    }

  if (scheduler::in_handler_mode () || (signals == 0))
    {
      return (int32_t) 0x80000000;
    }

  thread::sigset_t sig;
  ((Thread*) (thread_id))->sig_clear ((thread::sigset_t) signals, &sig);

  return (int32_t) sig;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

/**
 * @details
 * Suspend the execution of the current RUNNING thread until all
 * specified signal flags with the parameter signals are set.
 * When the parameter signals is 0 the current RUNNING thread
 * is suspended until any signal is set. When these signal flags are
 * already set, the function returns instantly. Otherwise the thread
 * is put into the state WAITING. Signal flags that are reported as
 * event are automatically cleared.
 *
 * The argument millisec specifies how long the system waits for
 * the specified signal flags. While the system waits the tread
 * calling this function is put into the state WAITING. The timeout
 * value can have the following values:
 *
 * - when millisec is 0, the function returns instantly.
 * - when millisec is set to osWaitForever the function will wait
 * an infinite time until a specified signal is set.
 * - all other values specify a time in millisecond for a timeout.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osEvent
osSignalWait (int32_t signals, uint32_t millisec)
{
  osEvent event;

  if (scheduler::in_handler_mode ())
    {
      event.status = osErrorISR;
      return event;
    }

  if ((uint32_t) signals & 0x80000000)
    {
      event.status = osErrorValue;
      return event;
    }

  result_t res;
  if (millisec == osWaitForever)
    {
      res = this_thread::sig_wait ((thread::sigset_t) signals,
                                   (thread::sigset_t*) &event.value.signals);
    }
  else if (millisec == 0)
    {
      res = this_thread::try_sig_wait (
          (thread::sigset_t) signals, (thread::sigset_t*) &event.value.signals);
    }
  else
    {
      res = this_thread::timed_sig_wait (
          (thread::sigset_t) signals,
          Systick_clock::ticks_cast (millisec * 1000u),
          (thread::sigset_t*) &event.value.signals);
    }

  if (res == result::ok)
    {
      event.status = osEventSignal;
    }
  else if (res == EWOULDBLOCK)
    {
      event.status = osOK; // Only for try_sig_wait().
    }
  else if (res == ETIMEDOUT)
    {
      event.status = osEventTimeout; // Only for timed_sig_wait().
    }
  else if (res == EPERM)
    {
      event.status = osErrorISR;
    }
  else if (res == EINVAL)
    {
      event.status = osErrorValue;
    }
  else
    {
      event.status = osErrorOS;
    }

  return event;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

//  ==== Mutex Management ====

/**
 * @details
 * Create and initialize a mutex object.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osMutexId
osMutexCreate (const osMutexDef_t* mutex_def)
{
  if (scheduler::in_handler_mode ())
    {
      return nullptr;
    }

  if (mutex_def == nullptr)
    {
      return nullptr;
    }

  mutex::Attributes attr
    { mutex_def->name };
  attr.mx_type = mutex::type::recursive;
  attr.mx_protocol = mutex::protocol::inherit;

  return reinterpret_cast<osMutexId> (new ((void*) mutex_def->data) Mutex (attr));
}

/**
 * @details
 * Wait until a Mutex becomes available. If no other thread has
 * obtained the Mutex, the function instantly returns and blocks
 * the mutex object.
 *
 * The argument millisec specifies how long the system waits for
 * a mutex. While the system waits the thread that is calling this
 * function is put into the state WAITING. The millisec timeout can
 * have the following values:
 *
 * - when millisec is 0, the function returns instantly.
 * - when millisec is set to osWaitForever the function will
 * wait for an infinite time until the mutex becomes available.
 * - all other values specify a time in millisecond for a timeout.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osMutexWait (osMutexId mutex_id, uint32_t millisec)
{
  if (scheduler::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (mutex_id == nullptr)
    {
      return osErrorParameter;
    }

  result_t ret;
  if (millisec == osWaitForever)
    {
      ret = (reinterpret_cast<Mutex&> (*mutex_id)).lock ();
      // osErrorResource:
    }
  else if (millisec == 0)
    {
      ret = (reinterpret_cast<Mutex&> (*mutex_id)).try_lock ();
    }
  else
    {
      ret = (reinterpret_cast<Mutex&> (*mutex_id)).timed_lock (
          Systick_clock::ticks_cast (millisec * 1000u));
      // osErrorTimeoutResource:
    }

  if (ret == result::ok)
    {
      // osOK: the mutex has been obtained.
      return osOK;
    }
  else if (ret == EWOULDBLOCK)
    {
      // The mutex could not be obtained when no timeout was specified.
      // Only for try_lock().
      return osErrorResource;
    }
  else if (ret == ENOTRECOVERABLE)
    {
      // The mutex could not be obtained when no timeout was specified.
      // Only for lock().
      return osErrorResource;
    }
  else if (ret == ETIMEDOUT)
    {
      // The mutex could not be obtained in the given time.
      // Only for timed_lock().
      return osErrorTimeoutResource;
    }
  else
    {
      return osErrorOS;
    }

  /* NOTREACHED */
}

/**
 * @details
 * Release a Mutex that was obtained with osMutexWait. Other
 * threads that currently wait for the same mutex will be now
 * put into the state READY.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osMutexRelease (osMutexId mutex_id)
{
  if (scheduler::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (mutex_id == nullptr)
    {
      return osErrorParameter;
    }

  result_t res;
  res = (reinterpret_cast<Mutex&> (*mutex_id)).unlock ();

  if (res == result::ok)
    {
      return osOK;
    }
  else if ((res == EPERM) || (res == ENOTRECOVERABLE))
    {
      return osErrorResource;
    }
  else
    {
      return osErrorOS;
    }
}

/**
 * @details
 * Delete a mutex object. The function releases internal memory
 * obtained for mutex handling. After this call the mutex_id is no
 * longer valid and cannot be used. The mutex may be created again
 * using the function osMutexCreate.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osMutexDelete (osMutexId mutex_id)
{
  if (scheduler::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (mutex_id == nullptr)
    {
      return osErrorParameter;
    }

  (reinterpret_cast<Mutex&> (*mutex_id)).~Mutex ();
  return osOK;
}

// ----------------------------------------------------------------------------

//  ==== Semaphore Management Functions ====

#if (defined (osFeature_Semaphore)  &&  (osFeature_Semaphore != 0))

/**
 * @details
 * Create and initialise a semaphore object that is used to manage
 * access to shared resources. The parameter count specifies
 * the number of available resources. The count value 1 creates
 * a binary semaphore.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osSemaphoreId
osSemaphoreCreate (const osSemaphoreDef_t* semaphore_def, int32_t count)
{
  if (scheduler::in_handler_mode ())
    {
      return nullptr;
    }

  if (semaphore_def == nullptr)
    {
      return nullptr;
    }

  semaphore::Attributes attr
    { semaphore_def->name };
  attr.sm_initial_count = (semaphore::count_t) count;
  // The logic is very strange, the CMSIS expects both the max-count to be the
  // same as count, and also to accept a count of 0, which leads to
  // useless semaphores. We patch this behaviour in the wrapper, the main
  // object uses a more realistic max_count.
  attr.sm_max_count = (semaphore::count_t) (
      count == 0 ? osFeature_Semaphore : count);

  return reinterpret_cast<osSemaphoreId> (new ((void*) semaphore_def->data) Semaphore (
      attr));
}

/**
 * @details
 * Wait until a Semaphore token becomes available. When no Semaphore
 * token is available, the function waits for the time specified with
 * the parameter millisec.
 *
 * The argument millisec specifies how long the system waits for a
 * Semaphore token to become available. While the system waits the
 * thread that is calling this function is put into the state WAITING.
 * The millisec timeout can have the following values:
 *
 * - when millisec is 0, the function returns instantly.
 * - when millisec is set to osWaitForever the function will wait
 * for an infinite time until the Semaphore token becomes available.
 * - all other values specify a time in millisecond for a timeout.
 *
 * The return value indicates the number of available tokens (the
 * semaphore count value). If 0 is returned, then no semaphore was
 * available.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
int32_t
osSemaphoreWait (osSemaphoreId semaphore_id, uint32_t millisec)
{
  if (scheduler::in_handler_mode ())
    {
      return -1;
    }

  if (semaphore_id == nullptr)
    {
      return -1;
    }

  result_t res;
  if (millisec == osWaitForever)
    {
      res = (reinterpret_cast<Semaphore&> (*semaphore_id)).wait ();
    }
  else if (millisec == 0)
    {
      res = (reinterpret_cast<Semaphore&> (*semaphore_id)).try_wait ();
      if (res == EWOULDBLOCK)
        {
          return 0;
        }
    }
  else
    {
      res = (reinterpret_cast<Semaphore&> (*semaphore_id)).timed_wait (
          Systick_clock::ticks_cast (millisec * 1000u));
      if (res == ETIMEDOUT)
        {
          return 0;
        }
    }

  if (res == 0)
    {
      int count =
          (int32_t) (reinterpret_cast<Semaphore&> (*semaphore_id)).value ();
      return count + 1;
    }
  else
    {
      return -1;
    }
}

/**
 * @details
 * Release a Semaphore token. This increments the count of
 * available semaphore tokens.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
osStatus
osSemaphoreRelease (osSemaphoreId semaphore_id)
{
  if (semaphore_id == nullptr)
    {
      return osErrorParameter;
    }

  if ((reinterpret_cast<Semaphore&> (*semaphore_id)).initial_value () == 0)
    {
      return osErrorResource;
    }

  result_t res = (reinterpret_cast<Semaphore&> (*semaphore_id)).post ();

  if (res == result::ok)
    {
      return osOK;
    }
  else if (res == EAGAIN)
    {
      return osErrorResource;
    }
  else
    {
      return osErrorOS;
    }
}

/**
 * @details
 * Delete a Semaphore object. The function releases internal memory
 * obtained for Semaphore handling. After this call the semaphore_id
 * is no longer valid and cannot be used. The Semaphore may be created
 * again using the function osSemaphoreCreate.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osSemaphoreDelete (osSemaphoreId semaphore_id)
{
  if (scheduler::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (semaphore_id == nullptr)
    {
      return osErrorParameter;
    }

  (reinterpret_cast<Semaphore&> (*semaphore_id)).~Semaphore ();
  return osOK;
}

#endif /* Semaphore available */

// ----------------------------------------------------------------------------
//  ==== Memory Pool Management Functions ====

#if (defined (osFeature_Pool)  &&  (osFeature_Pool != 0))

/**
 * @details
 * Create and initialize a memory pool.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osPoolId
osPoolCreate (const osPoolDef_t* pool_def)
{
  if (scheduler::in_handler_mode ())
    {
      return nullptr;
    }

  if (pool_def == nullptr)
    {
      return nullptr;
    }

  mempool::Attributes attr
    { pool_def->name };
  attr.mp_pool_address = pool_def->pool;
  attr.mp_pool_size_bytes = pool_def->pool_sz;
  return reinterpret_cast<osPoolId> (new ((void*) pool_def->data) Memory_pool (
      attr, (mempool::size_t) pool_def->items,
      (mempool::size_t) pool_def->item_sz));
}

/**
 * @details
 * Allocate a memory block from the memory pool.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
void*
osPoolAlloc (osPoolId pool_id)
{
  if (pool_id == nullptr)
    {
      return nullptr;
    }

  return (reinterpret_cast<Memory_pool&> (*pool_id)).try_alloc ();
}

/**
 * @details
 * Allocate a memory block from a memory pool and set memory block to zero.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
void*
osPoolCAlloc (osPoolId pool_id)
{
  if (pool_id == nullptr)
    {
      return nullptr;
    }

  void* ret;
  ret = (reinterpret_cast<Memory_pool&> (*pool_id)).try_alloc ();
  if (ret != nullptr)
    {
      memset (ret, 0,
              (reinterpret_cast<Memory_pool&> (*pool_id)).block_size ());
    }

  return ret;
}

/**
 * @details
 * Return an allocated memory block back to the memory pool.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
osStatus
osPoolFree (osPoolId pool_id, void* block)
{
  if (pool_id == nullptr)
    {
      return osErrorParameter;
    }

  if (block == nullptr)
    {
      return osErrorParameter;
    }

  result_t res;
  res = (reinterpret_cast<Memory_pool&> (*pool_id)).free (block);

  if (res == result::ok)
    {
      return osOK;
    }
  else if (res == EINVAL)
    {
      return osErrorValue;
    }
  else
    {
      return osErrorOS;
    }
}

#endif /* Memory Pool Management available */

// ----------------------------------------------------------------------------
//  ==== Message Queue Management Functions ====

#if (defined (osFeature_MessageQ)  &&  (osFeature_MessageQ != 0))

/**
 * @details
 * Create and initialise a message queue.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osMessageQId
osMessageCreate (const osMessageQDef_t* queue_def,
                 osThreadId thread_id __attribute__((unused)))
{
  if (scheduler::in_handler_mode ())
    {
      return nullptr;
    }

  if (queue_def == nullptr)
    {
      return nullptr;
    }

  mqueue::Attributes attr
    { queue_def->name };
  attr.mq_queue_address = queue_def->queue;
  attr.mq_queue_size_bytes = queue_def->queue_sz;

  return reinterpret_cast<osMessageQId> (new ((void*) queue_def->data) Message_queue (
      attr, (mqueue::size_t) queue_def->items,
      (mqueue::size_t) queue_def->item_sz));
}

/**
 * @details
 * Put the message info in a message queue specified by queue_id.
 *
 * When the message queue is full, the system retries for a specified
 * time with millisec. While the system retries the thread that is
 * calling this function is put into the state WAITING. The millisec
 * timeout can have the following values:
 *
 * - when millisec is 0, the function returns instantly.
 * - when millisec is set to osWaitForever the function will wait
 *  for an infinite time until a message queue slot becomes available.
 * - all other values specify a time in millisecond for a timeout.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
osStatus
osMessagePut (osMessageQId queue_id, uint32_t info, uint32_t millisec)
{
#pragma GCC diagnostic push
#if defined ( __clang__ )
#pragma clang diagnostic ignored "-Wint-to-pointer-cast"
#endif

  if (queue_id == nullptr)
    {
      return osErrorParameter;
    }

  result_t res;
  if (millisec == osWaitForever)
    {
      if (scheduler::in_handler_mode ())
        {
          return osErrorParameter;
        }
      res = (reinterpret_cast<Message_queue&> (*queue_id)).send (
          (const char*) &info, sizeof(uint32_t), 0);
      // osOK, osErrorResource, osErrorParameter
    }
  else if (millisec == 0)
    {
      res = (reinterpret_cast<Message_queue&> (*queue_id)).try_send (
          (const char*) &info, sizeof(uint32_t), 0);
      // osOK, osErrorResource, osErrorParameter
    }
  else
    {
      if (scheduler::in_handler_mode ())
        {
          return osErrorParameter;
        }
      res = (reinterpret_cast<Message_queue&> (*queue_id)).timed_send (
          (const char*) &info, sizeof(uint32_t), 0,
          Systick_clock::ticks_cast (millisec * 1000u));
      // osOK, osErrorTimeoutResource, osErrorParameter
    }

  if (res == result::ok)
    {
      // The message was put into the queue.
      return osOK;
    }
  else if (res == EWOULDBLOCK)
    {
      // No memory in the queue was available
      return osErrorResource;
    }
  else if (res == ETIMEDOUT)
    {
      // No memory in the queue was available during the given time limit.
      return osErrorTimeoutResource;
    }
  else if (res == EINVAL || res == EMSGSIZE)
    {
      // A parameter is invalid or outside of a permitted range.
      return osErrorParameter;
    }
  else
    {
      return osErrorOS;
    }

#pragma GCC diagnostic pop
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

/**
 * @details
 * Suspend the execution of the current RUNNING thread until a
 * message arrives. When a message is already in the queue,
 * the function returns instantly with the message information.
 *
 * The argument millisec specifies how long the system waits for
 * a message to become available. While the system waits the thread
 * that is calling this function is put into the state WAITING.
 * The millisec timeout value can have the following values:
 *
 * - when millisec is 0, the function returns instantly.
 * - when millisec is set to osWaitForever the function will
 * wait for an infinite time until a message arrives.
 * - all other values specify a time in millisecond for a timeout.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
osEvent
osMessageGet (osMessageQId queue_id, uint32_t millisec)
{
  osEvent event;
  result_t res;

  if (queue_id == nullptr)
    {
      event.status = osErrorParameter;
      return event;
    }

  if (millisec == osWaitForever)
    {
      if (scheduler::in_handler_mode ())
        {
          event.status = osErrorParameter;
          return event;
        }
      res = (reinterpret_cast<Message_queue&> (*queue_id)).receive (
          (char*) &event.value.v, sizeof(uint32_t),
          NULL);
      // result::event_message;
    }
  else if (millisec == 0)
    {
      res = (reinterpret_cast<Message_queue&> (*queue_id)).try_receive (
          (char*) &event.value.v, sizeof(uint32_t), NULL);
      // result::event_message when message;
      // result::ok when no meessage
    }
  else
    {
      if (scheduler::in_handler_mode ())
        {
          event.status = osErrorParameter;
          return event;
        }
      res = (reinterpret_cast<Message_queue&> (*queue_id)).timed_receive (
          (char*) &event.value.v, sizeof(uint32_t), NULL,
          Systick_clock::ticks_cast (millisec * 1000u));
      // result::event_message when message;
      // result::event_timeout when timeout;
    }

  if (res == result::ok)
    {
      // Message received, value.p contains the pointer to message.
      event.status = osEventMessage;
    }
  else if (res == ETIMEDOUT)
    {
      // No message has arrived during the given timeout period.
      event.status = osEventTimeout;
    }
  else if (res == EINVAL || res == EMSGSIZE)
    {
      // A parameter is invalid or outside of a permitted range.
      event.status = osErrorParameter;
    }
  else if (res == EWOULDBLOCK)
    {
      // No message is available in the queue and no timeout was specified.
      event.status = osOK;
    }
  else
    {
      event.status = osErrorOS;
    }

  return event;
}

#pragma GCC diagnostic pop

#endif /* Message Queues available */

// ----------------------------------------------------------------------------
//  ==== Mail Queue Management Functions ====

#if (defined (osFeature_MailQ)  &&  (osFeature_MailQ != 0))

/**
 * @details
 * Create and initialise a mail queue.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osMailQId
osMailCreate (const osMailQDef_t* queue_def,
              osThreadId thread_id __attribute__((unused)))
{
  if (scheduler::in_handler_mode ())
    {
      return nullptr;
    }

  if (queue_def == nullptr)
    {
      return nullptr;
    }

  mempool::Attributes pool_attr
    { queue_def->name };
  pool_attr.mp_pool_address = queue_def->pool;
  pool_attr.mp_pool_size_bytes = queue_def->pool_sz;
  new ((void*) &queue_def->data->pool) Memory_pool (
      pool_attr, (mempool::size_t) queue_def->items,
      (mempool::size_t) queue_def->pool_item_sz);

  mqueue::Attributes queue_attr
    { queue_def->name };
  queue_attr.mq_queue_address = queue_def->queue;
  queue_attr.mq_queue_size_bytes = queue_def->queue_sz;
  new ((void*) &queue_def->data->queue) Message_queue (
      queue_attr, (mqueue::size_t) queue_def->items,
      (mqueue::size_t) queue_def->queue_item_sz);

  return (osMailQId) (queue_def->data);
}

/**
 * @details
 * Allocate a memory block from the mail queue that is filled
 * with the mail information.
 *
 * The argument queue_id specifies a mail queue identifier that
 * is obtain with osMailCreate.
 *
 * The argument millisec specifies how long the system waits for
 * a mail slot to become available. While the system waits the
 * tread calling this function is put into the state WAITING.
 * The millisec timeout can have the following values:
 *
 * - when millisec is 0, the function returns instantly.
 * - when millisec is set to osWaitForever the function will
 * wait for an infinite time until a mail slot can be allocated.
 * - all other values specify a time in millisecond for a timeout.
 *
 * A NULL pointer is returned when no memory slot can be obtained
 * or queue specifies an illegal parameter.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
void*
osMailAlloc (osMailQId queue_id, uint32_t millisec)
{
  if (queue_id == nullptr)
    {
      return nullptr;
    }

  void* ret = nullptr;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
  if (millisec == osWaitForever)
    {
      if (scheduler::in_handler_mode ())
        {
          return nullptr;
        }
      ret = (reinterpret_cast<Memory_pool&> (queue_id->pool)).alloc ();
    }
  else if (millisec == 0)
    {
      ret = (reinterpret_cast<Memory_pool&> (queue_id->pool)).try_alloc ();
    }
  else
    {
      if (scheduler::in_handler_mode ())
        {
          return nullptr;
        }
      ret = (reinterpret_cast<Memory_pool&> (queue_id->pool)).timed_alloc (
          Systick_clock::ticks_cast (millisec * 1000u));
    }
#pragma GCC diagnostic pop
  return ret;
}

/**
 * @details
 * Allocate a memory block from the mail queue that is filled with
 * the mail information. The memory block returned is cleared.
 *
 * The argument queue_id specifies a mail queue identifier that is
 * obtain with osMailCreate.
 *
 * The argument millisec specifies how long the system waits for a
 * mail slot to become available. While the system waits the thread
 * that is calling this function is put into the state WAITING.
 * The millisec timeout can have the following values:
 *
 * - when millisec is 0, the function returns instantly.
 * - when millisec is set to osWaitForever the function will
 * wait for an infinite time until a mail slot can be allocated.
 * - all other values specify a time in millisecond for a timeout.
 *
 * A NULL pointer is returned when no memory block can be obtained
 * or queue specifies an illegal parameter.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
void*
osMailCAlloc (osMailQId queue_id, uint32_t millisec)
{
  void* ret = osMailAlloc (queue_id, millisec);
  if (ret != nullptr)
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
      memset (ret, 0,
              (reinterpret_cast<Memory_pool&> (queue_id->pool)).block_size ());
#pragma GCC diagnostic pop
    }
  return ret;
}

/**
 * @details
 * Put the memory block specified with mail into the mail queue
 * specified by queue.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
osStatus
osMailPut (osMailQId queue_id, void* mail)
{
  if (queue_id == nullptr)
    {
      return osErrorParameter;
    }
  if (mail == nullptr)
    {
      return osErrorValue;
    }

  // Validate pointer.
  Memory_pool* pool = reinterpret_cast<Memory_pool*> (&queue_id->pool);
  if (((char*) mail < (char*) (pool->pool ()))
      || (((char*) mail)
          >= ((char*) (pool->pool ()) + pool->capacity () * pool->block_size ())))
    {
      return osErrorValue;
    }

  result_t res;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
  res = (reinterpret_cast<Message_queue&> (queue_id->queue)).try_send (
      (const char*) &mail, sizeof(void*), 0);
#pragma GCC diagnostic pop
  if (res == result::ok)
    {
      return osOK;
    }
  else
    {
      return osErrorOS;
    }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"
#pragma GCC diagnostic ignored "-Wstrict-aliasing"

/**
 * @details
 * Suspend the execution of the current RUNNING thread until a mail
 * arrives. When a mail is already in the queue, the function returns
 * instantly with the mail information.
 *
 * The argument millisec specifies how long the system waits for a
 * mail to arrive. While the system waits the thread that is calling
 * this function is put into the state WAITING. The millisec timeout
 * can have the following values:
 *
 * - when millisec is 0, the function returns instantly.
 * - when millisec is set to osWaitForever the function will wait
 * for an infinite time until a mail arrives.
 * - all other values specify a time in millisecond for a timeout.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
osEvent
osMailGet (osMailQId mail_id, uint32_t millisec)
{
  osEvent event;
  result_t res;

  if (mail_id == nullptr)
    {
      event.status = osErrorParameter;
      return event;
    }

  if (millisec == osWaitForever)
    {
      if (scheduler::in_handler_mode ())
        {
          event.status = osErrorParameter;
          return event;
        }
      res = (reinterpret_cast<Message_queue&> ((mail_id->queue))).receive (
          (char*) &event.value.p, sizeof(void*),
          NULL);
      // osEventMail for ok,
    }
  else if (millisec == 0)
    {
      res = (reinterpret_cast<Message_queue&> (mail_id->queue)).try_receive (
          (char*) &event.value.p, sizeof(void*),
          NULL);
      // osEventMail for ok,
    }
  else
    {
      if (scheduler::in_handler_mode ())
        {
          event.status = osErrorParameter;
          return event;
        }
      res = (reinterpret_cast<Message_queue&> (mail_id->queue)).timed_receive (
          (char*) &event.value.p, sizeof(void*), NULL,
          Systick_clock::ticks_cast (millisec * 1000u));
      // osEventMail for ok, osEventTimeout
    }

  if (res == result::ok)
    {
      // Mail received, value.p contains the pointer to mail content.
      event.status = osEventMail;
    }
  else if (res == EWOULDBLOCK)
    {
      // No mail is available in the queue and no timeout was specified.
      event.status = osOK;
    }
  else if (res == EINVAL || res == EMSGSIZE)
    {
      // A parameter is invalid or outside of a permitted range.
      event.status = osErrorParameter;
    }
  else if (res == ETIMEDOUT)
    {
      // No mail has arrived during the given timeout period.
      event.status = osEventTimeout;
    }
  else
    {
      event.status = osErrorOS;
    }

  return event;
}

#pragma GCC diagnostic pop

/**
 * @details
 * Free the memory block specified by mail and return it to the mail queue.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
osStatus
osMailFree (osMailQId mail_id, void* mail)
{
  if (mail_id == nullptr)
    {
      return osErrorParameter;
    }
  if (mail == nullptr)
    {
      return osErrorValue;
    }

  return osPoolFree (&(mail_id->pool), mail);
}

#endif /* Mail Queues available */

// ----------------------------------------------------------------------------

#pragma GCC diagnostic pop
