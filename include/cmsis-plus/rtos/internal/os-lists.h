/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#ifndef CMSIS_PLUS_RTOS_INTERNAL_OS_LISTS_H_
#define CMSIS_PLUS_RTOS_INTERNAL_OS_LISTS_H_

// ----------------------------------------------------------------------------

#ifdef  __cplusplus

// ----------------------------------------------------------------------------

#include <cmsis-plus/utils/lists.h>

#include <cstdint>
#include <cstddef>
#include <cassert>
#include <iterator>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    class thread;

    namespace internal
    {
      // ======================================================================

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wpadded"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpadded"
#endif

      /**
       * @brief Double linked list node, with thread reference.
       */
      class waiting_thread_node : public utils::double_list_links
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a node with references to the thread.
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
         * @brief Destruct the node.
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
         * @brief Pointer to waiting thread.
         */
        rtos::thread* thread_;

        /**
         * @}
         */
      };

#pragma GCC diagnostic pop

      // ======================================================================

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wpadded"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpadded"
#endif

      /**
       * @brief Double linked list node, with time stamp.
       */
      class timestamp_node : public utils::double_list_links
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a node with a time stamp.
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
         * @brief Destruct the node.
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
         * @par Returns
         *  Nothing.
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

      // ======================================================================

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wpadded"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpadded"
#endif

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
         * @brief Construct a clock timeout node.
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
         * @brief Destruct the node.
         */
        virtual
        ~timeout_thread_node () override;

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
         * @par Returns
         *  Nothing.
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

      // ======================================================================

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wpadded"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpadded"
#endif

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
         * @brief Construct a clock timer node.
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
         * @brief Destruct the node.
         */
        virtual
        ~timer_node () override;

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
         * @par Returns
         *  Nothing.
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

      // ======================================================================

      /**
       * @brief List of children threads.
       */
      class thread_children_list : public utils::double_list
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a list of waiting threads.
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
         * @brief Destruct the list.
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
         * @par Returns
         *  Nothing.
         */
        void
        link (thread& thread);

        // TODO add iterator begin(), end()

        /**
         * @}
         */
      };

      // ======================================================================

      /**
       * @brief Priority ordered list of threads waiting too run.
       */
      class ready_threads_list : public utils::static_double_list
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a list of waiting threads.
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
         * @brief Destruct the list.
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
         * @par Returns
         *  Nothing.
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

      // ======================================================================

      /**
       * @brief Priority ordered list of threads.
       */
      class waiting_threads_list : public utils::double_list
      {
      public:

        /**
         * @name Types and constants
         * @{
         */

        /**
         * @brief Iterator over the list threads.
         */
        using iterator = utils::double_list_iterator<thread, waiting_thread_node, &waiting_thread_node::thread_>;

        /**
         * @}
         */

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a list of waiting threads.
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
         * @brief Destruct the list.
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
         * @par Returns
         *  Nothing.
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
         * @retval true The list may have further entries.
         * @retval false The list is empty.
         */
        bool
        resume_one (void);

        /**
         * @brief Wake-up all threads in the list.
         * @par Parameters
         *  None.
         * @par Returns
         *  Nothing.
         */
        void
        resume_all (void);

        /**
         * @brief Iterator begin.
         * @return An iterator positioned at the first element.
         */
        iterator
        begin () const;

        /**
         * @brief Iterator begin.
         * @return An iterator positioned after the last element.
         */
        iterator
        end () const;

        /**
         * @}
         */
      };

      // ======================================================================

      /**
       * @brief Ordered list of time stamp nodes.
       */
      class clock_timestamps_list : public utils::double_list
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a list of clock time stamps.
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
         * @brief Destruct the list.
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
         * @par Returns
         *  Nothing.
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
         * @par Returns
         *  Nothing.
         */
        void
        check_timestamp (port::clock::timestamp_t now);

        /**
         * @}
         */
      };

      // ======================================================================

      /**
       * @brief Unordered list of threads.
       */
      class terminated_threads_list : public utils::static_double_list
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a list of waiting threads.
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
         * @brief Destruct the list.
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
         * @par Returns
         *  Nothing.
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

    } /* namespace internal */

    // ------------------------------------------------------------------------
    namespace scheduler
    {

      /**
       * @cond ignore
       */

      void
      internal_link_node (internal::waiting_threads_list& list,
                          internal::waiting_thread_node& node);

      void
      internal_unlink_node (internal::waiting_thread_node& node);

      void
      internal_link_node (internal::waiting_threads_list& list,
                          internal::waiting_thread_node& node,
                          internal::clock_timestamps_list& timeout_list,
                          internal::timeout_thread_node& timeout_node);

      void
      internal_unlink_node (internal::waiting_thread_node& node,
                            internal::timeout_thread_node& timeout_node);

    /**
     * @endcond
     */

    } /* namespace this_thread */

  // --------------------------------------------------------------------------

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    namespace internal
    {

      // ======================================================================

      inline
      waiting_thread_node::waiting_thread_node (rtos::thread& th) :
          thread_ (&th)
      {
      }

      inline
      waiting_thread_node::~waiting_thread_node ()
      {
      }

      // ======================================================================

      /**
       * @details
       * The initial list status is empty.
       */
      inline
      thread_children_list::thread_children_list ()
      {
      }

      inline
      thread_children_list::~thread_children_list ()
      {
      }

      // ======================================================================

      /**
       * @details
       * The initial list status is empty.
       */
      inline
      ready_threads_list::ready_threads_list ()
      {
      }

      inline
      ready_threads_list::~ready_threads_list ()
      {
      }

      inline volatile waiting_thread_node*
      ready_threads_list::head (void) const
      {
        return static_cast<volatile waiting_thread_node*> (static_double_list::head ());
      }

      // ======================================================================

      /**
       * @details
       * The initial list status is empty.
       */
      inline
      waiting_threads_list::waiting_threads_list ()
      {
      }

      inline
      waiting_threads_list::~waiting_threads_list ()
      {
      }

      inline volatile waiting_thread_node*
      waiting_threads_list::head (void) const
      {
        return static_cast<volatile waiting_thread_node*> (double_list::head ());
      }

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Waggregate-return"
#endif
      inline waiting_threads_list::iterator
      waiting_threads_list::begin () const
      {
        return iterator
          {
              static_cast<waiting_threads_list::iterator::iterator_pointer> (head_.next ()) };
      }

      inline waiting_threads_list::iterator
      waiting_threads_list::end () const
      {
        return iterator
          {
              static_cast<waiting_threads_list::iterator::iterator_pointer> (const_cast<utils::static_double_list_links*> (&head_)) };
      }
#pragma GCC diagnostic pop

      // ======================================================================

      inline
      clock_timestamps_list::clock_timestamps_list ()
      {
      }

      inline
      clock_timestamps_list::~clock_timestamps_list ()
      {
      }

      inline volatile timestamp_node*
      clock_timestamps_list::head (void) const
      {
        return static_cast<volatile timestamp_node*> (double_list::head ());
      }

      // ======================================================================

      /**
       * @details
       * The initial list status is empty.
       */
      inline
      terminated_threads_list::terminated_threads_list ()
      {
      }

      inline
      terminated_threads_list::~terminated_threads_list ()
      {
      }

      inline volatile waiting_thread_node*
      terminated_threads_list::head (void) const
      {
        return static_cast<volatile waiting_thread_node*> (static_double_list::head ());
      }

    // ------------------------------------------------------------------------
    } /* namespace internal */
  } /* namespace rtos */
} /* namespace os */

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_RTOS_INTERNAL_OS_LISTS_H_ */
