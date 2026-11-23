/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2015-2025 Liviu Ionescu. All rights reserved.
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

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/memory/block-pool.h>
#include <cmsis-plus/memory/lifo.h>
#include <cmsis-plus/estd/memory_resource>
#include <cmsis-plus/estd/mutex>

#include <algorithm>
#include <cstdint>

#include <test-cpp-api.h>

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

static const char* test_name = "Test C++ API";

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

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wpadded"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpadded"
#endif

typedef struct my_msg_s
{
  int i;
  const char* s;
} my_msg_t;

typedef struct my_blk_s
{
  int i;
  const char* s;
} my_blk_t;

#pragma GCC diagnostic pop

void*
func (void* args);

void*
func (void* args __attribute__ ((unused)))
{
  printf ("> %s() on thread %s\n", __func__, this_thread::thread ().name ());
  // fflush(stdout);

  return nullptr;
}

void
tmfunc (void* args);

void
tmfunc (void* args __attribute__ ((unused)))
{
  printf (">> %s()\n", __func__);
  // fflush(stdout);
}

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

#if !defined(OS_USE_RTOS_PORT_SCHEDULER)

void
iterate_threads (thread* th = nullptr, unsigned int depth = 0);

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
static const char* thread_state[]
    = { "undf", "rdy", "run", "wait", "term", "dead", "init" };
#pragma GCC diagnostic pop

void
iterate_threads (thread* th, unsigned int depth)
{
  for (auto&& p : scheduler::children_threads (th))
    {
      class thread::stack& stk = p.stack ();
      unsigned int used
          = static_cast<unsigned int> (stk.size () - stk.available ());
      unsigned int used_proc
          = static_cast<unsigned int> (used * 100 / stk.size ());
      unsigned int st = static_cast<unsigned int> (p.state ());
      assert (st < sizeof (thread_state) / sizeof (thread_state[0]));

      statistics::counter_t thread_switches
          = p.statistics ().context_switches ();

      statistics::duration_t thread_cpu_cycles = p.statistics ().cpu_cycles ();

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
      printf ("%s, %u%% (%u/%u), %s, %u, %u \n", p.name (), used_proc, used,
              static_cast<unsigned int> (stk.size ()), thread_state[st],
              static_cast<unsigned int> (thread_switches),
              static_cast<unsigned int> (thread_cpu_cycles));
#pragma GCC diagnostic pop

      iterate_threads (&p, depth + 1);
    }
}

#endif

void
pass_mutex_up1 (const std::unique_ptr<mutex>& pp);

void
pass_mutex_up1 (const std::unique_ptr<mutex>& pp)
{
  printf ("%p\n", pp.get ());
  // fflush(stdout);
}

void
pass_mutex_up2 (std::unique_ptr<mutex>& pp);

void
pass_mutex_up2 (std::unique_ptr<mutex>& pp)
{
  printf ("%p\n", pp.get ());
  // fflush(stdout);
}

void
pass_mutex_up3 (std::unique_ptr<mutex> pp);

void
pass_mutex_up3 (std::unique_ptr<mutex> pp)
{
  printf ("%p\n", pp.get ());
  // fflush(stdout);
}

int
test_cpp_api (void)
{
  // ==========================================================================

  printf ("\n%s - started\n", test_name);
  // fflush(stdout);

#if 0
  for (auto p = scheduler::top_threads_list2_.begin ();
      p != scheduler::top_threads_list2_.end (); ++p)
    {
      printf ("%s\n", p->name ());
    }
#endif

#if 1
#if !defined(OS_USE_RTOS_PORT_SCHEDULER)
  sysclock.sleep_for (5);
  printf ("\nThreads:\n");
  iterate_threads ();
  // fflush(stdout);
#endif
#endif

#if 0
  std::for_each (scheduler::top_threads_list2_.begin (),
      scheduler::top_threads_list2_.end (), [](thread &th)
        { printf ("%s\n", th.name ());});
#endif

  // ==========================================================================

  printf ("\n%s - Memory managers\n", test_name);
  // fflush(stdout);

  {
    char arena[60];

    // The basic object, with explicit separate arena.
    os::memory::block_pool bp1{ "bp1", 2, sizeof (my_blk_t), arena,
                                sizeof (arena) };

    void* b1;
    b1 = bp1.allocate (1, 1);

    void* b2;
    b2 = bp1.allocate (1, 1);

    void* b3;
    b3 = bp1.allocate (1, 1);
    if (b3 == nullptr)
      {
        assert (b3 == nullptr);
      }

    bp1.deallocate (b1, 0, 1);
    bp1.deallocate (b2, 0, 1);
  }

  {
    using my_pool = os::memory::block_pool_typed_inclusive<my_blk_t, 2>;

    // The arena is typed and included in the pool object.
    my_pool bp2{ "bp2" };

    void* b1;
    b1 = bp2.allocate (1, 1);

    void* b2;
    b2 = bp2.allocate (1, 1);

    void* b3;
    b3 = bp2.allocate (1, 1);
    if (b3 == nullptr)
      {
        assert (b3 == nullptr);
      }

    bp2.deallocate (b1, 0, 1);
    bp2.deallocate (b2, 0, 1);
  }

  {
    using my_pool = os::memory::block_pool_typed_allocated<
        my_blk_t, os::rtos::memory::allocator<my_blk_t>>;

    // The arena is typed and dynamically allocated.
    my_pool bp3{ "bp3", 2 };

    void* b1;
    b1 = bp3.allocate (1, 1);

    void* b2;
    b2 = bp3.allocate (1, 1);

    void* b3;
    b3 = bp3.allocate (1, 1);
    if (b3 == nullptr)
      {
        assert (b3 == nullptr);
      }

    bp3.deallocate (b1, 0, 1);
    bp3.deallocate (b2, 0, 1);
  }

  // ==========================================================================

  printf ("\n%s - Threads\n", test_name);
  // fflush(stdout);

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
    // Static threads with allocated stacks.
    thread th1{ func, nullptr };
    thread th2{ "th2", func, nullptr };

    th1.join ();
    th2.join ();
  }

  {
    // Dynamically allocated threads with allocated stacks.
    thread* th3 = new thread{ "th3", func, nullptr };

    th3->join ();
    delete th3;
  }

  {
    // Dynamically allocated threads with allocated stacks.
    std::unique_ptr<thread> th4{ new thread{ "th4", func, nullptr } };

    th4->join ();
  }

  {
    auto th5 = std::make_unique<thread> ("th5", func, nullptr);

    th5->join ();
  }

  {
    // Dynamically allocated threads with allocated stacks.
    // The smart pointer internal data is allocated with the
    // application allocator, but the thread is allocated with
    // the system allocator.
    std::shared_ptr<thread> th6{ new thread{ "th6", func, nullptr } };

    th6->join ();
  }

  {
    // Warning, the thread is not allocated with the system allocator,
    // but with the application allocator.
    auto th7 = std::make_shared<thread> ("th7", func, nullptr);

    th7->join ();
  }

  {
    auto th8 = std::allocate_shared<thread> (
        rtos::memory::allocator<thread> (), "th8", func, nullptr);

    th8->join ();
  }

  {
    auto th9 = rtos::make_shared<thread> ("th9", func, nullptr);

    th9->join ();
  }

  {
    // Smart pointer to thread. Allocated with the mutex allocator (pool).
    rtos::memory::unique_ptr<thread> th
        = rtos::memory::allocate_unique<thread> (
            rtos::memory::allocator_typed<thread> (), "th10", func, nullptr);

    th->join ();

    auto th2 = rtos::memory::allocate_unique<thread> (
        rtos::memory::allocator_typed<thread> (), "th11", func, nullptr);

    th2->join ();
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

  // --------------------------------------------------------------------------

  using my_thread = thread_allocated<
      rtos::memory::allocator<thread::stack::allocation_element_t>>;

  {
    // Static threads with custom allocated stacks.
    my_thread ath1{ func, nullptr };
    my_thread ath2{ "ath2", func, nullptr };

    ath1.join ();
    ath2.join ();
  }

  // --------------------------------------------------------------------------

  {
#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
    // Statically allocated threads.
    static thread_inclusive<> sth1{ func, nullptr };
    static thread_inclusive<> sth2{ "sth2", func, nullptr };
#pragma GCC diagnostic pop

    sth1.join ();
    sth2.join ();
  }

  // ==========================================================================

  printf ("\n%s - Thread stack\n", test_name);
  // fflush(stdout);

  {
    std::size_t n;

    n = thread::stack::default_size ();
    thread::stack::default_size (n);

    n = thread::stack::min_size ();
    thread::stack::min_size (n);

    class thread::stack& stack = this_thread::thread ().stack ();

    stack.bottom ();

    stack.top ();

    stack.check_bottom_magic ();
    stack.check_top_magic ();
  }

  // ==========================================================================

  printf ("\n%s - Thread event flags\n", test_name);
  // fflush(stdout);

  {
    this_thread::flags_clear (flags::all);

    this_thread::thread ().flags_raise (0x3);
    this_thread::flags_wait (0x3, nullptr, flags::mode::all);

    this_thread::thread ().flags_raise (0x3);
    this_thread::flags_try_wait (0x3);

    this_thread::thread ().flags_raise (0x3);
    this_thread::flags_timed_wait (0x3, 10);
  }

  if (test_lost_wakeup (test_name) != 0)
    {
      return 1;
    }

  // ==========================================================================

  printf ("\n%s - Message queues\n", test_name);
  // fflush(stdout);

  // Define two messages.

  my_msg_t msg_out{ 1, "msg" };

  my_msg_t msg_in;

  // --------------------------------------------------------------------------

  // Classic static usage; message size and cast to char* must be supplied
  // manually.
  {
    message_queue cq1{ 3, sizeof (my_msg_t) };

    cq1.send (&msg_out, sizeof (my_msg_t));

    message_queue cq2{ "cq2", 3, sizeof (my_msg_t) };

    cq2.send (&msg_out, sizeof (my_msg_t));

    cq2.reset ();
  }

  // --------------------------------------------------------------------------

  // Classic dynamic usage; message size and cast to char* must be supplied
  // manually.
  {
    message_queue* cq3;
    cq3 = new message_queue{ "cq3", 3, sizeof (my_msg_t) };

    cq3->send (&msg_out, sizeof (my_msg_t));

    // Mandatory delete.
    delete cq3;
  }

  {
    // Smart pointer to message queue. Allocated with the system allocator.
    std::unique_ptr<message_queue> q
        = std::make_unique<message_queue> ("cq4", 3, sizeof (my_msg_t));

    q->send (&msg_out, sizeof (my_msg_t));

    // No delete needed.
  }

  {
    // Smart pointer to mutex. Allocated with the mutex allocator (pool).
    rtos::memory::unique_ptr<message_queue> q
        = rtos::memory::allocate_unique<message_queue> (
            rtos::memory::allocator_typed<message_queue> (), "cq5", 3,
            sizeof (my_msg_t));

    q->send (&msg_out, sizeof (my_msg_t));

    auto q2 = rtos::memory::allocate_unique<message_queue> (
        rtos::memory::allocator_typed<message_queue> (), "cq6", 3,
        sizeof (my_msg_t));

    q2->send (&msg_out, sizeof (my_msg_t));
  }

  // --------------------------------------------------------------------------

  // Template usage; message size and cast are supplied automatically.

  // Define a custom queue type parametrised with the
  // message type.
  using My_queue = message_queue_typed<my_msg_t>;

  {
    My_queue tq1{ 7 };

    tq1.send (&msg_out);
    tq1.receive (&msg_in);

    tq1.try_send (&msg_out);
    tq1.try_receive (&msg_in);

    tq1.timed_send (&msg_out, 1);
    tq1.timed_receive (&msg_in, 1);

    My_queue tq2{ "tq2", 7 };

    tq2.send (&msg_out);
    tq2.receive (&msg_in);
  }

  // --------------------------------------------------------------------------

  {
    My_queue* tq;
    tq = new My_queue{ "tq3", 7 };

    tq->send (&msg_out);
    tq->receive (&msg_in);

    delete tq;
  }

  {
    // Smart pointer to message queue. Allocated with the system allocator.
    std::unique_ptr<My_queue> tq = std::make_unique<My_queue> ("tq4", 7);

    tq->send (&msg_out);
    tq->receive (&msg_in);

    // No delete needed.
  }

  {
    // Uninitialised smart pointer.
    rtos::memory::unique_ptr<My_queue, message_queue> tq0;
    if (tq0 != nullptr)
      {
        tq0->send (&msg_out);
      }

    // Smart pointer to message queue. Allocated with the message
    // queue allocator (pool).
    rtos::memory::unique_ptr<My_queue, message_queue> tq
        = rtos::memory::allocate_unique<My_queue> (
            rtos::memory::allocator_typed<My_queue, message_queue> (), "tq5",
            7);

    tq->send (&msg_out);
    tq->receive (&msg_in);

    auto tq2 = rtos::memory::allocate_unique<My_queue> (
        rtos::memory::allocator_typed<My_queue, message_queue> (), "tq6", 7);

    tq2->send (&msg_out);
    tq2->receive (&msg_in);
  }

  // --------------------------------------------------------------------------

  // Allocated template usage; message size and cast are supplied
  // automatically.

  // Define a custom queue type parametrised with the
  // message type and the queue size.
  using My_inclusive_queue = message_queue_inclusive<my_msg_t, 4>;

  {
    // The space for the queue is allocated inside the queue
    // object, in this case on the stack.
    My_inclusive_queue sq1;

    sq1.send (&msg_out);
    sq1.receive (&msg_in);

    sq1.try_send (&msg_out);
    sq1.try_receive (&msg_in);

    sq1.timed_send (&msg_out, 1);
    sq1.timed_receive (&msg_in, 1);

    My_inclusive_queue sq2{ "sq2" };

    sq2.send (&msg_out);
    sq2.receive (&msg_in);
  }

  // ==========================================================================

  my_blk_t* blk;

  printf ("\n%s - Memory pools\n", test_name);
  // fflush(stdout);

  // Classic static usage; block size and cast to char* must be supplied
  // manually.
  {
    memory_pool cp1{ 3, sizeof (my_blk_t) };

    blk = static_cast<my_blk_t*> (cp1.alloc ());
    cp1.free (blk);

    blk = static_cast<my_blk_t*> (cp1.try_alloc ());
    cp1.free (blk);

    blk = static_cast<my_blk_t*> (cp1.timed_alloc (1));
    cp1.free (blk);

    cp1.reset ();

    memory_pool cp2{ "cp2", 3, sizeof (my_blk_t) };

    blk = static_cast<my_blk_t*> (cp2.alloc ());
    cp2.free (blk);
  }

  // --------------------------------------------------------------------------

  // Classic dynamic usage; block size and cast to char* must be supplied
  // manually.
  {
    memory_pool* cp;
    cp = new memory_pool{ "cp3", 3, sizeof (my_blk_t) };

    blk = static_cast<my_blk_t*> (cp->alloc ());
    cp->free (blk);

    delete cp;
  }

  {
    // Smart pointer to memory pool. Allocated with the system allocator.
    std::unique_ptr<memory_pool> cp
        = std::make_unique<memory_pool> ("cp4", 3, sizeof (my_blk_t));

    blk = static_cast<my_blk_t*> (cp->alloc ());
    cp->free (blk);

    // No delete needed.
  }

  {
    // Uninitialised smart pointer.
    rtos::memory::unique_ptr<memory_pool> cp0;
    if (cp0 != nullptr)
      {
        blk = static_cast<my_blk_t*> (cp0->alloc ());
      }

    // Smart pointer to memory pool. Allocated with the
    // memory pool allocator (pool).
    rtos::memory::unique_ptr<memory_pool> cp
        = rtos::memory::allocate_unique<memory_pool> (
            rtos::memory::allocator_typed<memory_pool> (), "cp5", 3,
            sizeof (my_blk_t));

    blk = static_cast<my_blk_t*> (cp->alloc ());
    cp->free (blk);

    auto cp2 = rtos::memory::allocate_unique<memory_pool> (
        rtos::memory::allocator_typed<memory_pool> (), "cp6", 3,
        sizeof (my_blk_t));

    blk = static_cast<my_blk_t*> (cp2->alloc ());
    cp2->free (blk);
  }

  // --------------------------------------------------------------------------

  // Template usage; block size and cast are supplied automatically.

  // Define a custom queue type parametrised with the
  // message type.
  using My_pool = memory_pool_typed<my_blk_t>;

  {
    My_pool tp1{ 7 };

    blk = tp1.alloc ();
    tp1.free (blk);

    blk = tp1.try_alloc ();
    tp1.free (blk);

    blk = tp1.timed_alloc (1);
    tp1.free (blk);

    My_pool tp2{ "tp2", 7 };

    blk = tp2.alloc ();
    tp2.free (blk);
  }

  // --------------------------------------------------------------------------

  {
    My_pool* tp;
    tp = new My_pool{ "tp3", 7 };

    blk = tp->alloc ();
    tp->free (blk);

    delete tp;
  }

  {
    // Smart pointer to memory pool. Allocated with the system allocator.
    std::unique_ptr<My_pool> tp = std::make_unique<My_pool> ("tp4", 7);

    blk = tp->alloc ();
    tp->free (blk);

    // No delete needed.
  }

  {
    // Uninitialised smart pointer.
    rtos::memory::unique_ptr<My_pool, message_queue> tp0;
    if (tp0 != nullptr)
      {
        blk = tp0->alloc ();
        tp0->free (blk);
      }

    // Smart pointer to memory pool. Allocated with the memory pool
    // allocator (pool).
    rtos::memory::unique_ptr<My_pool, memory_pool> tp
        = rtos::memory::allocate_unique<My_pool> (
            rtos::memory::allocator_typed<My_pool, memory_pool> (), "tp5", 7);

    blk = tp->alloc ();
    tp->free (blk);

    auto tp2 = rtos::memory::allocate_unique<My_pool> (
        rtos::memory::allocator_typed<My_pool, memory_pool> (), "tp6", 7);

    blk = tp2->alloc ();
    tp2->free (blk);
  }

  // --------------------------------------------------------------------------

  // Allocated template usage; block size is supplied automatically.

  // Define a custom pool type parametrised with the
  // block type and the pool size.
  using My_inclusive_pool = memory_pool_inclusive<my_blk_t, 4>;

  {
    // The space for the pool is allocated inside the pool
    // object, in this case on the stack.
    My_inclusive_pool sp1;

    blk = sp1.alloc ();
    sp1.free (blk);

    blk = sp1.try_alloc ();
    sp1.free (blk);

    blk = sp1.timed_alloc (1);
    sp1.free (blk);

    My_inclusive_pool sp2{ "sp2" };

    blk = sp2.alloc ();
    sp2.free (blk);
  }

  // ==========================================================================

  printf ("\n%s - Condition variables\n", test_name);
  // fflush(stdout);

  {
    condition_variable cv1;
    cv1.signal ();

    condition_variable cv2{ "cv2" };
    cv2.signal ();
    cv2.broadcast ();
  }

  {
    condition_variable* cv;
    cv = new condition_variable{ "cv3" };

    cv->signal ();

    // Mandatory delete.
    delete cv;
  }

  {
    // Smart pointer to condition variable. Allocated with the system
    // allocator.
    std::unique_ptr<condition_variable> cv
        = std::make_unique<condition_variable> ("cv4");

    cv->signal ();

    auto cv2 = std::make_unique<condition_variable> ("cv5");

    cv2->signal ();

    // No delete needed.
  }

  {
    // Uninitialised smart pointer.
    rtos::memory::unique_ptr<condition_variable> cv0;
    if (cv0 != nullptr)
      {
        cv0->signal ();
      }

    // Smart pointer to condition variable. Allocated with the
    // condition variable allocator (pool).
    rtos::memory::unique_ptr<condition_variable> cv
        = rtos::memory::allocate_unique<condition_variable> (
            rtos::memory::allocator_typed<condition_variable> (), "cv6");

    cv->signal ();

    auto cv2 = rtos::memory::allocate_unique<condition_variable> (
        rtos::memory::allocator_typed<condition_variable> (), "cv7");

    cv2->signal ();
  }

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

  printf ("\n%s - Event flags\n", test_name);
  // fflush(stdout);

  {
    event_flags ev1;
    ev1.clear (1);

    event_flags ev2{ "ev2" };
    ev2.clear (1);
  }

  {
    event_flags* ev;
    ev = new event_flags{ "ev3" };
    ev->clear (1);

    // Mandatory delete.
    delete ev;
  }

  {
    // Smart pointer to event flags. Allocated with the system allocator.
    std::unique_ptr<event_flags> ev = std::make_unique<event_flags> ("ev4");

    ev->clear (1);

    auto ev2 = std::make_unique<event_flags> ("ev5");

    ev2->clear (1);

    // No delete needed.
  }

  {
    // Uninitialised smart pointer.
    rtos::memory::unique_ptr<event_flags> ev0;
    if (ev0 != nullptr)
      {
        ev0->clear (1);
      }

    // Smart pointer to mutex. Allocated with the mutex allocator (pool).
    rtos::memory::unique_ptr<event_flags> ev
        = rtos::memory::allocate_unique<event_flags> (
            rtos::memory::allocator_typed<event_flags> (), "ev6");

    ev->clear (1);

    auto ev2 = rtos::memory::allocate_unique<event_flags> (
        rtos::memory::allocator_typed<event_flags> (), "ev7");

    ev2->clear (1);
  }

  // ==========================================================================

  printf ("\n%s - Mutexes\n", test_name);
  // fflush(stdout);

  {
    // Unnamed mutex.
    mutex mx1;
    mx1.lock ();
    mx1.unlock ();

    mx1.try_lock ();
    mx1.unlock ();

    mx1.timed_lock (10);
    mx1.unlock ();

    mx1.name ();

    mx1.type ();
    mx1.protocol ();
    mx1.robustness ();
    mx1.owner ();

    thread::priority_t prio = mx1.prio_ceiling ();
    mx1.prio_ceiling (prio);

    mx1.reset ();
  }

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

  {
    // Named mutex created in the local scope (the stack).
    mutex mx{ "mx2" };

    mx.lock ();
    mx.unlock ();
  }

  {
    // Recursive mutex created in the local scope (the stack).
    mutex mx{ "mx3", mutex::initializer_recursive };

    mx.lock ();
    mx.unlock ();

    mutex_recursive mx2{ "mx4" };

    mx2.lock ();
    mx2.unlock ();
  }

  {
    // Raw pointer to mutex. Allocated with the system allocator.
    mutex* mx = new mutex{ "mx5" };

    mx->lock ();
    mx->unlock ();

    // Mandatory delete.
    delete mx;
  }

  {
    // Smart pointer to mutex. Allocated with the system allocator.
    std::unique_ptr<mutex> mx = std::make_unique<mutex> ("mx6");

    mx->lock ();
    mx->unlock ();

    auto mx2 = std::make_unique<mutex> ("mx6");

    mx2->lock ();
    mx2->unlock ();

    // No delete needed.
  }

  {
    // Uninitialised smart pointer.
    rtos::memory::unique_ptr<mutex> mx0;
    if (mx0 != nullptr)
      {
        mx0->lock ();
        mx0->unlock ();
      }

    // Smart pointer to mutex. Allocated with the mutex allocator (pool).
    rtos::memory::unique_ptr<mutex> mx = rtos::memory::allocate_unique<mutex> (
        rtos::memory::allocator_typed<mutex> (), "mx7");

    mx->lock ();
    mx->unlock ();

    auto mx2 = rtos::memory::allocate_unique<mutex> (
        rtos::memory::allocator_typed<mutex> (), "mx8");

    mx2->lock ();
    mx2->unlock ();
  }

  // Unique pointer handling test.
  {
    std::unique_ptr<mutex> mx;
    mx = std::make_unique<mutex> ("mx9");

    mx->lock ();
    mx->unlock ();

    // Pass by const reference.
    pass_mutex_up1 (mx);

    printf ("%p\n", mx.get ());
    // fflush(stdout);

    // Pass by non-const reference.
    pass_mutex_up2 (mx);

    printf ("%p\n", mx.get ());
    // fflush(stdout);

    // Move.
    pass_mutex_up3 (std::move (mx));

    // This pointer is null now, ownership was transferred to function,
    // which in our case destroyed the object.
    printf ("%p\n", mx.get ());
    // fflush(stdout);
  }

  // ==========================================================================

  printf ("\n%s - Semaphores\n", test_name);
  // fflush(stdout);

  {
    // Unnamed counting semaphore.
    semaphore sp;
    sp.post ();
  }

  {
    // Named counting semaphore.
    semaphore sp{ "sp2" };

    sp.post ();
    sp.wait ();

    sp.post ();
    sp.try_wait ();

    sp.post ();
    sp.timed_wait (1);

    sp.post ();
    sp.timed_wait (0xFFFFFFFF);

    sp.reset ();
  }

  {
    // Named binary semaphore.
    semaphore sp{ "sp3", semaphore::initializer_binary };

    sp.post ();
  }

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

  {
    // Raw pointer to semaphore. Allocated with the system allocator.
    semaphore* sp;
    sp = new semaphore{ "sp4" };

    sp->post ();

    // Mandatory delete.
    delete sp;
  }

  {
    // Smart pointer to semaphore. Allocated with the system allocator.
    std::unique_ptr<semaphore> sp = std::make_unique<semaphore> ("sp5");

    sp->post ();

    // No delete needed.
  }

  {
    // Uninitialised smart pointer.
    rtos::memory::unique_ptr<semaphore> sp0;
    if (sp0 != nullptr)
      {
        sp0->post ();
      }

    // Smart pointer to semaphore. Allocated with the
    // semaphore allocator (pool).
    rtos::memory::unique_ptr<semaphore> sp
        = rtos::memory::allocate_unique<semaphore> (
            rtos::memory::allocator_typed<semaphore> (), "sp6");

    sp->post ();

    auto sp2 = rtos::memory::allocate_unique<semaphore> (
        rtos::memory::allocator_typed<semaphore> (), "sp7");

    sp2->post ();
  }

  // ==========================================================================

  printf ("\n%s - Timers\n", test_name);
  // fflush(stdout);

  {
    // Single-shot timer.
    timer tm{ tmfunc, nullptr };
    sysclock.sleep_for (1); // Sync
    tm.start (1);

    sysclock.sleep_for (2);
    tm.stop ();
  }

  {
    // Named single-shot timer.
    timer tm{ "tm2", tmfunc, nullptr };
    sysclock.sleep_for (1); // Sync
    tm.start (1);

    sysclock.sleep_for (2);
    tm.stop ();
  }

  {
    // Static named periodic timer.
    timer tm{ "tm3", tmfunc, nullptr, timer::periodic_initializer };
    sysclock.sleep_for (1); // Sync
    tm.start (1);

    sysclock.sleep_for (2);
    tm.stop ();
  }

  {
    // Dynamic named single-shot timer. Allocated with the system allocator.
    timer* tm;
    tm = new timer{ "tm4", tmfunc, nullptr };

    sysclock.sleep_for (1); // Sync
    tm->start (1);

    sysclock.sleep_for (2);
    tm->stop ();

    // Mandatory delete.
    delete tm;
  }

  {
    // Smart pointer to timer. Allocated with the system allocator.
    std::unique_ptr<timer> tm
        = std::make_unique<timer> ("tm5", tmfunc, nullptr);

    sysclock.sleep_for (1); // Sync
    tm->start (1);

    sysclock.sleep_for (2);
    tm->stop ();

    auto tm2 = std::make_unique<timer> ("tm6", tmfunc, nullptr);

    sysclock.sleep_for (1); // Sync
    tm2->start (1);

    sysclock.sleep_for (2);
    tm2->stop ();

    // No delete needed.
  }

  {
    // Uninitialised smart pointer.
    rtos::memory::unique_ptr<timer> tm0;
    if (tm0 != nullptr)
      {
        tm0->stop ();
      }

    // Smart pointer to mutex. Allocated with the mutex allocator (pool).
    rtos::memory::unique_ptr<timer> tm = rtos::memory::allocate_unique<timer> (
        rtos::memory::allocator_typed<timer> (), "tm7", tmfunc, nullptr);

    sysclock.sleep_for (1); // Sync
    tm->start (1);

    sysclock.sleep_for (2);
    tm->stop ();

    auto tm2 = rtos::memory::allocate_unique<timer> (
        rtos::memory::allocator_typed<timer> (), "tm8", tmfunc, nullptr);

    sysclock.sleep_for (1); // Sync
    tm2->start (1);

    sysclock.sleep_for (2);
    tm2->stop ();
  }

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
  // fflush(stdout);

  return 0;
}

// ----------------------------------------------------------------------------
