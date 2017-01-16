// /*AFLA*/ #ifndef _LINUX_TYPES_H
// /*AFLA*/ #define _LINUX_TYPES_H
// /*AFLA*/ 
// /*AFLA*/ #define __EXPORTED_HEADERS__
// /*AFLA*/ #include <uapi/linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ #define DECLARE_BITMAP(name,bits) \
// /*AFLA*/ 	unsigned long name[BITS_TO_LONGS(bits)]
// /*AFLA*/ 
// /*AFLA*/ typedef __u32 __kernel_dev_t;
// /*AFLA*/ 
// /*AFLA*/ typedef __kernel_fd_set		fd_set;
// /*AFLA*/ typedef __kernel_dev_t		dev_t;
// /*AFLA*/ typedef __kernel_ino_t		ino_t;
// /*AFLA*/ typedef __kernel_mode_t		mode_t;
// /*AFLA*/ typedef unsigned short		umode_t;
// /*AFLA*/ typedef __u32			nlink_t;
// /*AFLA*/ typedef __kernel_off_t		off_t;
// /*AFLA*/ typedef __kernel_pid_t		pid_t;
// /*AFLA*/ typedef __kernel_daddr_t	daddr_t;
// /*AFLA*/ typedef __kernel_key_t		key_t;
// /*AFLA*/ typedef __kernel_suseconds_t	suseconds_t;
// /*AFLA*/ typedef __kernel_timer_t	timer_t;
// /*AFLA*/ typedef __kernel_clockid_t	clockid_t;
// /*AFLA*/ typedef __kernel_mqd_t		mqd_t;
// /*AFLA*/ 
// /*AFLA*/ typedef _Bool			bool;
// /*AFLA*/ 
// /*AFLA*/ typedef __kernel_uid32_t	uid_t;
// /*AFLA*/ typedef __kernel_gid32_t	gid_t;
// /*AFLA*/ typedef __kernel_uid16_t        uid16_t;
// /*AFLA*/ typedef __kernel_gid16_t        gid16_t;
// /*AFLA*/ 
// /*AFLA*/ typedef unsigned long		uintptr_t;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HAVE_UID16
// /*AFLA*/ /* This is defined by include/asm-{arch}/posix_types.h */
// /*AFLA*/ typedef __kernel_old_uid_t	old_uid_t;
// /*AFLA*/ typedef __kernel_old_gid_t	old_gid_t;
// /*AFLA*/ #endif /* CONFIG_UID16 */
// /*AFLA*/ 
// /*AFLA*/ #if defined(__GNUC__)
// /*AFLA*/ typedef __kernel_loff_t		loff_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The following typedefs are also protected by individual ifdefs for
// /*AFLA*/  * historical reasons:
// /*AFLA*/  */
// /*AFLA*/ #ifndef _SIZE_T
// /*AFLA*/ #define _SIZE_T
// /*AFLA*/ typedef __kernel_size_t		size_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef _SSIZE_T
// /*AFLA*/ #define _SSIZE_T
// /*AFLA*/ typedef __kernel_ssize_t	ssize_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef _PTRDIFF_T
// /*AFLA*/ #define _PTRDIFF_T
// /*AFLA*/ typedef __kernel_ptrdiff_t	ptrdiff_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef _TIME_T
// /*AFLA*/ #define _TIME_T
// /*AFLA*/ typedef __kernel_time_t		time_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef _CLOCK_T
// /*AFLA*/ #define _CLOCK_T
// /*AFLA*/ typedef __kernel_clock_t	clock_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef _CADDR_T
// /*AFLA*/ #define _CADDR_T
// /*AFLA*/ typedef __kernel_caddr_t	caddr_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* bsd */
// /*AFLA*/ typedef unsigned char		u_char;
// /*AFLA*/ typedef unsigned short		u_short;
// /*AFLA*/ typedef unsigned int		u_int;
// /*AFLA*/ typedef unsigned long		u_long;
// /*AFLA*/ 
// /*AFLA*/ /* sysv */
// /*AFLA*/ typedef unsigned char		unchar;
// /*AFLA*/ typedef unsigned short		ushort;
// /*AFLA*/ typedef unsigned int		uint;
// /*AFLA*/ typedef unsigned long		ulong;
// /*AFLA*/ 
// /*AFLA*/ #ifndef __BIT_TYPES_DEFINED__
// /*AFLA*/ #define __BIT_TYPES_DEFINED__
// /*AFLA*/ 
// /*AFLA*/ typedef		__u8		u_int8_t;
// /*AFLA*/ typedef		__s8		int8_t;
// /*AFLA*/ typedef		__u16		u_int16_t;
// /*AFLA*/ typedef		__s16		int16_t;
// /*AFLA*/ typedef		__u32		u_int32_t;
// /*AFLA*/ typedef		__s32		int32_t;
// /*AFLA*/ 
// /*AFLA*/ #endif /* !(__BIT_TYPES_DEFINED__) */
// /*AFLA*/ 
// /*AFLA*/ typedef		__u8		uint8_t;
// /*AFLA*/ typedef		__u16		uint16_t;
// /*AFLA*/ typedef		__u32		uint32_t;
// /*AFLA*/ 
// /*AFLA*/ #if defined(__GNUC__)
// /*AFLA*/ typedef		__u64		uint64_t;
// /*AFLA*/ typedef		__u64		u_int64_t;
// /*AFLA*/ typedef		__s64		int64_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* this is a special 64bit data type that is 8-byte aligned */
// /*AFLA*/ #define aligned_u64 __u64 __attribute__((aligned(8)))
// /*AFLA*/ #define aligned_be64 __be64 __attribute__((aligned(8)))
// /*AFLA*/ #define aligned_le64 __le64 __attribute__((aligned(8)))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * The type used for indexing onto a disc or disc partition.
// /*AFLA*/  *
// /*AFLA*/  * Linux always considers sectors to be 512 bytes long independently
// /*AFLA*/  * of the devices real block size.
// /*AFLA*/  *
// /*AFLA*/  * blkcnt_t is the type of the inode's block count.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_LBDAF
// /*AFLA*/ typedef u64 sector_t;
// /*AFLA*/ typedef u64 blkcnt_t;
// /*AFLA*/ #else
// /*AFLA*/ typedef unsigned long sector_t;
// /*AFLA*/ typedef unsigned long blkcnt_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The type of an index into the pagecache.
// /*AFLA*/  */
// /*AFLA*/ #define pgoff_t unsigned long
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * A dma_addr_t can hold any valid DMA address, i.e., any address returned
// /*AFLA*/  * by the DMA API.
// /*AFLA*/  *
// /*AFLA*/  * If the DMA API only uses 32-bit addresses, dma_addr_t need only be 32
// /*AFLA*/  * bits wide.  Bus addresses, e.g., PCI BARs, may be wider than 32 bits,
// /*AFLA*/  * but drivers do memory-mapped I/O to ioremapped kernel virtual addresses,
// /*AFLA*/  * so they don't care about the size of the actual bus addresses.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_ARCH_DMA_ADDR_T_64BIT
// /*AFLA*/ typedef u64 dma_addr_t;
// /*AFLA*/ #else
// /*AFLA*/ typedef u32 dma_addr_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ typedef unsigned __bitwise__ gfp_t;
// /*AFLA*/ typedef unsigned __bitwise__ fmode_t;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PHYS_ADDR_T_64BIT
// /*AFLA*/ typedef u64 phys_addr_t;
// /*AFLA*/ #else
// /*AFLA*/ typedef u32 phys_addr_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ typedef phys_addr_t resource_size_t;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This type is the placeholder for a hardware interrupt number. It has to be
// /*AFLA*/  * big enough to enclose whatever representation is used by a given platform.
// /*AFLA*/  */
// /*AFLA*/ typedef unsigned long irq_hw_number_t;
// /*AFLA*/ 
// /*AFLA*/ typedef struct {
// /*AFLA*/ 	int counter;
// /*AFLA*/ } atomic_t;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_64BIT
// /*AFLA*/ typedef struct {
// /*AFLA*/ 	long counter;
// /*AFLA*/ } atomic64_t;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct list_head {
// /*AFLA*/ 	struct list_head *next, *prev;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct hlist_head {
// /*AFLA*/ 	struct hlist_node *first;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct hlist_node {
// /*AFLA*/ 	struct hlist_node *next, **pprev;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct ustat {
// /*AFLA*/ 	__kernel_daddr_t	f_tfree;
// /*AFLA*/ 	__kernel_ino_t		f_tinode;
// /*AFLA*/ 	char			f_fname[6];
// /*AFLA*/ 	char			f_fpack[6];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct callback_head - callback structure for use with RCU and task_work
// /*AFLA*/  * @next: next update requests in a list
// /*AFLA*/  * @func: actual update function to call after the grace period.
// /*AFLA*/  *
// /*AFLA*/  * The struct is aligned to size of pointer. On most architectures it happens
// /*AFLA*/  * naturally due ABI requirements, but some architectures (like CRIS) have
// /*AFLA*/  * weird ABI and we need to ask it explicitly.
// /*AFLA*/  *
// /*AFLA*/  * The alignment is required to guarantee that bits 0 and 1 of @next will be
// /*AFLA*/  * clear under normal conditions -- as long as we use call_rcu(),
// /*AFLA*/  * call_rcu_bh(), call_rcu_sched(), or call_srcu() to queue callback.
// /*AFLA*/  *
// /*AFLA*/  * This guarantee is important for few reasons:
// /*AFLA*/  *  - future call_rcu_lazy() will make use of lower bits in the pointer;
// /*AFLA*/  *  - the structure shares storage spacer in struct page with @compound_head,
// /*AFLA*/  *    which encode PageTail() in bit 0. The guarantee is needed to avoid
// /*AFLA*/  *    false-positive PageTail().
// /*AFLA*/  */
// /*AFLA*/ struct callback_head {
// /*AFLA*/ 	struct callback_head *next;
// /*AFLA*/ 	void (*func)(struct callback_head *head);
// /*AFLA*/ } __attribute__((aligned(sizeof(void *))));
// /*AFLA*/ #define rcu_head callback_head
// /*AFLA*/ 
// /*AFLA*/ typedef void (*rcu_callback_t)(struct rcu_head *head);
// /*AFLA*/ typedef void (*call_rcu_func_t)(struct rcu_head *head, rcu_callback_t func);
// /*AFLA*/ 
// /*AFLA*/ /* clocksource cycle base type */
// /*AFLA*/ typedef u64 cycle_t;
// /*AFLA*/ 
// /*AFLA*/ #endif /*  __ASSEMBLY__ */
// /*AFLA*/ #endif /* _LINUX_TYPES_H */
