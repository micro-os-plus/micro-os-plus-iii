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
// #include <cmsis-plus/rtos/os-lists.h>

// Better be the last, to undef putchar()
#include <cmsis-plus/diag/trace.h>

namespace os
{
  namespace rtos
  {

    // ========================================================================

    /**
     * @details
     * The initial list status is empty.
     */
    Double_list::Double_list ()
    {
      clear ();
    }

    /**
     * @details
     * There must be no nodes in the list.
     */
    Double_list::~Double_list ()
    {
      assert(head_ == nullptr);
      assert(count_ == 0);
    }

    /**
     * @details
     * Reset the count and clear the head pointer.
     */
    void
    Double_list::clear (void)
    {
      head_ = nullptr;
      count_ = 0;
    }

    /**
     * @details
     * If the list has more than one node, update the neighbours to
     * point to each other, skipping the node.
     *
     * For lists with only one node, simply clear the list.
     *
     * For more robustness, to prevent unexpected accesses,
     * the links in the removed node are nullified.
     */
    void
    Double_list::remove (Double_list_links& n)
    {
      // Check if not already removed.
      if (n.next == nullptr)
        {
          return;
        }

      if (count_ > 1)
        {
          if (head_ == &n)
            {
              // Move head to next node.
              head_ = (Double_list_links*) n.next;
            }

          // Make neighbours point to each other.
          n.prev->next = n.next;
          n.next->prev = n.prev;

          --count_;
        }
      else if (count_ == 1)
        {
          clear ();
        }

      // Nullify both pointers in the removed node.
      n.prev = nullptr;
      n.next = nullptr;
    }

    // ========================================================================

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
     */

    /**
     * @details
     * Based on priority, the node is inserted
     * - at the end of the list,
     * - at the beginning of the list,
     * - in the middle of the list, which
     * requires a partial list traversal (done from the end).
     *
     * If the list is empty, the new node is added with references to
     * itself, to satisfy the circular double linked list requirements.
     */
    void
    Waiting_threads_list::add (Double_list_node_thread& n)
    {
      if (head_ == nullptr)
        {
          // Make the n point to itself, to satisfy the double
          // linked circular list.
          n.prev = &n;
          n.next = &n;

          head_ = &n;
          count_ = 1;
        }
      else
        {
          thread::priority_t prio = n.thread.sched_prio ();

          Double_list_node_thread* after =
              (Double_list_node_thread*) (head_->prev);

          if (prio <= after->thread.sched_prio ())
            {
              // Insert at the end of the list.
            }
          else if (prio > head ()->thread.sched_prio ())
            {
              // Insert at the beginning of the list
              // and update the new head.
              head_ = &n;
              trace::printf ("%s() head \n", __func__);
            }
          else
            {
              // Insert in the middle of the list.
              // The loop is guaranteed to terminate and the
              // weight is small, sched_prio() is only an accessor.
              while (prio > after->thread.sched_prio ())
                {
                  after = (Double_list_node_thread*) after->prev;
                }
            }

          // Make the new node point to its neighbours.
          n.prev = after;
          n.next = after->next;

          // Make the neighbours point to the n. The order is important.
          after->next->prev = &n;
          after->next = &n;

          ++count_;
        }

    }

    void
    Waiting_threads_list::remove (Double_list_node_thread& n)
    {
      trace::printf ("%s() waiting \n", __func__);

      Double_list::remove (n);
    }

    /**
     * @details
     * Atomically get the top thread from the list, remove the node
     * and wake-up the thread.
     */
    void
    Waiting_threads_list::wakeup_one (void)
    {
      Thread* thread;
        {
          interrupts::Critical_section cs; // ----- Critical section -----

          // If the list is empty, silently return.
          if (empty ())
            {
              return;
            }

          thread = &(head ()->thread);
          remove (*head ());
        }
      assert(thread != nullptr);

      thread::state_t state = thread->sched_state ();
      if (state != thread::state::destroyed)
        {
          thread->wakeup ();
        }
      else
        {
          trace::printf ("%s() gone \n", __func__);
        }
    }

    void
    Waiting_threads_list::wakeup_all (void)
    {
      while (!empty ())
        {
          wakeup_one ();
        }
    }

    // ========================================================================

    Double_list_node_timestamp::Double_list_node_timestamp (
        Double_list& lst, clock::timestamp_t ts) :
        Double_list_links
          { lst }, //
        timestamp (ts)
    {
      ;
    }

    Double_list_node_timestamp::~Double_list_node_timestamp ()
    {
      ;
    }

    // ========================================================================

    void
    Clock_timestamps_list::add (Double_list_node_timestamp& n)
    {
      if (head_ == nullptr)
        {
          // Make the node point to itself, to satisfy the double
          // linked circular list.
          n.prev = &n;
          n.next = &n;

          trace::printf ("%s() %u \n", __func__, (uint32_t) n.timestamp);

          head_ = &n;
          count_ = 1;
        }
      else
        {
          clock::timestamp_t timestamp = n.timestamp;

          Double_list_node_clock* after =
              (Double_list_node_clock*) (head_->prev);

          if (timestamp >= after->timestamp)
            {
              // Insert at the end of the list.
            }
          else if (timestamp < head ()->timestamp)
            {
              // Insert at the beginning of the list
              // and update the new head.
              head_ = &n;
              trace::printf ("%s() head \n", __func__);
            }
          else
            {
              // Insert in the middle of the list.
              // The loop is guaranteed to terminate.
              while (timestamp < after->timestamp)
                {
                  after = (Double_list_node_clock*) (after->prev);
                }
            }

          ++count_;

          trace::printf ("%s() %u after %u #%d\n", __func__,
                         (uint32_t) timestamp, (uint32_t) after->timestamp,
                         count_);

          if (after->timestamp == 0)
            {
              trace::printf ("zero \n");
            }
          // Make the new node point to its neighbours.
          n.prev = after;
          n.next = after->next;

          // Make the neighbours point to the node. The order is important.
          after->next->prev = &n;
          after->next = &n;
        }
    }

    void
    Clock_timestamps_list::remove (Double_list_node_timestamp& n)
    {
      trace::printf ("%s() timeout \n", __func__);

      Double_list::remove (n);
    }

    void
    Clock_timestamps_list::check_wakeup (clock::timestamp_t now)
    {
      // Multiple threads can wait for the same time stamp, so
      // iterate until a future node is identified.
      for (; !empty ();)
        {
          clock::timestamp_t head_ts =
              ((Double_list_node_clock*) head_)->timestamp;
          if (now >= head_ts)
            {
              trace::printf ("%s() %u \n", __func__,
                             (uint32_t) systick_clock.now ());
              head ()->action ();
            }
          else
            {
              break;
            }
        }
    }

    void
    Clock_timestamps_list::wakeup_one (void)
    {
      Thread* thread;
        {
          interrupts::Critical_section cs; // ----- Critical section -----

          // If the list is empty, silently return.
          if (empty ())
            {
              return;
            }

          thread = &(((Double_list_node_clock*) head ())->thread);
          remove (*((Double_list_node_clock*) head ()));
        }

      assert(thread != nullptr);

      thread::state_t state = thread->sched_state ();
      if (state != thread::state::destroyed)
        {
          thread->wakeup ();
        }
      else
        {
          trace::printf ("%s() gone \n", __func__);
        }

    }

    void
    Clock_timestamps_list::wakeup_all (void)
    {
      while (!empty ())
        {
          wakeup_one ();
        }
    }

  // ----------------------------------------------------------------------

  } /* namespace rtos */
} /* namespace os */
