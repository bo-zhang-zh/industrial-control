/*
 * YAFFS: Yet another Flash File System . A NAND-flash specific file system.
 *
 * Copyright (C) 2002-2011 Aleph One Ltd.
 *   for Toby Churchill Ltd and Brightstar Engineering
 *
 * Created by Charles Manning <charles@aleph1.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1 as
 * published by the Free Software Foundation.
 *
 * Note: Only YAFFS headers are LGPL, YAFFS C code is covered by GPL.
 */


#ifndef __YPORTENV_H__
#define __YPORTENV_H__

#include <dfs_posix.h>

#define CONFIG_YAFFS_DIRECT
#define CONFIG_YAFFS_PROVIDE_DEFS
#define CONFIG_YAFFSFS_PROVIDE_VALUES

#ifdef NO_inline
	#define inline
#else
	#ifdef __CC_ARM  
	    #define inline   __inline
	#elif defined (__GNUC__)        		/* GNU GCC Compiler */
	    #define inline   __inline
	#else
		#error "Please set a macro inline with a valid args" 	
	#endif
#endif

#if defined(__CC_ARM)
	#ifdef RT_USING_NEWLIB
		#error "error: when use armcc, please Don't USE NEWLIB!!!"
	#endif
	
	#ifndef off_t
	typedef long off_t;
	#endif
	
	#ifndef loff_t
	typedef unsigned long loff_t;
	#endif
	
	#ifndef dev_t
	typedef long dev_t;
	#endif
	
	#ifndef mode_t
	typedef int mode_t;
	#endif

	/* just like strlen(3), but cap the number of bytes we count */
	rt_inline size_t strnlen(const char *s, size_t max) {
		register const char *p;
		for(p = s; *p && max--; ++p);
		return(p - s);
	}

#elif defined (__GNUC__) && !defined(__CC_ARM)

	#ifndef loff_t
	typedef unsigned long loff_t;
	#endif

	#ifndef dev_t
	typedef long dev_t;
	#endif
#endif

/* Definition of types */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned u32;
 
#ifndef S_ISDIR
	#ifndef _DEV_T_DEFINED
	typedef unsigned int _dev_t;            /* device code */
	#if     !__STDC__
	/* Non-ANSI name for compatibility */
	typedef unsigned int dev_t;
	#endif
	 
	#define _DEV_T_DEFINED
	#endif

	#define __S_IFMT 0170000 /* These bits determine file type.  */
	/* File types.  */
	#define __S_IFDIR 0040000 /* Directory.  */
	#define __S_IFCHR 0020000 /* Character device.  */
	#define __S_IFBLK 0060000 /* Block device.  */
	#define __S_IFREG 0100000 /* Regular file.  */
	#define __S_IFIFO 0010000 /* FIFO.  */
	#define __S_IFLNK 0120000 /* Symbolic link.  */
	#define __S_IFSOCK 0140000 /* Socket.  */
	 
	/* Protection bits.  */
	#define __S_ISUID 04000 /* Set user ID on execution.  */
	#define __S_ISGID 02000 /* Set group ID on execution.  */
	#define __S_ISVTX 01000 /* Save swapped text after use (sticky).  */
	#define __S_IREAD 0400 /* Read by owner.  */
	#define __S_IWRITE 0200 /* Write by owner.  */
	#define __S_IEXEC 0100 /* Execute by owner.  */
	#define __S_ISTYPE(mode, mask) (((mode) & __S_IFMT) == (mask))

	#define S_ISDIR(mode)  __S_ISTYPE((mode), __S_IFDIR)
	#define S_ISCHR(mode)  __S_ISTYPE((mode), __S_IFCHR)
	#define S_ISBLK(mode)  __S_ISTYPE((mode), __S_IFBLK)
	#define S_ISREG(mode)  __S_ISTYPE((mode), __S_IFREG)
	#define S_ISSOCK(mode) __S_ISTYPE((mode), __S_IFSOCK)
	#define S_ISFIFO(mode) __S_ISTYPE((mode), __S_IFSOCK)
	#define S_IRUSR __S_IREAD // Read by owner.  
	#define S_IWUSR __S_IWRITE // Write by owner. 
	#define S_IXUSR __S_IEXEC // Execute by owner. 
	#define S_IREAD S_IRUSR
	#define S_IWRITE S_IWUSR
	#define S_IEXEC S_IXUSR
	#define S_IFDIR __S_IFDIR
#endif 

#ifdef CONFIG_YAFFS_PROVIDE_DEFS
/* File types */

#ifndef DT_UNKNOWN
	#define DT_UNKNOWN	0
#endif
#ifndef	DT_FIFO
	#define DT_FIFO		1
#endif

#ifndef	DT_CHR
	#define DT_CHR		2
#endif

#ifndef	DT_DIR
	#define DT_DIR		4
#endif
#ifndef	DT_BLK
	#define DT_BLK		6
#endif
#ifndef	DT_REG
	#define DT_REG		8
#endif
#ifndef	DT_LNK
	#define DT_LNK		10
#endif
#ifndef	DT_SOCK
	#define DT_SOCK		12
#endif
#ifndef	 DT_WHT
	#define DT_WHT		14
#endif


/*
 * Attribute flags.
 * These are or-ed together to select what has been changed.
 */
#define ATTR_MODE	1
#define ATTR_UID	2
#define ATTR_GID	4
#define ATTR_SIZE	8
#define ATTR_ATIME	16
#define ATTR_MTIME	32
#define ATTR_CTIME	64

struct iattr {
	unsigned int ia_valid;
	unsigned ia_mode;
	unsigned ia_uid;
	unsigned ia_gid;
	unsigned ia_size;
	unsigned ia_atime;
	unsigned ia_mtime;
	unsigned ia_ctime;
	unsigned int ia_attr_flags;
};

#endif


#if defined CONFIG_YAFFS_WINCE

#include "ywinceenv.h"


#elif defined CONFIG_YAFFS_DIRECT

/* Direct interface */
#include "ydirectenv.h"

#elif defined CONFIG_YAFFS_UTIL

#include "yutilsenv.h"

#else
/* Should have specified a configuration type */
#error Unknown configuration

#endif

#if defined(CONFIG_YAFFS_DIRECT) || defined(CONFIG_YAFFS_WINCE)

#ifdef CONFIG_YAFFSFS_PROVIDE_VALUES

#ifndef O_RDONLY
#define O_RDONLY        00
#endif

#ifndef O_WRONLY
#define O_WRONLY	01
#endif

#ifndef O_RDWR
#define O_RDWR		02
#endif

#ifndef O_CREAT		
#define O_CREAT 	0100
#endif

#ifndef O_EXCL
#define O_EXCL		0200
#endif

#ifndef O_TRUNC
#define O_TRUNC		01000
#endif

#ifndef O_APPEND
#define O_APPEND	02000
#endif

#ifndef SEEK_SET
#define SEEK_SET	0
#endif

#ifndef SEEK_CUR
#define SEEK_CUR	1
#endif

#ifndef SEEK_END
#define SEEK_END	2
#endif

#ifndef EBUSY
#define EBUSY	16
#endif

#ifndef ENODEV
#define ENODEV	19
#endif

#ifndef EINVAL
#define EINVAL	22
#endif

#ifndef ENFILE
#define ENFILE	23
#endif

#ifndef EBADF
#define EBADF	9
#endif

#ifndef EACCES
#define EACCES	13
#endif

#ifndef EXDEV	
#define EXDEV	18
#endif

#ifndef ENOENT
#define ENOENT	2
#endif

#ifndef ENOSPC
#define ENOSPC	28
#endif

#ifndef EROFS
#define EROFS	30
#endif

#ifndef ERANGE
#define ERANGE 34
#endif

#ifndef ENODATA
#define ENODATA 61
#endif

#ifndef ENOTEMPTY
#define ENOTEMPTY 39
#endif

#ifndef ENAMETOOLONG
#define ENAMETOOLONG 36
#endif

#ifndef ENOMEM
#define ENOMEM 12
#endif

#ifndef EFAULT
#define EFAULT 14
#endif

#ifndef EEXIST
#define EEXIST 17
#endif

#ifndef ENOTDIR
#define ENOTDIR 20
#endif

#ifndef EISDIR
#define EISDIR 21
#endif

#ifndef ELOOP
#define ELOOP	40
#endif


// Mode flags

#ifndef S_IFMT
#define S_IFMT		0170000
#endif

#ifndef S_IFLNK
#define S_IFLNK		0120000
#endif

#ifndef S_IFDIR
#define S_IFDIR		0040000
#endif

#ifndef S_IFREG
#define S_IFREG		0100000
#endif

#ifndef S_IREAD 
#define S_IREAD		0000400
#endif

#ifndef S_IWRITE
#define	S_IWRITE	0000200
#endif

#ifndef S_IEXEC
#define	S_IEXEC	0000100
#endif

#ifndef XATTR_CREATE
#define XATTR_CREATE 1
#endif

#ifndef XATTR_REPLACE
#define XATTR_REPLACE 2
#endif

#ifndef R_OK
#define R_OK	4
#define W_OK	2
#define X_OK	1
#define F_OK	0
#endif

#else
//#include <errno.h>
//#include <sys/stat.h>
//#include <fcntl.h>
#ifndef XATTR_CREATE
#define XATTR_CREATE 1
#endif

#ifndef XATTR_REPLACE
#define XATTR_REPLACE 2
#endif

#endif

#endif

#ifndef Y_DUMP_STACK
#define Y_DUMP_STACK() do { } while (0)
#endif

#ifndef BUG
#define BUG() do {\
	yaffs_trace(YAFFS_TRACE_BUG,\
		"==>> yaffs bug: " __FILE__ " %d",\
		__LINE__);\
	Y_DUMP_STACK();\
} while (0)
#endif

#endif
