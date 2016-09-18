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

#include <cmsis-plus/os-versions.h>

/**
 * @brief Tell the world that CMSIS++ is in use.
 * @details
 * Macro to inform including files that CMSIS++ RTOS
 * definitions are available.
 */
#define OS_USE_CMSIS_PLUS

// Include the application specific definitions.
#include <cmsis-plus/os-app-config.h>
// Include the non-portable portable types, enums and constants declarations.
#include <cmsis-plus/rtos/port/os-decls.h>

#include <cmsis-plus/diag/trace.h>

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

#include <cstdint>
#include <cstddef>
#include <cerrno>
#include <cstring>

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
    class clock;
    class clock_rtc;
    class clock_systick;

    class condition_variable;
    class event_flags;
    class memory_pool;
    class message_queue;
    class mutex;
    class semaphore;
    class thread;
    class timer;

    // ------------------------------------------------------------------------

    namespace memory
    {
      template<typename T>
        class allocator_stateless_default_resource;
    } /* namespace memory */

    // ------------------------------------------------------------------------

    /**
     * @brief Type of values returned by RTOS functions.
     *
     * @details
     * For error processing reasons, most CMSIS++ RTOS functions
     * return a numeric result, which, according to POSIX,
     * when the call was successful, must be `0`
     * (`result::ok`) or an error code defined in `<errno.h>` otherwise.
     */
    using result_t = uint32_t;

    /**
     * @brief Values returned by RTOS functions.
     *
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
       *
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

    /**
     * @brief Scheduler namespace.
     * @ingroup cmsis-plus-rtos-core
     */
    namespace scheduler
    {
      /**
       * @brief Type of variables holding scheduler state codes.
       *
       * @details
       * Usually a boolean telling if the scheduler is
       * locked or not, but for recursive locks it might also be a
       * numeric counter.
       */
      using state_t = port::scheduler::state_t;

    } /* namespace scheduler */

    // ------------------------------------------------------------------------

    /**
     * @brief Statistics namespace.
     * @ingroup cmsis-plus-rtos-core
     */
    namespace statistics
    {
      /**
       * @brief Type of variables holding context switches counters.
       */
      using counter_t = uint64_t;

      /**
       * @brief Type of variables holding durations in CPU cycles.
       */
      using duration_t = uint64_t;

    } /* namespace statistics */

    // ------------------------------------------------------------------------

    /**
     * @brief Interrupts namespace.
     * @ingroup cmsis-plus-rtos-core
     */
    namespace interrupts
    {
      /**
       * @brief Type of variables holding interrupts statu codes.
       * @details
       * Usually an integer large enough to hold the CPU register
       * where the interrupt priorities are stored.
       *
       * Used to temporarily store the CPU register
       * during critical sections.
       */
      using state_t = port::interrupts::state_t;

    } /* namespace interrupts */

    // ------------------------------------------------------------------------

    /**
     * @brief Generic flags namespace.
     * @ingroup cmsis-plus-rtos-core
     * @details
     * The os::rtos::flags namespace groups event types and enumerations.
     */
    namespace flags
    {
      /**
       * @brief Type of variables holding flags masks.
       * @details
       * An unsigned type large enough to store all the flags, usually
       * 32-bits wide.
       *
       * Both thread event flags and generic event flags use this definition.
       */
      using mask_t = uint32_t;

      /**
       * @brief Type of variables holding flags modes.
       * @details
       * An unsigned type used to hold the mode bits passed to
       * functions returning flags.
       *
       * Both thread event flags and generic event flags use this definition.
       */
      using mode_t = uint32_t;

      /**
       * @brief Flags modes.
       * @details
       * Container for generic flags enumerations.
       */
      namespace mode
      {
        /**
         * @brief Bits used to specify the flags modes.
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

      /**
       * @brief Flags sets with special meaning.
       */
      enum
        : mask_t
          {
            /**
             * @brief Special mask to represent any flag.
             */
            any = 0,

            /**
             * Special mask to represent all flags.
             */
            all = 0xFFFFFFFF,
      };

    } /* namespace flags */

    // ------------------------------------------------------------------------

    /**
     * @brief A convenience namespace to access the current running thread.
     * @ingroup cmsis-plus-rtos-thread
     */
    namespace this_thread
    {
    }

    // ------------------------------------------------------------------------

    /**
     * @brief A namespace to group all internal implementation objects.
     */
    namespace internal
    {

      // ======================================================================

      /**
       * @brief Base class for named objects.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class object_named
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a named object instance.
         */
        object_named ();

        /**
         * @brief Construct a named object instance.
         * @param [in] name Null terminated name. If `nullptr`,
         * "-" is assigned.
         */
        object_named (const char* name);

        /**
         * @cond ignore
         */
        object_named (const object_named&) = default;
        object_named (object_named&&) = default;
        object_named&
        operator= (const object_named&) = default;
        object_named&
        operator= (object_named&&) = default;
        /**
         * @endcond
         */

        /**
         * @brief Destruct the named object instance.
         */
        ~object_named () = default;

        /**
         * @}
         */

      public:

        /**
         * @name Public Member Functions
         * @{
         */

        /**
         * @brief Get object name.
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
         * @cond ignore
         */

        /**
         * @brief Pointer to name.
         */
        const char* const name_ = "-";

        /**
         * @endcond
         */

        /**
         * @}
         */
      };

      // ======================================================================

      /**
       * @brief Base class for named system objects.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class object_named_system : public object_named
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a named system object instance.
         */
        object_named_system ();

        /**
         * @brief Construct a named system object instance.
         * @param [in] name Null terminated name. If `nullptr`,
         * "-" is assigned.
         */
        object_named_system (const char* name);

        /**
         * @cond ignore
         */
        object_named_system (const object_named_system&) = default;
        object_named_system (object_named_system&&) = default;
        object_named_system&
        operator= (const object_named_system&) = default;
        object_named_system&
        operator= (object_named_system&&) = default;
        /**
         * @endcond
         */

        /**
         * @brief Destruct the named system object instance.
         */
        ~object_named_system () = default;

        /**
         * @}
         */

        /**
         * @name Operators
         * @{
         */

        /**
         * @brief Allocate space for a new object instance using the
         * RTOS system allocator.
         * @param bytes Number of bytes to allocate.
         * @return Pointer to allocated object.
         */
        static void*
        operator new (std::size_t bytes);

        /**
         * @brief Allocate space for an array of new object instances using the
         * RTOS system allocator.
         * @param bytes Number of bytes to allocate.
         * @return Pointer to allocated array.
         */
        static void*
        operator new[] (std::size_t bytes);

        /**
         * @brief Emplace a new object instance.
         * @param bytes Number of bytes to emplace.
         * @param ptr Pointer to location to emplace the object.
         * @return Pointer to emplaced object.
         */
        static void*
        operator new (std::size_t bytes, void* ptr);

        /**
         * @brief Emplace an array of new object instances.
         * @param bytes Number of bytes to emplace.
         * @param ptr Pointer to location to emplace the object.
         * @return Pointer to emplaced array.
         */
        static void*
        operator new[] (std::size_t bytes, void* ptr);

        /**
         * @brief Deallocate the dynamically allocated object instance.
         * using the RTOS system allocator.
         * @param ptr Pointer to object.
         * @param bytes Number of bytes to deallocate.
         * @par Returns
         *  Nothing.
         */
        static void
        operator delete (void* ptr, std::size_t bytes);

        /**
         * @brief Deallocate the dynamically allocated array of object.
         * instances using the RTOS system allocator.
         * @param ptr Pointer to array of objects.
         * @param bytes Number of bytes to deallocate.
         * @par Returns
         *  Nothing.
         */
        static void
        operator delete[] (void* ptr, std::size_t bytes);

        /**
         * @}
         */

      };

      // ======================================================================

      /**
       * @brief Base class for attributes.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class attributes_clocked
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a named object instance.
         * @par Parameters
         *  None.
         */
        constexpr
        attributes_clocked ();

        /**
         * @cond ignore
         */
        attributes_clocked (const attributes_clocked&) = default;
        attributes_clocked (attributes_clocked&&) = default;
        attributes_clocked&
        operator= (const attributes_clocked&) = default;
        attributes_clocked&
        operator= (attributes_clocked&&) = default;
        /**
         * @endcond
         */

        /**
         * @brief Destruct the named object instance.
         */
        ~attributes_clocked () = default;

        /**
         * @}
         */

        /**
         * @name Public Member Variables
         * @{
         */

        /**
         * @brief Attribute with the address of the clock to be used
         *  for timeouts.
         * @details
         * It may be one of `os::rtos::sysclock`, `os::rtos::rtclock`,
         * or any other user object derived from class `os::rtos::clock`.
         *
         * If `nullptr`, the default clock is `os::rtos::sysclock`.
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
    } /* namespace internal */

    // ========================================================================

    /**
     * @brief Null locker.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @details
     * This dummy object can be passed as parameter to templates
     * requiring a lockable, but it does nothing.
     */
    class null_locker
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct a null lockable object instance.
       * @par Parameters
       *  None.
       */

      constexpr
      null_locker ();

      /**
       * @cond ignore
       */

      null_locker (const null_locker&) = delete;
      null_locker (null_locker&&) = delete;
      null_locker&
      operator= (const null_locker&) = delete;
      null_locker&
      operator= (null_locker&&) = delete;

      /**
       * @endcond
       */

      /**
       * @brief Destruct the null lockable object instance.
       */
      ~null_locker ();

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Pretend to lock scheduler.
       * @par Parameters
       *  None.
       * @par Returns
       *  Nothing.
       */
      void
      lock (void);

      /**
       * @brief Pretend to unlock the scheduler.
       * @par Parameters
       *  None.
       * @par Returns
       *  Nothing.
       */
      void
      unlock (void);

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
    // ========================================================================

    inline
    null_locker::~null_locker ()
    {
      ;
    }

    inline void
    null_locker::lock (void)
    {
      ; // Does nothing.
    }

    inline void
    null_locker::unlock (void)
    {
      ; // Does nothing.
    }

    namespace internal
    {
      // ======================================================================

      /**
       * @details
       * All objects return a non-null string; anonymous objects
       * return `"-"`.
       *
       * @note Can be invoked from Interrupt Service Routines.
       */
      inline const char*
      object_named::name (void) const
      {
        return name_;
      }

      // ======================================================================

      inline
      object_named_system::object_named_system ()
      {
        ;
      }

      inline
      object_named_system::object_named_system (const char* name) :
          object_named (name)
      {
        ;
      }

      // ======================================================================

      /**
       * @details
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      constexpr
      attributes_clocked::attributes_clocked ()
      {
        ;
      }

    // ========================================================================
    } /* namespace internal */
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

      // ======================================================================

      namespace clock
      {
        /**
         * @brief Type of variables holding timer durations.
         * @details
         * A numeric type intended to hold a generic duration, either in ticks
         * cycles or seconds.
         */
        using duration_t = uint32_t;

        /**
         * @brief Type of variables holding time stamps.
         * @details
         * A numeric type intended to hold a generic timestamp, either in ticks
         * cycles or seconds.
         */
        using timestamp_t = uint64_t;

        using offset_t = int64_t;
      } /* namespace clock */

      // ----------------------------------------------------------------------

      namespace interrupts
      {

        bool
        in_handler_mode (void);

        // ====================================================================

        class critical_section
        {
        public:

          critical_section () = delete;

          // Enter an IRQ critical section
          static rtos::interrupts::state_t
          enter (void);

          // Exit an IRQ critical section
          static void
          exit (rtos::interrupts::state_t state);

        };

        // ====================================================================

        class uncritical_section
        {
        public:

          uncritical_section () = delete;

          // Enter an IRQ uncritical section
          static rtos::interrupts::state_t
          enter (void);

          // Exit an IRQ uncritical section
          static void
          exit (rtos::interrupts::state_t state);

        };

      } /* namespace interrupts */

      // ----------------------------------------------------------------------

      namespace scheduler
      {

        void
        greeting (void);

        result_t
        initialize (void);

        [[noreturn]] void
        start (void);

        port::scheduler::state_t
        lock (void);

        port::scheduler::state_t
        unlock (void);

        port::scheduler::state_t
        locked (port::scheduler::state_t state);

        bool
        locked (void);

        void
        reschedule (void);

        stack::element_t*
        switch_stacks (stack::element_t* sp);

        void
        _wait_for_interrupt (void);

      } /* namespace scheduler */

      // ----------------------------------------------------------------------

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

#if defined(OS_USE_RTOS_PORT_SCHEDULER)

        static void
        create (rtos::thread* obj);

        [[noreturn]]
        static void
        destroy_this (rtos::thread* obj);

        static void
        destroy_other (rtos::thread* obj);

        static void
        resume (rtos::thread* obj);

        // TODO: move it to port
        using priority_t = uint8_t;

        static priority_t
        priority (rtos::thread* obj);

        static result_t
        priority (rtos::thread* obj, priority_t prio);

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
        internal_interrupt_service_routine (void);

      };

      // ======================================================================

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
        internal_interrupt_service_routine (void);

      };

      // ======================================================================

      class clock_highres
      {
      public:

        clock_highres () = delete;

        static void
        start (void);

        static uint32_t
        cycles_per_tick (void);

        static uint32_t
        cycles_since_tick (void);

        static uint32_t
        input_clock_frequency_hz (void);
      };

    // ========================================================================

    } /* namespace port */
  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

// Must be included after the declarations
#include <cmsis-plus/rtos/internal/os-lists.h>

// ----------------------------------------------------------------------------

extern "C"
{

  /**
   * @cond ignore
   */

  int
  os_main (int argc, char* argv[]);

/**
 * @endcond
 */

}

// ----------------------------------------------------------------------------

#include <cmsis-plus/rtos/os-types.h>

// ----------------------------------------------------------------------------

// Required by os_assert_throw()
#include <cmsis-plus/estd/system_error>

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

#endif /* __cplusplus */

// ----------------------------------------------------------------------------

// Default definitions for various configuration macros.
// Redefine them in `<os-app-config.h>` to the actual values.

#if !defined(OS_INTEGER_SYSTICK_FREQUENCY_HZ)
#define OS_INTEGER_SYSTICK_FREQUENCY_HZ                     (1000)
#endif

// ----------------------------------------------------------------------------

#if !defined(OS_INTEGER_RTOS_MAIN_STACK_SIZE_BYTES)
#define OS_INTEGER_RTOS_MAIN_STACK_SIZE_BYTES               (os::rtos::port::stack::default_size_bytes)
#endif

#if !defined(OS_INTEGER_RTOS_IDLE_STACK_SIZE_BYTES)
#define OS_INTEGER_RTOS_IDLE_STACK_SIZE_BYTES               (os::rtos::port::stack::default_size_bytes)
#endif

#if !defined(OS_BOOL_RTOS_SCHEDULER_PREEMPTIVE)
#define OS_BOOL_RTOS_SCHEDULER_PREEMPTIVE                   (true)
#endif

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_RTOS_OS_DECLS_H_ */
