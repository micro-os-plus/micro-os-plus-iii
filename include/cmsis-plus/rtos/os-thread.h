/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 * Copyright (c) 2013 ARM LIMITED
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

#ifndef CMSIS_PLUS_RTOS_OS_THREAD_H_
#define CMSIS_PLUS_RTOS_OS_THREAD_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

#include <cmsis-plus/rtos/os-decls.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    namespace this_thread
    {

      // ========================================================================

      /**
       * @brief Get the current running thread.
       * @par Parameters
       *  None
       * @return Reference to the current running thread.
       */
      Thread&
      thread (void);

      /**
       * @brief Yield execution to the next thread ready thread.
       * @par Parameters
       *  None
       * @return Nothing.
       */
      void
      yield (void);

      /**
       * @brief Suspend the current running thread.
       * @par Parameters
       *  None
       * @return Nothing.
       */
      void
      sleep (void);

      /**
       * @brief Terminate the current running thread.
       * @param [in] exit_ptr Pointer to object to return. (Optional).
       * @return Nothing.
       */
      void
      exit (void* exit_ptr = nullptr);

      /**
       * @brief Check if the wake-up is due to a timeout.
       * @par Parameters
       *  None
       * @retval true The previous sleep returned after the entire duration.
       * @retval false The previous sleep returned due to an event.
       */
      bool
      is_timeout (void);

      /**
       * @brief Wait for signal flags.
       * @param [in] mask The expected flags (OR-ed bit-mask);
       *  may be zero.
       * @param [out] oflags Pointer where to store the current flags;
       *  may be `nullptr`.
       * @param [in] mode Mode bits to select if either all or any flags
       *  are expected, and if the flags should be cleared.
       * @retval result::ok All expected flags are raised.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EINVAL The mask is outside of the permitted range.
       * @retval EINTR The operation was interrupted.
       * @retval ENOTRECOVERABLE Wait failed.
       */
      result_t
      sig_wait (thread::sigset_t mask, thread::sigset_t* oflags = nullptr,
                flags::mode_t mode = flags::mode::all | flags::mode::clear);

      /**
       * @brief Try to wait for signal flags.
       * @param [in] mask The expected flags (OR-ed bit-mask);
       *  may be zero.
       * @param [out] oflags Pointer where to store the current flags;
       *  may be `nullptr`.
       * @param [in] mode Mode bits to select if either all or any flags
       *  are expected, and if the flags should be cleared.
       * @retval result::ok All expected flags are raised.
       * @retval EINVAL The mask is outside of the permitted range.
       * @retval EAGAIN The expected condition did not occur.
       * @retval ENOTRECOVERABLE Wait failed.
       */
      result_t
      try_sig_wait (thread::sigset_t mask, thread::sigset_t* oflags = nullptr,
                    flags::mode_t mode = flags::mode::all | flags::mode::clear);

      /**
       * @brief Timed wait for signal flags.
       * @param [in] mask The expected flags (OR-ed bit-mask);
       *  may be zero.
       * @param [out] oflags Pointer where to store the current flags;
       *  may be `nullptr`.
       * @param [in] mode Mode bits to select if either all or any flags
       *  are expected, and if the flags should be cleared.
       * @param [in] timeout Timeout to wait, in clock units (ticks or seconds).
       * @retval result::ok All expected flags are raised.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval ETIMEDOUT The expected condition did not occur during the
       *  entire timeout duration.
       * @retval EINVAL The mask is outside of the permitted range.
       * @retval EINTR The operation was interrupted.
       * @retval ENOTRECOVERABLE Wait failed.
       */
      result_t
      timed_sig_wait (
          thread::sigset_t mask, clock::duration_t timeout,
          thread::sigset_t* oflags = nullptr,
          flags::mode_t mode = flags::mode::all | flags::mode::clear);

    } /* namespace this_thread */

    // ========================================================================

    namespace thread
    {

      // ======================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

      /**
       * @brief %Thread attributes.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class Attributes : public Named_object
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Create thread attributes.
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
         * @brief Destroy thread attributes.
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
         * @brief Thread user stack address attribute.
         */
        void* th_stack_address;

        /**
         * @brief Thread user stack size attribute.
         */
        std::size_t th_stack_size_bytes;

        /**
         * @brief Thread priority attribute.
         */
        priority_t th_priority;

        // Add more attributes.

        /**
         * @}
         */

      };

#pragma GCC diagnostic pop

      /**
       * @brief Default thread initialiser.
       */
      extern const Attributes initializer;
    }

#if !defined(OS_INCLUDE_RTOS_CUSTOM_THREAD_USER_STORAGE)
    /**
     * @brief Default empty thread user storage.
     *
     * If the application requires to store some additional data
     * to each thread, redefine this structure in the `<os-app-conf.h>`
     * file.
     *
     * To get the address of the user storage associated with a
     * given thread, use @ref Thread::user_storage().
     */
    typedef struct
      {
        ;
      }os_thread_user_storage_t;
#endif

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @brief POSIX compliant **thread**.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos
     */
    class Thread : public Named_object
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a thread with default settings.
       * @param [in] function Pointer to thread function.
       * @param [in] args Pointer to arguments.
       */
      Thread (thread::func_t function, thread::func_args_t args);

      /**
       * @brief Create a thread with custom settings.
       * @param [in] attr Reference to attributes.
       * @param [in] function Pointer to thread function.
       * @param [in] args Pointer to arguments.
       */
      Thread (const thread::Attributes& attr, thread::func_t function,
              thread::func_args_t args);

      /**
       * @cond ignore
       */
      Thread (const Thread&) = delete;
      Thread (Thread&&) = delete;
      Thread&
      operator= (const Thread&) = delete;
      Thread&
      operator= (Thread&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the thread.
       */
      ~Thread ();

      /**
       * @}
       * @name Operators
       * @{
       */

      /**
       * @brief Compare threads.
       * @retval true The given thread is the same as this thread.
       * @retval false The threads are different.
       */
      bool
      operator== (const Thread& rhs) const;

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Cancel thread execution.
       * @par Parameters
       *  None.
       * @retval result::ok The cancel request was sent to the thread.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       */
      result_t
      cancel (void);

      /**
       * @brief Wait for thread termination.
       * @retval result::ok The thread was terminated.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       */
      result_t
      join (void** exit_ptr = nullptr);

      /**
       * @brief Detach a thread.
       * @par Parameters
       *  None
       * @retval result::ok The thread was detached.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       */
      result_t
      detach (void);

      // Accessors & mutators.

      /**
       * @brief Set dynamic scheduling priority.
       * @retval result::ok The priority was set.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EINVAL The value of prio is invalid for the
       *  scheduling policy of the specified thread.
       */
      result_t
      sched_prio (thread::priority_t prio);

      /**
       * @brief Get the current scheduling priority.
       * @par Parameters
       *  None.
       * @return The thread priority.
       */
      thread::priority_t
      sched_prio (void);

#if 0
      // ???
      result_t
      set_cancel_state (int, int*);
      result_t
      set_cancel_type (int, int*);

      result_t
      get_sched_param (int*, struct sched_param*);
      result_t
      set_sched_param (int, const struct sched_param*);

      //void test_cancel(void);
#endif

      // TODO: study how to integrate signals and POSIX cancellation.
      /**
       * @brief Check if interrupted.
       * @par Parameters
       *  None
       * @retval true The thread was interrupted.
       * @retval false The thread was not interrupted.
       */
      bool
      interrupted (void);

      /**
       * @brief Get scheduler status of this thread.
       * @par Parameters
       *  None
       * @return Thread scheduler state.
       */
      thread::state_t
      sched_state (void) const;

      /**
       * @brief Wake-up the thread.
       * @par Parameters
       *  None
       * @return  Nothing.
       *
       * @note Can be invoked from Interrupt Service Routines.
       */
      void
      wakeup (void);

      /**
       * @brief Get the thread function arguments.
       * @par Parameters
       *  None.
       * @return Pointer to arguments.
       */
      void*
      function_args (void) const;

#if 0
      // Maybe make it a structure.
      result_t
      wakeup_reason (void) const;
#endif

      /**
       * @brief Get user storage.
       * @par Parameters
       *  None
       * @return The address of the thread user storage.
       */
      os_thread_user_storage_t*
      user_storage (void);

      /**
       * @brief Raise thread signal flags.
       * @param [in] mask The OR-ed flags to raise.
       * @param [out] oflags Optional pointer where to store the
       *  previous flags; may be `nullptr`.
       * @retval result::ok The flags were raised.
       * @retval EINVAL The mask is zero.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       */
      result_t
      sig_raise (thread::sigset_t mask, thread::sigset_t* oflags);

      /**
       * @brief Clear thread signal flags.
       * @param [out] oflags Optional pointer where to store the
       *  previous flags; may be `nullptr`.
       * @param [in] mask The OR-ed flags to clear.
       * @retval result::ok The flags were cleared.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EINVAL The mask is zero.
       */
      result_t
      sig_clear (thread::sigset_t mask, thread::sigset_t* oflags);

      /**
       * @brief Get/clear thread signal flags.
       * @param [in] mask The OR-ed flags to get/clear; may be zero.
       * @param [in] mode Mode bits to select if the flags should be
       *  cleared (the other bits are ignored).
       * @retval flags The selected bits from the current thread
       *  signal flags mask.
       * @retval sig::all Cannot be invoked from an Interrupt Service Routines.
       */
      thread::sigset_t
      sig_get (thread::sigset_t mask, flags::mode_t mode);

      /**
       * @brief Force thread termination.
       * @par Parameters
       *  None
       * @retval result::ok The tread was terminated.
       */
      result_t
      kill (void);

      /**
       * @}
       */

    protected:

      /**
       * @name Private Friends
       * @{
       */

      friend void
      this_thread::sleep (void);

      friend void
      this_thread::exit (void* exit_ptr);

      friend result_t
      this_thread::sig_wait (thread::sigset_t mask, thread::sigset_t* oflags,
                             flags::mode_t mode);

      friend result_t
      this_thread::try_sig_wait (thread::sigset_t mask,
                                 thread::sigset_t* oflags, flags::mode_t mode);
      friend result_t
      this_thread::timed_sig_wait (thread::sigset_t mask,
                                   clock::duration_t timeout,
                                   thread::sigset_t* oflags,
                                   flags::mode_t mode);

      friend class Waiting_threads_list;
      friend class Clock_timestamps_list;

      template<typename CS_T, typename List_T, typename Node_T>
        friend class Clock_list_guard;

      template<typename CS_T, typename List_T, typename Node_T>
        friend class Thread_list_guard;

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Functions
       * @{
       */

      /**
       * @brief Suspend this thread.
       * @par Parameters
       *  None
       * @return  Nothing.
       */
      void
      _sleep (void);

      /**
       * @brief Terminate thread by itself.
       * @param [in] exit_ptr Pointer to object to return (optional).
       * @return  Nothing.
       */
      void
      _exit (void* exit_ptr = nullptr);

      /**
       * @brief Invoke terminating thread function.
       * @param [in] thread The static `this`.
       * @return  Nothing.
       */
      static void
      _invoke_with_exit (Thread* thread);

      /**
       * @brief Wait for signal flags.
       * @param [in] mask The expected flags (OR-ed bit-mask);
       *  may be zero.
       * @param [out] oflags Pointer where to store the current flags;
       *  may be `nullptr`.
       * @param [in] mode Mode bits to select if either all or any flags
       *  are expected, and if the flags should be cleared.
       * @retval result::ok All expected flags are raised.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EINVAL The mask is outside of the permitted range.
       * @retval EINTR The operation was interrupted.
       * @retval ENOTRECOVERABLE Wait failed.
       */
      result_t
      _sig_wait (thread::sigset_t mask, thread::sigset_t* oflags,
                 flags::mode_t mode);

      /**
       * @brief Try to wait for signal flags.
       * @param [in] mask The expected flags (OR-ed bit-mask);
       *  may be zero.
       * @param [out] oflags Pointer where to store the current flags;
       *  may be `nullptr`.
       * @param [in] mode Mode bits to select if either all or any flags
       *  are expected, and if the flags should be cleared.
       * @retval result::ok All expected flags are raised.
       * @retval EINVAL The mask is outside of the permitted range.
       * @retval EAGAIN The expected condition did not occur.
       * @retval ENOTRECOVERABLE Wait failed.
       */
      result_t
      _try_sig_wait (thread::sigset_t mask, thread::sigset_t* oflags,
                     flags::mode_t mode);

      /**
       * @brief Timed wait for signal flags.
       * @param [in] mask The expected flags (OR-ed bit-mask);
       *  may be zero.
       * @param [out] oflags Pointer where to store the current flags;
       *  may be `nullptr`.
       * @param [in] mode Mode bits to select if either all or any flags
       *  are expected, and if the flags should be cleared.
       * @param [in] timeout Timeout to wait, in clock units (ticks or seconds).
       * @retval result::ok All expected flags are raised.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval ETIMEDOUT The expected condition did not occur during the
       *  entire timeout duration.
       * @retval EINVAL The mask is outside of the permitted range.
       * @retval EINTR The operation was interrupted.
       * @retval ENOTRECOVERABLE Wait failed.
       */
      result_t
      _timed_sig_wait (thread::sigset_t mask, clock::duration_t timeout,
                       thread::sigset_t* oflags, flags::mode_t mode);

      /**
       * @brief Internal wait for signal.
       * @param [in] mask The expected flags (OR-ed bit-mask);
       *  may be zero.
       * @param [out] oflags Pointer where to store the current flags;
       *  may be `nullptr`.
       * @param [in] mode Mode bits to select if either all or any flags
       *  are expected, and if the flags should be cleared.
       * @retval result::ok All expected flags are raised.
       * @retval EINVAL The mask is outside of the permitted range.
       * @retval EAGAIN The expected condition did not occur.
       * @retval ENOTRECOVERABLE Wait failed.
       */
      result_t
      _try_wait (thread::sigset_t mask, thread::sigset_t* oflags,
                 flags::mode_t mode);

      /**
       * @brief The actual destructor, also called from exit() and kill().
       * @par Parameters
       *  None
       * @return  Nothing.
       */
      void
      _destroy (void);

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Variables
       * @{
       */

      // TODO: group them in a Stack object
      void* stack_addr_;
      thread::func_t func_;
      thread::func_args_t func_args_;
      void* func_result_;

      // Implementation
#if defined(OS_INCLUDE_RTOS_PORT_THREAD)
      friend class port::Thread;
      os_thread_port_data_t port_;
#endif

      Thread* joiner_;
      Waiting_thread_node* waiting_node_;
      Timeout_thread_node* clock_node_;

      std::size_t stack_size_bytes_;
      thread::state_t sched_state_;
      thread::priority_t prio_;

      // TODO: needed?
      result_t wakeup_reason_;

      // volatile, but used in critical sections.
      thread::sigset_t sig_mask_;

      os_thread_user_storage_t user_storage_;

      // Add other internal data

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
    namespace this_thread
    {
      /**
       * @details
       * Remove the current running thread from the ready list and pass
       * control to the next thread that is in **READY** state. The
       * thread will not be automatically rescheduled, it requires
       * some other tread or interrupt service routine to add it
       * back to the READY state (via `Thread::wakeup()`).
       *
       * This is different from `yield()` which automatically
       * reschedules the current thread before passing control to
       * the next thread (which might be the same if no other
       * threads with at least the same priority are ready).
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline void
      sleep (void)
      {
        this_thread::thread ()._sleep ();
      }

      /**
       * @details
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline result_t
      sig_wait (thread::sigset_t mask, thread::sigset_t* oflags,
                flags::mode_t mode)
      {
        return this_thread::thread ()._sig_wait (mask, oflags, mode);
      }

      /**
       * @details
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline result_t
      try_sig_wait (thread::sigset_t mask, thread::sigset_t* oflags,
                    flags::mode_t mode)
      {
        return this_thread::thread ()._try_sig_wait (mask, oflags, mode);
      }

      /**
       * @details
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline result_t
      timed_sig_wait (thread::sigset_t mask, clock::duration_t timeout,
                      thread::sigset_t* oflags, flags::mode_t mode)
      {
        return this_thread::thread ()._timed_sig_wait (mask, timeout, oflags,
                                                       mode);
      }

      inline void
      exit (void* exit_ptr)
      {
        return this_thread::thread ()._exit (exit_ptr);
      }

    } /* namespace this_thread */

    namespace thread
    {
      inline
      Attributes::Attributes (const char* name) :
          Named_object (name)
      {
        th_stack_address = nullptr;
        th_stack_size_bytes = 0;
        th_priority = thread::priority::normal;
      }
    } /* namespace thread */

    /**
     * @details
     * Identical threads should have the same memory address.
     *
     * Compatible with POSIX pthread_equal().
     * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_equal.html
     */
    inline bool
    Thread::operator== (const Thread& rhs) const
    {
      return this == &rhs;
    }

    inline thread::state_t
    Thread::sched_state (void) const
    {
      return sched_state_;
    }

    inline void*
    Thread::function_args (void) const
    {
      return func_args_;
    }

#if 0
    inline result_t
    Thread::wakeup_reason (void) const
      {
        return wakeup_reason_;
      }
#endif

    /**
     * @details
     * The user storage is a custom structure defined in `<os-app-config.h>`,
     * which is added to each and every thread storage. Applications
     * can store here any data.
     *
     * Inspired by (actually a generalisation of) µC-OS III task user
     * registers and FreeRTOS thread local storage, which proved useful
     * when implementing CMSIS+ over FreeRTOS.
     */
    inline os_thread_user_storage_t*
    Thread::user_storage (void)
    {
      return &user_storage_;
    }

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_THREAD_H_ */
