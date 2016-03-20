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

    using DoubleListNodeThread = DoubleListNodeRef<Thread>;

    // Simple collection of threads, ordered by priorities.
    class Waiting_threads_list
    {
    public:

      Waiting_threads_list ();
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

      void
      add (DoubleListNodeThread& node);

      void
      remove (DoubleListNodeThread& node);

      void
      wakeup_one (void);

      void
      wakeup_all (void);

      void
      clear (void);

      bool
      empty (void) const;

      std::size_t
      length (void) const;

      // TODO add iterator begin(), end()

    protected:

      void
      remove (std::size_t pos);

    protected:

      DoubleListNodeThread* head_;
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
