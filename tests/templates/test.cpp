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
#include <cmsis-plus/diag/trace.h>

#include <cstdio>

using namespace os;
using namespace os::rtos;

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

void*
func (void* args);

void*
func (void* args __attribute__((unused)))
{
  trace::printf ("%s\n", __func__);

  return nullptr;
}

int
run_tests (void)
{
  // ==========================================================================

    {
      // Regular threads.
      Thread th1
        { func, nullptr };
      Thread th2
        { "th2", func, nullptr };

      th1.join ();
      th2.join ();
    }

  // --------------------------------------------------------------------------

  using my_thread = Thread_allocated<memory::new_delete_allocator<stack::allocation_element_t>>;

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
      // Statically allocated threads.
      static Thread_static<> sth1
        { func, nullptr };
      static Thread_static<> sth2
        { "sth2", func, nullptr };

      sth1.join ();
      sth2.join ();
    }

  // ==========================================================================

  // Define two messages.

  my_msg_t msg_out
    { 1, "msg" };

  my_msg_t msg_in;

  // --------------------------------------------------------------------------

  // Classic usage; message size and cast to char* must be supplied manually.
    {
      Message_queue cq1
        { 3, sizeof(my_msg_t) };

      cq1.send ((char*) &msg_out, sizeof(my_msg_t));

      Message_queue cq2
        { "cq2", 3, sizeof(my_msg_t) };

      cq2.send ((char*) &msg_out, sizeof(my_msg_t));

    }

  // --------------------------------------------------------------------------

  // Template usage; message size and cast are supplied automatically.

  // Define a custom queue type parametrised with the
  // message type.
  using My_queue = Message_queue_typed<my_msg_t>;

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
  using My_static_queue = Message_queue_static<my_msg_t, 4>;

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

  my_blk_t* blk;

  // Classic usage; block size and cast to char* must be supplied manually.
    {
      Memory_pool cp1
        { 3, sizeof(my_blk_t) };

      blk = static_cast<my_blk_t*> (cp1.alloc ());
      cp1.free (blk);

      blk = static_cast<my_blk_t*> (cp1.try_alloc ());
      cp1.free (blk);

      blk = static_cast<my_blk_t*> (cp1.timed_alloc (1));
      cp1.free (blk);

      Memory_pool cp2
        { "cp2", 3, sizeof(my_blk_t) };

      blk = static_cast<my_blk_t*> (cp2.alloc ());
      cp2.free (blk);

    }

  // --------------------------------------------------------------------------

  // Template usage; block size and cast are supplied automatically.

  // Define a custom queue type parametrised with the
  // message type.
  using My_pool = Memory_pool_typed<my_blk_t>;

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
  using My_static_pool = Memory_pool_static<my_blk_t, 4>;

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

    {
      Condition_variable cv1;
      cv1.signal ();

      Condition_variable cv2
        { "cv2" };
      cv2.signal ();
    }

  // ==========================================================================

    {
      Event_flags ev1;
      ev1.clear (1);

      Event_flags ev2
        { "ev2" };
      ev2.clear (1);
    }

  // ==========================================================================

    {
      Mutex mx1;
      mx1.lock ();
      mx1.unlock ();

      Mutex mx2
        { "mx2" };
      mx2.lock ();
      mx2.unlock ();
    }

  // ==========================================================================

    {
      Semaphore sp1;
      sp1.post ();

      Semaphore sp2
        { "sp2" };
      sp2.post ();
    }

  // ==========================================================================

  trace::puts ("\nDone.");
  return 0;
}

