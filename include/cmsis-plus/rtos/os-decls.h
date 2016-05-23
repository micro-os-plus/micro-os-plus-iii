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

#ifndef CMSIS_PLUS_RTOS_OS_DECLS_H_
#define CMSIS_PLUS_RTOS_OS_DECLS_H_

// ----------------------------------------------------------------------------

/**
 * @brief Tell the world that CMSIS++ is in use.
 * @details
 * Macro to inform including files that CMSIS++ RTOS
 * definitions are available.
 */
#define OS_USE_CMSIS_PLUS

// Include the application specific definitions.
#include <cmsis-plus/rtos/os-app-config.h>
// Include the non-portable portable types, enums and constants declarations.
#include <cmsis-plus/rtos/port/os-decls.h>

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

#include <cstdint>
#include <cstddef>
#include <cerrno>

/**
 * @brief System namespace.
 */
namespace os
{
  /**
   * @brief RTOS namespace.
   */
  namespace rtos
  {
    // ========================================================================

    // Forward references.
    class named_object;
    class clock;
    class condition_variable;
    class event_flags;
    class memory_pool;
    class message_queue;
    class mutex;
    class clock_rtc;
    class semaphore;
    class clock_systick;
    class thread;
    class timer;

    class waiting_threads_list;
    class clock_threads_list;
    class waiting_threads_list3;
    class clock_timestamps_list;

    namespace memory
    {
      template<typename T>
        class new_delete_allocator;

      template<typename T>
        class polymorphic_allocator;
    } /* namespace memory */

    // ------------------------------------------------------------------------

    /**
     * @brief Type of values returned by RTOS functions.
     * @details
     * For error processing reasons, most CMSIS++ RTOS functions
     * return a numeric result, which, according to POSIX,
     * when the call was successful, must be `0`
     * (`result::ok`) or an error code defined in `<errno.h>` otherwise.
     */
    using result_t = uint32_t;

    /**
     * @brief Values returned by RTOS functions.
     * @details
     * This namespace is dedicated to grouping all
     * status code values returned by CMSIS++ RTOS functions.
     *
     * However, CMSIS++ favours POSIX error codes, so,
     * except a few enumerated values, most of them are exactly those
     * defined by POSIX, in the
     * `<errno.h>` header, and are not redefined here.
     *
     * Currently in use are:
     * - `EPERM` - Operation not permitted. An attempt was made to perform
     * an operation limited to processes with appropriate privileges
     * or to the owner of a file or other resource. In CMSIS++ this
     * usually means that the call is not available in handler mode.
     * - `EINVAL` - Invalid argument. Some invalid argument was supplied;
     * - `EWOULDBLOCK` - Operation would block.
     * In CMSIS++ case, this usually means that a call to `try_xxx()`
     * found the resource busy.
     * - `EAGAIN` - Resource temporarily unavailable. This is a temporary
     * condition and later calls to the same routine may complete normally.
     * - `ENOTRECOVERABLE` - State not recoverable. In CMSIS++ this
     * usually means an unrecoverable error occurred.
     * - `EDEADLOCK` - Resource deadlock would occur. An attempt was made
     * to lock a system resource that would have resulted in a deadlock
     * situation.
     * - `EMSGSIZE` - Message too large. A message sent on a transport
     * provider was larger than an internal message buffer or some other
     * network limit, or inappropriate message buffer length.
     * - `EBADMSG` - Bad message. The implementation has detected a
     * corrupted message.
     * - `EINTR` - Interrupted function call. In CMSIS++ this
     * usually mens that a thread waiting for a message is waken
     * before the event or the timeout occurred, at user request.
     * - `ETIMEDOUT` - Operation timed out. The time limit associated
     * with the operation was exceeded before the operation completed.
     * - `EOWNERDEAD` - Previous owner died. The owner of a robust mutex
     * terminated while holding the mutex lock.
     *
     * @par Example
     *
     * @code{.cpp}
     * void
     * func (void)
     * {
     *    mutex mx;
     *    ...
     *    result_t res = mx.try_lock();
     *    if (res == result::ok)
     *      {
     *        // All is well, mutex locked.
     *      }
     *    else if (res == EWOULDBLOCK)
     *      {
     *        // Mutex busy, try again later.
     *      }
     * }
     * @endcode
     *
     */
    namespace result
    {
      /**
       * @brief Custom enumerated values.
       * @details
       * There are not many custom values returned by
       * CMSIS++ RTOS functions, currently there is only one,
       * `ok`, represented by `0`.
       *
       * If more custom codes will be needed and are not present in POSIX,
       * this is the place where to add them. Just be sure their numeric
       * values do not overlap POSIX values (check for a definition with
       * the last allocated POSIX error number).
       */
      enum
        : result_t
          {
            /**
             * @brief Function completed; no errors or events occurred.
             */
            ok = 0,

      };
    } /* namespace result */

    // ------------------------------------------------------------------------

    //using DoubleListNodeClock = DoubleListNodeTimestamp<clock::timestamp_t>;
    //using Waiting_threads_list3 = Waiting_threads_list2<DoubleListNodeClock>;

    // ------------------------------------------------------------------------

    /**
     * @brief Scheduler namespace.
     * @ingroup cmsis-plus-rtos
     */
    namespace scheduler
    {
      /**
       * @brief Type of a variable holding scheduler status codes.
       * @details
       * Usually a boolean telling if the scheduler is
       * locked or not, but for recursive locks it might also be a
       * numeric counter.
       */
      using status_t = bool;

      constexpr status_t init_status = false;

    } /* namespace scheduler */

    /**
     * @brief Interrupts namespace.
     */
    namespace interrupts
    {
      /**
       * @brief Type of a variable holding interrupts status.
       * @details
       * Usually an integer large enough to hold the CPU status register
       * where the interrupt status is stored.
       *
       * It is used to temporarily store the CPU status register
       * during critical sections.
       */
      using status_t = port::interrupts::status_t;

    } /* namespace interrupts */

    // ----------------------------------------------------------------------

    /**
     * @brief Generic flags namespace.
     * @ingroup cmsis-plus-rtos
     */
    namespace flags
    {
      /**
       * @brief Type of a variable holding a flags mask.
       * @details
       * An unsigned type large enough to store all the flags, usually
       * 32-bits wide.
       *
       * Both thread signal flags and event flags use this definition.
       * @ingroup cmsis-plus-rtos
       */
      using mask_t = uint32_t;

      /**
       * @brief Type of a variable holding the flags mode.
       * @details
       * An unsigned type used to hold the mode bits passed to
       * functions returning flags.
       *
       * Both thread signal flags and event flags use this definition.
       * @ingroup cmsis-plus-rtos
       */
      using mode_t = uint32_t;

      /**
       * @brief Flags modes.
       * @details
       * Container for generic flags enumerations.
       * @ingroup cmsis-plus-rtos
       */
      namespace mode
      {
        /**
         * @brief Bits used to specify the flags mode.
         * @ingroup cmsis-plus-rtos
         */
        enum
          : mode_t
            {
              /**
               * @brief Return when all flags are set.
               */
              all = 1,

              /**
               * @brief Return when at least one flag is set.
               */
              any = 2,

              /**
               * @brief Ask for flags to be cleared after read.
               */
              clear = 4
        };
      } /* namespace mode */
    } /* namespace flags */

    /**
     * @brief A convenience namespace to access the current running thread.
     * @ingroup cmsis-plus-rtos
     */
    namespace this_thread
    {
    }

    // ========================================================================

    /**
     * @brief Base class for named objects.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     */
    class named_object
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      named_object ();

      /**
       * @brief Create a named object.
       * @param [in] name Null terminated name. If `nullptr`,
       * "-" is assigned.
       */
      named_object (const char* name);

      /**
       * @cond ignore
       */
      named_object (const named_object&) = default;
      named_object (named_object&&) = default;
      named_object&
      operator= (const named_object&) = default;
      named_object&
      operator= (named_object&&) = default;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the named object.
       */
      ~named_object () = default;

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Get name.
       * @par Parameters
       *  None.
       * @return A null terminated string.
       */
      const char*
      name (void) const;

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Variables
       * @{
       */

      /**
       * @brief Pointer to name.
       */
      const char* const name_ = "-";

      /**
       * @}
       */
    };

    // ========================================================================

    /**
     * @brief Base class for attributes.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     */
    class clocked_attributes
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a named object.
       * @par Parameters
       *  None.
       */
      constexpr
      clocked_attributes ();

      /**
       * @cond ignore
       */
      clocked_attributes (const clocked_attributes&) = default;
      clocked_attributes (clocked_attributes&&) = default;
      clocked_attributes&
      operator= (const clocked_attributes&) = default;
      clocked_attributes&
      operator= (clocked_attributes&&) = default;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the named object.
       */
      ~clocked_attributes () = default;

      /**
       * @}
       * @name Public Member Variables
       * @{
       */

      /**
       * @brief Pointer to clock.
       */
      rtos::clock* clock = nullptr;

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Variables
       * @{
       */

      /**
       * @}
       */
    };

  // ==========================================================================
  } /* namespace rtos */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace rtos
  {
    // ------------------------------------------------------------------------

    /**
     * @details
     * All objects return a non-null string; anonymous objects
     * return `"-"`.
     */
    inline const char*
    named_object::name (void) const
    {
      return name_;
    }

    // ========================================================================

    constexpr
    clocked_attributes::clocked_attributes ()
    {
      ;
    }

  // ==========================================================================
  } /* namespace rtos */
} /* namespace os */

// ===== Port related definitions =============================================

namespace os
{
  namespace rtos
  {
    namespace port
    {
      class thread;
      class timer;
      class mutex;

      class condition_variable;
      class semaphore;
      class memory_pool;
      class message_queue;
      class event_flags;

      namespace clock
      {
        /**
         * @brief Type of variables holding timer durations.
         * @details
         * A numeric type intended to hold a generic duration, either in ticks
         * or in seconds.
         */
        using duration_t = uint32_t;

        /**
         * @brief Type of variables holding time stamps.
         * @details
         * A numeric type intended to hold a generic timestamp, either in ticks
         * or in seconds.
         */
        using timestamp_t = uint64_t;

        using offset_t = int64_t;
      } /* namespace clock */

      namespace interrupts
      {

        // ====================================================================

        class critical_section
        {
        public:

          critical_section () = delete;

          // Enter an IRQ critical section
          static rtos::interrupts::status_t
          enter (void);

          // Exit an IRQ critical section
          static void
          exit (rtos::interrupts::status_t status);

        };

        // ====================================================================

        class uncritical_section
        {
        public:

          uncritical_section () = delete;

          // Enter an IRQ uncritical section
          static rtos::interrupts::status_t
          enter (void);

          // Exit an IRQ uncritical section
          static void
          exit (rtos::interrupts::status_t status);

        };

      } /* namespace interrupts */

      namespace scheduler
      {

        void
        greeting (void);

        result_t
        initialize (void);

        [[noreturn]] void
        start (void);

        bool
        in_handler_mode (void);

        void
        lock (rtos::scheduler::status_t status);

        void
        reschedule (void);

        stack::element_t*
        switch_stacks (stack::element_t* sp);

        void
        _wait_for_interrupt (void);

      } /* namespace scheduler */

      namespace this_thread
      {

        void
        prepare_suspend (void);

        void
        yield (void);

      } /* namespace this_thread */

      // ====================================================================

      class context
      {
      public:

        context () = delete;

        // Used to avoid a complex casts below,
        // that might confuse the Eclipse formatter.
        typedef void
        (*func_t) (void);

        static void
        create (void* context, void* func, void* args);

      };
      /* class context */

      // ======================================================================
      class thread
      {
      public:

        thread () = delete;

#if defined(OS_INCLUDE_RTOS_PORT_SCHEDULER)

        static void
        create (rtos::thread* obj);

        [[noreturn]]
        static void
        destroy_this (rtos::thread* obj);

        static void
        destroy_other (rtos::thread* obj);

        static void
        resume (rtos::thread* obj);

        static rtos::thread::priority_t
        sched_prio (rtos::thread* obj);

        static result_t
        sched_prio (rtos::thread* obj, rtos::thread::priority_t prio);

        static result_t
        detach (rtos::thread* obj);

#endif

      };
      /* class thread */

      // ======================================================================
      class clock_systick
      {
      public:

        clock_systick () = delete;

        static void
        start (void);

        static result_t
        wait_for (clock::duration_t ticks);

        /**
         * @brief SysTick implementation hook.
         * @details
         * It is called from `os_systick_handler()` after the
         * scheduler was started.
         */
        static void
        _interrupt_service_routine (void);

      };

      class clock_rtc
      {
      public:

        clock_rtc () = delete;

        /**
         * @brief RTC implementation hook.
         * @details
         * It is called from `os_systick_handler()` after the
         * scheduler was started.
         */
        static void
        _interrupt_service_routine (void);

      };

    } /* namespace port */
  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

// Must be included after the declarations
#include <cmsis-plus/rtos/os-lists.h>

// ----------------------------------------------------------------------------

extern "C"
{

  /**
   * @brief Main thread.
   * @param argc Count of arguments.
   * @param argv Array of pointers to arguments.
   * @retval 0 The proram terminated normally.
   * @retval 1 The program terminated with an error.
   *
   * @details
   * The standard `main()` creates a dedicated thread to run this function.
   *
   * The returned value is used in semihosted tests, to inform the
   * host on the result of the test.
   */
  int
  os_main (int argc, char* argv[]);

#if !defined(OS_INCLUDE_RTOS_CUSTOM_THREAD_USER_STORAGE)
  /**
   * @brief Default empty thread user storage.
   *
   * If the application requires to store some additional data
   * to each thread, redefine this structure in the `<os-app-conf.h>`
   * file.
   *
   * To get the address of the user storage associated with a
   * given thread, use thread::user_storage().
   */
  typedef struct
  {
    char dummy;
  } os_thread_user_storage_t;
#endif
}

// ----------------------------------------------------------------------------

#include <cmsis-plus/rtos/os-types.h>

// ----------------------------------------------------------------------------

// Required by os_assert_throw()
#include <cmsis-plus/iso/system_error>

/**
 * @brief Assert or return an error.
 * @details
 * As required by the ANSI standards, if `NDEBUG` is defined,
 * the assertion is disabled and if the condition is true, the
 * given error code is returned.
 */
#ifdef NDEBUG           /* ANSI standard */
#define os_assert_err(__e, __er) \
  do { if (!(__e)) return __er; } while (false)
#else
#define os_assert_err(__e, __er) assert(__e)
#endif

/**
 * @brief Assert or throw a system error exception.
 * @details
 * As required by the ANSI standards, if `NDEBUG` is defined,
 * the assertion is disabled and if the condition is true, a
 * ssytem error exception is thrown (which is replaced by an
 * `abort()` if exceptions are disabled).
 */
#ifdef NDEBUG           /* ANSI standard */
#define os_assert_throw(__e, __er) \
  do { if (!(__e)) os::estd::__throw_system_error(__er, #__e); } while (false)
#else
#define os_assert_throw(__e, __er) assert(__e)
#endif

// ----------------------------------------------------------------------------

#if !defined(OS_INTEGER_SYSTICK_FREQUENCY_HZ)
/**
 * @brief Default definition for the SysTick frequency, in Hz.
 * @details
 * Redefine it in `<os-app-config.h>` to the actual value.
 */
#define OS_INTEGER_SYSTICK_FREQUENCY_HZ                     (1000)
#endif

// ----------------------------------------------------------------------------

#if !defined(OS_INTEGER_RTOS_MAIN_STACK_SIZE_BYTES)
/**
 * @brief Default definition for the `main()` stack size, in bytes.
 * @details
 * Redefine it in `<os-app-config.h>` to the actual value.
 *
 * @note Ignored for synthetic platforms.
 */
#define OS_INTEGER_RTOS_MAIN_STACK_SIZE_BYTES               (os::rtos::port::stack::default_size_bytes)
#endif

#if !defined(OS_INTEGER_RTOS_IDLE_STACK_SIZE_BYTES)
/**
 * @brief Default definition for the idle thread stack size, in bytes.
 * @details
 * Redefine it in `<os-app-config.h>` to the actual value.
 *
 * @note Ignored for synthetic platforms.
 */
#define OS_INTEGER_RTOS_IDLE_STACK_SIZE_BYTES               (os::rtos::port::stack::default_size_bytes)
#endif

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_DECLS_H_ */
