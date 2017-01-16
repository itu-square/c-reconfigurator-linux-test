// /*AFLA*/ #ifndef _LINUX_POSIX_TYPES_H
// /*AFLA*/ #define _LINUX_POSIX_TYPES_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/stddef.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This allows for 1024 file descriptors: if NR_OPEN is ever grown
// /*AFLA*/  * beyond that you'll have to change this too. But 1024 fd's seem to be
// /*AFLA*/  * enough even for such "real" unices like OSF/1, so hopefully this is
// /*AFLA*/  * one limit that doesn't have to be changed [again].
// /*AFLA*/  *
// /*AFLA*/  * Note that POSIX wants the FD_CLEAR(fd,fdsetp) defines to be in
// /*AFLA*/  * <sys/time.h> (and thus <linux/time.h>) - but this is a more logical
// /*AFLA*/  * place for them. Solved by having dummy defines in <sys/time.h>.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This macro may have been defined in <gnu/types.h>. But we always
// /*AFLA*/  * use the one here.
// /*AFLA*/  */
// /*AFLA*/ #undef __FD_SETSIZE
// /*AFLA*/ #define __FD_SETSIZE	1024
// /*AFLA*/ 
// /*AFLA*/ typedef struct {
// /*AFLA*/ 	unsigned long fds_bits[__FD_SETSIZE / (8 * sizeof(long))];
// /*AFLA*/ } __kernel_fd_set;
// /*AFLA*/ 
// /*AFLA*/ /* Type of a signal handler.  */
// /*AFLA*/ typedef void (*__kernel_sighandler_t)(int);
// /*AFLA*/ 
// /*AFLA*/ /* Type of a SYSV IPC key.  */
// /*AFLA*/ typedef int __kernel_key_t;
// /*AFLA*/ typedef int __kernel_mqd_t;
// /*AFLA*/ 
// /*AFLA*/ #include <asm/posix_types.h>
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_POSIX_TYPES_H */
