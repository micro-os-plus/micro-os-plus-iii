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

#include <cmsis-plus/std/thread>
//#include <diag/trace.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace cmsis
  {
    namespace std
    {
      // ----------------------------------------------------------------------

      thread::thread (thread&& t) noexcept
      {
        swap (t);
      }

      thread&
      thread::operator= (thread&& t) noexcept
      {
        if (joinable ())
          {
#if defined(TRACE)
            os::trace::printf ("%s() @%p attempt to assign a running thread\n", __PRETTY_FUNCTION__, this);
#endif
            ::std::abort (); // in ISO it is std::terminate()
          }
        swap (t);
        return *this;
      }

      thread::~thread ()
      {
#if defined(TRACE)
        os::trace::printf ("%s() @%p\n", __func__, this);
#endif
        if (joinable ())
          {
#if defined(TRACE)
            os::trace::printf ("%s() @%p attempt to destruct a running thread\n", __PRETTY_FUNCTION__, this);
#endif
            ::std::abort (); // in ISO it is std::terminate()
          }

        // Manually delete the system thread. An unique_ptr in id and
        // the move semantic would probably be a better idea.
        delete id_.system_thread_;
      }

      // ----------------------------------------------------------------------

      void
      thread::swap (thread& t) noexcept
      {
        ::std::swap (id_, t.id_);
      }

      bool
      thread::joinable () const noexcept
      {
        return !(id_ == thread::id ());
      }

      void
      thread::join ()
      {
#if defined(TRACE)
        os::trace::printf ("%s() @%p\n", __func__, this);
#endif
        if (id_ != id ())
          {
            id_.system_thread_->join ();

            // The thread was terminated, free resources.
            delete id_.system_thread_;
          }

        id_ = id ();
#if defined(TRACE)
        os::trace::printf ("%s() @%p joined\n", __func__, this);
#endif
      }

      void
      thread::detach ()
      {
#if defined(TRACE)
        os::trace::printf ("%s() @%p\n", __func__, this);
#endif
        if (id_ != id ())
          {
            id_.system_thread_->detach ();
          }

        // The detached thread will continue to run, but we'll not have
        // access to it from here, not even to delete it.
        // TODO: arrange to delete it at exit()?

        id_ = id ();
#if defined(TRACE)
        os::trace::printf ("%s() @%p detached\n", __func__, this);
#endif
      }

    } /* namespace std */
  } /* namespace cmsis */
} /* namespace os */
