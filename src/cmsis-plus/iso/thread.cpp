/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 *
 * µOS++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 3.
 *
 * µOS++ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <cmsis-plus/iso/thread>
#include <cstdlib>

// ----------------------------------------------------------------------------

namespace os
{
  namespace estd
  {
    // ----------------------------------------------------------------------

    using namespace os;

    thread::thread (thread&& t) noexcept
    {
      swap (t);
    }

    thread&
    thread::operator= (thread&& t) noexcept
    {
      if (joinable ())
        {
          trace::printf ("%s() @%p attempt to assign a running thread\n",
                         __PRETTY_FUNCTION__, this);
          ::std::abort (); // in ISO it is std::terminate()
        }
      swap (t);
      return *this;
    }

    void
    thread::delete_system_thread (void)
    {
      if (id_ != id ())
        {
          void* args = id_.native_thread_->function_args ();
          if (args != nullptr && function_object_deleter_ != nullptr)
            {
              // Manually delete the function object used to store arguments.
              function_object_deleter_ (args);
            }

          // Manually delete the system thread.
          delete id_.native_thread_;
        }
    }

    thread::~thread ()
    {
      trace::printf ("%s() @%p\n", __func__, this);
      if (joinable ())
        {
          trace::printf ("%s() @%p attempt to destruct a running thread\n",
                         __PRETTY_FUNCTION__, this);
          ::std::abort (); // in ISO it is std::terminate()
        }

      delete_system_thread ();
    }

    // ----------------------------------------------------------------------

    void
    thread::swap (thread& t) noexcept
    {
      ::std::swap (id_, t.id_);
      ::std::swap (function_object_deleter_, t.function_object_deleter_);
    }

    bool
    thread::joinable () const noexcept
    {
      return !(id_ == thread::id ());
    }

    void
    thread::join ()
    {
      trace::printf ("%s() @%p\n", __func__, this);

      delete_system_thread ();

      id_ = id ();
      trace::printf ("%s() @%p joined\n", __func__, this);
    }

    void
    thread::detach ()
    {
      trace::printf ("%s() @%p\n", __func__, this);
      if (id_ != id ())
        {
          id_.native_thread_->detach ();
        }

      // The detached thread will continue to run, but we'll not have
      // access to it from here, not even to delete it.
      // TODO: arrange to delete it at exit()?

      id_ = id ();
      trace::printf ("%s() @%p detached\n", __func__, this);
    }

    namespace this_thread
    {

    } /* namespace this_thread */

  // ------------------------------------------------------------------------

  } /* namespace estd */
} /* namespace os */

// ----------------------------------------------------------------------------
