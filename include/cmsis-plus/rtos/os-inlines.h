/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#ifndef CMSIS_PLUS_RTOS_OS_INLINES_H_
#define CMSIS_PLUS_RTOS_OS_INLINES_H_

/*
 * References are to ISO/IEC 14882:2011(E) Third edition (2011-09-01)
 */

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    namespace internal
    {
      /**
       * @details
       * The allocation function (3.7.4.1) called by a new-expression (5.3.4)
       * to allocate a storage of size _bytes_ suitably aligned to represent
       * any object of that size.
       * Return a non-null pointer to suitably aligned storage (3.7.4).
       *
       * The storage is allocated using the RTOS system allocator.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline void*
      object_named_system::operator new (std::size_t bytes)
      {
        assert (!interrupts::in_handler_mode ());

        return rtos::memory::allocator<char> ().allocate (bytes);
      }

      /**
       * @details
       * The allocation function (3.7.4.1) called by the array form of a
       * new-expression (5.3.4) to allocate a storage of size _bytes_  suitably
       * aligned to represent any array object of that size or smaller.
       *
       * The storage is allocated using the RTOS system allocator.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline void*
      object_named_system::operator new[] (std::size_t bytes)
      {
        // Forward array allocation to single element allocation.
        return operator new (bytes);
      }

      /**
       * @details
       * The allocation function (3.7.4.1) called by a
       * placement new-expression
       * to allocate a storage of size _bytes_ suitably aligned to represent
       * any object of that size.
       * Return a non-null pointer to suitably aligned storage (3.7.4).
       *
       * The storage is allocated using the RTOS system allocator.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline void*
      object_named_system::operator new (std::size_t, void* ptr)
      {
        return ptr;
      }

      /**
       * @details
       * The allocation function (3.7.4.1) called by the array form of a
       * placement new-expression to allocate a storage of size _bytes_
       * suitably
       * aligned to represent any array object of that size or smaller.
       *
       * The storage is allocated using the RTOS system allocator.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline void*
      object_named_system::operator new[] (std::size_t bytes, void* ptr)
      {
        // Forward array allocation to single element allocation.
        return operator new (bytes, ptr);
      }

      /**
       * @details
       * The deallocation function (3.7.4.2) called by a delete-expression
       * to render the value of _ptr_ invalid.
       *
       * _ptr_ shall be a null pointer or its value shall be a value returned by
       * an earlier call to the (possibly replaced) operator new()
       * which has not
       * been invalidated by an intervening call to operator delete(void*).
       *
       * If _ptr_ is null, does nothing. Otherwise, reclaims the storage
       * allocated by the earlier call to operator new.
       *
       * The storage is deallocated using the RTOS system allocator.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline void
      object_named_system::operator delete (void* ptr, std::size_t bytes)
      {
        assert (!interrupts::in_handler_mode ());

         rtos::memory::allocator<char> ().deallocate (
            static_cast<char*> (ptr), bytes);
      }

      /**
       * @details
       * The deallocation function (3.7.4.2) called by the array form of
       * a delete-expression to render the value of _ptr_ invalid.
       *
       * If _ptr_ is null, does nothing. Otherwise, reclaims the storage
       * allocated by the earlier call to operator new.
       *
       * The storage is deallocated using the RTOS system allocator.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline void
      object_named_system::operator delete[] (void* ptr, std::size_t bytes)
      {
        // Forward array deallocation to single element deallocation.
        operator delete (ptr, bytes);
      }

    } /* namespace internal */
  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_RTOS_OS_INLINES_H_ */
