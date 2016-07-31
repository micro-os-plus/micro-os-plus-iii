/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
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

#include <cstdio>
#include <algorithm>

#include <test-cpp-api.h>

// ----------------------------------------------------------------------------

static const char* test_name = "Test C++ API";

using namespace os;
using namespace os::rtos;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

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
func (void* args __attribute__((unused)))
{
  printf ("%s\n", __func__);

  return nullptr;
}

void
tmfunc (void* args);

void
tmfunc (void* args __attribute__((unused)))
{
  printf ("%s\n", __func__);
}

#if !defined(OS_USE_RTOS_PORT_SCHEDULER)

void
iterate_threads (thread* th = nullptr, unsigned int depth = 0);

static const char* thread_state[] =
  { "undf", "inac", "rdy", "run", "wait", "term", "dead" };

void
iterate_threads (thread* th, unsigned int depth)
{
  for (auto&& p : scheduler::children_threads (th))
    {
      class thread::stack& stk = p.stack ();
      unsigned int used = static_cast<unsigned int> (stk.size ()
          - stk.available ());
      unsigned int used_proc = static_cast<unsigned int> (used * 100
          / stk.size ());
      unsigned int st = static_cast<unsigned int> (p.state ());

      statistics::counter_t thread_switches =
          p.statistics ().context_switches ();

      statistics::duration_t thread_cpu_cycles = p.statistics ().cpu_cycles ();

      printf ("%s, %u%% (%u/%u), %s, %u, %u \n", p.name (), used_proc, used,
              static_cast<unsigned int> (stk.size ()), thread_state[st],
              static_cast<unsigned int> (thread_switches),
              static_cast<unsigned int> (thread_cpu_cycles));

      iterate_threads (&p, depth + 1);
    }
}

#endif

int
test_cpp_api (void)
{
  // ==========================================================================

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
#endif
#endif

#if 0
  std::for_each (scheduler::top_threads_list2_.begin (),
      scheduler::top_threads_list2_.end (), [](thread &th)
        { printf ("%s\n", th.name ());});
#endif

  // ==========================================================================

  printf ("\n%s - Threads.\n", test_name);

    {
      // Regular threads.
      thread th1
        { func, nullptr };
      thread th2
        { "th2", func, nullptr };

      th1.join ();
      th2.join ();
    }

  // --------------------------------------------------------------------------

  using my_thread = thread_allocated<memory::new_delete_allocator<thread::stack::allocation_element_t>>;

  // Allocated threads.
    {
      my_thread ath1
        { func, nullptr };
      my_thread ath2
        { "ath2", func, nullptr };

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
      static thread_static<> sth1
        { func, nullptr };
      static thread_static<> sth2
        { "sth2", func, nullptr };
#pragma GCC diagnostic pop

      sth1.join ();
      sth2.join ();
    }

  // ==========================================================================

  printf ("\n%s - Thread stack.\n", test_name);

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

  printf ("\n%s - Thread event flags.\n", test_name);

    {
      this_thread::flags_clear (flags::all);

      this_thread::thread ().flags_raise (0x3);
      this_thread::flags_wait (0x3, nullptr, flags::mode::all);

      this_thread::thread ().flags_raise (0x3);
      this_thread::flags_try_wait (0x3);

      this_thread::thread ().flags_raise (0x3);
      this_thread::flags_timed_wait (0x3, 10);
    }

  // ==========================================================================

  printf ("\n%s - Message queues.\n", test_name);

  // Define two messages.

  my_msg_t msg_out
    { 1, "msg" };

  my_msg_t msg_in;

  // --------------------------------------------------------------------------

  // Classic usage; message size and cast to char* must be supplied manually.
    {
      message_queue cq1
        { 3, sizeof(my_msg_t) };

      cq1.send (&msg_out, sizeof(my_msg_t));

      message_queue cq2
        { "cq2", 3, sizeof(my_msg_t) };

      cq2.send (&msg_out, sizeof(my_msg_t));

    }

  // --------------------------------------------------------------------------

  // Template usage; message size and cast are supplied automatically.

  // Define a custom queue type parametrised with the
  // message type.
  using My_queue = message_queue_typed<my_msg_t>;

    {
      My_queue tq1
        { 7 };

      tq1.send (&msg_out);
      tq1.receive (&msg_in);

      tq1.try_send (&msg_out);
      tq1.try_receive (&msg_in);

      tq1.timed_send (&msg_out, 1);
      tq1.timed_receive (&msg_in, 1);

      My_queue tq2
        { "tq2", 7 };

      tq2.send (&msg_out);
      tq2.receive (&msg_in);

    }

  // --------------------------------------------------------------------------

  // Allocated template usage; message size and cast are supplied automatically.

  // Define a custom queue type parametrised with the
  // message type and the queue size.
  using My_static_queue = message_queue_static<my_msg_t, 4>;

    {
      // The space for the queue is allocated inside the queue
      // object, in this case on the stack.
      My_static_queue sq1;

      sq1.send (&msg_out);
      sq1.receive (&msg_in);

      sq1.try_send (&msg_out);
      sq1.try_receive (&msg_in);

      sq1.timed_send (&msg_out, 1);
      sq1.timed_receive (&msg_in, 1);

      My_static_queue sq2
        { "sq2" };

      sq2.send (&msg_out);
      sq2.receive (&msg_in);

    }

  // ==========================================================================

  printf ("\n%s - Memory pools.\n", test_name);

  my_blk_t* blk;

  // Classic usage; block size and cast to char* must be supplied manually.
    {
      memory_pool cp1
        { 3, sizeof(my_blk_t) };

      blk = static_cast<my_blk_t*> (cp1.alloc ());
      cp1.free (blk);

      blk = static_cast<my_blk_t*> (cp1.try_alloc ());
      cp1.free (blk);

      blk = static_cast<my_blk_t*> (cp1.timed_alloc (1));
      cp1.free (blk);

      memory_pool cp2
        { "cp2", 3, sizeof(my_blk_t) };

      blk = static_cast<my_blk_t*> (cp2.alloc ());
      cp2.free (blk);

    }

  // --------------------------------------------------------------------------

  // Template usage; block size and cast are supplied automatically.

  // Define a custom queue type parametrised with the
  // message type.
  using My_pool = memory_pool_typed<my_blk_t>;

    {
      My_pool tp1
        { 7 };

      blk = tp1.alloc ();
      tp1.free (blk);

      blk = tp1.try_alloc ();
      tp1.free (blk);

      blk = tp1.timed_alloc (1);
      tp1.free (blk);

      My_pool tp2
        { "tp2", 7 };

      blk = tp2.alloc ();
      tp2.free (blk);

    }

  // --------------------------------------------------------------------------

  // Allocated template usage; block size is supplied automatically.

  // Define a custom pool type parametrised with the
  // block type and the pool size.
  using My_static_pool = memory_pool_static<my_blk_t, 4>;

    {
      // The space for the pool is allocated inside the pool
      // object, in this case on the stack.
      My_static_pool sp1;

      blk = sp1.alloc ();
      sp1.free (blk);

      blk = sp1.try_alloc ();
      sp1.free (blk);

      blk = sp1.timed_alloc (1);
      sp1.free (blk);

      My_static_pool sp2
        { "sp2" };

      blk = sp2.alloc ();
      sp2.free (blk);

    }

  // ==========================================================================

  printf ("\n%s - Condition variables.\n", test_name);
    {
      condition_variable cv1;
      cv1.signal ();

      condition_variable cv2
        { "cv2" };
      cv2.signal ();
    }

  // ==========================================================================

  printf ("\n%s - Event flags.\n", test_name);

    {
      event_flags ev1;
      ev1.clear (1);

      event_flags ev2
        { "ev2" };
      ev2.clear (1);
    }

  // ==========================================================================

  printf ("\n%s - Mutexes.\n", test_name);

    {
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
      mutex mx2
        { "mx2" };
      mx2.lock ();
      mx2.unlock ();
    }

    {
      mutex mx3
        { "mx2", mutex::initializer_recursive };
      mx3.lock ();
      mx3.unlock ();
    }

  // ==========================================================================

  printf ("\n%s - Semaphores.\n", test_name);

    {
      // Counting semaphore.
      semaphore sp1;
      sp1.post ();
    }

    {
      // Named counting semaphore.
      semaphore sp2
        { "sp2" };

      sp2.post ();
      sp2.wait ();

      sp2.post ();
      sp2.try_wait ();

      sp2.post ();
      sp2.timed_wait (1);

      sp2.post ();
      sp2.timed_wait (0xFFFFFFFF);
    }

    {
      // Named binary semaphore.
      semaphore sp3
        { "sp3", semaphore::initializer_binary };
      sp3.post ();
    }

  // ==========================================================================

  printf ("\n%s - Timers.\n", test_name);

    {
      // Single-shot timer.
      timer tm1
        { tmfunc, nullptr };
      sysclock.sleep_for (1); // Sync
      tm1.start (1);

      sysclock.sleep_for (2);
      tm1.stop ();
    }

    {
      // Named single-shot timer.
      timer tm2
        { "tm2", tmfunc, nullptr };
      sysclock.sleep_for (1); // Sync
      tm2.start (1);

      sysclock.sleep_for (2);
      tm2.stop ();
    }

    {
      // Named periodic timer.
      timer tm3
        { "tm3", tmfunc, nullptr, timer::periodic_initializer };
      sysclock.sleep_for (1); // Sync
      tm3.start (1);

      sysclock.sleep_for (2);
      tm3.stop ();
    }

  // ==========================================================================

  printf ("\n%s - Done.\n", test_name);
  return 0;
}

