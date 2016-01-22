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
            // TODO: maybe abort()??
            ::std::terminate ();
          }
        swap (t);
        return *this;
      }

      thread::~thread ()
      {
        if (joinable ())
          {
            // TODO: maybe abort()??
            ::std::terminate ();
          }
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
        id_.thread_->join ();
      }

      void
      thread::detach ()
      {
        id_.thread_->detach ();
      }

    } /* namespace std */
  } /* namespace cmsis */
} /* namespace os */
