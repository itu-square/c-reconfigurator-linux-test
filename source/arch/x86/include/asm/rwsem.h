// /*AFLA*/ /* rwsem.h: R/W semaphores implemented using XADD/CMPXCHG for i486+
// /*AFLA*/  *
// /*AFLA*/  * Written by David Howells (dhowells@redhat.com).
// /*AFLA*/  *
// /*AFLA*/  * Derived from asm-x86/semaphore.h
// /*AFLA*/  *
// /*AFLA*/  *
// /*AFLA*/  * The MSW of the count is the negated number of active writers and waiting
// /*AFLA*/  * lockers, and the LSW is the total number of active locks
// /*AFLA*/  *
// /*AFLA*/  * The lock count is initialized to 0 (no active and no waiting lockers).
// /*AFLA*/  *
// /*AFLA*/  * When a writer subtracts WRITE_BIAS, it'll get 0xffff0001 for the case of an
// /*AFLA*/  * uncontended lock. This can be determined because XADD returns the old value.
// /*AFLA*/  * Readers increment by 1 and see a positive value when uncontended, negative
// /*AFLA*/  * if there are writers (and maybe) readers waiting (in which case it goes to
// /*AFLA*/  * sleep).
// /*AFLA*/  *
// /*AFLA*/  * The value of WAITING_BIAS supports up to 32766 waiting processes. This can
// /*AFLA*/  * be extended to 65534 by manually checking the whole MSW rather than relying
// /*AFLA*/  * on the S flag.
// /*AFLA*/  *
// /*AFLA*/  * The value of ACTIVE_BIAS supports up to 65535 active processes.
// /*AFLA*/  *
// /*AFLA*/  * This should be totally fair - if anything is waiting, a process that wants a
// /*AFLA*/  * lock will go to the back of the queue. When the currently active lock is
// /*AFLA*/  * released, if there's a writer at the front of the queue, then that and only
// /*AFLA*/  * that will be woken up; if there's a bunch of consecutive readers at the
// /*AFLA*/  * front, then they'll all be woken up, but no other readers will be.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _ASM_X86_RWSEM_H
// /*AFLA*/ #define _ASM_X86_RWSEM_H
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_RWSEM_H
// /*AFLA*/ #error "please don't include asm/rwsem.h directly, use linux/rwsem.h instead"
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef __KERNEL__
// /*AFLA*/ #include <asm/asm.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The bias values and the counter type limits the number of
// /*AFLA*/  * potential readers/writers to 32767 for 32 bits and 2147483647
// /*AFLA*/  * for 64 bits.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_64
// /*AFLA*/ # define RWSEM_ACTIVE_MASK		0xffffffffL
// /*AFLA*/ #else
// /*AFLA*/ # define RWSEM_ACTIVE_MASK		0x0000ffffL
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define RWSEM_UNLOCKED_VALUE		0x00000000L
// /*AFLA*/ #define RWSEM_ACTIVE_BIAS		0x00000001L
// /*AFLA*/ #define RWSEM_WAITING_BIAS		(-RWSEM_ACTIVE_MASK-1)
// /*AFLA*/ #define RWSEM_ACTIVE_READ_BIAS		RWSEM_ACTIVE_BIAS
// /*AFLA*/ #define RWSEM_ACTIVE_WRITE_BIAS		(RWSEM_WAITING_BIAS + RWSEM_ACTIVE_BIAS)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * lock for reading
// /*AFLA*/  */
// /*AFLA*/ static inline void __down_read(struct rw_semaphore *sem)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile("# beginning down_read\n\t"
// /*AFLA*/ 		     LOCK_PREFIX _ASM_INC "(%1)\n\t"
// /*AFLA*/ 		     /* adds 0x00000001 */
// /*AFLA*/ 		     "  jns        1f\n"
// /*AFLA*/ 		     "  call call_rwsem_down_read_failed\n"
// /*AFLA*/ 		     "1:\n\t"
// /*AFLA*/ 		     "# ending down_read\n\t"
// /*AFLA*/ 		     : "+m" (sem->count)
// /*AFLA*/ 		     : "a" (sem)
// /*AFLA*/ 		     : "memory", "cc");
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * trylock for reading -- returns 1 if successful, 0 if contention
// /*AFLA*/  */
// /*AFLA*/ static inline bool __down_read_trylock(struct rw_semaphore *sem)
// /*AFLA*/ {
// /*AFLA*/ 	long result, tmp;
// /*AFLA*/ 	asm volatile("# beginning __down_read_trylock\n\t"
// /*AFLA*/ 		     "  mov          %0,%1\n\t"
// /*AFLA*/ 		     "1:\n\t"
// /*AFLA*/ 		     "  mov          %1,%2\n\t"
// /*AFLA*/ 		     "  add          %3,%2\n\t"
// /*AFLA*/ 		     "  jle	     2f\n\t"
// /*AFLA*/ 		     LOCK_PREFIX "  cmpxchg  %2,%0\n\t"
// /*AFLA*/ 		     "  jnz	     1b\n\t"
// /*AFLA*/ 		     "2:\n\t"
// /*AFLA*/ 		     "# ending __down_read_trylock\n\t"
// /*AFLA*/ 		     : "+m" (sem->count), "=&a" (result), "=&r" (tmp)
// /*AFLA*/ 		     : "i" (RWSEM_ACTIVE_READ_BIAS)
// /*AFLA*/ 		     : "memory", "cc");
// /*AFLA*/ 	return result >= 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * lock for writing
// /*AFLA*/  */
// /*AFLA*/ #define ____down_write(sem, slow_path)			\
// /*AFLA*/ ({							\
// /*AFLA*/ 	long tmp;					\
// /*AFLA*/ 	struct rw_semaphore* ret;			\
// /*AFLA*/ 	register void *__sp asm(_ASM_SP);		\
// /*AFLA*/ 							\
// /*AFLA*/ 	asm volatile("# beginning down_write\n\t"	\
// /*AFLA*/ 		     LOCK_PREFIX "  xadd      %1,(%4)\n\t"	\
// /*AFLA*/ 		     /* adds 0xffff0001, returns the old value */ \
// /*AFLA*/ 		     "  test " __ASM_SEL(%w1,%k1) "," __ASM_SEL(%w1,%k1) "\n\t" \
// /*AFLA*/ 		     /* was the active mask 0 before? */\
// /*AFLA*/ 		     "  jz        1f\n"			\
// /*AFLA*/ 		     "  call " slow_path "\n"		\
// /*AFLA*/ 		     "1:\n"				\
// /*AFLA*/ 		     "# ending down_write"		\
// /*AFLA*/ 		     : "+m" (sem->count), "=d" (tmp), "=a" (ret), "+r" (__sp) \
// /*AFLA*/ 		     : "a" (sem), "1" (RWSEM_ACTIVE_WRITE_BIAS) \
// /*AFLA*/ 		     : "memory", "cc");			\
// /*AFLA*/ 	ret;						\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ static inline void __down_write(struct rw_semaphore *sem)
// /*AFLA*/ {
// /*AFLA*/ 	____down_write(sem, "call_rwsem_down_write_failed");
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int __down_write_killable(struct rw_semaphore *sem)
// /*AFLA*/ {
// /*AFLA*/ 	if (IS_ERR(____down_write(sem, "call_rwsem_down_write_failed_killable")))
// /*AFLA*/ 		return -EINTR;
// /*AFLA*/ 
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * trylock for writing -- returns 1 if successful, 0 if contention
// /*AFLA*/  */
// /*AFLA*/ static inline bool __down_write_trylock(struct rw_semaphore *sem)
// /*AFLA*/ {
// /*AFLA*/ 	bool result;
// /*AFLA*/ 	long tmp0, tmp1;
// /*AFLA*/ 	asm volatile("# beginning __down_write_trylock\n\t"
// /*AFLA*/ 		     "  mov          %0,%1\n\t"
// /*AFLA*/ 		     "1:\n\t"
// /*AFLA*/ 		     "  test " __ASM_SEL(%w1,%k1) "," __ASM_SEL(%w1,%k1) "\n\t"
// /*AFLA*/ 		     /* was the active mask 0 before? */
// /*AFLA*/ 		     "  jnz          2f\n\t"
// /*AFLA*/ 		     "  mov          %1,%2\n\t"
// /*AFLA*/ 		     "  add          %4,%2\n\t"
// /*AFLA*/ 		     LOCK_PREFIX "  cmpxchg  %2,%0\n\t"
// /*AFLA*/ 		     "  jnz	     1b\n\t"
// /*AFLA*/ 		     "2:\n\t"
// /*AFLA*/ 		     CC_SET(e)
// /*AFLA*/ 		     "# ending __down_write_trylock\n\t"
// /*AFLA*/ 		     : "+m" (sem->count), "=&a" (tmp0), "=&r" (tmp1),
// /*AFLA*/ 		       CC_OUT(e) (result)
// /*AFLA*/ 		     : "er" (RWSEM_ACTIVE_WRITE_BIAS)
// /*AFLA*/ 		     : "memory");
// /*AFLA*/ 	return result;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * unlock after reading
// /*AFLA*/  */
// /*AFLA*/ static inline void __up_read(struct rw_semaphore *sem)
// /*AFLA*/ {
// /*AFLA*/ 	long tmp;
// /*AFLA*/ 	asm volatile("# beginning __up_read\n\t"
// /*AFLA*/ 		     LOCK_PREFIX "  xadd      %1,(%2)\n\t"
// /*AFLA*/ 		     /* subtracts 1, returns the old value */
// /*AFLA*/ 		     "  jns        1f\n\t"
// /*AFLA*/ 		     "  call call_rwsem_wake\n" /* expects old value in %edx */
// /*AFLA*/ 		     "1:\n"
// /*AFLA*/ 		     "# ending __up_read\n"
// /*AFLA*/ 		     : "+m" (sem->count), "=d" (tmp)
// /*AFLA*/ 		     : "a" (sem), "1" (-RWSEM_ACTIVE_READ_BIAS)
// /*AFLA*/ 		     : "memory", "cc");
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * unlock after writing
// /*AFLA*/  */
// /*AFLA*/ static inline void __up_write(struct rw_semaphore *sem)
// /*AFLA*/ {
// /*AFLA*/ 	long tmp;
// /*AFLA*/ 	asm volatile("# beginning __up_write\n\t"
// /*AFLA*/ 		     LOCK_PREFIX "  xadd      %1,(%2)\n\t"
// /*AFLA*/ 		     /* subtracts 0xffff0001, returns the old value */
// /*AFLA*/ 		     "  jns        1f\n\t"
// /*AFLA*/ 		     "  call call_rwsem_wake\n" /* expects old value in %edx */
// /*AFLA*/ 		     "1:\n\t"
// /*AFLA*/ 		     "# ending __up_write\n"
// /*AFLA*/ 		     : "+m" (sem->count), "=d" (tmp)
// /*AFLA*/ 		     : "a" (sem), "1" (-RWSEM_ACTIVE_WRITE_BIAS)
// /*AFLA*/ 		     : "memory", "cc");
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * downgrade write lock to read lock
// /*AFLA*/  */
// /*AFLA*/ static inline void __downgrade_write(struct rw_semaphore *sem)
// /*AFLA*/ {
// /*AFLA*/ 	asm volatile("# beginning __downgrade_write\n\t"
// /*AFLA*/ 		     LOCK_PREFIX _ASM_ADD "%2,(%1)\n\t"
// /*AFLA*/ 		     /*
// /*AFLA*/ 		      * transitions 0xZZZZ0001 -> 0xYYYY0001 (i386)
// /*AFLA*/ 		      *     0xZZZZZZZZ00000001 -> 0xYYYYYYYY00000001 (x86_64)
// /*AFLA*/ 		      */
// /*AFLA*/ 		     "  jns       1f\n\t"
// /*AFLA*/ 		     "  call call_rwsem_downgrade_wake\n"
// /*AFLA*/ 		     "1:\n\t"
// /*AFLA*/ 		     "# ending __downgrade_write\n"
// /*AFLA*/ 		     : "+m" (sem->count)
// /*AFLA*/ 		     : "a" (sem), "er" (-RWSEM_WAITING_BIAS)
// /*AFLA*/ 		     : "memory", "cc");
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* __KERNEL__ */
// /*AFLA*/ #endif /* _ASM_X86_RWSEM_H */
