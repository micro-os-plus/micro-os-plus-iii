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

#include <cstring>
#include <new>

using namespace os::rtos;

// ----------------------------------------------------------------------------

// Validate C structs sizes (should match the C++ objects sizes).
// TODO: validate individual members (size & offset).

static_assert(sizeof(Thread) == sizeof(osThread), "adjust size of osThread");
static_assert(sizeof(thread::Attributes) == sizeof(osThreadAttr), "adjust size of osThreadAttr");

static_assert(sizeof(Timer) == sizeof(osTimer), "adjust size of osTimer");
static_assert(sizeof(timer::Attributes) == sizeof(osTimerAttr), "adjust size of osTimerAttr");

static_assert(sizeof(Mutex) == sizeof(osMutex), "adjust size of osMutex");
static_assert(sizeof(mutex::Attributes) == sizeof(osMutexAttr), "adjust size of osMutexAttr");

static_assert(sizeof(Semaphore) == sizeof(osSemaphore), "adjust size of osSemaphore");
static_assert(sizeof(semaphore::Attributes) == sizeof(osSemaphoreAttr), "adjust size of osSemaphoreAttr");

static_assert(sizeof(Memory_pool) == sizeof(osPool), "adjust size of osPool");
static_assert(sizeof(mempool::Attributes) == sizeof(osPoolAttr), "adjust size of osPoolAttr");

static_assert(sizeof(Message_queue) == sizeof(osMessageQ), "adjust size of osMessageQ");
static_assert(sizeof(mqueue::Attributes) == sizeof(osMessageQAttr), "adjust size of osMessageQAttr");

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
  return static_cast<osStatus> (scheduler::start ());
}

int32_t
osKernelRunning (void)
{
  return scheduler::is_running ();
}

#if (defined (osFeature_SysTick)  &&  (osFeature_SysTick != 0))     // System Timer available

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
osThreadCreateEx (osThread* addr, const osThreadAttr* attr, os_pthread function,
                  const void* args)
{
  Thread* thread = new (addr) Thread ((thread::Attributes&) *attr,
                                      (thread::func_t) function, (void*) args);
  return reinterpret_cast<osThreadId> (thread);
}

osThreadId
osThreadGetId (void)
{
  return reinterpret_cast<osThreadId> (&this_thread::get ());
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
  return static_cast<osStatus> (this_thread::yield ());
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

osTimerId
osTimerCreateEx (osTimer* addr, osTimerAttr* attr, os_ptimer function,
                 void* args)
{
  return reinterpret_cast<osTimerId> (new ((void*) addr) Timer (
      (timer::Attributes&) (*attr), (timer::func_t) function,
      (timer::func_args_t) args));
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

osMutexId
osMutexCreateEx (osMutex* addr, const osMutexAttr* attr)
{
  return reinterpret_cast<osMutexId> (new ((void*) addr) Mutex (
      (const mutex::Attributes&) *attr));
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

osSemaphoreId
osSemaphoreCreateEx (osSemaphoreId addr, const osSemaphoreAttr* attr)
{
  return reinterpret_cast<osSemaphoreId> (new ((void*) addr) Semaphore (
      (const semaphore::Attributes&) *attr));
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

  semaphore::count_t value;
  (reinterpret_cast<Semaphore&> (semaphore_id)).get_value (&value);
  return (int32_t) value;
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

osPoolId
osPoolCreateEx (osPool* addr, const osPoolAttr* attr, size_t items,
                size_t item_size_bytes)
{
  return reinterpret_cast<osPoolId> (new ((void*) addr) Memory_pool (
      (mempool::Attributes&) *attr, (mempool::size_t) items,
      (mempool::size_t) item_size_bytes));
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

void
osPoolDeleteEx (osPoolId pool_id)
{
  (reinterpret_cast<Memory_pool&> (pool_id)).~Memory_pool ();
}

#endif /* Memory Pool Management available */

// ----------------------------------------------------------------------------

//  ==== Message Queue Management Functions ====

#if (defined (osFeature_MessageQ)  &&  (osFeature_MessageQ != 0))

osMessageQId
osMessageCreate (const osMessageQDef_t* queue_def,
                 osThreadId thread_id __attribute__((unused)))
{
#if 0
  return reinterpret_cast<osMessageQId> (new ((void*) &queue_def->data) Fixed_message_queue (
          queue_def->name, queue_def->queue_sz, queue_def->mempool,
          reinterpret_cast<Thread*> (thread_id)));
#else
  mqueue::Attributes attr
    { queue_def->name };
  attr.queue_address = queue_def->queue;
  attr.queue_size_bytes = queue_def->queue_sz;

  return reinterpret_cast<osMessageQId> (new ((void*) &queue_def->data) Message_queue (
      attr, (mqueue::size_t) queue_def->items,
      (mqueue::size_t) queue_def->item_sz));
#endif
}

osMessageQId
osMessageCreateEx (osMessageQ* addr, const osMessageQAttr* attr, size_t items,
                   size_t item_size)
{
  return reinterpret_cast<osMessageQId> (new ((void*) addr) Message_queue (
      (mqueue::Attributes&) (*attr), (mqueue::size_t) items,
      (mqueue::size_t) item_size));
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

void
osMailDeleteEx (osMailQId queue_id)
{
  osPoolDeleteEx (&(queue_id->pool));
  osMessageDeleteEx (&(queue_id->queue));
}

#endif /* Mail Queues available */

// ----------------------------------------------------------------------------
