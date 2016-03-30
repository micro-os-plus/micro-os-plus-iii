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
    class Timer_node;

    // ========================================================================

    /**
     * @brief The core of a double linked list, pointers to next,
     * previous and list.
     */
    class Double_list_links
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create the node with a reference to the list.
       * @param lst Reference to the list.
       */
      Double_list_links (Double_list& lst);

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

    public:

      /**
       * @name Public Member Variables
       * @{
       */

      /**
       * @brief Pointer to previous node.
       */
      Double_list_links* prev;

      /**
       * @brief Pointer to next node.
       */
      Double_list_links* next;

      /**
       * @brief Reference to list linking this node.
       */
      Double_list& list;

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
       * @brief Create a node with references to the list and thread.
       * @param lst Reference to the list.
       * @param th Reference to the thread.
       */
      Waiting_thread_node (Double_list& lst, Thread& th);

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
       * @brief Create a node with a reference to the list and a time stamp.
       * @param lst Reference to the list.
       * @param ts Time stamp.
       */
      Timestamp_node (Double_list& lst, clock::timestamp_t ts);

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
       * @param [in] lst Reference to the list.
       * @param [in] ts Time stamp.
       * @param [in] th Reference to thread.
       */
      Timeout_thread_node (Double_list& lst, clock::timestamp_t ts, Thread& th);

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
    class Timer_node : public Timestamp_node
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a clock timer node.
       * @param [in] lst Reference to the list.
       * @param [in] ts Time stamp.
       * @param [in] tm Reference to timer.
       */
      Timer_node (Double_list& lst, clock::timestamp_t ts, Timer& tm);

      /**
       * @cond ignore
       */
      Timer_node (const Timer_node&) = delete;
      Timer_node (Timer_node&&) = delete;
      Timer_node&
      operator= (const Timer_node&) = delete;
      Timer_node&
      operator= (Timer_node&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the node.
       */
      virtual
      ~Timer_node ();

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
      Timer& timer;

      /**
       * @}
       */
    };

#pragma GCC diagnostic pop

    // ========================================================================

    /**
     * @brief Circular double linked list of nodes.
     */
    class Double_list
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

      /**
       * @brief Get the number of nodes in the list.
       * @par Parameters
       *  None.
       * @return A non negative integer with the number of nodes.
       */
      std::size_t
      length (void) const;

      // TODO add iterator begin(), end()

      /**
       * @brief Remove the node from the list.
       * @param [in] node Reference to the list node to remove from the list.
       * @return Nothing.
       */
      void
      remove (Double_list_links& node);

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Variables
       * @{
       */

      /**
       * @brief Pointer to the list first node.
       * @details
       * For empty lists, this value is 'nullptr'.
       */
      Double_list_links* volatile head_;

      /**
       * @brief Count of nodes in the list.
       * @details
       * A non negative integer, updated with each insertion/removal, to
       * reflect the actual number of nodes in the list.
       */
      std::size_t volatile count_;

      /**
       * @}
       */
    };

    // ========================================================================

    /**
     * @brief Ordered double linked circular list of threads.
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
      add (Waiting_thread_node& node);

      /**
       * @brief Get list head.
       * @par Parameters
       *  None.
       * @return Casted pointer.
       */
      Waiting_thread_node*
      head (void);

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

    protected:

      /**
       * @name Private Member Variables
       * @{
       */

      // None.
      /**
       * @}
       */
    };

    // ========================================================================

    /**
     * @brief Ordered double linked circular list of time stamp nodes.
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
      add (Timestamp_node& node);

      /**
       * @brief Get list head.
       * @par Parameters
       *  None.
       * @return Casted pointer.
       */
      Timestamp_node*
      head (void);

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

    protected:

      /**
       * @name Private Member Variables
       * @{
       */

      // None.
      /**
       * @}
       */
    };

    // ========================================================================

    /**
     * @brief RAII list guard.
     */
    template<typename CS_T, typename List_T, typename Node_T>
      class Thread_list_guard
      {
      public:

        /**
         * @name Public Types
         * @{
         */

        using Critical_section = CS_T;
        using List = List_T;
        using Node = Node_T;

        /**
         * @}
         */

      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief RAII add node to the list.
         * @param node Reference to node.
         */
        Thread_list_guard (Node& node);

        /**
         * @cond ignore
         */
        Thread_list_guard (const Thread_list_guard&) = delete;
        Thread_list_guard (Thread_list_guard&&) = delete;
        Thread_list_guard&
        operator= (const Thread_list_guard&) = delete;
        Thread_list_guard&
        operator= (Thread_list_guard&&) = delete;
        /**
         * @endcond
         */

        /**
         * @brief RAII remove node from the list.
         */
        ~Thread_list_guard ();

        /**
         * @}
         */

      protected:

        /**
         * @name Private Member Variables
         * @{
         */

        /**
         * @brief Reference to node.
         */
        Node& node_;

        /**
         * @}
         */
      };

    template<typename CS_T>
      using Waiting_threads_list_guard =
      Thread_list_guard<CS_T, Waiting_threads_list, Waiting_thread_node>;

    // ========================================================================

    /**
     * @brief RAII list guard.
     */
    template<typename CS_T, typename List_T, typename Node_T>
      class Clock_list_guard
      {
      public:

        /**
         * @name Public Types
         * @{
         */

        using Critical_section = CS_T;
        using List = List_T;
        using Node = Node_T;

        /**
         * @}
         */

      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief RAII add node to the list.
         * @param node Reference to node.
         */
        Clock_list_guard (Node& node);

        /**
         * @cond ignore
         */
        Clock_list_guard (const Clock_list_guard&) = delete;
        Clock_list_guard (Clock_list_guard&&) = delete;
        Clock_list_guard&
        operator= (const Clock_list_guard&) = delete;
        Clock_list_guard&
        operator= (Clock_list_guard&&) = delete;
        /**
         * @endcond
         */

        /**
         * @brief RAII remove node from the list.
         */
        ~Clock_list_guard ();

        /**
         * @}
         */

      protected:

        /**
         * @brief Reference to node.
         */
        Node& node_;
      };

    template<typename CS_T>
      using Clock_timestamps_list_guard =
      Clock_list_guard<CS_T, Clock_timestamps_list, Timeout_thread_node>;

  // --------------------------------------------------------------------------

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    // ========================================================================

    inline
    Double_list_links::Double_list_links (Double_list& lst) :
        list (lst)
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
    Waiting_thread_node::Waiting_thread_node (Double_list& lst, Thread& th) :
        Double_list_links
          { lst }, //
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

    inline bool
    Double_list::empty (void) const
    {
      return (head_ == nullptr);
    }

    inline std::size_t
    Double_list::length (void) const
    {
      return count_;
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

    /**
     * @details
     * There must be no nodes in the list.
     */
    inline
    Waiting_threads_list::~Waiting_threads_list ()
    {
      ;
    }

    inline Waiting_thread_node*
    Waiting_threads_list::head (void)
    {
      return (Waiting_thread_node*) head_;
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

    inline Timestamp_node*
    Clock_timestamps_list::head (void)
    {
      return (Timestamp_node*) head_;
    }

    // ========================================================================

    template<typename CS_T, typename List_T, typename Node_T>
      inline
      Thread_list_guard<CS_T, List_T, Node_T>::Thread_list_guard (Node& node) :
          node_ (node)
      {
        Critical_section cs;

        ((List&) node.list).add (node);
        node.thread.waiting_node_ = &node;
      }

    template<typename CS_T, typename List_T, typename Node_T>
      inline
      Thread_list_guard<CS_T, List_T, Node_T>::~Thread_list_guard ()
      {
        Critical_section cs;

        node_.thread.waiting_node_ = nullptr;
        ((List&) node_.list).remove (node_);
      }

    // ========================================================================

    template<typename CS_T, typename List_T, typename Node_T>
      inline
      Clock_list_guard<CS_T, List_T, Node_T>::Clock_list_guard (Node& node) :
          node_ (node)
      {
        Critical_section cs;

        ((List&) node.list).add (node);
        node.thread.clock_node_ = &node;
      }

    template<typename CS_T, typename List_T, typename Node_T>
      inline
      Clock_list_guard<CS_T, List_T, Node_T>::~Clock_list_guard ()
      {
        Critical_section cs;

        node_.thread.clock_node_ = nullptr;
        ((List&) node_.list).remove (node_);
      }
  // ----------------------------------------------------------------------

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_LISTS_H_ */
