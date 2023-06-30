/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/rtos/os.h>

// ----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

// ----------------------------------------------------------------------------

using namespace os;
using namespace os::rtos;

// ----------------------------------------------------------------------------

void*
os_idle (thread::func_args_t args);

// os-thread.h
// void
// os_rtos_idle_actions (void);

/**
 * @details
 * The hook must check an application specific condition to determine
 * if it is required to enter a power saving mode, and, if necessary,
 * actually enter the desired power saving mode.
 *
 * The application must ensure that all interrupts associated with
 * the external events used to awake the device are enabled. Usually
 * the RTC is used for this purpose, but other devices too, like USB,
 * GPIO pins, etc may be used to end the power saving mode.
 *
 * This function is executed by the idle thread on each iteration,
 * and must limit complexity to reasonably levels.
 *
 * If the user function decides not to enter a power saving mode,
 * it must return `false`, which will make the idle thread proceed as
 * usual, by entering a shallow sleep waiting for the next interrupt.
 */
bool
__attribute__((weak))
os_rtos_idle_enter_power_saving_mode_hook (void)
{
  return false;
}

void
__attribute__((weak))
os_rtos_idle_actions (void)
{
  while (!scheduler::terminated_threads_list_.empty ())
    {
      internal::waiting_thread_node* node;
        {
          // ----- Enter critical section ---------------------------------
          interrupts::critical_section ics;
          node =
              const_cast<internal::waiting_thread_node*> (scheduler::terminated_threads_list_.head ());
          node->unlink ();
          // ----- Exit critical section ----------------------------------
        }
      node->thread_->internal_destroy_ ();

      this_thread::yield ();
    }

#if defined(OS_HAS_INTERRUPTS_STACK)
  // Simple test to verify that the interrupts
  // did not underflow the stack.
  assert(rtos::interrupts::stack ()->check_bottom_magic ());
#endif

  if (!os_rtos_idle_enter_power_saving_mode_hook ())
    {
      port::scheduler::wait_for_interrupt ();
    }
}

#if !defined(OS_USE_RTOS_PORT_SCHEDULER) || defined(__DOXYGEN__)

/**
 * @cond ignore
 */
extern thread* os_idle_thread;

thread* os_idle_thread;

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#endif

#if defined(OS_EXCLUDE_DYNAMIC_MEMORY_ALLOCATIONS)

static thread_inclusive<OS_INTEGER_RTOS_IDLE_STACK_SIZE_BYTES> os_idle_thread_
  { "idle", os_idle, nullptr};

#else

static std::unique_ptr<thread> os_idle_thread_;

#endif /* defined(OS_EXCLUDE_DYNAMIC_MEMORY_ALLOCATIONS) */

#pragma GCC diagnostic pop

void
__attribute__((weak))
os_startup_create_thread_idle (void)
{
#if defined(OS_EXCLUDE_DYNAMIC_MEMORY_ALLOCATIONS)

  // The thread object instance was created by the static constructors.
  os_idle_thread = &os_idle_thread_;

#else

  thread::attributes attr = thread::initializer;
  attr.th_stack_size_bytes = OS_INTEGER_RTOS_IDLE_STACK_SIZE_BYTES;

  // No need for an explicit delete, it is deallocated by the unique_ptr.
  os_idle_thread_ = std::unique_ptr<thread> (
      new thread ("idle", os_idle, nullptr, attr));

  os_idle_thread = os_idle_thread_.get ();

#endif /* defined(OS_EXCLUDE_DYNAMIC_MEMORY_ALLOCATIONS) */
}

void*
os_idle (thread::func_args_t args __attribute__((unused)))
{

  // The thread was created with the default priority, and the
  // idle thread must run with th lowest possible priority.

#if defined(OS_BOOL_RTOS_THREAD_IDLE_PRIORITY_BELOW_IDLE)
  // The CMSIS RTOS validator creates threads with `priority::idle`,
  // so, to be sure that the system idle thread has the lowest priority,
  // go one step below the idle priority.
  this_thread::thread ().priority (thread::priority::idle - 1);
#else
  this_thread::thread ().priority (thread::priority::idle);
#endif

  while (true)
    {
      os_rtos_idle_actions ();

      // Possibly switch to threads that were resumed during sleep.
      this_thread::yield ();
    }
}

/**
 * @endcond
 */
#endif /* !defined(OS_USE_RTOS_PORT_SCHEDULER) */

// ----------------------------------------------------------------------------
