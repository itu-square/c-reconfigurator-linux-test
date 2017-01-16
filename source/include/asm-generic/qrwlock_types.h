// /*AFLA*/ #ifndef __ASM_GENERIC_QRWLOCK_TYPES_H
// /*AFLA*/ #define __ASM_GENERIC_QRWLOCK_TYPES_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <asm/spinlock_types.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The queue read/write lock data structure
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ typedef struct qrwlock {
// /*AFLA*/ 	atomic_t		cnts;
// /*AFLA*/ 	arch_spinlock_t		wait_lock;
// /*AFLA*/ } arch_rwlock_t;
// /*AFLA*/ 
// /*AFLA*/ #define	__ARCH_RW_LOCK_UNLOCKED {		\
// /*AFLA*/ 	.cnts = ATOMIC_INIT(0),			\
// /*AFLA*/ 	.wait_lock = __ARCH_SPIN_LOCK_UNLOCKED,	\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASM_GENERIC_QRWLOCK_TYPES_H */
