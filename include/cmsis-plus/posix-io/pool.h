/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
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

#ifndef CMSIS_PLUS_POSIX_IO_POOL_H_
#define CMSIS_PLUS_POSIX_IO_POOL_H_

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#include <cstddef>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    /**
     * @brief Pool of objects.
     * @headerfile pool.h <cmsis-plus/posix-io/pool.h>
     * @ingroup cmsis-plus-posix-io-utils
     */
    class pool
    {
      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      pool (std::size_t size);
      pool (const pool&) = delete;

      virtual
      ~pool ();

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

      void*
      aquire (void);

      bool
      release (void* obj);

      // ----------------------------------------------------------------------

      std::size_t
      size (void) const;

      void*
      object (std::size_t index) const;

      bool
      in_use (std::size_t index) const;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    protected:

      /**
       * @cond ignore
       */

      // Referred directly in pool_typed.
      void** array_;
      bool* in_use_;
      std::size_t size_;

      /**
       * @endcond
       */

    };

    // ========================================================================

    /**
     * @brief Pool of typed objects class template.
     * @headerfile pool.h <cmsis-plus/posix-io/pool.h>
     * @ingroup cmsis-plus-posix-io-utils
     */
    template<typename T>
      class pool_typed : public pool
      {

        /**
         * @name Types & Constants
         * @{
         */

      public:

        /**
         * @brief Standard type definition.
         */
        using value_type = T;

        /**
         * @}
         */

        /**
         * @name Constructors & Destructor
         * @{
         */

      public:

        pool_typed (std::size_t size);

        pool_typed (const pool_typed&) = delete;

        virtual
        ~pool_typed ();

        /**
         * @}
         */

        // ----------------------------------------------------------------------
        /**
         * @name Public Member Functions
         * @{
         */

      public:

        value_type*
        aquire (void);

        bool
        release (value_type* obj);

        /**
         * @}
         */

      };

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    inline std::size_t
    pool::size (void) const
    {
      return size_;
    }

    inline void*
    pool::object (std::size_t index) const
    {
      return array_[index];
    }

    inline bool
    pool::in_use (std::size_t index) const
    {
      return in_use_[index];
    }

    // ========================================================================

    template<typename T>
      pool_typed<T>::pool_typed (std::size_t size) :
          pool (size)
      {
        array_ = reinterpret_cast<void**> (new value_type*[size]);
        for (std::size_t i = 0; i < size; ++i)
          {
            array_[i] = new value_type;
          }
      }

    template<typename T>
      pool_typed<T>::~pool_typed ()
      {
        for (std::size_t i = 0; i < size_; ++i)
          {
            delete static_cast<value_type*> (array_[i]);
          }
        delete[] array_;
        size_ = 0;
      }

    // ----------------------------------------------------------------------

    template<typename T>
      inline typename pool_typed<T>::value_type*
      __attribute__((always_inline))
      pool_typed<T>::aquire (void)
      {
        return static_cast<value_type*> (pool::aquire ());
      }

    template<typename T>
      inline bool
      __attribute__((always_inline))
      pool_typed<T>::release (value_type* obj)
      {
        return pool::release (obj);
      }

  // ========================================================================

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_POOL_H_ */
