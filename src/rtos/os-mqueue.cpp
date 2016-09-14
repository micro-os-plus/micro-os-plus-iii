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

#include <cmsis-plus/rtos/os.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    // ------------------------------------------------------------------------

    /**
     * @class message_queue::attributes
     * @details
     * Allow to assign a name and custom attributes (like a static
     * address) to the message queue.
     *
     * To simplify access, the member variables are public and do not
     * require accessors or mutators.
     *
     * @par POSIX compatibility
     *  Inspired by `mq_attr`
     *  from [<mqueue.h>](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/mqueue.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     */

    /**
     * @var void* message_queue::attributes::mq_queue_address
     * @details
     * Set this variable to a user defined memory area large enough
     * to store the message queue. Usually this is a statically
     * allocated array of structures.
     *
     * The default value is `nullptr`, which means there is no
     * user defined message queue.
     */

    /**
     * @var std::size_t message_queue::attributes::mq_queue_size_bytes
     * @details
     * The message queue size must match exactly the allocated size. It is
     * used for validation; when the message queue is initialised,
     * this size must be large enough to accommodate the desired
     * message queue.
     *
     * If the @ref mq_queue_address is `nullptr`, this variable is not
     * checked, but it is recommended to leave it zero.
     */

    /**
     * @details
     * This variable is used by the default constructor.
     */
    const message_queue::attributes message_queue::initializer;

    // ------------------------------------------------------------------------

    /**
     * @class message_queue_allocated
     * @details
     * POSIX message queues allow threads to exchange data in the form of
     * messages. Messages are transferred to and from a queue using
     * send() and receive(). Each message has an associated priority,
     * and messages are always delivered to the receiving process
     * highest priority first.
     *
     * The storage for the message queue is allocated dynamically,
     * using a standard C++11 allocator. By default, this is the
     * RTOS specific allocator (`os::memory::allocator`).
     *
     * For special cases, the storage can be allocated outside the
     * class and specified via the `mq_queue_address` and
     * `mq_queue_size_bytes` attributes.
     *
     * A representative instance of this template is `message_queue`,
     * which is also used by the C API.
     *
     * @par Example
     *
     * @code{.cpp}
     * // Define the message type.
     * typedef struct {
     *   uint32_t id;
     * } msg_t;
     *
     * // Define the queue size.
     * constexpr uint32_t queue_size = 5;
     *
     * // User defined allocator.
     * template<typename T>
     *   class my_allocator;
     *
     * // The queue storage is allocated dynamically on the heap.
     * message_queue_allocated<my_allocator<void*>> mq { queue_size, sizeof(msg_t) };
     *
     * void
     * consumer(void)
     * {
     *   // Do something
     *   msg_t msg;
     *   for (; some_condition();)
     *     {
     *       mq.receive(&msg, sizeof(msg));
     *       // Process message
     *       if (msg.id == 7)
     *         {
     *           // Something special
     *         }
     *     }
     *   // Do something else.
     * }
     *
     * void
     * producer(void)
     * {
     *   // Do something
     *   msg_t msg;
     *   msg.id = 7;
     *   mq.send(&msg, sizeof(msg));
     *   // Do something else.
     * }
     * @endcode
     *
     * @par POSIX compatibility
     *  Inspired by `mqd_t`
     *  from [<mqueue.h>](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/mqueue.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     */

    /**
     * @class message_queue
     * @details
     * POSIX message queues allow threads to exchange data in the form of
     * messages. Messages are transferred to and from a queue using
     * send() and receive(). Each message has an associated priority,
     * and messages are always delivered to the receiving process
     * highest priority first.
     *
     * The storage for the message queue is allocated dynamically,
     * using the
     * RTOS specific allocator (`os::memory::allocator`).
     *
     * For special cases, the storage can be allocated outside the
     * class and specified via the `mq_queue_address` and
     * `mq_queue_size_bytes` attributes.
     *
     * message_queue is a representative instance of the
     * message_queue_allocated template;
     * it is also used by the C API.
     *
     * @par Example
     *
     * @code{.cpp}
     * // Define the message type.
     * typedef struct {
     *   uint32_t id;
     * } msg_t;
     *
     * // Define the queue size.
     * constexpr uint32_t queue_size = 5;
     *
     * // The queue storage is allocated dynamically on the heap.
     * message_queue mq { queue_size, sizeof(msg_t) };
     *
     * void
     * consumer(void)
     * {
     *   // Do something
     *   msg_t msg;
     *   for (; some_condition();)
     *     {
     *       mq.receive(&msg, sizeof(msg));
     *       // Process message
     *       if (msg.id == 7)
     *         {
     *           // Something special
     *         }
     *     }
     *   // Do something else.
     * }
     *
     * void
     * producer(void)
     * {
     *   // Do something
     *   msg_t msg;
     *   msg.id = 7;
     *   mq.send(&msg, sizeof(msg));
     *   // Do something else.
     * }
     * @endcode
     *
     * @par POSIX compatibility
     *  Inspired by `mqd_t`
     *  from [<mqueue.h>](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/mqueue.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     */

    /**
     * @class message_queue_typed
     * @details
     * If the message is defined as a type (usually an aggregate), it is
     * possible to automate even further the use of message queues,
     * by using the `message_queue_typed` template, which automatically
     * provides message sizes.
     *
     * @par Example
     *
     * @code{.cpp}
     * // Define the message type.
     * typedef struct {
     *   uint32_t id;
     * } msg_t;
     *
     * // Define the queue size.
     * constexpr uint32_t queue_size = 5;
     *
     * // The queue storage is allocated dynamically on the heap.
     * message_queue_typed<msg_t> mq { queue_size };
     *
     * void
     * consumer(void)
     * {
     *   // Do something
     *   msg_t msg;
     *   for (; some_condition();)
     *     {
     *       mq.receive(&msg);
     *       // Process message
     *       if (msg.id == 7)
     *         {
     *           // Something special
     *         }
     *     }
     *   // Do something else.
     * }
     *
     * void
     * producer(void)
     * {
     *   // Do something
     *   msg_t msg;
     *   msg.id = 7;
     *   mq.send(&msg);
     *   // Do something else.
     * }
     * @endcode
     */

    // ------------------------------------------------------------------------
    /**
     * @class message_queue_inclusive
     * @details
     * If the queue size is known at compile time and the queue is used
     * for the entire application life cycle, it might be preferred to allocate
     * the queue storage statically inside the queue instance.
     *
     * For convenience, `message_queue_inclusive` are also typed, so
     * the message size is handled automatically.
     *
     * @par Example
     *
     * @code{.cpp}
     * // Define the message type.
     * typedef struct {
     *   uint32_t id;
     * } msg_t;
     *
     * // Define the queue size.
     * constexpr uint32_t queue_size = 5;
     *
     * // The queue storage is allocated statically inside this instance.
     * message_queue_inclusive<msg_t, queue_size> mq;
     *
     * void
     * consumer(void)
     * {
     *   // Do something
     *   msg_t msg;
     *   for (; some_condition();)
     *     {
     *       mq.receive(&msg);
     *       // Process message
     *       if (msg.id == 7)
     *         {
     *           // Something special
     *         }
     *     }
     *   // Do something else.
     * }
     *
     * void
     * producer(void)
     * {
     *   // Do something
     *   msg_t msg;
     *   msg.id = 7;
     *   mq.send(&msg);
     *   // Do something else.
     * }
     * @endcode
     */

    // ------------------------------------------------------------------------
    /**
     * @cond ignore
     */

    // Protected internal constructor.
    message_queue::message_queue ()
    {
#if defined(OS_TRACE_RTOS_MQUEUE)
      trace::printf ("%s() @%p %s\n", __func__, this, this->name ());
#endif
    }

    message_queue::message_queue (const char* name) :
        object_named_system
          { name }
    {
#if defined(OS_TRACE_RTOS_MQUEUE)
      trace::printf ("%s() @%p %s\n", __func__, this, this->name ());
#endif
    }

    /**
     * @endcond
     */

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
    message_queue::message_queue (std::size_t msgs, std::size_t msg_size_bytes,
                                  const attributes& attr,
                                  const allocator_type& allocator) :
        message_queue
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
    message_queue::message_queue (const char* name, std::size_t msgs,
                                  std::size_t msg_size_bytes,
                                  const attributes& attr,
                                  const allocator_type& allocator) :
        object_named_system
          { name }
    {
#if defined(OS_TRACE_RTOS_MQUEUE)
      trace::printf ("%s() @%p %s %u %u\n", __func__, this, this->name (), msgs,
                     msg_size_bytes);
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
     * This destructor shall destroy the message queue object; the object
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
    message_queue::~message_queue ()
    {
#if defined(OS_TRACE_RTOS_MQUEUE)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

#if !defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE)

      assert(send_list_.empty ());
      assert(receive_list_.empty ());

#endif

      if (allocated_queue_addr_ != nullptr)
        {
          typedef typename std::allocator_traits<allocator_type>::pointer pointer;

          static_cast<allocator_type*> (const_cast<void*> (allocator_))->deallocate (
              reinterpret_cast<pointer> (allocated_queue_addr_),
              allocated_queue_size_elements_);
        }

#if defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE)

      port::message_queue::destroy (this);

#endif
    }

    /**
     * @cond ignore
     */

    void
    message_queue::internal_construct_ (std::size_t msgs,
                                        std::size_t msg_size_bytes,
                                        const attributes& attr,
                                        void* queue_address,
                                        std::size_t queue_size_bytes)
    {
      os_assert_throw(!interrupts::in_handler_mode (), EPERM);

#if !defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE)
      clock_ = attr.clock != nullptr ? attr.clock : &sysclock;
#endif
      msg_size_bytes_ = static_cast<message_queue::msg_size_t> (msg_size_bytes);
      assert(msg_size_bytes_ == msg_size_bytes);
      assert(msg_size_bytes_ > 0);

      msgs_ = static_cast<message_queue::size_t> (msgs);
      assert(msgs_ == msgs);
      assert(msgs > 0);

      // If the storage is given explicitly, override attributes.
      if (queue_address != nullptr)
        {
          // The attributes should not define any storage in this case.
          assert(attr.mq_queue_address == nullptr);

          queue_addr_ = queue_address;
          queue_size_bytes_ = queue_size_bytes;
        }
      else
        {
          queue_addr_ = attr.mq_queue_address;
          queue_size_bytes_ = attr.mq_queue_size_bytes;
        }

#if defined(OS_TRACE_RTOS_MQUEUE)
      trace::printf ("%s() @%p %s %u %u %p %u\n", __func__, this, name (),
                     msgs_, msg_size_bytes_, queue_addr_, queue_size_bytes_);
#endif

#if !defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE)
      std::size_t storage_size = compute_allocated_size_bytes<void*> (
          msgs, msg_size_bytes);
#endif
      if (queue_addr_ != nullptr)
        {
          os_assert_throw(queue_size_bytes_ > 0, EINVAL);
#if defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE)
          os_assert_throw(
              queue_size_bytes_ >= (std::size_t) (msgs * msg_size_bytes),
              EINVAL);
#else
          os_assert_throw(queue_size_bytes_ >= storage_size, EINVAL);
#endif
        }

#if defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE)

      count_ = 0;
      port::message_queue::create (this);

#else

      head_ = no_index;

      os_assert_throw(queue_addr_ != nullptr, ENOMEM);

      // The array of prev indexes follows immediately after the content array.
      prev_array_ =
          reinterpret_cast<index_t*> (static_cast<char*> (queue_addr_)
              + msgs
                  * ((msg_size_bytes + (sizeof(void*) - 1))
                      & ~(sizeof(void*) - 1)));
      // The array of next indexes follows immediately the prev array.
      next_array_ =
          reinterpret_cast<index_t*> (reinterpret_cast<char*> (const_cast<index_t*> (prev_array_))
              + msgs * sizeof(index_t));
      // The array of priorities follows immediately the next array.
      prio_array_ =
          reinterpret_cast<priority_t*> (reinterpret_cast<char*> (const_cast<index_t*> (next_array_))
              + msgs * sizeof(index_t));

#if !defined(NDEBUG)
      char* p =
          reinterpret_cast<char*> (reinterpret_cast<char*> (const_cast<priority_t*> (prio_array_))
              + msgs * sizeof(priority_t));

      assert(
          p - static_cast<char*> (queue_addr_)
              <= static_cast<ptrdiff_t> (queue_size_bytes_));
#endif

      internal_init_ ();
#endif
    }

    void
    message_queue::internal_init_ (void)
    {
      count_ = 0;

#if !defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE)

      // Construct a linked list of blocks. Store the pointer at
      // the beginning of each block. Each block
      // will hold the address of the next free block,
      // or `nullptr` at the end.
      char* p = static_cast<char*> (queue_addr_);
      for (std::size_t i = 1; i < msgs_; ++i)
        {
          // Compute the address of the next block;
          char* pn = p + msg_size_bytes_;

          // Make this block point to the next one.
          *(static_cast<void**> (static_cast<void*> (p))) = pn;
          // Advance pointer
          p = pn;
        }

      // Mark end of list.
      *(static_cast<void**> (static_cast<void*> (p))) = nullptr;

      first_free_ = queue_addr_; // Pointer to first block.

      head_ = no_index;

      // Need not be inside the critical section,
      // the lists are protected by inner `resume_one()`.

      // Wake-up all threads, if any.
      send_list_.resume_all ();
      receive_list_.resume_all ();

#endif /* !defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE) */

    }

#if !defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE)

    /*
     * Internal function.
     * Should be called from an interrupts critical section.
     */
    bool
    message_queue::internal_try_send_ (const void* msg, std::size_t nbytes,
                                       priority_t mprio)
    {
      if (first_free_ == nullptr)
        {
          // No available space to send the message.
          return false;
        }

      // The first step is to remove the free block from the list,
      // so another concurrent call will not get it too.

      // Get the address where the message will be copied.
      // This is the first free memory block.
      char* dest = static_cast<char*> (first_free_);

      // Update to next free, if any (the last one has nullptr).
      first_free_ = *(static_cast<void**> (first_free_));

      // The second step is to copy the message from the user buffer.
        {
          // ----- Enter uncritical section -----------------------------------
          // interrupts::uncritical_section iucs;

          // Copy message from user buffer to queue storage.
          std::memcpy (dest, msg, nbytes);
          if (nbytes < msg_size_bytes_)
            {
              // Fill in the remaining space with 0x00.
              std::memset (dest + nbytes, 0x00, msg_size_bytes_ - nbytes);
            }
          // ----- Exit uncritical section ------------------------------------
        }

      // The third step is to link the buffer to the list.

      // Using the address, compute the index in the array.
      std::size_t msg_ix = (static_cast<std::size_t> (dest
          - static_cast<char*> (queue_addr_)) / msg_size_bytes_);
      prio_array_[msg_ix] = mprio;

      if (head_ == no_index)
        {
          // No other message in the queue, enlist this one
          // as head, with links to itself.
          head_ = static_cast<index_t> (msg_ix);
          prev_array_[msg_ix] = static_cast<index_t> (msg_ix);
          next_array_[msg_ix] = static_cast<index_t> (msg_ix);
        }
      else
        {
          std::size_t ix;
          // Arrange to insert between head and tail.
          ix = prev_array_[head_];
          // Check if the priority is higher than the head priority.
          if (mprio > prio_array_[head_])
            {
              // Having the highest priority, the new message
              // becomes the new head.
              head_ = static_cast<index_t> (msg_ix);
            }
          else
            {
              // If not higher than the head, try to insert at the tail,
              // but advance up until the same priority is found.
              while ((mprio > prio_array_[ix]))
                {
                  ix = prev_array_[ix];
                }
            }
          prev_array_[msg_ix] = static_cast<index_t> (ix);
          next_array_[msg_ix] = next_array_[ix];

          // Break the chain and insert the new index.
          std::size_t tmp_ix = next_array_[ix];
          next_array_[ix] = static_cast<index_t> (msg_ix);
          prev_array_[tmp_ix] = static_cast<index_t> (msg_ix);
        }

      // One more message added to the queue.
      ++count_;

      // Wake-up one thread, if any.
      receive_list_.resume_one ();

      return true;
    }

#endif /* !defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE) */

#if !defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE)

    /*
     * Internal function.
     * Should be called from an interrupts critical section.
     */
    bool
    message_queue::internal_try_receive_ (void* msg, std::size_t nbytes,
                                          priority_t* mprio)
    {

      if (head_ == no_index)
        {
          return false;
        }

      // Compute the message source address.
      char* src = static_cast<char*> (queue_addr_) + head_ * msg_size_bytes_;
      priority_t prio = prio_array_[head_];

#if defined(OS_TRACE_RTOS_MQUEUE_)
      trace::printf ("%s(%p,%u) @%p %s src %p %p\n", __func__, msg, nbytes,
          this, name (), src, first_free_);
#endif

      // Unlink it from the list, so another concurrent call will
      // not get it too.
      if (count_ > 1)
        {
          // Remove the current element from the list.
          prev_array_[next_array_[head_]] = prev_array_[head_];
          next_array_[prev_array_[head_]] = next_array_[head_];

          // Next becomes the new head.
          head_ = next_array_[head_];
        }
      else
        {
          // If there was only one, the list is empty now.
          head_ = no_index;
        }

      --count_;

      // Copy to destination
        {
          // ----- Enter uncritical section -----------------------------------
          interrupts::uncritical_section iucs;

          // Copy message from queue to user buffer.
          memcpy (msg, src, nbytes);
          if (mprio != nullptr)
            {
              *mprio = prio;
            }
          // ----- Exit uncritical section ------------------------------------
        }

      // After the message was copied, the block can be released.

      // Perform a push_front() on the single linked LIFO list,
      // i.e. add the block to the beginning of the list.

      // Link previous list to this block; may be null, but it does
      // not matter.
      *(static_cast<void**> (static_cast<void*> (src))) = first_free_;

      // Now this block is the first one.
      first_free_ = src;

      // Wake-up one thread, if any.
      send_list_.resume_one ();

      return true;
    }

#endif /* !defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE) */

    /**
     * @endcond
     */

    /**
     * @details
     * The `send()` function shall add the message
     * pointed to by the argument
     * _msg_ to the message queue. The _nbytes_ argument specifies the length
     * of the message, in bytes, pointed to by _msg_. The value of _nbytes_
     * shall be less than or equal to the _msg_size_bytes_ parameter of the
     * message queue object, or `send()` shall fail.
     *
     * If the specified message queue is not full, `send()`
     * shall behave
     * as if the message is inserted into the message queue at the
     * position indicated by the _mprio_ argument. A message with a
     * larger numeric value of _mprio_ shall be inserted before messages
     * with lower values of _mprio_. A message shall be inserted after
     * other messages in the queue, if any, with equal _mprio_. The
     * value of _mprio_ shall be less than `message_queue::max_priority`.
     *
     * If the specified message queue is full, `send()`
     * shall block
     * until space becomes available to enqueue the message, or
     * until `send()` is cancelled/interrupted.
     * If more than one
     * thread is waiting to send when space becomes available
     * in the message queue and the Priority Scheduling option is
     * supported, then the thread of the highest priority that has
     * been waiting the longest shall be unblocked to send its
     * message. Otherwise, it is unspecified which waiting thread
     * is unblocked.
     *
     * @par POSIX compatibility
     *  Inspired by [`mq_send()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html)
     *  with `O_NONBLOCK` not set,
     *  from [<mqueue.h>](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/mqueue.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    message_queue::send (const void* msg, std::size_t nbytes, priority_t mprio)
    {
#if defined(OS_TRACE_RTOS_MQUEUE)
      trace::printf ("%s(%p,%d,%d) @%p %s\n", __func__, msg, nbytes, mprio,
                     this, name ());
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);
      os_assert_err(!scheduler::locked (), EPERM);
      os_assert_err(msg != nullptr, EINVAL);
      os_assert_err(nbytes <= msg_size_bytes_, EMSGSIZE);

#if defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE)

      return port::message_queue::send (this, msg, nbytes, mprio);

#else

        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          if (internal_try_send_ (msg, nbytes, mprio))
            {
              return result::ok;
            }
          // ----- Exit critical section --------------------------------------
        }

      thread& crt_thread = this_thread::thread ();

      // Prepare a list node pointing to the current thread.
      // Do not worry for being on stack, it is temporarily linked to the
      // list and guaranteed to be removed before this function returns.
      internal::waiting_thread_node node
        { crt_thread };

      for (;;)
        {
            {
              // ----- Enter critical section ---------------------------------
              interrupts::critical_section ics;

              if (internal_try_send_ (msg, nbytes, mprio))
                {
                  return result::ok;
                }

              // Add this thread to the message queue send waiting list.
              scheduler::internal_link_node (send_list_, node);
              // state::suspended set in above link().
              // ----- Exit critical section ----------------------------------
            }

          port::scheduler::reschedule ();

          // Remove the thread from the message queue send waiting list,
          // if not already removed by receive().
          scheduler::internal_unlink_node (node);

          if (crt_thread.interrupted ())
            {
#if defined(OS_TRACE_RTOS_MQUEUE)
              trace::printf ("%s(%p,%d,%d) EINTR @%p %s\n", __func__, msg,
                             nbytes, mprio, this, name ());
#endif
              return EINTR;
            }
        }

      /* NOTREACHED */
      return ENOTRECOVERABLE;

#endif
    }

    /**
     * @details
     * The `try_send()` function shall try to add the message
     * pointed to by the argument
     * _msg_ to the message queue. The _nbytes_ argument specifies the length
     * of the message, in bytes, pointed to by _msg_. The value of _nbytes_
     * shall be less than or equal to the _msg_size_bytes_ parameter of the
     * message queue object, or `try_send()` shall fail.
     *
     * If the message queue is not full, `try_send()`
     * shall behave
     * as if the message is inserted into the message queue at the
     * position indicated by the _mprio_ argument. A message with a
     * larger numeric value of _mprio_ shall be inserted before messages
     * with lower values of _mprio_. A message shall be inserted after
     * other messages in the queue, if any, with equal _mprio_. The
     * value of _mprio_ shall be less than `message_queue::max_priority`.
     *
     * If the message queue is full, the message shall
     * not be queued and `try_send()` shall return an error.
     *
     * @par POSIX compatibility
     *  Inspired by [`mq_send()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_send.html)
     *  with `O_NONBLOCK` set,
     *  from [<mqueue.h>](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/mqueue.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *  <br>Differences from the standard:
     *  - for consistency reasons, EWOULDBLOCK is used, instead of EAGAIN
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    result_t
    message_queue::try_send (const void* msg, std::size_t nbytes,
                             priority_t mprio)
    {
#if defined(OS_TRACE_RTOS_MQUEUE)
      trace::printf ("%s(%p,%u,%u) @%p %s\n", __func__, msg, nbytes, mprio,
                     this, name ());
#endif

      os_assert_err(msg != nullptr, EINVAL);
      os_assert_err(nbytes <= msg_size_bytes_, EMSGSIZE);

#if defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE)

      return port::message_queue::try_send (this, msg, nbytes, mprio);

#else
      assert(port::interrupts::is_priority_valid ());

        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          if (internal_try_send_ (msg, nbytes, mprio))
            {
              return result::ok;
            }
          else
            {
              return EWOULDBLOCK;
            }
          // ----- Exit critical section --------------------------------------
        }

#endif
    }

    /**
     * @details
     * The `timed_send()` function shall add the message
     * pointed to by the argument
     * _msg_ to the message queue. The _nbytes_ argument specifies the length
     * of the message, in bytes, pointed to by _msg_. The value of _nbytes_
     * shall be less than or equal to the _msg_size_bytes_ attribute of the
     * message queue object, or `timed_send()` shall fail.
     *
     * If the message queue is not full, `timed_send()` shall behave
     * as if the message is inserted into the message queue at the
     * position indicated by the _mprio_ argument. A message with a
     * larger numeric value of _mprio_ shall be inserted before messages
     * with lower values of _mprio_. A message shall be inserted after
     * other messages in the queue, if any, with equal _mprio_. The
     * value of _mprio_ shall be less than `message_queue::max_priority`.
     *
     * If the message queue is full, the wait for sufficient
     * room in the queue shall be terminated when the specified timeout
     * expires.
     *
     * The timeout shall expire after the number of time units (that
     * is when the value of that clock equals or exceeds (now()+timeout).
     * The resolution of the timeout shall be the resolution of the
     * clock on which it is based.
     *
     * Under no circumstance shall the operation fail with a timeout
     * if there is sufficient room in the queue to add the message
     * immediately.
     *
     * The clock used for timeouts can be specified via the `clock`
     * attribute. By default, the clock derived from the scheduler
     * timer is used, and the durations are expressed in ticks.
     *
     * @par POSIX compatibility
     *  Inspired by [`mq_timedsend()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_timedsend.html)
     *  with `O_NONBLOCK` not set,
     *  from [<mqueue.h>](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/mqueue.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *  <br>Differences from the standard:
     *  - the timeout is not expressed as an absolute time point, but
     * as a relative number of timer ticks (by default, the SysTick
     * clock for CMSIS).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    message_queue::timed_send (const void* msg, std::size_t nbytes,
                               clock::duration_t timeout, priority_t mprio)
    {
#if defined(OS_TRACE_RTOS_MQUEUE)
      trace::printf ("%s(%p,%u,%u,%u) @%p %s\n", __func__, msg, nbytes, mprio,
                     timeout, this, name ());
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);
      os_assert_err(!scheduler::locked (), EPERM);
      os_assert_err(msg != nullptr, EINVAL);
      os_assert_err(nbytes <= msg_size_bytes_, EMSGSIZE);

#if defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE)

      return port::message_queue::timed_send (this, msg, nbytes, timeout, mprio);

#else

      // Extra test before entering the loop, with its inherent weight.
      // Trade size for speed.
        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          if (internal_try_send_ (msg, nbytes, mprio))
            {
              return result::ok;
            }
          // ----- Exit critical section --------------------------------------
        }

      thread& crt_thread = this_thread::thread ();

      // Prepare a list node pointing to the current thread.
      // Do not worry for being on stack, it is temporarily linked to the
      // list and guaranteed to be removed before this function returns.
      internal::waiting_thread_node node
        { crt_thread };

      internal::clock_timestamps_list& clock_list = clock_->steady_list ();

      clock::timestamp_t timeout_timestamp = clock_->steady_now () + timeout;

      // Prepare a timeout node pointing to the current thread.
      internal::timeout_thread_node timeout_node
        { timeout_timestamp, crt_thread };

      for (;;)
        {
            {
              // ----- Enter critical section ---------------------------------
              interrupts::critical_section ics;

              if (internal_try_send_ (msg, nbytes, mprio))
                {
                  return result::ok;
                }

              // Add this thread to the semaphore waiting list,
              // and the clock timeout list.
              scheduler::internal_link_node (send_list_, node, clock_list,
                                             timeout_node);
              // state::suspended set in above link().
              // ----- Exit critical section ----------------------------------
            }

          port::scheduler::reschedule ();

          // Remove the thread from the message queue send waiting list,
          // if not already removed by receive() and from the clock timeout list,
          // if not already removed by the timer.
          scheduler::internal_unlink_node (node, timeout_node);

          if (crt_thread.interrupted ())
            {
#if defined(OS_TRACE_RTOS_MQUEUE)
              trace::printf ("%s(%p,%u,%u,%u) EINTR @%p %s\n", __func__, msg,
                             nbytes, mprio, timeout, this, name ());
#endif
              return EINTR;
            }

          if (clock_->steady_now () >= timeout_timestamp)
            {
#if defined(OS_TRACE_RTOS_MQUEUE)
              trace::printf ("%s(%p,%u,%u,%u) ETIMEDOUT @%p %s\n", __func__,
                             msg, nbytes, mprio, timeout, this, name ());
#endif
              return ETIMEDOUT;
            }
        }

      /* NOTREACHED */
      return ENOTRECOVERABLE;

#endif
    }

    /**
     * @details
     * The `receive()` function shall receive the oldest
     * of the highest
     * priority message(s) from the message queue. If the size of the
     * buffer in bytes, specified by the _nbytes_ argument, is less than
     * the _msg_size_bytes_ attribute of the message queue, the function
     * shall fail and return an error. Otherwise, the selected message
     * shall be removed from the queue and copied to the buffer pointed
     * to by the _msg_ argument.
     *
     * If the value of _nbytes_ is greater than `message_queue::max_size`,
     * the result is implementation-defined.
     *
     * If the argument _mprio_ is not nullptr, the priority of the selected
     * message shall be stored in the location referenced by _mprio_.
     *
     * If the message queue is empty, `receive()` shall block
     * until a message is enqueued on the message queue or until
     * `receive()` is cancelled/interrupted. If more than one thread
     * is waiting to receive a message when a message arrives at
     * an empty queue and the Priority Scheduling option is supported,
     * then the thread of highest priority that has been waiting the
     * longest shall be selected to receive the message. Otherwise,
     * it is unspecified which waiting thread receives the message.
     *
     * @par POSIX compatibility
     *  Inspired by [`mq_receive()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html)
     *  with `O_NONBLOCK` not set,
     *  from [<mqueue.h>](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/mqueue.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    message_queue::receive (void* msg, std::size_t nbytes, priority_t* mprio)
    {
#if defined(OS_TRACE_RTOS_MQUEUE)
      trace::printf ("%s(%p,%u) @%p %s\n", __func__, msg, nbytes, this,
                     name ());
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);
      os_assert_err(!scheduler::locked (), EPERM);
      os_assert_err(msg != nullptr, EINVAL);
      os_assert_err(nbytes <= msg_size_bytes_, EMSGSIZE);
      os_assert_err(nbytes <= max_size, EMSGSIZE);

#if defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE)

      return port::message_queue::receive (this, msg, nbytes, mprio);

#else

      // Extra test before entering the loop, with its inherent weight.
      // Trade size for speed.
        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          if (internal_try_receive_ (msg, nbytes, mprio))
            {
              return result::ok;
            }
          // ----- Exit critical section --------------------------------------
        }

      thread& crt_thread = this_thread::thread ();

      // Prepare a list node pointing to the current thread.
      // Do not worry for being on stack, it is temporarily linked to the
      // list and guaranteed to be removed before this function returns.
      internal::waiting_thread_node node
        { crt_thread };

      for (;;)
        {
            {
              // ----- Enter critical section ---------------------------------
              interrupts::critical_section ics;

              if (internal_try_receive_ (msg, nbytes, mprio))
                {
                  return result::ok;
                }

              // Add this thread to the message queue receive waiting list.
              scheduler::internal_link_node (receive_list_, node);
              // state::suspended set in above link().
              // ----- Exit critical section ----------------------------------
            }

          port::scheduler::reschedule ();

          // Remove the thread from the message queue receive waiting list,
          // if not already removed by send().
          scheduler::internal_unlink_node (node);

          if (crt_thread.interrupted ())
            {
#if defined(OS_TRACE_RTOS_MQUEUE)
              trace::printf ("%s(%p,%u) EINTR @%p %s\n", __func__, msg, nbytes,
                             this, name ());
#endif
              return EINTR;
            }
        }

      /* NOTREACHED */
      return ENOTRECOVERABLE;

#endif
    }

    /**
     * @details
     * The `try_receive()` function shall try to receive the
     * oldest of the highest
     * priority message(s) from the message queue. If the size of the
     * buffer in bytes, specified by the nbytes argument, is less than
     * the _msg_size_bytes_ attribute of the message queue, the function
     * shall fail and return an error. Otherwise, the selected message
     * shall be removed from the queue and copied to the buffer pointed
     * to by the _msg_ argument.
     *
     * If the value of _nbytes_ is greater than `message_queue::max_size`,
     * the result is implementation-defined.
     *
     * If the argument _mprio_ is not nullptr, the priority of the selected
     * message shall be stored in the location referenced by _mprio_.
     *
     * If the message queue is empty, no message shall be removed
     * from the queue, and `try_receive()` shall return an error.
     *
     * @par POSIX compatibility
     *  Inspired by [`mq_receive()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html)
     *  with `O_NONBLOCK` set,
     *  from [<mqueue.h>](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/mqueue.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *  <br>Differences from the standard:
     *  - for consistency reasons, EWOULDBLOCK is used, instead of EAGAIN
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    result_t
    message_queue::try_receive (void* msg, std::size_t nbytes,
                                priority_t* mprio)
    {
#if defined(OS_TRACE_RTOS_MQUEUE)
      trace::printf ("%s(%p,%u) @%p %s\n", __func__, msg, nbytes, this,
                     name ());
#endif

      os_assert_err(msg != nullptr, EINVAL);
      os_assert_err(nbytes <= msg_size_bytes_, EMSGSIZE);
      os_assert_err(nbytes <= max_size, EMSGSIZE);

#if defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE)

      return port::message_queue::try_receive (this, msg, nbytes, mprio);

#else

      assert(port::interrupts::is_priority_valid ());

        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          if (internal_try_receive_ (msg, nbytes, mprio))
            {
              return result::ok;
            }
          else
            {
              return EWOULDBLOCK;
            }
          // ----- Exit critical section --------------------------------------
        }

#endif
    }

    /**
     * @details
     * The `timed_receive()` function shall receive the
     * oldest of the highest
     * priority message(s) from the message queue. If the size of the
     * buffer in bytes, specified by the _nbytes_ argument, is less than
     * the _msg_size_bytes_ attribute of the message queue, the function
     * shall fail and return an error. Otherwise, the selected message
     * shall be removed from the queue and copied to the buffer pointed
     * to by the _msg_ argument.
     *
     * If the value of _nbytes_ is greater than `message_queue::max_size`,
     * the result is implementation-defined.
     *
     * If the argument _mprio_ is not nullptr, the priority of the selected
     * message shall be stored in the location referenced by _mprio_.
     *
     * If the message queue is empty, `timed_receive()` shall block
     * until a message is enqueued on the message queue or until
     * `timed_receive()` is cancelled/interrupted. If more than one thread
     * is waiting to receive a message when a message arrives at
     * an empty queue and the Priority Scheduling option is supported,
     * then the thread of highest priority that has been waiting the
     * longest shall be selected to receive the message. Otherwise,
     * it is unspecified which waiting thread receives the message.
     *
     * The `timed_receive()` function shall receive the oldest of
     * the highest priority messages from the message queue as described
     * for the `receive()` function. However, if no message exists on the
     * queue to satisfy the receive, the wait for such a message shall
     * be terminated when the specified timeout expires.
     *
     * The timeout shall expire after the number of time units (that
     * is when the value of that clock equals or exceeds (now()+duration).
     * The resolution of the timeout shall be the resolution of the
     * clock on which it is based.
     *
     * Under no circumstance shall the operation fail with a timeout
     * if a message can be removed from the message queue immediately.
     *
     * The clock used for timeouts can be specified via the `clock`
     * attribute. By default, the clock derived from the scheduler
     * timer is used, and the durations are expressed in ticks.
     *
     * Compatible with POSIX `mq_receive()` with O_NONBLOCK set.
     * http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_receive.html#
     *
     * @par POSIX compatibility
     *  Inspired by [`mq_timedreceive()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/mq_timedreceive.html)
     *  with `O_NONBLOCK` not set,
     *  from [<mqueue.h>](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/mqueue.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *  <br>Differences from the standard:
     *  - the timeout is not expressed as an absolute time point, but
     * as a relative number of timer ticks (by default, the SysTick
     * clock for CMSIS).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    message_queue::timed_receive (void* msg, std::size_t nbytes,
                                  clock::duration_t timeout, priority_t* mprio)
    {
#if defined(OS_TRACE_RTOS_MQUEUE)
      trace::printf ("%s(%p,%u,%u) @%p %s\n", __func__, msg, nbytes, timeout,
                     this, name ());
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);
      os_assert_err(!scheduler::locked (), EPERM);
      os_assert_err(msg != nullptr, EINVAL);
      os_assert_err(nbytes <= msg_size_bytes_, EMSGSIZE);
      os_assert_err(nbytes <= max_size, EMSGSIZE);

#if defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE)

      return port::message_queue::timed_receive (this, msg, nbytes,
          timeout, mprio);

#else

      // Extra test before entering the loop, with its inherent weight.
      // Trade size for speed.
        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          if (internal_try_receive_ (msg, nbytes, mprio))
            {
              return result::ok;
            }
          // ----- Exit critical section --------------------------------------
        }

      thread& crt_thread = this_thread::thread ();

      // Prepare a list node pointing to the current thread.
      // Do not worry for being on stack, it is temporarily linked to the
      // list and guaranteed to be removed before this function returns.
      internal::waiting_thread_node node
        { crt_thread };

      internal::clock_timestamps_list& clock_list = clock_->steady_list ();
      clock::timestamp_t timeout_timestamp = clock_->steady_now () + timeout;

      // Prepare a timeout node pointing to the current thread.
      internal::timeout_thread_node timeout_node
        { timeout_timestamp, crt_thread };

      for (;;)
        {
            {
              // ----- Enter critical section ---------------------------------
              interrupts::critical_section ics;

              if (internal_try_receive_ (msg, nbytes, mprio))
                {
                  return result::ok;
                }

              // Add this thread to the message queue receive waiting list,
              // and the clock timeout list.
              scheduler::internal_link_node (receive_list_, node, clock_list,
                                             timeout_node);
              // state::suspended set in above link().
              // ----- Exit critical section ----------------------------------
            }

          port::scheduler::reschedule ();

          // Remove the thread from the semaphore waiting list,
          // if not already removed by send()and from the clock
          // timeout list, if not already removed by the timer.
          scheduler::internal_unlink_node (node, timeout_node);

          if (crt_thread.interrupted ())
            {
#if defined(OS_TRACE_RTOS_MQUEUE)
              trace::printf ("%s(%p,%u,%u) EINTR @%p %s\n", __func__, msg,
                             nbytes, timeout, this, name ());
#endif
              return EINTR;
            }

          if (clock_->steady_now () >= timeout_timestamp)
            {
#if defined(OS_TRACE_RTOS_MQUEUE)
              trace::printf ("%s(%p,%u,%u) ETIMEDOUT @%p %s\n", __func__, msg,
                             nbytes, timeout, this, name ());
#endif
              return ETIMEDOUT;
            }
        }

      /* NOTREACHED */
      return ENOTRECOVERABLE;

#endif
    }

    /**
     * @details
     * Clear both send and receive counter and return the queue to the
     * initial state.
     *
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    message_queue::reset (void)
    {
#if defined(OS_TRACE_RTOS_MQUEUE)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);

#if defined(OS_USE_RTOS_PORT_MESSAGE_QUEUE)

      return port::message_queue::reset (this);

#else

        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          internal_init_ ();
          return result::ok;
          // ----- Exit critical section --------------------------------------
        }

#endif
    }

  // --------------------------------------------------------------------------

  } /* namespace rtos */
} /* namespace os */
