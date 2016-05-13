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

using namespace os;
using namespace os::rtos;

int
run_template_tests (void);

typedef struct my_msg_s
{
  int i;
  const char* s;
} my_msg_t;

int
run_template_tests (void)
{
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

#endif

  trace::puts ("\nDone.");
  return 0;
}

