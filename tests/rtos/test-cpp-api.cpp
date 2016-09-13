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
#include <cmsis-plus/memory/block-pool.h>
#include <cmsis-plus/memory/lifo.h>
#include <cmsis-plus/estd/memory_resource>
#include <cmsis-plus/estd/mutex>

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

void
pass_mutex_up1 (const std::unique_ptr<mutex>& pp);

void
pass_mutex_up1 (const std::unique_ptr<mutex>& pp)
{
  trace::printf ("%p\n", pp.get ());
}

void
pass_mutex_up2 (std::unique_ptr<mutex>& pp);

void
pass_mutex_up2 (std::unique_ptr<mutex>& pp)
{
  trace::printf ("%p\n", pp.get ());
}

void
pass_mutex_up3 (std::unique_ptr<mutex> pp);

void
pass_mutex_up3 (std::unique_ptr<mutex> pp)
{
  trace::printf ("%p\n", pp.get ());
}

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

  my_blk_t* blk;

  // ==========================================================================

  printf ("\n%s - Memory managers.\n", test_name);

    {
      char arena[60];

      // The basic object, with explicit separate arena.
      os::memory::block_pool bp1
        { "bp2", 2, sizeof(my_blk_t), arena, sizeof(arena) };

      void* b1;
      b1 = bp1.allocate (1, 1);

      void* b2;
      b2 = bp1.allocate (1, 1);

      void* b3;
      b3 = bp1.allocate (1, 1);
      if (b3 == nullptr)
        {
          assert(b3 == nullptr);
        }

      bp1.deallocate (b1, 0, 1);
      bp1.deallocate (b2, 0, 1);
    }

    {
      using my_pool = os::memory::block_pool_typed_static<my_blk_t, 2>;

      // The arena is typed and included in the pool object.
      my_pool bp2
        { "bp2" };

      void* b1;
      b1 = bp2.allocate (1, 1);

      void* b2;
      b2 = bp2.allocate (1, 1);

      void* b3;
      b3 = bp2.allocate (1, 1);
      if (b3 == nullptr)
        {
          assert(b3 == nullptr);
        }

      bp2.deallocate (b1, 0, 1);
      bp2.deallocate (b2, 0, 1);
    }

    {
      using my_pool = os::memory::block_pool_typed_allocated<my_blk_t,
      os::rtos::memory::allocator<my_blk_t>>;

      // The arena is typed and dynamically allocated.
      my_pool bp3
        { "bp3", 2 };

      void* b1;
      b1 = bp3.allocate (1, 1);

      void* b2;
      b2 = bp3.allocate (1, 1);

      void* b3;
      b3 = bp3.allocate (1, 1);
      if (b3 == nullptr)
        {
          assert(b3 == nullptr);
        }

      bp3.deallocate (b1, 0, 1);
      bp3.deallocate (b2, 0, 1);
    }

  // ==========================================================================

  printf ("\n%s - Threads.\n", test_name);

    {
      // Static threads with allocated stacks.
      thread th1
        { func, nullptr };
      thread th2
        { "th2", func, nullptr };

      th1.join ();
      th2.join ();
    }

    {
      // Dynamically allocated threads with allocated stacks.
      thread* th3 = new thread
        { "th3", func, nullptr };

      th3->join ();
      delete th3;
    }

    {
      // Dynamically allocated threads with allocated stacks.
      std::unique_ptr<thread> th4
        { new thread
          { "th4", func, nullptr } };

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
      std::shared_ptr<thread> th6
        { new thread
          { "th6", func, nullptr } };

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

  // --------------------------------------------------------------------------

  using my_thread = thread_allocated<rtos::memory::allocator<thread::stack::allocation_element_t>>;

    {
      // Static threads with custom allocated stacks.
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

  // Classic static usage; message size and cast to char* must be supplied manually.
    {
      message_queue cq1
        { 3, sizeof(my_msg_t) };

      cq1.send (&msg_out, sizeof(my_msg_t));

      message_queue cq2
        { "cq2", 3, sizeof(my_msg_t) };

      cq2.send (&msg_out, sizeof(my_msg_t));
    }

  // --------------------------------------------------------------------------

  // Classic dynamic usage; message size and cast to char* must be supplied manually.
    {
      message_queue* cq3;
      cq3 = new message_queue
        { "cq3", 3, sizeof(my_msg_t) };

      cq3->send (&msg_out, sizeof(my_msg_t));

      delete cq3;
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

    {
      My_queue* tq3;
      tq3 = new My_queue
        { "tq3", 7 };

      tq3->send (&msg_out);
      tq3->receive (&msg_in);

      delete tq3;
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

  // Classic static usage; block size and cast to char* must be supplied manually.
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

  // Classic dynamic usage; block size and cast to char* must be supplied manually.
    {
      memory_pool* cp3;
      cp3 = new memory_pool
        { "cp3", 3, sizeof(my_blk_t) };

      blk = static_cast<my_blk_t*> (cp3->alloc ());
      cp3->free (blk);

      delete cp3;
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

    {

      My_pool* tp3;
      tp3 = new My_pool
        { "tp3", 7 };

      blk = tp3->alloc ();
      tp3->free (blk);

      delete tp3;
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

    {
      condition_variable* cv3;
      cv3 = new condition_variable
        { "cv3" };

      cv3->signal ();

      delete cv3;
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

    {
      event_flags* ev3;
      ev3 = new event_flags
        { "ev3" };
      ev3->clear (1);

      delete ev3;
    }

  // ==========================================================================

  printf ("\n%s - Mutexes.\n", test_name);

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
      // Named mutex created in the local scope (the stack).
      mutex mx
        { "mx2" };

      mx.lock ();
      mx.unlock ();
    }

    {
      // Recursive mutex created in the local scope (the stack).
      mutex mx
        { "mx3", mutex::initializer_recursive };

      mx.lock ();
      mx.unlock ();

      mutex_recursive mx2 { "mx4" };

      mx2.lock ();
      mx2.unlock ();
    }

    {
      // Raw pointer to mutex. Allocated with the system allocator.
      mutex* mx = new mutex
        { "mx5" };

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

      // No delete needed.
    }

    {
      std::unique_ptr<mutex> mx;
      mx = std::make_unique<mutex> ("mx7");

      mx->lock ();
      mx->unlock ();

      // Pass by const reference.
      pass_mutex_up1 (mx);

      trace::printf ("%p\n", mx.get ());

      // Pass by non-const reference.
      pass_mutex_up2 (mx);

      trace::printf ("%p\n", mx.get ());

      // Move.
      pass_mutex_up3 (std::move (mx));

      // This pointer is null now, ownership was transfered to function,
      // which in our case destroyed the object.
      trace::printf ("%p\n", mx.get ());
    }

    {
      // Smart pointer to mutex. Allocated with the mutex allocator (pool).
      rtos::memory::unique_ptr<mutex> mx =
          rtos::memory::allocate_unique<mutex> (
              rtos::memory::allocator_typed<mutex> (), "mx8");

      mx->lock ();
      mx->unlock ();

      auto mx2 = rtos::memory::allocate_unique<mutex> (
          rtos::memory::allocator_typed<mutex> (), "mx9");

      mx2->lock ();
      mx2->unlock ();
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

    {
      // Named semaphore.
      semaphore* sp4;
      sp4 = new semaphore
        { "sp4" };
      sp4->post ();

      delete sp4;
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
      // Static named periodic timer.
      timer tm3
        { "tm3", tmfunc, nullptr, timer::periodic_initializer };
      sysclock.sleep_for (1); // Sync
      tm3.start (1);

      sysclock.sleep_for (2);
      tm3.stop ();
    }

    {
      // Dynamic named single-shot timer.
      timer* tm4;
      tm4 = new timer
        { "tm4", tmfunc, nullptr };
      sysclock.sleep_for (1); // Sync
      tm4->start (1);

      sysclock.sleep_for (2);
      tm4->stop ();

      delete tm4;
    }

  // ==========================================================================

  printf ("\n%s - Done.\n", test_name);
  return 0;
}

