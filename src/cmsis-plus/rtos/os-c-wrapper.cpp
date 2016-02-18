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

// ----------------------------------------------------------------------------

os_result_t
os_sched_initialize (void)
{
  return static_cast<os_result_t> (scheduler::initialize ());
}

os_result_t
os_sched_start (void)
{
  return static_cast<os_result_t> (scheduler::start ());
}

bool
os_sched_is_started (void)
{
  return scheduler::is_started ();
}

// ----------------------------------------------------------------------------

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
  (reinterpret_cast<Thread&> (thread)).~Thread ();
}

void
os_thread_exit (os_thread_t* thread, void* exit_ptr)
{
  (reinterpret_cast<Thread&> (thread)).exit (exit_ptr);
}

os_result_t
os_thread_join (os_thread_t* thread, void** exit_ptr)
{
  return (reinterpret_cast<Thread&> (thread)).join (exit_ptr);
}

os_thread_prio_t
os_thread_get_prio (os_thread_t* thread)
{
  return (reinterpret_cast<Thread&> (thread)).sched_prio ();
}

os_result_t
os_thread_set_prio (os_thread_t* thread, os_thread_prio_t prio)
{
  return (reinterpret_cast<Thread&> (thread)).sched_prio (prio);
}

void
os_thread_wakeup (os_thread_t* thread)
{
  return (reinterpret_cast<Thread&> (thread)).wakeup ();
}

// ----------------------------------------------------------------------------

void
os_mutex_create (os_mutex_t* mutex, const os_mutex_attr_t* attr)
{
  new (mutex) Mutex ((mutex::Attributes&) *attr);
}

void
os_mutex_destroy (os_mutex_t* mutex)
{
  (reinterpret_cast<Mutex&> (mutex)).~Mutex ();
}

os_result_t
os_mutex_lock (os_mutex_t* mutex)
{
  return (os_result_t) (reinterpret_cast<Mutex&> (mutex)).lock ();
}

os_result_t
os_mutex_try_lock (os_mutex_t* mutex)
{
  return (os_result_t) (reinterpret_cast<Mutex&> (mutex)).try_lock ();
}

os_result_t
os_mutex_timed_lock (os_mutex_t* mutex, os_systicks_t ticks)
{
  return (os_result_t) (reinterpret_cast<Mutex&> (mutex)).timed_lock (ticks);
}

os_result_t
os_mutex_unlock (os_mutex_t* mutex)
{
  return (os_result_t) (reinterpret_cast<Mutex&> (mutex)).unlock ();
}

os_thread_prio_t
os_get_mutex_prio_ceiling (os_mutex_t* mutex)
{
  return (os_thread_prio_t) (reinterpret_cast<Mutex&> (mutex)).prio_ceiling ();
}

os_result_t
os_set_mutex_prio_ceiling (os_mutex_t* mutex, os_thread_prio_t prio_ceiling,
                           os_thread_prio_t* old_prio_ceiling)
{
  return (os_result_t) (reinterpret_cast<Mutex&> (mutex)).prio_ceiling (
      prio_ceiling, old_prio_ceiling);
}

// ----------------------------------------------------------------------------

void
os_condvar_create (os_condvar_t* condvar, os_condvar_attr_t* attr)
{
  new (condvar) Condition_variable ((condvar::Attributes&) *attr);
}

void
os_condvar_destroy (os_condvar_t* condvar)
{
  (reinterpret_cast<Condition_variable&> (condvar)).~Condition_variable ();
}

os_result_t
os_condvar_signal (os_condvar_t* condvar)
{
  return (os_result_t) (reinterpret_cast<Condition_variable&> (condvar)).signal ();
}

os_result_t
os_condvar_broadcast (os_condvar_t* condvar)
{
  return (os_result_t) (reinterpret_cast<Condition_variable&> (condvar)).broadcast ();
}

os_result_t
os_condvar_wait (os_condvar_t* condvar, os_mutex_t* mutex)
{
  return (os_result_t) (reinterpret_cast<Condition_variable&> (condvar)).wait (
      (Mutex&) *mutex);
}

os_result_t
os_condvar_timed_wait (os_condvar_t* condvar, os_mutex_t* mutex,
                       os_systicks_t ticks)
{
  return (os_result_t) (reinterpret_cast<Condition_variable&> (condvar)).timed_wait (
      (Mutex&) *mutex, ticks);
}

// ----------------------------------------------------------------------------

void
os_semaphore_create (os_semaphore_t* semaphore, os_semaphore_attr_t* attr)
{
  new (semaphore) Semaphore ((semaphore::Attributes&) *attr);
}

void
os_semaphore_destroy (os_semaphore_t* semaphore)
{
  (reinterpret_cast<Semaphore&> (semaphore)).~Semaphore ();
}

os_result_t
os_semaphore_post (os_semaphore_t* semaphore)
{
  return (os_result_t) (reinterpret_cast<Semaphore&> (semaphore)).post ();
}

os_result_t
os_semaphore_wait (os_semaphore_t* semaphore)
{
  return (os_result_t) (reinterpret_cast<Semaphore&> (semaphore)).wait ();
}

os_result_t
os_semaphore_try_wait (os_semaphore_t* semaphore)
{
  return (os_result_t) (reinterpret_cast<Semaphore&> (semaphore)).try_wait ();
}

os_result_t
os_semaphore_timed_wait (os_semaphore_t* semaphore, os_systicks_t ticks)
{
  return (os_result_t) (reinterpret_cast<Semaphore&> (semaphore)).timed_wait (
      ticks);
}

os_semaphore_count_t
os_semaphore_get_value (os_semaphore_t* semaphore)
{
  return (os_semaphore_count_t) (reinterpret_cast<Semaphore&> (semaphore)).value ();
}

os_result_t
os_semaphore_reset (os_semaphore_t* semaphore)
{
  return (os_result_t) (reinterpret_cast<Semaphore&> (semaphore)).reset ();
}

// ----------------------------------------------------------------------------

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
  (reinterpret_cast<Memory_pool&> (mempool)).~Memory_pool ();
}

void*
os_mempool_alloc (os_mempool_t* mempool)
{
  return (reinterpret_cast<Memory_pool&> (mempool)).alloc ();
}

void*
os_mempool_try_alloc (os_mempool_t* mempool)
{
  return (reinterpret_cast<Memory_pool&> (mempool)).try_alloc ();
}

void*
os_mempool_timed_alloc (os_mempool_t* mempool, os_systicks_t ticks)
{
  return (reinterpret_cast<Memory_pool&> (mempool)).timed_alloc (ticks);
}

os_result_t
os_mempool_free (os_mempool_t* mempool, void* block)
{
  return (os_result_t) (reinterpret_cast<Memory_pool&> (mempool)).free (block);
}

size_t
os_mempool_get_size (os_mempool_t* mempool)
{
  return (reinterpret_cast<Memory_pool&> (mempool)).size ();
}

size_t
os_mempool_get_count (os_mempool_t* mempool)
{
  return (reinterpret_cast<Memory_pool&> (mempool)).count ();
}

size_t
os_mempool_get_block_size (os_mempool_t* mempool)
{
  return (reinterpret_cast<Memory_pool&> (mempool)).block_size ();
}

bool
os_mempool_is_empty (os_mempool_t* mempool)
{
  return (reinterpret_cast<Memory_pool&> (mempool)).is_empty ();
}

bool
os_mempool_is_full (os_mempool_t* mempool)
{
  return (reinterpret_cast<Memory_pool&> (mempool)).is_full ();
}

os_result_t
os_mempool_reset (os_mempool_t* mempool)
{
  return (os_result_t) (reinterpret_cast<Memory_pool&> (mempool)).reset ();
}

// --------------------------------------------------------------------------

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
  (reinterpret_cast<Message_queue&> (mqueue)).~Message_queue ();
}

os_result_t
os_mqueue_send (os_mqueue_t* mqueue, const char* msg, size_t nbytes,
                os_mqueue_prio_t mprio)
{
  return (os_result_t) (reinterpret_cast<Message_queue&> (mqueue)).send (msg,
                                                                         nbytes,
                                                                         mprio);
}

os_result_t
os_mqueue_try_send (os_mqueue_t* mqueue, const char* msg, size_t nbytes,
                    os_mqueue_prio_t mprio)
{
  return (os_result_t) (reinterpret_cast<Message_queue&> (mqueue)).try_send (
      msg, nbytes, mprio);
}

os_result_t
os_mqueue_timed_send (os_mqueue_t* mqueue, const char* msg, size_t nbytes,
                      os_mqueue_prio_t mprio, os_systicks_t ticks)
{
  return (os_result_t) (reinterpret_cast<Message_queue&> (mqueue)).timed_send (
      msg, nbytes, mprio, ticks);
}

os_result_t
os_mqueue_receive (os_mqueue_t* mqueue, const char* msg, size_t nbytes,
                   os_mqueue_prio_t* mprio)
{
  return (os_result_t) (reinterpret_cast<Message_queue&> (mqueue)).receive (
      msg, nbytes, mprio);
}

os_result_t
os_mqueue_try_receive (os_mqueue_t* mqueue, const char* msg, size_t nbytes,
                       os_mqueue_prio_t* mprio)
{
  return (os_result_t) (reinterpret_cast<Message_queue&> (mqueue)).try_receive (
      msg, nbytes, mprio);
}

os_result_t
os_mqueue_timed_receive (os_mqueue_t* mqueue, const char* msg, size_t nbytes,
                         os_mqueue_prio_t* mprio, os_systicks_t ticks)
{
  return (os_result_t) (reinterpret_cast<Message_queue&> (mqueue)).timed_receive (
      msg, nbytes, mprio, ticks);
}

size_t
os_mqueue_get_length (os_mqueue_t* mqueue)
{
  return (reinterpret_cast<Message_queue&> (mqueue)).length ();
}

size_t
os_mqueue_get_size (os_mqueue_t* mqueue)
{
  return (reinterpret_cast<Message_queue&> (mqueue)).size ();
}

size_t
os_mqueue_get_msg_size (os_mqueue_t* mqueue)
{
  return (reinterpret_cast<Message_queue&> (mqueue)).msg_size ();
}

bool
os_mqueue_get_is_empty (os_mqueue_t* mqueue)
{
  return (reinterpret_cast<Message_queue&> (mqueue)).is_empty ();
}

bool
os_mqueue_get_is_full (os_mqueue_t* mqueue)
{
  return (reinterpret_cast<Message_queue&> (mqueue)).is_full ();
}

os_result_t
os_mqueue_reset (os_mqueue_t* mqueue)
{
  return (os_result_t) (reinterpret_cast<Message_queue&> (mqueue)).reset ();
}

// ****************************************************************************
// ***** Legacy CMSIS RTOS implementation *****

#include <cmsis_os.h>

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
 */
osStatus
osKernelInitialize (void)
{
  if (scheduler::is_in_isr ())
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
 */
osStatus
osKernelStart (void)
{
  return static_cast<osStatus> (scheduler::start ());
}

/**
 * @details
 * Identifies if the RTOS scheduler is started. For systems with the
 * option to start the main function as a thread this allows
 * you to identify that the RTOS scheduler is already running.
 */
int32_t
osKernelRunning (void)
{
  return scheduler::is_started () ? 1 : 0;
}

#if (defined (osFeature_SysTick)  &&  (osFeature_SysTick != 0))

uint32_t
osKernelSysTick (void)
{
  Systick_clock::current_t crt;
  Systick_clock::now (&crt);
  // Convert ticks to cycles.
  return static_cast<uint32_t> (crt.ticks) * crt.divisor + crt.cycles;
}

#endif    // System Timer available

// ----------------------------------------------------------------------------
//  ==== Thread Management ====

osThreadId
osThreadCreate (const osThreadDef_t* thread_def, void* args)
{
  Thread* thread = new (thread_def->data) Thread (
      (thread::func_t) thread_def->pthread, args);
  return reinterpret_cast<osThreadId> (thread);
}

osThreadId
osThreadGetId (void)
{
  return reinterpret_cast<osThreadId> (&this_thread::thread ());
}

osStatus
osThreadTerminate (osThreadId thread_id)
{
  // TODO: check if something needs to be done to terminate.
  (reinterpret_cast<Thread&> (thread_id)).~Thread ();
  return osOK;
}

/**
 * @details Pass control to next thread that is in state @b READY.
 *
 */
osStatus
osThreadYield (void)
{
  if (scheduler::is_in_isr ())
    {
      return osErrorISR;
    }

  this_thread::yield ();

  return osOK;
}

osStatus
osThreadSetPriority (osThreadId thread_id, osPriority priority)
{
  thread::priority_t prio = static_cast<thread::priority_t> (priority * 10);
  return static_cast<osStatus> ((reinterpret_cast<Thread&> (thread_id)).sched_prio (
      prio));
}

osPriority
osThreadGetPriority (osThreadId thread_id)
{
  thread::priority_t prio =
      (reinterpret_cast<Thread&> (thread_id)).sched_prio ();
  return static_cast<osPriority> (prio / 10);
}

// ----------------------------------------------------------------------------
//  ==== Generic Wait Functions ====

osStatus
osDelay (uint32_t millisec)
{
  return static_cast<osStatus> (Systick_clock::sleep_for (
      Systick_clock::ticks_cast (millisec * 1000u)));
}

#if (defined (osFeature_Wait)  &&  (osFeature_Wait != 0))

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

osEvent
osWait (uint32_t millisec)
{
  osEvent event;

  result_t res = Systick_clock::sleep_for (
      Systick_clock::ticks_cast (millisec));
  // TODO: return events
  event.status = static_cast<osStatus> (res);
  return event;
}

#pragma GCC diagnostic pop

#endif  // Generic Wait available

// ----------------------------------------------------------------------------
//  ==== Timer Management Functions ====

osTimerId
osTimerCreate (const osTimerDef_t* timer_def, os_timer_type type, void* args)
{
  timer::Attributes attr
    { timer_def->name };
  attr.tm_type = (timer::type_t) type;

  return reinterpret_cast<osTimerId> (new ((void*) &timer_def->data) Timer (
      attr, (timer::func_t) timer_def->ptimer, (timer::func_args_t) args));
}

osStatus
osTimerStart (osTimerId timer_id, uint32_t millisec)
{
  return static_cast<osStatus> ((reinterpret_cast<Timer&> (timer_id)).start (
      Systick_clock::ticks_cast (millisec)));
}

osStatus
osTimerStop (osTimerId timer_id)
{
  return static_cast<osStatus> ((reinterpret_cast<Timer&> (timer_id)).stop ());
}

osStatus
osTimerDelete (osTimerId timer_id)
{
  (reinterpret_cast<Timer&> (timer_id)).~Timer ();
  return osOK;
}

// ----------------------------------------------------------------------------
//  ==== Signal Management ====

int32_t
osSignalSet (osThreadId thread_id, int32_t signals)
{
  int32_t ret;
  flags::set ((Thread&) (*thread_id), (event_flags_t) signals,
              (event_flags_t*) &ret);
  return ret;
}

int32_t
osSignalClear (osThreadId thread_id, int32_t signals)
{
  int32_t ret;
  flags::clear ((Thread&) (*thread_id), (event_flags_t) signals,
                (event_flags_t*) &ret);
  return ret;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

osEvent
osSignalWait (int32_t signals, uint32_t millisec)
{
  osEvent event;
  result_t res;
  if (millisec == osWaitForever)
    {
      res = flags::wait ((event_flags_t) signals,
                         (event_flags_t*) &event.value.signals);
    }
  else if (millisec == 0)
    {
      res = flags::try_wait ((event_flags_t) signals,
                             (event_flags_t*) &event.value.signals);
    }
  else
    {
      res = flags::timed_wait ((event_flags_t) signals,
                               (event_flags_t*) &event.value.signals,
                               Systick_clock::ticks_cast (millisec * 1000u));
    }
  // TODO: set osEventSignal, osEventTimeout
  event.status = static_cast<osStatus> (res);
  return event;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

//  ==== Mutex Management ====

osMutexId
osMutexCreate (const osMutexDef_t* mutex_def)
{
  return reinterpret_cast<osMutexId> (new ((void*) &mutex_def->data) Mutex ());
}

osStatus
osMutexWait (osMutexId mutex_id, uint32_t millisec)
{
  result_t status;
  if (millisec == osWaitForever)
    {
      status = (reinterpret_cast<Mutex&> (mutex_id)).lock ();
    }
  else if (millisec == 0)
    {
      status = (reinterpret_cast<Mutex&> (mutex_id)).try_lock ();
    }
  else
    {
      status = (reinterpret_cast<Mutex&> (mutex_id)).timed_lock (
          Systick_clock::ticks_cast (millisec * 1000u));
    }

  // TODO: return legacy code for POSIX codes
  return static_cast<osStatus> (status);
}

osStatus
osMutexRelease (osMutexId mutex_id)
{
  result_t status;
  status = (reinterpret_cast<Mutex&> (mutex_id)).unlock ();

  // TODO: return legacy code for POSIX codes
  return static_cast<osStatus> (status);
}

osStatus
osMutexDelete (osMutexId mutex_id)
{
  (reinterpret_cast<Mutex&> (mutex_id)).~Mutex ();
  return osOK;
}

// ----------------------------------------------------------------------------

//  ==== Semaphore Management Functions ====

#if (defined (osFeature_Semaphore)  &&  (osFeature_Semaphore != 0))

osSemaphoreId
osSemaphoreCreate (const osSemaphoreDef_t* semaphore_def, int32_t count)
{
  semaphore::Attributes attr
    { semaphore_def->name };
  attr.sm_initial_count = (semaphore::count_t) count;
  return reinterpret_cast<osSemaphoreId> (new ((void*) &semaphore_def->data) Semaphore (
      attr));
}

int32_t
osSemaphoreWait (osSemaphoreId semaphore_id, uint32_t millisec)
{
  if (millisec == osWaitForever)
    {
      (reinterpret_cast<Semaphore&> (semaphore_id)).wait ();
    }
  else if (millisec == 0)
    {
      (reinterpret_cast<Semaphore&> (semaphore_id)).try_wait ();
    }
  else
    {
      (reinterpret_cast<Semaphore&> (semaphore_id)).timed_wait (
          Systick_clock::ticks_cast (millisec * 1000u));
    }

  return (int32_t) (reinterpret_cast<Semaphore&> (semaphore_id)).value ();
}

osStatus
osSemaphoreRelease (osSemaphoreId semaphore_id)
{
  return static_cast<osStatus> ((reinterpret_cast<Semaphore&> (semaphore_id)).post ());
}

osStatus
osSemaphoreDelete (osSemaphoreId semaphore_id)
{
  (reinterpret_cast<Semaphore&> (semaphore_id)).~Semaphore ();
  return osOK;
}

#endif /* Semaphore available */

// ----------------------------------------------------------------------------
//  ==== Memory Pool Management Functions ====

#if (defined (osFeature_Pool)  &&  (osFeature_Pool != 0))

osPoolId
osPoolCreate (const osPoolDef_t* pool_def)
{
  mempool::Attributes attr
    { pool_def->name };
  attr.mp_pool_address = pool_def->pool;
  return reinterpret_cast<osPoolId> (new ((void*) &pool_def->data) Memory_pool (
      (mempool::size_t) pool_def->pool_sz, (mempool::size_t) pool_def->item_sz));
}

void*
osPoolAlloc (osPoolId pool_id)
{
  return (reinterpret_cast<Memory_pool&> (pool_id)).try_alloc ();
}

void*
osPoolCAlloc (osPoolId pool_id)
{
  void* ret;
  ret = (reinterpret_cast<Memory_pool&> (pool_id)).try_alloc ();
  if (ret != nullptr)
    {
      memset (ret, 0, (reinterpret_cast<Memory_pool&> (pool_id)).block_size ());
    }
  return ret;
}

osStatus
osPoolFree (osPoolId pool_id, void* block)
{
  return static_cast<osStatus> ((reinterpret_cast<Memory_pool&> (pool_id)).free (
      block));
}

#endif /* Memory Pool Management available */

// ----------------------------------------------------------------------------
//  ==== Message Queue Management Functions ====

#if (defined (osFeature_MessageQ)  &&  (osFeature_MessageQ != 0))

osMessageQId
osMessageCreate (const osMessageQDef_t* queue_def,
                 osThreadId thread_id __attribute__((unused)))
{
  mqueue::Attributes attr
    { queue_def->name };
  attr.queue_address = queue_def->queue;
  attr.queue_size_bytes = queue_def->queue_sz;

  return reinterpret_cast<osMessageQId> (new ((void*) &queue_def->data) Message_queue (
      attr, (mqueue::size_t) queue_def->items,
      (mqueue::size_t) queue_def->item_sz));
}

osStatus
osMessagePut (osMessageQId queue_id, uint32_t info, uint32_t millisec)
{
#pragma GCC diagnostic push
#if defined ( __clang__ )
#pragma clang diagnostic ignored "-Wint-to-pointer-cast"
#endif
  if (millisec == osWaitForever)
    {
      return static_cast<osStatus> ((reinterpret_cast<Message_queue&> (queue_id)).send (
          (const char*) info, sizeof(uint32_t), 0));
    }
  else if (millisec == 0)
    {
      return static_cast<osStatus> ((reinterpret_cast<Message_queue&> (queue_id)).try_send (
          (const char*) info, sizeof(uint32_t), 0));
    }
  else
    {
      return static_cast<osStatus> ((reinterpret_cast<Message_queue&> (queue_id)).timed_send (
          (const char*) info, sizeof(uint32_t), 0,
          Systick_clock::ticks_cast (millisec * 1000u)));
    }
#pragma GCC diagnostic pop
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

osEvent
osMessageGet (osMessageQId queue_id, uint32_t millisec)
{
  osEvent event;
  uint32_t msg;
  result_t res;
  if (millisec == osWaitForever)
    {
      res = (reinterpret_cast<Message_queue&> (queue_id)).receive (
          (const char*) &msg, sizeof(uint32_t), NULL);
      // result::event_message;
    }
  else if (millisec == 0)
    {
      res = (reinterpret_cast<Message_queue&> (queue_id)).try_receive (
          (const char*) &msg, sizeof(uint32_t), NULL);
      // result::event_message when message;
      // result::ok when no meessage
    }
  else
    {
      res = (reinterpret_cast<Message_queue&> (queue_id)).timed_receive (
          (const char*) &msg, sizeof(uint32_t), NULL,
          Systick_clock::ticks_cast (millisec * 1000u));
      // result::event_message when message;
      // result::event_timeout when timeout;
    }

  // TODO: be sure osEventMessage is returned when appropriate.
  event.status = static_cast<osStatus> (res);
  event.value.v = msg;
  return event;
}

#pragma GCC diagnostic pop

#endif /* Message Queues available */

// ----------------------------------------------------------------------------
//  ==== Mail Queue Management Functions ====

#if (defined (osFeature_MailQ)  &&  (osFeature_MailQ != 0))

osMailQId
osMailCreate (const osMailQDef_t* queue_def,
              osThreadId thread_id __attribute__((unused)))
{
  mempool::Attributes pool_attr
    { queue_def->name };
  pool_attr.mp_pool_address = queue_def->pool;
  new ((void*) &queue_def->data->pool) Memory_pool (
      (mempool::size_t) queue_def->pool_sz,
      (mempool::size_t) queue_def->pool_item_sz);

  mqueue::Attributes queue_attr
    { queue_def->name };
  queue_attr.queue_address = queue_def->queue;
  queue_attr.queue_size_bytes = queue_def->queue_sz;
  new ((void*) &queue_def->data->queue) Message_queue (
      queue_attr, (mqueue::size_t) queue_def->items,
      (mqueue::size_t) queue_def->queue_item_sz);

  return (osMailQId) (&queue_def->data);
}

void*
osMailAlloc (osMailQId queue_id, uint32_t millisec)
{
  void* ret = nullptr;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
  if (millisec == osWaitForever)
    {
      ret = (reinterpret_cast<Memory_pool&> (queue_id->pool)).alloc ();
    }
  else if (millisec == 0)
    {
      ret = (reinterpret_cast<Memory_pool&> (queue_id->pool)).try_alloc ();
    }
  else
    {
      ret = (reinterpret_cast<Memory_pool&> (queue_id->pool)).timed_alloc (
          Systick_clock::ticks_cast (millisec * 1000u));
    }
#pragma GCC diagnostic pop
  return ret;
}

void*
osMailCAlloc (osMailQId queue_id, uint32_t millisec)
{
  void* ret = osMailAlloc (queue_id, millisec);
  if (ret != nullptr)
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
      memset (
          ret, 0,
          (reinterpret_cast<Memory_pool&> ((queue_id->pool))).block_size ());
#pragma GCC diagnostic pop
    }
  return ret;
}

osStatus
osMailPut (osMailQId queue_id, void* mail)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
  return static_cast<osStatus> ((reinterpret_cast<Message_queue&> ((queue_id->queue))).try_send (
      (const char*) mail, sizeof(void*), 0));
#pragma GCC diagnostic pop
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

osEvent
osMailGet (osMailQId queue_id, uint32_t millisec)
{
  osEvent event;
  void* msg;
  result_t res;
  if (millisec == osWaitForever)
    {
      res = (reinterpret_cast<Message_queue&> (queue_id)).receive (
          (const char*) &msg, sizeof(void*), NULL);
    }
  else if (millisec == 0)
    {
      res = (reinterpret_cast<Message_queue&> (queue_id)).try_receive (
          (const char*) &msg, sizeof(void*), NULL);
    }
  else
    {
      res = (reinterpret_cast<Message_queue&> (queue_id)).timed_receive (
          (const char*) &msg, sizeof(void*), NULL,
          Systick_clock::ticks_cast (millisec * 1000u));
    }

  event.status = static_cast<osStatus> (res);
  if (event.status == osEventMessage)
    {
      event.status = osEventMail;
    }
  event.value.p = msg;
  return event;
}

#pragma GCC diagnostic pop

osStatus
osMailFree (osMailQId queue_id, void* mail)
{
  return osPoolFree (&(queue_id->pool), mail);
}

#endif /* Mail Queues available */

// ----------------------------------------------------------------------------

#if 0

osThreadId
osThreadCreateEx (osThread* addr, const osThreadAttr* attr, os_pthread function,
    const void* args)
  {
    Thread* thread = new (addr) Thread ((thread::Attributes&) *attr,
        (thread::func_t) function,
        (thread::func_args_t) args);
    return reinterpret_cast<osThreadId> (thread);
  }

osTimerId
osTimerCreateEx (osTimer* addr, osTimerAttr* attr, os_ptimer function,
    void* args)
  {
    return reinterpret_cast<osTimerId> (new ((void*) addr) Timer (
            (timer::Attributes&) (*attr), (timer::func_t) function,
            (timer::func_args_t) args));
  }

osMutexId
osMutexCreateEx (osMutex* addr, const osMutexAttr* attr)
  {
    return reinterpret_cast<osMutexId> (new ((void*) addr) Mutex (
            (const mutex::Attributes&) *attr));
  }

osSemaphoreId
osSemaphoreCreateEx (osSemaphoreId addr, const osSemaphoreAttr* attr)
  {
    return reinterpret_cast<osSemaphoreId> (new ((void*) addr) Semaphore (
            (const semaphore::Attributes&) *attr));
  }

osPoolId
osPoolCreateEx (osPool* addr, const osPoolAttr* attr, size_t items,
    size_t item_size_bytes)
  {
    return reinterpret_cast<osPoolId> (new ((void*) addr) Memory_pool (
            (mempool::Attributes&) *attr, (mempool::size_t) items,
            (mempool::size_t) item_size_bytes));
  }

void
osPoolDeleteEx (osPoolId pool_id)
  {
    (reinterpret_cast<Memory_pool&> (pool_id)).~Memory_pool ();
  }

osMessageQId
osMessageCreateEx (osMessageQ* addr, const osMessageQAttr* attr, size_t items,
    size_t item_size)
  {
    return reinterpret_cast<osMessageQId> (new ((void*) addr) Message_queue (
            (mqueue::Attributes&) (*attr), (mqueue::size_t) items,
            (mqueue::size_t) item_size));
  }

void
osMessageDeleteEx (osMessageQId queue_id)
  {
    (reinterpret_cast<Message_queue&> (queue_id)).~Message_queue ();
  }

void
osMailDeleteEx (osMailQId queue_id)
  {
    osPoolDeleteEx (&(queue_id->pool));
    osMessageDeleteEx (&(queue_id->queue));
  }

#endif

// ----------------------------------------------------------------------------
