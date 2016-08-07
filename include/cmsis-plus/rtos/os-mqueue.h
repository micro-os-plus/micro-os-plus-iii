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

#include <cmsis-plus/diag/trace.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @brief POSIX compliant **message queue**, using the
     * default RTOS allocator.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos-mqueue
     */
    class message_queue : public internal::object_named
    {
    public:

      // ======================================================================

      /**
       * @brief Type of a queue size storage.
       * @details
       * A numeric value to hold the message queue size, usually
       * an 8-bits value, possibly a 16-bits value if longer
       * queues are needed.
       * @ingroup cmsis-plus-rtos-mqueue
       */
#if defined(OS_BOOL_RTOS_MESSAGE_QUEUE_SIZE_16BITS)
      using size_t = uint16_t;
#else
      using size_t = uint8_t;
#endif

      /**
       * @brief Maximum queue size.
       * @ingroup cmsis-plus-rtos-mqueue
       */
      static constexpr message_queue::size_t max_size = 0xFF;

      /**
       * @brief Type of message size storage.
       * @ingroup cmsis-plus-rtos-mqueue
       */
      using msg_size_t = uint16_t;

      /**
       * @brief Maximum message size.
       * @ingroup cmsis-plus-rtos-mqueue
       */
      static constexpr msg_size_t max_msg_size = 0xFFFF;

      /**
       * @brief Type of list index storage.
       * @ingroup cmsis-plus-rtos-mqueue
       */
      using index_t = message_queue::size_t;

      /**
       * @brief Index value to represent an illegal index.
       * @ingroup cmsis-plus-rtos-mqueue
       */
      static constexpr index_t no_index = max_size;

      /**
       * @brief Type of message priority storage.
       * @details
       * A numeric value to hold the message priority, which
       * controls the order in which messages are added to the
       * queue (higher values represent higher priorities).
       * @ingroup cmsis-plus-rtos-mqueue
       */
      using priority_t = uint8_t;

      /**
       * @brief Default message priority.
       * @details
       * Use this value with `send()` if no special priorities are required.
       * @ingroup cmsis-plus-rtos-mqueue
       */
      static constexpr priority_t default_priority = 0;

      /**
       * @brief Maximum message priority.
       * @details
       * The maximum value allowed by the type, usually used for
       * validation.
       * @ingroup cmsis-plus-rtos-mqueue
       */
      static constexpr priority_t max_priority = 0xFF;

      // ======================================================================

      /**
       * @brief Message queue attributes.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @ingroup cmsis-plus-rtos-mqueue
       */
      class attributes : public internal::attributes_clocked
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a message queue attributes object instance.
         * @par Parameters
         *  None
         */
        constexpr
        attributes ();

        /**
         * @cond ignore
         */

        attributes (const attributes&) = default;
        attributes (attributes&&) = default;
        attributes&
        operator= (const attributes&) = default;
        attributes&
        operator= (attributes&&) = default;

        /**
         * @endcond
         */

        /**
         * @brief Destruct the message queue attributes object instance.
         */
        ~attributes () = default;

        /**
         * @}
         */

      public:

        /**
         * @name Public Member Variables
         * @{
         */

        // Public members; no accessors and mutators required.
        // Warning: must match the type & order of the C file header.
        /**
         * @brief Address of the user defined storage for the message queue.
         */
        void* mq_queue_address = nullptr;

        /**
         * @brief Size of the user defined storage for the message queue.
         */
        std::size_t mq_queue_size_bytes = 0;

        // Add more attributes here.

        /**
         * @}
         */

      }; /* class attributes */

      /**
       * @brief Default message queue initialiser.
       * @ingroup cmsis-plus-rtos-mqueue
       */
      static const attributes initializer;

      /**
       * @brief Default RTOS allocator.
       * @ingroup cmsis-plus-rtos-mqueue
       */
      using allocator_type = memory::allocator<thread::stack::allocation_element_t>;

      /**
       * @brief Storage for a static message queue.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @details
       * Each message is stored in an element
       * extended to a multiple of pointers. The lists are kept in two arrays
       * of indices and the priorities are kept in a separate array.
       */
      template<typename T, std::size_t msgs, std::size_t msg_size_bytes>
        class arena
        {
        public:
          T queue[(msgs * msg_size_bytes + sizeof(T) - 1) / sizeof(T)];
          T links[((2 * msgs) * sizeof(index_t) + sizeof(T) - 1) / sizeof(T)];
          T prios[(msgs * sizeof(priority_t) + sizeof(T) - 1) / sizeof(T)];
        };

      /**
       * @brief Calculator for queue storage requirements.
       * @param msgs Number of messages.
       * @param msg_size_bytes Size of message.
       * @return Total required storage in bytes, including
       * internal alignment.
       */
      template<typename T>
        constexpr std::size_t
        compute_allocated_size_bytes (std::size_t msgs,
                                      std::size_t msg_size_bytes)
        {
          // Align each message
          return (msgs * ((msg_size_bytes + (sizeof(T) - 1)) & ~(sizeof(T) - 1)))
          // Align the indices array
              + ((2 * msgs * sizeof(index_t) + (sizeof(T) - 1))
                  & ~(sizeof(T) - 1))
              // Align the priority array
              + ((msgs * sizeof(priority_t) + (sizeof(T) - 1))
                  & ~(sizeof(T) - 1));
        }

      // ======================================================================

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct a message queue object instance.
       * @param [in] msgs The number of messages.
       * @param [in] msg_size_bytes The message size, in bytes.
       * @param [in] attr Reference to attributes.
       * @param [in] allocator Reference to allocator. Default a
       * local temporary instance.
       */
      message_queue (std::size_t msgs, std::size_t msg_size_bytes,
                     const attributes& attr = initializer,
                     const allocator_type& allocator = allocator_type ());

      /**
       * @brief Construct a named message queue object instance.
       * @param [in] name Pointer to name.
       * @param [in] msgs The number of messages.
       * @param [in] msg_size_bytes The message size, in bytes.
       * @param [in] attr Reference to attributes.
       * @param [in] allocator Reference to allocator. Default a
       * local temporary instance.
       */
      message_queue (const char* name, std::size_t msgs,
                     std::size_t msg_size_bytes, const attributes& attr =
                         initializer,
                     const allocator_type& allocator = allocator_type ());

    protected:

      /**
       * @cond ignore
       */

      // Internal constructors, used from templates.
      message_queue ();
      message_queue (const char* name);

    public:

      message_queue (const message_queue&) = delete;
      message_queue (message_queue&&) = delete;
      message_queue&
      operator= (const message_queue&) = delete;
      message_queue&
      operator= (message_queue&&) = delete;

      /**
       * @endcond
       */

      /**
       * @brief Destruct the message queue object instance.
       */
      virtual
      ~message_queue ();

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
      operator== (const message_queue& rhs) const;

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
      send (const void* msg, std::size_t nbytes, priority_t mprio =
                default_priority);

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
      try_send (const void* msg, std::size_t nbytes, priority_t mprio =
                    default_priority);

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
                  clock::duration_t timeout,
                  priority_t mprio = default_priority);

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
      receive (void* msg, std::size_t nbytes, priority_t* mprio = nullptr);

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
      try_receive (void* msg, std::size_t nbytes, priority_t* mprio = nullptr);

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
                     priority_t* mprio = nullptr);

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
       * @cond ignore
       */

      /**
       * @brief Internal function used during message queue construction.
       * @param [in] msgs The number of messages.
       * @param [in] msg_size_bytes The message size, in bytes.
       * @param [in] attr Reference to attributes.
       * @param [in] queue_address Pointer to queue storage.
       * @param [in] queue_size_bytes Size of queue storage.
       * @par Returns
       *  Nothing.
       */
      void
      internal_construct_ (std::size_t msgs, std::size_t msg_size_bytes,
                           const attributes& attr, void* queue_address,
                           std::size_t queue_size_bytes);

      /**
       * @brief Internal function used to initialise the queue to empty state.
       * @par Parameters
       *  None
       * @par Returns
       *  Nothing.
       */
      void
      internal_init_ (void);

#if !defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE)

      /**
       * @brief Internal function used to enqueue a message, if possible.
       * @param [in] msg The address of the message to enqueue.
       * @param [in] nbytes The length of the message. Must be not
       *  higher than the value used when creating the queue.
       * @param [in] mprio The message priority.
       * @retval true The message was enqueued.
       * @retval false The message queue is full.
       */
      bool
      internal_try_send_ (const void* msg, std::size_t nbytes,
                          priority_t mprio);

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
      internal_try_receive_ (void* msg, std::size_t nbytes, priority_t* mprio);

#endif /* !defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE) */

      /**
       * @endcond
       */

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Variables
       * @{
       */

      /**
       * @cond ignore
       */

      // Keep these in sync with the structure declarations in os-c-decl.h.
#if !defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE)
      /**
       * @brief List of threads waiting to send.
       */
      internal::waiting_threads_list send_list_;
      /**
       * @brief List of threads waiting to receive.
       */
      internal::waiting_threads_list receive_list_;
      /**
       * @brief Pointer to clock to be used for timeouts.
       */
      clock* clock_ = nullptr;

      // To save space, the double linked list is built
      // using short indexes, not pointers.
      /**
       * @brief Pointer to array with indices to previous nodes.
       */
      volatile index_t* prev_array_ = nullptr;
      /**
       * @brief Pointer to array with indices to next nodes.
       */
      volatile index_t* next_array_ = nullptr;
      /**
       * @brief Pointer to array of priorities.
       */
      volatile priority_t* prio_array_ = nullptr;

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
#endif /* !defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE) */

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

#if defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE)
      friend class port::message_queue;
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
      message_queue::msg_size_t msg_size_bytes_ = 0;
      /**
       * @brief Max number of messages.
       */
      message_queue::size_t msgs_ = 0;
      /**
       * @brief Current number of messages in the queue.
       */
      message_queue::size_t count_ = 0;

#if !defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE)
      /**
       * @brief Index of the first message in the queue.
       */
      index_t head_ = 0;
#endif /* !defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE) */

      /**
       * @endcond
       */

      /**
       * @}
       */

    };

    // ========================================================================

    /**
     * @brief Template of a POSIX compliant **message queue** with allocator.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos-mqueue
     */
    template<typename Allocator = memory::allocator<void*>>
      class message_queue_allocated : public message_queue
      {
      public:

        /**
         * @brief Standard allocator type definition.
         */
        using allocator_type = Allocator;

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a message queue object instance.
         * @param [in] msgs The number of messages.
         * @param [in] msg_size_bytes The message size, in bytes.
         * @param [in] attr Reference to attributes.
         * @param [in] allocator Reference to allocator. Default a
         * local temporary instance.
         */
        message_queue_allocated (std::size_t msgs, std::size_t msg_size_bytes,
                                 const attributes& attr = initializer,
                                 const allocator_type& allocator =
                                     allocator_type ());

        /**
         * @brief Construct a named message queue object instance.
         * @param [in] name Pointer to name.
         * @param [in] msgs The number of messages.
         * @param [in] msg_size_bytes The message size, in bytes.
         * @param [in] attr Reference to attributes.
         * @param [in] allocator Reference to allocator. Default a
         * local temporary instance.
         */
        message_queue_allocated (const char* name, std::size_t msgs,
                                 std::size_t msg_size_bytes,
                                 const attributes& attr = initializer,
                                 const allocator_type& allocator =
                                     allocator_type ());

        /**
         * @cond ignore
         */

      public:

        message_queue_allocated (const message_queue_allocated&) = delete;
        message_queue_allocated (message_queue_allocated&&) = delete;
        message_queue_allocated&
        operator= (const message_queue_allocated&) = delete;
        message_queue_allocated&
        operator= (message_queue_allocated&&) = delete;

        /**
         * @endcond
         */

        /**
         * @brief Destruct the message queue.
         */
        virtual
        ~message_queue_allocated ();

        /**
         * @}
         */

      };

    // ========================================================================

    /**
     * @brief Template of a POSIX compliant **message queue** with
     * message type and allocator.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos-mqueue
     */
    template<typename T, typename Allocator = memory::allocator<void*>>
      class message_queue_typed : public message_queue_allocated<Allocator>
      {
      public:

        /**
         * @brief Standard allocator type definition.
         */
        using value_type = T;
        /**
         * @brief Standard allocator type definition.
         */
        using allocator_type = Allocator;

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a typed message queue object instance.
         * @param [in] msgs The number of messages.
         * @param [in] attr Reference to attributes.
         * @param [in] allocator Reference to allocator. Default a
         * local temporary instance.
         */
        message_queue_typed (std::size_t msgs,
                             const message_queue::attributes& attr =
                                 message_queue::initializer,
                             const allocator_type& allocator =
                                 allocator_type ());

        /**
         * @brief Construct a named typed message queue object instance.
         * @param [in] name Pointer to name.
         * @param [in] msgs The number of messages.
         * @param [in] attr Reference to attributes.
         * @param [in] allocator Reference to allocator. Default a
         * local temporary instance.
         */
        message_queue_typed (const char* name, std::size_t msgs,
                             const message_queue::attributes& attr =
                                 message_queue::initializer,
                             const allocator_type& allocator =
                                 allocator_type ());

        /**
         * @cond ignore
         */

        message_queue_typed (const message_queue_typed&) = delete;
        message_queue_typed (message_queue_typed&&) = delete;
        message_queue_typed&
        operator= (const message_queue_typed&) = delete;
        message_queue_typed&
        operator= (message_queue_typed&&) = delete;

        /**
         * @endcond
         */

        /**
         * @brief Destruct the typed message queue object instance.
         */
        virtual
        ~message_queue_typed ();

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
        send (const value_type* msg, message_queue::priority_t mprio =
                  message_queue::default_priority);

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
        try_send (const value_type* msg, message_queue::priority_t mprio =
                      message_queue::default_priority);

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
        timed_send (const value_type* msg, clock::duration_t timeout,
                    message_queue::priority_t mprio =
                        message_queue::default_priority);

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
        receive (value_type* msg, message_queue::priority_t* mprio = nullptr);

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
        try_receive (value_type* msg,
                     message_queue::priority_t* mprio = nullptr);

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
        timed_receive (value_type* msg, clock::duration_t timeout,
                       message_queue::priority_t* mprio = nullptr);

        /**
         * @}
         */

      };

    // ========================================================================

    /**
     * @brief Template of a POSIX compliant **message queue** with
     * message type and local storage.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos-mqueue
     */
    template<typename T, std::size_t N>
      class message_queue_static : public message_queue
      {
      public:

        /**
         * @brief Local type of message.
         */
        using value_type = T;

        /**
         * @brief Local constant based on template definition.
         */
        static const std::size_t msgs = N;

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a typed message queue object instance.
         * @param [in] attr Reference to attributes.
         */
        message_queue_static (const attributes& attr = initializer);

        /**
         * @brief Construct a named typed message queue object instance.
         * @param [in] name Pointer to name.
         * @param [in] attr Reference to attributes.
         */
        message_queue_static (const char* name, const attributes& attr =
                                  initializer);

        /**
         * @cond ignore
         */

        message_queue_static (const message_queue_static&) = delete;
        message_queue_static (message_queue_static&&) = delete;
        message_queue_static&
        operator= (const message_queue_static&) = delete;
        message_queue_static&
        operator= (message_queue_static&&) = delete;

        /**
         * @endcond
         */

        /**
         * @brief Destruct the typed message queue object instance.
         */
        virtual
        ~message_queue_static ();

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
        send (const value_type* msg, priority_t mprio = default_priority);

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
        try_send (const value_type* msg, priority_t mprio = default_priority);

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
        timed_send (const value_type* msg, clock::duration_t timeout,
                    priority_t mprio = default_priority);

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
        receive (value_type* msg, priority_t* mprio = nullptr);

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
        try_receive (value_type* msg, priority_t* mprio = nullptr);

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
        timed_receive (value_type* msg, clock::duration_t timeout,
                       priority_t* mprio = nullptr);

        /**
         * @}
         */

      protected:

        /**
         * @name Private Member Variables
         * @{
         */

        /**
         * @cond ignore
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
        arena<void*, msgs, sizeof(value_type)> arena_;

        /**
         * @endcond
         */

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
    constexpr
    message_queue::attributes::attributes ()
    {
      ;
    }

    // ========================================================================

    /**
     * @details
     * Identical message queues should have the same memory address.
     *
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     */
    inline bool
    message_queue::operator== (const message_queue& rhs) const
    {
      return this == &rhs;
    }

    /**
     * @details
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    inline std::size_t
    message_queue::length (void) const
    {
      return count_;
    }

    /**
     * @details
     * @par POSIX compatibility
     *
     * @note Can be invoked from Interrupt Service Routines.
     *  Extension to standard, no POSIX similar functionality identified.
     */
    inline std::size_t
    message_queue::capacity (void) const
    {
      return msgs_;
    }

    /**
     * @details
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    inline std::size_t
    message_queue::msg_size (void) const
    {
      return msg_size_bytes_;
    }

    /**
     * @details
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    inline bool
    message_queue::empty (void) const
    {
      return (length () == 0);
    }

    /**
     * @details
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    inline bool
    message_queue::full (void) const
    {
      return (length () == capacity ());
    }

    // ========================================================================

    /**
     * @details
     * This constructor shall initialise a message queue object
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
     * appropriate, the variable `message_queue::initializer` can be used to
     * initialise message queue.
     * The effect shall be equivalent to creating a message queue
     * object with the simple constructor.
     *
     * If the attributes define a storage area (via `mq_queue_address` and
     * `mq_queue_size_bytes`), that storage is used, otherwise
     * the storage is dynamically allocated using the RTOS specific allocator
     * (`rtos::memory::allocator`).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename Allocator>
      inline
      message_queue_allocated<Allocator>::message_queue_allocated (
          std::size_t msgs, std::size_t msg_size_bytes, const attributes& attr,
          const allocator_type& allocator) :
          message_queue_allocated
            { nullptr, msgs, msg_size_bytes, attr, allocator }
      {
        ;
      }

    /**
     * @details
     * This constructor shall initialise a named message queue object
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
     * appropriate, the variable `message_queue::initializer` can be used to
     * initialise message queue.
     * The effect shall be equivalent to creating a message queue
     * object with the simple constructor.
     *
     * If the attributes define a storage area (via `mq_queue_address` and
     * `mq_queue_size_bytes`), that storage is used, otherwise
     * the storage is dynamically allocated using the RTOS specific allocator
     * (`rtos::memory::allocator`).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename Allocator>
      message_queue_allocated<Allocator>::message_queue_allocated (
          const char* name, std::size_t msgs, std::size_t msg_size_bytes,
          const attributes& attr, const allocator_type& allocator) :
          message_queue
            { name }
      {
#if defined(OS_TRACE_RTOS_MQUEUE)
        trace::printf ("%s() @%p %s %d %d\n", __func__, this, this->name (),
                       msgs, msg_size_bytes);
#endif

        if (attr.mq_queue_address != nullptr)
          {
            // Do not use any allocator at all.
            internal_construct_ (msgs, msg_size_bytes, attr, nullptr, 0);
          }
        else
          {
            allocator_ = &allocator;

            // If no user storage was provided via attributes,
            // allocate it dynamically via the allocator.
            allocated_queue_size_elements_ = (compute_allocated_size_bytes<
                typename allocator_type::value_type> (msgs, msg_size_bytes)
                + sizeof(typename allocator_type::value_type) - 1)
                / sizeof(typename allocator_type::value_type);

            allocated_queue_addr_ =
                const_cast<allocator_type&> (allocator).allocate (
                    allocated_queue_size_elements_);

            internal_construct_ (
                msgs,
                msg_size_bytes,
                attr,
                allocated_queue_addr_,
                allocated_queue_size_elements_
                    * sizeof(typename allocator_type::value_type));
          }
      }

    /**
     * @details
     * This destructor shall destroy a message queue object; the object
     * becomes, in effect, uninitialised. An implementation may cause
     * the destructor to set the object to an invalid value.
     *
     * It shall be safe to destroy an initialised message queue object
     * upon which no threads are currently blocked. Attempting to
     * destroy a message queue object upon which other threads are
     * currently blocked results in undefined behaviour.
     *
     * If the storage for the message queue was dynamically allocated,
     * it is deallocated using the same allocator.
     */
    template<typename Allocator>
      message_queue_allocated<Allocator>::~message_queue_allocated ()
      {
#if defined(OS_TRACE_RTOS_MQUEUE)
        trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif
        typedef typename std::allocator_traits<allocator_type>::pointer pointer;

        if (allocated_queue_addr_ != nullptr)
          {
            static_cast<allocator_type*> (const_cast<void*> (allocator_))->deallocate (
                static_cast<pointer> (allocated_queue_addr_),
                allocated_queue_size_elements_);

            allocated_queue_addr_ = nullptr;
          }
      }

    // ========================================================================

    /**
     * @details
     * This constructor shall initialise a message queue object
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
     * appropriate, the variable `message_queue::initializer` can be used to
     * initialise message queue.
     * The effect shall be equivalent to creating a message queue
     * object with the simple constructor.
     *
     * If the attributes define a storage area (via `mq_queue_address` and
     * `mq_queue_size_bytes`), that storage is used, otherwise
     * the storage is dynamically allocated using the RTOS specific allocator
     * (`rtos::memory::allocator`).
     *
     * Implemented as a wrapper over the parent constructor, automatically
     * passing the message size.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename T, typename Allocator>
      inline
      message_queue_typed<T, Allocator>::message_queue_typed (
          std::size_t msgs, const message_queue::attributes& attr,
          const allocator_type& allocator) :
          message_queue_allocated<allocator_type>
            { msgs, sizeof(value_type), attr, allocator }
      {
        ;
      }

    /**
     * @details
     * This constructor shall initialise a named message queue object
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
     * appropriate, the variable `message_queue::initializer` can be used to
     * initialise message queue.
     * The effect shall be equivalent to creating a message queue
     * object with the simple constructor.
     *
     * If the attributes define a storage area (via `mq_queue_address` and
     * `mq_queue_size_bytes`), that storage is used, otherwise
     * the storage is dynamically allocated using the RTOS specific allocator
     * (`rtos::memory::allocator`).
     *
     * Implemented as a wrapper over the parent constructor, automatically
     * passing the message size.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename T, typename Allocator>
      inline
      message_queue_typed<T, Allocator>::message_queue_typed (
          const char* name, std::size_t msgs,
          const message_queue::attributes& attr,
          const allocator_type& allocator) :
          message_queue_allocated<allocator_type>
            { name, msgs, sizeof(value_type), attr, allocator }
      {
        ;
      }

    /**
     * @details
     * This destructor shall destroy a message queue object; the object
     * becomes, in effect, uninitialised. An implementation may cause
     * the destructor to set the object to an invalid value.
     *
     * It shall be safe to destroy an initialised message queue object
     * upon which no threads are currently blocked. Attempting to
     * destroy a message queue object upon which other threads are
     * currently blocked results in undefined behaviour.
     *
     * If the storage for the message queue was dynamically allocated,
     * it is deallocated using the same allocator.
     *
     * Implemented as a wrapper over the parent destructor.
     */
    template<typename T, typename Allocator>
      message_queue_typed<T, Allocator>::~message_queue_typed ()
      {
        ;
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the message size.
     *
     * @see message_queue::send().
     */
    template<typename T, typename Allocator>
      inline result_t
      message_queue_typed<T, Allocator>::send (const value_type* msg,
                                               message_queue::priority_t mprio)
      {
        return message_queue_allocated<allocator_type>::send (
            reinterpret_cast<const char*> (msg), sizeof(value_type), mprio);
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the message size.
     *
     * @see message_queue::try_send().
     */
    template<typename T, typename Allocator>
      inline result_t
      message_queue_typed<T, Allocator>::try_send (
          const value_type* msg, message_queue::priority_t mprio)
      {
        return message_queue_allocated<allocator_type>::try_send (
            reinterpret_cast<const char*> (msg), sizeof(value_type), mprio);
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the message size.
     *
     * @see message_queue::timed_send().
     */
    template<typename T, typename Allocator>
      inline result_t
      message_queue_typed<T, Allocator>::timed_send (
          const value_type* msg, clock::duration_t timeout,
          message_queue::priority_t mprio)
      {
        return message_queue_allocated<allocator_type>::timed_send (
            reinterpret_cast<const char*> (msg), sizeof(value_type), timeout,
            mprio);
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the message size.
     *
     * @see message_queue::receive().
     */
    template<typename T, typename Allocator>
      inline result_t
      message_queue_typed<T, Allocator>::receive (
          value_type* msg, message_queue::priority_t* mprio)
      {
        return message_queue_allocated<allocator_type>::receive (
            reinterpret_cast<char*> (msg), sizeof(value_type), mprio);
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the message size.
     *
     * @see message_queue::try_receive().
     */
    template<typename T, typename Allocator>
      inline result_t
      message_queue_typed<T, Allocator>::try_receive (
          value_type* msg, message_queue::priority_t* mprio)
      {
        return message_queue_allocated<allocator_type>::try_receive (
            reinterpret_cast<char*> (msg), sizeof(value_type), mprio);
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the message size.
     *
     * @see message_queue::timed_receive().
     */
    template<typename T, typename Allocator>
      inline result_t
      message_queue_typed<T, Allocator>::timed_receive (
          value_type* msg, clock::duration_t timeout,
          message_queue::priority_t* mprio)
      {
        return message_queue_allocated<allocator_type>::timed_receive (
            reinterpret_cast<char*> (msg), sizeof(value_type), timeout, mprio);
      }

    // ========================================================================

    /**
     * @details
     * This constructor shall initialise a message queue object
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
     * appropriate, the variable `message_queue::initializer` can be used to
     * initialise message queue.
     * The effect shall be equivalent to creating a message queue
     * object with the simple constructor.
     *
     * The storage shall be statically allocated inside the
     * message queue object instance.
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
      message_queue_static<T, N>::message_queue_static (const attributes& attr) :
          message_queue_static
            { nullptr, attr }
      {
        ;
      }

    /**
     * @details
     * This constructor shall initialise a named message queue object
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
     * appropriate, the variable `message_queue::initializer` can be used to
     * initialise message queue.
     * The effect shall be equivalent to creating a message queue
     * object with the simple constructor.
     *
     * The storage shall be statically allocated inside the
     * message queue object instance.
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
      message_queue_static<T, N>::message_queue_static (const char* name,
                                                        const attributes& attr) :
          message_queue (name)
      {
        internal_construct_ (msgs, sizeof(value_type), attr, &arena_,
                             sizeof(arena_));
      }

    /**
     * @details
     * This destructor shall destroy the message queue object; the object
     * becomes, in effect, uninitialised. An implementation may cause
     * the destructor to set the object to an invalid value.
     *
     * It shall be safe to destroy an initialised message queue object
     * upon which no threads are currently blocked. Attempting to
     * destroy a message queue object upon which other threads are
     * currently blocked results in undefined behaviour.
     *
     * Implemented as a wrapper over the parent destructor.
     */
    template<typename T, std::size_t N>
      message_queue_static<T, N>::~message_queue_static ()
      {
        ;
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the message size.
     *
     * @see message_queue::send().
     */
    template<typename T, std::size_t N>
      inline result_t
      message_queue_static<T, N>::send (const value_type* msg, priority_t mprio)
      {
        return message_queue::send (reinterpret_cast<const char*> (msg),
                                    sizeof(value_type), mprio);
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the message size.
     *
     * @see message_queue::try_send().
     */
    template<typename T, std::size_t N>
      inline result_t
      message_queue_static<T, N>::try_send (const value_type* msg,
                                            priority_t mprio)
      {
        return message_queue::try_send (reinterpret_cast<const char*> (msg),
                                        sizeof(value_type), mprio);
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the message size.
     *
     * @see message_queue::timed_send().
     */
    template<typename T, std::size_t N>
      inline result_t
      message_queue_static<T, N>::timed_send (const value_type* msg,
                                              clock::duration_t timeout,
                                              priority_t mprio)
      {
        return message_queue::timed_send (reinterpret_cast<const char*> (msg),
                                          sizeof(value_type), timeout, mprio);
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the message size.
     *
     * @see message_queue::receive().
     */
    template<typename T, std::size_t N>
      inline result_t
      message_queue_static<T, N>::receive (value_type* msg, priority_t* mprio)
      {
        return message_queue::receive (reinterpret_cast<char*> (msg),
                                       sizeof(value_type), mprio);
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the message size.
     *
     * @see message_queue::try_receive().
     */
    template<typename T, std::size_t N>
      inline result_t
      message_queue_static<T, N>::try_receive (value_type* msg,
                                               priority_t* mprio)
      {
        return message_queue::try_receive (reinterpret_cast<char*> (msg),
                                           sizeof(value_type), mprio);
      }

    /**
     * @details
     * Wrapper over the parent method, automatically
     * passing the message size.
     *
     * @see message_queue::timed_receive().
     */
    template<typename T, std::size_t N>
      inline result_t
      message_queue_static<T, N>::timed_receive (value_type* msg,
                                                 clock::duration_t timeout,
                                                 priority_t* mprio)
      {
        return message_queue::timed_receive (reinterpret_cast<char*> (msg),
                                             sizeof(value_type), timeout, mprio);
      }

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_MQUEUE_H_ */
