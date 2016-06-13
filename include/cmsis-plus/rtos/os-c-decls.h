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
 * The structures declared in this file are used both in the C API
 * and in the legacy CMSIS API.
 *
 * Since there is no method to automatically sync them with the C++
 * definitions, they must be manually adjusted to match them, otherwise
 * the validation checks in os-c-wrapper.cpp will fail.
 */

#ifndef CMSIS_PLUS_RTOS_OS_C_DECLS_H_
#define CMSIS_PLUS_RTOS_OS_C_DECLS_H_

// ----------------------------------------------------------------------------

#include <cmsis-plus/rtos/os-app-config.h>
#include <cmsis-plus/rtos/port/os-decls.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// We definitely use CMSIS++.
#define OS_USE_CMSIS_PLUS

// ----------------------------------------------------------------------------

#ifdef  __cplusplus
extern "C"
{
#endif

  // --------------------------------------------------------------------------

  typedef uint32_t os_result_t;

  typedef struct os_double_list_links_s
  {
    void* prev;
    void* next;
  } os_double_list_links_t;

  typedef os_double_list_links_t os_threads_waiting_list_t;

  typedef struct os_thread_children_list_s
  {
    os_double_list_links_t links;
  } os_thread_children_list_t;

  typedef struct os_waiting_thread_node_s
  {
    os_double_list_links_t links;
    void* thread;
  } os_waiting_thread_node_t;

  typedef uint32_t os_flags_mode_t;
  typedef uint32_t os_flags_mask_t;

  enum
  {
    os_flags_mode_all = 1, //
    os_flags_mode_any = 2, //
    os_flags_mode_clear = 4, //
  };

  typedef struct os_clock_timestamps_list_s
  {
    os_double_list_links_t links;
  } os_clock_timestamps_list_t;

  // --------------------------------------------------------------------------

  typedef bool os_sched_status_t;
  typedef os_port_irq_status_t os_irq_status_t;

  // --------------------------------------------------------------------------

#define OS_THREAD_PRIO_SHIFT   (4)

  enum
  {
    // Must be ordered, with none the first and error the last.
    os_priority_none = 0, // not defined
    os_priority_idle = (1 << OS_THREAD_PRIO_SHIFT),
    os_priority_lowest = (2 << OS_THREAD_PRIO_SHIFT), // lowest
    os_priority_low = (2 << OS_THREAD_PRIO_SHIFT),
    os_priority_below_normal = (4 << OS_THREAD_PRIO_SHIFT),
    os_priority_normal = (6 << OS_THREAD_PRIO_SHIFT), // default
    os_priority_above_normal = (8 << OS_THREAD_PRIO_SHIFT),
    os_priority_high = (10 << OS_THREAD_PRIO_SHIFT),
    os_priority_realtime = (12 << OS_THREAD_PRIO_SHIFT),
    os_priority_highest = (((13 + 1) << OS_THREAD_PRIO_SHIFT) - 1),
    os_priority_isr = (((14 + 1) << OS_THREAD_PRIO_SHIFT) - 1),
    os_priority_error = (((15 + 1) << OS_THREAD_PRIO_SHIFT) - 1)
  };

  enum
  {
    os_thread_inactive = 0,
    os_thread_ready,
    os_thread_running,
    os_thread_waiting
  };

  enum
  {
    os_timer_once = 0, //
    os_timer_periodic = 1
  };

  typedef void* os_thread_func_args_t;
  typedef void*
  (*os_thread_func_t) (os_thread_func_args_t args);

  typedef uint8_t os_thread_state_t;
  typedef uint8_t os_thread_prio_t;

#if !defined(OS_INCLUDE_RTOS_CUSTOM_THREAD_USER_STORAGE) && !defined(__cplusplus)
  typedef struct
    {
      char dummy;
    }os_thread_user_storage_t;
#endif

  // --------------------------------------------------------------------------

  // Define clock types based on port definitions.
  typedef os_port_clock_timestamp_t os_clock_timestamp_t;
  typedef os_port_clock_duration_t os_clock_duration_t;
  typedef os_port_clock_offset_t os_clock_offset_t;

  // --------------------------------------------------------------------------

  // Generic iterator, implemented as a pointer.
  typedef void* os_iterator_t;

  // --------------------------------------------------------------------------

  typedef uint64_t os_statistics_counter_t;

  // --------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  typedef struct os_thread_stack_s
  {
    void* stack_addr;
    size_t stack_size_bytes;
  } os_thread_stack_t;

  typedef struct os_thread_context_s
  {
    os_thread_stack_t stack;
#if !defined(OS_INCLUDE_RTOS_PORT_SCHEDULER)
    os_port_thread_context_t port;
#endif
  } os_thread_context_t;

  typedef struct os_thread_statistics_s
  {
    os_statistics_counter_t context_switches;
  } os_thread_statistics_t;

  typedef struct os_thread_attr_s
  {
    void* clock;
    void* th_stack_address;
    size_t th_stack_size_bytes;
    os_thread_prio_t th_priority;
  } os_thread_attr_t;

  typedef struct os_thread_s
  {
    void* vtbl;
    const char* name;
    os_waiting_thread_node_t ready_node;
    int errno_; // Prevent the macro to expand (for example with a prefix).
    os_thread_func_t func;
    os_thread_func_args_t func_args;
    void* func_result_;
    void* parent;
    os_double_list_links_t child_links;
    os_thread_children_list_t children;
    void* joiner;
    void* waiting_node;
    void* clock_node;
    void* clock;
    void* allocator;
    void* allocted_stack_address;
    size_t acquired_mutexes;
    size_t allocated_stack_size_elements;
    os_thread_state_t state;
    os_thread_prio_t prio;
    os_flags_mask_t signals;
    os_thread_user_storage_t user_storage; //
    bool interrupted;
#if defined(OS_INCLUDE_RTOS_STATISTICS_CONTEXT_SWITCHES)
    os_thread_statistics_t statistics;
#endif /* defined(OS_INCLUDE_RTOS_STATISTICS_CONTEXT_SWITCHES) */

#if defined(OS_INCLUDE_RTOS_PORT_SCHEDULER)
    os_thread_port_data_t port;
#endif
    os_thread_context_t context;
  } os_thread_t;

#pragma GCC diagnostic pop

  // --------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  typedef struct os_clock_s
  {
    void* vtbl;
    const char* name;
    os_clock_timestamps_list_t steady_list;
    os_clock_duration_t sleep_count;
    os_clock_timestamp_t steady_count;

  } os_clock_t;

  typedef struct os_sysclock_current_s
  {
    uint64_t ticks; // Count of SysTick ticks since core reset
    uint32_t cycles; // Count of SysTick cycles since timer reload (24 bits)
    uint32_t divisor; // SysTick reload value (24 bits)
    uint32_t core_frequency_hz; // Core clock frequency Hz
  } os_sysclock_current_t;

#pragma GCC diagnostic pop

  // --------------------------------------------------------------------------

  typedef void* os_timer_func_args_t;
  typedef void
  (*os_timer_func_t) (os_timer_func_args_t args);

  typedef uint8_t os_timer_type_t;
  typedef uint8_t os_timer_state_t;

  enum
  {
    //
    os_timer_run_once = 0,
    os_timer_run_periodic = 1
  };

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  typedef struct os_timer_attr_s
  {
    void* clock;
    os_timer_type_t tm_type;
  } os_timer_attr_t;

  typedef struct os_clock_node_s
  {
    void* next;
    void* prev;
    void* list;
    os_clock_timestamp_t timestamp;
    void* timer;
  } os_clock_node_t;

  typedef struct os_timer_s
  {
    const char* name;
    os_timer_func_t func;
    os_timer_func_args_t func_args;
#if !defined(OS_INCLUDE_RTOS_PORT_TIMER)
    void* clock;
    os_clock_node_t clock_node;
    os_clock_duration_t period;
#endif
#if defined(OS_INCLUDE_RTOS_PORT_TIMER)
    os_timer_port_data_t port_;
#endif
    os_timer_type_t type;
    os_timer_state_t state;
  } os_timer_t;

#pragma GCC diagnostic pop

  // --------------------------------------------------------------------------

  typedef int16_t os_mutex_count_t;
  typedef uint8_t os_mutex_type_t;
  typedef uint8_t os_mutex_protocol_t;
  typedef uint8_t os_mutex_robustness_t;

  enum
  {
    //
    os_mutex_protocol_none = 0,
    os_mutex_protocol_inherit = 1,
    os_mutex_protocol_protect = 2
  };

  enum
  {
    //
    os_mutex_robustness_stalled = 0,
    os_mutex_robustness_robust = 1
  };

  enum
  {
    //
    os_mutex_type_normal = 0,
    os_mutex_type_errorcheck = 1,
    os_mutex_type_recursive = 2,
    os_mutex_type_default = os_mutex_type_normal,
  };

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  typedef struct os_mutex_attr_s
  {
    void* clock;
    os_thread_prio_t mx_priority_ceiling;
    os_mutex_protocol_t mx_protocol;
    os_mutex_robustness_t mx_robustness;
    os_mutex_type_t mx_type;
    os_mutex_count_t mx_max_count;
  } os_mutex_attr_t;

  typedef struct os_mutex_s
  {
    const char* name;
    void* owner;
#if !defined(OS_INCLUDE_RTOS_PORT_MUTEX)
    os_threads_waiting_list_t list;
    void* clock;
#endif
#if defined(OS_INCLUDE_RTOS_PORT_MUTEX)
    os_mutex_port_data_t port;
#endif
    os_mutex_count_t count;
    os_thread_prio_t prio_ceiling;
    os_thread_prio_t owner_prio;
    os_mutex_type_t type;
    os_mutex_protocol_t protocol;
    os_mutex_robustness_t robustness;
    os_mutex_count_t max_count;
  } os_mutex_t;

#pragma GCC diagnostic pop

  // --------------------------------------------------------------------------

  typedef struct os_condvar_attr_s
  {
    void* clock;
  } os_condvar_attr_t;

  typedef struct os_condvar_s
  {
    const char* name;
#if !defined(OS_INCLUDE_RTOS_PORT_CONDITION_VARIABLE)
    os_threads_waiting_list_t list;
    // void* clock;
#endif
  } os_condvar_t;

  // --------------------------------------------------------------------------

  typedef int16_t os_semaphore_count_t;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  typedef struct os_semaphore_attr_s
  {
    void* clock;
    os_semaphore_count_t sm_initial_count;
    os_semaphore_count_t sm_max_count;
  } os_semaphore_attr_t;

  typedef struct os_semaphore_s
  {
    const char* name;
#if !defined(OS_INCLUDE_RTOS_PORT_SEMAPHORE)
    os_threads_waiting_list_t list;
    void* clock;
#endif
#if defined(OS_INCLUDE_RTOS_PORT_SEMAPHORE)
    os_semaphore_port_data_t port;
#endif
    os_semaphore_count_t initial_count;
    os_semaphore_count_t count;
    os_semaphore_count_t max_count;
  } os_semaphore_t;

#pragma GCC diagnostic pop

  // --------------------------------------------------------------------------

  typedef uint16_t os_mempool_size_t;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  typedef struct os_mempool_attr_s
  {
    void* clock;
    void* mp_pool_address;
    size_t mp_pool_size_bytes;
  } os_mempool_attr_t;

  typedef struct os_mempool_s
  {
    void* vtbl;
    const char* name;
#if !defined(OS_INCLUDE_RTOS_PORT_MEMORY_POOL)
    os_threads_waiting_list_t list;
    void* clock;
#endif
    void* pool_addr;
    void* allocated_pool_addr;
    void* allocator;
#if defined(OS_INCLUDE_RTOS_PORT_MEMORY_POOL)
    os_mempool_port_data_t port;
#endif
    size_t pool_size_bytes;
    size_t allocated_pool_size_elements_;
    os_mempool_size_t blocks;
    os_mempool_size_t block_size_bytes;
    os_mempool_size_t count;
    void* first;
  } os_mempool_t;

#pragma GCC diagnostic pop

  // --------------------------------------------------------------------------

  typedef uint8_t os_mqueue_size_t;
  typedef uint16_t os_mqueue_msg_size_t;
  typedef uint8_t os_mqueue_prio_t;
  typedef os_mqueue_size_t os_mqueue_index_t;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  typedef struct os_mqueue_attr_s
  {
    void* clock;
    void* mq_queue_addr;
    size_t mq_queue_size_bytes;
  } os_mqueue_attr_t;

  typedef struct os_mqueue_s
  {
    void* vtbl;
    const char* name;
#if !defined(OS_INCLUDE_RTOS_PORT_MESSAGE_QUEUE)
    os_threads_waiting_list_t send_list;
    os_threads_waiting_list_t receive_list;
    void* clock;
    os_mqueue_index_t* prev_array;
    os_mqueue_index_t* next_array;
    os_mqueue_prio_t* prio_array;
    void* first_free;
#endif

    void* queue_addr;
    void* allocated_queue_addr;
    void* allocator;

#if defined(OS_INCLUDE_RTOS_PORT_MESSAGE_QUEUE)
    os_mqueue_port_data_t port;
#endif

    size_t queue_size_bytes;
    size_t allocated_queue_size_elements;

    os_mqueue_msg_size_t msg_size_bytes;
    os_mqueue_size_t msgs;

    os_mqueue_size_t count;
#if !defined(OS_INCLUDE_RTOS_PORT_MESSAGE_QUEUE)
    os_mqueue_index_t head;
#endif
  } os_mqueue_t;

#pragma GCC diagnostic pop

  // --------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  typedef struct os_evflags_attr_s
  {
    void* clock;
  } os_evflags_attr_t;

  typedef struct os_evflags_s
  {
    const char* name;
#if !defined(OS_INCLUDE_RTOS_PORT_EVENT_FLAGS)
    os_threads_waiting_list_t list;
    void* clock;
#endif

#if defined(OS_INCLUDE_RTOS_PORT_EVENT_FLAGS)
    os_evflags_port_data_t port_;
#endif

    os_flags_mask_t flags;
  } os_evflags_t;

#pragma GCC diagnostic pop

  // --------------------------------------------------------------------------

  typedef struct os_mail_queue_s
  {
    os_mempool_t pool;
    os_mqueue_t queue;
  } os_mail_queue_t;

  // --------------------------------------------------------------------------

  /**
   * @brief SysTick interrupt handler.
   * @ingroup cmsis-plus-rtos
   */
  void
  os_systick_handler (void);

  /**
   * @brief RTC interrupt handler.
   * @ingroup cmsis-plus-rtos
   */
  void
  os_rtc_handler (void);

// ----------------------------------------------------------------------------

#ifdef  __cplusplus
}
#endif

#endif /* CMSIS_PLUS_RTOS_OS_C_STRUCTS_H_ */
