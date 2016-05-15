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
#include <cmsis-plus/rtos/os-memory.h>

// Better be the last, to undef putchar()
#include <cmsis-plus/diag/trace.h>

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
      class Attributes : public Clocked_attributes
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
        constexpr
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
        void* mq_queue_address = nullptr;

        /**
         * @brief User defined message queue size.
         */
        std::size_t mq_queue_size_bytes = 0;

        // Add more attributes.

        /**
         * @}
         */
      };

      /**
       * @brief Default message queue initialiser.
       */
      extern const Attributes initializer;

      // Storage for a message queue. Each message is stored in an element
      // extended to a multiple of pointers. The lists are kept in two arrays
      // of indices and the priorities in a separate array.
      template<typename T, std::size_t msgs, std::size_t msg_size_bytes>
        class Arena
        {
        public:
          T queue[msgs * msg_size_bytes / sizeof(T)];
          T links[(2 * msgs) * sizeof(mqueue::index_t) / sizeof(T)];
          T prios[msgs * sizeof(mqueue::priority_t) / sizeof(T)];
        };

      template<typename T>
        constexpr std::size_t
        compute_allocated_size_bytes (std::size_t msgs,
                                      std::size_t msg_size_bytes)
        {
          // Align each message
          return (msgs * ((msg_size_bytes + (sizeof(T) - 1)) & ~(sizeof(T) - 1)))
          // Align the indices array
              + ((2 * msgs * sizeof(mqueue::index_t) + (sizeof(T) - 1))
                  & ~(sizeof(T) - 1))
              // Align the priority array
              + ((msgs * sizeof(mqueue::priority_t) + (sizeof(T) - 1))
                  & ~(sizeof(T) - 1));
        }

    } /* namespace mqueue */

// ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @brief Base class for POSIX compliant **message queue**.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     */
    class Message_queue_base : public Named_object
    {
    protected:

      /**
       * @name Constructors & Destructor
       * @{
       */

      // Internal constructors, used from templates.
      Message_queue_base ();
      Message_queue_base (const char* name);

    public:

      /**
       * @cond ignore
       */
      Message_queue_base (const Message_queue_base&) = delete;
      Message_queue_base (Message_queue_base&&) = delete;
      Message_queue_base&
      operator= (const Message_queue_base&) = delete;
      Message_queue_base&
      operator= (Message_queue_base&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the message queue.
       */
      ~Message_queue_base ();

      /**
       * @}
       */

      /**
       * @name Operators
       * @{
       */

      /**
       * @brief Compare memory queues.
       * @retval true The given memory queue is the same as this memory queue.
       * @retval false The memory queues are different.
       */
      bool
      operator== (const Message_queue_base& rhs) const;

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
       * @param [in] mprio The message priority. The default is 0.
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
      send (const void* msg, std::size_t nbytes, mqueue::priority_t mprio =
                mqueue::default_priority);

      /**
       * @brief Try to send a message to the queue.
       * @param [in] msg The address of the message to enqueue.
       * @param [in] nbytes The length of the message. Must be not
       *  higher than the value used when creating the queue.
       * @param [in] mprio The message priority. The default is 0.
       * @retval result::ok The message was enqueued.
       * @retval EWOULDBLOCK The specified message queue is full.
       * @retval EINVAL A parameter is invalid or outside of a permitted range.
       * @retval EMSGSIZE The specified message length, nbytes,
       *  exceeds the message size attribute of the message queue.
       * @retval ENOTRECOVERABLE The message could not be enqueue
       *  (extension to POSIX).
       */
      result_t
      try_send (const void* msg, std::size_t nbytes, mqueue::priority_t mprio =
                    mqueue::default_priority);

      /**
       * @brief Send a message to the queue with timeout.
       * @param [in] msg The address of the message to enqueue.
       * @param [in] nbytes The length of the message. Must be not
       *  higher than the value used when creating the queue.
       * @param [in] timeout The timeout duration.
       * @param [in] mprio The message priority. The default is 0.
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
      timed_send (const void* msg, std::size_t nbytes,
                  clock::duration_t timeout, mqueue::priority_t mprio =
                      mqueue::default_priority);

      /**
       * @brief Receive a message from the queue.
       * @param [out] msg The address where to store the dequeued message.
       * @param [in] nbytes The size of the destination buffer. Must
       *  be lower than the value used when creating the queue.
       * @param [out] mprio The address where to store the message
       *  priority. The default is `nullptr`.
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
      receive (void* msg, std::size_t nbytes, mqueue::priority_t* mprio =
                   nullptr);

      /**
       * @brief Try to receive a message from the queue.
       * @param [out] msg The address where to store the dequeued message.
       * @param [in] nbytes The size of the destination buffer. Must
       *  be lower than the value used when creating the queue.
       * @param [out] mprio The address where to store the message
       *  priority. The default is `nullptr`.
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
      try_receive (void* msg, std::size_t nbytes, mqueue::priority_t* mprio =
                       nullptr);

      /**
       * @brief Receive a message from the queue with timeout.
       * @param [out] msg The address where to store the dequeued message.
       * @param [in] nbytes The size of the destination buffer. Must
       *  be lower than the value used when creating the queue.
       * @param [in] timeout The timeout duration.
       * @param [out] mprio The address where to store the message
       *  priority. The default is `nullptr`.
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
      timed_receive (void* msg, std::size_t nbytes, clock::duration_t timeout,
                     mqueue::priority_t* mprio = nullptr);

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
       * @brief Internal function used during message queue construction.
       * @param [in] attr Reference to attributes.
       * @param [in] msgs The number of messages.
       * @param [in] msg_size_bytes The message size, in bytes.
       * @param [in] queue_address Pointer to queue storage.
       * @param [in] queue_size_bytes Size of queue storage.
       * @return Nothing.
       */
      void
      _construct (const mqueue::Attributes& attr, mqueue::size_t msgs,
                  mqueue::msg_size_t msg_size_bytes, void* queue_address,
                  std::size_t queue_size_bytes);

      /**
       * @brief Internal function used to initialise the queue to empty state.
       * @par Parameters
       *  None
       * @return Nothing.
       */
      void
      _init (void);

#if !defined(OS_INCLUDE_RTOS_PORT_MESSAGE_QUEUE)

      /**
       * @brief Internal function used to enqueue the message, if possible.
       * @param [in] msg The address of the message to enqueue.
       * @param [in] nbytes The length of the message. Must be not
       *  higher than the value used when creating the queue.
       * @param [in] mprio The message priority.
       * @retval true The message was enqueued.
       * @retval false The message queue is full.
       */
      bool
      _try_send (const void* msg, std::size_t nbytes, mqueue::priority_t mprio);

      /**
       * @brief Internal function used to dequeue a message, if available.
       * @param [out] msg The address where to store the dequeued message.
       * @param [in] nbytes The size of the destination buffer. Must
       *  be lower than the value used when creating the queue.
       * @param [out] mprio The address where to store the message
       *  priority.
       * @retval true The message was dequeued.
       * @retval false There are not messages in the queue.
       */
      bool
      _try_receive (void* msg, std::size_t nbytes, mqueue::priority_t* mprio);

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
      /**
       * @brief List of threads waiting to send.
       */
      Waiting_threads_list send_list_;
      /**
       * @brief List of threads waiting to receive.
       */
      Waiting_threads_list receive_list_;
      /**
       * @brief Pointer to clock to be used for timeouts.
       */
      Clock* clock_ = nullptr;

      // To save space, the double linked list is built
      // using short indexes, not pointers.
      /**
       * @brief Pointer to array with indices to previous nodes.
       */
      volatile mqueue::index_t* prev_array_ = nullptr;
      /**
       * @brief Pointer to array with indices to next nodes.
       */
      volatile mqueue::index_t* next_array_ = nullptr;
      /**
       * @brief Pointer to array of priorities.
       */
      volatile mqueue::priority_t* prio_array_ = nullptr;

      /**
       * @brief Pointer to the first free message, or `nullptr`.
       * @details
       * The free messages are in a single linked list, and
       * the allocation strategy is LIFO, messages freed by `receive()`
       * are added to the beginning, and messages requested by `send()`
       * are allocated also from the beginning, so only a pointer to
       * the beginning is required.
       */
      void* volatile first_free_ = nullptr;
#endif /* !defined(OS_INCLUDE_RTOS_PORT_MESSAGE_QUEUE) */

      /**
       * @brief The static address where the queue is stored
       * (from `attr.mq_queue_address`).
       */
      void* queue_addr_ = nullptr;
      /**
       * @brief The dynamic address if the queue was allocated
       * (and must be deallocated)
       */
      void* allocated_queue_addr_ = nullptr;
      /**
       * @brief Pointer to allocator.
       */
      const void* allocator_ = nullptr;

#if defined(OS_INCLUDE_RTOS_PORT_MESSAGE_QUEUE)
      friend class port::Message_queue_base;
      os_mqueue_port_data_t port_;
#endif

      /**
       * @brief Total size of the statically allocated queue storage
       * (from `attr.mq_queue_size_bytes`).
       */
      std::size_t queue_size_bytes_ = 0;
      /**
       * @brief Total size of the dynamically allocated queue storage.
       */
      std::size_t allocated_queue_size_elements_ = 0;

      /**
       * @brief Message size (aligned to size of pointer)
       */
      mqueue::msg_size_t msg_size_bytes_ = 0;
      /**
       * @brief Max number of messages.
       */
      mqueue::size_t msgs_ = 0;
      /**
       * @brief Current number of messages in the queue.
       */
      mqueue::size_t count_ = 0;

#if !defined(OS_INCLUDE_RTOS_PORT_MESSAGE_QUEUE)
      /**
       * @brief Index of the first message in the queue.
       */
      mqueue::index_t head_ = 0;
#endif /* !defined(OS_INCLUDE_RTOS_PORT_MESSAGE_QUEUE) */

      /**
       * @}
       */
    };

    // ========================================================================

    /**
     * @brief Template of a POSIX compliant **message queue** with allocator.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos
     */
    template<typename Allocator = memory::allocator<void*>>
      class Message_queue_allocated : public Message_queue_base
      {
      public:

        using allocator_type = Allocator;

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Create a message queue with default settings.
         * @param [in] msgs The number of messages.
         * @param [in] msg_size_bytes The message size, in bytes.
         * @param [in] allocator Reference to allocator. Default a local temporary instance.
         */
        Message_queue_allocated (mqueue::size_t msgs,
                                 mqueue::msg_size_t msg_size_bytes,
                                 const Allocator& allocator = Allocator ());

        /**
         * @brief Create a message queue with custom settings.
         * @param [in] attr Reference to attributes.
         * @param [in] msgs The number of messages.
         * @param [in] msg_size_bytes The message size, in bytes.
         * @param [in] allocator Reference to allocator. Default a local temporary instance.
         */
        Message_queue_allocated (const mqueue::Attributes& attr,
                                 mqueue::size_t msgs,
                                 mqueue::msg_size_t msg_size_bytes,
                                 const Allocator& allocator = Allocator ());

        /**
         * @cond ignore
         */
      public:

        Message_queue_allocated (const Message_queue_allocated&) = delete;
        Message_queue_allocated (Message_queue_allocated&&) = delete;
        Message_queue_allocated&
        operator= (const Message_queue_allocated&) = delete;
        Message_queue_allocated&
        operator= (Message_queue_allocated&&) = delete;
        /**
         * @endcond
         */

        /**
         * @brief Destroy the message queue.
         */
        ~Message_queue_allocated ();

        /**
         * @}
         */

      };

    // ========================================================================

    /**
     * @brief POSIX compliant **message queue**; instance of the
     * Message_queue_allocated template, using the default RTOS allocator.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos
     */
    class Message_queue : public Message_queue_allocated<>
    {
    public:

      using allocator_type = Message_queue_allocated::allocator_type;

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
      Message_queue (const mqueue::Attributes& attr, mqueue::size_t msgs,
                     mqueue::msg_size_t msg_size_bytes);

      /**
       * @cond ignore
       */
    public:

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
       */

    };

    // ========================================================================

    /**
     * @brief Template of a POSIX compliant **message queue** with
     * message type and allocator.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos
     */
    template<typename T, typename Allocator = memory::allocator<void*>>
      class Message_queue_typed : public Message_queue_allocated<Allocator>
      {
      public:

        using value_type = T;
        using allocator_type = Allocator;

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Create a typed message queue with default settings.
         * @param [in] msgs The number of messages.
         * @param [in] allocator Reference to allocator. Default a local temporary instance.
         */
        Message_queue_typed (mqueue::size_t msgs, const Allocator& allocator =
                                 Allocator ());

        /**
         * @brief Create a typed message queue with custom settings.
         * @param [in] attr Reference to attributes.
         * @param [in] msgs The number of messages.
         * @param [in] allocator Reference to allocator. Default a local temporary instance.
         */
        Message_queue_typed (const mqueue::Attributes& attr,
                             mqueue::size_t msgs, const Allocator& allocator =
                                 Allocator ());

        /**
         * @cond ignore
         */
        Message_queue_typed (const Message_queue_typed&) = delete;
        Message_queue_typed (Message_queue_typed&&) = delete;
        Message_queue_typed&
        operator= (const Message_queue_typed&) = delete;
        Message_queue_typed&
        operator= (Message_queue_typed&&) = delete;
        /**
         * @endcond
         */

        /**
         * @brief Destroy the typed message queue.
         */
        ~Message_queue_typed ();

        /**
         * @}
         */

      public:

        /**
         * @name Public Member Functions
         * @{
         */

        /**
         * @brief Send a typed message to the queue.
         * @param [in] msg The address of the message to enqueue.
         * @param [in] mprio The message priority. The default is 0.
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
        send (const T* msg,
              mqueue::priority_t mprio = mqueue::default_priority);

        /**
         * @brief Try to send a typed message to the queue.
         * @param [in] msg The address of the message to enqueue.
         *  higher than the value used when creating the queue.
         * @param [in] mprio The message priority. The default is 0.
         * @retval result::ok The message was enqueued.
         * @retval EWOULDBLOCK The specified message queue is full.
         * @retval EINVAL A parameter is invalid or outside of a permitted range.
         * @retval EMSGSIZE The specified message length, nbytes,
         *  exceeds the message size attribute of the message queue.
         * @retval ENOTRECOVERABLE The message could not be enqueue
         *  (extension to POSIX).
         */
        result_t
        try_send (const T* msg, mqueue::priority_t mprio =
                      mqueue::default_priority);

        /**
         * @brief Send a typed message to the queue with timeout.
         * @param [in] msg The address of the message to enqueue.
         * @param [in] timeout The timeout duration.
         * @param [in] mprio The message priority. The default is 0.
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
        timed_send (const T* msg, clock::duration_t timeout,
                    mqueue::priority_t mprio = mqueue::default_priority);

        /**
         * @brief Receive a typed message from the queue.
         * @param [out] msg The address where to store the dequeued message.
         * @param [out] mprio The address where to store the message
         *  priority. The default is `nullptr`.
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
        receive (T* msg, mqueue::priority_t* mprio = nullptr);

        /**
         * @brief Try to receive a typed message from the queue.
         * @param [out] msg The address where to store the dequeued message.
         *  be lower than the value used when creating the queue.
         * @param [out] mprio The address where to store the message
         *  priority. The default is `nullptr`.
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
        try_receive (T* msg, mqueue::priority_t* mprio = nullptr);

        /**
         * @brief Receive a typed message from the queue with timeout.
         * @param [out] msg The address where to store the dequeued message.
         * @param [in] timeout The timeout duration.
         * @param [out] mprio The address where to store the message
         *  priority. The default is `nullptr`.
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
        timed_receive (T* msg, clock::duration_t timeout,
                       mqueue::priority_t* mprio = nullptr);

        /**
         * @}
         */

      };

    // ========================================================================

    /**
     * @brief Template of a POSIX compliant **message queue** with
     * message type and local storage.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos
     */
    template<typename T, std::size_t N>
      class Message_queue_static : public Message_queue_base
      {
      public:

        using value_type = T;
        static const std::size_t msgs = N;

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Create a typed message queue with default settings.
         */
        Message_queue_static ();

        /**
         * @brief Create a typed message queue with custom settings.
         * @param [in] attr Reference to attributes.
         */
        Message_queue_static (const mqueue::Attributes& attr);

        /**
         * @cond ignore
         */
        Message_queue_static (const Message_queue_static&) = delete;
        Message_queue_static (Message_queue_static&&) = delete;
        Message_queue_static&
        operator= (const Message_queue_static&) = delete;
        Message_queue_static&
        operator= (Message_queue_static&&) = delete;
        /**
         * @endcond
         */

        /**
         * @brief Destroy the typed message queue.
         */
        ~Message_queue_static ();

        /**
         * @}
         */

      public:

        /**
         * @name Public Member Functions
         * @{
         */

        /**
         * @brief Send a typed message to the queue.
         * @param [in] msg The address of the message to enqueue.
         * @param [in] mprio The message priority. The default is 0.
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
        send (const T* msg,
              mqueue::priority_t mprio = mqueue::default_priority);

        /**
         * @brief Try to send a typed message to the queue.
         * @param [in] msg The address of the message to enqueue.
         *  higher than the value used when creating the queue.
         * @param [in] mprio The message priority. The default is 0.
         * @retval result::ok The message was enqueued.
         * @retval EWOULDBLOCK The specified message queue is full.
         * @retval EINVAL A parameter is invalid or outside of a permitted range.
         * @retval EMSGSIZE The specified message length, nbytes,
         *  exceeds the message size attribute of the message queue.
         * @retval ENOTRECOVERABLE The message could not be enqueue
         *  (extension to POSIX).
         */
        result_t
        try_send (const T* msg, mqueue::priority_t mprio =
                      mqueue::default_priority);

        /**
         * @brief Send a typed message to the queue with timeout.
         * @param [in] msg The address of the message to enqueue.
         * @param [in] timeout The timeout duration.
         * @param [in] mprio The message priority. The default is 0.
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
        timed_send (const T* msg, clock::duration_t timeout,
                    mqueue::priority_t mprio = mqueue::default_priority);

        /**
         * @brief Receive a typed message from the queue.
         * @param [out] msg The address where to store the dequeued message.
         * @param [out] mprio The address where to store the message
         *  priority. The default is `nullptr`.
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
        receive (T* msg, mqueue::priority_t* mprio = nullptr);

        /**
         * @brief Try to receive a typed message from the queue.
         * @param [out] msg The address where to store the dequeued message.
         *  be lower than the value used when creating the queue.
         * @param [out] mprio The address where to store the message
         *  priority. The default is `nullptr`.
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
        try_receive (T* msg, mqueue::priority_t* mprio = nullptr);

        /**
         * @brief Receive a typed message from the queue with timeout.
         * @param [out] msg The address where to store the dequeued message.
         * @param [in] timeout The timeout duration.
         * @param [out] mprio The address where to store the message
         *  priority. The default is `nullptr`.
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
        timed_receive (T* msg, clock::duration_t timeout,
                       mqueue::priority_t* mprio = nullptr);

        /**
         * @}
         */

      protected:

        /**
         * @name Private Member Variables
         * @{
         */

        /**
         * @brief Local storage for the queue.
         * @details
         * The local storage is large enough to include `msgs`
         * messages of type `T`, plus the separate linked lists and
         * priorities.
         * For performance reasons, the individual components are
         * aligned as pointers.
         */
        mqueue::Arena<void*, msgs, sizeof(T)> arena_;

        /**
         * @}
         */

      };

#pragma GCC diagnostic pop

  }
/* namespace rtos */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace rtos
  {
    namespace mqueue
    {
      constexpr
      Attributes::Attributes (const char* name) :
          Clocked_attributes
            { name }
      {
        ;
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
    Message_queue_base::operator== (const Message_queue_base& rhs) const
    {
      return this == &rhs;
    }

    /**
     * @details
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     */
    inline std::size_t
    Message_queue_base::length (void) const
    {
      return count_;
    }

    /**
     * @details
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     */
    inline std::size_t
    Message_queue_base::capacity (void) const
    {
      return msgs_;
    }

    /**
     * @details
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     */
    inline std::size_t
    Message_queue_base::msg_size (void) const
    {
      return msg_size_bytes_;
    }

    /**
     * @details
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     */
    inline bool
    Message_queue_base::empty (void) const
    {
      return (length () == 0);
    }

    /**
     * @details
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     */
    inline bool
    Message_queue_base::full (void) const
    {
      return (length () == capacity ());
    }

    // ========================================================================

    /**
     * @details
     * This constructor shall initialise the message queue object
     * with the given number of messages and default settings.
     * The effect shall be equivalent to creating a message queue object
     * referring to the attributes in `mqueue::initializer`.
     * Upon successful initialisation, the state of the message queue
     * object shall become initialised, with no messages in the queue.
     *
     * Only the message queue object itself may be used for performing
     * synchronisation. It is not allowed to make copies of
     * message queue objects.
     *
     * For default message queue objects, the storage is dynamically
     * allocated using the default allocator.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename Allocator>
      Message_queue_allocated<Allocator>::Message_queue_allocated (
          mqueue::size_t msgs, mqueue::msg_size_t msg_size_bytes,
          const Allocator& allocator)
      {
#if defined(OS_TRACE_RTOS_MQUEUE)
        trace::printf ("%s() @%p %s %d %d\n", __func__, this, name (), msgs,
                       msg_size_bytes);
#endif

        allocated_queue_size_elements_ = mqueue::compute_allocated_size_bytes<
            typename Allocator::value_type> (msgs, msg_size_bytes)
            / sizeof(typename Allocator::value_type);

        allocator_ = &allocator;

        allocated_queue_addr_ = const_cast<Allocator&> (allocator).allocate (
            allocated_queue_size_elements_);

        _construct (
            mqueue::initializer,
            msgs,
            msg_size_bytes,
            allocated_queue_addr_,
            allocated_queue_size_elements_
                * sizeof(typename Allocator::value_type));
      }

    /**
     * @details
     * This constructor shall initialise the message queue object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the memory pool attributes shall not be affected.
     * Upon successful initialisation, the state of the
     * message queue object shall become initialised.
     *
     * Only the message queue itself may be used for performing
     * synchronisation. It is not allowed to make copies of
     * message queue objects.
     *
     * In cases where default message queue attributes are
     * appropriate, the variable `mqueue::initializer` can be used to
     * initialise message queue.
     * The effect shall be equivalent to creating a message queue
     * object with the simple constructor.
     *
     * If the attributes define a storage area (via `mq_queue_address` and
     * `mq_queue_size_bytes`), that storage is used, otherwise
     * the storage is dynamically allocated.
     *
     * If the attributes define an allocator (via `mq_allocator`),
     * that allocator is used, otherwise
     * the default allocator is used.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename Allocator>
      Message_queue_allocated<Allocator>::Message_queue_allocated (
          const mqueue::Attributes& attr, mqueue::size_t msgs,
          mqueue::msg_size_t msg_size_bytes, const Allocator& allocator) :
          Message_queue_base (attr.name ())
      {
#if defined(OS_TRACE_RTOS_MQUEUE)
        trace::printf ("%s() @%p %s %d %d\n", __func__, this, name (), msgs,
                       msg_size_bytes);
#endif

        if (attr.mq_queue_address != nullptr)
          {
            // Do not use any allocator at all.
            _construct (mqueue::initializer, msgs, msg_size_bytes, nullptr, 0);
          }
        else
          {
            // If no user storage was provided via attributes,
            // allocate it dynamically via the allocator.
            allocated_queue_size_elements_ =
                mqueue::compute_allocated_size_bytes<
                    typename Allocator::value_type> (msgs, msg_size_bytes)
                    / sizeof(typename Allocator::value_type);

            allocator_ = &allocator;

            allocated_queue_addr_ =
                const_cast<Allocator&> (allocator).allocate (
                    allocated_queue_size_elements_);

            _construct (
                mqueue::initializer,
                msgs,
                msg_size_bytes,
                allocated_queue_addr_,
                allocated_queue_size_elements_
                    * sizeof(typename Allocator::value_type));
          }
      }

    /**
     * @details
     * Deallocate memory, either with the allocator defined via
     * attributes, or with the default allocator.
     */
    template<typename Allocator>
      Message_queue_allocated<Allocator>::~Message_queue_allocated ()
      {
#if defined(OS_TRACE_RTOS_MQUEUE)
        trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif
        typedef typename std::allocator_traits<Allocator>::pointer pointer;

        if (allocated_queue_addr_ != nullptr)
          {
            static_cast<Allocator*> (const_cast<void*> (allocator_))->deallocate (
                static_cast<pointer> (allocated_queue_addr_),
                allocated_queue_size_elements_);
          }
      }

    // ========================================================================

    /**
     * @details
     * This constructor shall initialise the message queue object
     * with the given number of messages and default settings.
     * The effect shall be equivalent to creating a message queue object
     * referring to the attributes in `mqueue::initializer`.
     * Upon successful initialisation, the state of the message queue
     * object shall become initialised, with no messages in the queue.
     *
     * Only the message queue object itself may be used for performing
     * synchronisation. It is not allowed to make copies of
     * message queue objects.
     *
     * For default message queue objects, the storage is dynamically
     * allocated using the RTOS specific allocator
     * (`rtos::memory::allocator`).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    inline
    Message_queue::Message_queue (mqueue::size_t msgs,
                                  mqueue::msg_size_t msg_size_bytes) :
        Message_queue_allocated (msgs, msg_size_bytes)
    {
      ;
    }

    /**
     * @details
     * This constructor shall initialise the message queue object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the memory pool attributes shall not be affected.
     * Upon successful initialisation, the state of the
     * message queue object shall become initialised.
     *
     * Only the message queue itself may be used for performing
     * synchronisation. It is not allowed to make copies of
     * message queue objects.
     *
     * In cases where default message queue attributes are
     * appropriate, the variable `mqueue::initializer` can be used to
     * initialise message queue.
     * The effect shall be equivalent to creating a message queue
     * object with the simple constructor.
     *
     * If the attributes define a storage area (via `mq_queue_address` and
     * `mq_queue_size_bytes`), that storage is used, otherwise
     * the storage is dynamically allocated.
     *
     * For these message queue objects, the storage is dynamically
     * allocated using the RTOS specific allocator
     * (`rtos::memory::allocator`).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    inline
    Message_queue::Message_queue (const mqueue::Attributes& attr,
                                  mqueue::size_t msgs,
                                  mqueue::msg_size_t msg_size_bytes) :
        Message_queue_allocated (attr, msgs, msg_size_bytes)
    {
      ;
    }

    /**
     * @details
     * Deallocate memory using the RTOS specific allocator
     * (`rtos::memory::allocator`).
     */
    inline
    Message_queue::~Message_queue ()
    {
      ;
    }

    // ========================================================================

    /**
     * @details
     * This constructor shall initialise the message queue object
     * with the given number of messages and default settings.
     * The effect shall be equivalent to creating a message queue object
     * referring to the attributes in `mqueue::initializer`.
     * Upon successful initialisation, the state of the message queue
     * object shall become initialised, with no messages in the queue.
     *
     * Only the message queue object itself may be used for performing
     * synchronisation. It is not allowed to make copies of
     * message queue objects.
     *
     * For default message queue objects, the storage shall be dynamically
     * allocated using the default allocator.
     *
     * Implemented as a wrapper over the parent constructor, automatically
     * passing the message size.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename T, typename Allocator>
      inline
      Message_queue_typed<T, Allocator>::Message_queue_typed (
          mqueue::size_t msgs, const Allocator& allocator) :
          Message_queue_allocated<Allocator> (msgs, sizeof(T), allocator)
      {
        ;
      }

    /**
     * @details
     * This constructor shall initialise the message queue object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the memory pool attributes shall not be affected.
     * Upon successful initialisation, the state of the
     * message queue object shall become initialised.
     *
     * Only the message queue itself may be used for performing
     * synchronisation. It is not allowed to make copies of
     * message queue objects.
     *
     * In cases where default message queue attributes are
     * appropriate, the variable `mqueue::initializer` can be used to
     * initialise message queue.
     * The effect shall be equivalent to creating a message queue
     * object with the simple constructor.
     *
     * If the attributes define a storage area, it shall be used, otherwise
     * storage shall be dynamically allocated, using the given allocator.
     *
     * Implemented as a wrapper over the parent constructor, automatically
     * passing the message size.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename T, typename Allocator>
      inline
      Message_queue_typed<T, Allocator>::Message_queue_typed (
          const mqueue::Attributes& attr, mqueue::size_t msgs,
          const Allocator& allocator) :
          Message_queue_allocated<Allocator> (attr, msgs, sizeof(T), allocator)
      {
        ;
      }

    /**
     * @details
     * Deallocate memory, either with the allocator defined via
     * attributes, or with the default allocator.
     *
     * Implemented as a wrapper over the parent destructor.
     */
    template<typename T, typename Allocator>
      inline
      Message_queue_typed<T, Allocator>::~Message_queue_typed ()
      {
        ;
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the message size.
     *
     * @see Message_queue_base::send().
     */
    template<typename T, typename Allocator>
      inline result_t
      Message_queue_typed<T, Allocator>::send (const T* msg,
                                               mqueue::priority_t mprio)
      {
        return Message_queue_allocated<Allocator>::send (
            reinterpret_cast<const char*> (msg), sizeof(T), mprio);
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the message size.
     *
     * @see Message_queue_base::try_send().
     */
    template<typename T, typename Allocator>
      inline result_t
      Message_queue_typed<T, Allocator>::try_send (const T* msg,
                                                   mqueue::priority_t mprio)
      {
        return Message_queue_allocated<Allocator>::try_send (
            reinterpret_cast<const char*> (msg), sizeof(T), mprio);
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the message size.
     *
     * @see Message_queue_base::timed_send().
     */
    template<typename T, typename Allocator>
      inline result_t
      Message_queue_typed<T, Allocator>::timed_send (const T* msg,
                                                     clock::duration_t timeout,
                                                     mqueue::priority_t mprio)
      {
        return Message_queue_allocated<Allocator>::timed_send (
            reinterpret_cast<const char*> (msg), sizeof(T), timeout, mprio);
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the message size.
     *
     * @see Message_queue_base::receive().
     */
    template<typename T, typename Allocator>
      inline result_t
      Message_queue_typed<T, Allocator>::receive (T* msg,
                                                  mqueue::priority_t* mprio)
      {
        return Message_queue_allocated<Allocator>::receive (
            reinterpret_cast<char*> (msg), sizeof(T), mprio);
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the message size.
     *
     * @see Message_queue_base::try_receive().
     */
    template<typename T, typename Allocator>
      inline result_t
      Message_queue_typed<T, Allocator>::try_receive (T* msg,
                                                      mqueue::priority_t* mprio)
      {
        return Message_queue_allocated<Allocator>::try_receive (
            reinterpret_cast<char*> (msg), sizeof(T), mprio);
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the message size.
     *
     * @see Message_queue_base::timed_receive().
     */
    template<typename T, typename Allocator>
      inline result_t
      Message_queue_typed<T, Allocator>::timed_receive (
          T* msg, clock::duration_t timeout, mqueue::priority_t* mprio)
      {
        return Message_queue_allocated<Allocator>::timed_receive (
            reinterpret_cast<char*> (msg), sizeof(T), timeout, mprio);
      }

    // ========================================================================

    /**
     * @details
     * This constructor shall initialise the message queue object
     * with the given number of messages and default settings.
     * The effect shall be equivalent to creating a message queue object
     * referring to the attributes in `mqueue::initializer`.
     * Upon successful initialisation, the state of the message queue
     * object shall become initialised, with no messages in the queue.
     *
     * Only the message queue object itself may be used for performing
     * synchronisation. It is not allowed to make copies of
     * message queue objects.
     *
     * The storage shall be statically allocated
     * inside the queue object instance.
     *
     * @note These objects are better instantiated as global static
     * objects. When instantiated on the thread stack, the stack
     * should be sized accordingly, including not only the
     * given number of messages, but also the internal lists overhead.
     *
     * Implemented as a wrapper over the parent constructor, automatically
     * passing the message size and the storage details.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename T, std::size_t N>
      inline
      Message_queue_static<T, N>::Message_queue_static ()
      {
        _construct (mqueue::initializer, msgs, sizeof(T), &arena_,
                    sizeof(arena_));
      }

    /**
     * @details
     * This constructor shall initialise the message queue object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the memory pool attributes shall not be affected.
     * Upon successful initialisation, the state of the
     * message queue object shall become initialised.
     *
     * Only the message queue itself may be used for performing
     * synchronisation. It is not allowed to make copies of
     * message queue objects.
     *
     * In cases where default message queue attributes are
     * appropriate, the variable `mqueue::initializer` can be used to
     * initialise message queue.
     * The effect shall be equivalent to creating a message queue
     * object with the simple constructor.
     *
     * The storage shall be statically allocated inside the queue
     * object instance.
     *
     * Passing a storage via the attributes is not allowed
     * and might trigger an assert.
     *
     * @note These objects are better instantiated as global static
     * objects. When instantiated on the thread stack, the stack
     * should be sized accordingly, including the internal lists overhead.
     *
     * Implemented as a wrapper over the parent constructor, automatically
     * passing the message size and the storage details.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename T, std::size_t N>
      inline
      Message_queue_static<T, N>::Message_queue_static (
          const mqueue::Attributes& attr) :
          Message_queue_base (attr.name ())
      {
        _construct (attr, msgs, sizeof(T), &arena_, sizeof(arena_));
      }

    template<typename T, std::size_t N>
      inline
      Message_queue_static<T, N>::~Message_queue_static ()
      {
        ;
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the message size.
     *
     * @see Message_queue_base::send().
     */
    template<typename T, std::size_t N>
      inline result_t
      Message_queue_static<T, N>::send (const T* msg, mqueue::priority_t mprio)
      {
        return Message_queue_base::send (reinterpret_cast<const char*> (msg),
                                         sizeof(T), mprio);
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the message size.
     *
     * @see Message_queue_base::try_send().
     */
    template<typename T, std::size_t N>
      inline result_t
      Message_queue_static<T, N>::try_send (const T* msg,
                                            mqueue::priority_t mprio)
      {
        return Message_queue_base::try_send (
            reinterpret_cast<const char*> (msg), sizeof(T), mprio);
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the message size.
     *
     * @see Message_queue_base::timed_send().
     */
    template<typename T, std::size_t N>
      inline result_t
      Message_queue_static<T, N>::timed_send (const T* msg,
                                              clock::duration_t timeout,
                                              mqueue::priority_t mprio)
      {
        return Message_queue_base::timed_send (
            reinterpret_cast<const char*> (msg), sizeof(T), timeout, mprio);
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the message size.
     *
     * @see Message_queue_base::receive().
     */
    template<typename T, std::size_t N>
      inline result_t
      Message_queue_static<T, N>::receive (T* msg, mqueue::priority_t* mprio)
      {
        return Message_queue_base::receive (reinterpret_cast<char*> (msg),
                                            sizeof(T), mprio);
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the message size.
     *
     * @see Message_queue_base::try_receive().
     */
    template<typename T, std::size_t N>
      inline result_t
      Message_queue_static<T, N>::try_receive (T* msg,
                                               mqueue::priority_t* mprio)
      {
        return Message_queue_base::try_receive (reinterpret_cast<char*> (msg),
                                                sizeof(T), mprio);
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the message size.
     *
     * @see Message_queue_base::timed_receive().
     */
    template<typename T, std::size_t N>
      inline result_t
      Message_queue_static<T, N>::timed_receive (T* msg,
                                                 clock::duration_t timeout,
                                                 mqueue::priority_t* mprio)
      {
        return Message_queue_base::timed_receive (reinterpret_cast<char*> (msg),
                                                  sizeof(T), timeout, mprio);
      }

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_MQUEUE_H_ */
