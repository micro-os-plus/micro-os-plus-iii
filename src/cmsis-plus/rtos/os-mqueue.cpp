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

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/diag/trace.h>

#include <cmsis-plus/rtos/port/os-decls.h>
#include <cmsis-plus/rtos/port/os-inlines.h>

// ----------------------------------------------------------------------------

#include <cassert>
#include <cerrno>
#include <cstdlib>

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    // ------------------------------------------------------------------------

    /**
     * @details
     * The os::rtos::mqueue namespace groups message queue attributes
     * and initialisers.
     */
    namespace mqueue
    {
      /**
       * @class Attributes
       * @details
       * Allow to assign a name and custom attributes (like a static
       * address) to the message queue.
       *
       * To simplify access, the member variables are public and do not
       * require accessors or mutators.
       *
       * @par POSIX compatibility
       *  Inspired by `mq_attr` from [<mqueue.h>](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/mqueue.h.html)
       *  (IEEE Std 1003.1, 2013 Edition).
       */

      /**
       * @var void* Attributes::mq_queue_address
       * @details
       * Set this variable to a user defined memory area large enough
       * to store the message queue. Usually this is a statically
       * allocated array of structures.
       *
       * The default value is `nullptr`, which means there is no
       * user defined message queue.
       */

      /**
       * @var std::size_t Attributes::mq_queue_size_bytes
       * @details
       * The message queue size must match exactly the allocated size. It is
       * used for validation; when the message queue is initialised,
       * this size must be large enough to accommodate the desired
       * message queue.
       *
       * If the @ref mq_queue_address is `nullptr`, this variable is not
       * checked, but it is recommended to leave it zero.
       */

      const Attributes initializer
        { nullptr };

    } /* namespace mqueue */

    // ------------------------------------------------------------------------

    /**
     * @class Message_queue
     * @details
     * Priority based, fixed size FIFO.
     *
     * @par POSIX compatibility
     *  Inspired by `mqd_t` from [<mqueue.h>](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/mqueue.h.html)
     *  (IEEE Std 1003.1, 2013 Edition).
     */

    /**
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Message_queue::Message_queue (mqueue::size_t msgs,
                                  mqueue::size_t msg_size_bytes) :
        Message_queue (mqueue::initializer, msgs, msg_size_bytes)
    {
      ;
    }

    /**
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Message_queue::Message_queue (const mqueue::Attributes&attr,
                                  mqueue::size_t msgs,
                                  mqueue::size_t msg_size_bytes) :
        Named_object
          { attr.name () }, //
        msgs_ (msgs), //
        msg_size_bytes_ (msg_size_bytes)
    {
      os_assert_throw(!scheduler::in_handler_mode (), EPERM);

      queue_addr_ = attr.mq_queue_address;
      queue_size_bytes_ = attr.mq_queue_size_bytes;
      if (queue_addr_ != nullptr)
        {
          os_assert_throw(queue_size_bytes_ > 0, EINVAL);
          os_assert_throw(
              queue_size_bytes_ >= (std::size_t) (msgs * msg_size_bytes),
              EINVAL);
        }

      count_ = 0;

      trace::printf ("%s() @%p %s %d %d\n", __func__, this, name (), msgs_,
                     msg_size_bytes_);

#if defined(OS_INCLUDE_PORT_RTOS_MESSAGE_QUEUE)

      port::Message_queue::create (this);

#else

      if (queue_addr_ == nullptr)
        {
          // TODO: dynamically alloc queue (msgs * msg_size_bytes).
        }

      // TODO
#endif
    }

    /**
     *
     */
    Message_queue::~Message_queue ()
    {
      trace::printf ("%s() @%p %s\n", __func__, this, name ());

#if defined(OS_INCLUDE_PORT_RTOS_MESSAGE_QUEUE)

      port::Message_queue::destroy (this);

#else

      // TODO: Free dynamically allocated structures.

#endif
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    /**
     * @details
     * The send() function shall add the message pointed to by the argument
     * msg to the message queue. The nbytes argument specifies the length
     * of the message, in bytes, pointed to by msg. The value of nbytes
     * shall be less than or equal to the msg_size_bytes parameter of the
     * message queue object, or send() shall fail.
     *
     * If the specified message queue is not full, send() shall behave
     * as if the message is inserted into the message queue at the
     * position indicated by the mprio argument. A message with a
     * larger numeric value of mprio shall be inserted before messages
     * with lower values of mprio. A message shall be inserted after
     * other messages in the queue, if any, with equal mprio. The
     * value of mprio shall be less than {MQ_PRIO_MAX}.
     *
     * If the specified message queue is full, send() shall block
     * until space becomes available to enqueue the message, or
     * until send() is cancelled/interrupted. If more than one
     * thread is waiting to send when space becomes available
     * in the message queue and the Priority Scheduling option is
     * supported, then the thread of the highest priority that has
     * been waiting the longest shall be unblocked to send its
     * message. Otherwise, it is unspecified which waiting thread
     * is unblocked.
     *
     * Compatible with POSIX `mq_send()` with O_NONBLOCK not set.
     * http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_timedsend.html
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Message_queue::send (const char* msg, std::size_t nbytes,
                         mqueue::priority_t mprio)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);
      os_assert_err(msg != nullptr, EINVAL);
      os_assert_err(nbytes >= msg_size_bytes_, EINVAL);

      trace::printf ("%s(%p,%d,%d) @%p %s\n", __func__, msg, nbytes, mprio,
                     this, name ());

#if defined(OS_INCLUDE_PORT_RTOS_MESSAGE_QUEUE)

      return port::Message_queue::send (this, msg, nbytes, mprio);

#else

      // TODO

      ++count_;
      return result::ok;

#endif
    }

    /**
     * @details
     * The send() function shall add the message pointed to by the argument
     * msg to the message queue. The nbytes argument specifies the length
     * of the message, in bytes, pointed to by msg. The value of nbytes
     * shall be less than or equal to the msg_size_bytes parameter of the
     * message queue object, or send() shall fail.
     *
     * If the message queue is not full, send() shall behave
     * as if the message is inserted into the message queue at the
     * position indicated by the mprio argument. A message with a
     * larger numeric value of mprio shall be inserted before messages
     * with lower values of mprio. A message shall be inserted after
     * other messages in the queue, if any, with equal mprio. The
     * value of mprio shall be less than {MQ_PRIO_MAX}.
     *
     * If the message queue is full, the message shall
     * not be queued and try_send() shall return an error.
     *
     * Compatible with POSIX `mq_send()` with O_NONBLOCK set.
     * http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_timedsend.html
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    result_t
    Message_queue::try_send (const char* msg, std::size_t nbytes,
                             mqueue::priority_t mprio)
    {
      os_assert_err(msg != nullptr, EINVAL);
      os_assert_err(nbytes >= msg_size_bytes_, EINVAL);

      trace::printf ("%s(%p,%d,%d) @%p %s\n", __func__, msg, nbytes, mprio,
                     this, name ());

#if defined(OS_INCLUDE_PORT_RTOS_MESSAGE_QUEUE)

      return port::Message_queue::try_send (this, msg, nbytes, mprio);

#else

      // TODO

      ++count_;
      return result::ok;

#endif
    }

    /**
     * @details
     * The send() function shall add the message pointed to by the argument
     * msg to the message queue. The nbytes argument specifies the length
     * of the message, in bytes, pointed to by msg. The value of nbytes
     * shall be less than or equal to the msg_size_bytes attribute of the
     * message queue object, or send() shall fail.
     *
     * If the message queue is not full, send() shall behave
     * as if the message is inserted into the message queue at the
     * position indicated by the mprio argument. A message with a
     * larger numeric value of mprio shall be inserted before messages
     * with lower values of mprio. A message shall be inserted after
     * other messages in the queue, if any, with equal mprio. The
     * value of mprio shall be less than {MQ_PRIO_MAX}.
     *
     * If the message queue is full, the wait for sufficient
     * room in the queue shall be terminated when the specified timeout
     * expires.
     *
     * The timeout shall expire after the number of time units (that
     * is when the value of that clock equals or exceeds (now()+duration).
     * The resolution of the timeout shall be the resolution of the
     * clock on which it is based (the SysTick clock for CMSIS).
     *
     * Under no circumstance shall the operation fail with a timeout
     * if there is sufficient room in the queue to add the message
     * immediately.
     *
     * Compatible with POSIX `mq_timedsend()`.
     * http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_timedsend.html
     *
     * Differences from the standard:
     * - the timeout is not expressed as an absolute time point, but
     * as a relative number of system ticks.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Message_queue::timed_send (const char* msg, std::size_t nbytes,
                               mqueue::priority_t mprio, systicks_t ticks)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);
      os_assert_err(msg != nullptr, EINVAL);
      os_assert_err(nbytes >= msg_size_bytes_, EINVAL);

      trace::printf ("%s(%p,%d,%d,%d_ticks) @%p %s\n", __func__, msg, nbytes,
                     mprio, ticks, this, name ());

      if (ticks == 0)
        {
          ticks = 1;
        }

#if defined(OS_INCLUDE_PORT_RTOS_MESSAGE_QUEUE)

      return port::Message_queue::timed_send (this, msg, nbytes, mprio, ticks);

#else

      // TODO

      ++count_;
      return result::ok;

#endif
    }

    /**
     * @details
     * The receive() function shall receive the oldest of the highest
     * priority message(s) from the message queue. If the size of the
     * buffer in bytes, specified by the nbytes argument, is less than
     * the msg_size_bytes attribute of the message queue, the function
     * shall fail and return an error. Otherwise, the selected message
     * shall be removed from the queue and copied to the buffer pointed
     * to by the msg argument.
     *
     * If the value of nbytes is greater than {SSIZE_MAX}, the result
     * is implementation-defined.
     *
     * If the argument mprio is not NULL, the priority of the selected
     * message shall be stored in the location referenced by mprio.
     *
     * If the message queue is empty, receive() shall block
     * until a message is enqueued on the message queue or until
     * receive() is cancelled/interrupted. If more than one thread
     * is waiting to receive a message when a message arrives at
     * an empty queue and the Priority Scheduling option is supported,
     * then the thread of highest priority that has been waiting the
     * longest shall be selected to receive the message. Otherwise,
     * it is unspecified which waiting thread receives the message.
     *
     * Compatible with POSIX `mq_receive()` with O_NONBLOCK not set.
     * http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Message_queue::receive (char* msg, std::size_t nbytes,
                            mqueue::priority_t* mprio)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);
      os_assert_err(msg != nullptr, EINVAL);
      os_assert_err(nbytes >= msg_size_bytes_, EMSGSIZE);

      trace::printf ("%s(%p,%d) @%p %s\n", __func__, msg, nbytes, this,
                     name ());

#if defined(OS_INCLUDE_PORT_RTOS_MESSAGE_QUEUE)

      return port::Message_queue::receive (this, msg, nbytes, mprio);

#else

      // TODO

      --count_;
      return result::ok;

#endif
    }

    /**
     * @details
     * The receive() function shall receive the oldest of the highest
     * priority message(s) from the message queue. If the size of the
     * buffer in bytes, specified by the nbytes argument, is less than
     * the msg_size_bytes attribute of the message queue, the function
     * shall fail and return an error. Otherwise, the selected message
     * shall be removed from the queue and copied to the buffer pointed
     * to by the msg argument.
     *
     * If the value of nbytes is greater than {SSIZE_MAX}, the result
     * is implementation-defined.
     *
     * If the argument mprio is not NULL, the priority of the selected
     * message shall be stored in the location referenced by mprio.
     *
     * If the message queue is empty, no message shall be removed
     * from the queue, and receive() shall return an error.
     *
     * Compatible with POSIX `mq_receive()` with O_NONBLOCK set.
     * http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    result_t
    Message_queue::try_receive (char* msg, std::size_t nbytes,
                                mqueue::priority_t* mprio)
    {
      os_assert_err(msg != nullptr, EINVAL);
      os_assert_err(nbytes >= msg_size_bytes_, EMSGSIZE);

      trace::printf ("%s(%p,%d) @%p %s\n", __func__, msg, nbytes, this,
                     name ());

#if defined(OS_INCLUDE_PORT_RTOS_MESSAGE_QUEUE)

      return port::Message_queue::try_receive (this, msg, nbytes, mprio);

#else

      // TODO
      --count_;
      return result::ok;

#endif
    }

    /**
     * @details
     * The receive() function shall receive the oldest of the highest
     * priority message(s) from the message queue. If the size of the
     * buffer in bytes, specified by the nbytes argument, is less than
     * the msg_size_bytes attribute of the message queue, the function
     * shall fail and return an error. Otherwise, the selected message
     * shall be removed from the queue and copied to the buffer pointed
     * to by the msg argument.
     *
     * If the value of nbytes is greater than {SSIZE_MAX}, the result
     * is implementation-defined.
     *
     * If the argument mprio is not NULL, the priority of the selected
     * message shall be stored in the location referenced by mprio.
     *
     * If the message queue is empty, receive() shall block
     * until a message is enqueued on the message queue or until
     * receive() is cancelled/interrupted. If more than one thread
     * is waiting to receive a message when a message arrives at
     * an empty queue and the Priority Scheduling option is supported,
     * then the thread of highest priority that has been waiting the
     * longest shall be selected to receive the message. Otherwise,
     * it is unspecified which waiting thread receives the message.
     *
     * The timed_receive() function shall receive the oldest of
     * the highest priority messages from the message queue as described
     * for the receive() function. However, if no message exists on the
     * queue to satisfy the receive, the wait for such a message shall
     * be terminated when the specified timeout expires.
     *
     * The timeout shall expire after the number of time units (that
     * is when the value of that clock equals or exceeds (now()+duration).
     * The resolution of the timeout shall be the resolution of the
     * clock on which it is based (the SysTick clock for CMSIS).
     *
     * Under no circumstance shall the operation fail with a timeout
     * if a message can be removed from the message queue immediately.
     * The validity of the abstime parameter need not be checked if a
     * message can be removed from the message queue immediately.
     *
     * Compatible with POSIX `mq_receive()` with O_NONBLOCK set.
     * http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Message_queue::timed_receive (char* msg, std::size_t nbytes,
                                  mqueue::priority_t* mprio, systicks_t ticks)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);
      os_assert_err(msg != nullptr, EINVAL);
      os_assert_err(nbytes >= msg_size_bytes_, EMSGSIZE);

      trace::printf ("%s(%p,%d,%d_ticks) @%p %s\n", __func__, msg, nbytes,
                     ticks, this, name ());

      if (ticks == 0)
        {
          ticks = 1;
        }

#if defined(OS_INCLUDE_PORT_RTOS_MESSAGE_QUEUE)

      return port::Message_queue::timed_receive (this, msg, nbytes, mprio,
                                                 ticks);

#else

      // TODO
      --count_;
      return result::ok;

#endif
    }

#pragma GCC diagnostic pop

    /**
     * @details
     * Clear both send and receive counter and return the queue to the
     * initial state.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Message_queue::reset (void)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s() @%p %s\n", __func__, this, name ());

#if defined(OS_INCLUDE_PORT_RTOS_MESSAGE_QUEUE)

      return port::Message_queue::reset (this);

#else

      // TODO
      count_ = 0;
      return result::ok;

#endif
    }

  // --------------------------------------------------------------------------

  } /* namespace rtos */
} /* namespace os */
