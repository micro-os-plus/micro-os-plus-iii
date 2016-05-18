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

int
run_template_tests (void);

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
  printf ("%s\n", __func__);

  return nullptr;
}

int
run_template_tests (void)
{
  using my_thread = Thread_allocated<memory::new_delete_allocator<stack::allocation_element_t>>;

    {
      my_thread th
        { func, nullptr };
    }

  static Thread_static<> ths
    { func, nullptr };

#if 1
  // Define two messages.

  my_msg_t msg_out
    { 1, "msg" };

  my_msg_t msg_in;

  // --------------------------------------------------------------------------

  // Classic usage; message size and cast to char* must be supplied manually.
    {
      Message_queue cq
        { 3, sizeof(my_msg_t) };

      cq.send ((char*) &msg_out, sizeof(my_msg_t));
    }

  // --------------------------------------------------------------------------

  // Template usage; message size and cast are supplied automatically.

  // Define a custom queue type parametrised with the
  // message type.
  using My_queue = Message_queue_typed<my_msg_t>;

    {
      My_queue q
        { 7 };

      q.send (&msg_out);
      q.receive (&msg_in);

      q.try_send (&msg_out);
      q.try_receive (&msg_in);

      q.timed_send (&msg_out, 1);
      q.timed_receive (&msg_in, 1);
    }

  // --------------------------------------------------------------------------

  // Allocated template usage; message size and cast are supplied automatically.

  // Define a custom queue type parametrised with the
  // message type and the queue size.
  using My_static_queue = Message_queue_static<my_msg_t, 4>;

    {
      // The space for the queue is allocated inside the queue
      // object, in this case on the stack.
      My_static_queue sq;

      sq.send (&msg_out);
      sq.receive (&msg_in);

      sq.try_send (&msg_out);
      sq.try_receive (&msg_in);

      sq.timed_send (&msg_out, 1);
      sq.timed_receive (&msg_in, 1);
    }

  // ==========================================================================

  my_blk_t* blk;

  // Classic usage; block size and cast to char* must be supplied manually.
    {
      Memory_pool cp
        { 3, sizeof(my_blk_t) };

      blk = static_cast<my_blk_t*> (cp.alloc ());
      cp.free (blk);

      blk = static_cast<my_blk_t*> (cp.try_alloc ());
      cp.free (blk);

      blk = static_cast<my_blk_t*> (cp.timed_alloc (1));
      cp.free (blk);
    }

  // --------------------------------------------------------------------------

  // Template usage; block size and cast are supplied automatically.

  // Define a custom queue type parametrised with the
  // message type.
  using My_pool = Memory_pool_typed<my_blk_t>;

    {
      My_pool p
        { 7 };

      blk = p.alloc ();
      p.free (blk);

      blk = p.try_alloc ();
      p.free (blk);

      blk = p.timed_alloc (1);
      p.free (blk);
    }

  // --------------------------------------------------------------------------

  // Allocated template usage; block size is supplied automatically.

  // Define a custom pool type parametrised with the
  // block type and the pool size.
  using My_static_pool = Memory_pool_static<my_blk_t, 4>;

    {
      // The space for the pool is allocated inside the pool
      // object, in this case on the stack.
      My_static_pool sp;

      blk = sp.alloc ();
      sp.free (blk);

      blk = sp.try_alloc ();
      sp.free (blk);

      blk = sp.timed_alloc (1);
      sp.free (blk);
    }

#endif

  trace::puts ("\nDone.");
  return 0;
}

