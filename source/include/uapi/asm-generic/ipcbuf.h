// /*AFLA*/ #ifndef __ASM_GENERIC_IPCBUF_H
// /*AFLA*/ #define __ASM_GENERIC_IPCBUF_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The generic ipc64_perm structure:
// /*AFLA*/  * Note extra padding because this structure is passed back and forth
// /*AFLA*/  * between kernel and user space.
// /*AFLA*/  *
// /*AFLA*/  * ipc64_perm was originally meant to be architecture specific, but
// /*AFLA*/  * everyone just ended up making identical copies without specific
// /*AFLA*/  * optimizations, so we may just as well all use the same one.
// /*AFLA*/  *
// /*AFLA*/  * Pad space is left for:
// /*AFLA*/  * - 32-bit mode_t on architectures that only had 16 bit
// /*AFLA*/  * - 32-bit seq
// /*AFLA*/  * - 2 miscellaneous 32-bit values
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ struct ipc64_perm {
// /*AFLA*/ 	__kernel_key_t		key;
// /*AFLA*/ 	__kernel_uid32_t	uid;
// /*AFLA*/ 	__kernel_gid32_t	gid;
// /*AFLA*/ 	__kernel_uid32_t	cuid;
// /*AFLA*/ 	__kernel_gid32_t	cgid;
// /*AFLA*/ 	__kernel_mode_t		mode;
// /*AFLA*/ 				/* pad if mode_t is u16: */
// /*AFLA*/ 	unsigned char		__pad1[4 - sizeof(__kernel_mode_t)];
// /*AFLA*/ 	unsigned short		seq;
// /*AFLA*/ 	unsigned short		__pad2;
// /*AFLA*/ 	__kernel_ulong_t	__unused1;
// /*AFLA*/ 	__kernel_ulong_t	__unused2;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASM_GENERIC_IPCBUF_H */
