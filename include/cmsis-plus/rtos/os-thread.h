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
#include <cmsis-plus/rtos/os-clocks.h>
#include <cmsis-plus/rtos/internal/os-flags.h>

#if !defined(__ARM_EABI__)
#include <memory>
#endif

// ----------------------------------------------------------------------------

/**
 * @cond ignore
 */

namespace
{
  // Anonymous definition required for the next forward definition.
  using _func_args_t = void*;
}

void*
os_idle (_func_args_t args);

/**
 * @endcond
 */

namespace os
{
  namespace rtos
  {
    // ------------------------------------------------------------------------

    namespace this_thread
    {
      /**
       * @brief Get the current running thread.
       * @par Parameters
       *  None.
       * @return Reference to the current running thread.
       */
      thread&
      thread (void);

      /**
       * @cond ignore
       */

      rtos::thread*
      _thread (void);

      /**
       * @endcond
       */

      /**
       * @brief Yield execution to the next ready thread.
       * @par Parameters
       *  None.
       * @par Returns
       *  Nothing.
       */
      void
      yield (void);

      /**
       * @brief Suspend the current running thread to wait for an event.
       * @par Parameters
       *  None.
       * @par Returns
       *  Nothing.
       */
      void
      suspend (void);

      /**
       * @brief Terminate the current running thread.
       * @param [in] exit_ptr Pointer to object to return. (Optional,
       * may be nullptr).
       * @par Returns
       *  Nothing.
       */
      [[noreturn]] void
      exit (void* exit_ptr = nullptr);

      /**
       * @brief Wait for thread event flags.
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
      flags_wait (flags::mask_t mask, flags::mask_t* oflags = nullptr,
                  flags::mode_t mode = flags::mode::all | flags::mode::clear);

      /**
       * @brief Try to wait for thread event flags.
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
      flags_try_wait (
          flags::mask_t mask, flags::mask_t* oflags = nullptr,
          flags::mode_t mode = flags::mode::all | flags::mode::clear);

      /**
       * @brief Timed wait for thread event flags.
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
      flags_timed_wait (
          flags::mask_t mask, clock::duration_t timeout, flags::mask_t* oflags =
              nullptr,
          flags::mode_t mode = flags::mode::all | flags::mode::clear);

      /**
       * @brief Clear thread event flags.
       * @param [in] mask The OR-ed flags to clear.
       * @param [out] oflags Optional pointer where to store the
       *  previous flags; may be `nullptr`.
       * @retval result::ok The flags were cleared.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EINVAL The mask is zero.
       */
      result_t
      flags_clear (flags::mask_t mask, flags::mask_t* oflags = nullptr);

      /**
       * @brief Get/clear thread event flags.
       * @param [in] mask The OR-ed flags to get/clear; may be zero.
       * @param [in] mode Mode bits to select if the flags should be
       *  cleared (the other bits are ignored).
       * @retval flags The selected bits from the current thread
       *  event flags mask.
       * @retval flags::all Cannot be invoked from an Interrupt Service Routines.
       */
      flags::mask_t
      flags_get (flags::mask_t mask,
                 flags::mode_t mode = flags::mode::all | flags::mode::clear);

      /**
       * @brief Implementation of the library `__errno()` function.
       * @return Pointer to thread specific `errno`.
       */
      int*
      __errno (void);

    } /* namespace this_thread */

    // Forward definitions required by thread friends.
    namespace scheduler
    {

    } /* namespace scheduler */

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @brief POSIX compliant **thread**, using the
     * default RTOS allocator.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos-thread
     */
    class thread : public internal::object_named_system
    {
    public:

      // ======================================================================

      /**
       * @brief Type of variables holding thread priorities.
       * @details
       * A numeric type used to hold thread priorities, affecting the thread
       * behaviour, like scheduling and thread wakeup due to events;
       * usually an unsigned 8-bits type.
       *
       * Higher values represent higher priorities.
       * @ingroup cmsis-plus-rtos-thread
       */
      using priority_t = uint8_t;

      /**
       * @brief Thread priorities.
       * @details
       * The os::rtos::thread::priority definition is a container for
       * priorities not restricted to an enumeration.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @ingroup cmsis-plus-rtos-thread
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
         */
        static constexpr uint32_t range = 4;

        /**
         * @brief Thread priorities; intermediate values are also possible.
         * @ingroup cmsis-plus-rtos-thread
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
       * @brief Type of variables holding thread states.
       */
      using state_t = uint8_t;

      /**
       * @brief Thread states.
       * @details
       * The os::rtos::thread::state definition is a container for
       * thread states.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @ingroup cmsis-plus-rtos-thread
       */
      struct state
      {
        /**
         * @brief An enumeration with all possible thread states.
         */
        enum
          : state_t
            {
              /**
               * @brief Used to catch uninitialised threads.
               */
              undefined = 0, //
          /**
           * @brief Present in the READY list and competing for CPU.
           */
          ready = 1, //
          /**
           * @brief Has the CPU and runs.
           */
          running = 2, //
          /**
           * @brief Not present in the READY list, waiting for an event.
           */
          suspended = 3, //
          /**
           * @brief No longer usable, but resources not yet released.
           */
          terminated = 4, //
          /**
           * @brief Terminated and resources (like stack) released.
           */
          destroyed = 5
        };
        /* enum  */
      }; /* struct state */

      /**
       * @brief Type of thread function arguments.
       * @details
       * Useful to cast other similar types
       * to silence possible compiler warnings.
       */
      using func_args_t = _func_args_t;

      /**
       * @brief Type of thread function.
       * @details
       * Useful to cast other similar types
       * to silence possible compiler warnings.
       */
      using func_t = void* (*) (func_args_t args);

      // ======================================================================

      /**
       * @brief Thread stack.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @ingroup cmsis-plus-rtos-thread
       */
      class stack
      {
      public:

        /**
         * @brief Type of a stack element.
         * @details
         * The stack is organised as an array of platform words
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

        static const element_t magic = os::rtos::port::stack::magic;

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a thread stack object instance.
         */
        stack ();

        /**
         * @cond ignore
         */

        // The rule of five.
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
         * @brief Destruct the stack object instance.
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
         * @brief Clear the stack pointer and size.
         * @par Parameters
         *  None.
         * @par Returns
         *  Nothing
         */
        void
        clear (void);

        /**
         * @brief Set the stack address and size.
         * @param [in] address Bottom stack address.
         * @param [in] size_bytes Reserved stack size, in bytes.
         * @par Parameters
         *  None.
         * @par Returns
         *  Nothing
         */
        void
        set (stack::element_t* address, std::size_t size_bytes);

        /**
         * @brief Align the pointers and initialise to a known pattern.
         * @par Parameters
         *  None.
         * @par Returns
         *  Nothing.
         */
        void
        initialize (void);

        /**
         * @brief Get the stack lowest reserved address.
         * @par Parameters
         *  None.
         * @return  The address of the stack reserved area.
         */
        stack::element_t*
        bottom (void);

        /**
         * @brief Get the top stack address.
         * @par Parameters
         *  None.
         * @return The address after the last stack element.
         */
        stack::element_t*
        top (void);

        /**
         * @brief Get the stack size.
         * @par Parameters
         *  None.
         * @return  The stack size in bytes.
         */
        std::size_t
        size (void);

        /**
         * @brief Check if bottom magic word is still there.
         * @par Parameters
         *  None.
         * @retval true  The magic word is still there.
         * @retval false  The magic word was overwritten.
         */
        bool
        check_bottom_magic (void);

        /**
         * @brief Check if top magic word is still there.
         * @par Parameters
         *  None.
         * @retval true  The magic word is still there.
         * @retval false  The magic word was overwritten.
         */
        bool
        check_top_magic (void);

        /**
         * @brief Compute how much available stack remains.
         * @par Parameters
         *  None.
         * @return Number of available bytes.
         */
        std::size_t
        available (void);

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
         *  None.
         * @return  The min stack size in bytes.
         */
        static std::size_t
        min_size (void);

        /**
         * @brief Set the min stack size.
         * @param [in] size_bytes Minimum stack size in bytes.
         * @return  The previous value of the min stack size in bytes.
         */
        static std::size_t
        min_size (std::size_t size_bytes);

        /**
         * @brief Get the default stack size.
         * @par Parameters
         *  None.
         * @return  The default stack size in bytes.
         */
        static std::size_t
        default_size (void);

        /**
         * @brief Set the default stack size.
         * @param [in] size_bytes Default stack size in bytes.
         * @return  The previous value of the default stack size in bytes.
         */
        static std::size_t
        default_size (std::size_t size_bytes);

        /**
         * @}
         */

      protected:

        /**
         * @cond ignore
         */

        friend class rtos::thread;

        stack::element_t* bottom_address_;
        std::size_t size_bytes_;

        static std::size_t min_size_bytes_;
        static std::size_t default_size_bytes_;

        /**
         * @endcond
         */

      }; /* class stack */

      // ======================================================================
      /**
       * @brief Thread context.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @ingroup cmsis-plus-rtos-thread
       */
      class context
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a thread context object instance.
         */
        context ();

        /**
         * @cond ignore
         */

        // The rule of five.
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
         * @brief Destruct the context object instance.
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

        /**
         * @brief Get the associated stack.
         * @par Parameters
         *  None.
         * @return Reference to stack object.
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

        /**
         * @cond ignore
         */

        friend class rtos::thread;
        friend class rtos::port::thread;
        friend void
        port::scheduler::start (void);
        friend void
        port::scheduler::reschedule ();

#if !defined(OS_USE_RTOS_PORT_SCHEDULER)

        friend class port::context;

        friend port::stack::element_t*
        port::scheduler::switch_stacks (port::stack::element_t* sp);

#endif
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

        /**
         * @brief Stack object instance.
         */
        thread::stack stack_;

#if !defined(OS_USE_RTOS_PORT_SCHEDULER)

        /**
         * @brief Non-portable context data.
         */
        port::thread_context_t port_;

#endif

        /**
         * @endcond
         */

        /**
         * @}
         */

      }; /* class context */

      // ======================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

      /**
       * @brief Thread attributes.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @ingroup cmsis-plus-rtos-thread
       */
      class attributes : public internal::attributes_clocked
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a thread attributes object instance.
         * @par Parameters
         *  None.
         */
        constexpr
        attributes ();

        // The rule of five.
        attributes (const attributes&) = default;
        attributes (attributes&&) = default;
        attributes&
        operator= (const attributes&) = default;
        attributes&
        operator= (attributes&&) = default;

        /**
         * @brief Destruct the thread attributes object instance.
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
         * @brief Address of the user defined storage for the thread stack.
         * @details
         * If `nullptr`, the default is to dynamically allocate the stack.
         */
        void* th_stack_address = 0;

        /**
         * @brief Size of the user defined storage for the thread
         * stack, in bytes.
         * @details
         * If 0, the default is `thread::stack::default_size()`.
         *
         * A convenient and explicit variant to this attribute
         * is to call `thread::stack::default_size (std::size_t)` just before
         * creating the thread. However mind setting this from different
         * threads at the same time.
         */
        std::size_t th_stack_size_bytes = 0;

        /**
         * @brief Thread initial priority.
         * @details
         * If 0, the default is `thread::priority::normal`.
         *
         * A convenient and explicit variant to this attribute
         * is to call `thread::priority (priority_t)` at the
         * beginning of the thread function.
         */
        priority_t th_priority = priority::normal;

        // Add more attributes here.

        /**
         * @}
         */

      }; /* class attributes */

#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES) \
  || defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)

      /**
       * @brief Thread statistics.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @ingroup cmsis-plus-rtos-thread
       */
      class statistics
      {
      public:
        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a thread attributes object instance.
         * @par Parameters
         *  None.
         */
        statistics () = default;

        /**
         * @cond ignore
         */

        // The rule of five.
        statistics (const statistics&) = delete;
        statistics (statistics&&) = delete;
        statistics&
        operator= (const statistics&) = delete;
        statistics&
        operator= (statistics&&) = delete;

        /**
         * @endcond
         */

        /**
         * @brief Destruct the thread attributes object instance.
         */
        ~statistics () = default;

        /**
         * @}
         */

      public:

        /**
         * @name Public Member Functions
         * @{
         */

#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)

        /**
         * @brief Get the number of thread context switches.
         * @par Parameters
         *  None.
         * @return A long integer with the number of times the thread
         * was scheduled for execution.
         */
        rtos::statistics::counter_t
        context_switches (void);

#endif /* defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES) */

#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)

        /**
         * @brief Get the thread execution time.
         * @par Parameters
         *  None.
         * @return A long integer with accumulated number of CPU cycles.
         */
        rtos::statistics::duration_t
        cpu_cycles (void);

#endif /* defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES) */

        /**
         * @}
         */

      protected:

        /**
         * @cond ignore
         */

        friend void
        rtos::scheduler::internal_switch_threads (void);

#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)
        rtos::statistics::counter_t context_switches_ = 0;
#endif /* defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES) */

#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)
        rtos::statistics::duration_t cpu_cycles_ = 0;
#endif /* defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES) */

        /**
         * @endcond
         */

      };

#endif /* defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES) || defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES) */

#pragma GCC diagnostic pop

      /**
       * @brief Default thread initialiser.
       */
      static const attributes initializer;

      // ======================================================================

      /**
       * @brief Default RTOS allocator.
       */
      using allocator_type = memory::allocator<stack::allocation_element_t>;

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct a thread object instance.
       * @param [in] function Pointer to thread function.
       * @param [in] args Pointer to thread function arguments.
       * @param [in] attr Reference to attributes.
       * @param [in] allocator Reference to allocator. Default a local temporary instance.
       */
      thread (func_t function, func_args_t args, const attributes& attr =
                  initializer,
              const allocator_type& allocator = allocator_type ());

      /**
       * @brief Construct a named thread object instance.
       * @param [in] name Pointer to name.
       * @param [in] function Pointer to thread function.
       * @param [in] args Pointer to thread function arguments.
       * @param [in] attr Reference to attributes.
       * @param [in] allocator Reference to allocator. Default a local temporary instance.
       */
      thread (const char* name, func_t function, func_args_t args,
              const attributes& attr = initializer,
              const allocator_type& allocator = allocator_type ());

    protected:

      /**
       * @cond ignore
       */

      // Internal constructors, used from templates.
      thread ();
      thread (const char* name);

      /**
       * @endcond
       */

    public:

      /**
       * @cond ignore
       */

      // The rule of five.
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
       * @brief Destruct the thread object instance.
       */
      virtual
      ~thread ();

      /**
       * @}
       */

      /**
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
       * @brief Detach a thread.
       * @par Parameters
       *  None.
       * @retval result::ok The thread was detached.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       */
      result_t
      detach (void);

      /**
       * @brief Wait for thread termination.
       * @param [in] exit_ptr Pointer to thread exit value. (Optional, may be nullptr).
       * @retval result::ok The thread was terminated.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       */
      result_t
      join (void** exit_ptr = nullptr);

      // Accessors & mutators.

      /**
       * @brief Set the assigned scheduling priority.
       * @param [in] prio New priority.
       * @retval result::ok The priority was set.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EINVAL The value of prio is invalid for the
       *  scheduling policy of the specified thread.
       */
      result_t
      priority (priority_t prio);

      /**
       * @brief Set the inherited scheduling priority.
       * @param [in] prio New priority.
       * @retval result::ok The priority was set.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EINVAL The value of prio is invalid for the
       *  scheduling policy of the specified thread.
       */
      result_t
      priority_inherited (priority_t prio);

      /**
       * @brief Get the current scheduling priority.
       * @par Parameters
       *  None.
       * @return The thread priority.
       */
      priority_t
      priority (void);

      /**
       * @brief Get the inherited scheduling priority.
       * @par Parameters
       *  None.
       * @return The thread inherited priority. May be `priority::none`.
       */
      priority_t
      priority_inherited (void);

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
       *  None.
       * @retval true The thread was interrupted.
       * @retval false The thread was not interrupted.
       */
      bool
      interrupted (void);

      /**
       * @brief Set the interrupt flag, possibly interrupting the thread.
       * @param [in] interrupt Flag.
       * @return The previous value of the interrupt flag.
       */
      bool
      interrupt (bool interrupt = true);

      /**
       * @brief Get thread scheduler state.
       * @par Parameters
       *  None.
       * @return Thread scheduler state.
       */
      state_t
      state (void) const;

      /**
       * @brief Resume the thread.
       * @par Parameters
       *  None.
       * @return  Nothing.
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
       * @brief Get the user storage.
       * @par Parameters
       *  None.
       * @return The address of the thread user storage.
       */
      os_thread_user_storage_t*
      user_storage (void);

      /**
       * @brief Raise thread event flags.
       * @param [in] mask The OR-ed flags to raise.
       * @param [out] oflags Optional pointer where to store the
       *  previous flags; may be `nullptr`.
       * @retval result::ok The flags were raised.
       * @retval EINVAL The mask is zero.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       */
      result_t
      flags_raise (flags::mask_t mask, flags::mask_t* oflags = nullptr);

#if defined(OS_INCLUDE_RTOS_THREAD_PUBLIC_FLAGS_CLEAR)

      // This is a kludge required to support CMSIS RTOS V1
      // public osSignalClear().
      result_t
      flags_clear (flags::mask_t mask, flags::mask_t* oflags);

#endif

      /**
       * @brief Force thread termination.
       * @par Parameters
       *  None.
       * @retval result::ok The tread was terminated.
       */
      result_t
      kill (void);

      /**
       * @brief Get the thread context stack.
       * @par Parameters
       *  None.
       * @return A reference to the context stack object instance.
       */
      class thread::stack&
      stack (void);

#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES) \
  || defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)

      class thread::statistics&
      statistics (void);

#endif

      /**
       * @}
       */

    protected:

      /**
       * @name Private Friends
       * @{
       */

      /**
       * @cond ignore
       */

      friend class mutex;

      friend void
      this_thread::suspend (void);

      friend void
      this_thread::exit (void* exit_ptr);

      friend result_t
      this_thread::flags_wait (flags::mask_t mask, flags::mask_t* oflags,
                               flags::mode_t mode);

      friend result_t
      this_thread::flags_try_wait (flags::mask_t mask, flags::mask_t* oflags,
                                   flags::mode_t mode);
      friend result_t
      this_thread::flags_timed_wait (flags::mask_t mask,
                                     clock::duration_t timeout,
                                     flags::mask_t* oflags, flags::mode_t mode);

      friend result_t
      this_thread::flags_clear (flags::mask_t mask, flags::mask_t* oflags);

      friend flags::mask_t
      this_thread::flags_get (flags::mask_t mask, flags::mode_t mode);

      friend int*
      this_thread::__errno (void);

      friend void
      scheduler::internal_link_node (internal::waiting_threads_list& list,
                                     internal::waiting_thread_node& node);

      friend void
      scheduler::internal_unlink_node (internal::waiting_thread_node& node);

      friend void
      scheduler::internal_link_node (
          internal::waiting_threads_list& list,
          internal::waiting_thread_node& node,
          internal::clock_timestamps_list& timeout_list,
          internal::timeout_thread_node& timeout_node);

      friend void
      scheduler::internal_unlink_node (
          internal::waiting_thread_node& node,
          internal::timeout_thread_node& timeout_node);

      friend void
      scheduler::internal_switch_threads (void);

      friend void
      port::scheduler::reschedule (void);

      friend void
      port::scheduler::start (void);

      friend port::stack::element_t*
      port::scheduler::switch_stacks (port::stack::element_t* sp);

      friend void*
      ::os_idle (func_args_t args);

      friend class internal::ready_threads_list;
      friend class internal::thread_children_list;
      friend class internal::waiting_threads_list;
      friend class internal::clock_timestamps_list;
      friend class internal::terminated_threads_list;

      friend class clock;
      friend class condition_variable;
      friend class mutex;

      /**
       * @endcond
       */

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
       * @brief Internal function used during thread construction.
       * @param [in] function Pointer to thread function.
       * @param [in] args Pointer to thread function arguments.
       * @param [in] attr Reference to attributes.
       * @param [in] stack_address Pointer to stack storage or nullptr.
       * @param [in] stack_size_bytes Size of stack storage or 0.
       */
      void
      internal_construct_ (func_t function, func_args_t args,
                           const attributes& attr, void* stack_address,
                           std::size_t stack_size_bytes);

      /**
       * @brief Suspend this thread and wait for an event.
       * @par Parameters
       *  None.
       * @par Returns
       *  Nothing.
       */
      void
      internal_suspend_ (void);

      /**
       * @brief Terminate thread by itself.
       * @param [in] exit_ptr Pointer to object to return (optional).
       * @return  Nothing.
       */
      [[noreturn]]
      void
      internal_exit_ (void* exit_ptr = nullptr);

      /**
       * @brief Invoke terminating thread function.
       * @param [in] thread The static `this`.
       * @par Returns
       *  Nothing.
       */
      [[noreturn]]
      static void
      internal_invoke_with_exit_ (thread* thread);

      /**
       * @brief Wait for event flags.
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
      internal_flags_wait_ (flags::mask_t mask, flags::mask_t* oflags,
                            flags::mode_t mode);

      /**
       * @brief Try to wait for event flags.
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
      internal_flags_try_wait_ (flags::mask_t mask, flags::mask_t* oflags,
                                flags::mode_t mode);

      /**
       * @brief Timed wait for event flags.
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
      internal_flags_timed_wait_ (flags::mask_t mask, clock::duration_t timeout,
                                  flags::mask_t* oflags, flags::mode_t mode);

      /**
       * @brief Internal wait for event flags.
       * @param [in] mask The expected flags (OR-ed bit-mask);
       *  may be zero.
       * @param [out] oflags Pointer where to store the current flags;
       *  may be `nullptr`.
       * @param [in] mode Mode bits to select if either all or any flags
       *  are expected, and if the flags should be cleared.
       * @retval true All expected flags are raised.
       * @retval false The expected flags are not raised.
       */
      bool
      internal_try_wait_ (flags::mask_t mask, flags::mask_t* oflags,
                          flags::mode_t mode);

      /**
       * @brief Clear thread event flags.
       * @param [in] mask The OR-ed flags to clear. Zero means 'all'
       * @param [out] oflags Optional pointer where to store the
       *  previous flags; may be `nullptr`.
       * @retval result::ok The flags were cleared.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EINVAL The mask is zero.
       */
      result_t
      internal_flags_clear_ (flags::mask_t mask, flags::mask_t* oflags);

      /**
       * @brief Get/clear thread event flags.
       * @param [in] mask The OR-ed flags to get/clear; may be zero.
       * @param [in] mode Mode bits to select if the flags should be
       *  cleared (the other bits are ignored).
       * @retval flags The selected bits from the current thread
       *  event flags mask.
       * @retval flags::all Cannot be invoked from an Interrupt Service Routines.
       */
      flags::mask_t
      internal_flags_get_ (flags::mask_t mask, flags::mode_t mode);

      /**
       * @brief The actual destructor, also called from exit() and kill().
       * @par Parameters
       *  None.
       * @par Returns
       *  Nothing.
       */
      virtual void
      internal_destroy_ (void);

      /**
       * @par Parameters
       *  None.
       * @par Returns
       *  Nothing.
       */
      void
      internal_relink_running_ (void);

      /**
       * @par Parameters
       *  None.
       * @par Returns
       *  Nothing.
       */
      void
      internal_check_stack_ (void);

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

      // TODO: make it fully intrusive with computed offset.
      internal::waiting_thread_node ready_node_
        { *this };

      int errno_ = 0;

      func_t func_ = nullptr;
      func_args_t func_args_ = nullptr;
      void* func_result_ = nullptr;

      // Pointer to parent, or null for top/detached thread.
      thread* parent_ = nullptr;

    public:

      // Intrusive node used to link this thread to parent list.
      internal::double_list_links child_links_;

      using threads_list = internal::intrusive_list<
      thread, internal::double_list_links, &thread::child_links_>;

      // List of children threads. Force a clear.
      threads_list children_
        { true };

      // List of mutexes that this thread owns.
      internal::double_list mutexes_;

    protected:

      // Thread waiting to join.
      thread* joiner_ = nullptr;

      // Pointer to waiting node (stored on stack)
      internal::waiting_thread_node* waiting_node_ = nullptr;

      // Pointer to timeout node (stored on stack)
      internal::timeout_thread_node* clock_node_ = nullptr;

      /**
       * @brief Pointer to clock to be used for timeouts.
       */
      clock* clock_ = nullptr;

      /**
       * @brief Pointer to allocator.
       */
      const void* allocator_ = nullptr;

      stack::element_t* allocated_stack_address_ = nullptr;

      std::size_t allocated_stack_size_elements_ = 0;

      // TODO: Add a list, to properly process robustness.
      std::size_t volatile acquired_mutexes_ = 0;

      // The thread state is set:
      // - running - in ready_threads_list::unlink_head()
      // - ready - in ready_threads_list::link()
      // - waiting - in clock::internal_wait_until(),
      //              scheduler::internal_link_node()
      //              thread::_timed_flags_wait()
      // - terminated - in state::internal_exit_()
      // - destroyed - in thread::internal_destroy_()
      state_t volatile state_ = state::undefined;

      // There are two values used as thread priority. The main one is
      // assigned via `priority(int)`, and is stored in `prio_assigned_`.
      // This value is normally used by the scheduler.
      // However, to prevent priority inversion, mutexes might temporarily
      // boost priorities via `priority_inherited(int)`; this second
      // value is stored in `prio_inherited_`.

      // POSIX: While a thread is holding a mutex which has been
      // initialised with the mutex::protocol::inherit or
      // mutex::protocol::protect protocol attributes, it shall
      // not be subject to being moved to the tail of the scheduling
      // queue at its priority in the event that its original
      // priority is changed, such as by a POSIX call to sched_setparam().
      priority_t volatile prio_assigned_ = priority::none;
      priority_t volatile prio_inherited_ = priority::none;

      bool volatile interrupted_ = false;

      internal::event_flags event_flags_;

      os_thread_user_storage_t user_storage_;

#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)

      class statistics statistics_;

#endif /* defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES) */

      // Add other internal data

      // Implementation
#if defined(OS_USE_RTOS_PORT_SCHEDULER)
      friend class port::thread;
      os_thread_port_data_t port_;
#endif

      // Better be the last one!
      context context_;

      /**
       * @endcond
       */

      /**
       * @}
       */
    };
    /* class thread */

    /**
     * @brief Template of a POSIX compliant **thread** with allocator.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos-thread
     *
     * @tparam Allocator Standard allocator used to allocate the stack area.
     */
    template<typename Allocator = memory::allocator<void*>>
      class thread_allocated : public thread
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
         * @brief Construct a thread object instance.
         * @param [in] function Pointer to thread function.
         * @param [in] args Pointer to thread function arguments.
         * @param [in] attr Reference to attributes.
         * @param [in] allocator Reference to allocator. Default a
         * local temporary instance.
         */
        thread_allocated (func_t function, func_args_t args,
                          const attributes& attr = initializer,
                          const allocator_type& allocator = allocator_type ());

        /**
         * @brief Construct a named thread object instance.
         * @param [in] name Pointer to name.
         * @param [in] function Pointer to thread function.
         * @param [in] args Pointer to thread function arguments.
         * @param [in] attr Reference to attributes.
         * @param [in] allocator Reference to allocator. Default a
         * local temporary instance.
         */
        thread_allocated (const char* name, func_t function, func_args_t args,
                          const attributes& attr = initializer,
                          const allocator_type& allocator = allocator_type ());

        /**
         * @cond ignore
         */

        // The rule of five.
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
         * @brief Destruct the thread object instance.
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

        /**
         * @cond ignore
         */

        virtual void
        internal_destroy_ (void) override;

        /**
         * @endcond
         */

        /**
         * @}
         */

      };

    /**
     * @brief Template of a POSIX compliant **thread** with local stack.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos-thread
     *
     * @tparam N Size of statically allocated stack in bytes.
     */
    template<std::size_t N = port::stack::default_size_bytes>
      class thread_inclusive : public thread
      {
      public:

        /**
         * @brief Local constant based on template definition.
         */
        static const std::size_t stack_size_bytes = N;

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a thread object instance.
         * @param [in] function Pointer to thread function.
         * @param [in] args Pointer to thread function arguments.
         * @param [in] attr Reference to attributes.
         */
        thread_inclusive (func_t function, func_args_t args,
                          const attributes& attr = initializer);

        /**
         * @brief Construct a named thread object instance.
         * @param [in] name Pointer to name.
         * @param [in] function Pointer to thread function.
         * @param [in] args Pointer to thread function arguments.
         * @param [in] attr Reference to attributes.
         */
        thread_inclusive (const char* name, func_t function, func_args_t args,
                          const attributes& attr = initializer);

        /**
         * @cond ignore
         */

        // The rule of five.
        thread_inclusive (const thread_inclusive&) = delete;
        thread_inclusive (thread_inclusive&&) = delete;
        thread_inclusive&
        operator= (const thread_inclusive&) = delete;
        thread_inclusive&
        operator= (thread_inclusive&&) = delete;

        /**
         * @endcond
         */

        /**
         * @brief Destruct the thread object instance.
         */
        virtual
        ~thread_inclusive ();

        /**
         * @}
         */

      private:

        /**
         * @cond ignore
         */

        stack::allocation_element_t stack_[(stack_size_bytes
            + sizeof(stack::allocation_element_t) - 1)
            / sizeof(stack::allocation_element_t)];

        /**
         * @endcond
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
      suspend (void)
      {
        this_thread::thread ().internal_suspend_ ();
      }

      /**
       * @details
       * If the flags::mode::all bit is set, the function expects
       * all given flags to be raised; otherwise, if the flags::mode::any
       * bit is set, the function expects any single flag to be raised.
       *
       * If the expected event flags are
       * raised, the function returns instantly.
       *
       * Otherwise suspend the execution of the current thread until all/any
       * specified event flags are raised.
       *
       * When the parameter mask is 0, the thread is suspended
       * until any event flag is raised. In this case, if any event flags
       * are already raised, the function returns instantly.
       *
       * If the flags::mode::clear bit is set, the event flags that are
       * returned are automatically cleared.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline result_t
      flags_wait (flags::mask_t mask, flags::mask_t* oflags, flags::mode_t mode)
      {
        return this_thread::thread ().internal_flags_wait_ (mask, oflags, mode);
      }

      /**
       * @details
       * If the flags::mode::all bit is set, the function expects
       * all given flags to be raised; otherwise, if the flags::mode::any
       * bit is set, the function expects any single flag to be raised.
       *
       * The function does not blocks, if the expected event flags are
       * not raised, but returns `EGAIN`.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline result_t
      flags_try_wait (flags::mask_t mask, flags::mask_t* oflags,
                      flags::mode_t mode)
      {
        return this_thread::thread ().internal_flags_try_wait_ (mask, oflags,
                                                                mode);
      }

      /**
       * @details
       * If the flags::mode::all bit is set, the function expects
       * all given flags to be raised; otherwise, if the flags::mode::any
       * bit is set, the function expects any single flag to be raised.
       *
       * If the expected event flags are
       * raised, the function returns instantly.
       *
       * Otherwise suspend the execution of the thread until all/any
       * specified event flags are raised.
       *
       * When the parameter mask is 0, the thread is suspended
       * until any event flag is raised. In this case, if any event flags
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
       * if the event flags are already raised. The validity of
       * the timeout need not be checked if the expected flags
       * are already raised and the call can return immediately.
       *
       * The clock used for timeouts can be specified via the `clock`
       * attribute. By default, the clock derived from the scheduler
       * timer is used, and the durations are expressed in ticks.
       *
       * If the flags::mode::clear bit is set, the event flags that are
       * returned are automatically cleared.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline result_t
      flags_timed_wait (flags::mask_t mask, clock::duration_t timeout,
                        flags::mask_t* oflags, flags::mode_t mode)
      {
        return this_thread::thread ().internal_flags_timed_wait_ (mask, timeout,
                                                                  oflags, mode);
      }

      /**
       * @details
       * Select the requested bits from the thread current flags mask
       * and return them. If requested, clear the selected bits in the
       * thread flags mask.
       *
       * If the mask is zero, return the full thread flags mask,
       * without any masking or subsequent clearing.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline flags::mask_t
      flags_get (flags::mask_t mask, flags::mode_t mode)
      {
        return this_thread::thread ().internal_flags_get_ (mask, mode);
      }

      /**
       * @details
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline result_t
      flags_clear (flags::mask_t mask, flags::mask_t* oflags)
      {
        return this_thread::thread ().internal_flags_clear_ (mask, oflags);
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
       * exit code.
       * The behaviour of `internal_exit_()` is undefined if called from a
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
        this_thread::thread ().internal_exit_ (exit_ptr);
      }

      /**
       * @details
       * The actual C library function, used by newlib,
       * is in `os-core.cpp`.
       *
       * @see __errno()
       *
       */
      inline int*
      __attribute__ ((always_inline))
      __errno (void)
      {
        return &this_thread::thread ().errno_;
      }

    } /* namespace this_thread */

    constexpr
    thread::attributes::attributes ()
    {
      ;
    }

    // ======================================================================

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    inline
    thread::stack::stack ()
    {
      clear ();
    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    inline void
    thread::stack::clear (void)
    {
      bottom_address_ = nullptr;
      size_bytes_ = 0;
    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    inline void
    thread::stack::set (stack::element_t* address, std::size_t size_bytes)
    {
      bottom_address_ = address;
      size_bytes_ = size_bytes;
    }

    /**
     * @details
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    inline thread::stack::element_t*
    thread::stack::bottom (void)
    {
      return bottom_address_;
    }

    /**
     * @details
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    inline thread::stack::element_t*
    thread::stack::top (void)
    {
      return bottom_address_ + (size_bytes_ / sizeof(element_t));
    }

    /**
     * @details
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    inline std::size_t
    thread::stack::size (void)
    {
      return size_bytes_;
    }

    /**
     * @details
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    inline bool
    thread::stack::check_bottom_magic (void)
    {
      return *bottom () == stack::magic;
    }

    /**
     * @details
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    inline bool
    thread::stack::check_top_magic (void)
    {
      return *top () == stack::magic;
    }

    /**
     * @details
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    inline std::size_t
    thread::stack::min_size (void)
    {
      return min_size_bytes_;
    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    inline std::size_t
    thread::stack::min_size (std::size_t size_bytes)
    {
      std::size_t tmp = min_size_bytes_;
      min_size_bytes_ = size_bytes;
      return tmp;
    }

    /**
     * @details
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    inline std::size_t
    thread::stack::default_size (void)
    {
      return default_size_bytes_;
    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    inline std::size_t
    thread::stack::default_size (std::size_t size_bytes)
    {
      assert(size_bytes != 0);
      assert(size_bytes >= min_size_bytes_);

      std::size_t tmp = default_size_bytes_;
      default_size_bytes_ = size_bytes;
      return tmp;
    }

    // ========================================================================

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    inline
    thread::context::context ()
    {
      ;
    }

    /**
     * @details
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    inline class thread::stack&
    thread::context::stack ()
    {
      return stack_;
    }

    // ========================================================================

#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)

    /**
     * @details
     * Each time the scheduler performs a context switch, it increments
     * the thread counter and the scheduler total counter.
     *
     * This value can be used together with the corresponding
     * scheduler function, to compute percentages.
     *
     * @note This function is available only when
     * @ref OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES
     * is defined.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    inline statistics::counter_t
    thread::statistics::context_switches (void)
    {
      return context_switches_;
    }

#endif /* defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES) */

#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)

    /**
     * @details
     *
     * This value can be used together with the corresponding
     * scheduler function, to compute percentages.
     *
     * @note This function is available only when
     * @ref OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES
     * is defined.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    inline rtos::statistics::duration_t
    thread::statistics::cpu_cycles (void)
    {
      return cpu_cycles_;
    }

#endif /* defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES) */

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

    /**
     * @details
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    inline thread::state_t
    thread::state (void) const
    {
      return state_;
    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    inline void*
    thread::function_args (void) const
    {
      return func_args_;
    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
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
     *
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    inline os_thread_user_storage_t*
    thread::user_storage (void)
    {
      return &user_storage_;
    }

#if !defined(OS_USE_RTOS_PORT_SCHEDULER)

    /**
     * @cond ignore
     */

    inline void
    thread::internal_relink_running_ (void)
    {
      if (state_ == state::running)
        {
          // If the current thread is running, add it to the
          // ready list, so that it will be resumed later.
          internal::waiting_thread_node& crt_node = ready_node_;
          if (crt_node.next () == nullptr)
            {
              rtos::scheduler::ready_threads_list_.link (crt_node);
              // Ready state set in above link().
            }

          // Simple test to verify that the old thread
          // did not underflow the stack.
          assert(stack ().check_bottom_magic ());
        }
    }

    /**
     * @endcond
     */

#endif

    /**
     * @details
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    inline class thread::stack&
    thread::stack (void)
    {
      return context_.stack_;
    }

#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    inline class thread::statistics&
    thread::statistics (void)
    {
      return statistics_;
    }

#endif /* defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES) */

#if defined(OS_INCLUDE_RTOS_THREAD_PUBLIC_FLAGS_CLEAR)

    inline result_t
    thread::flags_clear (flags::mask_t mask, flags::mask_t* oflags)
    {
      return internal_flags_clear_ (mask, oflags);
    }

#endif

    // ========================================================================

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
     * the return value of function as the exit code. Note that
     * the thread in which `main()` was originally invoked differs
     * from this. When it returns from `main()`, the effect shall
     * be as if there was an implicit call to `exit()` using the
     * return value of `main()` as the exit code.
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
      thread_allocated<Allocator>::thread_allocated (
          func_t function, func_args_t args, const attributes& attr,
          const allocator_type& allocator) :
          thread_allocated
            { nullptr, function, args, attr, allocator }
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
     * the return value of function as the exit code. Note that
     * the thread in which `main()` was originally invoked differs
     * from this. When it returns from `main()`, the effect shall
     * be as if there was an implicit call to `exit()` using the
     * return value of `main()` as the exit code.
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
      thread_allocated<Allocator>::thread_allocated (
          const char* name, func_t function, func_args_t args,
          const attributes& attr, const allocator_type& allocator) :
          thread
            { name }
      {
#if defined(OS_TRACE_RTOS_THREAD)
        trace::printf ("%s @%p %s\n", __func__, this, this->name ());
#endif
        if (attr.th_stack_address != nullptr
            && attr.th_stack_size_bytes > stack::min_size ())
          {
            internal_construct_ (function, args, attr, nullptr, 0);
          }
        else
          {
            allocator_ = &allocator;

            if (attr.th_stack_size_bytes > stack::min_size ())
              {
                allocated_stack_size_elements_ = (attr.th_stack_size_bytes
                    + sizeof(typename allocator_type::value_type) - 1)
                    / sizeof(typename allocator_type::value_type);
              }
            else
              {
                allocated_stack_size_elements_ = (stack::default_size ()
                    + sizeof(typename allocator_type::value_type) - 1)
                    / sizeof(typename allocator_type::value_type);
              }

            // The reinterpret_cast<> is required since the allocator
            // uses allocation_element_t, which is usually larger.
            allocated_stack_address_ =
                reinterpret_cast<stack::element_t*> ((const_cast<allocator_type&> (allocator)).allocate (
                    allocated_stack_size_elements_));

            assert(allocated_stack_address_ != nullptr);

            internal_construct_ (
                function,
                args,
                attr,
                allocated_stack_address_,
                allocated_stack_size_elements_
                    * sizeof(typename allocator_type::value_type));
          }
      }

    /**
     * @cond ignore
     */

    template<typename Allocator>
      void
      thread_allocated<Allocator>::internal_destroy_ (void)
      {
#if defined(OS_TRACE_RTOS_THREAD)
        trace::printf ("thread_allocated::%s() @%p %s\n", __func__, this,
                       name ());
#endif

        if (allocated_stack_address_ != nullptr)
          {
            internal_check_stack_ ();

            typedef typename std::allocator_traits<allocator_type>::pointer pointer;

            static_cast<allocator_type*> (const_cast<void*> (allocator_))->deallocate (
                reinterpret_cast<pointer> (allocated_stack_address_),
                allocated_stack_size_elements_);

            allocated_stack_address_ = nullptr;
          }

        thread::internal_destroy_ ();
      }

    /**
     * @endcond
     */

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
     * the return value of function as the exit code. Note that
     * the thread in which `main()` was originally invoked differs
     * from this. When it returns from `main()`, the effect shall
     * be as if there was an implicit call to `exit()` using the
     * return value of `main()` as the exit code.
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
      thread_inclusive<N>::thread_inclusive (func_t function, func_args_t args,
                                             const attributes& attr) :
          thread_inclusive<N>
            { nullptr, function, args, attr }
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
     * the return value of function as the exit code. Note that
     * the thread in which `main()` was originally invoked differs
     * from this. When it returns from `main()`, the effect shall
     * be as if there was an implicit call to `exit()` using the
     * return value of `main()` as the exit code.
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
      thread_inclusive<N>::thread_inclusive (const char* name, func_t function,
                                             func_args_t args,
                                             const attributes& attr) :
          thread
            { name }
      {
#if defined(OS_TRACE_RTOS_THREAD)
        trace::printf ("%s @%p %s\n", __func__, this, this->name ());
#endif
        internal_construct_ (function, args, attr, &stack_, stack_size_bytes);
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
      thread_inclusive<N>::~thread_inclusive ()
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
