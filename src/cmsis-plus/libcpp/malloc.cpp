//
// This file is part of the µOS++ distribution.
// Copyright (c) 2013 Liviu Ionescu.
//
// [Partly inspired from the LLVM libcxx sources].
// Copyright (c) 2009-2013 by the contributors listed in
// 'LLVM libcxx Credits.txt'. See 'LLVM libcxx License.txt' for details.
//
// References are to ISO/IEC 14882:2011(E) Third edition (2011-09-01).
//

/// \file
/// \brief Local malloc() & free() declarations.

#include <cmsis-plus/iso/malloc.h>
#include <cmsis-plus/rtos/os.h>

#include <cstdlib>

using namespace os::rtos;

namespace os
{
  namespace estd
  {
    /// \details
    /// The malloc() function allocates size bytes of memory and returns a
    /// pointer to the allocated memory. Currently it calls
    /// the C function.
    ///
    /// \note Synchronisation is provided by using a scheduler lock.
    void*
    malloc(std::size_t size) noexcept
    {
      void* p;
        {
          // ----- Begin of critical section ----------------------------------
          scheduler::Critical_section cs;

          p = ::malloc(size);
          // ----- End of critical section ------------------------------------
        }
#if defined(_DEBUG)
      os::diag::trace.putString("os::core::malloc(");
      os::diag::trace.putDec(size);
      os::diag::trace.putString(") -> ");
      os::diag::trace.putHex(p);
      os::diag::trace.putNewLine();
#endif
      return p;
    }

    /// \details
    /// The free() function deallocates the memory allocation pointed
    /// to by ptr.
    /// If ptr is a NULL pointer, no operation is performed.
    /// Currently it calls
    /// the C function.
    ///
    /// \note Synchronisation is provided by using a scheduler lock.
    void
    free(void *ptr) noexcept
    {
#if defined(_DEBUG)
      os::diag::trace.putString("os::core::free(");
      os::diag::trace.putHex(ptr);
      os::diag::trace.putString(")");
      os::diag::trace.putNewLine();
#endif
        {
          // ----- Begin of critical section ----------------------------------
          scheduler::Critical_section cs;

          return ::free(ptr);
          // ----- End of critical section ------------------------------------
        }
    }
  } /* namespace estd */
} /* namespace os */
