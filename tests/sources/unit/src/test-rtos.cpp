/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2026 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can be
 * obtained from https://opensource.org/licenses/mit.
 */

/*
 * Functional tests of the RTOS: the behaviour of threads, the
 * synchronisation objects, timers and clocks in specific scenarios
 * (races, wake-ups, priority inheritance, interrupts), beyond the
 * presence of the API calls checked by the rtos-apis test.
 */

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/rtos/os.h>

#include <algorithm>
#include <cstdint>
#include <cstdio>

#include <test-unit.h>

// ----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#pragma clang diagnostic ignored "-Wformat-pedantic"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Waggregate-return"
#pragma GCC diagnostic ignored "-Wuseless-cast"
#endif

// ----------------------------------------------------------------------------

using namespace os;
using namespace os::rtos;

// ----------------------------------------------------------------------------

static const char* test_name = "Unit RTOS";

// ----------------------------------------------------------------------------

#if defined(__ARM_ARCH_PROFILE) && (__ARM_ARCH_PROFILE == 'M')

// Cortex-M only: system calls are never valid from NMI (fixed priority,
// not masked by the RTOS critical sections); record what the port says.
namespace
{
  bool volatile nmi_taken;
  bool volatile nmi_priority_valid;
} // namespace

// The top of the interrupt stack, from the linker script.
extern "C" unsigned int __stack;

extern "C" void
NMI_Handler (void);

// Overrides the weak default.
extern "C" void
NMI_Handler (void)
{
  nmi_priority_valid = port::interrupts::is_priority_valid ();
  nmi_taken = true;
}

#endif

// ----------------------------------------------------------------------------

namespace
{
  // A thread created by a static constructor, before the scheduler is
  // started (with interrupts already enabled by the startup code); it
  // must only become ready, and run after the scheduler is started.
  // Its stack is inside the object, so it does not use the heap for the
  // whole program.
  bool volatile static_thread_ran;
  bool volatile static_thread_ran_started;

  void*
  static_thread_func (void* args __attribute__ ((unused)))
  {
    static_thread_ran_started = scheduler::started ();
    static_thread_ran = true;
    return nullptr;
  }

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif
  thread_inclusive<OS_INTEGER_RTOS_DEFAULT_STACK_SIZE_BYTES> static_thread{
    "th-static", static_thread_func, nullptr
  };

  // A static constructor which, like many others could, wakes the thread
  // (resume() requests a context switch outside a scheduler critical
  // section), still before the scheduler is started.
  struct static_thread_waker
  {
    static_thread_waker ()
    {
      static_thread.flags_raise (0x1);
    }
  };
  static_thread_waker static_waker;
#pragma GCC diagnostic pop
} // namespace

// ----------------------------------------------------------------------------

namespace
{
  // Lost wake-up stress test: a thread event flag raised from a timer
  // callback (an interrupt context) must wake a thread which is
  // entering flags_wait(), even if the interrupt arrives between the
  // flags test and the suspend. The window is a few instructions wide,
  // so the test cannot hit it deterministically; it varies the moment
  // the waiter enters flags_wait() around the timer tick, to make the
  // race likely to show if the implementation has it.

  constexpr flags::mask_t lost_wakeup_flag = 0x1;
  constexpr uint32_t lost_wakeup_iterations = 2000;
  // The waiter enters flags_wait() up to this many calibration loops
  // before the tick.
  constexpr uint32_t lost_wakeup_offsets = 16;

  uint32_t volatile lost_wakeup_progress;
  bool volatile lost_wakeup_done;

  void
  lost_wakeup_raise (void* args)
  {
    static_cast<thread*> (args)->flags_raise (lost_wakeup_flag);
  }

  // Busy wait for the next tick; return the number of loops.
  uint32_t
  lost_wakeup_wait_tick (void)
  {
    clock::timestamp_t begin = sysclock.now ();
    uint32_t loops = 0;
    while (sysclock.now () == begin)
      {
        ++loops;
      }
    return loops;
  }

  // Busy wait for the given number of loops, each the same as one
  // in lost_wakeup_wait_tick().
  void
  lost_wakeup_spin (uint32_t loops)
  {
    clock::timestamp_t begin = sysclock.now ();
    for (uint32_t i = 0; i < loops; ++i)
      {
        if (sysclock.now () != begin)
          {
            break;
          }
      }
  }

  void*
  lost_wakeup_func (void* args __attribute__ ((unused)))
  {
    timer tm{ "tm-lw", lost_wakeup_raise, &this_thread::thread () };

    // Calibrate: the number of loops in one tick.
    lost_wakeup_wait_tick ();
    uint32_t loops_per_tick = lost_wakeup_wait_tick ();
    for (int i = 0; i < 3; ++i)
      {
        loops_per_tick = std::min (loops_per_tick, lost_wakeup_wait_tick ());
      }
    uint32_t offsets = std::min (lost_wakeup_offsets, loops_per_tick);

    for (uint32_t i = 0; i < lost_wakeup_iterations; ++i)
      {
        this_thread::flags_clear (lost_wakeup_flag);

        // The waiter is normally just past a tick, either at the end of
        // the calibration or woken by the previous timer callback.
        // Raise the flag on the next tick, from the timer callback.
        tm.start (1);
        lost_wakeup_spin (loops_per_tick - (i % offsets));

        this_thread::flags_wait (lost_wakeup_flag);
        lost_wakeup_progress = lost_wakeup_progress + 1;
      }

    tm.stop ();
    lost_wakeup_done = true;

    return nullptr;
  }

  int
  test_lost_wakeup (const char* name)
  {
    printf ("\n%s - Thread event flags raised from interrupts\n", name);

    lost_wakeup_progress = 0;
    lost_wakeup_done = false;

    // Above the current thread, so that it is not delayed by it.
    thread::attributes attr;
    attr.th_priority = thread::priority::above_normal;
    thread th{ "th-lw", lost_wakeup_func, nullptr, attr };

    uint32_t lost = 0;
    while (!lost_wakeup_done)
      {
        uint32_t progress = lost_wakeup_progress;
        sysclock.sleep_for (20);
        if (!lost_wakeup_done && lost_wakeup_progress == progress
            && th.state () == thread::state::suspended)
          {
            // The flag was raised long ago, but the waiter sleeps.
            ++lost;
            th.flags_raise (lost_wakeup_flag);
          }
      }

    th.join ();

    if (lost != 0)
      {
        printf ("%s - %u lost wake-ups in %u iterations\n", name,
                static_cast<unsigned int> (lost),
                static_cast<unsigned int> (lost_wakeup_iterations));
        return 1;
      }

    printf ("%s - no lost wake-ups in %u iterations\n", name,
            static_cast<unsigned int> (lost_wakeup_iterations));
    return 0;
  }

  // --------------------------------------------------------------------------

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wpadded"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpadded"
#endif

  // Shared with the condition variable timed_wait() waiter.
  struct cv_timed_wait_args
  {
    mutex* mx;
    condition_variable* cv;
    clock::duration_t timeout;
    result_t res;
  };

#pragma GCC diagnostic pop

  // --------------------------------------------------------------------------

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wpadded"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpadded"
#endif

  // Shared with the mutex waiters.
  struct mx_wait_args
  {
    mutex* mx;
    clock::duration_t timeout;
    result_t res;
    bool volatile done;
  };

#pragma GCC diagnostic pop

  void*
  mx_timed_lock_func (void* args)
  {
    mx_wait_args* p = static_cast<mx_wait_args*> (args);

    p->res = p->mx->timed_lock (p->timeout);
    if (p->res == result::ok)
      {
        p->mx->unlock ();
      }
    p->done = true;

    return nullptr;
  }

  void*
  mx_lock_func (void* args)
  {
    mx_wait_args* p = static_cast<mx_wait_args*> (args);

    p->res = p->mx->lock ();
    if (p->res == result::ok)
      {
        p->mx->unlock ();
      }
    p->done = true;

    return nullptr;
  }

  // --------------------------------------------------------------------------

  // Waiters for the single wake-up tests: the releasing call wakes
  // only one waiter; if it leaves (timeout, interrupt) before it runs,
  // the wake-up must be passed on to the next one.

  using lw_pool = memory_pool_inclusive<std::uintptr_t, 1>;
  using lw_queue = message_queue_inclusive<std::uintptr_t, 1>;

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wpadded"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpadded"
#endif

  struct lw_args
  {
    semaphore* sem;
    lw_pool* pool;
    lw_queue* queue;
    clock::duration_t timeout; // 0 for the untimed calls
    result_t res;
    std::uintptr_t* blk;
    std::uintptr_t msg;
    bool volatile done;
  };

#pragma GCC diagnostic pop

  void*
  lw_sem_func (void* args)
  {
    lw_args* p = static_cast<lw_args*> (args);
    p->res = (p->timeout != 0) ? p->sem->timed_wait (p->timeout)
                               : p->sem->wait ();
    p->done = true;
    return nullptr;
  }

  void*
  lw_pool_func (void* args)
  {
    lw_args* p = static_cast<lw_args*> (args);
    p->blk = (p->timeout != 0) ? p->pool->timed_alloc (p->timeout)
                               : p->pool->alloc ();
    p->res = (p->blk != nullptr) ? static_cast<result_t> (result::ok)
                                 : static_cast<result_t> (ETIMEDOUT);
    p->done = true;
    return nullptr;
  }

  void*
  lw_queue_func (void* args)
  {
    lw_args* p = static_cast<lw_args*> (args);
    p->res = (p->timeout != 0) ? p->queue->timed_receive (&p->msg, p->timeout)
                               : p->queue->receive (&p->msg);
    p->done = true;
    return nullptr;
  }

  // Busy wait with the scheduler locked, so that a woken thread cannot
  // run until its deadline has passed.
  void
  lw_busy_wait (clock::duration_t ticks)
  {
    clock::timestamp_t begin = sysclock.now ();
    while (sysclock.now () - begin < ticks)
      {
        __asm__ volatile ("" ::: "memory");
      }
  }

  // The block allocated by the main thread in the pool test.
  std::uintptr_t* lw_main_blk;

  void
  lw_sem_release (lw_args& p)
  {
    p.sem->post ();
  }

  void
  lw_pool_release (lw_args& p)
  {
    p.pool->free (lw_main_blk);
  }

  void
  lw_queue_release (lw_args& p)
  {
    std::uintptr_t m = 42;
    // Non-blocking, it is called with the scheduler locked.
    p.queue->try_send (&m);
  }

  // Waiter A (high) and waiter C (above_normal) block with `func`. With
  // the scheduler locked, `release` wakes A, the head of the waiting
  // list, and A is made to leave, by an interrupt or by its timeout
  // expiring, before it can run. C must then get the resource.
  int
  lw_run (const char* name, const char* what, void* (*func) (void*),
          void (*release) (lw_args&), lw_args& a, lw_args& c, bool interrupt)
  {
    thread::attributes attr;
    attr.th_priority = thread::priority::high;
    thread tha{ "th-lwa", func, &a, attr };
    attr.th_priority = thread::priority::above_normal;
    thread thc{ "th-lwc", func, &c, attr };
    // Let both run and block.
    sysclock.sleep_for (1);

    {
      // ----- Enter critical section -----------------------------------------
      scheduler::critical_section scs;

      release (a);
      if (interrupt)
        {
          tha.interrupt ();
        }
      else
        {
          lw_busy_wait (2 * a.timeout);
        }
      // ----- Exit critical section ------------------------------------------
    }

    // Let both waiters run.
    sysclock.sleep_for (2);

    bool lost = !c.done;
    if (lost)
      {
        // Release the remaining waiter.
        thc.interrupt ();
      }
    tha.join ();
    thc.join ();

    result_t expected = interrupt ? EINTR : ETIMEDOUT;
    if (a.res != expected || lost || c.res != result::ok)
      {
        printf ("%s - %s: first waiter returned %d, next waiter %s\n", name,
                what, static_cast<int> (a.res), lost ? "not woken" : "woken");
        return 1;
      }

    printf ("%s - %s passes the wake-up on ok\n", name, what);
    return 0;
  }

  // --------------------------------------------------------------------------

  // Pending interrupt tests: a wait entered with the interrupt flag
  // already set must return EINTR without blocking.

  constexpr clock::duration_t pi_timeout = 10;

  // Check the result, then clear the flag.
  bool
  pi_check (const char* name, const char* what, result_t res,
            clock::timestamp_t begin)
  {
    clock::duration_t elapsed
        = static_cast<clock::duration_t> (sysclock.now () - begin);
    this_thread::thread ().interrupt (false);
    // Without blocking, well before the timeout.
    if (res != EINTR || elapsed >= pi_timeout / 2)
      {
        printf ("%s - interrupted %s returned %d after %u ticks\n", name, what,
                static_cast<int> (res), static_cast<unsigned int> (elapsed));
        return false;
      }
    return true;
  }

  void*
  pi_mutex_owner_func (void* args)
  {
    mutex* mx = static_cast<mutex*> (args);
    mx->lock ();
    sysclock.sleep_for (2 * pi_timeout);
    mx->unlock ();
    return nullptr;
  }

  void*
  pi_post_func (void* args)
  {
    sysclock.sleep_for (pi_timeout / 2);
    static_cast<semaphore*> (args)->post ();
    return nullptr;
  }

  int
  test_pending_interrupt (const char* name)
  {
    printf ("\n%s - Pending interrupts\n", name);

    thread& crt = this_thread::thread ();
    clock::timestamp_t begin;
    result_t res;

    // Thread event flags.
    crt.interrupt ();
    begin = sysclock.now ();
    res = this_thread::flags_timed_wait (0x1, pi_timeout);
    if (!pi_check (name, "flags_timed_wait()", res, begin))
      {
        return 1;
      }

    // Clock sleep.
    crt.interrupt ();
    begin = sysclock.now ();
    res = sysclock.sleep_for (pi_timeout);
    if (!pi_check (name, "sleep_for()", res, begin))
      {
        return 1;
      }

    // Semaphore.
    semaphore sem{ "sem-pi" };
    crt.interrupt ();
    begin = sysclock.now ();
    res = sem.timed_wait (pi_timeout);
    if (!pi_check (name, "semaphore timed_wait()", res, begin))
      {
        return 1;
      }

    // A wait that completes without blocking still succeeds.
    sem.post ();
    crt.interrupt ();
    res = sem.timed_wait (pi_timeout);
    crt.interrupt (false);
    if (res != result::ok)
      {
        printf ("%s - interrupted semaphore timed_wait() with a token "
                "returned %d\n",
                name, static_cast<int> (res));
        return 1;
      }

    // After the flag is cleared, waits block again.
    res = sem.timed_wait (2);
    if (res != ETIMEDOUT)
      {
        printf ("%s - semaphore timed_wait() after interrupt(false) returned "
                "%d\n",
                name, static_cast<int> (res));
        return 1;
      }

    // Untimed: a helper posts later, so that a wait which blocks does
    // not hang the test.
    {
      thread th{ "th-pi-post", pi_post_func, &sem };
      crt.interrupt ();
      begin = sysclock.now ();
      res = sem.wait ();
      if (!pi_check (name, "semaphore wait()", res, begin))
        {
          return 1;
        }
      // The helper token is still delivered.
      res = sem.wait ();
      th.join ();
      if (res != result::ok)
        {
          printf ("%s - semaphore wait() after interrupt(false) returned "
                  "%d\n",
                  name, static_cast<int> (res));
          return 1;
        }
    }

    // Mutex owned by another thread.
    {
      mutex mx{ "mx-pi" };
      thread th{ "th-pi-mx", pi_mutex_owner_func, &mx };
      // Let the owner lock it.
      sysclock.sleep_for (1);

      crt.interrupt ();
      begin = sysclock.now ();
      res = mx.timed_lock (pi_timeout);
      if (!pi_check (name, "mutex timed_lock()", res, begin))
        {
          return 1;
        }
      th.join ();
    }

    // Memory pool, no free block.
    {
      lw_pool pool{ "mp-pi" };
      std::uintptr_t* blk = pool.alloc ();
      crt.interrupt ();
      begin = sysclock.now ();
      std::uintptr_t* p = pool.timed_alloc (pi_timeout);
      res = (p == nullptr) ? static_cast<result_t> (EINTR)
                           : static_cast<result_t> (result::ok);
      if (!pi_check (name, "memory pool timed_alloc()", res, begin))
        {
          return 1;
        }
      pool.free (blk);
    }

    // Message queue, empty.
    {
      lw_queue queue{ "mq-pi" };
      std::uintptr_t msg;
      crt.interrupt ();
      begin = sysclock.now ();
      res = queue.timed_receive (&msg, pi_timeout);
      if (!pi_check (name, "message queue timed_receive()", res, begin))
        {
          return 1;
        }
    }

    // Event flags, not raised.
    {
      event_flags ev{ "ev-pi" };
      crt.interrupt ();
      begin = sysclock.now ();
      res = ev.timed_wait (0x1, pi_timeout);
      if (!pi_check (name, "event flags timed_wait()", res, begin))
        {
          return 1;
        }
    }

    printf ("%s - pending interrupts ok\n", name);
    return 0;
  }

  // --------------------------------------------------------------------------

  // Timer callback counter for the timers tests.
  uint32_t volatile tm_count;

  void
  tm_count_func (void* args __attribute__ ((unused)))
  {
    tm_count = tm_count + 1;
  }

  // --------------------------------------------------------------------------

  // Threads for the kill() tests.
  void*
  kill_sleeper_func (void* args __attribute__ ((unused)))
  {
    sysclock.sleep_for (100);
    return nullptr;
  }

  void*
  kill_quick_func (void* args __attribute__ ((unused)))
  {
    return nullptr;
  }

  // --------------------------------------------------------------------------

  void*
  cv_timed_wait_func (void* args)
  {
    cv_timed_wait_args* p = static_cast<cv_timed_wait_args*> (args);

    p->mx->lock ();
    p->res = p->cv->timed_wait (*p->mx, p->timeout);
    p->mx->unlock ();

    return nullptr;
  }
} // namespace

// ----------------------------------------------------------------------------

int
test_unit_rtos (void)
{
  printf ("\n%s - started\n", test_name);

  // ==========================================================================

  printf ("\n%s - Threads\n", test_name);

  {
    // The thread created by a static constructor.
    static_thread.join ();
    if (!static_thread_ran || !static_thread_ran_started)
      {
        printf ("%s - thread created by a static constructor ran %s the "
                "scheduler was started\n",
                test_name,
                static_thread_ran ? "before" : "neither before nor after");
        return 1;
      }
    printf ("%s - thread created by a static constructor ok\n", test_name);
  }

  {
    // A thread with a small stack, above the port minimum (twice the
    // minimum: 512 bytes on Cortex-M, where debug builds used to reject
    // any stack up to 548 bytes).
    thread::attributes attr;
    attr.th_stack_size_bytes = thread::stack::min_size () * 2;
#if defined(TRACE) \
    && (defined(OS_TRACE_RTOS_THREAD) \
        || defined(OS_TRACE_RTOS_THREAD_CONTEXT) \
        || defined(OS_TRACE_RTOS_THREAD_FLAGS) \
        || defined(OS_TRACE_RTOS_SCHEDULER) || defined(OS_TRACE_RTOS_LISTS))
    // The minimum does not include the RTOS trace: with these options
    // the thread calls trace::printf() on its own stack while starting
    // and exiting (about 800 bytes in debug builds, for the buffer and
    // vsnprintf()); allow 1 KB more (the 548 bytes check is then not
    // exercised).
    attr.th_stack_size_bytes += 1024;
#endif
    thread th{ "th-small-stack", kill_quick_func, nullptr, attr };
    th.join ();
    printf ("%s - thread with a %u bytes stack ok\n", test_name,
            static_cast<unsigned int> (attr.th_stack_size_bytes));
  }

  {
    // A thread created inside an interrupts critical section must run
    // with the interrupts (the clock) enabled, once the critical section
    // is left. On POSIX the context used to inherit the blocked clock
    // signal from its creator (checks
    // micro-os-plus/micro-os-plus-iii-posix-arch#4); the thread
    // busy-waits for 5 ticks, at most 1 second.
    static clock::timestamp_t ticks_seen;
    ticks_seen = 0;
    thread* pth;
    {
      interrupts::critical_section ics;
      pth = new thread{ "th-created-in-ics", [] (void*) -> void*
        {
          clock::timestamp_t t0 = sysclock.now ();
          clock::timestamp_t h0 = hrclock.now ();
          clock::timestamp_t limit = hrclock.input_clock_frequency_hz ();
          while ((sysclock.now () - t0) < 5 && (hrclock.now () - h0) < limit)
            {
            }
          ticks_seen = sysclock.now () - t0;
          return nullptr;
        }, nullptr };
    }
    pth->join ();
    delete pth;
    if (ticks_seen < 5)
      {
        printf ("%s - thread created in a critical section saw %u ticks\n",
                test_name, static_cast<unsigned int> (ticks_seen));
        return 1;
      }
    printf ("%s - thread created in a critical section ok\n", test_name);
  }

  {
    // kill() a thread blocked in a wait.
    thread th{ "th-kill1", kill_sleeper_func, nullptr };
    // Let it start and block in sleep_for().
    sysclock.sleep_for (1);

    result_t res = th.kill ();
    if (res != result::ok || th.state () != thread::state::destroyed)
      {
        printf ("%s - kill() of a waiting thread returned %d, state %u\n",
                test_name, static_cast<int> (res),
                static_cast<unsigned int> (th.state ()));
        return 1;
      }
  }

  {
    // kill() a thread which terminated itself, before the idle thread
    // destroys it. Above the current thread, so it runs to completion
    // at once; idle cannot run meanwhile.
    thread::attributes attr;
    attr.th_priority = thread::priority::above_normal;
    thread th{ "th-kill2", kill_quick_func, nullptr, attr };

    if (th.state () != thread::state::terminated)
      {
        printf ("%s - self-terminated thread state %u\n", test_name,
                static_cast<unsigned int> (th.state ()));
        return 1;
      }

    result_t res = th.kill ();
    if (res != result::ok || th.state () != thread::state::destroyed)
      {
        printf ("%s - kill() of a terminated thread returned %d, state %u\n",
                test_name, static_cast<int> (res),
                static_cast<unsigned int> (th.state ()));
        return 1;
      }

    // Already destroyed.
    res = th.kill ();
    if (res != result::ok)
      {
        printf ("%s - kill() of a destroyed thread returned %d\n", test_name,
                static_cast<int> (res));
        return 1;
      }

    // Let idle run; the thread is no longer on the terminated list, so
    // it must not be destroyed again.
    sysclock.sleep_for (2);

    printf ("%s - kill() ok\n", test_name);
  }

  // ==========================================================================

  // Thread event flags (prints its own title).
  if (test_lost_wakeup (test_name) != 0)
    {
      return 1;
    }

  // ==========================================================================

  printf ("\n%s - Condition variables\n", test_name);

  {
    // A wait with a mutex not owned by the caller fails in unlock(),
    // after the thread was linked to the condition variable and marked
    // suspended. It must return the error with the thread running;
    // a thread left `suspended` would be lost at the next context
    // switch, so the state is checked before yielding.
    mutex::attributes attr;
    attr.mx_type = mutex::type::errorcheck;
    mutex mx{ "mx-ec", attr };
    condition_variable cv{ "cv-ec" };

    result_t res = cv.wait (mx);
    if (res != EPERM
        || this_thread::thread ().state () != thread::state::running)
      {
        printf ("%s - wait() with a mutex not owned returned %d, state %u\n",
                test_name, static_cast<int> (res),
                static_cast<unsigned int> (this_thread::thread ().state ()));
        return 1;
      }

    res = cv.timed_wait (mx, 10);
    if (res != EPERM
        || this_thread::thread ().state () != thread::state::running)
      {
        printf (
            "%s - timed_wait() with a mutex not owned returned %d, state %u\n",
            test_name, static_cast<int> (res),
            static_cast<unsigned int> (this_thread::thread ().state ()));
        return 1;
      }

    // Still scheduled.
    this_thread::yield ();
    sysclock.sleep_for (1);

    printf ("%s - wait() with a mutex not owned ok\n", test_name);
  }

  {
    mutex mx{ "mx-tw" };
    condition_variable cv{ "cv-tw" };

    // Not signalled: the timeout expires, and the mutex is re-acquired.
    mx.lock ();
    result_t res = cv.timed_wait (mx, 2);
    if (res != ETIMEDOUT || mx.owner () != &this_thread::thread ())
      {
        printf ("%s - timed_wait() not signalled returned %d\n", test_name,
                static_cast<int> (res));
        return 1;
      }
    mx.unlock ();

    // Signalled before the deadline, but the mutex is held past it: the
    // result must be ok, the signal was consumed.
    cv_timed_wait_args args{ &mx, &cv, 5, ENOTRECOVERABLE };

    // Above the current thread, so it starts at once, locks the mutex
    // and blocks in timed_wait(), which releases the mutex.
    thread::attributes attr;
    attr.th_priority = thread::priority::above_normal;
    thread th{ "th-tw", cv_timed_wait_func, &args, attr };

    mx.lock ();
    // The waiter wakes and blocks re-acquiring the mutex.
    cv.signal ();
    // Hold the mutex past the waiter deadline.
    sysclock.sleep_for (2 * args.timeout);
    mx.unlock ();

    th.join ();

    if (args.res != result::ok)
      {
        printf ("%s - timed_wait() signalled before the deadline returned "
                "%d\n",
                test_name, static_cast<int> (args.res));
        return 1;
      }

    printf ("%s - timed_wait() ok\n", test_name);
  }

  // ==========================================================================

  printf ("\n%s - Mutexes\n", test_name);

  {
    // Priority inheritance after a timed_lock() timeout. The waiters
    // are above the current thread, so they start at once and block.
    mutex::attributes mx_attr;
    mx_attr.mx_protocol = mutex::protocol::inherit;
    mutex mx{ "mx-pi", mx_attr };

    thread& crt = this_thread::thread ();
    thread::priority_t prio = crt.priority ();
    thread::attributes th_attr;

    // The only waiter times out: the boost must be undone.
    {
      mx.lock ();
      mx_wait_args a{ &mx, 5, ENOTRECOVERABLE, false };
      th_attr.th_priority = thread::priority::high;
      thread th{ "th-pi0", mx_timed_lock_func, &a, th_attr };
      if (crt.priority () != thread::priority::high)
        {
          printf ("%s - mutex owner not boosted (%u)\n", test_name,
                  static_cast<unsigned int> (crt.priority ()));
          return 1;
        }

      sysclock.sleep_for (2 * a.timeout);
      if (a.res != ETIMEDOUT || crt.priority () != prio)
        {
          printf ("%s - timed_lock() timeout returned %d, owner prio %u\n",
                  test_name, static_cast<int> (a.res),
                  static_cast<unsigned int> (crt.priority ()));
          return 1;
        }
      mx.unlock ();
      th.join ();
    }

    // One of two waiters times out: the owner inherits the remaining
    // waiter priority, and its assigned priority is not changed.
    {
      mx.lock ();
      mx_wait_args a1{ &mx, 5, ENOTRECOVERABLE, false };
      mx_wait_args a2{ &mx, 100, ENOTRECOVERABLE, false };
      th_attr.th_priority = thread::priority::high;
      thread th1{ "th-pi1", mx_timed_lock_func, &a1, th_attr };
      th_attr.th_priority = thread::priority::above_normal;
      thread th2{ "th-pi2", mx_timed_lock_func, &a2, th_attr };
      // The owner is boosted above th2; let th2 run and block.
      sysclock.sleep_for (1);

      sysclock.sleep_for (2 * a1.timeout);
      if (a1.res != ETIMEDOUT
          || crt.priority () != thread::priority::above_normal)
        {
          printf ("%s - timed_lock() timeout returned %d, owner prio %u\n",
                  test_name, static_cast<int> (a1.res),
                  static_cast<unsigned int> (crt.priority ()));
          return 1;
        }

      mx.unlock ();
      th1.join ();
      th2.join ();
      if (a2.res != result::ok || crt.priority () != prio)
        {
          printf ("%s - after unlock() waiter returned %d, owner prio %u\n",
                  test_name, static_cast<int> (a2.res),
                  static_cast<unsigned int> (crt.priority ()));
          return 1;
        }
    }

    printf ("%s - timed_lock() priority inheritance ok\n", test_name);
  }

  {
    // unlock() wakes a single waiter; if that waiter times out before
    // it runs, the wake-up must be passed to the next waiter.
    mutex mx{ "mx-lw" };
    thread::attributes th_attr;

    mx.lock ();
    mx_wait_args a{ &mx, 5, ENOTRECOVERABLE, false };
    mx_wait_args c{ &mx, 0, ENOTRECOVERABLE, false };
    th_attr.th_priority = thread::priority::high;
    thread tha{ "th-lwa", mx_timed_lock_func, &a, th_attr };
    th_attr.th_priority = thread::priority::above_normal;
    thread thc{ "th-lwc", mx_lock_func, &c, th_attr };
    // The owner is boosted above thc; let thc run and block.
    sysclock.sleep_for (1);

    {
      // ----- Enter critical section -----------------------------------------
      scheduler::critical_section scs;

      // Wakes the head of the waiting list, the higher priority one,
      // which cannot run yet.
      mx.unlock ();

      // Busy wait past its deadline.
      clock::timestamp_t begin = sysclock.now ();
      while (sysclock.now () - begin < 2 * a.timeout)
        {
          __asm__ volatile ("" ::: "memory");
        }
      // ----- Exit critical section ------------------------------------------
    }

    // Let both waiters run.
    sysclock.sleep_for (2);

    bool lost = !c.done;
    if (lost)
      {
        // Release the remaining waiter.
        mx.lock ();
        mx.unlock ();
      }
    tha.join ();
    thc.join ();

    if (a.res != ETIMEDOUT || lost || c.res != result::ok)
      {
        printf ("%s - timed out waiter returned %d, next waiter %s\n",
                test_name, static_cast<int> (a.res),
                lost ? "not woken" : "woken");
        return 1;
      }

    printf ("%s - timed_lock() timeout passes the wake-up on ok\n", test_name);
  }

  {
    // lock() interrupted: the boost given to the owner must be undone.
    mutex mx{ "mx-intr" };
    thread& crt = this_thread::thread ();
    thread::priority_t prio = crt.priority ();
    thread::attributes th_attr;

    mx.lock ();
    mx_wait_args a{ &mx, 0, ENOTRECOVERABLE, false };
    th_attr.th_priority = thread::priority::high;
    thread tha{ "th-intr", mx_lock_func, &a, th_attr };
    // Boosting the owner to the waiter priority yields to it before the
    // waiter blocks; let the waiter finish blocking, since an interrupt
    // before it waits would be lost (a separate, known limitation).
    sysclock.sleep_for (1);
    if (crt.priority () != thread::priority::high)
      {
        printf ("%s - mutex owner not boosted (%u)\n", test_name,
                static_cast<unsigned int> (crt.priority ()));
        return 1;
      }

    tha.interrupt ();
    // Let the waiter run and leave.
    sysclock.sleep_for (1);

    if (!a.done || a.res != EINTR || crt.priority () != prio
        || mx.owner () != &crt)
      {
        printf ("%s - interrupted lock() returned %d, owner prio %u\n",
                test_name, static_cast<int> (a.res),
                static_cast<unsigned int> (crt.priority ()));
        return 1;
      }
    mx.unlock ();
    tha.join ();

    printf ("%s - interrupted lock() priority inheritance ok\n", test_name);
  }

  {
    // lock() interrupted after unlock() woke it: the wake-up must be
    // passed to the next waiter.
    mutex mx{ "mx-intr2" };
    thread::attributes th_attr;

    mx.lock ();
    mx_wait_args a{ &mx, 0, ENOTRECOVERABLE, false };
    mx_wait_args c{ &mx, 0, ENOTRECOVERABLE, false };
    th_attr.th_priority = thread::priority::high;
    thread tha{ "th-intra", mx_lock_func, &a, th_attr };
    th_attr.th_priority = thread::priority::above_normal;
    thread thc{ "th-intrc", mx_lock_func, &c, th_attr };
    // The owner is boosted above thc; let thc run and block.
    sysclock.sleep_for (1);

    {
      // ----- Enter critical section -----------------------------------------
      scheduler::critical_section scs;

      // Wake the head of the waiting list, then interrupt it before it
      // can run.
      mx.unlock ();
      tha.interrupt ();
      // ----- Exit critical section ------------------------------------------
    }

    // Let both waiters run.
    sysclock.sleep_for (2);

    bool lost = !c.done;
    if (lost)
      {
        // Release the remaining waiter.
        mx.lock ();
        mx.unlock ();
      }
    tha.join ();
    thc.join ();

    if (a.res != EINTR || lost || c.res != result::ok)
      {
        printf ("%s - interrupted waiter returned %d, next waiter %s\n",
                test_name, static_cast<int> (a.res),
                lost ? "not woken" : "woken");
        return 1;
      }

    printf ("%s - interrupted lock() passes the wake-up on ok\n", test_name);
  }

  // ==========================================================================

  printf ("\n%s - Semaphores, memory pools and message queues\n", test_name);

  // A waiter woken by a single wake-up and then leaving (timeout,
  // interrupt) must pass the wake-up on to the next waiter.
  {
    semaphore sem{ "sem-lw1" };
    lw_args a{ &sem, nullptr, nullptr, 5, ENOTRECOVERABLE, nullptr, 0, false };
    lw_args c{ &sem, nullptr, nullptr, 0, ENOTRECOVERABLE, nullptr, 0, false };
    if (lw_run (test_name, "semaphore timed_wait() timeout", lw_sem_func,
                lw_sem_release, a, c, false)
        != 0)
      {
        return 1;
      }
  }

  {
    semaphore sem{ "sem-lw2" };
    lw_args a{ &sem, nullptr, nullptr, 0, ENOTRECOVERABLE, nullptr, 0, false };
    lw_args c{ &sem, nullptr, nullptr, 0, ENOTRECOVERABLE, nullptr, 0, false };
    if (lw_run (test_name, "semaphore wait() interrupted", lw_sem_func,
                lw_sem_release, a, c, true)
        != 0)
      {
        return 1;
      }
  }

  {
    lw_pool pool{ "mp-lw" };
    lw_main_blk = pool.alloc ();
    lw_args a{
      nullptr, &pool, nullptr, 5, ENOTRECOVERABLE, nullptr, 0, false
    };
    lw_args c{
      nullptr, &pool, nullptr, 0, ENOTRECOVERABLE, nullptr, 0, false
    };
    if (lw_run (test_name, "memory pool timed_alloc() timeout", lw_pool_func,
                lw_pool_release, a, c, false)
        != 0)
      {
        return 1;
      }
    pool.free (c.blk);
  }

  {
    lw_queue queue{ "mq-lw" };
    lw_args a{
      nullptr, nullptr, &queue, 5, ENOTRECOVERABLE, nullptr, 0, false
    };
    lw_args c{
      nullptr, nullptr, &queue, 0, ENOTRECOVERABLE, nullptr, 0, false
    };
    if (lw_run (test_name, "message queue timed_receive() timeout",
                lw_queue_func, lw_queue_release, a, c, false)
        != 0)
      {
        return 1;
      }
  }

  // ==========================================================================

  printf ("\n%s - Cooperative scheduler and timers\n", test_name);

  {
    // Cooperative mode: while this thread sleeps, idle runs; the tick
    // interrupt that ends the sleep cannot switch context, so only
    // idle's yield() can bring this thread back. If idle does not
    // yield when exactly one thread is ready, this hangs.
    bool was_preemptive = scheduler::preemptive (false);

    clock::timestamp_t begin = sysclock.now ();
    sysclock.sleep_for (2);
    clock::timestamp_t end = sysclock.now ();

    scheduler::preemptive (was_preemptive);

    if (end - begin < 2)
      {
        printf ("%s - cooperative sleep_for(2) returned after %u ticks\n",
                test_name, static_cast<unsigned int> (end - begin));
        return 1;
      }
    printf ("%s - cooperative sleep_for() ok\n", test_name);
  }

  {
    // Periodic timer: called on each period, and no more after stop().
    timer tm{ "tm-per", tm_count_func, nullptr, timer::periodic_initializer };
    tm_count = 0;
    sysclock.sleep_for (1); // Sync
    tm.start (2);
    sysclock.sleep_for (11);

    uint32_t count = tm_count;
    result_t res = tm.stop ();
    if (count < 4 || res != result::ok)
      {
        printf ("%s - periodic timer called %u times, stop() returned %d\n",
                test_name, static_cast<unsigned int> (count),
                static_cast<int> (res));
        return 1;
      }

    sysclock.sleep_for (10);
    res = tm.stop ();
    if (tm_count != count || res != EAGAIN)
      {
        printf ("%s - stopped periodic timer called %u more times, stop() "
                "returned %d\n",
                test_name, static_cast<unsigned int> (tm_count - count),
                static_cast<int> (res));
        return 1;
      }
  }

  {
    // One-shot timer: called once, then completed.
    timer tm{ "tm-once", tm_count_func, nullptr };
    tm_count = 0;
    sysclock.sleep_for (1); // Sync
    tm.start (2);
    sysclock.sleep_for (10);

    result_t res = tm.stop ();
    if (tm_count != 1 || res != EAGAIN)
      {
        printf ("%s - one-shot timer called %u times, stop() returned %d\n",
                test_name, static_cast<unsigned int> (tm_count),
                static_cast<int> (res));
        return 1;
      }

    printf ("%s - periodic and one-shot timers ok\n", test_name);
  }

  // ==========================================================================

  printf ("\n%s - Clocks\n", test_name);

  {
    // The high resolution clock must be monotonic, also when read in a
    // critical section while the tick interrupt is held pending (the
    // counter wrapped, but the tick was not yet accounted). Only one
    // wrap can be detected by the hardware, so each round keeps the
    // critical section for less than one tick: a tick already pending
    // when the round starts (delayed by the interrupt latency) has just
    // wrapped, so no second wrap can follow within the round.
    const clock::timestamp_t cycles_per_tick
        = hrclock.input_clock_frequency_hz ()
          / OS_INTEGER_SYSTICK_FREQUENCY_HZ;
    uint32_t backwards = 0;
    uint32_t crossed = 0;
    for (int round = 0; round < 20; ++round)
      {
        clock::timestamp_t ticks_before = sysclock.now ();
        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          clock::timestamp_t begin = hrclock.now ();
          clock::timestamp_t prev = begin;
          // The iterations limit ends the round if the clock is broken.
          for (uint32_t i = 0; i < 2000000; ++i)
            {
              clock::timestamp_t now = hrclock.now ();
              if (now < prev)
                {
                  ++backwards;
                  break;
                }
              prev = now;
              if (now - begin >= (cycles_per_tick * 3) / 4)
                {
                  break;
                }
            }
          // ----- Exit critical section --------------------------------------
        }
        // The pending tick is processed here; a round exercises the
        // case only if a tick occurred while it was masked.
        if (sysclock.now () != ticks_before)
          {
            ++crossed;
          }
      }

    if (backwards != 0)
      {
        printf ("%s - hrclock went backwards %u times\n", test_name,
                static_cast<unsigned int> (backwards));
        return 1;
      }
    if (crossed == 0)
      {
        printf ("%s - hrclock monotonic, but no tick was held pending\n",
                test_name);
      }
    else
      {
        printf ("%s - hrclock monotonic ok (%u rounds over a tick)\n",
                test_name, static_cast<unsigned int> (crossed));
      }
  }

  // ==========================================================================

  printf ("\n%s - Scheduler lock\n", test_name);

  {
    // A higher priority thread woken while the scheduler is locked must
    // run as soon as the scheduler is unlocked, not at the next tick.
    // mutex::unlock() wakes the waiter in a scheduler critical section.
    mutex::attributes mx_attr;
    mx_attr.mx_protocol = mutex::protocol::none;
    mutex mx{ "mx-sl", mx_attr };
    mx.lock ();
    mx_wait_args a{ &mx, 0, ENOTRECOVERABLE, false };
    thread::attributes th_attr;
    th_attr.th_priority = thread::priority::high;
    // Above the current thread, so it starts at once and blocks.
    thread th{ "th-sl", mx_lock_func, &a, th_attr };

    mx.unlock ();
    bool at_once = a.done;
    th.join ();
    if (!at_once)
      {
        printf ("%s - waiter woken by unlock() did not run at once\n",
                test_name);
        return 1;
      }
  }

  {
    // The same with an explicit scheduler critical section.
    semaphore sem{ "sem-sl" };
    lw_args a{ &sem, nullptr, nullptr, 0, ENOTRECOVERABLE, nullptr, 0, false };
    thread::attributes th_attr;
    th_attr.th_priority = thread::priority::high;
    thread th{ "th-sl2", lw_sem_func, &a, th_attr };

    {
      // ----- Enter critical section -----------------------------------------
      scheduler::critical_section scs;

      sem.post ();
      // ----- Exit critical section ------------------------------------------
    }
    bool at_once = a.done;
    th.join ();
    if (!at_once)
      {
        printf ("%s - thread woken in a scheduler critical section did not "
                "run at once\n",
                test_name);
        return 1;
      }

    printf ("%s - deferred context switch at unlock ok\n", test_name);
  }

  // ==========================================================================

#if defined(__ARM_ARCH_PROFILE) && (__ARM_ARCH_PROFILE == 'M')

  printf ("\n%s - Interrupt priorities\n", test_name);

  {
    // Pend NMI (ICSR.NMIPENDSET, bit 31); the handler records whether
    // the port considers system calls valid from it.
    volatile uint32_t* const icsr
        = reinterpret_cast<volatile uint32_t*> (0xE000ED04);
    *icsr = (1UL << 31);
    __asm__ volatile ("dsb\n isb" ::: "memory");

    if (!nmi_taken || nmi_priority_valid)
      {
        printf ("%s - NMI %s, is_priority_valid() %s\n", test_name,
                nmi_taken ? "taken" : "not taken",
                nmi_priority_valid ? "true" : "false");
        return 1;
      }
    printf ("%s - system calls not valid from NMI ok\n", test_name);
  }

#if !defined(OS_DISABLE_CORTEXM_SET_MSP_VIA_VTOR)
  {
    // In Thread mode no handler is active, so MSP is at the base of
    // the interrupt stack, set by the scheduler start.
    uint32_t msp = __get_MSP ();
    if (msp != reinterpret_cast<uint32_t> (&__stack))
      {
        printf (
            "%s - MSP %08X, __stack %08X\n", test_name,
            static_cast<unsigned int> (msp),
            static_cast<unsigned int> (reinterpret_cast<uint32_t> (&__stack)));
        return 1;
      }
    printf ("%s - interrupt stack at __stack ok\n", test_name);
  }
#endif

#endif

  // ==========================================================================

  if (test_pending_interrupt (test_name) != 0)
    {
      return 1;
    }

  printf ("\n%s - done\n", test_name);

  return 0;
}

// ----------------------------------------------------------------------------
