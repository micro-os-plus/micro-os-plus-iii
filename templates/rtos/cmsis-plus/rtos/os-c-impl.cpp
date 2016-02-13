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

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/diag/trace.h>
#include <cmsis-plus/rtos/os-c-api.h>

// ----------------------------------------------------------------------------

#include <cassert>
#include <cerrno>
#include <new>

// ----------------------------------------------------------------------------

using namespace os::rtos;

// ----------------------------------------------------------------------------

// Validate C structs sizes (should match the C++ objects sizes).
// TODO: validate individual members (size & offset).

static_assert(sizeof(Thread) == sizeof(os_thread_t), "adjust os_thread_t size");
static_assert(sizeof(thread::Attributes) == sizeof(os_thread_attr_t), "adjust os_thread_attr_t size");

#if 0
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
#endif

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

int
__attribute__((weak))
main (int argc, char* argv[])
{
  // Create a thread and call os_main()
}

#pragma GCC diagnostic pop

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
os_sched_is_running (void)
{
  return scheduler::is_running ();
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
os_thread_exit (os_thread_t* thread, void* exit_ptr)
{
  (reinterpret_cast<Thread&> (thread)).exit(exit_ptr);
}

os_result_t
os_thread_join (os_thread_t* thread, void** exit_ptr)
{
  return (reinterpret_cast<Thread&> (thread)).join(exit_ptr);
}

os_thread_prio_t
os_thread_get_prio (os_thread_t* thread)
{
  return (reinterpret_cast<Thread&> (thread)).sched_prio();
}

os_result_t
os_thread_set_prio (os_thread_t* thread, os_thread_prio_t prio)
{
  return (reinterpret_cast<Thread&> (thread)).sched_prio(prio);
}

void
os_thread_wakeup (os_thread_t* thread)
{
  return (reinterpret_cast<Thread&> (thread)).wakeup();
}

// ----------------------------------------------------------------------------
