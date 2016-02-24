/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 * Copyright (c) 2013 ARM LIMITED
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

  os_result_t
  os_sched_start (void);

  bool
  os_sched_is_started (void);

  // --------------------------------------------------------------------------

  void
  os_thread_create (os_thread_t* thread, const os_thread_attr_t* attr,
                    os_thread_func_t func, const os_thread_func_args_t args);

  void
  os_thread_destroy (os_thread_t* thread);

  void
  os_this_thread_exit (void* exit_ptr);

  os_result_t
  os_thread_join (os_thread_t* thread, void** exit_ptr);

  os_thread_prio_t
  os_thread_get_prio (os_thread_t* thread);

  os_result_t
  os_thread_set_prio (os_thread_t* thread, os_thread_prio_t prio);

  void
  os_thread_wakeup (os_thread_t* thread);

  // --------------------------------------------------------------------------

  void
  os_mutex_create (os_mutex_t* mutex, const os_mutex_attr_t* attr);

  void
  os_mutex_destroy (os_mutex_t* mutex);

  os_result_t
  os_mutex_lock (os_mutex_t* mutex);

  os_result_t
  os_mutex_try_lock (os_mutex_t* mutex);

  os_result_t
  os_mutex_timed_lock (os_mutex_t* mutex, os_systicks_t ticks);

  os_result_t
  os_mutex_unlock (os_mutex_t* mutex);

  os_thread_prio_t
  os_get_mutex_prio_ceiling (os_mutex_t* mutex);

  os_result_t
  os_set_mutex_prio_ceiling (os_mutex_t* mutex, os_thread_prio_t prio_ceiling,
                             os_thread_prio_t* old_prio_ceiling);

  // --------------------------------------------------------------------------

  void
  os_condvar_create (os_condvar_t* condvar, os_condvar_attr_t* attr);

  void
  os_condvar_destroy (os_condvar_t* condvar);

  os_result_t
  os_condvar_signal (os_condvar_t* condvar);

  os_result_t
  os_condvar_broadcast (os_condvar_t* condvar);

  os_result_t
  os_condvar_wait (os_condvar_t* condvar, os_mutex_t* mutex);

  os_result_t
  os_condvar_timed_wait (os_condvar_t* condvar, os_mutex_t* mutex,
                         os_systicks_t ticks);

  // --------------------------------------------------------------------------

  void
  os_semaphore_create (os_semaphore_t* semaphore, os_semaphore_attr_t* attr);

  void
  os_semaphore_destroy (os_semaphore_t* semaphore);

  os_result_t
  os_semaphore_post (os_semaphore_t* semaphore);

  os_result_t
  os_semaphore_wait (os_semaphore_t* semaphore);

  os_result_t
  os_semaphore_try_wait (os_semaphore_t* semaphore);

  os_result_t
  os_semaphore_timed_wait (os_semaphore_t* semaphore, os_systicks_t ticks);

  os_semaphore_count_t
  os_semaphore_get_value (os_semaphore_t* semaphore);

  os_result_t
  os_semaphore_reset (os_semaphore_t* semaphore);

  // --------------------------------------------------------------------------

  void
  os_mempool_create (os_mempool_t* mempool, os_mempool_attr_t* attr,
                     os_mempool_size_t blocks,
                     os_mempool_size_t block_size_bytes);

  void
  os_mempool_destroy (os_mempool_t* mempool);

  void*
  os_mempool_alloc (os_mempool_t* mempool);

  void*
  os_mempool_try_alloc (os_mempool_t* mempool);

  void*
  os_mempool_timed_alloc (os_mempool_t* mempool, os_systicks_t ticks);

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

  // --------------------------------------------------------------------------

  void
  os_mqueue_create (os_mqueue_t* mqueue, os_mqueue_attr_t* attr,
                    os_mqueue_size_t msgs, os_mqueue_size_t msg_size_bytes);

  void
  os_mqueue_destroy (os_mqueue_t* mqueue);

  os_result_t
  os_mqueue_send (os_mqueue_t* mqueue, const char* msg, size_t nbytes,
                  os_mqueue_prio_t mprio);

  os_result_t
  os_mqueue_try_send (os_mqueue_t* mqueue, const char* msg, size_t nbytes,
                      os_mqueue_prio_t mprio);

  os_result_t
  os_mqueue_timed_send (os_mqueue_t* mqueue, const char* msg, size_t nbytes,
                        os_mqueue_prio_t mprio, os_systicks_t ticks);

  os_result_t
  os_mqueue_receive (os_mqueue_t* mqueue, char* msg, size_t nbytes,
                     os_mqueue_prio_t* mprio);

  os_result_t
  os_mqueue_try_receive (os_mqueue_t* mqueue, char* msg, size_t nbytes,
                         os_mqueue_prio_t* mprio);

  os_result_t
  os_mqueue_timed_receive (os_mqueue_t* mqueue, char* msg, size_t nbytes,
                           os_mqueue_prio_t* mprio, os_systicks_t ticks);

  size_t
  os_mqueue_get_length (os_mqueue_t* mqueue);

  size_t
  os_mqueue_get_capacity (os_mqueue_t* mqueue);

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
  os_systick_handler (void);

  void
  os_rtc_handler (void);

// --------------------------------------------------------------------------

#ifdef  __cplusplus
}
#endif

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_RTOS_OS_C_API_H_ */
