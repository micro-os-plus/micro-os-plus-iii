/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <cmsis-plus/rtos/os.h>

#include <cmsis-plus/diag/trace.h>

namespace os
{
  namespace rtos
  {
    namespace internal
    {

      // ======================================================================

      /**
       * @class static_double_list_links
       * @details
       * This is the simplest list node, used as base class for other
       * list nodes and as storage for static_double_list,
       * that must be available for any statically constructed
       * objects while still avoiding the 'static initialisation order fiasco'.
       *
       * The idea is to design the object in such a way as to benefit
       * from the standard BSS initialisation, in other words take `nullptr`
       * as starting values.
       */

      /**
       * @details
       * Update the neighbours to
       * point to each other, skipping the node.
       *
       * For more robustness, to prevent unexpected accesses,
       * the links in the removed node are nullified.
       */
      void
      static_double_list_links::unlink (void)
      {
        // Check if not already unlinked.
        if (unlinked ())
          {
            assert(prev_ == nullptr);
#if defined(OS_TRACE_RTOS_LISTS)
            trace::printf ("%s() %p nop\n", __func__, this);
#endif
            return;
          }

#if defined(OS_TRACE_RTOS_LISTS)
        trace::printf ("%s() %p \n", __func__, this);
#endif

        // Make neighbours point to each other.
        prev_->next_ = next_;
        next_->prev_ = prev_;

        // Nullify both pointers in the unlinked node.
        prev_ = nullptr;
        next_ = nullptr;
      }

      // ======================================================================

      /**
       * @class static_double_list
       * @details
       * This is the simplest list, used as base class for scheduler
       * lists that must be available for any statically constructed
       * thread while still avoiding the 'static initialisation order fiasco'.
       *
       * The idea is to design the object in such a way as to benefit
       * from the standard BSS initialisation, in other words take `nullptr`
       * as starting values.
       *
       * This has the downside of requiring additional tests before
       * adding new nodes to the list, to create the initial self
       * links, and when checking if the list is empty.
       */

      /**
       * @details
       * Initialise the mandatory node with links to itself.
       */
      void
      static_double_list::clear (void)
      {
        head_.next (const_cast<static_double_list_links*> (&head_));
        head_.prev (const_cast<static_double_list_links*> (&head_));
      }

      void
      static_double_list::insert_after (static_double_list_links& node,
                                        static_double_list_links* after)
      {
#if defined(OS_TRACE_RTOS_LISTS)
        trace::printf ("%s() n=%p after %p\n", __func__, &node, after);
#endif

        assert(node.prev () == nullptr);
        assert(node.next () == nullptr);
        assert(after->next () != nullptr);

        // Make the new node point to its neighbours.
        node.prev (after);
        node.next (after->next ());

        // Make the neighbours point to the node. The order is important.
        after->next ()->prev (&node);
        after->next (&node);
      }

      // ======================================================================

      /**
       * @details
       * The initial list status is empty.
       */
      double_list::double_list ()
      {
#if defined(OS_TRACE_RTOS_LISTS_CONSTRUCT)
        trace::printf ("%s() %p \n", __func__, this);
#endif

        clear ();
      }

      /**
       * @details
       * There must be no nodes in the list.
       */
      double_list::~double_list ()
      {
#if defined(OS_TRACE_RTOS_LISTS_CONSTRUCT)
        trace::printf ("%s() %p \n", __func__, this);
#endif

        assert(empty ());
      }

      // ======================================================================

      void
      thread_children_list::link (thread& thread)
      {
        // Add thread intrusive node at the end of the list.
        insert_after (thread.child_links_,
                      const_cast<static_double_list_links*> (tail ()));
      }

      // ======================================================================

      void
      ready_threads_list::link (waiting_thread_node& node)
      {
        if (head_.prev () == nullptr)
          {
            // If this is the first time, initialise the list to empty.
            clear ();
          }

        thread::priority_t prio = node.thread_->priority ();

        waiting_thread_node* after =
            static_cast<waiting_thread_node*> (const_cast<static_double_list_links *> (tail ()));

        if (empty ())
          {
            // Insert at the end of the list.
#if defined(OS_TRACE_RTOS_LISTS)
            trace::printf ("ready %s() empty +%u\n", __func__, prio);
#endif
          }
        else if (prio <= after->thread_->priority ())
          {
            // Insert at the end of the list.
#if defined(OS_TRACE_RTOS_LISTS)
            trace::printf ("ready %s() back %u +%u \n", __func__,
                           after->thread_->priority (), prio);
#endif
          }
        else if (prio > head ()->thread_->priority ())
          {
            // Insert at the beginning of the list.
            after =
                static_cast<waiting_thread_node*> (const_cast<static_double_list_links *> (&head_));
#if defined(OS_TRACE_RTOS_LISTS)
            trace::printf ("ready %s() front +%u %u \n", __func__, prio,
                           head ()->thread_->priority ());
#endif
          }
        else
          {
            // Insert in the middle of the list.
            // The loop is guaranteed to terminate, and not hit the head.
            // The weight is relatively small, priority() is not heavy.
            while (prio > after->thread_->priority ())
              {
                after =
                    static_cast<waiting_thread_node*> (const_cast<static_double_list_links *> (after->prev ()));
              }
#if defined(OS_TRACE_RTOS_LISTS)
            trace::printf ("ready %s() middle %u +%u \n", __func__,
                           after->thread_->priority (), prio);
#endif
          }

        insert_after (node, after);

        node.thread_->state_ = thread::state::ready;
      }

      /**
       * @details
       * Must be called in a critical section.
       */
      thread*
      ready_threads_list::unlink_head (void)
      {
        assert(!empty ());

        thread* th = head ()->thread_;

#if defined(OS_TRACE_RTOS_LISTS)
        trace::printf ("ready %s() %p %s\n", __func__, th, th->name ());
#endif

        const_cast<waiting_thread_node*> (head ())->unlink ();

        assert(th != nullptr);

        // Unlinking is immediately followed by a context switch,
        // so in order to guarantee that the thread is marked as
        // running, it is saver to do it here.

        th->state_ = thread::state::running;
        return th;
      }

      // ======================================================================

      /**
       * @class waiting_threads_list
       * @details
       * There are at least two strategies:
       * - keep the list ordered by priorities and have the top node
       *  easily accessible the head
       * - preserve the insertion order and perform a full list traversal
       *  to determine the top node.
       *
       * The first strategy requires a partial list traverse with each
       * insert, to find the place to insert the node, but makes
       * retrieving the top priority node trivial, by a single access
       * to the list head.
       *
       * The second strategy might minimise the overall processing
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
       * To satisfy the circular double linked list requirements,
       * an empty list still contains the head node with references
       * to itself.
       */
      void
      waiting_threads_list::link (waiting_thread_node& node)
      {
        thread::priority_t prio = node.thread_->priority ();

        waiting_thread_node* after =
            static_cast<waiting_thread_node*> (const_cast<static_double_list_links *> (tail ()));

        if (empty ())
          {
            // Insert at the end of the list.
#if defined(OS_TRACE_RTOS_LISTS)
            trace::printf ("wait %s() empty +%u\n", __func__, prio);
#endif
          }
        else if (prio <= after->thread_->priority ())
          {
            // Insert at the end of the list.
#if defined(OS_TRACE_RTOS_LISTS)
            trace::printf ("wait %s() back %u +%u \n", __func__,
                           after->thread_->priority (), prio);
#endif
          }
        else if (prio > head ()->thread_->priority ())
          {
            // Insert at the beginning of the list.
            after =
                static_cast<waiting_thread_node*> (const_cast<static_double_list_links *> (&head_));
#if defined(OS_TRACE_RTOS_LISTS)
            trace::printf ("wait %s() front +%u %u \n", __func__, prio,
                           head ()->thread_->priority ());
#endif
          }
        else
          {
            // Insert in the middle of the list.
            // The loop is guaranteed to terminate, and not hit the head.
            // The weight is relatively small, priority() is not heavy.
            while (prio > after->thread_->priority ())
              {
                after =
                    static_cast<waiting_thread_node*> (const_cast<static_double_list_links *> (after->prev ()));
              }
#if defined(OS_TRACE_RTOS_LISTS)
            trace::printf ("wait %s() middle %u +%u \n", __func__,
                           after->thread_->priority (), prio);
#endif
          }

        insert_after (node, after);
      }

      /**
       * @details
       * Atomically get the top thread from the list, remove the node
       * and wake-up the thread.
       */
      bool
      waiting_threads_list::resume_one (void)
      {
        thread* th;
          {
            // ----- Enter critical section -----------------------------------
            interrupts::critical_section ics;

            // If the list is empty, silently return.
            if (empty ())
              {
                return false;
              }

            // The top priority is to remove the entry from the list
            // so that subsequent wakeups to address different threads.
            th = head ()->thread_;
            const_cast<waiting_thread_node*> (head ())->unlink ();
            // ----- Exit critical section ------------------------------------
          }
        assert(th != nullptr);

        thread::state_t state = th->state ();
        if (state != thread::state::destroyed)
          {
            th->resume ();
          }
        else
          {
#if defined(OS_TRACE_RTOS_LISTS)
            trace::printf ("%s() gone \n", __func__);
#endif
          }

        return true;
      }

      void
      waiting_threads_list::resume_all (void)
      {
        while (resume_one ())
          ;
      }

      // ======================================================================

      timestamp_node::timestamp_node (clock::timestamp_t ts) :
          timestamp (ts)
      {
#if defined(OS_TRACE_RTOS_LISTS_CONSTRUCT)
        trace::printf ("%s() %p \n", __func__, this);
#endif
      }

      timestamp_node::~timestamp_node ()
      {
#if defined(OS_TRACE_RTOS_LISTS_CONSTRUCT)
        trace::printf ("%s() %p \n", __func__, this);
#endif
      }

      // ======================================================================

      timeout_thread_node::timeout_thread_node (clock::timestamp_t ts,
                                                rtos::thread& th) :
          timestamp_node
            { ts }, //
          thread (th)
      {
#if defined(OS_TRACE_RTOS_LISTS_CONSTRUCT)
        trace::printf ("%s() %p \n", __func__, this);
#endif
      }

      timeout_thread_node::~timeout_thread_node ()
      {
#if defined(OS_TRACE_RTOS_LISTS_CONSTRUCT)
        trace::printf ("%s() %p \n", __func__, this);
#endif
      }

      // Must be called in a critical section.
      void
      timeout_thread_node::action (void)
      {
        rtos::thread* th = &this->thread;
        this->unlink ();

        thread::state_t state = th->state ();
        if (state != thread::state::destroyed)
          {
            th->resume ();
          }
      }

      // ======================================================================

#if !defined(OS_USE_RTOS_PORT_TIMER)

      timer_node::timer_node (clock::timestamp_t ts, timer& tm) :
          timestamp_node
            { ts }, //
          tmr (tm)
      {
#if defined(OS_TRACE_RTOS_LISTS_CONSTRUCT)
        trace::printf ("%s() %p \n", __func__, this);
#endif
      }

      timer_node::~timer_node ()
      {
#if defined(OS_TRACE_RTOS_LISTS_CONSTRUCT)
        trace::printf ("%s() %p \n", __func__, this);
#endif
      }

      /**
       * @details
       * Remove the node from the list and perform the timer actions.
       */
      void
      timer_node::action (void)
      {
        this->unlink ();
        tmr.internal_interrupt_service_routine ();
      }

#endif

      // ======================================================================

      /**
       * @details
       * The list is kept in ascending time stamp order.
       *
       * Based on time stamp, the node is inserted
       * - at the end of the list,
       * - at the beginning of the list,
       * - in the middle of the list, which
       * requires a partial list traversal (done from the end).
       *
       * To satisfy the circular double linked list requirements,
       * an empty list still contains the head node with references
       * to itself.
       */
      void
      clock_timestamps_list::link (timestamp_node& node)
      {
        clock::timestamp_t timestamp = node.timestamp;

        timeout_thread_node* after =
            static_cast<timeout_thread_node*> (const_cast<static_double_list_links *> (tail ()));

        if (empty ())
          {
            // Insert at the end of the list.
#if defined(OS_TRACE_RTOS_LISTS_CLOCKS)
            trace::printf ("clock %s() empty +%u\n", __func__,
                static_cast<uint32_t> (timestamp));
#endif
          }
        else if (timestamp >= after->timestamp)
          {
            // Insert at the end of the list.
#if defined(OS_TRACE_RTOS_LISTS_CLOCKS)
            trace::printf ("clock %s() back %u +%u\n", __func__,
                static_cast<uint32_t> (after->timestamp),
                static_cast<uint32_t> (timestamp));
#endif
          }
        else if (timestamp < head ()->timestamp)
          {
            // Insert at the beginning of the list
            // and update the new head.
            after =
                static_cast<timeout_thread_node*> (const_cast<static_double_list_links *> (&head_));
#if defined(OS_TRACE_RTOS_LISTS_CLOCKS)
            trace::printf ("clock %s() front +%u %u\n", __func__,
                static_cast<uint32_t> (timestamp),
                static_cast<uint32_t> (head ()->timestamp));
#endif
          }
        else
          {
            // Insert in the middle of the list.
            // The loop is guaranteed to terminate.
            while (timestamp < after->timestamp)
              {
                after =
                    static_cast<timeout_thread_node*> (const_cast<static_double_list_links *> (after->prev ()));
              }
#if defined(OS_TRACE_RTOS_LISTS_CLOCKS)
            trace::printf ("clock %s() middle %u +%u\n", __func__,
                static_cast<uint32_t> (after->timestamp),
                static_cast<uint32_t> (timestamp));
#endif
          }

        insert_after (node, after);
      }

      /**
       * @details
       * With the list ordered, check if the list head time stamp was
       * reached and run the node action.
       *
       * Repeat for all nodes that have overdue time stamps.
       */
      void
      clock_timestamps_list::check_timestamp (clock::timestamp_t now)
      {
        if (head_.next () == nullptr)
          {
            // This happens before the constructors are executed.
            return;
          }

        // Multiple threads can wait for the same time stamp, so
        // iterate until a node with future time stamp is identified.
        for (;;)
          {
            // ----- Enter critical section -----------------------------------
            interrupts::critical_section ics;

            if (empty ())
              {
                break;
              }
            clock::timestamp_t head_ts = head ()->timestamp;
            if (now >= head_ts)
              {
#if defined(OS_TRACE_RTOS_LISTS_CLOCKS)
                trace::printf ("%s() %u \n", __func__,
                    static_cast<uint32_t> (sysclock.now ()));
#endif
                const_cast<timestamp_node*> (head ())->action ();
              }
            else
              {
                break;
              }
            // ----- Exit critical section ------------------------------------
          }
      }

      // ======================================================================

      void
      terminated_threads_list::link (waiting_thread_node& node)
      {
        if (head_.prev () == nullptr)
          {
            // If this is the first time, initialise the list to empty.
            clear ();
          }

        waiting_thread_node* after =
            static_cast<waiting_thread_node*> (const_cast<static_double_list_links *> (tail ()));

#if defined(OS_TRACE_RTOS_THREAD)
        trace::printf ("terminated %s() %p %s\n", __func__, &node.thread_,
                       node.thread_->name ());
#endif

        node.thread_->state_ = thread::state::terminated;

        insert_after (node, after);
      }

    // ------------------------------------------------------------------------
    } /* namespace internal */
  } /* namespace rtos */
} /* namespace os */
