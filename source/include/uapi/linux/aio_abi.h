// /*AFLA*/ /* include/linux/aio_abi.h
// /*AFLA*/  *
// /*AFLA*/  * Copyright 2000,2001,2002 Red Hat.
// /*AFLA*/  *
// /*AFLA*/  * Written by Benjamin LaHaise <bcrl@kvack.org>
// /*AFLA*/  *
// /*AFLA*/  * Distribute under the terms of the GPLv2 (see ../../COPYING) or under 
// /*AFLA*/  * the following terms.
// /*AFLA*/  *
// /*AFLA*/  * Permission to use, copy, modify, and distribute this software and its
// /*AFLA*/  * documentation is hereby granted, provided that the above copyright
// /*AFLA*/  * notice appears in all copies.  This software is provided without any
// /*AFLA*/  * warranty, express or implied.  Red Hat makes no representations about
// /*AFLA*/  * the suitability of this software for any purpose.
// /*AFLA*/  *
// /*AFLA*/  * IN NO EVENT SHALL RED HAT BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
// /*AFLA*/  * SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF
// /*AFLA*/  * THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF RED HAT HAS BEEN ADVISED
// /*AFLA*/  * OF THE POSSIBILITY OF SUCH DAMAGE.
// /*AFLA*/  *
// /*AFLA*/  * RED HAT DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// /*AFLA*/  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// /*AFLA*/  * PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND
// /*AFLA*/  * RED HAT HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
// /*AFLA*/  * ENHANCEMENTS, OR MODIFICATIONS.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __LINUX__AIO_ABI_H
// /*AFLA*/ #define __LINUX__AIO_ABI_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <asm/byteorder.h>
// /*AFLA*/ 
// /*AFLA*/ typedef __kernel_ulong_t aio_context_t;
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	IOCB_CMD_PREAD = 0,
// /*AFLA*/ 	IOCB_CMD_PWRITE = 1,
// /*AFLA*/ 	IOCB_CMD_FSYNC = 2,
// /*AFLA*/ 	IOCB_CMD_FDSYNC = 3,
// /*AFLA*/ 	/* These two are experimental.
// /*AFLA*/ 	 * IOCB_CMD_PREADX = 4,
// /*AFLA*/ 	 * IOCB_CMD_POLL = 5,
// /*AFLA*/ 	 */
// /*AFLA*/ 	IOCB_CMD_NOOP = 6,
// /*AFLA*/ 	IOCB_CMD_PREADV = 7,
// /*AFLA*/ 	IOCB_CMD_PWRITEV = 8,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Valid flags for the "aio_flags" member of the "struct iocb".
// /*AFLA*/  *
// /*AFLA*/  * IOCB_FLAG_RESFD - Set if the "aio_resfd" member of the "struct iocb"
// /*AFLA*/  *                   is valid.
// /*AFLA*/  */
// /*AFLA*/ #define IOCB_FLAG_RESFD		(1 << 0)
// /*AFLA*/ 
// /*AFLA*/ /* read() from /dev/aio returns these structures. */
// /*AFLA*/ struct io_event {
// /*AFLA*/ 	__u64		data;		/* the data field from the iocb */
// /*AFLA*/ 	__u64		obj;		/* what iocb this event came from */
// /*AFLA*/ 	__s64		res;		/* result code for this event */
// /*AFLA*/ 	__s64		res2;		/* secondary result */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #if defined(__BYTE_ORDER) ? __BYTE_ORDER == __LITTLE_ENDIAN : defined(__LITTLE_ENDIAN)
// /*AFLA*/ #define PADDED(x,y)	x, y
// /*AFLA*/ #elif defined(__BYTE_ORDER) ? __BYTE_ORDER == __BIG_ENDIAN : defined(__BIG_ENDIAN)
// /*AFLA*/ #define PADDED(x,y)	y, x
// /*AFLA*/ #else
// /*AFLA*/ #error edit for your odd byteorder.
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * we always use a 64bit off_t when communicating
// /*AFLA*/  * with userland.  its up to libraries to do the
// /*AFLA*/  * proper padding and aio_error abstraction
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ struct iocb {
// /*AFLA*/ 	/* these are internal to the kernel/libc. */
// /*AFLA*/ 	__u64	aio_data;	/* data to be returned in event's data */
// /*AFLA*/ 	__u32	PADDED(aio_key, aio_reserved1);
// /*AFLA*/ 				/* the kernel sets aio_key to the req # */
// /*AFLA*/ 
// /*AFLA*/ 	/* common fields */
// /*AFLA*/ 	__u16	aio_lio_opcode;	/* see IOCB_CMD_ above */
// /*AFLA*/ 	__s16	aio_reqprio;
// /*AFLA*/ 	__u32	aio_fildes;
// /*AFLA*/ 
// /*AFLA*/ 	__u64	aio_buf;
// /*AFLA*/ 	__u64	aio_nbytes;
// /*AFLA*/ 	__s64	aio_offset;
// /*AFLA*/ 
// /*AFLA*/ 	/* extra parameters */
// /*AFLA*/ 	__u64	aio_reserved2;	/* TODO: use this for a (struct sigevent *) */
// /*AFLA*/ 
// /*AFLA*/ 	/* flags for the "struct iocb" */
// /*AFLA*/ 	__u32	aio_flags;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * if the IOCB_FLAG_RESFD flag of "aio_flags" is set, this is an
// /*AFLA*/ 	 * eventfd to signal AIO readiness to
// /*AFLA*/ 	 */
// /*AFLA*/ 	__u32	aio_resfd;
// /*AFLA*/ }; /* 64 bytes */
// /*AFLA*/ 
// /*AFLA*/ #undef IFBIG
// /*AFLA*/ #undef IFLITTLE
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX__AIO_ABI_H */
// /*AFLA*/ 
