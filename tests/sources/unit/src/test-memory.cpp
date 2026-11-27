/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2026 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can be
 * obtained from https://opensource.org/licenses/mit.
 */

/*
 * Functional tests of the memory allocators.
 */

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/memory/block-pool.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#if defined(OS_IS_CROSS_BUILD)
// The µOS++ malloc() family, including malloc_usable_size(), is used
// only in cross builds; native builds use the host C library (macOS
// has neither <malloc.h> nor malloc_usable_size()).
#include <malloc.h>
#endif

// ----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#if (__clang_major__ >= 20)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage-in-libc-call"
#endif // (__clang_major__ >= 20)
#elif defined(__GNUC__)
// The casts to unsigned int are needed on 64-bit hosts.
#pragma GCC diagnostic ignored "-Wuseless-cast"
#endif

// ----------------------------------------------------------------------------

namespace
{
  struct mem_test_item
  {
    char data[24];
  };
} // namespace
#include <test-unit.h>

// ----------------------------------------------------------------------------

int
test_unit_memory (void)
{

  printf ("\n%s - started\n", __func__);

  // A growing realloc() must preserve the old contents (#7).
  {
    const char text[] = "realloc() keeps the contents";
    char* p = static_cast<char*> (malloc (sizeof (text)));
    if (p == nullptr)
      {
        printf ("%s - malloc() failed\n", __func__);
        return 1;
      }
    // #pragma clang diagnostic ignored "-Wunsafe-buffer-usage-in-libc-call"
    memcpy (p, text, sizeof (text));

#if defined(OS_IS_CROSS_BUILD)
    size_t usable = malloc_usable_size (p);
    if (usable < sizeof (text))
      {
        printf ("%s - malloc_usable_size()=%u < %u\n", __func__,
                static_cast<unsigned int> (usable),
                static_cast<unsigned int> (sizeof (text)));
        free (p);
        return 1;
      }
    if (malloc_usable_size (nullptr) != 0)
      {
        printf ("%s - malloc_usable_size(nullptr) != 0\n", __func__);
        free (p);
        return 1;
      }
#endif

    char* q = static_cast<char*> (realloc (p, 4 * sizeof (text)));
    if (q == nullptr)
      {
        printf ("%s - realloc() failed\n", __func__);
        free (p);
        return 1;
      }
    // #pragma clang diagnostic ignored "-Wunsafe-buffer-usage-in-libc-call"
    if (memcmp (q, text, sizeof (text)) != 0)
      {
        printf ("%s - realloc() lost the contents\n", __func__);
        free (q);
        return 1;
      }
    free (q);
    printf ("%s - realloc() ok\n", __func__);
  }

  // A block pool knows the size of its blocks.
  {
    using item = mem_test_item;
    os::memory::block_pool_typed_inclusive<item, 2> pool{ "pool" };

    void* b = pool.allocate (sizeof (item));
    if (b == nullptr)
      {
        printf ("%s - block_pool::allocate() failed\n", __func__);
        return 1;
      }
    std::size_t sz = pool.usable_size (b);
    pool.deallocate (b, sizeof (item));
    if (sz != sizeof (item))
      {
        printf ("%s - block_pool::usable_size()=%u != %u\n", __func__,
                static_cast<unsigned int> (sz),
                static_cast<unsigned int> (sizeof (item)));
        return 1;
      }
    printf ("%s - block_pool::usable_size() ok\n", __func__);
  }

  return 0;
}

// ----------------------------------------------------------------------------
