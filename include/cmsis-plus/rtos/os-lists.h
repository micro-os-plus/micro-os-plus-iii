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
    class Double_list_node_clock;
    class Double_list_node_timer;

    // ========================================================================

    /**
     * @brief The core of a double linked list, pointers to next and previous.
     */
    class Double_list_links
    {
    public:
      Double_list_links (Double_list& lst);
      ~Double_list_links ();

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

    public:

      Double_list_links* prev;
      Double_list_links* next;
      Double_list& list;
    };

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @brief Template for a double linked list node, with payload.
     */
    template<typename Payload_T>
      class Double_list_node : public Double_list_links
      {
      public:
        using Payload = Payload_T;

        Double_list_node (Double_list& lst, Payload payload);
        ~Double_list_node ();

        /**
         * @cond ignore
         */
        Double_list_node (const Double_list_node&) = delete;
        Double_list_node (Double_list_node&&) = delete;
        Double_list_node&
        operator= (const Double_list_node&) = delete;
        Double_list_node&
        operator= (Double_list_node&&) = delete;
        /**
         * @endcond
         */

      public:

        Payload node;
      };

#pragma GCC diagnostic pop

    // ========================================================================

    struct thread_node_s
    {
      Thread* thread;
    };

    /**
     * @brief A double linked list node, referencing a thread.
     */
    using Double_list_node_thread_ = Double_list_node<thread_node_s>;

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @brief A double linked list node, with thread reference.
     */
    class Double_list_node_thread : public Double_list_links
    {
    public:
      Double_list_node_thread (Double_list& lst, Thread& th);
      ~Double_list_node_thread ();

      /**
       * @cond ignore
       */
      Double_list_node_thread (const Double_list_node_thread&) = delete;
      Double_list_node_thread (Double_list_node_thread&&) = delete;
      Double_list_node_thread&
      operator= (const Double_list_node_thread&) = delete;
      Double_list_node_thread&
      operator= (Double_list_node_thread&&) = delete;
      /**
       * @endcond
       */

    public:

      Thread& thread;
    };

#pragma GCC diagnostic pop
    // ========================================================================

    using clock_node_type_t = enum class clock_node_type
    : uint32_t
      { //
        timeout = 1,
        timer = 2
      };

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    class Double_list_node_timestamp : public Double_list_links
    {
    public:

      Double_list_node_timestamp (Double_list& lst, clock::timestamp_t ts);

      /**
       * @cond ignore
       */
      Double_list_node_timestamp (const Double_list_node_timestamp&) = delete;
      Double_list_node_timestamp (Double_list_node_timestamp&&) = delete;
      Double_list_node_timestamp&
      operator= (const Double_list_node_timestamp&) = delete;
      Double_list_node_timestamp&
      operator= (Double_list_node_timestamp&&) = delete;
      /**
       * @endcond
       */

      virtual
      ~Double_list_node_timestamp ();

      virtual void
      action (void) = 0;

    public:

      clock::timestamp_t timestamp;
    };

#pragma GCC diagnostic pop

    // ========================================================================

    /**
     * @brief Double linked circular list of threads.
     */
    class Double_list
    {
    public:

      /**
       * Create a list.
       */
      Double_list ();

      /**
       * Destroy the list.
       */
      ~Double_list ();

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
       * @brief Clear the list.
       */
      void
      clear (void);

      /**
       * @brief Check if the list is empty.
       * @retval true The list has no nodes.
       * @retval false The list has at least one node.
       */
      bool
      empty (void) const;

      /**
       * @brief Get the number of nodes in the list.
       * @return A non negative integer with the number of nodes.
       */
      std::size_t
      length (void) const;

      // TODO add iterator begin(), end()

    protected:

      /**
       * @brief Remove the node from the list.
       * @param node Reference to the list node to remove from the list.
       */
      void
      remove (Double_list_links& node);

    protected:

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
    };

    // ========================================================================

    /**
     * @brief Double linked circular list of threads.
     */
    class Waiting_threads_list : public Double_list
    {
    public:

      /**
       * Create a list.
       */
      Waiting_threads_list ();

      /**
       * Destroy the list.
       */
      ~Waiting_threads_list ();

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
       * @brief Add a new thread node to the list.
       * @param node Reference to a list node containing the thread reference.
       */
      void
      add (Double_list_node_thread& node);

      void
      remove (Double_list_node_thread& node);

      Double_list_node_thread*
      head (void);

      /**
       * @brief Wake-up one task (the oldest with the highest priority)
       */
      void
      wakeup_one (void);

      /**
       * @brief Wake-up all tasks in the list.
       */
      void
      wakeup_all (void);

      // TODO add iterator begin(), end()

    protected:

    };

    // ========================================================================

    class Clock_timestamps_list : public Double_list
    {
    public:
      /**
       * Create a list.
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
       * Destroy the list.
       */
      ~Clock_timestamps_list ();

      /**
       * @brief Add a new thread node to the list.
       * @param node Reference to a list node containing the thread reference.
       */
      void
      add (Double_list_node_timestamp& node);

      void
      remove (Double_list_node_timestamp& node);

      Double_list_node_timestamp*
      head (void);

      void
      check_wakeup (clock::timestamp_t timestamp);

      /**
       * @brief Wake-up one task (the oldest with the highest priority)
       */
      void
      wakeup_one (void);

      /**
       * @brief Wake-up all tasks in the list.
       */
      void
      wakeup_all (void);

    protected:

    };

    // ========================================================================

    template<typename CS_T, typename List_T, typename Node_T>
      class ThreadListGuard
      {
      public:
        using Critical_section = CS_T;
        using List = List_T;
        using Node = Node_T;

        ThreadListGuard (Node& node);
        ~ThreadListGuard ();

      protected:

        Node& node_;
      };

    template<typename CS_T>
      using Waiting_threads_list_guard =
      ThreadListGuard<CS_T, Waiting_threads_list, Double_list_node_thread>;

    // ========================================================================

    template<typename CS_T, typename List_T, typename Node_T>
      class ClockListGuard
      {
      public:
        using Critical_section = CS_T;
        using List = List_T;
        using Node = Node_T;

        ClockListGuard (Node& node);
        ~ClockListGuard ();

      protected:

        Node& node_;
      };

    template<typename CS_T>
      using Clock_timestamps_list_guard =
      ClockListGuard<CS_T, Clock_timestamps_list, Double_list_node_clock>;

  // --------------------------------------------------------------------------

  }
/* namespace rtos */
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
    Double_list_node_thread::Double_list_node_thread (Double_list& lst,
                                                      Thread& th) :
        Double_list_links
          { lst }, //
        thread (th)
    {
      ;
    }

    inline
    Double_list_node_thread::~Double_list_node_thread ()
    {
      ;
    }

    // ========================================================================

    template<typename Payload_T>
      inline
      Double_list_node<Payload_T>::Double_list_node (Double_list& lst,
                                                     Payload payload) :
          Double_list_links (lst)
      {
        this->node = payload;
      }

    template<typename Payload_T>
      inline
      Double_list_node<Payload_T>::~Double_list_node ()
      {
        ;
      }

// ========================================================================

    inline bool
    Double_list::empty (void) const
    {
      return count_ == 0;
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

    inline Double_list_node_thread*
    Waiting_threads_list::head (void)
    {
      return (Double_list_node_thread*) head_;
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

    inline Double_list_node_timestamp*
    Clock_timestamps_list::head (void)
    {
      return (Double_list_node_timestamp*) head_;
    }

    // ========================================================================

    template<typename CS_T, typename List_T, typename Node_T>
      inline
      ThreadListGuard<CS_T, List_T, Node_T>::ThreadListGuard (Node& node) :
          node_ (node)
      {
        Critical_section cs;

        ((List&) node.list).add (node);
        node.thread.waiting_node_ = &node;
      }

    template<typename CS_T, typename List_T, typename Node_T>
      inline
      ThreadListGuard<CS_T, List_T, Node_T>::~ThreadListGuard ()
      {
        Critical_section cs;

        node_.thread.waiting_node_ = nullptr;
        ((List&) node_.list).remove (node_);
      }

    // ========================================================================

    template<typename CS_T, typename List_T, typename Node_T>
      inline
      ClockListGuard<CS_T, List_T, Node_T>::ClockListGuard (Node& node) :
          node_ (node)
      {
        Critical_section cs;

        ((List&) node.list).add (node);
        node.thread.clock_node_ = &node;
      }

    template<typename CS_T, typename List_T, typename Node_T>
      inline
      ClockListGuard<CS_T, List_T, Node_T>::~ClockListGuard ()
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
