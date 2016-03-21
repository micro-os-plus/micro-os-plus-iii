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

    // ========================================================================

    /**
     * @brief The core of a double linked list, pointers to next and previous.
     */
    class DoubleListLinks
    {
    public:
      DoubleListLinks ();
      ~DoubleListLinks ();

      /**
       * @cond ignore
       */
      DoubleListLinks (const DoubleListLinks&) = delete;
      DoubleListLinks (DoubleListLinks&&) = delete;
      DoubleListLinks&
      operator= (const DoubleListLinks&) = delete;
      DoubleListLinks&
      operator= (DoubleListLinks&&) = delete;
      /**
       * @endcond
       */

    public:

      DoubleListLinks* prev;
      DoubleListLinks* next;
    };

    /**
     * @brief Template for a double linked list node, with reference to payload.
     */
    template<typename P_T>
      class DoubleListNodeRef : public DoubleListLinks
      {
      public:
        using Payload = P_T;

        DoubleListNodeRef (Payload& node);
        ~DoubleListNodeRef ();

        /**
         * @cond ignore
         */
        DoubleListNodeRef (const DoubleListNodeRef&) = delete;
        DoubleListNodeRef (DoubleListNodeRef&&) = delete;
        DoubleListNodeRef&
        operator= (const DoubleListNodeRef&) = delete;
        DoubleListNodeRef&
        operator= (DoubleListNodeRef&&) = delete;
        /**
         * @endcond
         */

      public:

        Payload& node;
      };

    /**
     * @brief A double linked list node, with reference to threads.
     */
    using DoubleListNodeThread = DoubleListNodeRef<Thread>;

    /**
     * @brief Double linked circular list of threads.
     */
    class Waiting_threads_list
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
      add (DoubleListNodeThread& node);

      /**
       * @brief Remove the node from the list.
       * @param node Reference to the list node to remove from the list.
       */
      void
      remove (DoubleListNodeThread& node);

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
       * @brief Pointer to the list first node.
       * @details
       * For empty lists, this value is 'nullptr'.
       */
      DoubleListNodeThread* head_;

      /**
       * @brief Count of nodes in the list.
       * @details
       * A non negative integer, updated with each insertion/removal, to
       * reflect the actual number of nodes in the list.
       */
      std::size_t count_;
    };

  // --------------------------------------------------------------------------

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    // ------------------------------------------------------------------------

    inline
    DoubleListLinks::DoubleListLinks ()
    {
      prev = nullptr;
      next = nullptr;
    }

    inline
    DoubleListLinks::~DoubleListLinks ()
    {
      ;
    }

    // ----------------------------------------------------------------------

    template<typename P_T>
      DoubleListNodeRef<P_T>::DoubleListNodeRef (Payload& payload) :
          node (payload)
      {
        ;
      }

    template<typename P_T>
      DoubleListNodeRef<P_T>::~DoubleListNodeRef ()
      {
        ;
      }

    // ----------------------------------------------------------------------

    inline bool
    Waiting_threads_list::empty (void) const
    {
      return count_ == 0;
    }

    inline std::size_t
    Waiting_threads_list::length (void) const
    {
      return count_;
    }

  // ----------------------------------------------------------------------

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_LISTS_H_ */
