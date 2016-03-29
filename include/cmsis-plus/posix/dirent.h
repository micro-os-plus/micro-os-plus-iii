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

#ifndef POSIX_IO_DIRENT_H_
#define POSIX_IO_DIRENT_H_

#if !defined(__ARM_EABI__)
#include <dirent.h>
#else

#include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

// ----------------------------------------------------------------------------

#if !defined(OS_INTEGER_DIRENT_NAME_MAX)
#define OS_INTEGER_DIRENT_NAME_MAX  (256)
#endif

// ----------------------------------------------------------------------------

  struct dirent
  {
    ino_t d_ino;
    char d_name[OS_INTEGER_DIRENT_NAME_MAX];
  };

// The content of this structure is not relevant, it is here just to keep
// POSIX compatibility, in real life the Directory class is used
// and casted to DIR.
  typedef struct
  {
    ;
  } DIR;

// ----------------------------------------------------------------------------

  DIR*
  opendir (const char* dirname);

  struct dirent*
  readdir (DIR* dirp);

#if 0
  int
  readdir_r (DIR* dirp, struct dirent* entry, struct dirent** result);
#endif

  void
  rewinddir (DIR* dirp);

  int
  closedir (DIR* dirp);

// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* __ARM_EABI__ */

#endif /* POSIX_IO_DIRENT_H_ */
