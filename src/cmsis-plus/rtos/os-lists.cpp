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

    /**
     * @class Waiting_threads_list
     * @details
     * There are at least two strategies:
     * - keep the list ordered by priorities and have the top node
     *  easily accessible the head
     * - preserve the insertion order and perform a full list traversal
     *  to determine the top node.
     *
     *  The first strategy requires a partial list traverse with each
     * insert, to find the place to insert the node, but makes
     * retrieving the top priority node trivial, by a single access
     * to the list head.
     *
     *  The second strategy might minimise the overall processing
     * time, but always requires a full list traversal to determine
     * the top priority node.
     *
     * On the other hand, typical waiting lists contain only one
     * element, and in this case there is no distinction. Mutex
     * objects occasionally might have two entries (and rarely more).
     * Condition variables might also have several waiting threads,
     * the number is usually small. In these cases, the distinction
     * between the two strategies is also minimal.
     *
     * In the rare cases when the waiting list is large, the first
     * strategy favours top node retrieval, possibly
     * improving the response time, and is thus preferred.
     *
     */

    /**
     * @details
     * The initial list status is empty.
     */
    Waiting_threads_list::Waiting_threads_list ()
    {
      clear ();
    }

    /**
     * @details
     * There must be no nodes in the list.
     */
    Waiting_threads_list::~Waiting_threads_list ()
    {
      assert(head_ == nullptr);
      assert(count_ == 0);
    }

    /**
     * @details
     * Reset the count and clear the head pointer.
     */
    void
    Waiting_threads_list::clear (void)
    {
      head_ = nullptr;
      count_ = 0;
    }

    /**
     * @details
     * Based on priority, the node is inserted at the end of the list,
     * at the beginning of the list, or in the middle of the list, which
     * requires a partial list traversal (done from the end).
     *
     * If the list is empty, the new node is added with references to
     * itself, to satisfy the circular double link list requirements.
     */
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
          thread::priority_t prio = node.node.sched_prio ();

          DoubleListLinks* after = (head_->prev);

          if (prio
              <= ((DoubleListNodeThread*) (head_->next))->node.sched_prio ())
            {
              // Insert at the end of the list.
            }
          else if (prio > ((DoubleListNodeThread*) (head_))->node.sched_prio ())
            {
              // Insert at the beginning of the list
              // and update the new head.
              head_ = &node;
            }
          else
            {
              // Insert in the middle of the list.
              // The loop is guaranteed to terminate and the
              // weight is small, sched_prio() is only an accessor.
              while (prio
                  > ((DoubleListNodeThread*) (after))->node.sched_prio ())
                {
                  after = after->prev;
                }
            }

          // Make the new node point to its neighbours.
          node.prev = after;
          node.next = after->next;

          // Make the neighbours point to the node. The order is important.
          after->next->prev = &node;
          after->next = &node;

          ++count_;
        }
    }

    /**
     * If the list has more than one node, update the neighbours to
     * point to each other, skipping the node.
     *
     * For lists with only one node, simply clear the list.
     *
     * For more robustness, to prevent unexpected accesses,
     * the links in the removed node are nullified.
     */
    void
    Waiting_threads_list::remove (DoubleListNodeThread& node)
    {
      // Check if not already removed.
      if (node.next == nullptr)
        {
          return;
        }

      if (count_ > 1)
        {
          if (head_ == &node)
            {
              // Move head to next node.
              head_ = (DoubleListNodeThread*) node.next;
            }

          // Make neighbours point to each other.
          node.prev->next = node.next;
          node.next->prev = node.prev;

          --count_;
        }
      else if (count_ == 1)
        {
          clear ();
        }

      // Nullify both pointers in the removed node.
      node.prev = nullptr;
      node.next = nullptr;
    }

    void
    Waiting_threads_list::wakeup_one (void)
    {
      interrupts::Critical_section cs; // ----- Critical section -----

      // If the list is empty, silently return.
      if (head_ == nullptr)
        {
          return;
        }

      head_->node.wakeup ();
      remove (*head_);
    }

    void
    Waiting_threads_list::wakeup_all (void)
    {
      while (head_ != nullptr)
        {
          wakeup_one ();
        }
    }

  // ----------------------------------------------------------------------

  } /* namespace rtos */
} /* namespace os */
