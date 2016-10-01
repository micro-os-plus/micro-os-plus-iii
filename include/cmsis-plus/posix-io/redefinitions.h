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

#ifndef CMSIS_PLUS_POSIX_IO_REDEFINITIONS_H_
#define CMSIS_PLUS_POSIX_IO_REDEFINITIONS_H_

// These definitions might be useful in some tests, to check
// if both prefixed and not prefixed names are ok.

#define __posix_accept accept
#define __posix_bind bind
#define __posix_chdir chdir
#define __posix_chmod chmod
#define __posix_chown chown
#define __posix_clock clock
#define __posix_close close
#define __posix_closedir closedir
#define __posix_connect connect
#define __posix_execve execve
#define __posix_fcntl fcntl
#define __posix_fork fork
#define __posix_fstat fstat
#define __posix_ftruncate ftruncate
#define __posix_fsync fsync
#define __posix_getcwd getcwd
#define __posix_getpeername getpeername
#define __posix_getpid getpid
#define __posix_getsockname getsockname
#define __posix_getsockopt getsockopt
#define __posix_gettimeofday gettimeofday
#define __posix_ioctl ioctl
#define __posix_isatty isatty
#define __posix_kill kill
#define __posix_link link
#define __posix_listen listen
#define __posix_lseek lseek
#define __posix_mkdir mkdir
#define __posix_open open
#define __posix_opendir opendir
#define __posix_raise raise
#define __posix_read read
#define __posix_readdir readdir
#define __posix_readdir_r readdir_r
#define __posix_readlink readlink
#define __posix_recv recv
#define __posix_recvfrom recvfrom
#define __posix_recvmsg recvmsg
#define __posix_rename rename
#define __posix_rewinddir rewinddir
#define __posix_rmdir rmdir
#define __posix_select select
#define __posix_send send
#define __posix_sendmsg sendmsg
#define __posix_sendto sendto
#define __posix_setsockopt setsockopt
#define __posix_shutdown shutdown
#define __posix_sockatmark sockatmark
#define __posix_socket socket
#define __posix_socketpair socketpair
#define __posix_stat stat
#define __posix_symlink symlink
#define __posix_sync sync
#define __posix_system system
#define __posix_times times
#define __posix_truncate truncate
#define __posix_unlink unlink
#define __posix_utime utime
#define __posix_wait wait
#define __posix_write write
#define __posix_writev writev

#endif /* CMSIS_PLUS_POSIX_IO_REDEFINITIONS_H_ */
