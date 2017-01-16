// /*AFLA*/ #ifndef _UAPI_ASM_GENERIC_IOCTL_H
// /*AFLA*/ #define _UAPI_ASM_GENERIC_IOCTL_H
// /*AFLA*/ 
// /*AFLA*/ /* ioctl command encoding: 32 bits total, command in lower 16 bits,
// /*AFLA*/  * size of the parameter structure in the lower 14 bits of the
// /*AFLA*/  * upper 16 bits.
// /*AFLA*/  * Encoding the size of the parameter structure in the ioctl request
// /*AFLA*/  * is useful for catching programs compiled with old versions
// /*AFLA*/  * and to avoid overwriting user space outside the user buffer area.
// /*AFLA*/  * The highest 2 bits are reserved for indicating the ``access mode''.
// /*AFLA*/  * NOTE: This limits the max parameter size to 16kB -1 !
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The following is for compatibility across the various Linux
// /*AFLA*/  * platforms.  The generic ioctl numbering scheme doesn't really enforce
// /*AFLA*/  * a type field.  De facto, however, the top 8 bits of the lower 16
// /*AFLA*/  * bits are indeed used as a type field, so we might just as well make
// /*AFLA*/  * this explicit here.  Please be sure to use the decoding macros
// /*AFLA*/  * below from now on.
// /*AFLA*/  */
// /*AFLA*/ #define _IOC_NRBITS	8
// /*AFLA*/ #define _IOC_TYPEBITS	8
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Let any architecture override either of the following before
// /*AFLA*/  * including this file.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _IOC_SIZEBITS
// /*AFLA*/ # define _IOC_SIZEBITS	14
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef _IOC_DIRBITS
// /*AFLA*/ # define _IOC_DIRBITS	2
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define _IOC_NRMASK	((1 << _IOC_NRBITS)-1)
// /*AFLA*/ #define _IOC_TYPEMASK	((1 << _IOC_TYPEBITS)-1)
// /*AFLA*/ #define _IOC_SIZEMASK	((1 << _IOC_SIZEBITS)-1)
// /*AFLA*/ #define _IOC_DIRMASK	((1 << _IOC_DIRBITS)-1)
// /*AFLA*/ 
// /*AFLA*/ #define _IOC_NRSHIFT	0
// /*AFLA*/ #define _IOC_TYPESHIFT	(_IOC_NRSHIFT+_IOC_NRBITS)
// /*AFLA*/ #define _IOC_SIZESHIFT	(_IOC_TYPESHIFT+_IOC_TYPEBITS)
// /*AFLA*/ #define _IOC_DIRSHIFT	(_IOC_SIZESHIFT+_IOC_SIZEBITS)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Direction bits, which any architecture can choose to override
// /*AFLA*/  * before including this file.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _IOC_NONE
// /*AFLA*/ # define _IOC_NONE	0U
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef _IOC_WRITE
// /*AFLA*/ # define _IOC_WRITE	1U
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef _IOC_READ
// /*AFLA*/ # define _IOC_READ	2U
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define _IOC(dir,type,nr,size) \
// /*AFLA*/ 	(((dir)  << _IOC_DIRSHIFT) | \
// /*AFLA*/ 	 ((type) << _IOC_TYPESHIFT) | \
// /*AFLA*/ 	 ((nr)   << _IOC_NRSHIFT) | \
// /*AFLA*/ 	 ((size) << _IOC_SIZESHIFT))
// /*AFLA*/ 
// /*AFLA*/ #ifndef __KERNEL__
// /*AFLA*/ #define _IOC_TYPECHECK(t) (sizeof(t))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* used to create numbers */
// /*AFLA*/ #define _IO(type,nr)		_IOC(_IOC_NONE,(type),(nr),0)
// /*AFLA*/ #define _IOR(type,nr,size)	_IOC(_IOC_READ,(type),(nr),(_IOC_TYPECHECK(size)))
// /*AFLA*/ #define _IOW(type,nr,size)	_IOC(_IOC_WRITE,(type),(nr),(_IOC_TYPECHECK(size)))
// /*AFLA*/ #define _IOWR(type,nr,size)	_IOC(_IOC_READ|_IOC_WRITE,(type),(nr),(_IOC_TYPECHECK(size)))
// /*AFLA*/ #define _IOR_BAD(type,nr,size)	_IOC(_IOC_READ,(type),(nr),sizeof(size))
// /*AFLA*/ #define _IOW_BAD(type,nr,size)	_IOC(_IOC_WRITE,(type),(nr),sizeof(size))
// /*AFLA*/ #define _IOWR_BAD(type,nr,size)	_IOC(_IOC_READ|_IOC_WRITE,(type),(nr),sizeof(size))
// /*AFLA*/ 
// /*AFLA*/ /* used to decode ioctl numbers.. */
// /*AFLA*/ #define _IOC_DIR(nr)		(((nr) >> _IOC_DIRSHIFT) & _IOC_DIRMASK)
// /*AFLA*/ #define _IOC_TYPE(nr)		(((nr) >> _IOC_TYPESHIFT) & _IOC_TYPEMASK)
// /*AFLA*/ #define _IOC_NR(nr)		(((nr) >> _IOC_NRSHIFT) & _IOC_NRMASK)
// /*AFLA*/ #define _IOC_SIZE(nr)		(((nr) >> _IOC_SIZESHIFT) & _IOC_SIZEMASK)
// /*AFLA*/ 
// /*AFLA*/ /* ...and for the drivers/sound files... */
// /*AFLA*/ 
// /*AFLA*/ #define IOC_IN		(_IOC_WRITE << _IOC_DIRSHIFT)
// /*AFLA*/ #define IOC_OUT		(_IOC_READ << _IOC_DIRSHIFT)
// /*AFLA*/ #define IOC_INOUT	((_IOC_WRITE|_IOC_READ) << _IOC_DIRSHIFT)
// /*AFLA*/ #define IOCSIZE_MASK	(_IOC_SIZEMASK << _IOC_SIZESHIFT)
// /*AFLA*/ #define IOCSIZE_SHIFT	(_IOC_SIZESHIFT)
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_ASM_GENERIC_IOCTL_H */
