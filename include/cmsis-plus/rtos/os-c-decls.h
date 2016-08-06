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

#include <cmsis-plus/os-versions.h>

/**
 * @brief Tell the world that CMSIS++ is in use.
 * @details
 * Macro to inform including files that CMSIS++ RTOS
 * definitions are available.
 */
#define OS_USE_CMSIS_PLUS

#include <cmsis-plus/os-app-config.h>
#include <cmsis-plus/rtos/port/os-decls.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// ----------------------------------------------------------------------------

#ifdef  __cplusplus
extern "C"
{
#endif

  // ==========================================================================

  typedef struct os_internal_double_list_links_s
  {
    void* prev;
    void* next;
  } os_internal_double_list_links_t;

  typedef os_internal_double_list_links_t os_internal_threads_waiting_list_t;

  typedef struct os_internal_thread_children_list_s
  {
    os_internal_double_list_links_t links;
  } os_internal_thread_children_list_t;

  typedef struct os_internal_waiting_thread_node_s
  {
    os_internal_double_list_links_t links;
    void* thread;
  } os_internal_waiting_thread_node_t;

  typedef struct os_internal_clock_timestamps_list_s
  {
    os_internal_double_list_links_t links;
  } os_internal_clock_timestamps_list_t;

  /**
   * @addtogroup cmsis-plus-rtos-c-core
   * @{
   */

  /**
   * @brief Type of values returned by RTOS functions.
   * @details
   * For error processing reasons, most CMSIS++ RTOS functions
   * return a numeric result, which, according to POSIX,
   * when the call was successful, must be `0`
   * (`os_ok`) or an error code defined in `<errno.h>` otherwise.
   *
   * @see os::rtos::result_t
   */
  typedef uint32_t os_result_t;

  /**
   * @brief Type of variables holding flags modes.
   * @details
   * An unsigned type used to hold the mode bits passed to
   * functions returning flags.
   *
   * Both thread event flags and generic event flags use this definition.
   *
   * @see os::rtos::flags::mode_t
   */
  typedef uint32_t os_flags_mode_t;

  /**
   * @brief Type of variables holding flags masks.
   * @details
   * An unsigned type large enough to store all the flags, usually
   * 32-bits wide.
   *
   * Both thread event flags and generic event flags use this definition.
   *
   * @see os::rtos::flags::mask_t
   */
  typedef uint32_t os_flags_mask_t;

  /**
   * @brief Bits used to specify the flags modes.
   *
   * @see os::rtos::flags::mode
   */
  enum
  {
    os_flags_mode_all = 1, //
    os_flags_mode_any = 2, //
    os_flags_mode_clear = 4, //
  };

  /**
   * @brief Special mask to represent any flag.
   */
#define os_flags_any 0

  /**
   * Special mask to represent all flags.
   */
#define os_flags_all 0xFFFFFFFF

  // --------------------------------------------------------------------------

  /**
   * @brief Type of variables holding scheduler state codes.
   * @details
   * Usually a boolean telling if the scheduler is
   * locked or not, but for recursive locks it might also be a
   * numeric counter.
   *
   * @see os::rtos::scheduler::state_t
   */
  typedef os_port_scheduler_state_t os_sched_state_t;

  /**
   * @brief Type of variables holding interrupts priority values.
   * @details
   * Usually an integer large enough to hold the CPU register
   * where the interrupt priorities are stored.
   *
   * Used to temporarily store the CPU register
   * during critical sections.
   *
   * @see os::rtos::interrupts::state_t
   */
  typedef os_port_irq_state_t os_irq_state_t;

  // --------------------------------------------------------------------------

  // Define clock types based on port definitions.

  /**
   * @brief Type of variables holding clock time stamps.
   * @details
   * A numeric type intended to store a clock timestamp, either in ticks
   * cycles or seconds.
   *
   * @see os::rtos::clock::timestamp_t
   */
  typedef os_port_clock_timestamp_t os_clock_timestamp_t;

  /**
   * @brief Type of variables holding clock durations.
   * @details
   * A numeric type intended to store a clock duration, either in ticks
   * cycles, or seconds.
   *
   * @see os::rtos::clock::duration_t
   */
  typedef os_port_clock_duration_t os_clock_duration_t;

  /**
   * @brief Type of variables holding clock offsets.
   * @details
   * A numeric type intended to store a clock offset
   * (difference to epoch), either in ticks
   * or in seconds.
   *
   * @see os::rtos::clock::duration_t
   */
  typedef os_port_clock_offset_t os_clock_offset_t;

  // --------------------------------------------------------------------------

  /**
   * @brief Generic iterator, implemented as a pointer.
   * @details
   * To simplify things, the C implementation of iterators
   * includes a single pointer to a C++ object instance. Internally,
   * the functions
   * used to iterate must cast this pointer properly, but this
   * should be transparent for the user.
   */
  typedef void* os_iterator_t;

  // --------------------------------------------------------------------------

  /**
   * @brief Type of variables holding context switches counters.
   *
   * @see os::rtos::statistics::counter_t
   */
  typedef uint64_t os_statistics_counter_t;

  /**
   * @brief Type of variables holding durations in CPU cycles.
   *
   * @see os::rtos::statistics::duration_t
   */
  typedef uint64_t os_statistics_duration_t;

  /**
   * @}
   */

  /**
   * @brief Internal event flags.
   *
   * @see os::rtos::internal::event_flags
   */
  typedef struct os_internal_evflags_s
  {
    os_flags_mask_t flags_mask;
  } os_internal_evflags_t;

  // ==========================================================================
#define OS_THREAD_PRIO_SHIFT   (4)

  /**
   * @addtogroup cmsis-plus-rtos-c-thread
   * @{
   */

  /**
   * @brief Thread priorities; intermediate values are also possible.
   *
   * @see os::rtos::thread::state
   */
  enum
  {
    // Ordered, with **none** as the first and **error** as the last.
    os_thread_priority_none = 0, // not defined
    os_thread_priority_idle = (1 << OS_THREAD_PRIO_SHIFT),
    os_thread_priority_lowest = (2 << OS_THREAD_PRIO_SHIFT), // lowest
    os_thread_priority_low = (2 << OS_THREAD_PRIO_SHIFT),
    os_thread_priority_below_normal = (4 << OS_THREAD_PRIO_SHIFT),
    os_thread_priority_normal = (6 << OS_THREAD_PRIO_SHIFT), // default
    os_thread_priority_above_normal = (8 << OS_THREAD_PRIO_SHIFT),
    os_thread_priority_high = (10 << OS_THREAD_PRIO_SHIFT),
    os_thread_priority_realtime = (12 << OS_THREAD_PRIO_SHIFT),
    os_thread_priority_highest = (((13 + 1) << OS_THREAD_PRIO_SHIFT) - 1),
    os_thread_priority_isr = (((14 + 1) << OS_THREAD_PRIO_SHIFT) - 1),
    os_thread_priority_error = (((15 + 1) << OS_THREAD_PRIO_SHIFT) - 1)
  };

  /**
   * @brief An enumeration with all possible thread states.
   *
   * @see os::rtos::thread::state
   */
  enum
  {
    /**
     * @brief Used to catch uninitialised threads.
     */
    os_thread_state_undefined = 0,
    /**
     * @brief Present in the READY list and competing for CPU.
     */
    os_thread_state_ready = 1,
    /**
     * @brief Has the CPU and runs.
     */
    os_thread_state_running = 2,
    /**
     * @brief Not present in the READY list, waiting for an event.
     */
    os_thread_state_suspended = 3,
    /**
     * @brief No longer usable, but resources not yet released.
     */
    os_thread_state_terminated = 4,
    /**
     * @brief Terminated and resources (like stack) released.
     */
    os_thread_state_destroyed = 5
  };

  /**
   * @brief Type of thread function arguments.
   * @details
   * Useful to cast other similar types
   * to silence possible compiler warnings.
   *
   * @see os::rtos::thread::func_args_t
   */
  typedef void* os_thread_func_args_t;

  /**
   * @brief Type of thread function.
   * @details
   * Useful to cast other similar types
   * to silence possible compiler warnings.
   *
   * @see os::rtos::thread::func_t
   */
  typedef void*
  (*os_thread_func_t) (os_thread_func_args_t args);

  /**
   * @brief Type of variables holding thread states.
   *
   * @see os::rtos::thread::state_t
   */
  typedef uint8_t os_thread_state_t;

  /**
   * @brief Type of variables holding thread priorities.
   * @details
   * A numeric type used to hold thread priorities, affecting the thread
   * behaviour, like scheduling and thread wakeup due to events;
   * usually an unsigned 8-bits type.
   *
   * Higher values represent higher priorities.
   *
   * @see os::rtos::thread::priority_t
   */
  typedef uint8_t os_thread_prio_t;

#if !defined(OS_INCLUDE_RTOS_CUSTOM_THREAD_USER_STORAGE) && !defined(__cplusplus)
  typedef struct
    {
      char dummy;
    }os_thread_user_storage_t;
#endif

  // --------------------------------------------------------------------------

  /**
   * @brief Type of variables holding stack words.
   * @details
   * A numeric type intended to store a stack word
   * as stored by push instructions.
   *
   * @see os::rtos::stack::element_t
   */
  typedef os_port_thread_stack_element_t os_thread_stack_element_t;

  /**
   * @brief Type of variables holding aligned stack elements.
   * @details
   * A numeric type intended to be used for stack allocations.
   *
   * @see os::rtos::stack::allocation_element_t
   */
  typedef os_port_thread_stack_allocation_element_t os_thread_stack_allocation_element_t;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  /**
   * @brief Thread stack.
   * @headerfile os-c-api.h <cmsis-plus/rtos/os-c-api.h>
   * @details
   * The members of this structure are hidden and should not
   * be accessed directly, but through associated functions.
   *
   * @see os::rtos::thread::stack
   */
  typedef struct os_thread_stack_s
  {
    /**
     * @cond ignore
     */

    void* stack_addr;
    size_t stack_size_bytes;

    /**
     * @endcond
     */

  } os_thread_stack_t;

  /**
   * @brief Thread context.
   * @headerfile os-c-api.h <cmsis-plus/rtos/os-c-api.h>
   * @details
   * The members of this structure are hidden and should not
   * be accessed directly, but through associated functions.
   *
   * @see os::rtos::thread::context
   */
  typedef struct os_thread_context_s
  {
    /**
     * @cond ignore
     */

    os_thread_stack_t stack;
#if !defined(OS_USE_RTOS_PORT_SCHEDULER)
    os_port_thread_context_t port;
#endif

    /**
     * @endcond
     */

  } os_thread_context_t;

#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES) \
  || defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)

  /**
   * @brief Thread statistics.
   * @headerfile os-c-api.h <cmsis-plus/rtos/os-c-api.h>
   * @details
   * The members of this structure are hidden and should not
   * be accessed directly, but through associated functions.
   *
   * @see os::rtos::thread::statistics
   */
  typedef struct os_thread_statistics_s
  {
    /**
     * @cond ignore
     */

#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)
    os_statistics_counter_t context_switches;
#endif /* defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES) */

#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)
    os_statistics_duration_t cpu_cycles;
#endif /* defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES) */

    /**
     * @endcond
     */

  } os_thread_statistics_t;

#endif

  /**
   * @brief Thread attributes.
   * @headerfile os-c-api.h <cmsis-plus/rtos/os-c-api.h>
   * @details
   * Initialise this structure with `os_thread_attr_init()`, and then
   * set any of the individual members directly.
   *
   * @see os::rtos::thread::attributes
   */
  typedef struct os_thread_attr_s
  {
    /**
     * @brief Address of the clock to use for timeouts.
     * @details
     * It may be `os_clock_get_sysclock()`, `os_clock_get_rtclock()`,
     * or any other user object derived from class `clock`.
     *
     * If `NULL`, the default is `sysclock`.
     */
    void* clock;

    /**
     * @brief Address of the user defined storage for the thread stack.
     * @details
     * If `NULL`, the default is to dynamically allocate the stack.
     */
    void* th_stack_address;

    /**
     * @brief Size of the user defined storage for the thread
     * stack, in bytes.
     * @details
     * If 0, the default is `os_thread_stack_get_default_size()`.
     *
     * A convenient and explicit variant to this attribute
     * is to call `os_thread_stack_set_default_size ()` just before
     * creating the thread. However mind setting this from different
     * threads at the same time.
     */
    size_t th_stack_size_bytes;

    /**
     * @brief Thread initial priority.
     * @details
     * If 0, the default is `os_thread_priority_normal`.
     *
     * A convenient and explicit variant to this attribute
     * is to call `os_thread_set_priority()` at the beginning of the thread
     * function.
     */
    os_thread_prio_t th_priority;

  } os_thread_attr_t;

  /**
   * @brief Thread object storage.
   * @headerfile os-c-api.h <cmsis-plus/rtos/os-c-api.h>
   * @details
   * This C structure has the same size as the C++ `os::rtos::thread` object
   * and must be initialised with `os_thread_create()`.
   *
   * Later on a pointer to it can be used both in C and C++
   * to refer to the thread object instance.
   *
   * The members of this structure are hidden and should not
   * be used directly, but only through specific functions.
   *
   * @see os::rtos::thread
   */
  typedef struct os_thread_s
  {
    /**
     * @cond ignore
     */

    void* vtbl;
    const char* name;
    os_internal_waiting_thread_node_t ready_node;
    int errno_; // Prevent the macro to expand (for example with a prefix).
    os_thread_func_t func;
    os_thread_func_args_t func_args;
    void* func_result_;
    void* parent;
    os_internal_double_list_links_t child_links;
    os_internal_thread_children_list_t children;
    os_internal_double_list_links_t mutexes;
    void* joiner;
    void* waiting_node;
    void* clock_node;
    void* clock;
    void* allocator;
    void* allocted_stack_address;
    size_t acquired_mutexes;
    size_t allocated_stack_size_elements;
    os_thread_state_t state;
    os_thread_prio_t prio_assigned;
    os_thread_prio_t prio_inherited;
    bool interrupted;
    os_internal_evflags_t event_flags;
    os_thread_user_storage_t user_storage; //

#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES) \
  || defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)
    os_thread_statistics_t statistics;
#endif /* defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES) */

#if defined(OS_USE_RTOS_PORT_SCHEDULER)
    os_thread_port_data_t port;
#endif
    os_thread_context_t context;

    /**
     * @endcond
     */

  } os_thread_t;

#pragma GCC diagnostic pop

  /**
   * @}
   */

  // ==========================================================================
  /**
   * @addtogroup cmsis-plus-rtos-c-clock
   * @{
   */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  /**
   * @brief Clock object storage.
   * @headerfile os-c-api.h <cmsis-plus/rtos/os-c-api.h>
   * @details
   * This C structure has the same size as the C++ `os::rtos::clock` object.
   *
   * The members of this structure are hidden and should not
   * be used directly, but only through specific functions.
   *
   * @see os::rtos::clock
   */
  typedef struct os_clock_s
  {
    /**
     * @cond ignore
     */

    void* vtbl;
    const char* name;
    os_internal_clock_timestamps_list_t steady_list;
    os_clock_duration_t sleep_count;
    os_clock_timestamp_t steady_count;

    /**
     * @endcond
     */

  } os_clock_t;

#pragma GCC diagnostic pop

  /**
   * @}
   */

  // ==========================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  typedef struct os_clock_node_s
  {
    void* next;
    void* prev;
    void* list;
    os_clock_timestamp_t timestamp;
    void* timer;
  } os_internal_clock_timer_node_t;

#pragma GCC diagnostic pop

  /**
   * @addtogroup cmsis-plus-rtos-c-timer
   * @{
   */

  /**
   * @brief An enumeration with the timer types.
   */
  enum
  {
    os_timer_once = 0, //
    os_timer_periodic = 1 //
  };

  /**
   * @brief Type of timer function arguments.
   * @details
   * Useful to cast other similar types
   * to silence possible compiler warnings.
   *
   * @see os::rtos::timer::func_args_t
   */
  typedef void* os_timer_func_args_t;

  /**
   * @brief Type of timer function.
   * @details
   * Useful to cast other similar types
   * to silence possible compiler warnings.
   *
   * @see os::rtos::timer::func_t
   */
  typedef void
  (*os_timer_func_t) (os_timer_func_args_t args);

  /**
   * @brief Type of variables holding timer types.
   *
   * @see os::rtos::timer::type_t
   */
  typedef uint8_t os_timer_type_t;

  /**
   * @brief Type of variables holding timer states.
   *
   * @see os::rtos::timer::state_t
   */
  typedef uint8_t os_timer_state_t;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  /**
   * @brief Timer attributes.
   * @headerfile os-c-api.h <cmsis-plus/rtos/os-c-api.h>
   * @details
   * Initialise this structure with `os_timer_attr_init()` and then
   * set any of the individual members directly.
   *
   * @see os::rtos::timer::attributes
   */
  typedef struct os_timer_attr_s
  {
    /**
     * @brief Pointer to clock object instance.
     */
    void* clock;

    /**
     * @brief Timer type.
     */
    os_timer_type_t tm_type;

  } os_timer_attr_t;

  /**
   * @brief Timer object storage.
   * @headerfile os-c-api.h <cmsis-plus/rtos/os-c-api.h>
   * @details
   * This C structure has the same size as the C++ `os::rtos::timer` object
   * and must be initialised with `os_timer_create()`.
   *
   * Later on a pointer to it can be used both in C and C++
   * to refer to the timer object instance.
   *
   * The members of this structure are hidden and should not
   * be used directly, but only through specific functions.
   *
   * @see os::rtos::timer
   */
  typedef struct os_timer_s
  {
    /**
     * @cond ignore
     */

    const char* name;
    os_timer_func_t func;
    os_timer_func_args_t func_args;
#if !defined(OS_USE_RTOS_PORT_TIMER)
    void* clock;
    os_internal_clock_timer_node_t clock_node;
    os_clock_duration_t period;
#endif
#if defined(OS_USE_RTOS_PORT_TIMER)
    os_timer_port_data_t port_;
#endif
    os_timer_type_t type;
    os_timer_state_t state;

    /**
     * @endcond
     */

  } os_timer_t;

#pragma GCC diagnostic pop

  /**
   * @}
   */

  // ==========================================================================
  typedef int16_t os_mutex_count_t;
  typedef uint8_t os_mutex_type_t;
  typedef uint8_t os_mutex_protocol_t;
  typedef uint8_t os_mutex_robustness_t;

  /**
   * @addtogroup cmsis-plus-rtos-c-mutex
   * @{
   */

  /**
   * @brief An enumeration with mutex protocols.
   *
   * @see os::rtos::mutex::protocol
   */
  enum
  {
    /**
     * @brief Priority and scheduling not affected by mutex ownership.
     */
    os_mutex_protocol_none = 0,

    /**
     * @brief Inherit priority from highest priority thread.
     */
    os_mutex_protocol_inherit = 1,

    /**
     * @brief Execute at the highest priority.
     */
    os_mutex_protocol_protect = 2,

    /**
     * @brief Default mutex protocol.
     */
    os_mutex_protocol_default = os_mutex_protocol_inherit,
  };

  /**
   * @brief An enumeration with mutex robustness.
   *
   * @see os::rtos::mutex::robustness
   */
  enum
  {
    /**
     * @brief Normal robustness.
     */
    os_mutex_robustness_stalled = 0,

    /**
     * @brief Enhanced robustness at thread termination.
     */
    os_mutex_robustness_robust = 1,

    /**
     * @brief Default mutex robustness.
     */
    os_mutex_robustness_default = os_mutex_robustness_stalled,
  };

  /**
   * @brief An enumeration with mutex types.
   *
   * @see os::rtos::mutex::type
   */
  enum
  {
    /**
     * @brief Normal mutex behaviour.
     */
    os_mutex_type_normal = 0,

    /**
     * @brief Check mutex behaviour.
     */
    os_mutex_type_errorcheck = 1,

    /**
     * @brief Recursive mutex behaviour.
     */
    os_mutex_type_recursive = 2,

    /**
     * @brief Default mutex type.
     */
    os_mutex_type_default = os_mutex_type_normal,
  };

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  /**
   * @brief Mutex attributes.
   * @headerfile os-c-api.h <cmsis-plus/rtos/os-c-api.h>
   * @details
   * Initialise this structure with `os_mutex_attr_init()` and then
   * set any of the individual members directly.
   *
   * @see os::rtos::mutex::attributes
   */
  typedef struct os_mutex_attr_s
  {
    /**
     * @brief Pointer to clock object instance.
     */
    void* clock;

    /**
     * @brief Mutex priority ceiling.
     */
    os_thread_prio_t mx_priority_ceiling;

    /**
     * @brief Mutex protocol.
     */
    os_mutex_protocol_t mx_protocol;

    /**
     * @brief Mutex robustness.
     */
    os_mutex_robustness_t mx_robustness;

    /**
     * @brief Mutex type.
     */
    os_mutex_type_t mx_type;

    /**
     * @brief Recursive mutex max count.
     */
    os_mutex_count_t mx_max_count;

  } os_mutex_attr_t;

  /**
   * @brief Mutex object storage.
   * @headerfile os-c-api.h <cmsis-plus/rtos/os-c-api.h>
   * @details
   * This C structure has the same size as the C++ `os::rtos::mutex` object
   * and must be initialised with `os_mutex_create()`.
   *
   * Later on a pointer to it can be used both in C and C++
   * to refer to the mutex object instance.
   *
   * The members of this structure are hidden and should not
   * be used directly, but only through specific functions.
   *
   * @see os::rtos::mutex
   */
  typedef struct os_mutex_s
  {
    /**
     * @cond ignore
     */

    const char* name;
    void* owner;
#if !defined(OS_USE_RTOS_PORT_MUTEX)
    os_internal_threads_waiting_list_t list;
    void* clock;
#endif
    os_internal_double_list_links_t owner_links;
#if defined(OS_USE_RTOS_PORT_MUTEX)
    os_mutex_port_data_t port;
#endif
    os_mutex_count_t count;
    os_thread_prio_t initial_prio_ceiling;
    os_thread_prio_t prio_ceiling;
    os_thread_prio_t bosted_prio;
    bool owner_dead;
    bool consistent;
    bool recoverable;
    os_mutex_type_t type;
    os_mutex_protocol_t protocol;
    os_mutex_robustness_t robustness;
    os_mutex_count_t max_count;

    /**
     * @endcond
     */

  } os_mutex_t;

#pragma GCC diagnostic pop

  /**
   * @}
   */

  // ==========================================================================
  /**
   * @addtogroup cmsis-plus-rtos-c-condvar
   * @{
   */

  /**
   * @brief Condition variable attributes.
   * @headerfile os-c-api.h <cmsis-plus/rtos/os-c-api.h>
   * @details
   * Initialise this structure with `os_condvar_attr_init()` and then
   * set any of the individual members directly.
   *
   * @see os::rtos::condition_variable::attributes
   */
  typedef struct os_condvar_attr_s
  {
    /**
     * @brief Pointer to clock object instance.
     */
    void* clock;

  } os_condvar_attr_t;

  /**
   * @brief Condition variable object storage.
   * @headerfile os-c-api.h <cmsis-plus/rtos/os-c-api.h>
   * @details
   * This C structure has the same size as the C++
   * @ref os::rtos::condition_variable
   * object and must be initialised with os_condvar_create().
   *
   * Later on a pointer to it can be used both in C and C++
   * to refer to the timer object instance.
   *
   * The members of this structure are hidden and should not
   * be used directly, but only through specific functions.
   *
   * @see os::rtos::condition_variable
   */
  typedef struct os_condvar_s
  {
    /**
     * @cond ignore
     */

    const char* name;
#if !defined(OS_USE_RTOS_PORT_CONDITION_VARIABLE)
    os_internal_threads_waiting_list_t list;
    // void* clock;
#endif

    /**
     * @endcond
     */

  } os_condvar_t;

  /**
   * @}
   */

  // ==========================================================================
  /**
   * @addtogroup cmsis-plus-rtos-c-semaphore
   * @{
   */

  /**
   * @brief Type of variables holding semaphore counts.
   *
   * @see os::rtos::semaphore::count_t
   */
  typedef int16_t os_semaphore_count_t;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  /**
   * @brief Semaphore attributes.
   * @headerfile os-c-api.h <cmsis-plus/rtos/os-c-api.h>
   * @details
   * Initialise this structure with `os_semaphore_attr_init()` and then
   * set any of the individual members directly.
   *
   * @see os::rtos::semaphore::attributes
   */
  typedef struct os_semaphore_attr_s
  {
    /**
     * @brief Pointer to clock object instance.
     */
    void* clock;

    /**
     * @brief Semaphore max count value.
     */
    os_semaphore_count_t sm_max_value;

    /**
     * @brief Semaphore initial count value.
     */
    os_semaphore_count_t sm_initial_value;

  } os_semaphore_attr_t;

  /**
   * @brief Semaphore object storage.
   * @headerfile os-c-api.h <cmsis-plus/rtos/os-c-api.h>
   * @details
   * This C structure has the same size as the C++ `os::rtos::semaphore`
   * object and must be initialised with `os_semaphore_create()`.
   *
   * Later on a pointer to it can be used both in C and C++
   * to refer to the semaphore object instance.
   *
   * The members of this structure are hidden and should not
   * be used directly, but only through specific functions.
   *
   * @see os::rtos::semaphore
   */
  typedef struct os_semaphore_s
  {
    /**
     * @cond ignore
     */

    const char* name;
#if !defined(OS_USE_RTOS_PORT_SEMAPHORE)
    os_internal_threads_waiting_list_t list;
    void* clock;
#endif
#if defined(OS_USE_RTOS_PORT_SEMAPHORE)
    os_semaphore_port_data_t port;
#endif
    os_semaphore_count_t initial_count;
    os_semaphore_count_t count;
    os_semaphore_count_t max_count;

    /**
     * @endcond
     */

  } os_semaphore_t;

#pragma GCC diagnostic pop

  /**
   * @}
   */

  // ==========================================================================
  typedef uint16_t os_mempool_size_t;

  /**
   * @addtogroup cmsis-plus-rtos-c-mempool
   * @{
   */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  /**
   * @brief Memory pool attributes.
   * @headerfile os-c-api.h <cmsis-plus/rtos/os-c-api.h>
   * @details
   * Initialise this structure with `os_mempool_attr_init()` and then
   * set any of the individual members directly.
   *
   * @see os::rtos::memory_pool::attributes
   */
  typedef struct os_mempool_attr_s
  {
    /**
     * @brief Pointer to clock object instance.
     */
    void* clock;

    /**
     * @brief Pointer to user provided memory pool area.
     */
    void* mp_pool_address;

    /**
     * @brief Size of user provided memory pool area, in bytes.
     */
    size_t mp_pool_size_bytes;

  } os_mempool_attr_t;

  /**
   * @brief Memory pool object storage.
   * @headerfile os-c-api.h <cmsis-plus/rtos/os-c-api.h>
   * @details
   * This C structure has the same size as the C++ `os::rtos::memory_pool`
   * object and must be initialised with `os_mempool_create()`.
   *
   * Later on a pointer to it can be used both in C and C++
   * to refer to the memory pool object instance.
   *
   * The members of this structure are hidden and should not
   * be used directly, but only through specific functions.
   *
   * @see os::rtos::memory_pool
   */
  typedef struct os_mempool_s
  {
    /**
     * @cond ignore
     */

    void* vtbl;
    const char* name;
#if !defined(OS_USE_RTOS_PORT_MEMORY_POOL)
    os_internal_threads_waiting_list_t list;
    void* clock;
#endif
    void* pool_addr;
    void* allocated_pool_addr;
    void* allocator;
#if defined(OS_USE_RTOS_PORT_MEMORY_POOL)
    os_mempool_port_data_t port;
#endif
    size_t pool_size_bytes;
    size_t allocated_pool_size_elements_;
    os_mempool_size_t blocks;
    os_mempool_size_t block_size_bytes;
    os_mempool_size_t count;
    void* first;

    /**
     * @endcond
     */

  } os_mempool_t;

#pragma GCC diagnostic pop

  /**
   * @}
   */

  // ==========================================================================
#if defined(OS_BOOL_RTOS_MESSAGE_QUEUE_SIZE_16BITS)
  typedef uint16_t os_mqueue_size_t;
#else
  typedef uint8_t os_mqueue_size_t;
#endif

  typedef uint16_t os_mqueue_msg_size_t;
  typedef os_mqueue_size_t os_mqueue_index_t;

  /**
   * @addtogroup cmsis-plus-rtos-c-mqueue
   * @{
   */

  /**
   * @brief Type of variables holding message queue priorities.
   *
   * @see os::rtos::message_queue::priority_t
   */
  typedef uint8_t os_mqueue_prio_t;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  /**
   * @brief Message queue attributes.
   * @headerfile os-c-api.h <cmsis-plus/rtos/os-c-api.h>
   * @details
   * Initialise this structure with `os_mqueue_attr_init()` and then
   * set any of the individual members directly.
   *
   * @see os::rtos::message_queue::attributes
   */
  typedef struct os_mqueue_attr_s
  {
    /**
     * @brief Pointer to clock object instance.
     */
    void* clock;

    /**
     * @brief Pointer to user provided message queue area.
     */
    void* mq_queue_addr;

    /**
     * @brief Size of user provided message queue area, in bytes.
     */
    size_t mq_queue_size_bytes;

  } os_mqueue_attr_t;

  /**
   * @brief Message queue object storage.
   * @headerfile os-c-api.h <cmsis-plus/rtos/os-c-api.h>
   * @details
   * This C structure has the same size as the C++ `os::rtos::message_queue`
   * object and must be initialised with `os_mqueue_create()`.
   *
   * Later on a pointer to it can be used both in C and C++
   * to refer to the message queue object instance.
   *
   * The members of this structure are hidden and should not
   * be used directly, but only through specific functions.
   *
   * @see os::rtos::message_queue
   */
  typedef struct os_mqueue_s
  {
    /**
     * @cond ignore
     */

    void* vtbl;
    const char* name;
#if !defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE)
    os_internal_threads_waiting_list_t send_list;
    os_internal_threads_waiting_list_t receive_list;
    void* clock;
    os_mqueue_index_t* prev_array;
    os_mqueue_index_t* next_array;
    os_mqueue_prio_t* prio_array;
    void* first_free;
#endif

    void* queue_addr;
    void* allocated_queue_addr;
    void* allocator;

#if defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE)
    os_mqueue_port_data_t port;
#endif

    size_t queue_size_bytes;
    size_t allocated_queue_size_elements;

    os_mqueue_msg_size_t msg_size_bytes;
    os_mqueue_size_t msgs;

    os_mqueue_size_t count;
#if !defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE)
    os_mqueue_index_t head;
#endif

    /**
     * @endcond
     */

  } os_mqueue_t;

#pragma GCC diagnostic pop

  /**
   * @}
   */

  // ==========================================================================
  /**
   * @addtogroup cmsis-plus-rtos-c-evflag
   * @{
   */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  /**
   * @brief Event flags attributes.
   * @headerfile os-c-api.h <cmsis-plus/rtos/os-c-api.h>
   * @details
   * Initialise this structure with `os_evflags_attr_init()` and then
   * set any of the individual members directly.
   *
   * @see os::rtos::event_flags::attributes
   */
  typedef struct os_evflags_attr_s
  {
    /**
     * @brief Pointer to clock object instance.
     */
    void* clock;

  } os_evflags_attr_t;

  /**
   * @brief Event flags object storage.
   * @headerfile os-c-api.h <cmsis-plus/rtos/os-c-api.h>
   * @details
   * This C structure has the same size as the C++ `os::rtos::event_flags`
   * object and must be initialised with `os_evflags_create()`.
   *
   * Later on a pointer to it can be used both in C and C++
   * to refer to the event flags object instance.
   *
   * The members of this structure are hidden and should not
   * be used directly, but only through specific functions.
   *
   * @see os::rtos::event_flags
   */
  typedef struct os_evflags_s
  {
    /**
     * @cond ignore
     */

    const char* name;
#if !defined(OS_USE_RTOS_PORT_EVENT_FLAGS)
    os_internal_threads_waiting_list_t list;
    void* clock;
#endif

#if defined(OS_USE_RTOS_PORT_EVENT_FLAGS)
    os_evflags_port_data_t port_;
#endif

    os_internal_evflags_t flags;

    /**
     * @endcond
     */

  } os_evflags_t;

#pragma GCC diagnostic pop

  /**
   * @}
   */

  // ==========================================================================
  /**
   * @addtogroup cmsis-plus-rtos-c-clock
   * @{
   */

  /**
   * @name Clock handlers
   * @{
   */

  /**
   * @brief SysTick interrupt handler.
   */
  void
  os_systick_handler (void);

  /**
   * @brief RTC interrupt handler.
   */
  void
  os_rtc_handler (void);

/**
 * @}
 */

/**
 * @}
 */

// ============================================================================
#ifdef  __cplusplus
}
#endif

#endif /* CMSIS_PLUS_RTOS_OS_C_STRUCTS_H_ */
