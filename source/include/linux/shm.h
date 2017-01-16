// /*AFLA*/ #ifndef _LINUX_SHM_H_
// /*AFLA*/ #define _LINUX_SHM_H_
// /*AFLA*/ 
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <asm/page.h>
// /*AFLA*/ #include <uapi/linux/shm.h>
// /*AFLA*/ #include <asm/shmparam.h>
// /*AFLA*/ 
// /*AFLA*/ struct shmid_kernel /* private to the kernel */
// /*AFLA*/ {	
// /*AFLA*/ 	struct kern_ipc_perm	shm_perm;
// /*AFLA*/ 	struct file		*shm_file;
// /*AFLA*/ 	unsigned long		shm_nattch;
// /*AFLA*/ 	unsigned long		shm_segsz;
// /*AFLA*/ 	time_t			shm_atim;
// /*AFLA*/ 	time_t			shm_dtim;
// /*AFLA*/ 	time_t			shm_ctim;
// /*AFLA*/ 	pid_t			shm_cprid;
// /*AFLA*/ 	pid_t			shm_lprid;
// /*AFLA*/ 	struct user_struct	*mlock_user;
// /*AFLA*/ 
// /*AFLA*/ 	/* The task created the shm object.  NULL if the task is dead. */
// /*AFLA*/ 	struct task_struct	*shm_creator;
// /*AFLA*/ 	struct list_head	shm_clist;	/* list by creator */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /* shm_mode upper byte flags */
// /*AFLA*/ #define	SHM_DEST	01000	/* segment will be destroyed on last detach */
// /*AFLA*/ #define SHM_LOCKED      02000   /* segment will not be swapped */
// /*AFLA*/ #define SHM_HUGETLB     04000   /* segment will use huge TLB pages */
// /*AFLA*/ #define SHM_NORESERVE   010000  /* don't check for reservations */
// /*AFLA*/ 
// /*AFLA*/ /* Bits [26:31] are reserved */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * When SHM_HUGETLB is set bits [26:31] encode the log2 of the huge page size.
// /*AFLA*/  * This gives us 6 bits, which is enough until someone invents 128 bit address
// /*AFLA*/  * spaces.
// /*AFLA*/  *
// /*AFLA*/  * Assume these are all power of twos.
// /*AFLA*/  * When 0 use the default page size.
// /*AFLA*/  */
// /*AFLA*/ #define SHM_HUGE_SHIFT  26
// /*AFLA*/ #define SHM_HUGE_MASK   0x3f
// /*AFLA*/ #define SHM_HUGE_2MB    (21 << SHM_HUGE_SHIFT)
// /*AFLA*/ #define SHM_HUGE_1GB    (30 << SHM_HUGE_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SYSVIPC
// /*AFLA*/ struct sysv_shm {
// /*AFLA*/ 	struct list_head shm_clist;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ long do_shmat(int shmid, char __user *shmaddr, int shmflg, unsigned long *addr,
// /*AFLA*/ 	      unsigned long shmlba);
// /*AFLA*/ bool is_file_shm_hugepages(struct file *file);
// /*AFLA*/ void exit_shm(struct task_struct *task);
// /*AFLA*/ #define shm_init_task(task) INIT_LIST_HEAD(&(task)->sysvshm.shm_clist)
// /*AFLA*/ #else
// /*AFLA*/ struct sysv_shm {
// /*AFLA*/ 	/* empty */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline long do_shmat(int shmid, char __user *shmaddr,
// /*AFLA*/ 			    int shmflg, unsigned long *addr,
// /*AFLA*/ 			    unsigned long shmlba)
// /*AFLA*/ {
// /*AFLA*/ 	return -ENOSYS;
// /*AFLA*/ }
// /*AFLA*/ static inline bool is_file_shm_hugepages(struct file *file)
// /*AFLA*/ {
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ static inline void exit_shm(struct task_struct *task)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void shm_init_task(struct task_struct *task)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_SHM_H_ */
