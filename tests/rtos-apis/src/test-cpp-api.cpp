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

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Waggregate-return"
#endif

// ----------------------------------------------------------------------------

using namespace os;
using namespace os::rtos;

// ----------------------------------------------------------------------------

static const char* test_name = "Test C++ API";

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
func (void* args __attribute__((unused)))
{
  printf ("> %s() on thread %s\n", __func__, this_thread::thread ().name ());

  return nullptr;
}

void
tmfunc (void* args);

void
tmfunc (void* args __attribute__((unused)))
{
  printf (">> %s()\n", __func__);
}

#if !defined(OS_USE_RTOS_PORT_SCHEDULER)

void
iterate_threads (thread* th = nullptr, unsigned int depth = 0);

static const char* thread_state[] =
  { "undf", "inac", "rdy", "run", "wait", "term", "dead", "init" };

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
  printf ("%p\n", pp.get ());
}

void
pass_mutex_up2 (std::unique_ptr<mutex>& pp);

void
pass_mutex_up2 (std::unique_ptr<mutex>& pp)
{
  printf ("%p\n", pp.get ());
}

void
pass_mutex_up3 (std::unique_ptr<mutex> pp);

void
pass_mutex_up3 (std::unique_ptr<mutex> pp)
{
  printf ("%p\n", pp.get ());
}

int
test_cpp_api (void)
{
  // ==========================================================================

  printf ("\n%s - started\n", test_name);

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

  printf ("\n%s - Memory managers\n", test_name);

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
      using my_pool = os::memory::block_pool_typed_inclusive<my_blk_t, 2>;

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

  printf ("\n%s - Threads\n", test_name);

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

    {
      // Smart pointer to thread. Allocated with the mutex allocator (pool).
      rtos::memory::unique_ptr<thread> th =
          rtos::memory::allocate_unique<thread> (
              rtos::memory::allocator_typed<thread> (), "th10", func, nullptr);

      th->join ();

      auto th2 = rtos::memory::allocate_unique<thread> (
          rtos::memory::allocator_typed<thread> (), "th11", func, nullptr);

      th2->join ();
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
      static thread_inclusive<> sth1
        { func, nullptr };
      static thread_inclusive<> sth2
        { "sth2", func, nullptr };
#pragma GCC diagnostic pop

      sth1.join ();
      sth2.join ();
    }

  // ==========================================================================

  printf ("\n%s - Thread stack\n", test_name);

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

  printf ("\n%s - Message queues\n", test_name);

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

      // Mandatory delete.
      delete cq3;
    }

    {
      // Smart pointer to message queue. Allocated with the system allocator.
      std::unique_ptr<message_queue> q = std::make_unique<message_queue> (
          "cq4", 3, sizeof(my_msg_t));

      q->send (&msg_out, sizeof(my_msg_t));

      // No delete needed.
    }

    {
      // Smart pointer to mutex. Allocated with the mutex allocator (pool).
      rtos::memory::unique_ptr<message_queue> q = rtos::memory::allocate_unique<
          message_queue> (rtos::memory::allocator_typed<message_queue> (),
                          "cq5", 3, sizeof(my_msg_t));

      q->send (&msg_out, sizeof(my_msg_t));

      auto q2 = rtos::memory::allocate_unique<message_queue> (
          rtos::memory::allocator_typed<message_queue> (), "cq6", 3,
          sizeof(my_msg_t));

      q2->send (&msg_out, sizeof(my_msg_t));
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
      My_queue* tq;
      tq = new My_queue
        { "tq3", 7 };

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
      rtos::memory::unique_ptr<My_queue, message_queue> tq =
          rtos::memory::allocate_unique<My_queue> (
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

  // Allocated template usage; message size and cast are supplied automatically.

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

      My_inclusive_queue sq2
        { "sq2" };

      sq2.send (&msg_out);
      sq2.receive (&msg_in);

    }

  // ==========================================================================

  printf ("\n%s - Memory pools\n", test_name);

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
      memory_pool* cp;
      cp = new memory_pool
        { "cp3", 3, sizeof(my_blk_t) };

      blk = static_cast<my_blk_t*> (cp->alloc ());
      cp->free (blk);

      delete cp;
    }

    {
      // Smart pointer to memory pool. Allocated with the system allocator.
      std::unique_ptr<memory_pool> cp = std::make_unique<memory_pool> (
          "cp4", 3, sizeof(my_blk_t));

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
      rtos::memory::unique_ptr<memory_pool> cp = rtos::memory::allocate_unique<
          memory_pool> (rtos::memory::allocator_typed<memory_pool> (), "cp5", 3,
                        sizeof(my_blk_t));

      blk = static_cast<my_blk_t*> (cp->alloc ());
      cp->free (blk);

      auto cp2 = rtos::memory::allocate_unique<memory_pool> (
          rtos::memory::allocator_typed<memory_pool> (), "cp6", 3,
          sizeof(my_blk_t));

      blk = static_cast<my_blk_t*> (cp2->alloc ());
      cp2->free (blk);
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
      My_pool* tp;
      tp = new My_pool
        { "tp3", 7 };

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
      rtos::memory::unique_ptr<My_pool, memory_pool> tp =
          rtos::memory::allocate_unique<My_pool> (
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

      My_inclusive_pool sp2
        { "sp2" };

      blk = sp2.alloc ();
      sp2.free (blk);
    }

  // ==========================================================================

  printf ("\n%s - Condition variables\n", test_name);
    {
      condition_variable cv1;
      cv1.signal ();

      condition_variable cv2
        { "cv2" };
      cv2.signal ();
    }

    {
      condition_variable* cv;
      cv = new condition_variable
        { "cv3" };

      cv->signal ();

      // Mandatory delete.
      delete cv;
    }

    {
      // Smart pointer to condition variable. Allocated with the system allocator.
      std::unique_ptr<condition_variable> cv = std::make_unique<
          condition_variable> ("cv4");

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
      rtos::memory::unique_ptr<condition_variable> cv =
          rtos::memory::allocate_unique<condition_variable> (
              rtos::memory::allocator_typed<condition_variable> (), "cv6");

      cv->signal ();

      auto cv2 = rtos::memory::allocate_unique<condition_variable> (
          rtos::memory::allocator_typed<condition_variable> (), "cv7");

      cv2->signal ();
    }

  // ==========================================================================

  printf ("\n%s - Event flags\n", test_name);

    {
      event_flags ev1;
      ev1.clear (1);

      event_flags ev2
        { "ev2" };
      ev2.clear (1);
    }

    {
      event_flags* ev;
      ev = new event_flags
        { "ev3" };
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
      rtos::memory::unique_ptr<event_flags> ev = rtos::memory::allocate_unique<
          event_flags> (rtos::memory::allocator_typed<event_flags> (), "ev6");

      ev->clear (1);

      auto ev2 = rtos::memory::allocate_unique<event_flags> (
          rtos::memory::allocator_typed<event_flags> (), "ev7");

      ev2->clear (1);
    }

  // ==========================================================================

  printf ("\n%s - Mutexes\n", test_name);

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

      mutex_recursive mx2
        { "mx4" };

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
      rtos::memory::unique_ptr<mutex> mx =
          rtos::memory::allocate_unique<mutex> (
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

      // Pass by non-const reference.
      pass_mutex_up2 (mx);

      printf ("%p\n", mx.get ());

      // Move.
      pass_mutex_up3 (std::move (mx));

      // This pointer is null now, ownership was transferred to function,
      // which in our case destroyed the object.
      printf ("%p\n", mx.get ());
    }

  // ==========================================================================

  printf ("\n%s - Semaphores\n", test_name);

    {
      // Unnamed counting semaphore.
      semaphore sp;
      sp.post ();
    }

    {
      // Named counting semaphore.
      semaphore sp
        { "sp2" };

      sp.post ();
      sp.wait ();

      sp.post ();
      sp.try_wait ();

      sp.post ();
      sp.timed_wait (1);

      sp.post ();
      sp.timed_wait (0xFFFFFFFF);
    }

    {
      // Named binary semaphore.
      semaphore sp
        { "sp3", semaphore::initializer_binary };

      sp.post ();
    }

    {
      // Raw pointer to semaphore. Allocated with the system allocator.
      semaphore* sp;
      sp = new semaphore
        { "sp4" };

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
      rtos::memory::unique_ptr<semaphore> sp = rtos::memory::allocate_unique<
          semaphore> (rtos::memory::allocator_typed<semaphore> (), "sp6");

      sp->post ();

      auto sp2 = rtos::memory::allocate_unique<semaphore> (
          rtos::memory::allocator_typed<semaphore> (), "sp7");

      sp2->post ();
    }

  // ==========================================================================

  printf ("\n%s - Timers\n", test_name);

    {
      // Single-shot timer.
      timer tm
        { tmfunc, nullptr };
      sysclock.sleep_for (1); // Sync
      tm.start (1);

      sysclock.sleep_for (2);
      tm.stop ();
    }

    {
      // Named single-shot timer.
      timer tm
        { "tm2", tmfunc, nullptr };
      sysclock.sleep_for (1); // Sync
      tm.start (1);

      sysclock.sleep_for (2);
      tm.stop ();
    }

    {
      // Static named periodic timer.
      timer tm
        { "tm3", tmfunc, nullptr, timer::periodic_initializer };
      sysclock.sleep_for (1); // Sync
      tm.start (1);

      sysclock.sleep_for (2);
      tm.stop ();
    }

    {
      // Dynamic named single-shot timer. Allocated with the system allocator.
      timer* tm;
      tm = new timer
        { "tm4", tmfunc, nullptr };

      sysclock.sleep_for (1); // Sync
      tm->start (1);

      sysclock.sleep_for (2);
      tm->stop ();

      // Mandatory delete.
      delete tm;
    }

    {
      // Smart pointer to timer. Allocated with the system allocator.
      std::unique_ptr<timer> tm = std::make_unique<timer> ("tm5", tmfunc,
                                                           nullptr);

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
      rtos::memory::unique_ptr<timer> tm =
          rtos::memory::allocate_unique<timer> (
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

  // ==========================================================================

  printf ("\n%s - done\n", test_name);
  return 0;
}

// ----------------------------------------------------------------------------
