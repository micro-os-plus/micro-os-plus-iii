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
    class Thread;
    class Double_list;
    class Timeout_thread_node;
    class timer_node;

    // ========================================================================

    /**
     * @brief Statically allocated core of a double linked list,
     * pointers to next, previous.
     */
    class Static_double_list_links
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a list node (BSS initialised).
       */
      Static_double_list_links ();

      /**
       * @cond ignore
       */
      Static_double_list_links (const Static_double_list_links&) = delete;
      Static_double_list_links (Static_double_list_links&&) = delete;
      Static_double_list_links&
      operator= (const Static_double_list_links&) = delete;
      Static_double_list_links&
      operator= (Static_double_list_links&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the node.
       */
      ~Static_double_list_links ();

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
      volatile Static_double_list_links* volatile prev;

      /**
       * @brief Pointer to next node.
       */
      volatile Static_double_list_links* volatile next;

      /**
       * @}
       */

    };

    // ========================================================================

    /**
     * @brief The core of a double linked list, pointers to next,
     * previous.
     */
    class Double_list_links : public Static_double_list_links
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a list node (explicitly set to nullptr).
       */
      Double_list_links ();

      /**
       * @cond ignore
       */
      Double_list_links (const Double_list_links&) = delete;
      Double_list_links (Double_list_links&&) = delete;
      Double_list_links&
      operator= (const Double_list_links&) = delete;
      Double_list_links&
      operator= (Double_list_links&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the node.
       */
      ~Double_list_links ();

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
    class Waiting_thread_node : public Double_list_links
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
      Waiting_thread_node (Thread& th);

      /**
       * @cond ignore
       */
      Waiting_thread_node (const Waiting_thread_node&) = delete;
      Waiting_thread_node (Waiting_thread_node&&) = delete;
      Waiting_thread_node&
      operator= (const Waiting_thread_node&) = delete;
      Waiting_thread_node&
      operator= (Waiting_thread_node&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the node.
       */
      ~Waiting_thread_node ();

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
      Thread& thread;

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
    class Timestamp_node : public Double_list_links
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
      Timestamp_node (clock::timestamp_t ts);

      /**
       * @cond ignore
       */
      Timestamp_node (const Timestamp_node&) = delete;
      Timestamp_node (Timestamp_node&&) = delete;
      Timestamp_node&
      operator= (const Timestamp_node&) = delete;
      Timestamp_node&
      operator= (Timestamp_node&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the node.
       */
      virtual
      ~Timestamp_node ();

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
      clock::timestamp_t timestamp;

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
    class Timeout_thread_node : public Timestamp_node
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
      Timeout_thread_node (clock::timestamp_t ts, Thread& th);

      /**
       * @cond ignore
       */
      Timeout_thread_node (const Timeout_thread_node&) = delete;
      Timeout_thread_node (Timeout_thread_node&&) = delete;
      Timeout_thread_node&
      operator= (const Timeout_thread_node&) = delete;
      Timeout_thread_node&
      operator= (Timeout_thread_node&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the node.
       */
      virtual
      ~Timeout_thread_node ();

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
      Thread& thread;

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
    class timer_node : public Timestamp_node
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
      timer_node (clock::timestamp_t ts, timer& tm);

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
    class Static_double_list
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a list.
       */
      Static_double_list ();

      /**
       * @cond ignore
       */
      Static_double_list (const Static_double_list&) = delete;
      Static_double_list (Static_double_list&&) = delete;
      Static_double_list&
      operator= (const Static_double_list&) = delete;
      Static_double_list&
      operator= (Static_double_list&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the list.
       */
      ~Static_double_list ();

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
      volatile Static_double_list_links*
      head (void) const;

      /**
       * @brief Get the list tail.
       * @par Parameters
       *  None.
       * @return Pointer to tail node.
       */
      volatile Static_double_list_links*
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
      insert_after (Static_double_list_links& node,
                    Static_double_list_links* after);

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
      Static_double_list_links volatile head_;

      /**
       * @}
       */
    };

    // ========================================================================

    /**
     * @brief Circular double linked list of nodes.
     */
    class Double_list : public Static_double_list
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a list.
       */
      Double_list ();

      /**
       * @cond ignore
       */
      Double_list (const Double_list&) = delete;
      Double_list (Double_list&&) = delete;
      Double_list&
      operator= (const Double_list&) = delete;
      Double_list&
      operator= (Double_list&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the list.
       */
      ~Double_list ();

      /**
       * @}
       */

    };

    // ========================================================================

    /**
     * @brief List of top level threads.
     */
    class Top_threads_list : public Static_double_list
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a list of waiting threads.
       */
      Top_threads_list ();

      /**
       * @cond ignore
       */
      Top_threads_list (const Top_threads_list&) = delete;
      Top_threads_list (Top_threads_list&&) = delete;
      Top_threads_list&
      operator= (const Top_threads_list&) = delete;
      Top_threads_list&
      operator= (Top_threads_list&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the list.
       */
      ~Top_threads_list ();

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
      link (Thread& thread);

      // TODO add iterator begin(), end()

      /**
       * @}
       */
    };

    // ========================================================================

    /**
     * @brief List of children threads.
     */
    class Thread_children_list : public Double_list
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a list of waiting threads.
       */
      Thread_children_list ();

      /**
       * @cond ignore
       */
      Thread_children_list (const Thread_children_list&) = delete;
      Thread_children_list (Thread_children_list&&) = delete;
      Thread_children_list&
      operator= (const Thread_children_list&) = delete;
      Thread_children_list&
      operator= (Thread_children_list&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the list.
       */
      ~Thread_children_list ();

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
      link (Thread& thread);

      // TODO add iterator begin(), end()

      /**
       * @}
       */
    };

    // ========================================================================

    /**
     * @brief Priority ordered list of threads waiting too run.
     */
    class Ready_threads_list : public Static_double_list
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a list of waiting threads.
       */
      Ready_threads_list ();

      /**
       * @cond ignore
       */
      Ready_threads_list (const Ready_threads_list&) = delete;
      Ready_threads_list (Ready_threads_list&&) = delete;
      Ready_threads_list&
      operator= (const Ready_threads_list&) = delete;
      Ready_threads_list&
      operator= (Ready_threads_list&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the list.
       */
      ~Ready_threads_list ();

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
      link (Waiting_thread_node& node);

      /**
       * @brief Get list head.
       * @par Parameters
       *  None.
       * @return Casted pointer to head node.
       */
      volatile Waiting_thread_node*
      head (void) const;

      /**
       * @brief Remove the top node from the list.
       * @par Parameters
       *  None.
       * @return Pointer to thread.
       */
      Thread*
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
    class Waiting_threads_list : public Double_list
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a list of waiting threads.
       */
      Waiting_threads_list ();

      /**
       * @cond ignore
       */
      Waiting_threads_list (const Waiting_threads_list&) = delete;
      Waiting_threads_list (Waiting_threads_list&&) = delete;
      Waiting_threads_list&
      operator= (const Waiting_threads_list&) = delete;
      Waiting_threads_list&
      operator= (Waiting_threads_list&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the list.
       */
      ~Waiting_threads_list ();

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
      link (Waiting_thread_node& node);

      /**
       * @brief Get list head.
       * @par Parameters
       *  None.
       * @return Casted pointer to head node.
       */
      volatile Waiting_thread_node*
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
    class Clock_timestamps_list : public Double_list
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a list of clock time stamps.
       */
      Clock_timestamps_list ();

      /**
       * @cond ignore
       */
      Clock_timestamps_list (const Clock_timestamps_list&) = delete;
      Clock_timestamps_list (Clock_timestamps_list&&) = delete;
      Clock_timestamps_list&
      operator= (const Clock_timestamps_list&) = delete;
      Clock_timestamps_list&
      operator= (Clock_timestamps_list&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the list.
       */
      ~Clock_timestamps_list ();

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
      link (Timestamp_node& node);

      /**
       * @brief Get list head.
       * @par Parameters
       *  None.
       * @return Casted pointer to head node.
       */
      volatile Timestamp_node*
      head (void) const;

      /**
       * @brief Check list time stamps.
       * @param [in] now The current clock time stamp.
       * @return Nothing.
       */
      void
      check_timestamp (clock::timestamp_t now);

      /**
       * @}
       */
    };

    // ========================================================================

    /**
     * @brief Unordered list of threads.
     */
    class Terminated_threads_list : public Static_double_list
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a list of waiting threads.
       */
      Terminated_threads_list ();

      /**
       * @cond ignore
       */
      Terminated_threads_list (const Terminated_threads_list&) = delete;
      Terminated_threads_list (Terminated_threads_list&&) = delete;
      Terminated_threads_list&
      operator= (const Terminated_threads_list&) = delete;
      Terminated_threads_list&
      operator= (Terminated_threads_list&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the list.
       */
      ~Terminated_threads_list ();

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
      link (Waiting_thread_node& node);

      /**
       * @brief Get list head.
       * @par Parameters
       *  None.
       * @return Casted pointer to head node.
       */
      volatile Waiting_thread_node*
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
      _link_node (Waiting_threads_list& list, Waiting_thread_node& node);

      void
      _unlink_node (Waiting_thread_node& node);

      void
      _link_node (Waiting_threads_list& list, Waiting_thread_node& node,
                  Clock_timestamps_list& timeout_list,
                  Timeout_thread_node& timeout_node);

      void
      _unlink_node (Waiting_thread_node& node,
                    Timeout_thread_node& timeout_node);

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
    Static_double_list_links::Static_double_list_links ()
    {
      ;
    }

    inline
    Static_double_list_links::~Static_double_list_links ()
    {
      ;
    }

    // ========================================================================

    inline
    Double_list_links::Double_list_links ()
    {
      prev = nullptr;
      next = nullptr;
    }

    inline
    Double_list_links::~Double_list_links ()
    {
      ;
    }

    // ========================================================================

    inline
    Waiting_thread_node::Waiting_thread_node (Thread& th) :
        thread (th)
    {
      ;
    }

    inline
    Waiting_thread_node::~Waiting_thread_node ()
    {
      ;
    }

    // ========================================================================

    /**
     * @details
     * The initial list status is empty by having the pointers null.
     */
    inline
    Static_double_list::Static_double_list ()
    {
      // By all means, do not add any code here.
      // The contructor was not `default` to benefit from inline.
    }

    /**
     * @details
     * There must be no nodes in the list.
     */
    inline
    Static_double_list::~Static_double_list ()
    {
      ;
    }

    inline bool
    Static_double_list::empty (void) const
    {
      // If it point to itself, it is empty.
      return (head_.next == &head_) || (head_.next == nullptr);
    }

    inline volatile Static_double_list_links*
    Static_double_list::head (void) const
    {
      return static_cast<volatile Static_double_list_links*> (head_.next);
    }

    inline volatile Static_double_list_links*
    Static_double_list::tail (void) const
    {
      return static_cast<volatile Static_double_list_links*> (head_.prev);
    }

#if 0
    inline void
    Static_double_list::insert_after (Static_double_list_links& node,
        Static_double_list_links* after)
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
    Top_threads_list::Top_threads_list ()
    {
      ;
    }

    inline
    Top_threads_list::~Top_threads_list ()
    {
      ;
    }

    // ========================================================================

    /**
     * @details
     * The initial list status is empty.
     */
    inline
    Thread_children_list::Thread_children_list ()
    {
      ;
    }

    inline
    Thread_children_list::~Thread_children_list ()
    {
      ;
    }

    // ========================================================================

    /**
     * @details
     * The initial list status is empty.
     */
    inline
    Ready_threads_list::Ready_threads_list ()
    {
      ;
    }

    inline
    Ready_threads_list::~Ready_threads_list ()
    {
      ;
    }

    inline volatile Waiting_thread_node*
    Ready_threads_list::head (void) const
    {
      return static_cast<volatile Waiting_thread_node*> (Static_double_list::head ());
    }

    // ========================================================================

    /**
     * @details
     * The initial list status is empty.
     */
    inline
    Waiting_threads_list::Waiting_threads_list ()
    {
      ;
    }

    inline
    Waiting_threads_list::~Waiting_threads_list ()
    {
      ;
    }

    inline volatile Waiting_thread_node*
    Waiting_threads_list::head (void) const
    {
      return static_cast<volatile Waiting_thread_node*> (Double_list::head ());
    }

    // ========================================================================

    inline
    Clock_timestamps_list::Clock_timestamps_list ()
    {
      ;
    }

    inline
    Clock_timestamps_list::~Clock_timestamps_list ()
    {
      ;
    }

    inline volatile Timestamp_node*
    Clock_timestamps_list::head (void) const
    {
      return static_cast<volatile Timestamp_node*> (Double_list::head ());
    }

    // ========================================================================

    /**
     * @details
     * The initial list status is empty.
     */
    inline
    Terminated_threads_list::Terminated_threads_list ()
    {
      ;
    }

    inline
    Terminated_threads_list::~Terminated_threads_list ()
    {
      ;
    }

    inline volatile Waiting_thread_node*
    Terminated_threads_list::head (void) const
    {
      return static_cast<volatile Waiting_thread_node*> (Static_double_list::head ());
    }

  // --------------------------------------------------------------------------

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_LISTS_H_ */
