/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2018-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#ifndef POSIX_SYS_IOCTL_H_
#define POSIX_SYS_IOCTL_H_

// ----------------------------------------------------------------------------

// Avoid warnings for _IOC* definitions.
#pragma GCC system_header

/*
 * From: https://github.com/torvalds/linux/blob/master/include/uapi/asm-generic/ioctl.h
 *
 * The following is for compatibility across the various Linux
 * platforms.  The generic ioctl numbering scheme doesn't really enforce
 * a type field.  De facto, however, the top 8 bits of the lower 16
 * bits are indeed used as a type field, so we might just as well make
 * this explicit here.  Please be sure to use the decoding macros
 * below from now on.
 */
#define _IOC_NRBITS 8
#define _IOC_TYPEBITS 8

#ifndef _IOC_SIZEBITS
# define _IOC_SIZEBITS  14
#endif

#ifndef _IOC_DIRBITS
# define _IOC_DIRBITS 2
#endif

#define _IOC_NRMASK ((1 << _IOC_NRBITS)-1)
#define _IOC_TYPEMASK ((1 << _IOC_TYPEBITS)-1)
#define _IOC_SIZEMASK ((1 << _IOC_SIZEBITS)-1)
#define _IOC_DIRMASK  ((1 << _IOC_DIRBITS)-1)

#define _IOC_NRSHIFT  0
#define _IOC_TYPESHIFT  (_IOC_NRSHIFT+_IOC_NRBITS)
#define _IOC_SIZESHIFT  (_IOC_TYPESHIFT+_IOC_TYPEBITS)
#define _IOC_DIRSHIFT (_IOC_SIZESHIFT+_IOC_SIZEBITS)

/*
 * Direction bits, which any architecture can choose to override
 * before including this file.
 *
 * NOTE: _IOC_WRITE means userland is writing and kernel is
 * reading. _IOC_READ means userland is reading and kernel is writing.
 */

#ifndef _IOC_NONE
# define _IOC_NONE  0U
#endif

#ifndef _IOC_WRITE
# define _IOC_WRITE 1U
#endif

#ifndef _IOC_READ
# define _IOC_READ  2U
#endif

#define _IOC(dir,type,nr,size) \
   (((dir)  << _IOC_DIRSHIFT) | \
   ((type) << _IOC_TYPESHIFT) | \
   ((nr)   << _IOC_NRSHIFT) | \
   ((size) << _IOC_SIZESHIFT))

#ifndef __KERNEL__
#define _IOC_TYPECHECK(t) (sizeof(t))
#endif

// ----------------------------------------------------------------------------

/*
 * From: https://github.com/torvalds/linux/blob/master/include/uapi/linux/fs.h
 *
 * Used to create numbers.
 *
 * NOTE: _IOW means userland is writing and kernel is reading. _IOR
 * means userland is reading and kernel is writing.
 */
#define _IO(type,nr)    _IOC(_IOC_NONE,(type),(nr),0)
#define _IOR(type,nr,size)  _IOC(_IOC_READ,(type),(nr),(_IOC_TYPECHECK(size)))
#define _IOW(type,nr,size)  _IOC(_IOC_WRITE,(type),(nr),(_IOC_TYPECHECK(size)))
#define _IOWR(type,nr,size) _IOC(_IOC_READ|_IOC_WRITE,(type),(nr),(_IOC_TYPECHECK(size)))

/* 108-111 have been used for various private purposes. */

#define BLKSSZGET  _IO(0x12,104) /* get block logical device sector size */
#define BLKGETSIZE64 _IOR(0x12,114,size_t)  /* get device size in bytes (u64 *arg) */
#define BLKPBSZGET _IO(0x12,123) /* get block physical device sector size */

// ----------------------------------------------------------------------------

#endif /* POSIX_SYS_IOCTL_H_ */

