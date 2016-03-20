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
#include <cmsis-plus/rtos/os-lists.h>
#include <cmsis-plus/diag/trace.h>

namespace os
{
  namespace rtos
  {
    // ------------------------------------------------------------------------

    Waiting_threads_list::Waiting_threads_list ()
    {
      clear ();
    }

    Waiting_threads_list::~Waiting_threads_list ()
    {
      assert(head_ == nullptr);
      assert(count_ == 0);
    }

    void
    Waiting_threads_list::clear (void)
    {
      head_ = nullptr;
      count_ = 0;
    }

    void
    Waiting_threads_list::add (DoubleListNodeThread& node)
    {
      if (head_ == nullptr)
        {
          // Make the node point to itself, to satisfy the double
          // linked circular list.
          node.prev = &node;
          node.next = &node;

          head_ = &node;
          count_ = 1;
        }
      else
        {
          // Insert at the end of the list.
          DoubleListLinks* after = (head_->prev);

          // Make the new node point to its neighbours.
          node.prev = after;
          node.next = after->next;

          // Make the neighbours point to the node. The order is important.
          after->next->prev = &node;
          after->next = &node;

          ++count_;
        }
    }

    void
    Waiting_threads_list::remove (DoubleListNodeThread& node)
    {
      if (count_ > 1)
        {
          node.prev->next = node.next;
          node.next->prev = node.prev;
          --count_;
        }
      else if (count_ == 1)
        {
          count_ = 0;
          head_ = nullptr;
        }
      node.prev = nullptr;
      node.next = nullptr;
    }

    void
    Waiting_threads_list::wakeup_one (void)
    {
      if (head_ == nullptr)
        {
          return;
        }

      DoubleListNodeThread* max_prio = head_;

      DoubleListNodeThread* crt;
      for (crt = head_; crt->next != head_;
          crt = (DoubleListNodeThread*) crt->next)
        {
          if (crt->node.sched_prio () > max_prio->node.sched_prio ())
            {
              // Remember the oldest with the highest priority.
              max_prio = crt;
            }
        }

      max_prio->node.wakeup ();
    }

    void
    Waiting_threads_list::wakeup_all (void)
    {
      if (head_ == nullptr)
        {
          return;
        }

      DoubleListNodeThread* crt;
      for (crt = head_; crt->next != head_;
          crt = (DoubleListNodeThread*) crt->next)
        {
          crt->node.wakeup ();
        }
    }

  // ----------------------------------------------------------------------

  } /* namespace rtos */
} /* namespace os */
