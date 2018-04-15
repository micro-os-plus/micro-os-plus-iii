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

#include <cmsis-plus/utils/lists.h>

#include <cmsis-plus/diag/trace.h>

namespace os
{
  namespace utils
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
#if defined(OS_TRACE_UTILS_LISTS)
          trace::printf ("%s() %p nop\n", __func__, this);
#endif
          return;
        }

#if defined(OS_TRACE_UTILS_LISTS)
      trace::printf ("%s() %p \n", __func__, this);
#endif

      // Make neighbours point to each other.
      prev_->next_ = next_;
      next_->prev_ = prev_;

      // Nullify both pointers in the unlinked node.
      prev_ = nullptr;
      next_ = nullptr;
    }

    // ========================================================================

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
#if defined(OS_TRACE_UTILS_LISTS)
      trace::printf ("%s() n=%p after %p\n", __func__, &node, after);
#endif

      // Unlinked nodes must have both pointers null.
      // If not, most probably the node was already linked.
      // Or the memory is corrupted.
      assert(node.prev () == nullptr);
      assert(node.next () == nullptr);

      // The `after` node must be linked. Only the `next` pointer is
      // tested, since only it is used.
      assert(after->next () != nullptr);

      // Make the new node point to its neighbours.
      node.prev (after);
      node.next (after->next ());

      // Make the neighbours point to the node. The order is important.
      after->next ()->prev (&node);
      after->next (&node);
    }

    // ========================================================================

    /**
     * @details
     * The initial list status is empty.
     */
    double_list::double_list ()
    {
#if defined(OS_TRACE_UTILS_LISTS_CONSTRUCT) || defined(OS_TRACE_UTILS_LISTS)
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
#if defined(OS_TRACE_UTILS_LISTS_CONSTRUCT) || defined(OS_TRACE_UTILS_LISTS)
      trace::printf ("%s() %p \n", __func__, this);
#endif

      assert(empty ());
    }

  } /* namespace utils */
} /* namespace os */
