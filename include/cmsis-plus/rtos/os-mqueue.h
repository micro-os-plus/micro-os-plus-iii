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
 * The initial CMSIS++ RTOS API was inspired by CMSIS RTOS API v1.x,
 * Copyright (c) 2013 ARM LIMITED.
 */

#ifndef CMSIS_PLUS_RTOS_OS_MQUEUE_H_
#define CMSIS_PLUS_RTOS_OS_MQUEUE_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

#include <cmsis-plus/rtos/os-decls.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    namespace mqueue
    {

      // ======================================================================

      /**
       * @brief Message queue attributes.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class Attributes : public Clocked_attribute
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Create message queue attributes.
         * @param [in] name Null terminated name. If `nullptr`, "-" is assigned.
         */
        Attributes (const char* name);

        /**
         * @cond ignore
         */
        Attributes (const Attributes&) = default;
        Attributes (Attributes&&) = default;
        Attributes&
        operator= (const Attributes&) = default;
        Attributes&
        operator= (Attributes&&) = default;
        /**
         * @endcond
         */

        /**
         * @brief Destroy the message queue attributes.
         */
        ~Attributes () = default;

        /**
         * @}
         */

      public:

        /**
         * @name Public Member Variables
         * @{
         */

        // Public members, no accessors and mutators required.
        // Warning: must match the type & order of the C file header.
        /**
         * @brief User defined message queue address.
         */
        void* mq_queue_address;

        /**
         * @brief User defined message queue size.
         */
        std::size_t mq_queue_size_bytes;

        //
        // TODO: add clock ID.
        //
        // Add more attributes.

        /**
         * @}
         */
      };

      /**
       * @brief Default message queue initialiser.
       */
      extern const Attributes initializer;

    } /* namespace mqueue */

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @brief POSIX compliant **message queue**.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos
     */
    class Message_queue : public Named_object
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a message queue with default settings.
       * @param [in] msgs The number of messages.
       * @param [in] msg_size_bytes The message size, in bytes.
       */
      Message_queue (mqueue::size_t msgs, mqueue::msg_size_t msg_size_bytes);

      /**
       * @brief Create a message queue with custom settings.
       * @param [in] attr Reference to attributes.
       * @param [in] msgs The number of messages.
       * @param [in] msg_size_bytes The message size, in bytes.
       */
      Message_queue (const mqueue::Attributes&attr, mqueue::size_t msgs,
                     mqueue::msg_size_t msg_size_bytes);

      /**
       * @cond ignore
       */
      Message_queue (const Message_queue&) = delete;
      Message_queue (Message_queue&&) = delete;
      Message_queue&
      operator= (const Message_queue&) = delete;
      Message_queue&
      operator= (Message_queue&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the message queue.
       */
      ~Message_queue ();

      /**
       * @}
       * @name Operators
       * @{
       */

      /**
       * @brief Compare memory queues.
       * @retval true The given memory queue is the same as this memory queue.
       * @retval false The memory queues are different.
       */
      bool
      operator== (const Message_queue& rhs) const;

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Send a message to the queue.
       * @param [in] msg The address of the message to enqueue.
       * @param [in] nbytes The length of the message. Must be not
       *  higher than the value used when creating the queue.
       * @param [in] mprio The message priority.
       * @retval result::ok The message was enqueued.
       * @retval EINVAL A parameter is invalid or outside of a permitted range.
       * @retval EMSGSIZE The specified message length, nbytes,
       *  exceeds the message size attribute of the message queue.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval ENOTRECOVERABLE The message could not be enqueue
       *  (extension to POSIX).
       * @retval EINTR The operation was interrupted.
       */
      result_t
      send (const char* msg, std::size_t nbytes, mqueue::priority_t mprio);

      /**
       * @brief Try to send a message to the queue.
       * @param [in] msg The address of the message to enqueue.
       * @param [in] nbytes The length of the message. Must be not
       *  higher than the value used when creating the queue.
       * @param [in] mprio The message priority.
       * @retval result::ok The message was enqueued.
       * @retval EWOULDBLOCK The specified message queue is full.
       * @retval EINVAL A parameter is invalid or outside of a permitted range.
       * @retval EMSGSIZE The specified message length, nbytes,
       *  exceeds the message size attribute of the message queue.
       * @retval ENOTRECOVERABLE The message could not be enqueue
       *  (extension to POSIX).
       */
      result_t
      try_send (const char* msg, std::size_t nbytes, mqueue::priority_t mprio);

      /**
       * @brief Send a message to the queue with timeout.
       * @param [in] msg The address of the message to enqueue.
       * @param [in] nbytes The length of the message. Must be not
       *  higher than the value used when creating the queue.
       * @param [in] mprio The message priority.
       * @param [in] timeout The timeout duration.
       * @retval result::ok The message was enqueued.
       * @retval EINVAL A parameter is invalid or outside of a permitted range.
       * @retval EMSGSIZE The specified message length, nbytes,
       *  exceeds the message size attribute of the message queue.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval ETIMEDOUT The timeout expired before the message
       *  could be added to the queue.
       * @retval ENOTRECOVERABLE The message could not be enqueue
       *  (extension to POSIX).
       * @retval EINTR The operation was interrupted.
       */
      result_t
      timed_send (const char* msg, std::size_t nbytes, mqueue::priority_t mprio,
                  clock::duration_t timeout);

      /**
       * @brief Receive a message from the queue.
       * @param [out] msg The address where to store the dequeued message.
       * @param [in] nbytes The size of the destination buffer. Must
       *  be lower than the value used when creating the queue.
       * @param [out] mprio The address where to store the message
       *  priority. May be `nullptr`.
       * @retval result::ok The message was received.
       * @retval EINVAL A parameter is invalid or outside of a permitted range.
       * @retval EMSGSIZE The specified message length, nbytes, is
       *  greater than the message size attribute of the message queue.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval ENOTRECOVERABLE The message could not be dequeued
       *  (extension to POSIX).
       * @retval EBADMSG The implementation has detected a data corruption
       *  problem with the message.
       * @retval EINTR The operation was interrupted.
       */
      result_t
      receive (char* msg, std::size_t nbytes, mqueue::priority_t* mprio);

      /**
       * @brief Try to receive a message from the queue.
       * @param [out] msg The address where to store the dequeued message.
       * @param [in] nbytes The size of the destination buffer. Must
       *  be lower than the value used when creating the queue.
       * @param [out] mprio The address where to store the message
       *  priority. May be `nullptr`.
       * @retval result::ok The message was received.
       * @retval EINVAL A parameter is invalid or outside of a permitted range.
       * @retval EMSGSIZE The specified message length, nbytes, is
       *  greater than the message size attribute of the message queue.
       * @retval ENOTRECOVERABLE The message could not be dequeued
       *  (extension to POSIX).
       * @retval EBADMSG The implementation has detected a data corruption
       *  problem with the message.
       * @retval EWOULDBLOCK The specified message queue is empty.
       */
      result_t
      try_receive (char* msg, std::size_t nbytes, mqueue::priority_t* mprio);

      /**
       * @brief Receive a message from the queue with timeout.
       * @param [out] msg The address where to store the dequeued message.
       * @param [in] nbytes The size of the destination buffer. Must
       *  be lower than the value used when creating the queue.
       * @param [out] mprio The address where to store the message
       *  priority. May be `nullptr`.
       * @param [in] timeout The timeout duration.
       * @retval result::ok The message was received.
       * @retval EINVAL A parameter is invalid or outside of a permitted range.
       * @retval EMSGSIZE The specified message length, nbytes, is
       *  greater than the message size attribute of the message queue.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval ENOTRECOVERABLE The message could not be dequeued
       *  (extension to POSIX).
       * @retval EBADMSG The implementation has detected a data corruption
       *  problem with the message.
       * @retval EINTR The operation was interrupted.
       * @retval ETIMEDOUT No message arrived on the queue before the
       *  specified timeout expired.
       */
      result_t
      timed_receive (char* msg, std::size_t nbytes, mqueue::priority_t* mprio,
                     clock::duration_t timeout);

      // TODO: check if some kind of peek() is useful.

      /**
       * @brief Get queue capacity.
       * @par Parameters
       *  None
       * @return The max number of messages that can be queued.
       */
      std::size_t
      capacity (void) const;

      /**
       * @brief Get queue length.
       * @par Parameters
       *  None
       * @return The number of messages in the queue.
       */
      std::size_t
      length (void) const;

      /**
       * @brief Get message size.
       * @par Parameters
       *  None
       * @return The message size, in bytes.
       */
      std::size_t
      msg_size (void) const;

      /**
       * @brief Check if the queue is empty.
       * @par Parameters
       *  None
       * @retval true The queue has no messages.
       * @retval false The queue has some messages.
       */
      bool
      empty (void) const;

      /**
       * @brief Check if the queue is full.
       * @par Parameters
       *  None
       * @retval true The queue is full.
       * @retval false The queue is not full.
       */
      bool
      full (void) const;

      /**
       * @brief Reset the message queue.
       * @par Parameters
       *  None
       * @retval result::ok The queue was reset.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       */
      result_t
      reset (void);

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Functions
       * @{
       */

      /**
       * @brief Internal initialisation.
       * @par Parameters
       *  None
       */
      void
      _init (void);

#if !defined(OS_INCLUDE_RTOS_PORT_MESSAGE_QUEUE)

      bool
      _try_send (const char* msg, std::size_t nbytes, mqueue::priority_t mprio);

      bool
      _try_receive (char* msg, std::size_t nbytes, mqueue::priority_t* mprio);

#endif /* !defined(OS_INCLUDE_RTOS_PORT_MESSAGE_QUEUE) */

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Variables
       * @{
       */

      // Keep these in sync with the structure declarations in os-c-decl.h.
#if !defined(OS_INCLUDE_RTOS_PORT_MESSAGE_QUEUE)
      port::Waiting_threads_list send_list_;
      port::Waiting_threads_list receive_list_;
      Clock& clock_;

      // To save space, the double linked list is built
      // using short indexes, not pointers.
      volatile mqueue::index_t* prev_array_;
      volatile mqueue::index_t* next_array_;
      volatile mqueue::priority_t* prio_array_;

      /**
       * @brief Pointer to the first free message, or `nullptr`.
       * @details
       * The free messages are in a single linked list, and
       * the allocation strategy is LIFO, messages freed by `receive()`
       * are added to the beginning, and messages requested by `send()`
       * are allocated also from the beginning, so only a pointer to
       * the beginning is required.
       */
      void* volatile first_free_;
#endif /* !defined(OS_INCLUDE_RTOS_PORT_MESSAGE_QUEUE) */

      void* queue_addr_;

#if defined(OS_INCLUDE_RTOS_PORT_MESSAGE_QUEUE)
      friend class port::Message_queue;
      os_mqueue_port_data_t port_;
#endif

      std::size_t queue_size_bytes_;

      const mqueue::msg_size_t msg_size_bytes_;
      const mqueue::size_t msgs_;
      mqueue::size_t count_;

#if !defined(OS_INCLUDE_RTOS_PORT_MESSAGE_QUEUE)
      mqueue::index_t head_;
#endif /* !defined(OS_INCLUDE_RTOS_PORT_MESSAGE_QUEUE) */

      /**
       * @brief Internal status bits.
       */
      uint8_t flags_;

      /**
       * @brief Internal bits.
       */
      enum
        : uint8_t
          {
            /**
             * @brief Remember to free the allocated memory block.
             */
            flags_allocated = 1
      };

      /**
       * @}
       */
    };

#pragma GCC diagnostic pop

  } /* namespace rtos */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace rtos
  {
    namespace mqueue
    {
      inline
      Attributes::Attributes (const char* name) :
          Clocked_attribute (name)
      {
        mq_queue_address = nullptr;
        mq_queue_size_bytes = 0;
      }

    } /* namespace mqueue */

    // ========================================================================

    /**
     * @details
     * Identical message queues should have the same memory address.
     *
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     */
    inline bool
    Message_queue::operator== (const Message_queue& rhs) const
    {
      return this == &rhs;
    }

    /**
     * @details
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     */
    inline std::size_t
    Message_queue::length (void) const
    {
      return count_;
    }

    /**
     * @details
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     */
    inline std::size_t
    Message_queue::capacity (void) const
    {
      return msgs_;
    }

    /**
     * @details
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     */
    inline std::size_t
    Message_queue::msg_size (void) const
    {
      return msg_size_bytes_;
    }

    /**
     * @details
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     */
    inline bool
    Message_queue::empty (void) const
    {
      return (length () == 0);
    }

    /**
     * @details
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     */
    inline bool
    Message_queue::full (void) const
    {
      return (length () == capacity ());
    }

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_MQUEUE_H_ */
