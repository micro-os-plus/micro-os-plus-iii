/**
 * @file
 * @brief Application configuration file.
 * @details
 * The `<cmsis-plus/os-ap-config.h>` header file is used to configure
 * all build
 * options available for CMSIS++ applications.
 *
 * Each application should provide such a configuration file
 * in the compiler include path, even if there are no
 * specific definitions.
 */

/**
 * @ingroup cmsis-plus-app-config-cmdline
 * @{
 */

/**
 * @brief Enable debug support.
 * @details
 * This definition must always be used in **debug** configurations.
 */
#define DEBUG

/**
 * @brief Enable trace support.
 * @details
 * This definition can be used to enable trace support. Without
 * this definition, all trace calls are inlined to empty statements.
 *
 * It is recommended to use `TRACE` in **debug** configurations.
 */
#define TRACE

/**
 * @brief Disable assert support.
 * @details
 * This is the standard ISO/ANSI definition used to
 * disable all `assert()` statements. Without this definition
 * all `assert()` statements are active, regardless of the presence
 * or absence of the `DEBUG` definition.
 *
 * Assertions are a very valuable mechanism to detect out-of-range
 * conditions, usually in debug configurations, but might add a
 * significant overhead in code size, and a certain overhead in
 * execution time.
 *
 * It is recommended to always use `NDEBUG` in **release**
 * configurations.
 */
#define NDEBUG

/**
 * @}
 */

// ----------------------------------------------------------------------------

/**
 * @ingroup cmsis-plus-app-config-rtos
 * @{
 */

/**
 * @brief Define the scheduler frequency, in Hz.
 * @par Default
 * 1000.
 */
#define OS_INTEGER_SYSTICK_FREQUENCY_HZ (1000)

/**
 * @brief For Cortex-M[347], define the interrupt priority level.
 * @details
 * Simple devices implement critical sections by disabling/enabling
 * all interrupts. Cortex-M[347] devices can selectively disable
 * interrupts up to a given priority (by using the BASEPRI register).
 *
 * When used, this option configures the critical sections to disable all
 * interrupts with priorities up to the given value
 * and keep enabled interrupts with higher priorities.
 *
 * @note
 * Considering the confusing ARM priority scheme, this means priorities
 * with a numeric value higher or equal the given value will be
 * disabled and priorities with a numeric value lower thant he given
 * value will remain enabled.
 *
 * @warning
 * The number of different priority levels is vendor dependent. For example
 * ST devices use 4 bits (0-15, with 15=lowest), but others may use
 * 3 bits (0-7, with 7=lowest).
 *
 * If the application does not use high priority interrupts, it is
 * recommend to do not use this option, and allow the system to
 * implement the critical sections by completely disabling/enabling
 * interrupts.
 *
 * If used, the recommended value is 3-4.
 *
 * @par Default
 *   Use of interrupts priorities is disabled.
 */
#define OS_INTEGER_RTOS_CRITICAL_SECTION_INTERRUPT_PRIORITY

/**
 * @brief Define the default thread stack size.
 */
#define OS_INTEGER_RTOS_DEFAULT_STACK_SIZE_BYTES

/**
 * @brief Define the minimum thread stack size.
 */
#define OS_INTEGER_RTOS_DEFAULT_STACK_SIZE_BYTES

/**
 * @brief Define the **main** thread stack size.
 */
#define OS_INTEGER_RTOS_MAIN_STACK_SIZE_BYTES

/**
 * @brief Define the **idle** thread stack size.
 */
#define OS_INTEGER_RTOS_IDLE_STACK_SIZE_BYTES

/**
 * @brief Include statistics to count thread CPU cycles.
 * @details
 * Add support to measure and accumulate accurate thread duration statistics.
 *
 * At each context switch, the high resolution clock is sampled and the
 * difference is added to the current thread statistics variables. At the
 * same time the value is added to a global variable.
 *
 * The RAM overhead of enabling this option is a uint64_t variable for
 * each thread and one global variable.
 *
 * The time overhead is a clock sampling (reading timer registers and
 * possibly adjusting for timer recycle) plus some subtractions/additions
 * on 64-bits.
 *
 * @see os::rtos::scheduler::statistics::cpu_cycles()
 * @see os::rtos::thread::statistics::cpu_cycles()
 *
 * @par Default
 * Disable. Do not include CPU cycles statistics.
 */
#define OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES	(1)

/**
 * @brief Include statistics to count thread context switches.
 * @details
 * Add support to count the number of times each thread was scheduled to run.
 *
 * At each context switch, the thread counter is incremented. At the
 * same time a global counter is also incremented.
 *
 * The RAM overhead of enabling this option is a uint64_t variable for
 * each thread and one global variable.
 *
 * The time overhead is low, incrementing two 64-bit variables.
 *
 * @see os::rtos::scheduler::statistics::context_switches()
 * @see os::rtos::thread::statistics::context_switches()
 *
 * @par Default
 * Disable. Do not include context switches statistics.
 */
#define OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES

/**
 * @brief Add a user defined storage to each thread.
 */
#define OS_INCLUDE_RTOS_CUSTOM_THREAD_USER_STORAGE

/**
 * @brief Extend the message size to 16 bits.
 * @details
 * For embedded applications the message queues are
 * optimised for small messages, up to 256 bytes.
 *
 * If larger messages are needed, this option extends the
 * message size to 65536 bytes.
 *
 * @par Default
 *  False (short messages).
 */
#define OS_BOOL_RTOS_MESSAGE_QUEUE_SIZE_16BITS  (false)

/**
 * @brief Push down the idle thread priority.
 * @details
 * Normally the applications should not create threads with
 * the idle priority.
 *
 * However, some applications, like the ARM CMSIS RTOS validator,
 * need to test the behaviour of idle threads. For such cases,
 * to guarantee that the idle thread is the lowest priority
 * thread, its priority can be lowered one additional step.
 *
 * @par Default
 *  False (the idle thread has the idle priority).
 */
#define OS_BOOL_RTOS_THREAD_IDLE_PRIORITY_BELOW_IDLE (false)

/**
 * @brief Force the stack trace to start with a 0x0.
 * @details
 * This option has no functional consequences, it is only
 * cosmetic, affecting how a debugger displays the stack trace.
 *
 * If your debugger has difficulties to properly display the
 * thread stack trace, enable this option and the stack will
 * always start with a 0x0.
 *
 * @par Default
 *  False (the stack trace starts with the first function).
 */
#define OS_BOOL_RTOS_PORT_CONTEXT_CREATE_ZERO_LR (false)

/**
 * @brief Do not enter sleep in the idle thread.
 * @details
 * Very fast debuggers need direct access to a RAM buffer, which
 * in turn need the clock that powers the bus where the RAM is
 * connected to be permanently on.
 *
 * Unfortunately, most devices disable this clock when entering
 * even the shallow sleep mode, disabling the debugger access to
 * the device.
 *
 * To prevent this, usually for the debug configurations,
 * it is possible to exclude the code that puts the device to sleep.
 */
#define OS_EXCLUDE_RTOS_IDLE_SLEEP

/**
 * @}
 */

// ----------------------------------------------------------------------------

/**
 * @ingroup cmsis-plus-app-config-startup
 * @{
 */

/**
 * @brief Initialise multiple RAM sections.
 */
#define OS_INCLUDE_STARTUP_INIT_MULTIPLE_RAM_SECTIONS

/**
 * @brief Enable guard checks for .bss and .data sections.
 * @details
 * Sometimes mistakes in the liner script prevent the .bss area
 * to be initialised to zero, and/or the .data area to be fully
 * initialised with content from flash.
 *
 * To validate that the start-up properly initialised these areas,
 * some guard words are defined at the begin/end of the .bss and
 * .data areas, and are explicitly checked.
 *
 * @note This option is available only if DEBUG is also defined.
 */
#define OS_BOOL_STARTUP_GUARD_CHECKS (true)

/**
 * @brief Always initialise the hardware FPU.
 */
#define OS_INCLUDE_STARTUP_INIT_FP

/**
 * @brief Make the application a fully semihosted application.
 * @details
 * When writing test applications it is necessary to use some
 * of the resources available from the host system (STDOUT/STDERR,
 * file system, time, exit code, etc).
 *
 * This requires the entire behaviour of the system calls to be
 * changed and most calls to be forwarded to the host. The
 * application is transformed into a 'fully semihosted' application.
 *
 * One major difference from a regular embedded application is that
 * a fully semihosted test application does not run in an endless
 * loop, but terminates, and the test result must be passed back
 * to the host system, via the exit code and optionally via a
 * detailed status file.
 */
#define OS_USE_SEMIHOSTING_SYSCALLS

/**
 * @brief Define the maximum number of semihosting open files.
 * @details
 * The implementation of the semihosting file support requires
 * an array of open files, to keep track of the host files.
 *
 * To simplify the implementation and avoid dynamic allocations, a static
 * array is used. This option defines the size of this array.
 *
 * @par Default
 *  20.
 */
#define OS_INTEGER_SEMIHOSTING_MAX_OPEN_FILES (20)

/**
 * @brief Include definitions for the standard POSIX system calls.
 * @todo update after POSIX I/O is updated.
 */
#define OS_INCLUDE_STANDARD_POSIX_FUNCTIONS

/**
 * @brief Include definitions for the newlib system calls.
 * @todo update after POSIX I/O is updated.
 */
#define OS_INCLUDE_NEWLIB_POSIX_FUNCTIONS

/**
 * @}
 */

// ----------------------------------------------------------------------------

/**
 * @ingroup cmsis-plus-app-config-lib
 * @{
 */

/**
 * @brief Define the size of the atexit() array.
 * @details
 * To simplify the implementation and avoid dynamic allocations, a static
 * array is used. This option defines the size of this array.
 */
#define OS_INTEGER_ATEXIT_ARRAY_SIZE (3)

/**
 * @brief Define the maximum size of a directory name.
 */
#define OS_INTEGER_DIRENT_NAME_MAX  (256)

/**
 * @}
 */


// ----------------------------------------------------------------------------

/**
 * @ingroup cmsis-plus-app-config-port
 * @{
 */

/**
 * @brief Use a custom scheduler implementation.
 * @details
 * When using a CMSIS++ port that runs on top of another RTOS,
 * this option disables the CMSIS++ reference
 * scheduler and forwards all related calls to the custom
 * implementation RTOS.
 *
 * The default is to use the CMSIS++ reference scheduler.
 */
#define OS_USE_RTOS_PORT_SCHEDULER

/**
 * @brief Use a custom condition variable implementation.
 */
#define OS_USE_RTOS_PORT_CONDITION_VARIABLE
/**
 * @brief Use a custom event flags implementation.
 */
#define OS_USE_RTOS_PORT_EVENT_FLAGS
/**
 * @brief Use a custom message queue implementation.
 */
#define OS_USE_RTOS_PORT_MESSAGE_QUEUE
/**
 * @brief Use a custom mutex implementation.
 */
#define OS_USE_RTOS_PORT_MUTEX
/**
 * @brief Use a custom semaphore implementation.
 */
#define OS_USE_RTOS_PORT_SEMAPHORE
/**
 * @brief Use a custom system tick sleep_for() implementation.
 */
#define OS_USE_RTOS_PORT_SYSTICK_CLOCK_SLEEP_FOR
/**
 * @brief Use a custom real time sleep_for() implementation.
 */
#define OS_USE_RTOS_PORT_REALTIME_CLOCK_SLEEP_FOR
/**
 * @brief Use a custom timer implementation.
 */
#define OS_USE_RTOS_PORT_TIMER

/**
 * @}
 */


// ----------------------------------------------------------------------------

/**
 * @ingroup cmsis-plus-app-config-trace
 * @{
 */

// TODO: add examples of output for each OS_TRACE_* option.

/**
 * @brief Forward trace messages via the ITM/SWO.
 * @details
 * ITM (Instrumentation Trace Macrocell) is one of the available ARM
 * technologies intended to facilitate debugging, by providing a trace
 * channel for printf()-like messages and various events.
 *
 * CMSIS++ is capable of forwarding the trace::printf() messages
 * via the ITM, and, if available, this is the standard recommended
 * trace channel.
 *
 * This option requires support from the debugger, to forward the
 * SWO pin to a separate console.
 *
 * @see OS_INTEGER_TRACE_ITM_STIMULUS_PORT
 */
#define OS_USE_TRACE_ITM

/**
 * @brief Forward trace messages via the semihosting debug channel.
 * @details
 * The semihosting debug channel is a dedicated output channel,
 * distinct from STDOUT and STDERR, intended for printf()-like messages.
 *
 * Traditionally the semihosting debug channel is quite slow,
 * especially when used to output single bytes. To slightly improve
 * performance, a small buffer is used internally (@ref
 * OS_INTEGER_TRACE_SEMIHOSTING_BUFF_ARRAY_SIZE).
 *
 * This option requires support from the debugger, to forward the
 * semihosting channel to a separate console. Most of the
 * debuggers require to explicitly enable semihosting
 * for this option to be functional.
 *
 * @note The use of this option affects only the trace channel
 * and does not transform the application into
 * a fully semihosted application, in other words it does not
 * change the behaviour of the other system calls, as does the
 * use of @ref OS_USE_SEMIHOSTING_SYSCALLS.
 *
 * @see OS_INTEGER_TRACE_SEMIHOSTING_BUFF_ARRAY_SIZE
 */
#define OS_USE_TRACE_SEMIHOSTING_DEBUG

/**
 * @brief Forward trace messages via the semihosting output stream.
 * @details
 * The semihosting output channel is the same as STDOUT, and
 * usually it is buffered, so characters may not be displayed
 * immediately, but be delayed until the line ends.
 *
 * This option requires support from the debugger, to forward the
 * semihosting channel to a separate console. Most of the
 * debuggers require to explicitly enable semihosting
 * for this option to be functional.
 *
 * @note The use of this option affects only the trace channel
 * and does not transform the application into
 * a fully semihosted application, in other words it does not
 * change the behaviour of the other system calls, as does the
 * use of @ref OS_USE_SEMIHOSTING_SYSCALLS.
 */
#define OS_USE_TRACE_SEMIHOSTING_STDOUT

/**
 * @brief Forward trace messages via the SEGGER RTT.
 * @details
 * SEGGER RTT (Real Time Terminal) is a very fast communication channel
 * available for J-Link probes.
 *
 * CMSIS++ is capable of forwarding the trace::printf() messages
 * via the RTT, and, if available, this is the fastest
 * trace channel.
 */
#define OS_USE_TRACE_SEGGER_RTT

/**
 * @brief Enable trace messages for RTOS clocks functions.
 */
#define OS_TRACE_RTOS_CLOCKS
/**
 * @brief Enable trace messages for RTOS condition variables functions.
 */
#define OS_TRACE_RTOS_CONDVAR
/**
 * @brief Enable trace messages for RTOS event flags functions.
 */
#define OS_TRACE_RTOS_EVFLAGS
/**
 * @brief Enable trace messages for RTOS memory pools functions.
 */
#define OS_TRACE_RTOS_MEMPOOL
/**
 * @brief Enable trace messages for RTOS message queues functions.
 */
#define OS_TRACE_RTOS_MQUEUE
/**
 * @brief Enable trace messages for RTOS mutex functions.
 */
#define OS_TRACE_RTOS_MUTEX
/**
 * @brief Display an exclamation mark for each RTC tick.
 */
#define OS_TRACE_RTOS_RTC_TICK
/**
 * @brief Enable trace messages for RTOS scheduler functions.
 */
#define OS_TRACE_RTOS_SCHEDULER
/**
 * @brief Enable trace messages for RTOS semaphore functions.
 */
#define OS_TRACE_RTOS_SEMAPHORE
/**
 * @brief Display a dot and a comma for each system clock tick.
 */
#define OS_TRACE_RTOS_SYSCLOCK_TICK
/**
 * @brief Enable trace messages for RTOS thread functions.
 */
#define OS_TRACE_RTOS_THREAD
/**
 * @brief Enable trace messages for RTOS thread context functions.
 * @warning
 * This option requires a fast trace channel, like SEGGER RTT or at least ITM.
 */
#define OS_TRACE_RTOS_THREAD_CONTEXT
/**
 * @brief Enable trace messages for RTOS thread signal functions.
 */
#define OS_TRACE_RTOS_THREAD_SIG
/**
 * @brief Enable trace messages for RTOS timer functions.
 */
#define OS_TRACE_RTOS_TIMER

/**
 * @brief Enable trace messages for RTOS list functions.
 * @warning
 * This option requires a fast trace channel, like SEGGER RTT or at least ITM.
 */
#define OS_TRACE_RTOS_LISTS

/**
 * @brief Enable trace messages for memory allocators.
 */
#define OS_TRACE_LIBC_MALLOC

/**
 * @brief Enable trace messages for the atexit() function.
 */
#define OS_TRACE_LIBC_ATEXIT

/**
 * @brief Define the ITM stimulus port used for the trace messages.
 * @details
 * ITM provides 32 distinct stimulus ports for separate trace channels (0-31).
 *
 * @par Default
 *  0 (zero).
 */
#define OS_INTEGER_TRACE_ITM_STIMULUS_PORT  (0)

/**
 * @brief Define the semihosting debug buffer size.
 * @details The size of the internal buffer used to improve
 * performance for the semihosting debug channel.
 *
 * @par Default
 *  16.
 */
#define OS_INTEGER_TRACE_SEMIHOSTING_BUFF_ARRAY_SIZE (16)

/**
 * @}
 */

// ----------------------------------------------------------------------------

