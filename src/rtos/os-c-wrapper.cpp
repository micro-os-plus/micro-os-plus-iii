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

// ----------------------------------------------------------------------------

using namespace os;
using namespace os::rtos;

// ----------------------------------------------------------------------------

// Validate C typedefs sizes & alignment.

static_assert(sizeof(os_result_t) == sizeof(result_t), "adjust size of os_result_t");
static_assert(alignof(os_result_t) == alignof(result_t), "adjust align of os_result_t");

static_assert(sizeof(os_flags_mode_t) == sizeof(flags::mode_t), "adjust size of os_flags_mode_t");
static_assert(alignof(os_flags_mode_t) == alignof(flags::mode_t), "adjust align of os_flags_mode_t");

static_assert(sizeof(os_flags_mask_t) == sizeof(flags::mask_t), "adjust size of os_flags_mask_t");
static_assert(alignof(os_flags_mask_t) == alignof(flags::mask_t), "adjust align of os_flags_mask_t");

static_assert(sizeof(os_sched_state_t) == sizeof(scheduler::state_t), "adjust size of os_sched_state_t");
static_assert(alignof(os_sched_state_t) == alignof(scheduler::state_t), "adjust align of os_sched_state_t");

static_assert(sizeof(os_irq_state_t) == sizeof(interrupts::state_t), "adjust size of os_irq_state_t");
static_assert(alignof(os_irq_state_t) == alignof(interrupts::state_t), "adjust align of os_irq_state_t");

static_assert(sizeof(os_clock_timestamp_t) == sizeof(clock::timestamp_t), "adjust size of os_port_clock_timestamp_t");
static_assert(alignof(os_clock_timestamp_t) == alignof(clock::timestamp_t), "adjust align of os_port_clock_timestamp_t");

static_assert(sizeof(os_clock_duration_t) == sizeof(clock::duration_t), "adjust size of os_port_clock_duration_t");
static_assert(alignof(os_clock_duration_t) == alignof(clock::duration_t), "adjust align of os_port_clock_duration_t");

static_assert(sizeof(os_clock_offset_t) == sizeof(clock::offset_t), "adjust size of os_port_clock_offset_t");
static_assert(alignof(os_clock_offset_t) == alignof(clock::offset_t), "adjust align of os_port_clock_offset_t");

static_assert(sizeof(os_statistics_counter_t) == sizeof(statistics::counter_t), "adjust size of os_statistics_counter_t");
static_assert(alignof(os_statistics_counter_t) == alignof(statistics::counter_t), "adjust align of os_statistics_counter_t");

static_assert(sizeof(os_statistics_duration_t) == sizeof(statistics::duration_t), "adjust size of os_statistics_duration_t");
static_assert(alignof(os_statistics_duration_t) == alignof(statistics::duration_t), "adjust align of os_statistics_duration_t");

static_assert(sizeof(os_thread_func_args_t) == sizeof(thread::func_args_t), "adjust size of os_thread_func_args_t");
static_assert(alignof(os_thread_func_args_t) == alignof(thread::func_args_t), "adjust align of os_thread_func_args_t");

static_assert(sizeof(os_thread_func_t) == sizeof(thread::func_t), "adjust size of os_thread_func_t");
static_assert(alignof(os_thread_func_t) == alignof(thread::func_t), "adjust align of os_thread_func_t");

static_assert(sizeof(os_thread_state_t) == sizeof(thread::state_t), "adjust size of os_thread_state_t");
static_assert(alignof(os_thread_state_t) == alignof(thread::state_t), "adjust align of os_thread_state_t");

static_assert(sizeof(os_thread_prio_t) == sizeof(thread::priority_t), "adjust size of os_thread_prio_t");
static_assert(alignof(os_thread_prio_t) == alignof(thread::priority_t), "adjust align of os_thread_prio_t");

static_assert(sizeof(os_timer_func_args_t) == sizeof(timer::func_args_t), "adjust size of os_timer_func_args_t");
static_assert(alignof(os_timer_func_args_t) == alignof(timer::func_args_t), "adjust align of os_timer_func_args_t");

static_assert(sizeof(os_timer_func_t) == sizeof(timer::func_t), "adjust size of os_timer_func_t");
static_assert(alignof(os_timer_func_t) == alignof(timer::func_t), "adjust align of os_timer_func_t");

static_assert(sizeof(os_timer_type_t) == sizeof(timer::type_t), "adjust size of os_timer_type_t");
static_assert(alignof(os_timer_type_t) == alignof(timer::type_t), "adjust align of os_timer_type_t");

static_assert(sizeof(os_timer_state_t) == sizeof(timer::state_t), "adjust size of os_timer_state_t");
static_assert(alignof(os_timer_state_t) == alignof(timer::state_t), "adjust align of os_timer_state_t");

static_assert(sizeof(os_mutex_count_t) == sizeof(mutex::count_t), "adjust size of os_mutex_count_t");
static_assert(alignof(os_mutex_count_t) == alignof(mutex::count_t), "adjust align of os_mutex_count_t");

static_assert(sizeof(os_mutex_type_t) == sizeof(mutex::type_t), "adjust size of os_mutex_type_t");
static_assert(alignof(os_mutex_type_t) == alignof(mutex::type_t), "adjust align of os_mutex_type_t");

static_assert(sizeof(os_mutex_protocol_t) == sizeof(mutex::protocol_t), "adjust size of os_mutex_protocol_t");
static_assert(alignof(os_mutex_protocol_t) == alignof(mutex::protocol_t), "adjust align of os_mutex_protocol_t");

static_assert(sizeof(os_mutex_robustness_t) == sizeof(mutex::robustness_t), "adjust size of os_mutex_robustness_t");
static_assert(alignof(os_mutex_robustness_t) == alignof(mutex::robustness_t), "adjust align of os_mutex_robustness_t");

static_assert(sizeof(os_semaphore_count_t) == sizeof(semaphore::count_t), "adjust size of os_semaphore_count_t");
static_assert(alignof(os_semaphore_count_t) == alignof(semaphore::count_t), "adjust align of os_semaphore_count_t");

static_assert(sizeof(os_mempool_size_t) == sizeof(memory_pool::size_t), "adjust size of os_mempool_size_t");
static_assert(alignof(os_mempool_size_t) == alignof(memory_pool::size_t), "adjust align of os_mempool_size_t");

static_assert(sizeof(os_mqueue_size_t) == sizeof(message_queue::size_t), "adjust size of os_mqueue_size_t");
static_assert(alignof(os_mqueue_size_t) == alignof(message_queue::size_t), "adjust align of os_mqueue_size_t");

static_assert(sizeof(os_mqueue_msg_size_t) == sizeof(message_queue::msg_size_t), "adjust size of os_mqueue_msg_size_t");
static_assert(alignof(os_mqueue_msg_size_t) == alignof(message_queue::msg_size_t), "adjust align of os_mqueue_msg_size_t");

static_assert(sizeof(os_mqueue_index_t) == sizeof(message_queue::index_t), "adjust size of os_mqueue_index_t");
static_assert(alignof(os_mqueue_index_t) == alignof(message_queue::index_t), "adjust align of os_mqueue_index_t");

static_assert(sizeof(os_mqueue_prio_t) == sizeof(message_queue::priority_t), "adjust size of os_mqueue_prio_t");
static_assert(alignof(os_mqueue_prio_t) == alignof(message_queue::priority_t), "adjust align of os_mqueue_prio_t");

// ----------------------------------------------------------------------------

// Validate C enumeration values

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wenum-compare"

static_assert(os_thread_priority_idle == thread::priority::idle, "adjust os_thread_priority_idle");
static_assert(os_thread_priority_low == thread::priority::low, "adjust os_thread_priority_low");
static_assert(os_thread_priority_below_normal == thread::priority::below_normal, "adjust os_thread_priority_below_normal");
static_assert(os_thread_priority_normal == thread::priority::normal, "adjust os_thread_priority_normal");
static_assert(os_thread_priority_above_normal == thread::priority::above_normal, "adjust os_thread_priority_above_normal");
static_assert(os_thread_priority_high == thread::priority::high, "adjust os_thread_priority_high");
static_assert(os_thread_priority_realtime == thread::priority::realtime, "adjust os_thread_priority_realtime");
static_assert(os_thread_priority_error == thread::priority::error, "adjust os_thread_priority_error");

static_assert(os_flags_mode_all == flags::mode::all, "adjust os_flags_mode_all");
static_assert(os_flags_mode_any == flags::mode::any, "adjust os_flags_mode_any");
static_assert(os_flags_mode_clear == flags::mode::clear, "adjust os_flags_mode_clear");

static_assert(os_thread_state_undefined == thread::state::undefined, "adjust os_thread_state_undefined");
static_assert(os_thread_state_ready == thread::state::ready, "adjust os_thread_state_ready");
static_assert(os_thread_state_running == thread::state::running, "adjust os_thread_state_running");
static_assert(os_thread_state_suspended == thread::state::suspended, "adjust os_thread_state_suspended");
static_assert(os_thread_state_terminated == thread::state::terminated, "adjust os_thread_state_terminated");
static_assert(os_thread_state_destroyed == thread::state::destroyed, "adjust os_thread_state_destroyed");

static_assert(os_timer_once == timer::run::once, "adjust os_timer_once");
static_assert(os_timer_periodic == timer::run::periodic, "adjust os_timer_periodic");

static_assert(os_mutex_protocol_none == mutex::protocol::none, "adjust os_mutex_protocol_none");
static_assert(os_mutex_protocol_inherit == mutex::protocol::inherit, "adjust os_mutex_protocol_inherit");
static_assert(os_mutex_protocol_protect == mutex::protocol::protect, "adjust os_mutex_protocol_protect");

static_assert(os_mutex_robustness_stalled == mutex::robustness::stalled, "adjust os_mutex_robustness_stalled");
static_assert(os_mutex_robustness_robust == mutex::robustness::robust, "adjust os_mutex_robustness_robust");

static_assert(os_mutex_type_normal == mutex::type::normal, "adjust os_mutex_type_normal");
static_assert(os_mutex_type_errorcheck == mutex::type::errorcheck, "adjust os_mutex_type_errorcheck");
static_assert(os_mutex_type_recursive == mutex::type::recursive, "adjust os_mutex_type_recursive");
static_assert(os_mutex_type_default == mutex::type::default_, "adjust os_mutex_type_default");

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

// Validate C structs sizes (should match the C++ objects sizes).
// Validate offset of individual members (if needed, validate member size).

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"

static_assert(sizeof(rtos::clock) == sizeof(os_clock_t), "adjust os_clock_t size");

static_assert(sizeof(rtos::thread) == sizeof(os_thread_t), "adjust os_thread_t size");
static_assert(sizeof(rtos::thread::attributes) == sizeof(os_thread_attr_t), "adjust os_thread_attr_t size");
static_assert(offsetof(rtos::thread::attributes, th_stack_address) == offsetof(os_thread_attr_t, th_stack_address), "adjust os_thread_attr_t members");
static_assert(offsetof(rtos::thread::attributes, th_stack_size_bytes) == offsetof(os_thread_attr_t, th_stack_size_bytes), "adjust os_thread_attr_t members");
static_assert(offsetof(rtos::thread::attributes, th_priority) == offsetof(os_thread_attr_t, th_priority), "adjust os_thread_attr_t members");

static_assert(sizeof(rtos::timer) == sizeof(os_timer_t), "adjust size of os_timer_t");
static_assert(sizeof(rtos::timer::attributes) == sizeof(os_timer_attr_t), "adjust size of os_timer_attr_t");
static_assert(offsetof(rtos::timer::attributes, tm_type) == offsetof(os_timer_attr_t, tm_type), "adjust os_timer_attr_t members");

static_assert(sizeof(rtos::mutex) == sizeof(os_mutex_t), "adjust size of os_mutex_t");
static_assert(sizeof(rtos::mutex::attributes) == sizeof(os_mutex_attr_t), "adjust size of os_mutex_attr_t");
static_assert(offsetof(rtos::mutex::attributes, mx_priority_ceiling) == offsetof(os_mutex_attr_t, mx_priority_ceiling), "adjust os_mutex_attr_t members");
static_assert(offsetof(rtos::mutex::attributes, mx_protocol) == offsetof(os_mutex_attr_t, mx_protocol), "adjust os_mutex_attr_t members");
static_assert(offsetof(rtos::mutex::attributes, mx_robustness) == offsetof(os_mutex_attr_t, mx_robustness), "adjust os_mutex_attr_t members");
static_assert(offsetof(rtos::mutex::attributes, mx_type) == offsetof(os_mutex_attr_t, mx_type), "adjust os_mutex_attr_t members");
static_assert(offsetof(rtos::mutex::attributes, mx_max_count) == offsetof(os_mutex_attr_t, mx_max_count), "adjust os_mutex_attr_t members");

static_assert(sizeof(rtos::condition_variable) == sizeof(os_condvar_t), "adjust size of os_condvar_t");
static_assert(sizeof(rtos::condition_variable::attributes) == sizeof(os_condvar_attr_t), "adjust size of os_condvar_attr_t");

static_assert(sizeof(rtos::semaphore) == sizeof(os_semaphore_t), "adjust size of os_semaphore_t");
static_assert(sizeof(rtos::semaphore::attributes) == sizeof(os_semaphore_attr_t), "adjust size of os_semaphore_attr_t");
static_assert(offsetof(rtos::semaphore::attributes, sm_initial_value) == offsetof(os_semaphore_attr_t, sm_initial_value), "adjust os_semaphore_attr_t members");
static_assert(offsetof(rtos::semaphore::attributes, sm_max_value) == offsetof(os_semaphore_attr_t, sm_max_value), "adjust os_semaphore_attr_t members");

static_assert(sizeof(rtos::memory_pool) == sizeof(os_mempool_t), "adjust size of os_mempool_t");
static_assert(sizeof(rtos::memory_pool::attributes) == sizeof(os_mempool_attr_t), "adjust size of os_mempool_attr_t");
static_assert(offsetof(rtos::memory_pool::attributes, mp_pool_address) == offsetof(os_mempool_attr_t, mp_pool_address), "adjust os_mempool_attr_t members");
static_assert(offsetof(rtos::memory_pool::attributes, mp_pool_size_bytes) == offsetof(os_mempool_attr_t, mp_pool_size_bytes), "adjust os_mempool_attr_t members");

static_assert(sizeof(rtos::message_queue) == sizeof(os_mqueue_t), "adjust size of os_mqueue_t");
static_assert(sizeof(rtos::message_queue::attributes) == sizeof(os_mqueue_attr_t), "adjust size of os_mqueue_attr_t");
static_assert(offsetof(rtos::message_queue::attributes, mq_queue_address) == offsetof(os_mqueue_attr_t, mq_queue_addr), "adjust os_mqueue_attr_t members");
static_assert(offsetof(rtos::message_queue::attributes, mq_queue_size_bytes) == offsetof(os_mqueue_attr_t, mq_queue_size_bytes), "adjust os_mqueue_attr_t members");

static_assert(sizeof(rtos::event_flags) == sizeof(os_evflags_t), "adjust size of os_evflags_t");
static_assert(sizeof(rtos::event_flags::attributes) == sizeof(os_evflags_attr_t), "adjust size of os_evflags_attr_t");

static_assert(sizeof(class thread::stack) == sizeof(os_thread_stack_t), "adjust size of os_thread_stack_t");
static_assert(sizeof(class thread::context) == sizeof(os_thread_context_t), "adjust size of os_thread_context_t");

#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES) \
  || defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)
static_assert(sizeof(class thread::statistics) == sizeof(os_thread_statistics_t), "adjust size of os_thread_statistics_t");
#endif

static_assert(sizeof(internal::timer_node) == sizeof(os_internal_clock_timer_node_t), "adjust size of os_internal_clock_timer_node_t");

#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::scheduler::initialize()
 */
os_result_t
os_sched_initialize (void)
{
  return (os_result_t) scheduler::initialize ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::scheduler::start()
 */
void
os_sched_start (void)
{
  scheduler::start ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::scheduler::started()
 */
bool
os_sched_is_started (void)
{
  return scheduler::started ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::scheduler::lock()
 */
os_sched_state_t
os_sched_lock (void)
{
  return scheduler::lock ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::scheduler::unlock()
 */
os_sched_state_t
os_sched_unlock (void)
{
  return scheduler::unlock ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::scheduler::lock()
 */
os_sched_state_t
os_sched_set_locked (os_sched_state_t state)
{
  return scheduler::locked (state);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::scheduler::locked()
 */
bool
os_sched_is_locked (void)
{
  return scheduler::locked ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::scheduler::preemptive()
 */
bool
os_sched_is_preemptive (void)
{
  return scheduler::preemptive ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::scheduler::preemptive(bool)
 */
bool
os_sched_set_preemptive (bool state)
{
  return scheduler::preemptive (state);
}

#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::scheduler::statistics::context_switches()
 */
os_statistics_counter_t
os_sched_stat_get_context_switches (void)
{
  return static_cast<os_statistics_counter_t> (scheduler::statistics::context_switches ());
}

#endif /* defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES) */

#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::scheduler::statistics::cpu_cycles()
 */
os_statistics_duration_t
os_sched_stat_get_cpu_cycles (void)
{
  return static_cast<os_statistics_duration_t> (scheduler::statistics::cpu_cycles ());
}

#endif /* defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES) */

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::interrupts::in_handler_mode()
 */
bool
os_irq_in_handler_mode (void)
{
  return interrupts::in_handler_mode ();
}

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::interrupts::critical_section::enter()
 */
os_irq_state_t
os_irq_critical_enter (void)
{
  return interrupts::critical_section::enter ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::interrupts::critical_section::exit()
 */
void
os_irq_critical_exit (os_irq_state_t state)
{
  interrupts::critical_section::exit (state);
}

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::interrupts::uncritical_section::enter()
 */
os_irq_state_t
os_irq_uncritical_enter (void)
{
  return interrupts::uncritical_section::enter ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::interrupts::uncritical_section::exit()
 */
void
os_irq_uncritical_exit (os_irq_state_t state)
{
  interrupts::uncritical_section::exit (state);
}

#if defined(OS_HAS_INTERRUPTS_STACK) || defined(__DOXYGEN__)

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::interrupts::stack()
 */

os_thread_stack_t*
os_irq_get_stack (void)
{
  return reinterpret_cast<os_thread_stack_t*> (rtos::interrupts::stack ());
}

#endif

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::this_thread::thread()
 */
os_thread_t*
os_this_thread (void)
{
  return (os_thread_t*) &this_thread::thread ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::this_thread::suspend()
 */
void
os_this_thread_suspend (void)
{
  return this_thread::suspend ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::this_thread::exit()
 */
void
os_this_thread_exit (void* exit_ptr)
{
  this_thread::exit (exit_ptr);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::this_thread::flags_wait()
 */
os_result_t
os_this_thread_flags_wait (os_flags_mask_t mask, os_flags_mask_t* oflags,
                           os_flags_mode_t mode)
{
  return (os_result_t) this_thread::flags_wait (mask, oflags, mode);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::this_thread::flags_try_wait()
 */
os_result_t
os_this_thread_flags_try_wait (os_flags_mask_t mask, os_flags_mask_t* oflags,
                               os_flags_mode_t mode)
{
  return (os_result_t) this_thread::flags_try_wait (mask, oflags, mode);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::this_thread::flags_timed_wait()
 */
os_result_t
os_this_thread_flags_timed_wait (os_flags_mask_t mask,
                                 os_clock_duration_t timeout,
                                 os_flags_mask_t* oflags, os_flags_mode_t mode)
{
  return (os_result_t) this_thread::flags_timed_wait (mask, timeout, oflags,
                                                      mode);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::this_thread::flags_clear()
 */
os_result_t
os_this_thread_flags_clear (os_flags_mask_t mask, os_flags_mask_t* oflags)
{
  return (os_result_t) this_thread::flags_clear (mask, oflags);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::this_thread::flags_get()
 */
os_flags_mask_t
os_this_thread_flags_get (os_flags_mask_t mask, os_flags_mode_t mode)
{
  return (os_flags_mask_t) this_thread::flags_get (mask, mode);
}

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread::attributes
 */
void
os_thread_attr_init (os_thread_attr_t* attr)
{
  assert(attr != nullptr);
  new (attr) thread::attributes ();
}

/**
 * @details
 *
 * @note Must be paired with `os_thread_destruct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread
 */
void
os_thread_construct (os_thread_t* thread, const char* name,
                     os_thread_func_t func, const os_thread_func_args_t args,
                     const os_thread_attr_t* attr)
{
  assert(thread != nullptr);
  if (attr == nullptr)
    {
      attr = (const os_thread_attr_t*) &thread::initializer;
    }
  new (thread) rtos::thread (name, (thread::func_t) func,
                             (thread::func_args_t) args,
                             (thread::attributes&) *attr);
}

/**
 * @details
 *
 * @note Must be paired with `os_thread_construct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread
 */
void
os_thread_destruct (os_thread_t* thread)
{
  assert(thread != nullptr);
  (reinterpret_cast<rtos::thread&> (*thread)).~thread ();
}

/**
 * @details
 *
 * Dynamically allocate the thread object instance using the RTOS
 * system allocator and construct it.
 *
 * @note Equivalent of C++ `new thread(...)`.
 * @note Must be paired with `os_thread_delete()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread
 */
os_thread_t*
os_thread_new (const char* name, os_thread_func_t func,
               const os_thread_func_args_t args, const os_thread_attr_t* attr)
{
  if (attr == nullptr)
    {
      attr = (const os_thread_attr_t*) &thread::initializer;
    }
  return reinterpret_cast<os_thread_t*> (new rtos::thread (
      name, (thread::func_t) func, (thread::func_args_t) args,
      (thread::attributes&) *attr));
}

/**
 * @details
 *
 * Destruct the thread and deallocate the dynamically allocated
 * space using the RTOS system allocator.
 *
 * @note Equivalent of C++ `delete ptr_th`.
 * @note Must be paired with `os_thread_new()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread
 */
void
os_thread_delete (os_thread_t* thread)
{
  assert(thread != nullptr);
  delete reinterpret_cast<rtos::thread*> (thread);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread::name()
 */
const char*
os_thread_get_name (os_thread_t* thread)
{
  assert(thread != nullptr);
  return (reinterpret_cast<rtos::thread&> (*thread)).name ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread::priority()
 */
os_thread_prio_t
os_thread_get_priority (os_thread_t* thread)
{
  assert(thread != nullptr);
  return (os_thread_prio_t) (reinterpret_cast<rtos::thread&> (*thread)).priority ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread::priority(priority_t)
 */
os_result_t
os_thread_set_priority (os_thread_t* thread, os_thread_prio_t prio)
{
  assert(thread != nullptr);
  return (os_result_t) (reinterpret_cast<rtos::thread&> (*thread)).priority (
      prio);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread::join()
 */
os_result_t
os_thread_join (os_thread_t* thread, void** exit_ptr)
{
  assert(thread != nullptr);
  return (os_result_t) reinterpret_cast<rtos::thread&> (*thread).join (exit_ptr);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread::resume()
 */
void
os_thread_resume (os_thread_t* thread)
{
  assert(thread != nullptr);
  return (reinterpret_cast<rtos::thread&> (*thread)).resume ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread::flags_raise()
 */
os_result_t
os_thread_flags_raise (os_thread_t* thread, os_flags_mask_t mask,
                       os_flags_mask_t* oflags)
{
  assert(thread != nullptr);
  return (os_result_t) (reinterpret_cast<rtos::thread&> (*thread)).flags_raise (
      mask, oflags);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread::state()
 */
os_thread_state_t
os_thread_get_state (os_thread_t* thread)
{
  assert(thread != nullptr);
  return reinterpret_cast<os_thread_state_t> ((reinterpret_cast<rtos::thread&> (*thread)).state ());
}

#if defined(OS_INCLUDE_RTOS_CUSTOM_THREAD_USER_STORAGE) || defined(__DOXYGEN__)

/**
 * @details
 *
 * @note
 *  Available only when `OS_INCLUDE_RTOS_CUSTOM_THREAD_USER_STORAGE`
 *  is defined.
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread::user_storage()
 */
os_thread_user_storage_t*
os_thread_get_user_storage (os_thread_t* thread)
{
  assert(thread != nullptr);
  return (reinterpret_cast<rtos::thread&> (*thread)).user_storage ();
}

#endif /* defined(OS_INCLUDE_RTOS_CUSTOM_THREAD_USER_STORAGE) */


/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread::stack()
 */
os_thread_stack_t*
os_thread_get_stack (os_thread_t* thread)
{
  assert(thread != nullptr);
  return reinterpret_cast<os_thread_stack_t*> (&(reinterpret_cast<rtos::thread&> (*thread)).stack ());
}

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread::stack::default_size()
 */
size_t
os_thread_stack_get_default_size (void)
{
  return thread::stack::default_size ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread::stack::default_size(std::size_t)
 */
size_t
os_thread_stack_set_default_size (size_t size_bytes)
{
  return thread::stack::default_size (size_bytes);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread::stack::min_size()
 */
size_t
os_thread_stack_get_min_size (void)
{
  return thread::stack::min_size ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread::stack::min_size(std::size_t)
 */
size_t
os_thread_stack_set_min_size (size_t size_bytes)
{
  return thread::stack::min_size (size_bytes);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread::stack::bottom()
 */
os_thread_stack_element_t*
os_thread_stack_get_bottom (os_thread_stack_t* stack)
{
  assert(stack != nullptr);
  return (reinterpret_cast<class rtos::thread::stack&> (*stack)).bottom ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread::stack::top()
 */
os_thread_stack_element_t*
os_thread_stack_get_top (os_thread_stack_t* stack)
{
  assert(stack != nullptr);
  return (reinterpret_cast<class rtos::thread::stack&> (*stack)).top ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread::stack::size()
 */
size_t
os_thread_stack_get_size (os_thread_stack_t* stack)
{
  assert(stack != nullptr);
  return (reinterpret_cast<class rtos::thread::stack&> (*stack)).size ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread::stack::available()
 */
size_t
os_thread_stack_get_available (os_thread_stack_t* stack)
{
  assert(stack != nullptr);
  return (reinterpret_cast<class rtos::thread::stack&> (*stack)).available ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread::stack::check_bottom_magic()
 */
bool
os_thread_stack_check_bottom_magic (os_thread_stack_t* stack)
{
  assert(stack != nullptr);
  return (reinterpret_cast<class rtos::thread::stack&> (*stack)).check_bottom_magic ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread::stack::check_top_magic()
 */
bool
os_thread_stack_check_top_magic (os_thread_stack_t* stack)
{
  assert(stack != nullptr);
  return (reinterpret_cast<class rtos::thread::stack&> (*stack)).check_top_magic ();
}

// ----------------------------------------------------------------------------

#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread::statistics::context_switches()
 */
os_statistics_counter_t
os_thread_stat_get_context_switches (os_thread_t* thread)
{
  assert(thread != nullptr);
  return static_cast<os_statistics_counter_t> ((reinterpret_cast<rtos::thread&> (*thread)).statistics ().context_switches ());
}

#endif /* defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES) */

#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::thread::statistics::cpu_cycles()
 */
os_statistics_duration_t
os_thread_stat_get_cpu_cycles (os_thread_t* thread)
{
  assert(thread != nullptr);
  return static_cast<os_statistics_duration_t> ((reinterpret_cast<rtos::thread&> (*thread)).statistics ().cpu_cycles ());
}

#endif /* defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES) */

// ----------------------------------------------------------------------------

/**
 * @details
 * Return the position of the first element in the list of
 * children threads of the given thread. If NULL, the position
 * first element in the list of top threads is returned.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
os_iterator_t
os_children_threads_iter_begin (os_thread_t* thread)
{
  return reinterpret_cast<os_iterator_t> (scheduler::children_threads (
      reinterpret_cast<rtos::thread*> (thread)).begin ().get_iterator_pointer ());
}

/**
 * @details
 * Return the position after the last element in the list of
 * children threads of the given thread. If NULL, the position
 * after the last element in the list of top threads is returned.
 *
 * The iteration loop must be terminated when the current
 * iterator is equal to this position, before entering the loop body.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
os_iterator_t
os_children_threads_iter_end (os_thread_t* thread)
{
  return reinterpret_cast<os_iterator_t> (scheduler::children_threads (
      reinterpret_cast<rtos::thread*> (thread)).end ().get_iterator_pointer ());
}

/**
 * @details
 * Calling it when the iterator is equal to the end position leads to
 * unpredictable results.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
os_thread_t*
os_children_threads_iter_get (os_iterator_t iterator)
{
  // Construct a local iterator object based on the pointer.
  thread::threads_list::iterator it
    { reinterpret_cast<internal::double_list_links*> (iterator) };
  return reinterpret_cast<os_thread_t*> (&(*it));
}

/**
 * @details
 * Calling it when the iterator is equal to the end position leads to
 * unpredictable results.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
os_iterator_t
os_children_threads_iter_next (os_iterator_t iterator)
{
  thread::threads_list::iterator it
    { reinterpret_cast<internal::double_list_links*> (iterator) };
  ++it;

  return reinterpret_cast<os_iterator_t> (it.get_iterator_pointer ());
}

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::clock::name()
 */
const char*
os_clock_get_name (os_clock_t* clock)
{
  assert(clock != nullptr);
  return (reinterpret_cast<rtos::clock&> (*clock)).name ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::clock::now()
 */
os_clock_timestamp_t
os_clock_now (os_clock_t* clock)
{
  assert(clock != nullptr);
  return (os_clock_timestamp_t) (reinterpret_cast<rtos::clock&> (*clock)).now ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::clock::steady_now()
 */
os_clock_timestamp_t
os_clock_steady_now (os_clock_t* clock)
{
  assert(clock != nullptr);
  return (os_clock_timestamp_t) (reinterpret_cast<rtos::clock&> (*clock)).steady_now ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::clock::sleep_for()
 */
os_result_t
os_clock_sleep_for (os_clock_t* clock, os_clock_duration_t duration)
{
  assert(clock != nullptr);
  return (os_result_t) (reinterpret_cast<rtos::clock&> (*clock)).sleep_for (
      duration);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::clock::sleep_until()
 */
os_result_t
os_clock_sleep_until (os_clock_t* clock, os_clock_timestamp_t timestamp)
{
  assert(clock != nullptr);
  return (os_result_t) (reinterpret_cast<rtos::clock&> (*clock)).sleep_until (
      timestamp);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::clock::wait_for()
 */
os_result_t
os_clock_wait_for (os_clock_t* clock, os_clock_duration_t timeout)
{
  assert(clock != nullptr);
  return (os_result_t) (reinterpret_cast<rtos::clock&> (*clock)).wait_for (
      timeout);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::adjustable_clock::offset()
 */
os_clock_offset_t
os_clock_get_offset (os_clock_t* clock)
{
  assert(clock != nullptr);
  return (os_clock_offset_t) (reinterpret_cast<rtos::clock&> (*clock)).offset ();

}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::adjustable_clock::offset(offset_t)
 */
os_clock_offset_t
os_clock_set_offset (os_clock_t* clock, os_clock_offset_t offset)
{
  assert(clock != nullptr);
  assert(clock != nullptr);
  return (os_clock_offset_t) (reinterpret_cast<rtos::clock&> (*clock)).offset (
      (clock::offset_t) offset);

}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::sysclock
 */
os_clock_t*
os_clock_get_sysclock (void)
{
  return (os_clock_t*) &sysclock;
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::rtclock
 */
os_clock_t*
os_clock_get_rtclock (void)
{
  return (os_clock_t*) &rtclock;
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::hrclock
 */
os_clock_t*
os_clock_get_hrclock (void)
{
  return (os_clock_t*) &hrclock;
}

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::clock::now()
 */
os_clock_timestamp_t
os_sysclock_now (void)
{
  return (os_clock_timestamp_t) sysclock.now ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::clock::sleep_for()
 */
os_result_t
os_sysclock_sleep_for (os_clock_duration_t duration)
{
  return (os_result_t) sysclock.sleep_for (duration);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::clock::sleep_until()
 */
os_result_t
os_sysclock_sleep_until (os_clock_timestamp_t timestamp)
{
  return (os_result_t) sysclock.sleep_until (timestamp);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::clock::wait_for()
 */
os_result_t
os_sysclock_wait_for (os_clock_duration_t timeout)
{
  return (os_result_t) sysclock.wait_for (timeout);
}

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::timer::attributes
 */
void
os_timer_attr_init (os_timer_attr_t* attr)
{
  assert(attr != nullptr);
  new (attr) timer::attributes ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::timer::attributes_periodic
 */
void
os_timer_attr_periodic_init (os_timer_attr_t* attr)
{
  assert(attr != nullptr);
  new (attr) timer::attributes_periodic ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::timer::periodic_initializer
 */
const os_timer_attr_t*
os_timer_attr_get_periodic (void)
{
  return (const os_timer_attr_t*) &timer::periodic_initializer;
}

/**
 * @details
 *
 * @note Must be paired with `os_timer_destruct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::timer
 */
void
os_timer_construct (os_timer_t* timer, const char* name,
                    os_timer_func_t function, os_timer_func_args_t args,
                    const os_timer_attr_t* attr)
{
  assert(timer != nullptr);
  if (attr == nullptr)
    {
      attr = (const os_timer_attr_t*) &timer::periodic_initializer;
    }
  new (timer) rtos::timer (name, (timer::func_t) function,
                           (timer::func_args_t) args,
                           (timer::attributes&) *attr);
}

/**
 * @details
 *
 * @note Must be paired with `os_timer_construct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::timer
 */
void
os_timer_destruct (os_timer_t* timer)
{
  assert(timer != nullptr);
  (reinterpret_cast<rtos::timer&> (*timer)).~timer ();
}

/**
 * @details
 *
 * Dynamically allocate the timer object instance using the RTOS
 * system allocator and construct it.
 *
 * @note Equivalent of C++ `new timer(...)`.
 * @note Must be paired with `os_timer_delete()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::timer
 */
os_timer_t*
os_timer_new (const char* name, os_timer_func_t function,
              os_timer_func_args_t args, const os_timer_attr_t* attr)
{
  if (attr == nullptr)
    {
      attr = (const os_timer_attr_t*) &timer::periodic_initializer;
    }
  return reinterpret_cast<os_timer_t*> (new rtos::timer (
      name, (timer::func_t) function, (timer::func_args_t) args,
      (timer::attributes&) *attr));
}

/**
 * @details
 *
 * Destruct the timer and deallocate the dynamically allocated
 * space using the RTOS system allocator.
 *
 * @note Equivalent of C++ `delete ptr_timer`.
 * @note Must be paired with `os_timer_new()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::timer
 */
void
os_timer_delete (os_timer_t* timer)
{
  assert(timer != nullptr);
  delete reinterpret_cast<rtos::timer*> (timer);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::timer::name()
 */
const char*
os_timer_get_name (os_timer_t* timer)
{
  assert(timer != nullptr);
  return (reinterpret_cast<rtos::timer&> (*timer)).name ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::timer::start()
 */
os_result_t
os_timer_start (os_timer_t* timer, os_clock_duration_t period)
{
  assert(timer != nullptr);
  return (os_result_t) (reinterpret_cast<rtos::timer&> (*timer)).start (period);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::timer::stop()
 */
os_result_t
os_timer_stop (os_timer_t* timer)
{
  assert(timer != nullptr);
  return (os_result_t) (reinterpret_cast<rtos::timer&> (*timer)).stop ();
}

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::mutex::attributes
 */
void
os_mutex_attr_init (os_mutex_attr_t* attr)
{
  assert(attr != nullptr);
  new (attr) mutex::attributes ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::mutex::attributes_recursive
 */
void
os_mutex_attr_recursive_init (os_mutex_attr_t* attr)
{
  assert(attr != nullptr);
  new (attr) mutex::attributes_recursive ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::mutex::initializer_recursive
 */
const os_mutex_attr_t*
os_mutex_attr_get_recursive (void)
{
  return (const os_mutex_attr_t*) &mutex::initializer_recursive;
}

/**
 * @details
 *
 * @note Must be paired with `os_mutex_destruct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::mutex
 */
void
os_mutex_construct (os_mutex_t* mutex, const char* name,
                    const os_mutex_attr_t* attr)
{
  assert(mutex != nullptr);
  if (attr == nullptr)
    {
      attr = (const os_mutex_attr_t*) &mutex::initializer_normal;
    }
  new (mutex) rtos::mutex (name, (mutex::attributes&) *attr);
}

/**
 * @details
 *
 * @note Must be paired with `os_mutex_destruct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::mutex_recursive
 */
void
os_mutex_recursive_construct (os_mutex_t* mutex, const char* name,
                              const os_mutex_attr_t* attr)
{
  assert(mutex != nullptr);
  if (attr == nullptr)
    {
      attr = (const os_mutex_attr_t*) &mutex::initializer_recursive;
    }
  new (mutex) rtos::mutex_recursive (name, (mutex::attributes&) *attr);
}

/**
 * @details
 *
 * @note Must be paired with `os_mutex_construct()` or
 *  `os_mutex_recursive_construct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::mutex
 */
void
os_mutex_destruct (os_mutex_t* mutex)
{
  assert(mutex != nullptr);
  (reinterpret_cast<rtos::mutex&> (*mutex)).~mutex ();
}

/**
 * @details
 *
 * Dynamically allocate the mutex object instance using the RTOS
 * system allocator and construct it.
 *
 * @note Equivalent of C++ `new mutex(...)`.
 * @note Must be paired with `os_mutex_delete()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::mutex
 */
os_mutex_t*
os_mutex_new (const char* name, const os_mutex_attr_t* attr)
{
  if (attr == nullptr)
    {
      attr = (const os_mutex_attr_t*) &mutex::initializer_normal;
    }
  return reinterpret_cast<os_mutex_t*> (new rtos::mutex (
      name, (mutex::attributes&) *attr));
}

/**
 * @details
 *
 * Dynamically allocate the recursive mutex object instance using the RTOS
 * system allocator and construct it.
 *
 * @note Equivalent of C++ `new mutex_recursive(...)`.
 * @note Must be paired with `os_mutex_delete()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::mutex_recursive
 */
os_mutex_t*
os_mutex_recursive_new (const char* name, const os_mutex_attr_t* attr)
{
  if (attr == nullptr)
    {
      attr = (const os_mutex_attr_t*) &mutex::initializer_recursive;
    }
  return reinterpret_cast<os_mutex_t*> (new rtos::mutex_recursive (
      name, (mutex::attributes&) *attr));
}

/**
 * @details
 *
 * Destruct the mutex and deallocate the dynamically allocated
 * space using the RTOS system allocator.
 *
 * @note Equivalent of C++ `delete ptr_mutex`.
 * @note Must be paired with `os_mutex_new()` or
 *  `os_mutex_recursive_new()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::mutex
 */
void
os_mutex_delete (os_mutex_t* mutex)
{
  assert(mutex != nullptr);
  delete reinterpret_cast<rtos::mutex*> (mutex);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::mutex::name()
 */
const char*
os_mutex_get_name (os_mutex_t* mutex)
{
  assert(mutex != nullptr);
  return (reinterpret_cast<rtos::mutex&> (*mutex)).name ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::mutex::lock()
 */
os_result_t
os_mutex_lock (os_mutex_t* mutex)
{
  assert(mutex != nullptr);
  return (os_result_t) (reinterpret_cast<rtos::mutex&> (*mutex)).lock ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::mutex::try_lock()
 */
os_result_t
os_mutex_try_lock (os_mutex_t* mutex)
{
  assert(mutex != nullptr);
  return (os_result_t) (reinterpret_cast<rtos::mutex&> (*mutex)).try_lock ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::mutex::timed_lock()
 */
os_result_t
os_mutex_timed_lock (os_mutex_t* mutex, os_clock_duration_t timeout)
{
  assert(mutex != nullptr);
  return (os_result_t) (reinterpret_cast<rtos::mutex&> (*mutex)).timed_lock (
      timeout);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::mutex::unlock()
 */
os_result_t
os_mutex_unlock (os_mutex_t* mutex)
{
  assert(mutex != nullptr);
  return (os_result_t) (reinterpret_cast<rtos::mutex&> (*mutex)).unlock ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::mutex::prio_ceiling()
 */
os_thread_prio_t
os_mutex_get_prio_ceiling (os_mutex_t* mutex)
{
  assert(mutex != nullptr);
  return (os_thread_prio_t) (reinterpret_cast<rtos::mutex&> (*mutex)).prio_ceiling ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::mutex::prio_ceiling(thread::priority_t, thread::priority_t*)
 */
os_result_t
os_mutex_set_prio_ceiling (os_mutex_t* mutex, os_thread_prio_t prio_ceiling,
                           os_thread_prio_t* old_prio_ceiling)
{
  assert(mutex != nullptr);
  return (os_result_t) (reinterpret_cast<rtos::mutex&> (*mutex)).prio_ceiling (
      prio_ceiling, old_prio_ceiling);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::mutex::consistent()
 */
os_result_t
os_mutex_mark_consistent (os_mutex_t* mutex)
{
  assert(mutex != nullptr);
  return (os_result_t) (reinterpret_cast<rtos::mutex&> (*mutex)).consistent ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::mutex::owner()
 */
os_thread_t*
os_mutex_get_owner (os_mutex_t* mutex)
{
  assert(mutex != nullptr);
  return (os_thread_t*) (reinterpret_cast<rtos::mutex&> (*mutex)).owner ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::mutex::type()
 */
os_mutex_type_t
os_mutex_get_type (os_mutex_t* mutex)
{
  assert(mutex != nullptr);
  return (reinterpret_cast<rtos::mutex&> (*mutex)).type ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::mutex::protocol()
 */
os_mutex_protocol_t
os_mutex_get_protocol (os_mutex_t* mutex)
{
  assert(mutex != nullptr);
  return (reinterpret_cast<rtos::mutex&> (*mutex)).protocol ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::mutex::robustness()
 */
os_mutex_robustness_t
os_mutex_get_robustness (os_mutex_t* mutex)
{
  assert(mutex != nullptr);
  return (reinterpret_cast<rtos::mutex&> (*mutex)).robustness ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::mutex::reset()
 */
os_result_t
os_mutex_reset (os_mutex_t* mutex)
{
  assert(mutex != nullptr);
  return (os_result_t) (reinterpret_cast<rtos::mutex&> (*mutex)).reset ();
}

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::condition_variable::attributes
 */
void
os_condvar_attr_init (os_condvar_attr_t* attr)
{
  assert(attr != nullptr);
  new (attr) condition_variable::attributes ();
}

/**
 * @details
 *
 * @note Must be paired with `os_condvar_destruct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::condition_variable
 */
void
os_condvar_construct (os_condvar_t* condvar, const char* name,
                      const os_condvar_attr_t* attr)
{
  assert(condvar != nullptr);
  if (attr == nullptr)
    {
      attr = (const os_condvar_attr_t*) &condition_variable::initializer;
    }
  new (condvar) condition_variable (name,
                                    (condition_variable::attributes&) *attr);
}

/**
 * @details
 *
 * @note Must be paired with `os_condvar_construct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::condition_variable
 */
void
os_condvar_destruct (os_condvar_t* condvar)
{
  assert(condvar != nullptr);
  (reinterpret_cast<condition_variable&> (*condvar)).~condition_variable ();
}

/**
 * @details
 *
 * Dynamically allocate the condition variable object instance using the RTOS
 * system allocator and construct it.
 *
 * @note Equivalent of C++ `new condition_variable(...)`.
 * @note Must be paired with `os_condvar_delete()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::condition_variable
 */
os_condvar_t*
os_condvar_new (const char* name, const os_condvar_attr_t* attr)
{
  if (attr == nullptr)
    {
      attr = (const os_condvar_attr_t*) &condition_variable::initializer;
    }
  return reinterpret_cast<os_condvar_t*> (new condition_variable (
      name, (condition_variable::attributes&) *attr));
}

/**
 * @details
 *
 * Destruct the condition variable and deallocate the dynamically allocated
 * space using the RTOS system allocator.
 *
 * @note Equivalent of C++ `delete ptr_condvar`.
 * @note Must be paired with `os_condvar_new()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::condition_variable
 */
void
os_condvar_delete (os_condvar_t* condvar)
{
  assert(condvar != nullptr);
  delete reinterpret_cast<condition_variable*> (condvar);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::condition_variable::name()
 */
const char*
os_condvar_get_name (os_condvar_t* condvar)
{
  assert(condvar != nullptr);
  return (reinterpret_cast<condition_variable&> (*condvar)).name ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::condition_variable::signal()
 */
os_result_t
os_condvar_signal (os_condvar_t* condvar)
{
  assert(condvar != nullptr);
  return (os_result_t) (reinterpret_cast<condition_variable&> (*condvar)).signal ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::condition_variable::broadcast()
 */
os_result_t
os_condvar_broadcast (os_condvar_t* condvar)
{
  assert(condvar != nullptr);
  return (os_result_t) (reinterpret_cast<condition_variable&> (*condvar)).broadcast ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::condition_variable::wait()
 */
os_result_t
os_condvar_wait (os_condvar_t* condvar, os_mutex_t* mutex)
{
  assert(condvar != nullptr);
  return (os_result_t) (reinterpret_cast<condition_variable&> (*condvar)).wait (
      reinterpret_cast<rtos::mutex&> (*mutex));
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::condition_variable::timed_wait()
 */
os_result_t
os_condvar_timed_wait (os_condvar_t* condvar, os_mutex_t* mutex,
                       os_clock_duration_t timeout)
{
  assert(condvar != nullptr);
  return (os_result_t) (reinterpret_cast<condition_variable&> (*condvar)).timed_wait (
      reinterpret_cast<rtos::mutex&> (*mutex), timeout);
}

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::semaphore::attributes
 */
void
os_semaphore_attr_init (os_semaphore_attr_t* attr)
{
  assert(attr != nullptr);
  new (attr) semaphore::attributes
    { };
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::semaphore::attributes_binary
 */
void
os_semaphore_attr_binary_init (os_semaphore_attr_t* attr,
                               const os_semaphore_count_t initial_value)
{
  assert(attr != nullptr);
  new (attr) semaphore::attributes_binary
    { initial_value };
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::semaphore::attributes_counting
 */
void
os_semaphore_attr_counting_init (os_semaphore_attr_t* attr,
                                 const os_semaphore_count_t max_value,
                                 const os_semaphore_count_t initial_value)
{
  assert(attr != nullptr);
  new (attr) semaphore::attributes_counting
    { max_value, initial_value };
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::semaphore::initializer_binary
 */
const os_semaphore_attr_t*
os_semaphore_attr_get_binary (void)
{
  return (const os_semaphore_attr_t*) &semaphore::initializer_binary;
}

/**
 * @details
 *
 * @note Must be paired with `os_semaphore_destruct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::semaphore
 */
void
os_semaphore_construct (os_semaphore_t* semaphore, const char* name,
                        const os_semaphore_attr_t* attr)
{
  assert(semaphore != nullptr);
  if (attr == nullptr)
    {
      attr = (const os_semaphore_attr_t*) &semaphore::initializer_binary;
    }
  new (semaphore) rtos::semaphore
    { name, (semaphore::attributes&) *attr };
}

/**
 * @details
 *
 * @note Must be paired with `os_semaphore_destruct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::semaphore_binary
 */
void
os_semaphore_binary_construct (os_semaphore_t* semaphore, const char* name,
                               const os_semaphore_count_t initial_value)
{
  assert(semaphore != nullptr);
  new (semaphore) rtos::semaphore_binary
    { name, initial_value };
}

/**
 * @details
 *
 * @note Must be paired with `os_semaphore_destruct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::semaphore_counting
 */
void
os_semaphore_counting_construct (os_semaphore_t* semaphore, const char* name,
                                 const os_semaphore_count_t max_value,
                                 const os_semaphore_count_t initial_value)
{
  assert(semaphore != nullptr);
  new (semaphore) rtos::semaphore_counting
    { name, max_value, initial_value };
}

/**
 * @details
 *
 * @note Must be paired with `os_semaphore_construct()` or
 * `os_semaphore_binary_construct()` or `os_semaphore_counting_construct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::semaphore
 */
void
os_semaphore_destruct (os_semaphore_t* semaphore)
{
  assert(semaphore != nullptr);
  (reinterpret_cast<rtos::semaphore&> (*semaphore)).~semaphore ();
}

/**
 * @details
 *
 * Dynamically allocate the semaphore object instance using the RTOS
 * system allocator and construct it.
 *
 * @note Equivalent of C++ `new semaphore(...)`.
 * @note Must be paired with `os_semaphore_delete()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::semaphore
 */
os_semaphore_t*
os_semaphore_new (const char* name, const os_semaphore_attr_t* attr)
{
  if (attr == nullptr)
    {
      attr = (const os_semaphore_attr_t*) &semaphore::initializer_binary;
    }
  return reinterpret_cast<os_semaphore_t*> (new rtos::semaphore
    { name, (semaphore::attributes&) *attr });
}

/**
 * @details
 *
 * Dynamically allocate the binary semaphore object instance using the RTOS
 * system allocator and construct it.
 *
 * @note Equivalent of C++ `new semaphore_binary(...)`.
 * @note Must be paired with `os_semaphore_delete()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::semaphore_binary
 */
os_semaphore_t*
os_semaphore_binary_new (const char* name,
                         const os_semaphore_count_t initial_value)
{
  return reinterpret_cast<os_semaphore_t*> (new rtos::semaphore_binary
    { name, initial_value });
}

/**
 * @details
 *
 * Dynamically allocate the counting semaphore object instance using the RTOS
 * system allocator and construct it.
 *
 * @note Equivalent of C++ `new semaphore_counting(...)`.
 * @note Must be paired with `os_semaphore_delete()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::semaphore_counting
 */
os_semaphore_t*
os_semaphore_counting_new (const char* name,
                           const os_semaphore_count_t max_value,
                           const os_semaphore_count_t initial_value)
{
  return reinterpret_cast<os_semaphore_t*> (new rtos::semaphore_counting
    { name, max_value, initial_value });
}

/**
 * @details
 *
 * Destruct the semaphore and deallocate the dynamically allocated
 * space using the RTOS system allocator.
 *
 * @note Equivalent of C++ `delete ptr_semaphore`.
 * @note Must be paired with `os_semaphore_new()` or
 * `os_semaphore_binary_new()` or `os_semaphore_counting_new()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::semaphore
 */
void
os_semaphore_delete (os_semaphore_t* semaphore)
{
  assert(semaphore != nullptr);
  delete reinterpret_cast<rtos::semaphore*> (semaphore);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::semaphore::name()
 */
const char*
os_semaphore_get_name (os_semaphore_t* semaphore)
{
  assert(semaphore != nullptr);
  return (reinterpret_cast<rtos::semaphore&> (*semaphore)).name ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::semaphore::post()
 */
os_result_t
os_semaphore_post (os_semaphore_t* semaphore)
{
  assert(semaphore != nullptr);
  return (os_result_t) (reinterpret_cast<rtos::semaphore&> (*semaphore)).post ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::semaphore::wait()
 */
os_result_t
os_semaphore_wait (os_semaphore_t* semaphore)
{
  assert(semaphore != nullptr);
  return (os_result_t) (reinterpret_cast<rtos::semaphore&> (*semaphore)).wait ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::semaphore::try_wait()
 */
os_result_t
os_semaphore_try_wait (os_semaphore_t* semaphore)
{
  assert(semaphore != nullptr);
  return (os_result_t) (reinterpret_cast<rtos::semaphore&> (*semaphore)).try_wait ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::semaphore::timed_wait()
 */
os_result_t
os_semaphore_timed_wait (os_semaphore_t* semaphore, os_clock_duration_t timeout)
{
  assert(semaphore != nullptr);
  return (os_result_t) (reinterpret_cast<rtos::semaphore&> (*semaphore)).timed_wait (
      timeout);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::semaphore::value()
 */
os_semaphore_count_t
os_semaphore_get_value (os_semaphore_t* semaphore)
{
  assert(semaphore != nullptr);
  return (os_semaphore_count_t) (reinterpret_cast<rtos::semaphore&> (*semaphore)).value ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::semaphore::reset()
 */
os_result_t
os_semaphore_reset (os_semaphore_t* semaphore)
{
  assert(semaphore != nullptr);
  return (os_result_t) (reinterpret_cast<rtos::semaphore&> (*semaphore)).reset ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::semaphore::initial_value()
 */
os_semaphore_count_t
os_semaphore_get_initial_value (os_semaphore_t* semaphore)
{
  assert(semaphore != nullptr);
  return (os_semaphore_count_t) (reinterpret_cast<rtos::semaphore&> (*semaphore)).initial_value ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::semaphore::max_value()
 */
os_semaphore_count_t
os_semaphore_get_max_value (os_semaphore_t* semaphore)
{
  assert(semaphore != nullptr);
  return (os_semaphore_count_t) (reinterpret_cast<rtos::semaphore&> (*semaphore)).max_value ();
}

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory_pool::attributes
 */
void
os_mempool_attr_init (os_mempool_attr_t* attr)
{
  assert(attr != nullptr);
  new (attr) memory_pool::attributes ();
}

/**
 * @details
 *
 * @note Must be paired with `os_mempool_destruct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory_pool
 */
void
os_mempool_construct (os_mempool_t* mempool, const char* name, size_t blocks,
                      size_t block_size_bytes, const os_mempool_attr_t* attr)
{
  assert(mempool != nullptr);
  if (attr == nullptr)
    {
      attr = (const os_mempool_attr_t*) &memory_pool::initializer;
    }
  new (mempool) memory_pool (name, blocks, block_size_bytes,
                             (memory_pool::attributes&) *attr);
}

/**
 * @details
 *
 * @note Must be paired with `os_mempool_construct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory_pool
 */
void
os_mempool_destruct (os_mempool_t* mempool)
{
  assert(mempool != nullptr);
  (reinterpret_cast<memory_pool&> (*mempool)).~memory_pool ();
}

/**
 * @details
 *
 * Dynamically allocate the memory pool object instance using the RTOS
 * system allocator and construct it.
 *
 * @note Equivalent of C++ `new memory_pool(...)`.
 * @note Must be paired with `os_mempool_delete()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory_pool
 */
os_mempool_t*
os_mempool_new (const char* name, size_t blocks, size_t block_size_bytes,
                const os_mempool_attr_t* attr)
{
  if (attr == nullptr)
    {
      attr = (const os_mempool_attr_t*) &memory_pool::initializer;
    }
  return reinterpret_cast<os_mempool_t*> (new memory_pool (
      name, blocks, block_size_bytes, (memory_pool::attributes&) *attr));
}

/**
 * @details
 *
 * Destruct the memory pool and deallocate the dynamically allocated
 * space using the RTOS system allocator.
 *
 * @note Equivalent of C++ `delete ptr_mempool`.
 * @note Must be paired with `os_mempool_new()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory_pool
 */
void
os_mempool_delete (os_mempool_t* mempool)
{
  assert(mempool != nullptr);
  delete reinterpret_cast<memory_pool*> (mempool);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory_pool::name()
 */
const char*
os_mempool_get_name (os_mempool_t* mempool)
{
  assert(mempool != nullptr);
  return (reinterpret_cast<memory_pool&> (*mempool)).name ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory_pool::alloc()
 */
void*
os_mempool_alloc (os_mempool_t* mempool)
{
  assert(mempool != nullptr);
  return (reinterpret_cast<memory_pool&> (*mempool)).alloc ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory_pool::try_alloc()
 */
void*
os_mempool_try_alloc (os_mempool_t* mempool)
{
  assert(mempool != nullptr);
  return (reinterpret_cast<memory_pool&> (*mempool)).try_alloc ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory_pool::timed_alloc()
 */
void*
os_mempool_timed_alloc (os_mempool_t* mempool, os_clock_duration_t timeout)
{
  assert(mempool != nullptr);
  return (reinterpret_cast<memory_pool&> (*mempool)).timed_alloc (timeout);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory_pool::free()
 */
os_result_t
os_mempool_free (os_mempool_t* mempool, void* block)
{
  assert(mempool != nullptr);
  return (os_result_t) (reinterpret_cast<memory_pool&> (*mempool)).free (block);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory_pool::capacity()
 */
size_t
os_mempool_get_capacity (os_mempool_t* mempool)
{
  assert(mempool != nullptr);
  return (reinterpret_cast<memory_pool&> (*mempool)).capacity ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory_pool::count()
 */
size_t
os_mempool_get_count (os_mempool_t* mempool)
{
  assert(mempool != nullptr);
  return (reinterpret_cast<memory_pool&> (*mempool)).count ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory_pool::block_size()
 */
size_t
os_mempool_get_block_size (os_mempool_t* mempool)
{
  assert(mempool != nullptr);
  return (reinterpret_cast<memory_pool&> (*mempool)).block_size ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory_pool::empty()
 */
bool
os_mempool_is_empty (os_mempool_t* mempool)
{
  assert(mempool != nullptr);
  return (reinterpret_cast<memory_pool&> (*mempool)).empty ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory_pool::full()
 */
bool
os_mempool_is_full (os_mempool_t* mempool)
{
  assert(mempool != nullptr);
  return (reinterpret_cast<memory_pool&> (*mempool)).full ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory_pool::reset()
 */
os_result_t
os_mempool_reset (os_mempool_t* mempool)
{
  assert(mempool != nullptr);
  return (os_result_t) (reinterpret_cast<memory_pool&> (*mempool)).reset ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory_pool::pool()
 */
void*
os_mempool_get_pool (os_mempool_t* mempool)
{
  assert(mempool != nullptr);
  return (void*) (reinterpret_cast<memory_pool&> (*mempool)).pool ();
}

// --------------------------------------------------------------------------

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::message_queue::attributes
 */
void
os_mqueue_attr_init (os_mqueue_attr_t* attr)
{
  assert(attr != nullptr);
  new (attr) message_queue::attributes ();
}

/**
 * @details
 *
 * @note Must be paired with `os_mqueue_destruct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::message_queue
 */
void
os_mqueue_construct (os_mqueue_t* mqueue, const char* name, size_t msgs,
                     size_t msg_size_bytes, const os_mqueue_attr_t* attr)
{
  assert(mqueue != nullptr);
  if (attr == nullptr)
    {
      attr = (const os_mqueue_attr_t*) &message_queue::initializer;
    }
  new (mqueue) message_queue (name, msgs, msg_size_bytes,
                              (message_queue::attributes&) *attr);
}

/**
 * @details
 *
 * @note Must be paired with `os_mqueue_construct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::message_queue
 */
void
os_mqueue_destruct (os_mqueue_t* mqueue)
{
  assert(mqueue != nullptr);
  (reinterpret_cast<message_queue&> (*mqueue)).~message_queue ();
}

/**
 * @details
 *
 * Dynamically allocate the message queue object instance using the RTOS
 * system allocator and construct it.
 *
 * @note Equivalent of C++ `new message_queue(...)`.
 * @note Must be paired with `os_mqueue_delete()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::message_queue
 */
os_mqueue_t*
os_mqueue_new (const char* name, size_t msgs, size_t msg_size_bytes,
               const os_mqueue_attr_t* attr)
{
  if (attr == nullptr)
    {
      attr = (const os_mqueue_attr_t*) &message_queue::initializer;
    }
  return reinterpret_cast<os_mqueue_t*> (new message_queue (
      name, msgs, msg_size_bytes, (message_queue::attributes&) *attr));
}

/**
 * @details
 *
 * Destruct the message queue and deallocate the dynamically allocated
 * space using the RTOS system allocator.
 *
 * @note Equivalent of C++ `delete ptr_mqueue`.
 * @note Must be paired with `os_mqueue_new()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::message_queue
 */
void
os_mqueue_delete (os_mqueue_t* mqueue)
{
  assert(mqueue != nullptr);
  delete reinterpret_cast<message_queue*> (mqueue);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::message_queue::name()
 */
const char*
os_mqueue_get_name (os_mqueue_t* mqueue)
{
  assert(mqueue != nullptr);
  return (reinterpret_cast<message_queue&> (*mqueue)).name ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::message_queue::send()
 */
os_result_t
os_mqueue_send (os_mqueue_t* mqueue, const void* msg, size_t nbytes,
                os_mqueue_prio_t mprio)
{
  assert(mqueue != nullptr);
  return (os_result_t) (reinterpret_cast<message_queue&> (*mqueue)).send (
      msg, nbytes, mprio);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::message_queue::try_send()
 */
os_result_t
os_mqueue_try_send (os_mqueue_t* mqueue, const void* msg, size_t nbytes,
                    os_mqueue_prio_t mprio)
{
  assert(mqueue != nullptr);
  return (os_result_t) (reinterpret_cast<message_queue&> (*mqueue)).try_send (
      msg, nbytes, mprio);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::message_queue::timed_send()
 */
os_result_t
os_mqueue_timed_send (os_mqueue_t* mqueue, const void* msg, size_t nbytes,
                      os_clock_duration_t timeout, os_mqueue_prio_t mprio)
{
  assert(mqueue != nullptr);
  return (os_result_t) (reinterpret_cast<message_queue&> (*mqueue)).timed_send (
      msg, nbytes, timeout, mprio);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::message_queue::receive()
 */
os_result_t
os_mqueue_receive (os_mqueue_t* mqueue, void* msg, size_t nbytes,
                   os_mqueue_prio_t* mprio)
{
  assert(mqueue != nullptr);
  return (os_result_t) (reinterpret_cast<message_queue&> (*mqueue)).receive (
      msg, nbytes, mprio);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::message_queue::try_receive()
 */
os_result_t
os_mqueue_try_receive (os_mqueue_t* mqueue, void* msg, size_t nbytes,
                       os_mqueue_prio_t* mprio)
{
  assert(mqueue != nullptr);
  return (os_result_t) (reinterpret_cast<message_queue&> (*mqueue)).try_receive (
      msg, nbytes, mprio);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::message_queue::timed_receive()
 */
os_result_t
os_mqueue_timed_receive (os_mqueue_t* mqueue, void* msg, size_t nbytes,
                         os_clock_duration_t timeout, os_mqueue_prio_t* mprio)
{
  assert(mqueue != nullptr);
  return (os_result_t) (reinterpret_cast<message_queue&> (*mqueue)).timed_receive (
      msg, nbytes, timeout, mprio);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::message_queue::length()
 */
size_t
os_mqueue_get_length (os_mqueue_t* mqueue)
{
  assert(mqueue != nullptr);
  return (reinterpret_cast<message_queue&> (*mqueue)).length ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::message_queue::capacity()
 */
size_t
os_mqueue_get_capacity (os_mqueue_t* mqueue)
{
  assert(mqueue != nullptr);
  return (reinterpret_cast<message_queue&> (*mqueue)).capacity ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::message_queue::msg_size()
 */
size_t
os_mqueue_get_msg_size (os_mqueue_t* mqueue)
{
  assert(mqueue != nullptr);
  return (reinterpret_cast<message_queue&> (*mqueue)).msg_size ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::message_queue::empty()
 */
bool
os_mqueue_is_empty (os_mqueue_t* mqueue)
{
  assert(mqueue != nullptr);
  return (reinterpret_cast<message_queue&> (*mqueue)).empty ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::message_queue::full()
 */
bool
os_mqueue_is_full (os_mqueue_t* mqueue)
{
  assert(mqueue != nullptr);
  return (reinterpret_cast<message_queue&> (*mqueue)).full ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::message_queue::reset()
 */
os_result_t
os_mqueue_reset (os_mqueue_t* mqueue)
{
  assert(mqueue != nullptr);
  return (os_result_t) (reinterpret_cast<message_queue&> (*mqueue)).reset ();
}

// --------------------------------------------------------------------------

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::event_flags::attributes
 */
void
os_evflags_attr_init (os_evflags_attr_t* attr)
{
  assert(attr != nullptr);
  new (attr) event_flags::attributes ();
}

/**
 * @details
 *
 * @note Must be paired with `os_evflags_destruct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::event_flags
 */
void
os_evflags_construct (os_evflags_t* evflags, const char* name,
                      const os_evflags_attr_t* attr)
{
  assert(evflags != nullptr);
  if (attr == nullptr)
    {
      attr = (const os_evflags_attr_t*) &event_flags::initializer;
    }
  new (evflags) event_flags (name, (event_flags::attributes&) *attr);
}

/**
 * @details
 *
 * @note Must be paired with `os_evflags_construct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::event_flags
 */
void
os_evflags_destruct (os_evflags_t* evflags)
{
  assert(evflags != nullptr);
  (reinterpret_cast<event_flags&> (*evflags)).~event_flags ();
}

/**
 * @details
 *
 * Dynamically allocate the event flags object instance using the RTOS
 * system allocator and construct it.
 *
 * @note Equivalent of C++ `new event_flags(...)`.
 * @note Must be paired with `os_evflags_delete()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::event_flags
 */
os_evflags_t*
os_evflags_new (const char* name, const os_evflags_attr_t* attr)
{
  if (attr == nullptr)
    {
      attr = (const os_evflags_attr_t*) &event_flags::initializer;
    }
  return reinterpret_cast<os_evflags_t*> (new event_flags (
      name, (event_flags::attributes&) *attr));
}

/**
 * @details
 *
 * Destruct the event flags and deallocate the dynamically allocated
 * space using the RTOS system allocator.
 *
 * @note Equivalent of C++ `delete ptr_evflags`.
 * @note Must be paired with `os_evflags_new()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::event_flags
 */
void
os_evflags_delete (os_evflags_t* evflags)
{
  assert(evflags != nullptr);
  delete reinterpret_cast<event_flags*> (evflags);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::event_flags::name()
 */
const char*
os_evflags_get_name (os_evflags_t* evflags)
{
  assert(evflags != nullptr);
  return (reinterpret_cast<event_flags&> (*evflags)).name ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::event_flags::wait()
 */
os_result_t
os_evflags_wait (os_evflags_t* evflags, os_flags_mask_t mask,
                 os_flags_mask_t* oflags, os_flags_mode_t mode)
{
  assert(evflags != nullptr);
  return (os_result_t) (reinterpret_cast<event_flags&> (*evflags)).wait (mask,
                                                                         oflags,
                                                                         mode);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::event_flags::try_wait()
 */
os_result_t
os_evflags_try_wait (os_evflags_t* evflags, os_flags_mask_t mask,
                     os_flags_mask_t* oflags, os_flags_mode_t mode)
{
  assert(evflags != nullptr);
  return (os_result_t) (reinterpret_cast<event_flags&> (*evflags)).try_wait (
      mask, oflags, mode);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::event_flags::timed_wait()
 */
os_result_t
os_evflags_timed_wait (os_evflags_t* evflags, os_flags_mask_t mask,
                       os_clock_duration_t timeout, os_flags_mask_t* oflags,
                       os_flags_mode_t mode)
{
  assert(evflags != nullptr);
  return (os_result_t) (reinterpret_cast<event_flags&> (*evflags)).timed_wait (
      mask, timeout, oflags, mode);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::event_flags::raise()
 */
os_result_t
os_evflags_raise (os_evflags_t* evflags, os_flags_mask_t mask,
                  os_flags_mask_t* oflags)
{
  assert(evflags != nullptr);
  return (os_result_t) (reinterpret_cast<event_flags&> (*evflags)).raise (
      mask, oflags);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::event_flags::clear()
 */
os_result_t
os_evflags_clear (os_evflags_t* evflags, os_flags_mask_t mask,
                  os_flags_mask_t* oflags)
{
  assert(evflags != nullptr);
  return (os_result_t) (reinterpret_cast<event_flags&> (*evflags)).clear (
      mask, oflags);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::event_flags::get()
 */
os_flags_mask_t
os_evflags_get (os_evflags_t* evflags, os_flags_mask_t mask,
                os_flags_mode_t mode)
{
  assert(evflags != nullptr);
  return (os_flags_mask_t) (reinterpret_cast<event_flags&> (*evflags)).get (
      mask, mode);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::event_flags::raise()
 */
bool
os_evflags_are_waiting (os_evflags_t* evflags)
{
  assert(evflags != nullptr);
  return (reinterpret_cast<event_flags&> (*evflags)).waiting ();
}

// --------------------------------------------------------------------------

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory::get_default_resource()
 */
os_memory_t*
os_memory_get_default (void)
{
  return reinterpret_cast<os_memory_t*> (rtos::memory::get_default_resource ());
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 * @warning Not thread safe, use a scheduler critical section to protect it.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory::memory_resource::allocate()
 */
void*
os_memory_allocate (os_memory_t* memory, size_t bytes, size_t alignment)
{
  assert(memory != nullptr);
  return (reinterpret_cast<rtos::memory::memory_resource&> (*memory)).allocate (
      bytes, alignment);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 * @warning Not thread safe, use a scheduler critical section to protect it.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory::memory_resource::deallocate()
 */
void
os_memory_deallocate (os_memory_t* memory, void* addr, size_t bytes,
                      size_t alignment)
{
  assert(memory != nullptr);
  (reinterpret_cast<rtos::memory::memory_resource&> (*memory)).deallocate (
      addr, bytes, alignment);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 * @warning Not thread safe, use a scheduler critical section to protect it.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory::memory_resource::reset()
 */
void
os_memory_reset (os_memory_t* memory)
{
  assert(memory != nullptr);
  (reinterpret_cast<rtos::memory::memory_resource&> (*memory)).reset ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 * @warning Not thread safe, use a scheduler critical section to protect it.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory::memory_resource::coalesce()
 */
bool
os_memory_coalesce (os_memory_t* memory)
{
  assert(memory != nullptr);
  return (reinterpret_cast<rtos::memory::memory_resource&> (*memory)).coalesce ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 * @warning Not thread safe, use a scheduler critical section to protect it.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory::memory_resource::total_bytes()
 */
size_t
os_memory_get_total_bytes (os_memory_t* memory)
{
  assert(memory != nullptr);
  return (reinterpret_cast<rtos::memory::memory_resource&> (*memory)).total_bytes ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 * @warning Not thread safe, use a scheduler critical section to protect it.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory::memory_resource::allocated_bytes()
 */
size_t
os_memory_get_allocated_bytes (os_memory_t* memory)
{
  assert(memory != nullptr);
  return (reinterpret_cast<rtos::memory::memory_resource&> (*memory)).allocated_bytes ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 * @warning Not thread safe, use a scheduler critical section to protect it.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory::memory_resource::free_bytes()
 */
size_t
os_memory_get_free_bytes (os_memory_t* memory)
{
  assert(memory != nullptr);
  return (reinterpret_cast<rtos::memory::memory_resource&> (*memory)).free_bytes ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 * @warning Not thread safe, use a scheduler critical section to protect it.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory::memory_resource::allocated_chunks()
 */
size_t
os_memory_get_allocated_chunks (os_memory_t* memory)
{
  assert(memory != nullptr);
  return (reinterpret_cast<rtos::memory::memory_resource&> (*memory)).allocated_chunks ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 * @warning Not thread safe, use a scheduler critical section to protect it.
 *
 * @par For the complete definition, see
 *  @ref os::rtos::memory::memory_resource::free_chunks()
 */
size_t
os_memory_get_free_chunks (os_memory_t* memory)
{
  assert(memory != nullptr);
  return (reinterpret_cast<rtos::memory::memory_resource&> (*memory)).free_chunks ();
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
  if (interrupts::in_handler_mode ())
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
  // Get the current SysTick timestamp down to cpu cycles.
  return static_cast<uint32_t> (hrclock.now ());
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
  if (interrupts::in_handler_mode ())
    {
      return nullptr;
    }

  if (thread_def == nullptr)
    {
      return nullptr;
    }

  thread::attributes attr;
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
      thread* th = (thread*) &thread_def->data[i];
      if (th->state () == thread::state::undefined
          || th->state () == thread::state::destroyed)
        {
          if (attr.th_stack_size_bytes > 0)
            {
              attr.th_stack_address = &thread_def->stack[(i)
                  * ((thread_def->stacksize + sizeof(uint64_t) - 1)
                      / sizeof(uint64_t))];
            }
          new (th) thread (thread_def->name,
                           (thread::func_t) thread_def->pthread, args, attr);

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
  if (interrupts::in_handler_mode ())
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
  if (interrupts::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (thread_id == nullptr)
    {
      return osErrorParameter;
    }

  thread::state_t state =
      (reinterpret_cast<rtos::thread&> (*thread_id)).state ();
  if (state == thread::state::undefined)
    {
      return osErrorResource;
    }

  if ((reinterpret_cast<thread*> (thread_id)) == &this_thread::thread ())
    {
      this_thread::exit ();
      /* NOTREACHED */
    }
  else
    {
      (reinterpret_cast<rtos::thread&> (*thread_id)).kill ();
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
  if (interrupts::in_handler_mode ())
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
  if (interrupts::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (thread_id == nullptr)
    {
      return osErrorParameter;
    }

  thread::state_t state =
      (reinterpret_cast<rtos::thread&> (*thread_id)).state ();
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
  result_t res =
      ((reinterpret_cast<rtos::thread&> (*thread_id)).priority (prio));

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
  if (interrupts::in_handler_mode ())
    {
      return osPriorityError;
    }

  if (thread_id == nullptr)
    {
      return osPriorityError;
    }

  // Call C++ accessor.
  thread::priority_t prio =
      (reinterpret_cast<rtos::thread&> (*thread_id)).priority ();
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
  if (interrupts::in_handler_mode ())
    {
      return osErrorISR;
    }

  result_t res = sysclock.sleep_for (
      clock_systick::ticks_cast ((uint64_t) (millisec * 1000u)));

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

  if (interrupts::in_handler_mode ())
    {
      event.status = osErrorISR;
      return event;
    }

  result_t res = sysclock.wait_for (
      clock_systick::ticks_cast ((uint64_t) (millisec * 1000u)));

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
  if (interrupts::in_handler_mode ())
    {
      return nullptr;
    }

  if (timer_def == nullptr)
    {
      return nullptr;
    }

  timer::attributes attr;
  attr.tm_type = (timer::type_t) type;

  return reinterpret_cast<osTimerId> (new ((void*) timer_def->data) timer (
      timer_def->name, (timer::func_t) timer_def->ptimer,
      (timer::func_args_t) args, attr));
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
  if (interrupts::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (timer_id == nullptr)
    {
      return osErrorParameter;
    }

  result_t res = (reinterpret_cast<rtos::timer&> (*timer_id)).start (
      clock_systick::ticks_cast ((uint64_t) (millisec * 1000u)));

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
  if (interrupts::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (timer_id == nullptr)
    {
      return osErrorParameter;
    }

  result_t res = (reinterpret_cast<rtos::timer&> (*timer_id)).stop ();
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
  if (interrupts::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (timer_id == nullptr)
    {
      return osErrorParameter;
    }

  (reinterpret_cast<rtos::timer&> (*timer_id)).~timer ();
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

  flags::mask_t osig;
  ((thread*) (thread_id))->flags_raise ((flags::mask_t) signals, &osig);
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

  if (interrupts::in_handler_mode () || (signals == 0))
    {
      return (int32_t) 0x80000000;
    }

  flags::mask_t sig;

#if defined(OS_INCLUDE_RTOS_THREAD_PUBLIC_FLAGS_CLEAR)
  ((thread*) (thread_id))->flags_clear ((flags::mask_t) signals, &sig);
#else
  assert(((thread* ) (thread_id)) == &this_thread::thread ());
  // IGNORE THREAD ID!
  this_thread::flags_clear ((flags::mask_t) signals, &sig);
#endif
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

  if (interrupts::in_handler_mode ())
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
      res = this_thread::flags_wait ((flags::mask_t) signals,
                                     (flags::mask_t*) &event.value.signals);
    }
  else if (millisec == 0)
    {
      res = this_thread::flags_try_wait ((flags::mask_t) signals,
                                         (flags::mask_t*) &event.value.signals);
    }
  else
    {
      res = this_thread::flags_timed_wait (
          (flags::mask_t) signals,
          clock_systick::ticks_cast ((uint64_t) (millisec * 1000u)),
          (flags::mask_t*) &event.value.signals);
    }

  if (res == result::ok)
    {
      event.status = osEventSignal;
    }
  else if (res == EWOULDBLOCK)
    {
      event.status = osOK; // Only for try_flags_wait().
    }
  else if (res == ETIMEDOUT)
    {
      event.status = osEventTimeout; // Only for timed_flags_wait().
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
 * Create and initialise a mutex object instance.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osMutexId
osMutexCreate (const osMutexDef_t* mutex_def)
{
  if (interrupts::in_handler_mode ())
    {
      return nullptr;
    }

  if (mutex_def == nullptr)
    {
      return nullptr;
    }

  mutex::attributes attr;
  attr.mx_type = mutex::type::recursive;
  attr.mx_protocol = mutex::protocol::inherit;

  return reinterpret_cast<osMutexId> (new ((void*) mutex_def->data) mutex (
      mutex_def->name, attr));
}

/**
 * @details
 * Wait until a mutex becomes available. If no other thread has
 * obtained the mutex, the function instantly returns and blocks
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
  if (interrupts::in_handler_mode ())
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
      ret = (reinterpret_cast<rtos::mutex&> (*mutex_id)).lock ();
      // osErrorResource:
    }
  else if (millisec == 0)
    {
      ret = (reinterpret_cast<rtos::mutex&> (*mutex_id)).try_lock ();
    }
  else
    {
      ret = (reinterpret_cast<rtos::mutex&> (*mutex_id)).timed_lock (
          clock_systick::ticks_cast ((uint64_t) (millisec * 1000u)));
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
 * Release a mutex that was obtained with osMutexWait. Other
 * threads that currently wait for the same mutex will be now
 * put into the state READY.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osMutexRelease (osMutexId mutex_id)
{
  if (interrupts::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (mutex_id == nullptr)
    {
      return osErrorParameter;
    }

  result_t res;
  res = (reinterpret_cast<rtos::mutex&> (*mutex_id)).unlock ();

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
 * Delete a mutex object instance. The function releases internal memory
 * obtained for mutex handling. After this call the mutex_id is no
 * longer valid and cannot be used. The mutex may be created again
 * using the function osMutexCreate.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osMutexDelete (osMutexId mutex_id)
{
  if (interrupts::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (mutex_id == nullptr)
    {
      return osErrorParameter;
    }

  (reinterpret_cast<rtos::mutex&> (*mutex_id)).~mutex ();
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
  if (interrupts::in_handler_mode ())
    {
      return nullptr;
    }

  if (semaphore_def == nullptr)
    {
      return nullptr;
    }

  semaphore::attributes attr;
  attr.sm_initial_value = (semaphore::count_t) count;
  // The logic is very strange, the CMSIS expects both the max-count to be the
  // same as count, and also to accept a count of 0, which leads to
  // useless semaphores. We patch this behaviour in the wrapper, the main
  // object uses a more realistic max_value.
  attr.sm_max_value = (semaphore::count_t) (
      count == 0 ? osFeature_Semaphore : count);

  return reinterpret_cast<osSemaphoreId> (new ((void*) semaphore_def->data) semaphore (
      semaphore_def->name, attr));
}

/**
 * @details
 * Wait until a semaphore token becomes available. When no semaphore
 * token is available, the function waits for the time specified with
 * the parameter millisec.
 *
 * The argument millisec specifies how long the system waits for a
 * semaphore token to become available. While the system waits the
 * thread that is calling this function is put into the state WAITING.
 * The millisec timeout can have the following values:
 *
 * - when millisec is 0, the function returns instantly.
 * - when millisec is set to osWaitForever the function will wait
 * for an infinite time until the semaphore token becomes available.
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
  if (interrupts::in_handler_mode ())
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
      res = (reinterpret_cast<rtos::semaphore&> (*semaphore_id)).wait ();
    }
  else if (millisec == 0)
    {
      res = (reinterpret_cast<rtos::semaphore&> (*semaphore_id)).try_wait ();
      if (res == EWOULDBLOCK)
        {
          return 0;
        }
    }
  else
    {
      res = (reinterpret_cast<rtos::semaphore&> (*semaphore_id)).timed_wait (
          clock_systick::ticks_cast ((uint64_t) (millisec * 1000u)));
      if (res == ETIMEDOUT)
        {
          return 0;
        }
    }

  if (res == 0)
    {
      int count =
          (int32_t) (reinterpret_cast<rtos::semaphore&> (*semaphore_id)).value ();
      return count + 1;
    }
  else
    {
      return -1;
    }
}

/**
 * @details
 * Release a semaphore token. This increments the count of
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

  if ((reinterpret_cast<rtos::semaphore&> (*semaphore_id)).initial_value ()
      == 0)
    {
      return osErrorResource;
    }

  result_t res = (reinterpret_cast<rtos::semaphore&> (*semaphore_id)).post ();

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
 * Delete a semaphore object instance. The function releases internal memory
 * obtained for semaphore handling. After this call the semaphore_id
 * is no longer valid and cannot be used. The semaphore may be created
 * again using the function osSemaphoreCreate.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osSemaphoreDelete (osSemaphoreId semaphore_id)
{
  if (interrupts::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (semaphore_id == nullptr)
    {
      return osErrorParameter;
    }

  (reinterpret_cast<rtos::semaphore&> (*semaphore_id)).~semaphore ();
  return osOK;
}

#endif /* Semaphore available */

// ----------------------------------------------------------------------------
//  ==== Memory Pool Management Functions ====

#if (defined (osFeature_Pool)  &&  (osFeature_Pool != 0))

/**
 * @details
 * Create and initialize a memory pool object instance.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osPoolId
osPoolCreate (const osPoolDef_t* pool_def)
{
  if (interrupts::in_handler_mode ())
    {
      return nullptr;
    }

  if (pool_def == nullptr)
    {
      return nullptr;
    }

  memory_pool::attributes attr;
  attr.mp_pool_address = pool_def->pool;
  attr.mp_pool_size_bytes = pool_def->pool_sz;
  return reinterpret_cast<osPoolId> (new ((void*) pool_def->data) memory_pool (
      pool_def->name, (std::size_t) pool_def->items,
      (std::size_t) pool_def->item_sz, attr));
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

  return (reinterpret_cast<memory_pool&> (*pool_id)).try_alloc ();
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
  ret = (reinterpret_cast<memory_pool&> (*pool_id)).try_alloc ();
  if (ret != nullptr)
    {
      memset (ret, 0,
              (reinterpret_cast<memory_pool&> (*pool_id)).block_size ());
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
  res = (reinterpret_cast<memory_pool&> (*pool_id)).free (block);

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
 * Create and initialise a message queue object instance.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osMessageQId
osMessageCreate (const osMessageQDef_t* queue_def,
                 osThreadId thread_id __attribute__((unused)))
{
  if (interrupts::in_handler_mode ())
    {
      return nullptr;
    }

  if (queue_def == nullptr)
    {
      return nullptr;
    }

  message_queue::attributes attr;
  attr.mq_queue_address = queue_def->queue;
  attr.mq_queue_size_bytes = queue_def->queue_sz;

  return reinterpret_cast<osMessageQId> (new ((void*) queue_def->data) message_queue (
      queue_def->name, (std::size_t) queue_def->items,
      (std::size_t) queue_def->item_sz, attr));
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
      if (interrupts::in_handler_mode ())
        {
          return osErrorParameter;
        }
      res = (reinterpret_cast<message_queue&> (*queue_id)).send (
          (const char*) &info, sizeof(uint32_t), 0);
      // osOK, osErrorResource, osErrorParameter
    }
  else if (millisec == 0)
    {
      res = (reinterpret_cast<message_queue&> (*queue_id)).try_send (
          (const char*) &info, sizeof(uint32_t), 0);
      // osOK, osErrorResource, osErrorParameter
    }
  else
    {
      if (interrupts::in_handler_mode ())
        {
          return osErrorParameter;
        }
      res = (reinterpret_cast<message_queue&> (*queue_id)).timed_send (
          (const char*) &info, sizeof(uint32_t),
          clock_systick::ticks_cast ((uint64_t) (millisec * 1000u)), 0);
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
      if (interrupts::in_handler_mode ())
        {
          event.status = osErrorParameter;
          return event;
        }
      res = (reinterpret_cast<message_queue&> (*queue_id)).receive (
          (char*) &event.value.v, sizeof(uint32_t),
          NULL);
      // result::event_message;
    }
  else if (millisec == 0)
    {
      res = (reinterpret_cast<message_queue&> (*queue_id)).try_receive (
          (char*) &event.value.v, sizeof(uint32_t), NULL);
      // result::event_message when message;
      // result::ok when no meessage
    }
  else
    {
      if (interrupts::in_handler_mode ())
        {
          event.status = osErrorParameter;
          return event;
        }
      res = (reinterpret_cast<message_queue&> (*queue_id)).timed_receive (
          (char*) &event.value.v, sizeof(uint32_t),
          clock_systick::ticks_cast ((uint64_t) (millisec * 1000u)), NULL);
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
 * Create and initialise a mail queue object instance.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osMailQId
osMailCreate (const osMailQDef_t* mail_def,
              osThreadId thread_id __attribute__((unused)))
{
  if (interrupts::in_handler_mode ())
    {
      return nullptr;
    }

  if (mail_def == nullptr)
    {
      return nullptr;
    }

  memory_pool::attributes pool_attr;
  pool_attr.mp_pool_address = mail_def->pool;
  pool_attr.mp_pool_size_bytes = mail_def->pool_sz;
  new ((void*) &mail_def->data->pool) memory_pool (
      mail_def->name, (std::size_t) mail_def->items,
      (std::size_t) mail_def->pool_item_sz, pool_attr);

  message_queue::attributes queue_attr;
  queue_attr.mq_queue_address = mail_def->queue;
  queue_attr.mq_queue_size_bytes = mail_def->queue_sz;
  new ((void*) &mail_def->data->queue) message_queue (
      mail_def->name, (std::size_t) mail_def->items,
      (std::size_t) mail_def->queue_item_sz, queue_attr);

  return (osMailQId) (mail_def->data);
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
osMailAlloc (osMailQId mail_id, uint32_t millisec)
{
  if (mail_id == nullptr)
    {
      return nullptr;
    }

  void* ret = nullptr;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
  if (millisec == osWaitForever)
    {
      if (interrupts::in_handler_mode ())
        {
          return nullptr;
        }
      ret = (reinterpret_cast<memory_pool&> (mail_id->pool)).alloc ();
    }
  else if (millisec == 0)
    {
      ret = (reinterpret_cast<memory_pool&> (mail_id->pool)).try_alloc ();
    }
  else
    {
      if (interrupts::in_handler_mode ())
        {
          return nullptr;
        }
      ret = (reinterpret_cast<memory_pool&> (mail_id->pool)).timed_alloc (
          clock_systick::ticks_cast ((uint64_t) (millisec * 1000u)));
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
osMailCAlloc (osMailQId mail_id, uint32_t millisec)
{
  void* ret = osMailAlloc (mail_id, millisec);
  if (ret != nullptr)
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
      memset (ret, 0,
              (reinterpret_cast<memory_pool&> (mail_id->pool)).block_size ());
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
osMailPut (osMailQId mail_id, void* mail)
{
  if (mail_id == nullptr)
    {
      return osErrorParameter;
    }
  if (mail == nullptr)
    {
      return osErrorValue;
    }

  // Validate pointer.
  memory_pool* pool = reinterpret_cast<memory_pool*> (&mail_id->pool);
  if (((char*) mail < (char*) (pool->pool ()))
      || (((char*) mail)
          >= ((char*) (pool->pool ()) + pool->capacity () * pool->block_size ())))
    {
      return osErrorValue;
    }

  result_t res;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
  res = (reinterpret_cast<message_queue&> (mail_id->queue)).try_send (
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
      if (interrupts::in_handler_mode ())
        {
          event.status = osErrorParameter;
          return event;
        }
      res = (reinterpret_cast<message_queue&> ((mail_id->queue))).receive (
          (char*) &event.value.p, sizeof(void*),
          NULL);
      // osEventMail for ok,
    }
  else if (millisec == 0)
    {
      res = (reinterpret_cast<message_queue&> (mail_id->queue)).try_receive (
          (char*) &event.value.p, sizeof(void*),
          NULL);
      // osEventMail for ok,
    }
  else
    {
      if (interrupts::in_handler_mode ())
        {
          event.status = osErrorParameter;
          return event;
        }
      res = (reinterpret_cast<message_queue&> (mail_id->queue)).timed_receive (
          (char*) &event.value.p, sizeof(void*),
          clock_systick::ticks_cast ((uint64_t) (millisec * 1000u)), NULL);
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
