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

#ifndef CMSIS_PLUS_UTILS_LISTS_H_
#define CMSIS_PLUS_UTILS_LISTS_H_

// ----------------------------------------------------------------------------

#ifdef  __cplusplus

#include <cstdint>
#include <cstddef>
#include <cassert>
#include <iterator>

namespace os
{
  namespace utils
  {
    // ========================================================================

    /**
     * @brief Statically allocated core of a double linked list,
     * pointers to next, previous.
     * @headerfile lists.h <cmsis-plus/utils/lists.h>
     * @ingroup cmsis-plus-utils
     */
    class static_double_list_links
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct a list node (BSS initialised).
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
       * @brief Destruct the node.
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
       * @par Returns
       *  Nothing.
       */
      void
      unlink (void);

      /**
       * @brief Check if the node is unlinked.
       * @retval true The node is not linked.
       * @retval false The node is linked to a list.
       */
      bool
      unlinked (void);

      static_double_list_links*
      next (void) const;

      static_double_list_links*
      prev (void) const;

      void
      next (static_double_list_links* n);

      void
      prev (static_double_list_links* n);

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Variables
       * @{
       */

      /**
       * @brief Pointer to previous node.
       */
      static_double_list_links* prev_;

      /**
       * @brief Pointer to next node.
       */
      static_double_list_links* next_;

      /**
       * @}
       */

    };

    // ========================================================================

    /**
     * @brief The core of a double linked list, pointers to next,
     * previous.
     * @headerfile lists.h <cmsis-plus/utils/lists.h>
     * @ingroup cmsis-plus-utils
     */
    class double_list_links : public static_double_list_links
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct a list node (explicitly set to nullptr).
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
       * @brief Destruct the node.
       */
      ~double_list_links ();

      /**
       * @}
       */

    };

    // ========================================================================

    /**
     * @brief Template for a double linked list iterator.
     * @headerfile lists.h <cmsis-plus/utils/lists.h>
     * @ingroup cmsis-plus-utils
     * @tparam T Type of object returned by the iterator.
     * @tparam N Type of intrusive node. Must have the public members
     * **prev** & **next**.
     * @tparam MP Name of the intrusive node member in object T.
     * @tparam U Type stored in the list, derived from T.
     *
     * @details
     * This class provides an interface similar to std::list::iterator.
     */
    template<typename T, typename N, T* N::* MP, typename U = T>
      class double_list_iterator
      {
      public:

        /**
         * @name Public Types
         * @{
         */

        /**
         * @brief Type of value "pointed to" by the iterator.
         */
        using value_type = U;

        /**
         * @brief Type of pointer to object "pointed to" by the iterator.
         */
        using pointer = U*;

        /**
         * @brief Type of reference to object "pointed to" by the iterator.
         */
        using reference = U&;

        /**
         * @brief Type of reference to the iterator internal pointer.
         */
        using iterator_pointer = N*;

        /**
         * @brief Type of pointer difference.
         */
        using difference_type = ptrdiff_t;

        /**
         * @brief Category of iterator.
         */
        using iterator_category = std::forward_iterator_tag;

        /**
         * @}
         */

        // --------------------------------------------------------------------
        /**
         * @name Constructors & Destructor
         * @{
         */

        constexpr
        double_list_iterator ();

        constexpr explicit
        double_list_iterator (iterator_pointer const node);

        constexpr explicit
        double_list_iterator (reference element);

        /**
         * @}
         */

        /**
         * @name Operators
         * @{
         */

        pointer
        operator-> () const;

        reference
        operator* () const;

        double_list_iterator&
        operator++ ();

        double_list_iterator
        operator++ (int);

        double_list_iterator&
        operator-- ();

        double_list_iterator
        operator-- (int);

        bool
        operator== (const double_list_iterator& other) const;

        bool
        operator!= (const double_list_iterator& other) const;

        /**
         * @}
         */

        /**
         * @name Public Member Functions
         * @{
         */

        /**
         * @brief Get the object node from the intrusive node.
         * @return Pointer to object node.
         */
        pointer
        get_pointer (void) const;

        iterator_pointer
        get_iterator_pointer () const;

        /**
         * @}
         */

      protected:

        /**
         * @name Private Member Variables
         * @{
         */

        /**
         * @brief Pointer to intrusive node.
         */
        iterator_pointer node_;

        /**
         * @}
         */

      };

    // ========================================================================

    /**
     * @brief Statically allocated circular double linked list of nodes.
     * @headerfile lists.h <cmsis-plus/utils/lists.h>
     * @ingroup cmsis-plus-utils
     */
    class static_double_list
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct a list.
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
       * @brief Destruct the list.
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
       * @brief Check if the list is unitialised.
       * @par Parameters
       *  None.
       * @retval true The list was not initialised.
       * @retval false The list was initialised.
       */
      bool
      uninitialized (void) const;

      /**
       * @brief Clear the list.
       * @par Parameters
       *  None.
       * @par Returns
       *  Nothing.
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
       * @par Returns
       *  Nothing.
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
      static_double_list_links head_;

      /**
       * @}
       */
    };

    // ========================================================================

    /**
     * @brief Circular double linked list of nodes.
     * @headerfile lists.h <cmsis-plus/utils/lists.h>
     * @ingroup cmsis-plus-utils
     */
    class double_list : public static_double_list
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct a list.
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
       * @brief Destruct the list.
       */
      ~double_list ();

      /**
       * @}
       */

    };

    // ========================================================================

    /**
     * @brief Template for an intrusive list iterator.
     * @headerfile lists.h <cmsis-plus/utils/lists.h>
     * @ingroup cmsis-plus-utils
     * @tparam T Type of object that includes the intrusive node.
     * @tparam N Type of intrusive node. Must have the public members
     * **prev** & **next**.
     * @tparam MP Name of the intrusive node member in object T.
     * @tparam U Type stored in the list, derived from T.
     *
     * @details
     * This class provides an interface similar to std::list::iterator.
     *
     * @par Examples
     *
     * @code{.cpp}
     * using threads_list = utils::intrusive_list<
     * thread, utils::double_list_links, &thread::child_links_>;
     * @endcode
     */
    template<typename T, typename N, N T::* MP, typename U = T>
      class intrusive_list_iterator
      {
      public:

        /**
         * @name Public Types
         * @{
         */

        /**
         * @brief Type of value "pointed to" by the iterator.
         */
        using value_type = U;

        /**
         * @brief Type of pointer to object "pointed to" by the iterator.
         */
        using pointer = U*;

        /**
         * @brief Type of reference to object "pointed to" by the iterator.
         */
        using reference = U&;

        /**
         * @brief Type of reference to the iterator internal pointer.
         */
        using iterator_pointer = N*;

        /**
         * @brief Type of pointer difference.
         */
        using difference_type = ptrdiff_t;

        /**
         * @brief Category of iterator.
         */
        using iterator_category = std::forward_iterator_tag;

        /**
         * @}
         */

        // --------------------------------------------------------------------
        /**
         * @name Constructors & Destructor
         * @{
         */

        constexpr
        intrusive_list_iterator ();

        constexpr explicit
        intrusive_list_iterator (iterator_pointer const node);

        constexpr explicit
        intrusive_list_iterator (reference element);

        /**
         * @}
         */

        /**
         * @name Operators
         * @{
         */

        pointer
        operator-> () const;

        reference
        operator* () const;

        intrusive_list_iterator&
        operator++ ();

        intrusive_list_iterator
        operator++ (int);

        intrusive_list_iterator&
        operator-- ();

        intrusive_list_iterator
        operator-- (int);

        bool
        operator== (const intrusive_list_iterator& other) const;

        bool
        operator!= (const intrusive_list_iterator& other) const;

        /**
         * @}
         */

        /**
         * @name Public Member Functions
         * @{
         */

        /**
         * @brief Get the object node from the intrusive node.
         * @return Pointer to object node.
         */
        pointer
        get_pointer (void) const;

        iterator_pointer
        get_iterator_pointer () const;

        /**
         * @}
         */

      protected:

        /**
         * @name Private Member Variables
         * @{
         */

        /**
         * @brief Pointer to intrusive node.
         */
        iterator_pointer node_;

        /**
         * @}
         */

      };

    // ========================================================================

    /**
     * @brief List of intrusive nodes.
     * @headerfile lists.h <cmsis-plus/utils/lists.h>
     * @ingroup cmsis-plus-utils
     */
    template<typename T, typename N, N T::* MP>
      class intrusive_list : public static_double_list
      {
      public:

        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using difference_type = ptrdiff_t;

        using iterator = intrusive_list_iterator<T, N, MP>;

        /**
         * @brief Type of reference to the iterator internal pointer.
         */
        using iterator_pointer = N*;

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct an intrusive list.
         */
        intrusive_list ();

        /**
         * @brief Construct an intrusive list with controlled inits.
         * @param clr If true, the list is cleared.
         */
        intrusive_list (bool clr);

        /**
         * @cond ignore
         */

        intrusive_list (const intrusive_list&) = delete;
        intrusive_list (intrusive_list&&) = delete;
        intrusive_list&
        operator= (const intrusive_list&) = delete;
        intrusive_list&
        operator= (intrusive_list&&) = delete;

        /**
         * @endcond
         */

        /**
         * @brief Destruct the list.
         */
        ~intrusive_list ();

        /**
         * @}
         */

      public:

        /**
         * @name Public Member Functions
         * @{
         */

        /**
         * @brief Add a node to the list.
         * @param [in] node Reference to a list node.
         * @par Returns
         *  Nothing.
         */
        void
        link (reference node);

        /**
         * @brief Iterator begin.
         * @return An iterator positioned at the first element.
         */
        iterator
        begin ();

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

  // --------------------------------------------------------------------------
  } /* namespace utils */
} /* namespace os */

// ----------------------------------------------------------------------------

namespace os
{
  namespace utils
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

    inline bool
    static_double_list_links::unlinked (void)
    {
      return (next_ == nullptr);
    }

    inline static_double_list_links*
    static_double_list_links::next (void) const
    {
      return next_;
    }

    inline static_double_list_links*
    static_double_list_links::prev (void) const
    {
      return prev_;
    }

    inline void
    static_double_list_links::next (static_double_list_links* n)
    {
      next_ = n;
    }

    inline void
    static_double_list_links::prev (static_double_list_links* n)
    {
      prev_ = n;
    }

    // ========================================================================

    inline
    double_list_links::double_list_links ()
    {
      prev_ = nullptr;
      next_ = nullptr;
    }

    inline
    double_list_links::~double_list_links ()
    {
      ;
    }

    // ========================================================================
    template<typename T, typename N, T* N::* MP, typename U>
      constexpr
      double_list_iterator<T, N, MP, U>::double_list_iterator () :
          node_
            { }
      {
        ;
      }

    template<typename T, typename N, T* N::* MP, typename U>
      constexpr
      double_list_iterator<T, N, MP, U>::double_list_iterator (
          iterator_pointer const node) :
          node_
            { node }
      {
        ;
      }

    template<typename T, typename N, T* N::* MP, typename U>
      constexpr
      double_list_iterator<T, N, MP, U>::double_list_iterator (
          reference element) :
          node_
            { &(element.*MP) }
      {
        static_assert(std::is_convertible<U, T>::value == true, "U must be implicitly convertible to T!");
      }

    template<typename T, typename N, T* N::* MP, typename U>
      inline typename double_list_iterator<T, N, MP, U>::pointer
      double_list_iterator<T, N, MP, U>::operator-> () const
      {
        return get_pointer ();
      }

    template<typename T, typename N, T* N::* MP, typename U>
      inline typename double_list_iterator<T, N, MP, U>::reference
      double_list_iterator<T, N, MP, U>::operator* () const
      {
        return *get_pointer ();
      }

    template<typename T, typename N, T* N::* MP, typename U>
      inline double_list_iterator<T, N, MP, U>&
      double_list_iterator<T, N, MP, U>::operator++ ()
      {
        node_ = static_cast<N*> (node_->next ());
        return *this;
      }

    template<typename T, typename N, T* N::* MP, typename U>
      inline double_list_iterator<T, N, MP, U>
      double_list_iterator<T, N, MP, U>::operator++ (int)
      {
        const auto tmp = *this;
        node_ = static_cast<iterator_pointer> (node_->next);
        return tmp;
      }

    template<typename T, typename N, T* N::* MP, typename U>
      inline double_list_iterator<T, N, MP, U>&
      double_list_iterator<T, N, MP, U>::operator-- ()
      {
        node_ = static_cast<iterator_pointer> (node_->prev);
        return *this;
      }

    template<typename T, typename N, T* N::* MP, typename U>
      double_list_iterator<T, N, MP, U>
      double_list_iterator<T, N, MP, U>::operator-- (int)
      {
        const auto tmp = *this;
        node_ = static_cast<iterator_pointer> (node_->prev);
        return tmp;
      }

    template<typename T, typename N, T* N::* MP, typename U>
      inline bool
      double_list_iterator<T, N, MP, U>::operator== (
          const double_list_iterator& other) const
      {
        return node_ == other.node_;
      }

    template<typename T, typename N, T* N::* MP, typename U>
      inline bool
      double_list_iterator<T, N, MP, U>::operator!= (
          const double_list_iterator& other) const
      {
        return node_ != other.node_;
      }

#if 1
    template<typename T, typename N, T* N::* MP, typename U>
      inline typename double_list_iterator<T, N, MP, U>::pointer
      double_list_iterator<T, N, MP, U>::get_pointer (void) const
      {
        return (node_->*MP);
      }

    template<typename T, typename N, T* N::* MP, typename U>
      inline typename double_list_iterator<T, N, MP, U>::iterator_pointer
      double_list_iterator<T, N, MP, U>::get_iterator_pointer () const
      {
        return node_;
      }
#endif

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
    static_double_list::uninitialized (void) const
    {
      // If it points to nowhere, it is not yet initialised.
      return (head_.prev () == nullptr);
    }

    inline bool
    static_double_list::empty (void) const
    {
      // If it points to itself, it is empty.
      return (head_.next () == &head_) || (head_.next () == nullptr);
    }

    inline volatile static_double_list_links*
    static_double_list::head (void) const
    {
      return static_cast<volatile static_double_list_links*> (head_.next ());
    }

    inline volatile static_double_list_links*
    static_double_list::tail (void) const
    {
      return (head_.prev ());
    }

    // ========================================================================

    template<typename T, typename N, N T::* MP, typename U>
      constexpr
      intrusive_list_iterator<T, N, MP, U>::intrusive_list_iterator () :
          node_
            { }
      {
        ;
      }

    template<typename T, typename N, N T::* MP, typename U>
      constexpr
      intrusive_list_iterator<T, N, MP, U>::intrusive_list_iterator (
          N* const node) :
          node_
            { node }
      {
        ;
      }

    template<typename T, typename N, N T::* MP, typename U>
      constexpr
      intrusive_list_iterator<T, N, MP, U>::intrusive_list_iterator (
          reference element) :
          node_
            { &(element.*MP) }
      {
        static_assert(std::is_convertible<U, T>::value == true, "U must be implicitly convertible to T!");
      }

    template<typename T, typename N, N T::* MP, typename U>
      inline typename intrusive_list_iterator<T, N, MP, U>::pointer
      intrusive_list_iterator<T, N, MP, U>::operator-> () const
      {
        return get_pointer ();
      }

    template<typename T, typename N, N T::* MP, typename U>
      inline typename intrusive_list_iterator<T, N, MP, U>::reference
      intrusive_list_iterator<T, N, MP, U>::operator* () const
      {
        return *get_pointer ();
      }

    template<typename T, typename N, N T::* MP, typename U>
      inline intrusive_list_iterator<T, N, MP, U>&
      intrusive_list_iterator<T, N, MP, U>::operator++ ()
      {
        node_ = static_cast<iterator_pointer> (node_->next ());
        return *this;
      }

    template<typename T, typename N, N T::* MP, typename U>
      inline intrusive_list_iterator<T, N, MP, U>
      intrusive_list_iterator<T, N, MP, U>::operator++ (int)
      {
        const auto tmp = *this;
        node_ = static_cast<iterator_pointer> (node_->next);
        return tmp;
      }

    template<typename T, typename N, N T::* MP, typename U>
      inline intrusive_list_iterator<T, N, MP, U>&
      intrusive_list_iterator<T, N, MP, U>::operator-- ()
      {
        node_ = static_cast<iterator_pointer> (node_->prev);
        return *this;
      }

    template<typename T, typename N, N T::* MP, typename U>
      intrusive_list_iterator<T, N, MP, U>
      intrusive_list_iterator<T, N, MP, U>::operator-- (int)
      {
        const auto tmp = *this;
        node_ = static_cast<iterator_pointer> (node_->prev);
        return tmp;
      }

    template<typename T, typename N, N T::* MP, typename U>
      inline bool
      intrusive_list_iterator<T, N, MP, U>::operator== (
          const intrusive_list_iterator& other) const
      {
        return node_ == other.node_;
      }

    template<typename T, typename N, N T::* MP, typename U>
      inline bool
      intrusive_list_iterator<T, N, MP, U>::operator!= (
          const intrusive_list_iterator& other) const
      {
        return node_ != other.node_;
      }

    template<typename T, typename N, N T::* MP, typename U>
      inline typename intrusive_list_iterator<T, N, MP, U>::pointer
      intrusive_list_iterator<T, N, MP, U>::get_pointer (void) const
      {
        // static_assert(std::is_convertible<U, T>::value == true, "U must be implicitly convertible to T!");

        // Compute the distance between the member intrusive link
        // node and the class begin.
        const auto offset =
            reinterpret_cast<difference_type> (&(static_cast<pointer> (nullptr)
                ->*MP));

        // Compute the address of the object which includes the
        // intrusive node, by adjusting down the node address.
        return reinterpret_cast<pointer> (reinterpret_cast<difference_type> (node_)
            - offset);
      }

    template<typename T, typename N, N T::* MP, typename U>
      inline typename intrusive_list_iterator<T, N, MP, U>::iterator_pointer
      intrusive_list_iterator<T, N, MP, U>::get_iterator_pointer () const
      {
        return node_;
      }

    // ========================================================================

    template<typename T, typename N, N T::* MP>
      inline
      intrusive_list<T, N, MP>::intrusive_list ()
      {
        ;
      }

    template<typename T, typename N, N T::* MP>
      inline
      intrusive_list<T, N, MP>::intrusive_list (bool clr)
      {
        if (clr)
          {
            clear ();
          }
      }

    template<typename T, typename N, N T::* MP>
      inline
      intrusive_list<T, N, MP>::~intrusive_list ()
      {
        ;
      }

    template<typename T, typename N, N T::* MP>
      void
      intrusive_list<T, N, MP>::link (T& node)
      {
        if (uninitialized ())
          {
            // If this is the first time, initialise the list to empty.
            clear ();
          }

        // Compute the distance between the member intrusive link
        // node and the class begin.
        const auto offset =
            reinterpret_cast<difference_type> (&(static_cast<pointer> (nullptr)
                ->*MP));

        // Add thread intrusive node at the end of the list.
        insert_after (
            *reinterpret_cast<static_double_list_links*> (reinterpret_cast<difference_type> (&node)
                + offset),
            const_cast<static_double_list_links *> (tail ()));
      }

    /**
     * @details
     * @note It is not `const` because it may initialise on first use.
     */
    template<typename T, typename N, N T::* MP>
      inline typename intrusive_list<T, N, MP>::iterator
      intrusive_list<T, N, MP>::begin ()
      {
        if (uninitialized ())
          {
            // If this is the first time, initialise the list to empty.
            clear ();
          }
        return iterator
          { static_cast<iterator_pointer> (head_.next ()) };
      }

    template<typename T, typename N, N T::* MP>
      inline typename intrusive_list<T, N, MP>::iterator
      intrusive_list<T, N, MP>::end () const
      {
        return iterator
          {
              static_cast<iterator_pointer> (const_cast<static_double_list_links*> (&head_)) };
      }

  } /* namespace utils */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_INTERNAL_OS_LISTS_H_ */
