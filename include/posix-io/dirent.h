/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
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

#ifndef INCLUDE_POSIX_IO_DIRENT_H_
#define INCLUDE_POSIX_IO_DIRENT_H_

#if !defined(OS_INTEGER_DIRENT_NAME_MAX)
#define OS_INTEGER_DIRENT_NAME_MAX  (256)
#endif

typedef struct dirent
{
  char d_name[OS_INTEGER_DIRENT_NAME_MAX];
} DIR;

DIR*
opendir (const char* dirname);

struct dirent*
readdir (DIR* dirp);

int
readdir_r (DIR* dirp, struct dirent* entry, struct dirent** result);

void
rewinddir (DIR* dirp);

int
closedir (DIR *dirp);


#endif /* INCLUDE_POSIX_IO_DIRENT_H_ */
