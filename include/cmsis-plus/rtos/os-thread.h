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

#ifndef CMSIS_PLUS_RTOS_OS_THREAD_H_
#define CMSIS_PLUS_RTOS_OS_THREAD_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

#include <cmsis-plus/rtos/os-decls.h>

// Better be the last, to undef putchar()
#include <cmsis-plus/diag/trace.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    // ------------------------------------------------------------------------

    // Forward definitions required by thread friends.
    namespace this_thread
    {
      void
      wait (void);

      [[noreturn]] void
      exit (void* exit_ptr);

      result_t
      sig_wait (flags::mask_t mask, flags::mask_t* oflags, flags::mode_t mode);

      result_t
      try_sig_wait (flags::mask_t mask, flags::mask_t* oflags,
                    flags::mode_t mode);
      result_t
      timed_sig_wait (flags::mask_t mask, clock::duration_t timeout,
                      flags::mask_t* oflags, flags::mode_t mode);

      int*
      error (void);
    } /* namespace this_thread */

    namespace
    {
      // Anonymous definition required for the next forward definition.
      using func_args_t = void*;
    }

    // Forward definitions required by thread friends.
    namespace scheduler
    {
      void*
      _idle_func (func_args_t args);
    } /* namespace scheduler */

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @brief POSIX compliant **thread**, using the
     * default RTOS allocator.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos
     */
    class thread : public named_object
    {
    public:

      // ======================================================================

      /**
       * @brief Type of a variable holding thread priorities.
       * @details
       * A numeric type used to hold thread priorities, affecting the thread
       * behaviour, like scheduling and thread wakeup due to events;
       * usually an unsigned 8-bits type.
       *
       * Higher values represent higher priorities.
       * @ingroup cmsis-plus-rtos
       */
      using priority_t = uint8_t;

      /**
       * @brief Thread priorities.
       * @details
       * The os::rtos::thread::priority definition is a container for
       * priorities not restricted to an enumeration.
       * @ingroup cmsis-plus-rtos
       */
      struct priority
      {
        /**
         * @brief Priorities pre-scaler.
         * @details
         * Decreasing this value narrows the range of allowed
         * priorities. It is recommended to keep it low to give the
         * scheduler a chance to optimise accesses to the ready list
         * with an array of priorities, which will require some
         * pointers and counters for each priority level.
         *
         * The default value of 4 gives the full range of 256 priorities;
         * 0 gives 16 priorities,
         * 1 gives 32 priorities, 2 gives 64 priorities, 3 gives 128
         * priorities.
         * @ingroup cmsis-plus-rtos
         */
        static constexpr uint32_t range = 4;

        /**
         * @brief Main priorities, intermediate values also possible.
         * @ingroup cmsis-plus-rtos
         */
        enum
          : priority_t
            {
              /**
               * Undefined, thread not initialised.
               */
              none = 0,

              /**
               * System reserved for the IDLE thread.
               */
              idle = (1 << range),

              /**
               * Lowest available for user code.
               */
              lowest = (2 << range),

              low = (2 << range),

              below_normal = (4 << range),

              /**
               * Default priority.
               */
              normal = (6 << range),

              above_normal = (8 << range),

              high = (10 << range),

              realtime = (12 << range),

              /**
               * Highest available for user code.
               */
              highest = (((13 + 1) << range) - 1),

              /**
               * System reserved for the ISR deferred thread.
               */
              isr = (((14 + 1) << range) - 1),

              /**
               * Error.
               */
              error = (((15 + 1) << range) - 1)
        };
      }; /* struct priority */

      /**
       * @brief Type of a variable holding the thread state.
       * @details
       * An enumeration with the possible thread states. The enumeration
       * is restricted to one of these values.
       */
      enum class state
        : uint8_t
          {
            /**
             * @brief Used to catch uninitialised threads.
             */
            undefined = 0, //
        inactive = 1, //
        ready = 2, //
        running = 3, //
        waiting = 4, //
        /**
         * @brief Reuse possible if terminated or higher.
         */
        terminated = 5,      // Test for here up for reuse
        destroyed = 6
      }; /* enum class state */

      using state_t = enum state;

      /**
       * @brief Type of a variable holding a signal set.
       * @details
       * An unsigned type large enough to store all the signal flags,
       * actually a reuse of the more generic flags mask type
       * @ref flags::mask_t.
       */
      using sigset_t = flags::mask_t;

      /**
       * @brief %Thread signals namespace.
       * @details
       * The os::rtos::thread::sig namespace is a container for
       * signal flags masks, which cannot be restricted to an enumeration..
       */
      struct sig
      {
        /**
         * @brief Signal sets with special meaning.
         */
        enum
          : sigset_t
            {
              /**
               * @brief Special signal mask to represent any flag.
               */
              any = 0,

              /**
               * Special signal mask to represent all flags.
               */
              all = 0xFFFFFFFF,
        };
      }; /* struct sig */

      /**
       * @brief Thread function arguments.
       * @details
       * Type of thread function arguments.
       */
      using func_args_t = func_args_t;

      /**
       * @brief Thread function.
       * @details
       * Type of thread functions. Useful to cast other similar types
       * to silence the compiler warnings.
       */
      using func_t = void* (*) (func_args_t args);

      // ======================================================================

      class stack
      {
      public:

        /**
         * @brief Type of a stack element.
         * @details
         * The stack is organised as platform words
         * (usually 4-bytes long on Cortex-M cores).
         */
        using element_t = os::rtos::port::stack::element_t;

        /**
         * @brief Type of a stack allocation element.
         * @details
         * For alignment reasons, the stack is allocated in
         * larger chunks, usually 8-bytes long on Cortex-M cores.
         */
        using allocation_element_t = os::rtos::port::stack::allocation_element_t;

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Create a thread stack.
         */
        stack ();

        /**
         * @cond ignore
         */
        stack (const stack&) = delete;
        stack (stack&&) = delete;
        stack&
        operator= (const stack&) = delete;
        stack&
        operator= (stack&&) = delete;
        /**
         * @endcond
         */

        /**
         * @brief Destroy stack.
         */
        ~stack () = default;

        /**
         * @}
         */

      public:

        /**
         * @name Public Member Functions
         * @{
         */

        /**
         * @brief Get the stack lowest reserved address.
         * @par Parameters
         *  None
         * @return  The address of the stack reserved area.
         */
        stack::element_t*
        bottom (void);

        /**
         * @brief Get the stack size.
         * @par Parameters
         *  None
         * @return  The stack size in bytes.
         */
        std::size_t
        size (void);

        /**
         * @}
         */

      public:

        /**
         * @name Public Static Member Functions
         * @{
         */

        /**
         * @brief Get the min stack size.
         * @par Parameters
         *  None
         * @return  The min stack size in bytes.
         */
        static std::size_t
        min_size (void);

        /**
         * @brief Set the min stack size.
         * @param [in] size_bytes Minimum stack size in bytes.
         * @return Nothing.
         */
        static void
        min_size (std::size_t size_bytes);

        /**
         * @brief Get the default stack size.
         * @par Parameters
         *  None
         * @return  The default stack size in bytes.
         */
        static std::size_t
        default_size (void);

        /**
         * @brief Set the default stack size.
         * @param [in] size_bytes Default stack size in bytes.
         * @return Nothing.
         */
        static void
        default_size (std::size_t size_bytes);

        /**
         * @}
         */

      protected:

        friend class rtos::thread;

        stack::element_t* bottom_address_;
        std::size_t size_bytes_;

        static std::size_t min_size_bytes_;
        static std::size_t default_size_bytes_;

      }; /* class stack */

      // ======================================================================
      class context
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Create a thread context.
         */
        context ();

        /**
         * @cond ignore
         */
        context (const context&) = delete;
        context (context&&) = delete;
        context&
        operator= (const context&) = delete;
        context&
        operator= (context&&) = delete;
        /**
         * @endcond
         */

        /**
         * @brief Destroy context.
         */
        ~context () = default;

        /**
         * @}
         */

      public:

        /**
         * @name Public Member Functions
         * @{
         */

        thread::stack&
        stack (void);

        /**
         * @}
         */

      protected:

        /**
         * @name Private Friends
         * @{
         */

        friend class rtos::thread;
        friend class rtos::port::thread;
        friend void
        port::scheduler::start (void);
        friend void
        port::scheduler::reschedule ();

#if !defined(OS_INCLUDE_RTOS_PORT_SCHEDULER)

        friend class port::context;

        friend port::stack::element_t*
        port::scheduler::switch_stacks (port::stack::element_t* sp);

#endif
        /**
         * @}
         */

      protected:

        /**
         * @name Private Member Variables
         * @{
         */

        /**
         * @brief Stack object.
         */
        thread::stack stack_;

#if !defined(OS_INCLUDE_RTOS_PORT_SCHEDULER)

        /**
         * @brief Non-portable context data.
         */
        port::thread_context_t port_;

#endif

        /**
         * @}
         */

      }; /* class context */

      // ======================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

      /**
       * @brief %Thread attributes.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class attributes : public clocked_attributes
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
        constexpr
        attributes (const char* name);

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
         * @brief Destroy thread attributes.
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

        // Public members, no accessors and mutators required.
        // Warning: must match the type & order of the C file header.
        /**
         * @brief Thread user stack address attribute.
         */
        void* th_stack_address = 0;

        /**
         * @brief Thread user stack size attribute.
         */
        std::size_t th_stack_size_bytes = 0;

        /**
         * @brief Thread priority attribute.
         */
        priority_t th_priority = priority::normal;

        // Add more attributes.

        /**
         * @}
         */

      }; /* class attributes */

#pragma GCC diagnostic pop

      /**
       * @brief Default thread initialiser.
       */
      static const attributes initializer;

      // ======================================================================

      using Allocator = memory::allocator<stack::allocation_element_t>;

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a thread with default settings.
       * @param [in] function Pointer to thread function.
       * @param [in] args Pointer to thread function arguments.
       * @param [in] allocator Reference to allocator. Default a local temporary instance.
       */
      thread (func_t function, func_args_t args, const Allocator& allocator =
                  Allocator ());

      /**
       * @brief Create a named thread with default settings.
       * @param [in] name Pointer to name.
       * @param [in] function Pointer to thread function.
       * @param [in] args Pointer to thread function arguments.
       * @param [in] allocator Reference to allocator. Default a local temporary instance.
       */
      thread (const char* name, func_t function, func_args_t args,
              const Allocator& allocator = Allocator ());

      /**
       * @brief Create a thread with custom settings.
       * @param [in] attr Reference to attributes.
       * @param [in] function Pointer to thread function.
       * @param [in] args Pointer to thread function arguments.
       * @param [in] allocator Reference to allocator. Default a local temporary instance.
       */
      thread (const attributes& attr, func_t function, func_args_t args,
              const Allocator& allocator = Allocator ());
      /**
       * @brief Create a named thread with custom settings.
       * @param [in] name Pointer to name.
       * @param [in] attr Reference to attributes.
       * @param [in] function Pointer to thread function.
       * @param [in] args Pointer to thread function arguments.
       * @param [in] allocator Reference to allocator. Default a local temporary instance.
       */
      thread (const char* name, const attributes& attr, func_t function,
              func_args_t args, const Allocator& allocator = Allocator ());

      /**
       * @cond ignore
       */
    protected:

      // Internal constructors, used from templates.
      thread ();
      thread (const char* name);
      thread (const char* given_name, const char* attr_name);

    public:

      thread (const thread&) = delete;
      thread (thread&&) = delete;
      thread&
      operator= (const thread&) = delete;
      thread&
      operator= (thread&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the thread.
       */
      virtual
      ~thread ();

      /**
       * @}
       */

      /*
       * @name Operators
       * @{
       */

      /**
       * @brief Compare threads.
       * @retval true The given thread is the same as this thread.
       * @retval false The threads are different.
       */
      bool
      operator== (const thread& rhs) const;

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
      sched_prio (priority_t prio);

      /**
       * @brief Get the current scheduling priority.
       * @par Parameters
       *  None.
       * @return The thread priority.
       */
      priority_t
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

      bool
      interrupt (bool interrupt = true);

      /**
       * @brief Get scheduler status of this thread.
       * @par Parameters
       *  None
       * @return Thread scheduler state.
       */
      state_t
      sched_state (void) const;

      /**
       * @brief Resume the thread.
       * @par Parameters
       *  None
       * @return  Nothing.
       *
       * @note Can be invoked from Interrupt Service Routines.
       */
      void
      resume (void);

      /**
       * @brief Get the thread function arguments.
       * @par Parameters
       *  None.
       * @return Pointer to arguments.
       */
      void*
      function_args (void) const;

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
      sig_raise (sigset_t mask, sigset_t* oflags);

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
      sig_clear (sigset_t mask, sigset_t* oflags);

      /**
       * @brief Get/clear thread signal flags.
       * @param [in] mask The OR-ed flags to get/clear; may be zero.
       * @param [in] mode Mode bits to select if the flags should be
       *  cleared (the other bits are ignored).
       * @retval flags The selected bits from the current thread
       *  signal flags mask.
       * @retval sig::all Cannot be invoked from an Interrupt Service Routines.
       */
      sigset_t
      sig_get (sigset_t mask, flags::mode_t mode);

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

      friend class mutex;

      friend void
      this_thread::wait (void);

      friend void
      this_thread::exit (void* exit_ptr);

      friend result_t
      this_thread::sig_wait (sigset_t mask, sigset_t* oflags,
                             flags::mode_t mode);

      friend result_t
      this_thread::try_sig_wait (sigset_t mask, sigset_t* oflags,
                                 flags::mode_t mode);
      friend result_t
      this_thread::timed_sig_wait (sigset_t mask, clock::duration_t timeout,
                                   sigset_t* oflags, flags::mode_t mode);
      friend int*
      this_thread::error (void);

      friend void
      scheduler::_link_node (Waiting_threads_list& list,
                             Waiting_thread_node& node);

      friend void
      scheduler::_unlink_node (Waiting_thread_node& node);

      friend void
      scheduler::_link_node (Waiting_threads_list& list,
                             Waiting_thread_node& node,
                             Clock_timestamps_list& timeout_list,
                             Timeout_thread_node& timeout_node);

      friend void
      scheduler::_unlink_node (Waiting_thread_node& node,
                               Timeout_thread_node& timeout_node);

      friend void
      port::scheduler::reschedule (void);

      friend void
      port::scheduler::start (void);

      friend port::stack::element_t*
      port::scheduler::switch_stacks (port::stack::element_t* sp);

      friend void*
      scheduler::_idle_func (thread::func_args_t args);

      friend class Ready_threads_list;
      friend class Top_threads_list;
      friend class Thread_children_list;
      friend class Waiting_threads_list;
      friend class Clock_timestamps_list;

      friend class Clock;
      friend class condition_variable;

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Functions
       * @{
       */

      /**
       * @brief Internal function used during thread construction.
       * @param [in] attr Reference to attributes.
       * @param [in] function Pointer to thread function.
       * @param [in] args Pointer to thread function arguments.
       * @param [in] stack_address Pointer to stack storage or nullptr.
       * @param [in] stack_size_bytes Size of stack storage or 0.
       */
      void
      _construct (const attributes& attr, func_t function, func_args_t args,
                  void* stack_address, std::size_t stack_size_bytes);

      /**
       * @brief Suspend this thread and wait for an event.
       * @par Parameters
       *  None
       * @return  Nothing.
       */
      void
      _wait (void);

      /**
       * @brief Terminate thread by itself.
       * @param [in] exit_ptr Pointer to object to return (optional).
       * @return  Nothing.
       */
      [[noreturn]]
      void
      _exit (void* exit_ptr = nullptr);

      /**
       * @brief Invoke terminating thread function.
       * @param [in] thread The static `this`.
       * @return  Nothing.
       */
      [[noreturn]]
      static void
      _invoke_with_exit (thread* thread);

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
      _sig_wait (sigset_t mask, sigset_t* oflags, flags::mode_t mode);

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
       * @retval EWOULDBLOCK The expected condition did not occur.
       * @retval ENOTRECOVERABLE Wait failed.
       */
      result_t
      _try_sig_wait (sigset_t mask, sigset_t* oflags, flags::mode_t mode);

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
      _timed_sig_wait (sigset_t mask, clock::duration_t timeout,
                       sigset_t* oflags, flags::mode_t mode);

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
       * @retval EWOULDBLOCK The expected condition did not occur.
       * @retval ENOTRECOVERABLE Wait failed.
       */
      result_t
      _try_wait (sigset_t mask, sigset_t* oflags, flags::mode_t mode);

      /**
       * @brief The actual destructor, also called from exit() and kill().
       * @par Parameters
       *  None
       * @return  Nothing.
       */
      virtual void
      _destroy (void);

      int*
      _error (void);

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Variables
       * @{
       */

#if !defined(OS_INCLUDE_RTOS_PORT_SCHEDULER)
      // TODO: make it fully intrusive with computed offset.
      Waiting_thread_node ready_node_
        { *this };
#endif

      int errno_ = 0;

      func_t func_ = nullptr;
      func_args_t func_args_ = nullptr;
      void* func_result_ = nullptr;

      // Implementation
#if defined(OS_INCLUDE_RTOS_PORT_SCHEDULER)
      friend class port::thread;
      os_thread_port_data_t port_;
#endif

      // Pointer to parent, or null for top/detached thread.
      thread* parent_ = nullptr;

      // List of children threads.
      Thread_children_list children_;

      // Intrusive node used to link this thread to parent list.
      Double_list_links child_links_;

      // Thread waiting to join.
      thread* joiner_ = nullptr;

      // Pointer to waiting node (stored on stack)
      Waiting_thread_node* waiting_node_ = nullptr;

      // Pointer to timeout node (stored on stack)
      Timeout_thread_node* clock_node_ = nullptr;

      /**
       * @brief Pointer to clock to be used for timeouts.
       */
      Clock* clock_ = nullptr;

      /**
       * @brief Pointer to allocator.
       */
      const void* allocator_ = nullptr;

      stack::element_t* allocated_stack_address_ = nullptr;

      std::size_t allocated_stack_size_elements_ = 0;

      std::size_t volatile acquired_mutexes_ = 0;
      state_t volatile sched_state_ = state::undefined;
      priority_t volatile prio_ = priority::none;

      sigset_t volatile sig_mask_ = 0;
      bool volatile interrupted_ = false;

      os_thread_user_storage_t user_storage_;

      // Add other internal data

      // Better be the last one!
      thread::context context_;

      /**
       * @}
       */
    };
    /* class thread */

    /**
     * @brief Template of a POSIX compliant **thread** with allocator.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos
     */
    template<typename Allocator = memory::allocator<void*>>
      class thread_allocated : public thread
      {
      public:

        using allocator_type = Allocator;

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Create a thread with default settings.
         * @param [in] function Pointer to thread function.
         * @param [in] args Pointer to thread function arguments.
         * @param [in] allocator Reference to allocator. Default a
         * local temporary instance.
         */
        thread_allocated (func_t function, func_args_t args,
                          const Allocator& allocator = Allocator ());

        /**
         * @brief Create a named thread with default settings.
         * @param [in] name Pointer to name.
         * @param [in] function Pointer to thread function.
         * @param [in] args Pointer to thread function arguments.
         * @param [in] allocator Reference to allocator. Default a
         * local temporary instance.
         */
        thread_allocated (const char* name, func_t function, func_args_t args,
                          const Allocator& allocator = Allocator ());

        /**
         * @brief Create a thread with custom settings.
         * @param [in] attr Reference to attributes.
         * @param [in] function Pointer to thread function.
         * @param [in] args Pointer to thread function arguments.
         * @param [in] allocator Reference to allocator. Default a
         * local temporary instance.
         */
        thread_allocated (const attributes& attr, func_t function,
                          func_args_t args, const Allocator& allocator =
                              Allocator ());

        /**
         * @brief Create a named thread with custom settings.
         * @param [in] name Pointer to name.
         * @param [in] attr Reference to attributes.
         * @param [in] function Pointer to thread function.
         * @param [in] args Pointer to thread function arguments.
         * @param [in] allocator Reference to allocator. Default a
         * local temporary instance.
         */
        thread_allocated (const char* name, const attributes& attr,
                          func_t function, func_args_t args,
                          const Allocator& allocator = Allocator ());

        /**
         * @cond ignore
         */
        thread_allocated (const thread_allocated&) = delete;
        thread_allocated (thread_allocated&&) = delete;
        thread_allocated&
        operator= (const thread_allocated&) = delete;
        thread_allocated&
        operator= (thread_allocated&&) = delete;
        /**
         * @endcond
         */

        /**
         * @brief Destroy the thread.
         */
        virtual
        ~thread_allocated ();

        /**
         * @}
         */

        /**
         * @name Private Member Functions
         * @{
         */

        virtual void
        _destroy (void) override;

        /**
         * @}
         */

      };

    /**
     * @brief Template of a POSIX compliant **thread** with local stack.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos
     */
    template<std::size_t N = port::stack::default_size_bytes>
      class thread_static : public thread
      {
      public:

        static const std::size_t stack_size_bytes = N;

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Create a thread with default settings.
         * @param [in] function Pointer to thread function.
         * @param [in] args Pointer to thread function arguments.
         */
        thread_static (func_t function, func_args_t args);

        /**
         * @brief Create a named thread with default settings.
         * @param [in] name Pointer to name.
         * @param [in] function Pointer to thread function.
         * @param [in] args Pointer to thread function arguments.
         */
        thread_static (const char* name, func_t function, func_args_t args);

        /**
         * @brief Create a thread with custom settings.
         * @param [in] attr Reference to attributes.
         * @param [in] function Pointer to thread function.
         * @param [in] args Pointer to thread function arguments.
         */
        thread_static (const attributes& attr, func_t function,
                       func_args_t args);

        /**
         * @brief Create a named thread with custom settings.
         * @param [in] name Pointer to name.
         * @param [in] attr Reference to attributes.
         * @param [in] function Pointer to thread function.
         * @param [in] args Pointer to thread function arguments.
         */
        thread_static (const char* name, const attributes& attr,
                       func_t function, func_args_t args);

        /**
         * @cond ignore
         */
        thread_static (const thread_static&) = delete;
        thread_static (thread_static&&) = delete;
        thread_static&
        operator= (const thread_static&) = delete;
        thread_static&
        operator= (thread_static&&) = delete;
        /**
         * @endcond
         */

        /**
         * @brief Destroy the thread.
         */
        virtual
        ~thread_static ();

        /**
         * @}
         */

      private:

        stack::allocation_element_t stack_[(stack_size_bytes
            + sizeof(stack::allocation_element_t) - 1)
            / sizeof(stack::allocation_element_t)];
      };

#pragma GCC diagnostic pop

    namespace this_thread
    {

      // ======================================================================

      /**
       * @brief Get the current running thread.
       * @par Parameters
       *  None
       * @return Reference to the current running thread.
       */
      thread&
      thread (void);

      /**
       * @brief Yield execution to the next ready thread.
       * @par Parameters
       *  None
       * @return Nothing.
       */
      void
      yield (void);

      /**
       * @brief Suspend the current running thread to wait for an event.
       * @par Parameters
       *  None
       * @return Nothing.
       */
      void
      wait (void);

      /**
       * @brief Terminate the current running thread.
       * @param [in] exit_ptr Pointer to object to return. (Optional).
       * @return Nothing.
       */
      [[noreturn]] void
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
      sig_wait (sigset_t mask, sigset_t* oflags = nullptr,
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
       * @retval EWOULDBLOCK The expected condition did not occur.
       * @retval ENOTRECOVERABLE Wait failed.
       */
      result_t
      try_sig_wait (sigset_t mask, sigset_t* oflags = nullptr,
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
          sigset_t mask, clock::duration_t timeout, sigset_t* oflags = nullptr,
          flags::mode_t mode = flags::mode::all | flags::mode::clear);

      int*
      error (void);

      rtos::thread*
      _thread (void);

    } /* namespace this_thread */

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
       * control to the next ready thread. The
       * thread will not be automatically rescheduled, it requires
       * some other tread or interrupt service routine to add it
       * back to the READY state (via `thread::resume()`).
       *
       * This is different from `yield()` which automatically
       * reschedules the current thread before passing control to
       * the next thread (which might be the same if no other
       * threads with at least the same priority are ready).
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline void
      wait (void)
      {
        this_thread::thread ()._wait ();
      }

      /**
       * @details
       * If the flags::mode::all bit is set, the function expects
       * all given flags to be raised; otherwise, if the flags::mode::any
       * bit is set, the function expects any single flag to be raised.
       *
       * If the expected signal flags are
       * raised, the function returns instantly.
       *
       * Otherwise suspend the execution of the current thread until all/any
       * specified signal flags are raised.
       *
       * When the parameter mask is 0, the thread is suspended
       * until any signal flag is raised. In this case, if any signal flags
       * are already raised, the function returns instantly.
       *
       * If the flags::mode::clear bit is set, the signal flags that are
       * returned are automatically cleared.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline result_t
      sig_wait (sigset_t mask, sigset_t* oflags, flags::mode_t mode)
      {
        return this_thread::thread ()._sig_wait (mask, oflags, mode);
      }

      /**
       * @details
       * If the flags::mode::all bit is set, the function expects
       * all given flags to be raised; otherwise, if the flags::mode::any
       * bit is set, the function expects any single flag to be raised.
       *
       * The function does not blocks, if the expected signal flags are
       * not raised, but returns `EGAIN`.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline result_t
      try_sig_wait (sigset_t mask, sigset_t* oflags, flags::mode_t mode)
      {
        return this_thread::thread ()._try_sig_wait (mask, oflags, mode);
      }

      /**
       * @details
       * If the flags::mode::all bit is set, the function expects
       * all given flags to be raised; otherwise, if the flags::mode::any
       * bit is set, the function expects any single flag to be raised.
       *
       * If the expected signal flags are
       * raised, the function returns instantly.
       *
       * Otherwise suspend the execution of the thread until all/any
       * specified signal flags are raised.
       *
       * When the parameter mask is 0, the thread is suspended
       * until any event flag is raised. In this case, if any signal flags
       * are already raised, the function returns instantly.
       *
       * The wait shall be terminated when the specified timeout
       * expires.
       *
       * The timeout shall expire after the number of time units (that
       * is when the value of that clock equals or exceeds (now()+duration).
       * The resolution of the timeout shall be the resolution of the
       * clock on which it is based (the SysTick clock for CMSIS).
       *
       * Under no circumstance shall the operation fail with a timeout
       * if the signal flags are already raised. The validity of
       * the timeout need not be checked if the expected flags
       * are already raised and the call can return immediately.
       *
       * The clock used for timeouts can be specified via the `clock`
       * attribute. By default, the clock derived from the scheduler
       * timer is used, and the durations are expressed in ticks.
       *
       * If the flags::mode::clear bit is set, the signal flags that are
       * returned are automatically cleared.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline result_t
      timed_sig_wait (sigset_t mask, clock::duration_t timeout,
                      sigset_t* oflags, flags::mode_t mode)
      {
        return this_thread::thread ()._timed_sig_wait (mask, timeout, oflags,
                                                       mode);
      }

      /**
       * @details
       * Terminate the calling thread and make the value _value_ptr_
       * available to any successful join with the terminating thread.
       * Any cancellation cleanup handlers that have been pushed and
       * not yet popped shall be popped in the reverse order that
       * they were pushed and then executed. After all cancellation
       * cleanup handlers have been executed, if the thread has any
       * thread-specific data, appropriate destructor functions
       * shall be called in an unspecified order. Thread termination
       * does not release any application visible process resources,
       * including, but not limited to, mutexes and file descriptors,
       * nor does it perform any process-level cleanup actions,
       * including, but not limited to, calling any `atexit()` routines
       * that may exist.
       * An implicit call to exit() is made when a thread other
       * than the thread in which main() was first invoked returns
       * from the start routine that was used to create it.
       * The function's return value shall serve as the thread's
       * exit status.
       * The behaviour of `_exit()` is undefined if called from a
       * cancellation cleanup handler or destructor function that
       * was invoked as a result of either an implicit or explicit
       * call to `exit()`.
       * After a thread has terminated, the result of access to
       * local (auto) variables of the thread is undefined.
       * Thus, references to local variables of the exiting
       * thread should not be used for the `exit()` _value_ptr_
       * parameter value.
       *
       * @par POSIX compatibility
       *  Inspired by [`pthread_exit()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_exit.html)
       *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
       *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline void
      exit (void* exit_ptr)
      {
        this_thread::thread ()._exit (exit_ptr);
      }

      inline int*
      error (void)
      {
        return this_thread::thread ()._error ();
      }

    } /* namespace this_thread */

    constexpr
    thread::attributes::attributes (const char* name) :
        clocked_attributes
          { name }
    {
      ;
    }

    // ======================================================================

    inline
    thread::stack::stack ()
    {
      bottom_address_ = nullptr;
      size_bytes_ = 0;
    }

    inline thread::stack::element_t*
    thread::stack::bottom (void)
    {
      return bottom_address_;
    }

    inline std::size_t
    thread::stack::size (void)
    {
      return size_bytes_;
    }

    inline std::size_t
    thread::stack::min_size (void)
    {
      return min_size_bytes_;
    }

    inline void
    thread::stack::min_size (std::size_t size_bytes)
    {
      min_size_bytes_ = size_bytes;
    }

    inline std::size_t
    thread::stack::default_size (void)
    {
      return default_size_bytes_;
    }

    inline void
    thread::stack::default_size (std::size_t size_bytes)
    {
      assert(size_bytes != 0);
      assert(size_bytes >= min_size_bytes_);

      default_size_bytes_ = size_bytes;
    }

    // ========================================================================

    inline
    thread::context::context ()
    {
      ;
    }

    inline thread::stack&
    thread::context::stack ()
    {
      return stack_;
    }

    // ========================================================================

    /**
     * @details
     * Identical threads should have the same memory address.
     *
     * Compatible with POSIX pthread_equal().
     * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_equal.html
     */
    inline bool
    thread::operator== (const thread& rhs) const
    {
      return this == &rhs;
    }

    inline thread::state_t
    thread::sched_state (void) const
    {
      return sched_state_;
    }

    inline void*
    thread::function_args (void) const
    {
      return func_args_;
    }

    inline bool
    thread::interrupt (bool interrupt)
    {
      bool tmp = interrupted_;
      interrupted_ = interrupt;
      return tmp;
    }

    inline bool
    thread::interrupted (void)
    {
      return interrupted_;
    }

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
    thread::user_storage (void)
    {
      return &user_storage_;
    }

    inline int*
    thread::_error (void)
    {
      return &errno_;
    }

    // ========================================================================

    /**
     * @details
     * This constructor shall initialise an unnamed thread object
     * with default settings.
     * The effect shall be equivalent to creating a thread object
     * referring to the attributes in `thread::initializer`.
     * Upon successful initialisation, the state of the
     * thread object shall become initialised, and the thread is
     * added to the ready list.
     *
     * Only the thread object itself may be used for running the
     * function. It is not allowed to make copies of
     * condition variable objects.
     *
     * The thread is created to execute _function_ with _args_ as its
     * sole argument. If the function returns, the effect
     * shall be as if there was an implicit call to `exit()` using
     * the return value of function as the exit status. Note that
     * the thread in which `main()` was originally invoked differs
     * from this. When it returns from `main()`, the effect shall
     * be as if there was an implicit call to `exit()` using the
     * return value of `main()` as the exit status.
     *
     * For default thread objects, the stack is dynamically allocated,
     * using the given allocator (default `rtos::memory::allocator`).
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_create()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_create.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename Allocator>
      inline
      thread_allocated<Allocator>::thread_allocated (func_t function,
                                                     func_args_t args,
                                                     const Allocator& allocator) :
          thread_allocated
            { nullptr, function, args, allocator }
      {
        ;
      }

    /**
     * @details
     * This constructor shall initialise a named thread object
     * with default settings.
     * The effect shall be equivalent to creating a thread object
     * referring to the attributes in `thread::initializer`.
     * Upon successful initialisation, the state of the
     * thread object shall become initialised, and the thread is
     * added to the ready list.
     *
     * Only the thread object itself may be used for running the
     * function. It is not allowed to make copies of
     * condition variable objects.
     *
     * The thread is created to execute _function_ with _args_ as its
     * sole argument. If the function returns, the effect
     * shall be as if there was an implicit call to `exit()` using
     * the return value of function as the exit status. Note that
     * the thread in which `main()` was originally invoked differs
     * from this. When it returns from `main()`, the effect shall
     * be as if there was an implicit call to `exit()` using the
     * return value of `main()` as the exit status.
     *
     * For default thread objects, the stack is dynamically allocated,
     * using the given allocator (default `rtos::memory::allocator`).
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_create()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_create.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename Allocator>
      thread_allocated<Allocator>::thread_allocated (const char* name,
                                                     func_t function,
                                                     func_args_t args,
                                                     const Allocator& allocator) :
          thread
            { name }
      {
#if defined(OS_TRACE_RTOS_THREAD)
        trace::printf ("%s @%p %s\n", __func__, this, this->name ());
#endif

        allocator_ = &allocator;

        allocated_stack_size_elements_ = (stack::default_size ()
            + sizeof(typename Allocator::value_type) - 1)
            / sizeof(typename Allocator::value_type);
        allocated_stack_address_ =
            reinterpret_cast<stack::element_t*> (const_cast<Allocator&> (allocator).allocate (
                allocated_stack_size_elements_));

        _construct (
            initializer,
            function,
            args,
            allocated_stack_address_,
            allocated_stack_size_elements_
                * sizeof(typename Allocator::value_type));
      }

    /**
     * @details
     * This constructor shall initialise a thread object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the thread attributes shall not be affected.
     * Upon successful initialisation, the state of the
     * thread object shall become initialised, and the thread is
     * added to the ready list.
     *
     * Only the thread object itself may be used for running the
     * function. It is not allowed to make copies of
     * thread objects.
     *
     * In cases where default thread attributes are
     * appropriate, the variable `thread::initializer` can be used to
     * initialise threads.
     * The effect shall be equivalent to creating a thread
     * object with the default constructor.
     *
     * The thread is created to execute _function_ with _args_ as its
     * sole argument. If the function returns, the effect
     * shall be as if there was an implicit call to `exit()` using
     * the return value of function as the exit status. Note that
     * the thread in which `main()` was originally invoked differs
     * from this. When it returns from `main()`, the effect shall
     * be as if there was an implicit call to `exit()` using the
     * return value of `main()` as the exit status.
     *
     * If the attributes define a stack area (via `th_stack_address` and
     * `th_stack_size_bytes`), that stack is used, otherwise
     * the stack is dynamically allocated using the RTOS specific allocator
     * (`rtos::memory::allocator`).
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_create()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_create.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename Allocator>
      inline
      thread_allocated<Allocator>::thread_allocated (const attributes& attr,
                                                     func_t function,
                                                     func_args_t args,
                                                     const Allocator& allocator) :
          thread_allocated
            { nullptr, attr, function, args, allocator }
      {
        ;
      }

    /**
     * @details
     * This constructor shall initialise a named thread object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the thread attributes shall not be affected.
     * Upon successful initialisation, the state of the
     * thread object shall become initialised, and the thread is
     * added to the ready list.
     *
     * Only the thread object itself may be used for running the
     * function. It is not allowed to make copies of
     * thread objects.
     *
     * In cases where default thread attributes are
     * appropriate, the variable `thread::initializer` can be used to
     * initialise threads.
     * The effect shall be equivalent to creating a thread
     * object with the default constructor.
     *
     * The thread is created to execute _function_ with _args_ as its
     * sole argument. If the function returns, the effect
     * shall be as if there was an implicit call to `exit()` using
     * the return value of function as the exit status. Note that
     * the thread in which `main()` was originally invoked differs
     * from this. When it returns from `main()`, the effect shall
     * be as if there was an implicit call to `exit()` using the
     * return value of `main()` as the exit status.
     *
     * If the attributes define a stack area (via `th_stack_address` and
     * `th_stack_size_bytes`), that stack is used, otherwise
     * the stack is dynamically allocated using the RTOS specific allocator
     * (`rtos::memory::allocator`).
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_create()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_create.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename Allocator>
      thread_allocated<Allocator>::thread_allocated (const char* name,
                                                     const attributes& attr,
                                                     func_t function,
                                                     func_args_t args,
                                                     const Allocator& allocator) :
          thread
            { name, attr.name () }
      {
#if defined(OS_TRACE_RTOS_THREAD)
        trace::printf ("%s @%p %s\n", __func__, this, this->name ());
#endif
        if (attr.th_stack_address != nullptr
            && attr.th_stack_size_bytes > stack::min_size ())
          {
            _construct (attr, function, args, nullptr, 0);
          }
        else
          {
            allocator_ = &allocator;

            if (attr.th_stack_size_bytes > stack::min_size ())
              {
                allocated_stack_size_elements_ = (attr.th_stack_size_bytes
                    + sizeof(typename Allocator::value_type) - 1)
                    / sizeof(typename Allocator::value_type);
              }
            else
              {
                allocated_stack_size_elements_ = (stack::default_size ()
                    + sizeof(typename Allocator::value_type) - 1)
                    / sizeof(typename Allocator::value_type);
              }
            allocated_stack_address_ =
                const_cast<Allocator&> (allocator).allocate (
                    allocated_stack_size_elements_);

            _construct (
                attr,
                function,
                args,
                allocated_stack_address_,
                allocated_stack_size_elements_
                    * sizeof(typename Allocator::value_type));
          }
      }

    template<typename Allocator>
      void
      thread_allocated<Allocator>::_destroy (void)
      {
#if defined(OS_TRACE_RTOS_THREAD)
        trace::printf ("thread_allocated::%s() @%p %s\n", __func__, this,
                       name ());
#endif

        if (allocated_stack_address_ != nullptr)
          {
            typedef typename std::allocator_traits<Allocator>::pointer pointer;

            static_cast<Allocator*> (const_cast<void*> (allocator_))->deallocate (
                reinterpret_cast<pointer> (allocated_stack_address_),
                allocated_stack_size_elements_);

            allocated_stack_address_ = nullptr;
          }

        thread::_destroy ();
      }

    /**
     * @details
     * This destructor shall destroy the thread object; the object
     * becomes, in effect, uninitialised. An implementation may cause
     * the destructor to set the object to an invalid value.
     *
     * The stack is deallocated using the same allocator.
     *
     * @par POSIX compatibility
     *  No POSIX similar functionality identified.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<typename Allocator>
      thread_allocated<Allocator>::~thread_allocated ()
      {
#if defined(OS_TRACE_RTOS_THREAD)
        trace::printf ("%s @%p %s\n", __func__, this, name ());
#endif
      }

    // ========================================================================

    /**
     * @details
     * This constructor shall initialise a thread object
     * with default settings.
     * The effect shall be equivalent to creating a thread object
     * referring to the attributes in `thread::initializer`.
     * Upon successful initialisation, the state of the
     * thread object shall become initialised, and the thread is
     * added to the ready list.
     *
     * Only the thread object itself may be used for running the
     * function. It is not allowed to make copies of
     * condition variable objects.
     *
     * The thread is created to execute _function_ with _args_ as its
     * sole argument. If the function returns, the effect
     * shall be as if there was an implicit call to `exit()` using
     * the return value of function as the exit status. Note that
     * the thread in which `main()` was originally invoked differs
     * from this. When it returns from `main()`, the effect shall
     * be as if there was an implicit call to `exit()` using the
     * return value of `main()` as the exit status.
     *
     * The storage shall be statically allocated inside the
     * thread object instance.
     *
     * @note These objects are better instantiated as global static
     * objects. When instantiated on the thread stack, the stack
     * should be sized accordingly.
     *
     * Implemented as a wrapper over the parent constructor, automatically
     * passing the stack size and address.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_create()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_create.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<std::size_t N>
      inline
      thread_static<N>::thread_static (func_t function, func_args_t args) :
          thread_static<N>
            { nullptr, function, args }
      {
        ;
      }

    /**
     * @details
     * This constructor shall initialise a named thread object
     * with default settings.
     * The effect shall be equivalent to creating a thread object
     * referring to the attributes in `thread::initializer`.
     * Upon successful initialisation, the state of the
     * thread object shall become initialised, and the thread is
     * added to the ready list.
     *
     * Only the thread object itself may be used for running the
     * function. It is not allowed to make copies of
     * condition variable objects.
     *
     * The thread is created to execute _function_ with _args_ as its
     * sole argument. If the function returns, the effect
     * shall be as if there was an implicit call to `exit()` using
     * the return value of function as the exit status. Note that
     * the thread in which `main()` was originally invoked differs
     * from this. When it returns from `main()`, the effect shall
     * be as if there was an implicit call to `exit()` using the
     * return value of `main()` as the exit status.
     *
     * The storage shall be statically allocated inside the
     * thread object instance.
     *
     * @note These objects are better instantiated as global static
     * objects. When instantiated on the thread stack, the stack
     * should be sized accordingly.
     *
     * Implemented as a wrapper over the parent constructor, automatically
     * passing the stack size and address.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_create()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_create.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<std::size_t N>
      thread_static<N>::thread_static (const char* name, func_t function,
                                       func_args_t args) :
          thread
            { name }
      {
#if defined(OS_TRACE_RTOS_THREAD)
        trace::printf ("%s @%p %s\n", __func__, this, this->name ());
#endif
        _construct (initializer, function, args, &stack_, stack_size_bytes);
      }

    /**
     * @details
     * This constructor shall initialise a thread object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the thread attributes shall not be affected.
     * Upon successful initialisation, the state of the
     * thread object shall become initialised, and the thread is
     * added to the ready list.
     *
     * Only the thread object itself may be used for running the
     * function. It is not allowed to make copies of
     * thread objects.
     *
     * In cases where default thread attributes are
     * appropriate, the variable `thread::initializer` can be used to
     * initialise threads.
     * The effect shall be equivalent to creating a thread
     * object with the default constructor.
     *
     * The thread is created to execute _function_ with _args_ as its
     * sole argument. If the function returns, the effect
     * shall be as if there was an implicit call to `exit()` using
     * the return value of function as the exit status. Note that
     * the thread in which `main()` was originally invoked differs
     * from this. When it returns from `main()`, the effect shall
     * be as if there was an implicit call to `exit()` using the
     * return value of `main()` as the exit status.
     *
     * The storage shall be statically allocated inside the
     * thread object instance.
     *
     * @note These objects are better instantiated as global static
     * objects. When instantiated on the thread stack, the stack
     * should be sized accordingly.
     *
     * Implemented as a wrapper over the parent constructor, automatically
     * passing the stack size and address.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_create()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_create.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<std::size_t N>
      inline
      thread_static<N>::thread_static (const attributes& attr, func_t function,
                                       func_args_t args) :
          thread_static<N>
            { nullptr, attr, function, args }
      {
        ;
      }

    /**
     * @details
     * This constructor shall initialise a named thread object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the thread attributes shall not be affected.
     * Upon successful initialisation, the state of the
     * thread object shall become initialised, and the thread is
     * added to the ready list.
     *
     * Only the thread object itself may be used for running the
     * function. It is not allowed to make copies of
     * thread objects.
     *
     * In cases where default thread attributes are
     * appropriate, the variable `thread::initializer` can be used to
     * initialise threads.
     * The effect shall be equivalent to creating a thread
     * object with the default constructor.
     *
     * The thread is created to execute _function_ with _args_ as its
     * sole argument. If the function returns, the effect
     * shall be as if there was an implicit call to `exit()` using
     * the return value of function as the exit status. Note that
     * the thread in which `main()` was originally invoked differs
     * from this. When it returns from `main()`, the effect shall
     * be as if there was an implicit call to `exit()` using the
     * return value of `main()` as the exit status.
     *
     * The storage shall be statically allocated inside the
     * thread object instance.
     *
     * @note These objects are better instantiated as global static
     * objects. When instantiated on the thread stack, the stack
     * should be sized accordingly.
     *
     * Implemented as a wrapper over the parent constructor, automatically
     * passing the stack size and address.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_create()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_create.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<std::size_t N>
      thread_static<N>::thread_static (const char* name, const attributes& attr,
                                       func_t function, func_args_t args) :
          thread
            { name, attr.name () }
      {
#if defined(OS_TRACE_RTOS_THREAD)
        trace::printf ("%s @%p %s\n", __func__, this, this->name ());
#endif
        _construct (attr, function, args, &stack_,
                    stack_size_bytes * sizeof(decltype(stack_[0])));
      }

    /**
     * @details
     * This destructor shall destroy the thread object; the object
     * becomes, in effect, uninitialised. An implementation may cause
     * the destructor to set the object to an invalid value.
     *
     * @par POSIX compatibility
     *  No POSIX similar functionality identified.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    template<std::size_t N>
      thread_static<N>::~thread_static ()
      {
#if defined(OS_TRACE_RTOS_THREAD)
        trace::printf ("%s @%p %s\n", __func__, this, name ());
#endif
      }

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_THREAD_H_ */
