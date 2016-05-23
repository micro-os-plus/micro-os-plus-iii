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
 * The initial CMSIS++ RTOS API was inspired by CMSIS RTOS API v1.x,
 * Copyright (c) 2013 ARM LIMITED.
 */

/*
 * This file is part of the CMSIS++ proposal, intended as a CMSIS
 * replacement for C++ applications, and provides a C API.
 *
 * The code is inspired by ARM CMSIS cmsis_os.h file, v1.02,
 * and tries to remain functionally close to the CMSIS specifications.
 */

#ifndef CMSIS_PLUS_RTOS_OS_C_API_H_
#define CMSIS_PLUS_RTOS_OS_C_API_H_

// ----------------------------------------------------------------------------

// Include the CMSIS++ C API structures declarations.
#include <cmsis-plus/rtos/os-c-decls.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// ----------------------------------------------------------------------------

#ifdef  __cplusplus
extern "C"
{
#endif

  // --------------------------------------------------------------------------

  enum
  {
    os_ok = 0, ///< function completed; no error or event occurred.
  };

  // --------------------------------------------------------------------------

  int
  os_main (int argc, char* argv[]);

  // --------------------------------------------------------------------------

  os_result_t
  os_sched_initialize (void);

  void
  __attribute__((noreturn))
  os_sched_start (void);

  bool
  os_sched_is_started (void);

  os_sched_status_t
  os_sched_lock (os_sched_status_t status);

  void
  os_sched_unlock (os_sched_status_t status);

  bool
  os_sched_is_locked (void);

  // --------------------------------------------------------------------------

  os_irq_status_t
  os_irq_critical_enter (void);

  void
  os_irq_critical_exit (os_irq_status_t status);

  // --------------------------------------------------------------------------

  os_irq_status_t
  os_irq_uncritical_enter (void);

  void
  os_irq_uncritical_exit (os_irq_status_t status);

  // --------------------------------------------------------------------------

  void
  os_this_thread_wait (void);

  void
  __attribute__((noreturn))
  os_this_thread_exit (void* exit_ptr);

  os_result_t
  os_this_thread_sig_wait (os_thread_sigset_t mask, os_thread_sigset_t* oflags,
                           os_flags_mode_t mode);

  os_result_t
  os_this_thread_try_sig_wait (os_thread_sigset_t mask,
                               os_thread_sigset_t* oflags,
                               os_flags_mode_t mode);

  os_result_t
  os_this_thread_timed_sig_wait (os_thread_sigset_t mask,
                                 os_clock_duration_t timeout,
                                 os_thread_sigset_t* oflags,
                                 os_flags_mode_t mode);

  // --------------------------------------------------------------------------

  void
  os_thread_attr_init (os_thread_attr_t* attr, const char* name);

  void
  os_thread_create (os_thread_t* thread, const char* name,
                    const os_thread_attr_t* attr, os_thread_func_t func,
                    const os_thread_func_args_t args);

  void
  os_thread_destroy (os_thread_t* thread);

  const char*
  os_thread_get_name (os_thread_t* thread);

  os_result_t
  os_thread_join (os_thread_t* thread, void** exit_ptr);

  os_thread_prio_t
  os_thread_get_prio (os_thread_t* thread);

  os_result_t
  os_thread_set_prio (os_thread_t* thread, os_thread_prio_t prio);

  void
  os_thread_resume (os_thread_t* thread);

  os_thread_user_storage_t*
  os_thread_get_user_storage (os_thread_t* thread);

  os_result_t
  os_thread_sig_raise (os_thread_t* thread, os_thread_sigset_t mask,
                       os_thread_sigset_t* oflags);

  os_result_t
  os_thread_sig_clear (os_thread_t* thread, os_thread_sigset_t mask,
                       os_thread_sigset_t* oflags);

  os_thread_sigset_t
  os_thread_sig_get (os_thread_t* thread, os_thread_sigset_t mask,
                     os_flags_mode_t mode);

  // --------------------------------------------------------------------------

  const char*
  os_clock_name (os_clock_t* clock);

  os_clock_timestamp_t
  os_clock_now (os_clock_t* clock);

  os_clock_timestamp_t
  os_clock_steady_now (os_clock_t* clock);

  os_result_t
  os_clock_sleep_for (os_clock_t* clock, os_clock_duration_t timeout);

  os_result_t
  os_clock_sleep_until (os_clock_t* clock, os_clock_timestamp_t timestamp);

  os_result_t
  os_clock_wait_for (os_clock_t* clock, os_clock_duration_t timeout);

  os_clock_t*
  os_clock_get_sysclock (void);

  os_clock_t*
  os_clock_get_rtclock (void);

  // --------------------------------------------------------------------------

  os_clock_timestamp_t
  os_sysclock_now_details (os_sysclock_current_t* details);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"

  inline os_clock_duration_t
  __attribute__((always_inline))
  os_sysclock_ticks_cast (uint32_t microsec)
  {
    return (os_clock_duration_t) ((((microsec)
        * ((uint32_t) OS_INTEGER_SYSTICK_FREQUENCY_HZ)) + (uint32_t) 1000000ul
        - 1) / (uint32_t) 1000000ul);
  }

  inline os_clock_duration_t
  __attribute__((always_inline))
  os_sysclock_ticks_cast_long (uint64_t microsec)
  {
    return (os_clock_duration_t) ((((microsec)
        * ((uint64_t) OS_INTEGER_SYSTICK_FREQUENCY_HZ)) + (uint64_t) 1000000ul
        - 1) / (uint64_t) 1000000ul);
  }

#pragma GCC diagnostic pop

  // --------------------------------------------------------------------------

  void
  os_timer_attr_init (os_timer_attr_t* attr, const char* name);

  void
  os_timer_create (os_timer_t* timer, const char* name,
                   const os_timer_attr_t* attr, os_timer_func_t func,
                   os_timer_func_args_t args);

  void
  os_timer_destroy (os_timer_t* timer);

  const char*
  os_timer_get_name (os_timer_t* timer);

  os_result_t
  os_timer_start (os_timer_t* timer, os_clock_duration_t timeout);

  os_result_t
  os_timer_stop (os_timer_t* timer);

  // --------------------------------------------------------------------------

  void
  os_mutex_attr_init (os_mutex_attr_t* attr, const char* name);

  void
  os_mutex_create (os_mutex_t* mutex, const char* name,
                   const os_mutex_attr_t* attr);

  void
  os_mutex_destroy (os_mutex_t* mutex);

  const char*
  os_mutex_get_name (os_mutex_t* mutex);

  os_result_t
  os_mutex_lock (os_mutex_t* mutex);

  os_result_t
  os_mutex_try_lock (os_mutex_t* mutex);

  os_result_t
  os_mutex_timed_lock (os_mutex_t* mutex, os_clock_duration_t timeout);

  os_result_t
  os_mutex_unlock (os_mutex_t* mutex);

  os_thread_prio_t
  os_mutex_get_prio_ceiling (os_mutex_t* mutex);

  os_result_t
  os_mutex_set_prio_ceiling (os_mutex_t* mutex, os_thread_prio_t prio_ceiling,
                             os_thread_prio_t* old_prio_ceiling);

  os_result_t
  os_mutex_mark_consistent (os_mutex_t* mutex);

  os_thread_t*
  os_mutex_get_owner (os_mutex_t* mutex);

  os_result_t
  os_mutex_reset (os_mutex_t* mutex);

  // --------------------------------------------------------------------------

  void
  os_condvar_attr_init (os_condvar_attr_t* attr, const char* name);

  void
  os_condvar_create (os_condvar_t* condvar, const char* name,
                     os_condvar_attr_t* attr);

  void
  os_condvar_destroy (os_condvar_t* condvar);

  const char*
  os_condvar_get_name (os_condvar_t* condvar);

  os_result_t
  os_condvar_signal (os_condvar_t* condvar);

  os_result_t
  os_condvar_broadcast (os_condvar_t* condvar);

  os_result_t
  os_condvar_wait (os_condvar_t* condvar, os_mutex_t* mutex);

  os_result_t
  os_condvar_timed_wait (os_condvar_t* condvar, os_mutex_t* mutex,
                         os_clock_duration_t timeout);

  // --------------------------------------------------------------------------

  void
  os_semaphore_attr_init (os_semaphore_attr_t* attr, const char* name);

  void
  os_semaphore_create (os_semaphore_t* semaphore, const char* name,
                       os_semaphore_attr_t* attr);

  void
  os_semaphore_destroy (os_semaphore_t* semaphore);

  const char*
  os_semaphore_get_name (os_semaphore_t* semaphore);

  os_result_t
  os_semaphore_post (os_semaphore_t* semaphore);

  os_result_t
  os_semaphore_wait (os_semaphore_t* semaphore);

  os_result_t
  os_semaphore_try_wait (os_semaphore_t* semaphore);

  os_result_t
  os_semaphore_timed_wait (os_semaphore_t* semaphore,
                           os_clock_duration_t timeout);

  os_semaphore_count_t
  os_semaphore_get_value (os_semaphore_t* semaphore);

  os_result_t
  os_semaphore_reset (os_semaphore_t* semaphore);

  os_semaphore_count_t
  os_semaphore_get_initial_value (os_semaphore_t* semaphore);

  os_semaphore_count_t
  os_semaphore_get_max_value (os_semaphore_t* semaphore);

  // --------------------------------------------------------------------------

  void
  os_mempool_attr_init (os_mempool_attr_t* attr, const char* name);

  void
  os_mempool_create (os_mempool_t* mempool, const char* name,
                     os_mempool_attr_t* attr, size_t blocks,
                     size_t block_size_bytes);

  void
  os_mempool_destroy (os_mempool_t* mempool);

  const char*
  os_mempool_get_name (os_mempool_t* mempool);

  void*
  os_mempool_alloc (os_mempool_t* mempool);

  void*
  os_mempool_try_alloc (os_mempool_t* mempool);

  void*
  os_mempool_timed_alloc (os_mempool_t* mempool, os_clock_duration_t timeout);

  os_result_t
  os_mempool_free (os_mempool_t* mempool, void* block);

  size_t
  os_mempool_get_capacity (os_mempool_t* mempool);

  size_t
  os_mempool_get_count (os_mempool_t* mempool);

  size_t
  os_mempool_get_block_size (os_mempool_t* mempool);

  bool
  os_mempool_is_empty (os_mempool_t* mempool);

  bool
  os_mempool_is_full (os_mempool_t* mempool);

  os_result_t
  os_mempool_reset (os_mempool_t* mempool);

  void*
  os_mempool_get_pool (os_mempool_t* mempool);

  // --------------------------------------------------------------------------

  void
  os_mqueue_attr_init (os_mqueue_attr_t* attr, const char* name);

  void
  os_mqueue_create (os_mqueue_t* mqueue, const char* name,
                    os_mqueue_attr_t* attr, size_t msgs, size_t msg_size_bytes);

  void
  os_mqueue_destroy (os_mqueue_t* mqueue);

  const char*
  os_mqueue_get_name (os_mqueue_t* mqueue);

  os_result_t
  os_mqueue_send (os_mqueue_t* mqueue, const char* msg, size_t nbytes,
                  os_mqueue_prio_t mprio);

  os_result_t
  os_mqueue_try_send (os_mqueue_t* mqueue, const char* msg, size_t nbytes,
                      os_mqueue_prio_t mprio);

  os_result_t
  os_mqueue_timed_send (os_mqueue_t* mqueue, const char* msg, size_t nbytes,
                        os_clock_duration_t timeout, os_mqueue_prio_t mprio);

  os_result_t
  os_mqueue_receive (os_mqueue_t* mqueue, char* msg, size_t nbytes,
                     os_mqueue_prio_t* mprio);

  os_result_t
  os_mqueue_try_receive (os_mqueue_t* mqueue, char* msg, size_t nbytes,
                         os_mqueue_prio_t* mprio);

  os_result_t
  os_mqueue_timed_receive (os_mqueue_t* mqueue, char* msg, size_t nbytes,
                           os_clock_duration_t timeout,
                           os_mqueue_prio_t* mprio);

  size_t
  os_mqueue_get_capacity (os_mqueue_t* mqueue);

  size_t
  os_mqueue_get_length (os_mqueue_t* mqueue);

  size_t
  os_mqueue_get_msg_size (os_mqueue_t* mqueue);

  bool
  os_mqueue_is_empty (os_mqueue_t* mqueue);

  bool
  os_mqueue_is_full (os_mqueue_t* mqueue);

  os_result_t
  os_mqueue_reset (os_mqueue_t* mqueue);

  // --------------------------------------------------------------------------

  void
  os_evflags_attr_init (os_evflags_attr_t* attr, const char* name);

  void
  os_evflags_create (os_evflags_t* evflags, const char* name,
                     os_evflags_attr_t* attr);

  void
  os_evflags_destroy (os_evflags_t* evflags);

  const char*
  os_evflags_get_name (os_evflags_t* evflags);

  os_result_t
  os_evflags_wait (os_evflags_t* evflags, os_flags_mask_t mask,
                   os_flags_mask_t* oflags, os_flags_mode_t mode);

  os_result_t
  os_evflags_try_wait (os_evflags_t* evflags, os_flags_mask_t mask,
                       os_flags_mask_t* oflags, os_flags_mode_t mode);

  os_result_t
  os_evflags_timed_wait (os_evflags_t* evflags, os_flags_mask_t mask,
                         os_clock_duration_t timeout, os_flags_mask_t* oflags,
                         os_flags_mode_t mode);

  os_result_t
  os_evflags_raise (os_evflags_t* evflags, os_flags_mask_t mask,
                    os_flags_mask_t* oflags);

  os_result_t
  os_evflags_clear (os_evflags_t* evflags, os_flags_mask_t mask,
                    os_flags_mask_t* oflags);

  os_flags_mask_t
  os_evflags_get (os_evflags_t* evflags, os_flags_mask_t mask,
                  os_flags_mode_t mode);

  bool
  os_evflags_get_waiting (os_evflags_t* evflags);

  // --------------------------------------------------------------------------

  // Internal functions, to be defined by the user.
  // (not to be called directly).

  void
  os_systick_handler (void);

  void
  os_rtc_handler (void);

// --------------------------------------------------------------------------

#ifdef  __cplusplus
}
#endif

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_RTOS_OS_C_API_H_ */
