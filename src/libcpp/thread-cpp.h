/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2016-2025 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can be
 * obtained from https://opensource.org/licenses/mit.
 */

// ============================================================================
// This file is for internal use in µOS++ and should not be included
// in applications.

thread::thread (thread&& t) noexcept
{
  swap (t);
}

thread&
thread::operator= (thread&& t) noexcept
{
  if (joinable ())
    {
      os::trace::printf ("%s() @%p attempt to assign a running thread\n",
                         __func__, this);
      std::abort (); // in ISO it is std::terminate()
    }
  swap (t);
  return *this;
}

void
thread::delete_system_thread (void)
{
  if (id_ != id ())
    {
      if (function_object_ != nullptr && function_object_deleter_ != nullptr)
        {
          // Manually delete the function object used to store arguments.
          // `function_object_` is our own copy: the kernel clears its
          // `func_args_` on exit, so reading it here would leak whenever the
          // thread has already finished.
          function_object_deleter_ (function_object_);
          function_object_ = nullptr;
        }

      // Manually delete the system thread.
      delete id_.native_thread_;
    }
}

thread::~thread ()
{
  os::trace::printf ("%s() @%p\n", __func__, this);
  if (joinable ())
    {
      os::trace::printf ("%s() @%p attempt to destruct a running thread\n",
                         __func__, this);
      std::abort (); // in ISO it is std::terminate()
    }

  delete_system_thread ();
}

// ------------------------------------------------------------------------

void
thread::swap (thread& t) noexcept
{
  std::swap (id_, t.id_);
  std::swap (function_object_deleter_, t.function_object_deleter_);
  std::swap (function_object_, t.function_object_);
}

bool
thread::joinable () const noexcept
{
  return !(id_ == thread::id ());
}

void
thread::join ()
{
  os::trace::printf ("%s() @%p\n", __func__, this);

  if (id_ != id ())
    {
      // Wait for the thread to end, as ISO join() does, before freeing what
      // it runs on. Deleting it straight away -- which is what this did --
      // frees its bound arguments and kills the system thread; on one core
      // the thread had usually finished by then, on SMP it is still running
      // on another core.
      id_.native_thread_->join ();

      if (function_object_ != nullptr && function_object_deleter_ != nullptr)
        {
          // Manually delete the function object used to store arguments.
          // Use our own pointer, because the kernel clears `func_args_` when
          // the thread exits -- which happens before `join()` returns for any
          // short-lived thread.
          function_object_deleter_ (function_object_);
          function_object_ = nullptr;
        }

      // Manually delete the system thread, destroyed by now.
      delete id_.native_thread_;
    }

  id_ = id ();
  os::trace::printf ("%s() @%p joined\n", __func__, this);
}

void
thread::detach ()
{
  os::trace::printf ("%s() @%p\n", __func__, this);
  if (id_ != id ())
    {
      id_.native_thread_->detach ();
    }

  // The detached thread will continue to run, but we'll not have
  // access to it from here, not even to delete it.
  // TODO: arrange to delete it at exit()?

  id_ = id ();
  os::trace::printf ("%s() @%p detached\n", __func__, this);
}

// ==========================================================================
