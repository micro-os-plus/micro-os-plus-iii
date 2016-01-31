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
 * replacement for C++ applications.
 *
 * The code provides a CMSIS RTOS compliant C implementation using the
 * CMSIC++ RTOS definitions.
 */

#include <cmsis_os_ex.h>
#include <cmsis-plus/rtos/os.h>

#include <new>

using namespace os::cmsis::rtos;

// ----------------------------------------------------------------------------

// Validate C structs sizes (should match the C++ objects sizes).

static_assert(sizeof(Thread) == sizeof(osThread), "adjust size of osThread");
static_assert(sizeof(thread_attr_t) == sizeof(osThreadAttr), "adjust size of osThreadAttr");

static_assert(sizeof(Timer) == sizeof(osTimer), "adjust size of osTimer");
static_assert(sizeof(Mutex) == sizeof(osMutex), "adjust size of osMutex");
static_assert(sizeof(Semaphore) == sizeof(osSemaphore), "adjust size of osSemaphore");
static_assert(sizeof(Pool) == sizeof(osPool), "adjust size of osPool");
static_assert(sizeof(Message_queue) == sizeof(osMessageQ), "adjust size of osMessageQ");
static_assert(sizeof(Mail_queue) == sizeof(osMailQ), "adjust size of osMailQ");

// ----------------------------------------------------------------------------

//  ==== Kernel Control Functions ====

osStatus
osKernelInitialize (void)
{
  return static_cast<osStatus> (kernel::initialize ());
}

osStatus
osKernelStart (void)
{
  return static_cast<osStatus> (kernel::start ());
}

int32_t
osKernelRunning (void)
{
  return kernel::is_running ();
}

#if (defined (osFeature_SysTick)  &&  (osFeature_SysTick != 0))     // System Timer available

uint32_t
osKernelSysTick (void)
{
  current_systick_t crt;
  kernel::get_current_systick (&crt);
  // Convert ticks to cycles.
  return static_cast<uint32_t> (crt.ticks) * crt.divisor + crt.cycles;
}

#endif    // System Timer available

// ----------------------------------------------------------------------------

//  ==== Thread Management ====

osThreadId
osThreadCreate (const osThreadDef_t *thread_def, void *args)
{
  return reinterpret_cast<osThreadId> (new (thread_def->data) Thread (
      (thread_attr_t*) nullptr, (thread_func_vp_t) thread_def->pthread, args));
}

#if 1
osThreadId
osThreadCreateEx (osThread* addr, const osThreadAttr* attr, os_pthread function,
                  const void* args)
{
  return reinterpret_cast<osThreadId> (new (addr) Thread (
      (thread_attr_t*) attr, (thread_func_vp_t) function, (void*) args));
}
#endif

osThreadId
osThreadGetId (void)
{
  return reinterpret_cast<osThreadId> (&thread::get_current ());
}

osStatus
osThreadTerminate (osThreadId thread_id)
{
  (reinterpret_cast<Thread&> (thread_id)).~Thread ();
  return osOK;
}

osStatus
osThreadYield (void)
{
  return static_cast<osStatus> (thread::yield ());
}

osStatus
osThreadSetPriority (osThreadId thread_id, osPriority priority)
{
  return static_cast<osStatus> ((reinterpret_cast<Thread&> (thread_id)).set_sched_prio (
      (priority_t) priority));
}

osPriority
osThreadGetPriority (osThreadId thread_id)
{
  return static_cast<osPriority> ((reinterpret_cast<Thread&> (thread_id)).get_sched_prio ());
}

// ----------------------------------------------------------------------------

//  ==== Generic Wait Functions ====

osStatus
osDelay (uint32_t millisec)
{
  return static_cast<osStatus> (thread::sleep (
      kernel::compute_sys_ticks (millisec * 1000)));
}

#if (defined (osFeature_Wait)  &&  (osFeature_Wait != 0))

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

osEvent
osWait (uint32_t millisec)
{
  osEvent event;

  status_t ret = thread::wait (millisec, (event_t*) &event);
  event.status = static_cast<osStatus> (ret);
  return event;
}

#pragma GCC diagnostic pop

osStatus
osWaitEx (uint32_t millisec)
{
  return static_cast<osStatus> (thread::wait (millisec, nullptr));
}

#endif  // Generic Wait available

// ----------------------------------------------------------------------------

//  ==== Timer Management Functions ====

osTimerId
osTimerCreate (const osTimerDef_t *timer_def, os_timer_type type, void *args)
{
  return reinterpret_cast<osTimerId> (new ((void*) &timer_def->data) Timer (
      timer_def->name, timer_def->ptimer, (timer_type_t) type, args));
}

osTimerId
osTimerCreateEx (osTimer* addr, const char* name, os_ptimer function,
                 os_timer_type type, void* args)
{
  return reinterpret_cast<osTimerId> (new ((void*) addr) Timer (
      name, function, (timer_type_t) type, args));
}

osStatus
osTimerStart (osTimerId timer_id, uint32_t millisec)
{
  return static_cast<osStatus> ((reinterpret_cast<Timer&> (timer_id)).start (
      millisec));
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

#if 0
int32_t
osSignalSet (osThreadId thread_id, int32_t signals)
  {
    return static_cast<int32_t> (((Thread&) (thread_id)).set_signals (signals));
  }

int32_t
osSignalClear (osThreadId thread_id, int32_t signals)
  {
    return static_cast<int32_t> (((Thread&) (thread_id)).clear_signals (signals));
  }
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

osEvent
osSignalWait (int32_t signals, uint32_t millisec)
{
  osEvent event;
  status_t ret = thread::wait_signals (signals, millisec,
                                       (signal_flags_t*) &event.value.signals);
  event.status = static_cast<osStatus> (ret);
  return event;
}

#pragma GCC diagnostic pop

osStatus
osSignalWaitEx (int32_t signals, uint32_t millisec)
{
  return static_cast<osStatus> (thread::wait_signals (signals, millisec,
                                                      nullptr));
}

// ----------------------------------------------------------------------------

//  ==== Mutex Management ====

osMutexId
osMutexCreate (const osMutexDef_t *mutex_def)
{
  return reinterpret_cast<osMutexId> (new ((void*) &mutex_def->data) Mutex (
      mutex_def->name));
}

osMutexId
osMutexCreateEx (osMutex* addr, const char* name)
{
  return reinterpret_cast<osMutexId> (new ((void*) addr) Mutex (name));
}

osStatus
osMutexWait (osMutexId mutex_id, uint32_t millisec)
{

  return static_cast<osStatus> ((reinterpret_cast<Mutex&> (mutex_id)).try_wait (
      kernel::compute_sys_ticks (millisec * 1000)));
}

osStatus
osMutexRelease (osMutexId mutex_id)
{
  return static_cast<osStatus> ((reinterpret_cast<Mutex&> (mutex_id)).release ());
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
osSemaphoreCreate (const osSemaphoreDef_t *semaphore_def, int32_t count)
{
  return reinterpret_cast<osSemaphoreId> (new ((void*) &semaphore_def->data) Semaphore (
      semaphore_def->name, count));
}

osSemaphoreId
osSemaphoreCreateEx (osSemaphoreId addr, const char* name,
                     int32_t initial_count, uint32_t max_count)
{
  return reinterpret_cast<osSemaphoreId> (new ((void*) addr) Semaphore (
      name, initial_count, max_count));
}

int32_t
osSemaphoreWait (osSemaphoreId semaphore_id, uint32_t millisec)
{
  return static_cast<osStatus> ((reinterpret_cast<Semaphore&> (semaphore_id)).wait (
      millisec));
}

osStatus
osSemaphoreRelease (osSemaphoreId semaphore_id)
{
  return static_cast<osStatus> ((reinterpret_cast<Semaphore&> (semaphore_id)).release ());
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
osPoolCreate (const osPoolDef_t *pool_def)
{
  return reinterpret_cast<osPoolId> (new ((void*) &pool_def->data) Pool (
      pool_def->name, pool_def->pool_sz, pool_def->item_sz, pool_def->pool));
}

osPoolId
osPoolCreateEx (osPool* addr, const char* name, size_t items,
                uint32_t item_size, void* mem)
{
  return reinterpret_cast<osPoolId> (new ((void*) addr) Pool (name, items,
                                                              item_size, mem));
}

void *
osPoolAlloc (osPoolId pool_id)
{
  return (reinterpret_cast<Pool&> (pool_id)).alloc ();
}

void *
osPoolCAlloc (osPoolId pool_id)
{
  return (reinterpret_cast<Pool&> (pool_id)).calloc ();
}

osStatus
osPoolFree (osPoolId pool_id, void *block)
{
  return static_cast<osStatus> ((reinterpret_cast<Pool&> (pool_id)).free (block));
}

void
osPoolDeleteEx (osPoolId pool_id)
{
  (reinterpret_cast<Pool&> (pool_id)).~Pool ();
}

#endif /* Memory Pool Management available */

// ----------------------------------------------------------------------------

//  ==== Message Queue Management Functions ====

#if (defined (osFeature_MessageQ)  &&  (osFeature_MessageQ != 0))

osMessageQId
osMessageCreate (const osMessageQDef_t *queue_def, osThreadId thread_id)
{
  return reinterpret_cast<osMessageQId> (new ((void*) &queue_def->data) Message_queue (
      queue_def->name, queue_def->queue_sz, queue_def->pool,
      reinterpret_cast<Thread*> (thread_id)));
}

osMessageQId
osMessageCreateEx (osMessageQ* addr, const char* name, size_t items, void* mem,
                   osThreadId thread_id)
{
  return reinterpret_cast<osMessageQId> (new ((void*) addr) Message_queue (
      name, items, mem, reinterpret_cast<Thread*> (thread_id)));
}

osStatus
osMessagePut (osMessageQId queue_id, uint32_t info, uint32_t millisec)
{
#pragma GCC diagnostic push
#if defined ( __clang__ )
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"
#endif
  return static_cast<osStatus> ((reinterpret_cast<Message_queue&> (queue_id)).put (
      (void*) info, millisec));
#pragma GCC diagnostic pop
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

osEvent
osMessageGet (osMessageQId queue_id, uint32_t millisec)
{
  osEvent event;
  status_t ret = (reinterpret_cast<Message_queue&> (queue_id)).get (
      millisec, (void**) &event.value.p);
  event.status = static_cast<osStatus> (ret);
  return event;
}

#pragma GCC diagnostic pop

osStatus
osMessageGetEx (osMessageQId queue_id, uint32_t millisec)
{
  return static_cast<osStatus> ((reinterpret_cast<Message_queue&> (queue_id)).get (
      millisec, nullptr));
}

void
osMessageDeleteEx (osMessageQId queue_id)
{
  (reinterpret_cast<Message_queue&> (queue_id)).~Message_queue ();
}

#endif /* Message Queues available */

// ----------------------------------------------------------------------------

//  ==== Mail Queue Management Functions ====

#if (defined (osFeature_MailQ)  &&  (osFeature_MailQ != 0))

osMailQId
osMailCreate (const osMailQDef_t *queue_def, osThreadId thread_id)
{
  return reinterpret_cast<osMailQId> (new ((void*) &queue_def->data) Mail_queue (
      queue_def->name, queue_def->queue_sz, queue_def->item_sz, queue_def->pool,
      reinterpret_cast<Thread*> (thread_id)));
}

osMailQId
osMailCreateEx (osMailQ* addr, const char* name, size_t messages,
                size_t message_size, void* mem, osThreadId thread_id)
{
  return reinterpret_cast<osMailQId> (new ((void*) addr) Mail_queue (
      name, messages, message_size, mem, reinterpret_cast<Thread*> (thread_id)));
}

void *
osMailAlloc (osMailQId queue_id, uint32_t millisec)
{
  return (reinterpret_cast<Mail_queue&> (queue_id)).alloc (millisec);
}

void *
osMailCAlloc (osMailQId queue_id, uint32_t millisec)
{
  return (reinterpret_cast<Mail_queue&> (queue_id)).calloc (millisec);
}

osStatus
osMailPut (osMailQId queue_id, void *mail)
{
  return static_cast<osStatus> ((reinterpret_cast<Mail_queue&> (queue_id)).put (
      mail));
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

osEvent
osMailGet (osMailQId queue_id, uint32_t millisec)
{
  osEvent event;
  status_t ret = (reinterpret_cast<Mail_queue&> (queue_id)).get (
      millisec, (void**) &event.value.p);
  event.status = static_cast<osStatus> (ret);
  return event;
}

#pragma GCC diagnostic pop

osStatus
osMailGetEx (osMailQId queue_id, uint32_t millisec)
{
  return static_cast<osStatus> ((reinterpret_cast<Mail_queue&> (queue_id)).get (
      millisec, nullptr));
}

osStatus
osMailFree (osMailQId queue_id, void *mail)
{
  return static_cast<osStatus> ((reinterpret_cast<Mail_queue&> (queue_id)).free (
      mail));
}

void
osMailDeleteEx (osMailQId queue_id)
{
  (reinterpret_cast<Mail_queue&> (queue_id)).~Mail_queue ();
}

#endif /* Mail Queues available */

// ----------------------------------------------------------------------------
