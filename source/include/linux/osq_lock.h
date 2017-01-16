// /*AFLA*/ #ifndef __LINUX_OSQ_LOCK_H
// /*AFLA*/ #define __LINUX_OSQ_LOCK_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * An MCS like lock especially tailored for optimistic spinning for sleeping
// /*AFLA*/  * lock implementations (mutex, rwsem, etc).
// /*AFLA*/  */
// /*AFLA*/ struct optimistic_spin_node {
// /*AFLA*/ 	struct optimistic_spin_node *next, *prev;
// /*AFLA*/ 	int locked; /* 1 if lock acquired */
// /*AFLA*/ 	int cpu; /* encoded CPU # + 1 value */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct optimistic_spin_queue {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Stores an encoded value of the CPU # of the tail node in the queue.
// /*AFLA*/ 	 * If the queue is empty, then it's set to OSQ_UNLOCKED_VAL.
// /*AFLA*/ 	 */
// /*AFLA*/ 	atomic_t tail;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define OSQ_UNLOCKED_VAL (0)
// /*AFLA*/ 
// /*AFLA*/ /* Init macro and function. */
// /*AFLA*/ #define OSQ_LOCK_UNLOCKED { ATOMIC_INIT(OSQ_UNLOCKED_VAL) }
// /*AFLA*/ 
// /*AFLA*/ static inline void osq_lock_init(struct optimistic_spin_queue *lock)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_set(&lock->tail, OSQ_UNLOCKED_VAL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern bool osq_lock(struct optimistic_spin_queue *lock);
// /*AFLA*/ extern void osq_unlock(struct optimistic_spin_queue *lock);
// /*AFLA*/ 
// /*AFLA*/ static inline bool osq_is_locked(struct optimistic_spin_queue *lock)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic_read(&lock->tail) != OSQ_UNLOCKED_VAL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif
