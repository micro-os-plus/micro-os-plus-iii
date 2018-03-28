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

// ============================================================================
// This file is for internal use in µOS++ and should not be included
// in applications.
/*
 * (References are to ISO/IEC DIS 14882:2017)
 *
 * A trivially copyable class is a class:
 * - where each copy constructor, move constructor, copy assignment
 * operator, and move assignment operator (15.8, 16.5.3) is either
 * deleted or trivial,
 * - that has at least one non-deleted copy constructor, move
 * constructor, copy assignment operator, or move assignment operator, and
 * - that has a trivial, non-deleted destructor (15.4).
 */

/**
 * @brief Standard thread.
 */
class thread
{
public:

  using native_handle_type = os::rtos::thread*; // See 33.2.3

  /**
   * @brief Thread unique id.
   *
   * @details
   * An object of type thread::id provides a unique identifier for each
   * thread of execution and a single distinct value for all thread
   * objects that do not represent a thread of execution (33.3.2).
   * Each thread of execution has an associated thread::id object that
   * is not equal to the thread::id object of any other thread of
   * execution and that is not equal to the thread::id object of any
   * thread object that does not represent threads of execution.
   *
   * thread::id shall be a trivially copyable class (Clause 12).
   * The library may reuse the value of a thread::id of a terminated
   * thread that can no longer be joined.
   */
  class id
  {
  public:
    id () noexcept;

    explicit
    id (native_handle_type system_thread) noexcept;

    id (const id&) = default;
    id&
    operator= (const id&) = default;

    ~id () = default;

  private:

    friend class thread;
    friend struct std::hash<thread::id>;

    // Only two of them, the other 4 are defined in terms of these.
    friend bool
    operator== (thread::id x, thread::id y) noexcept;

    friend bool
    operator< (thread::id x, thread::id y) noexcept;

    // The id is actually a pointer to the system thread.
    native_handle_type native_thread_;
  };

  thread () noexcept = default;

  template<typename F, //
      typename ... Args>
    explicit
    thread (F&& f, Args&&... args);

  ~thread ();

  thread (const thread&) = delete;
  thread (thread&& t) noexcept;

  thread&
  operator= (const thread&) = delete;
  thread&
  operator= (thread&& t) noexcept;

  // ----------------------------------------------------------------------

  void
  swap (thread& t) noexcept;

  bool
  joinable (void) const noexcept;

  void
  join (void);

  void
  detach (void);

  id
  get_id (void) const noexcept;

  native_handle_type
  native_handle ();

  static unsigned
  hardware_concurrency (void) noexcept;

private:

  template<typename F_T>
    static void
    run_function_object (const void* func_object);

  template<typename F_T>
    static void
    delete_function_object (const void* func_obj);

  void
  delete_system_thread (void);

  // The current implementation creates temporary id() objects
  // and copies (moves?) them here, but this is not a problem,
  // the id is actually a pointer.
  id id_;

  using function_object_deleter_t = void (*) (void*);
  function_object_deleter_t function_object_deleter_ = nullptr;

public:

};

// Enforce the copyable requirement.
static_assert(std::is_trivially_copyable<thread::id>::value,
    "thread::id must be trivially copyable");

// ========================================================================

void
swap (thread& x, thread& y) noexcept;

/*
 * Relational operators allow thread::id objects to be used as keys
 * in associative containers.
 */
bool
operator== (thread::id x, thread::id y) noexcept;
bool
operator!= (thread::id x, thread::id y) noexcept;
bool
operator< (thread::id x, thread::id y) noexcept;
bool
operator<= (thread::id x, thread::id y) noexcept;
bool
operator> (thread::id x, thread::id y) noexcept;
bool
operator>= (thread::id x, thread::id y) noexcept;

#if 0
template<class charT, class traits>
basic_ostream<charT, traits>&
operator<<(basic_ostream<charT, traits>& out, thread::id id);
#endif

// Hash support
template<class T>
  struct hash;

template<>
  struct hash<thread::id> ;

// ========================================================================
/**
 * @brief A namespace for functions applying to the current thread.
 *
 * @ingroup cmsis-plus-iso
 */
namespace this_thread
{

  /**
   * @brief Return the **id** of the current running thread.
   */
  thread::id
  get_id () noexcept;

  /**
   * @brief Yield the CPU to the next ready thread.
   */
  void
  yield () noexcept;

  /**
   * @brief Sleep for a given duration.
   * @param [in] rel_time
   * @details
   * extra Clock_T is an extension to the standard
   */
  template<typename Clock_T = os::estd::chrono::systick_clock, typename Rep_T,
      typename Period_T>
    constexpr void
    sleep_for (const std::chrono::duration<Rep_T, Period_T>& rel_time);

  /**
   * @brief Sleep until a given time point.
   * @param [in] abs_time
   */
  template<typename Clock_T, typename Duration_T>
    void
    sleep_until (const std::chrono::time_point<Clock_T, Duration_T>& abs_time);

} /* namespace this_thread */

// ========================================================================
// Inline & template implementations.

// ========================================================================

inline void
swap (thread& x, thread& y) noexcept
{
  x.swap (y);
}

inline bool
operator== (thread::id x, thread::id y) noexcept
{
  return x.native_thread_ == y.native_thread_;
}

inline bool
operator!= (thread::id x, thread::id y) noexcept
{
  return !(x == y);
}

inline bool
operator< (thread::id x, thread::id y) noexcept
{
  return x.native_thread_ < y.native_thread_;
}

inline bool
operator<= (thread::id x, thread::id y) noexcept
{
  return !(y < x);
}

inline bool
operator> (thread::id x, thread::id y) noexcept
{
  return y < x;
}

inline bool
operator>= (thread::id x, thread::id y) noexcept
{
  return !(x < y);
}

// ========================================================================

inline
thread::id::id () noexcept :
native_thread_ ( nullptr)
  {
    ;
  }

inline
thread::id::id (native_handle_type native_thread) noexcept :
native_thread_ ( native_thread)
  {
    ;
  }

// ------------------------------------------------------------------------

inline thread::id
thread::get_id () const noexcept
{
  return id_;
}

inline thread::native_handle_type
thread::native_handle ()
{
  return id_.native_thread_;
}

inline unsigned
thread::hardware_concurrency () noexcept
{
  return 1;
}

template<typename F_T>
  void
  thread::run_function_object (const void* func_obj)
  {
    os::trace::printf ("%s()\n", __PRETTY_FUNCTION__);

    using Function_object = F_T;
    const Function_object* f = static_cast<const Function_object*> (func_obj);
    (*f) ();
  }

template<typename F_T>
  void
  thread::delete_function_object (const void* func_obj)
  {
    os::trace::printf ("%s()\n", __PRETTY_FUNCTION__);

    using Function_object = F_T;
    const Function_object* f = static_cast<const Function_object*> (func_obj);

    // The delete now has the knowledge required to
    // correctly delete the object (i.e. the object size).
    delete f;
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

template<typename Callable_T, typename ... Args_T>
  thread::thread (Callable_T&& f, Args_T&&... args)
  {
    // static_assert(std::is_same<Attr_T, os::rtos::thread::attr_t>::value, "first param must be thread_attr_t*");

    os::trace::printf ("%s() @%p\n", __PRETTY_FUNCTION__, this);

    using Function_object = decltype(std::bind (std::forward<Callable_T> (f),
            std::forward<Args_T>(args)...));

    // Dynamic allocation! The size depends on the number of arguments.
    // This creates a small problem, since both running the function
    // and deleting the object requires the type. It is passes as
    // template functions.
    Function_object* funct_obj = new Function_object (
        std::bind (std::forward<Callable_T> (f),
                   std::forward<Args_T>(args)...));

    // The function to start the thread is a custom proxy that
    // knows how to get the variadic arguments.
    id_ = id
      { new os::rtos::thread (
          reinterpret_cast<os::rtos::thread::func_t> (&run_function_object<
              Function_object> ),
          reinterpret_cast<os::rtos::thread::func_args_t> (funct_obj)) };

    // The deleter, to be used during destruction.
    function_object_deleter_ =
        reinterpret_cast<function_object_deleter_t> (&delete_function_object<
            Function_object> );
  }

#pragma GCC diagnostic pop

// ========================================================================

namespace this_thread
{

  inline void
  __attribute__((always_inline))
  yield () noexcept
  {
    os::rtos::this_thread::yield ();
  }

  inline thread::id
  get_id () noexcept
  {
    return thread::id (&os::rtos::this_thread::thread ());
  }

  // This implementation currently supports only short
  // delays, since it uses the ticks timer.

  // Note: there is no absolute guarantee that the
  // sleep will not return earlier, so the application
  // might need to retry.

  // Note the constexpr return type, which tries to compute everything at
  // compile time. And, for constant durations, it succeeds.
#if 0
  template<class Rep_T, class Period_T>
  constexpr void
  sleep_for (const std::chrono::duration<Rep_T, Period_T>& rel_time)
    {
      using namespace std::chrono;

      if (rel_time > duration<Rep_T, Period_T>::zero ())
        {
          // Round up to micros, in case of nanos.
          microseconds micros =
          os::estd::chrono::ceil<microseconds> (rel_time);

          // Round up to ticks.
#if 0

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

          os::rtos::thread::sleep (
              (os::rtos::systicks_t) (os::estd::chrono::ceil<
                  systicks> (micros).count ()));
#pragma GCC diagnostic pop

#else
          // The code seems better with this variant, otherwise it
          // does not optimise constant calls.
          os::rtos::Systick_clock::sleep_for (
              os::rtos::Systick_clock::ticks_cast (
                  micros.count ()));
#endif

        }
    }
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

  template<typename Clock_T, class Rep_T, class Period_T>
    constexpr void
    sleep_for (const std::chrono::duration<Rep_T, Period_T>& rel_time)
    {
      using namespace std::chrono;

      using clock = Clock_T;
      using sleep_rep = typename clock::sleep_rep;

      if (rel_time > duration<Rep_T, Period_T>::zero ())
        {
          sleep_rep d = static_cast<sleep_rep> (os::estd::chrono::ceil<
              typename clock::duration> (rel_time).count ());

          clock::sleep_for (d);
        }
    }

#pragma GCC diagnostic pop

  template<typename Clock_T, typename Duration_T>
    void
    sleep_until (const std::chrono::time_point<Clock_T, Duration_T>& abs_time)
    {
      using clock = Clock_T;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

      auto now = clock::now ();

      while (now < abs_time)
        {
          sleep_for (abs_time - now);
          now = clock::now ();
        }

#pragma GCC diagnostic pop

    }

  template<typename Duration_T>
    void
    sleep_until (
        const std::chrono::time_point<os::estd::chrono::realtime_clock,
            Duration_T>& abs_time)
    {
      using clock = os::estd::chrono::realtime_clock;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

      auto now = clock::now ();
      while (now < abs_time)
        {
          typename clock::sleep_rep d = (os::estd::chrono::ceil<
              typename clock::sleep_duration> (abs_time - now)).count ();
          clock::sleep_for (d);
          now = clock::now ();
        }

#pragma GCC diagnostic pop

    }

  template<typename Duration_T>
    void
    sleep_until (
        const std::chrono::time_point<os::estd::chrono::systick_clock,
            Duration_T>& abs_time)
    {
      using clock = os::estd::chrono::systick_clock;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

      auto now = clock::now ();
      while (now < abs_time)
        {
          typename clock::sleep_rep d = (os::estd::chrono::ceil<
              typename clock::sleep_duration> (abs_time - now)).count ();
          clock::sleep_for (d);
          now = clock::now ();
        }

#pragma GCC diagnostic pop

    }
} /* namespace this_thread */
