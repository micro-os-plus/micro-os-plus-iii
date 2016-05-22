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

/*
 * This file is part of the CMSIS++ proposal, intended as a CMSIS
 * replacement for C++ applications.
 */

#ifndef CMSIS_PLUS_RTOS_OS_LISTS_H_
#define CMSIS_PLUS_RTOS_OS_LISTS_H_

// ----------------------------------------------------------------------------

#ifdef  __cplusplus

#include <cstdint>
#include <cstddef>
#include <cassert>

namespace os
{
  namespace rtos
  {
    class thread;
    class double_list;
    class timeout_thread_node;
    class timer_node;

    // ========================================================================

    /**
     * @brief Statically allocated core of a double linked list,
     * pointers to next, previous.
     */
    class static_double_list_links
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a list node (BSS initialised).
       */
      static_double_list_links ();

      /**
       * @cond ignore
       */
      static_double_list_links (const static_double_list_links&) = delete;
      static_double_list_links (static_double_list_links&&) = delete;
      static_double_list_links&
      operator= (const static_double_list_links&) = delete;
      static_double_list_links&
      operator= (static_double_list_links&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the node.
       */
      ~static_double_list_links ();

      /**
       * @}
       */

      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Remove the node from the list.
       * @return Nothing.
       */
      void
      unlink (void);

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Variables
       * @{
       */

      /**
       * @brief Pointer to previous node.
       */
      volatile static_double_list_links* volatile prev;

      /**
       * @brief Pointer to next node.
       */
      volatile static_double_list_links* volatile next;

      /**
       * @}
       */

    };

    // ========================================================================

    /**
     * @brief The core of a double linked list, pointers to next,
     * previous.
     */
    class double_list_links : public static_double_list_links
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a list node (explicitly set to nullptr).
       */
      double_list_links ();

      /**
       * @cond ignore
       */
      double_list_links (const double_list_links&) = delete;
      double_list_links (double_list_links&&) = delete;
      double_list_links&
      operator= (const double_list_links&) = delete;
      double_list_links&
      operator= (double_list_links&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the node.
       */
      ~double_list_links ();

      /**
       * @}
       */

    };

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @brief Double linked list node, with thread reference.
     */
    class waiting_thread_node : public double_list_links
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a node with references to the thread.
       * @param th Reference to the thread.
       */
      waiting_thread_node (thread& th);

      /**
       * @cond ignore
       */
      waiting_thread_node (const waiting_thread_node&) = delete;
      waiting_thread_node (waiting_thread_node&&) = delete;
      waiting_thread_node&
      operator= (const waiting_thread_node&) = delete;
      waiting_thread_node&
      operator= (waiting_thread_node&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the node.
       */
      ~waiting_thread_node ();

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Variables
       * @{
       */

      /**
       * @brief Reference to waiting thread.
       */
      rtos::thread& thread;

      /**
       * @}
       */
    };

#pragma GCC diagnostic pop

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @brief Double linked list node, with time stamp.
     */
    class timestamp_node : public double_list_links
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a node with a time stamp.
       * @param ts Time stamp.
       */
      timestamp_node (port::clock::timestamp_t ts);

      /**
       * @cond ignore
       */
      timestamp_node (const timestamp_node&) = delete;
      timestamp_node (timestamp_node&&) = delete;
      timestamp_node&
      operator= (const timestamp_node&) = delete;
      timestamp_node&
      operator= (timestamp_node&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the node.
       */
      virtual
      ~timestamp_node ();

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Action to perform when the time stamp is reached.
       * @par Parameters
       *  None.
       * @return Nothing.
       */
      virtual void
      action (void) = 0;

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Variables
       * @{
       */

      /**
       * @brief Time stamp when the next action will be performed.
       */
      port::clock::timestamp_t timestamp;

      /**
       * @}
       */

    };

#pragma GCC diagnostic pop

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @brief Double linked list node, with time stamp and thread.
     */
    class timeout_thread_node : public timestamp_node
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a clock timeout node.
       * @param [in] ts Time stamp.
       * @param [in] th Reference to thread.
       */
      timeout_thread_node (port::clock::timestamp_t ts, thread& th);

      /**
       * @cond ignore
       */
      timeout_thread_node (const timeout_thread_node&) = delete;
      timeout_thread_node (timeout_thread_node&&) = delete;
      timeout_thread_node&
      operator= (const timeout_thread_node&) = delete;
      timeout_thread_node&
      operator= (timeout_thread_node&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the node.
       */
      virtual
      ~timeout_thread_node ();

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Action to perform when the time stamp is reached.
       * @par Parameters
       *  None.
       * @return Nothing.
       */
      virtual void
      action (void) override;

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Variables
       * @{
       */

      /**
       * @brief Reference to thread who initiated the timeout.
       */
      rtos::thread& thread;

      /**
       * @}
       */
    };

#pragma GCC diagnostic pop

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @brief Double linked list node, with time stamp and timer.
     */
    class timer_node : public timestamp_node
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a clock timer node.
       * @param [in] ts Time stamp.
       * @param [in] tm Reference to timer.
       */
      timer_node (port::clock::timestamp_t ts, timer& tm);

      /**
       * @cond ignore
       */
      timer_node (const timer_node&) = delete;
      timer_node (timer_node&&) = delete;
      timer_node&
      operator= (const timer_node&) = delete;
      timer_node&
      operator= (timer_node&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the node.
       */
      virtual
      ~timer_node ();

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Action to perform when the time stamp is reached.
       * @par Parameters
       *  None.
       * @return Nothing.
       */
      virtual void
      action (void) override;

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Variables
       * @{
       */

      /**
       * @brief Reference to waiting timer.
       */
      timer& tmr;

      /**
       * @}
       */
    };

#pragma GCC diagnostic pop

    // ========================================================================

    /**
     * @brief Statically allocated circular double linked list of nodes.
     */
    class static_double_list
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a list.
       */
      static_double_list ();

      /**
       * @cond ignore
       */
      static_double_list (const static_double_list&) = delete;
      static_double_list (static_double_list&&) = delete;
      static_double_list&
      operator= (const static_double_list&) = delete;
      static_double_list&
      operator= (static_double_list&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the list.
       */
      ~static_double_list ();

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Clear the list.
       * @par Parameters
       *  None.
       * @return Nothing.
       */
      void
      clear (void);

      /**
       * @brief Check if the list is empty.
       * @par Parameters
       *  None.
       * @retval true The list has no nodes.
       * @retval false The list has at least one node.
       */
      bool
      empty (void) const;

      // TODO add iterator begin(), end()

      /**
       * @brief Get the list head.
       * @par Parameters
       *  None.
       * @return Pointer to head node.
       */
      volatile static_double_list_links*
      head (void) const;

      /**
       * @brief Get the list tail.
       * @par Parameters
       *  None.
       * @return Pointer to tail node.
       */
      volatile static_double_list_links*
      tail (void) const;

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Functions
       * @{
       */

      /**
       * @brief Insert a new node after existing node.
       * @param node Reference to node to insert.
       * @param after Reference to existing node.
       * @return Nothing.
       */
      void
      insert_after (static_double_list_links& node,
                    static_double_list_links* after);

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Variables
       * @{
       */

      /**
       * @brief A list node used to point to head and tail.
       * @details
       * To simplify processing, the list always has a node.
       */
      static_double_list_links volatile head_;

      /**
       * @}
       */
    };

    // ========================================================================

    /**
     * @brief Circular double linked list of nodes.
     */
    class double_list : public static_double_list
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a list.
       */
      double_list ();

      /**
       * @cond ignore
       */
      double_list (const double_list&) = delete;
      double_list (double_list&&) = delete;
      double_list&
      operator= (const double_list&) = delete;
      double_list&
      operator= (double_list&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the list.
       */
      ~double_list ();

      /**
       * @}
       */

    };

    // ========================================================================

    /**
     * @brief List of top level threads.
     */
    class top_threads_list : public static_double_list
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a list of waiting threads.
       */
      top_threads_list ();

      /**
       * @cond ignore
       */
      top_threads_list (const top_threads_list&) = delete;
      top_threads_list (top_threads_list&&) = delete;
      top_threads_list&
      operator= (const top_threads_list&) = delete;
      top_threads_list&
      operator= (top_threads_list&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the list.
       */
      ~top_threads_list ();

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Add a new thread node to the list.
       * @param [in] thread Reference to a list node.
       * @return Nothing.
       */
      void
      link (thread& thread);

      // TODO add iterator begin(), end()

      /**
       * @}
       */
    };

    // ========================================================================

    /**
     * @brief List of children threads.
     */
    class thread_children_list : public double_list
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a list of waiting threads.
       */
      thread_children_list ();

      /**
       * @cond ignore
       */
      thread_children_list (const thread_children_list&) = delete;
      thread_children_list (thread_children_list&&) = delete;
      thread_children_list&
      operator= (const thread_children_list&) = delete;
      thread_children_list&
      operator= (thread_children_list&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the list.
       */
      ~thread_children_list ();

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Add a new thread node to the list.
       * @param [in] thread Reference to a list node.
       * @return Nothing.
       */
      void
      link (thread& thread);

      // TODO add iterator begin(), end()

      /**
       * @}
       */
    };

    // ========================================================================

    /**
     * @brief Priority ordered list of threads waiting too run.
     */
    class ready_threads_list : public static_double_list
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a list of waiting threads.
       */
      ready_threads_list ();

      /**
       * @cond ignore
       */
      ready_threads_list (const ready_threads_list&) = delete;
      ready_threads_list (ready_threads_list&&) = delete;
      ready_threads_list&
      operator= (const ready_threads_list&) = delete;
      ready_threads_list&
      operator= (ready_threads_list&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the list.
       */
      ~ready_threads_list ();

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Add a new thread node to the list.
       * @param [in] node Reference to a list node.
       * @return Nothing.
       */
      void
      link (waiting_thread_node& node);

      /**
       * @brief Get list head.
       * @par Parameters
       *  None.
       * @return Casted pointer to head node.
       */
      volatile waiting_thread_node*
      head (void) const;

      /**
       * @brief Remove the top node from the list.
       * @par Parameters
       *  None.
       * @return Pointer to thread.
       */
      thread*
      unlink_head (void);

      // TODO add iterator begin(), end()

      /**
       * @}
       */
    };

    // ========================================================================

    /**
     * @brief Priority ordered list of threads.
     */
    class waiting_threads_list : public double_list
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a list of waiting threads.
       */
      waiting_threads_list ();

      /**
       * @cond ignore
       */
      waiting_threads_list (const waiting_threads_list&) = delete;
      waiting_threads_list (waiting_threads_list&&) = delete;
      waiting_threads_list&
      operator= (const waiting_threads_list&) = delete;
      waiting_threads_list&
      operator= (waiting_threads_list&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the list.
       */
      ~waiting_threads_list ();

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Add a new thread node to the list.
       * @param [in] node Reference to a list node.
       * @return Nothing.
       */
      void
      link (waiting_thread_node& node);

      /**
       * @brief Get list head.
       * @par Parameters
       *  None.
       * @return Casted pointer to head node.
       */
      volatile waiting_thread_node*
      head (void) const;

      /**
       * @brief Wake-up one thread (the oldest with the highest priority)
       * @par Parameters
       *  None.
       * @return Nothing.
       */
      void
      resume_one (void);

      /**
       * @brief Wake-up all threads in the list.
       * @par Parameters
       *  None.
       * @return Nothing.
       */
      void
      resume_all (void);

      // TODO add iterator begin(), end()

      /**
       * @}
       */
    };

    // ========================================================================

    /**
     * @brief Ordered list of time stamp nodes.
     */
    class clock_timestamps_list : public double_list
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a list of clock time stamps.
       */
      clock_timestamps_list ();

      /**
       * @cond ignore
       */
      clock_timestamps_list (const clock_timestamps_list&) = delete;
      clock_timestamps_list (clock_timestamps_list&&) = delete;
      clock_timestamps_list&
      operator= (const clock_timestamps_list&) = delete;
      clock_timestamps_list&
      operator= (clock_timestamps_list&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the list.
       */
      ~clock_timestamps_list ();

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Add a new thread node to the list.
       * @param [in] node Reference to a list node.
       * @return Nothing.
       */
      void
      link (timestamp_node& node);

      /**
       * @brief Get list head.
       * @par Parameters
       *  None.
       * @return Casted pointer to head node.
       */
      volatile timestamp_node*
      head (void) const;

      /**
       * @brief Check list time stamps.
       * @param [in] now The current clock time stamp.
       * @return Nothing.
       */
      void
      check_timestamp (port::clock::timestamp_t now);

      /**
       * @}
       */
    };

    // ========================================================================

    /**
     * @brief Unordered list of threads.
     */
    class terminated_threads_list : public static_double_list
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a list of waiting threads.
       */
      terminated_threads_list ();

      /**
       * @cond ignore
       */
      terminated_threads_list (const terminated_threads_list&) = delete;
      terminated_threads_list (terminated_threads_list&&) = delete;
      terminated_threads_list&
      operator= (const terminated_threads_list&) = delete;
      terminated_threads_list&
      operator= (terminated_threads_list&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the list.
       */
      ~terminated_threads_list ();

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Add a new thread node to the list.
       * @param [in] node Reference to a list node.
       * @return Nothing.
       */
      void
      link (waiting_thread_node& node);

      /**
       * @brief Get list head.
       * @par Parameters
       *  None.
       * @return Casted pointer to head node.
       */
      volatile waiting_thread_node*
      head (void) const;

      // TODO add iterator begin(), end()

      /**
       * @}
       */
    };

    // ------------------------------------------------------------------------

    namespace scheduler
    {
      void
      _link_node (waiting_threads_list& list, waiting_thread_node& node);

      void
      _unlink_node (waiting_thread_node& node);

      void
      _link_node (waiting_threads_list& list, waiting_thread_node& node,
                  clock_timestamps_list& timeout_list,
                  timeout_thread_node& timeout_node);

      void
      _unlink_node (waiting_thread_node& node,
                    timeout_thread_node& timeout_node);

    } /* namespace this_thread */

  // --------------------------------------------------------------------------

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    // ========================================================================

    // Code analysis may trigger:
    // "Member 'prev' was not initialized in constructor"
    // "Member 'next' was not initialized in constructor"

    inline
    static_double_list_links::static_double_list_links ()
    {
      ;
    }

    inline
    static_double_list_links::~static_double_list_links ()
    {
      ;
    }

    // ========================================================================

    inline
    double_list_links::double_list_links ()
    {
      prev = nullptr;
      next = nullptr;
    }

    inline
    double_list_links::~double_list_links ()
    {
      ;
    }

    // ========================================================================

    inline
    waiting_thread_node::waiting_thread_node (rtos::thread& th) :
        thread (th)
    {
      ;
    }

    inline
    waiting_thread_node::~waiting_thread_node ()
    {
      ;
    }

    // ========================================================================

    /**
     * @details
     * The initial list status is empty by having the pointers null.
     */
    inline
    static_double_list::static_double_list ()
    {
      // By all means, do not add any code here.
      // The contructor was not `default` to benefit from inline.
    }

    /**
     * @details
     * There must be no nodes in the list.
     */
    inline
    static_double_list::~static_double_list ()
    {
      ;
    }

    inline bool
    static_double_list::empty (void) const
    {
      // If it point to itself, it is empty.
      return (head_.next == &head_) || (head_.next == nullptr);
    }

    inline volatile static_double_list_links*
    static_double_list::head (void) const
    {
      return static_cast<volatile static_double_list_links*> (head_.next);
    }

    inline volatile static_double_list_links*
    static_double_list::tail (void) const
    {
      return static_cast<volatile static_double_list_links*> (head_.prev);
    }

#if 0
    inline void
    static_double_list::insert_after (static_double_list_links& node,
        static_double_list_links* after)
      {
        // Make the new node point to its neighbours.
        node.prev = after;
        node.next = after->next;

        // Make the neighbours point to the node. The order is important.
        after->next->prev = &node;
        after->next = &node;
      }
#endif

    // ========================================================================

    /**
     * @details
     * The initial list status is empty.
     */
    inline
    top_threads_list::top_threads_list ()
    {
      ;
    }

    inline
    top_threads_list::~top_threads_list ()
    {
      ;
    }

    // ========================================================================

    /**
     * @details
     * The initial list status is empty.
     */
    inline
    thread_children_list::thread_children_list ()
    {
      ;
    }

    inline
    thread_children_list::~thread_children_list ()
    {
      ;
    }

    // ========================================================================

    /**
     * @details
     * The initial list status is empty.
     */
    inline
    ready_threads_list::ready_threads_list ()
    {
      ;
    }

    inline
    ready_threads_list::~ready_threads_list ()
    {
      ;
    }

    inline volatile waiting_thread_node*
    ready_threads_list::head (void) const
    {
      return static_cast<volatile waiting_thread_node*> (static_double_list::head ());
    }

    // ========================================================================

    /**
     * @details
     * The initial list status is empty.
     */
    inline
    waiting_threads_list::waiting_threads_list ()
    {
      ;
    }

    inline
    waiting_threads_list::~waiting_threads_list ()
    {
      ;
    }

    inline volatile waiting_thread_node*
    waiting_threads_list::head (void) const
    {
      return static_cast<volatile waiting_thread_node*> (double_list::head ());
    }

    // ========================================================================

    inline
    clock_timestamps_list::clock_timestamps_list ()
    {
      ;
    }

    inline
    clock_timestamps_list::~clock_timestamps_list ()
    {
      ;
    }

    inline volatile timestamp_node*
    clock_timestamps_list::head (void) const
    {
      return static_cast<volatile timestamp_node*> (double_list::head ());
    }

    // ========================================================================

    /**
     * @details
     * The initial list status is empty.
     */
    inline
    terminated_threads_list::terminated_threads_list ()
    {
      ;
    }

    inline
    terminated_threads_list::~terminated_threads_list ()
    {
      ;
    }

    inline volatile waiting_thread_node*
    terminated_threads_list::head (void) const
    {
      return static_cast<volatile waiting_thread_node*> (static_double_list::head ());
    }

  // --------------------------------------------------------------------------

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_LISTS_H_ */
