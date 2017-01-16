// /*AFLA*/ #ifndef _LINUX_MM_H
// /*AFLA*/ #define _LINUX_MM_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/errno.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef __KERNEL__
// /*AFLA*/ 
// /*AFLA*/ #include <linux/mmdebug.h>
// /*AFLA*/ #include <linux/gfp.h>
// /*AFLA*/ #include <linux/bug.h>
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/mmzone.h>
// /*AFLA*/ #include <linux/rbtree.h>
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <linux/debug_locks.h>
// /*AFLA*/ #include <linux/mm_types.h>
// /*AFLA*/ #include <linux/range.h>
// /*AFLA*/ #include <linux/pfn.h>
// /*AFLA*/ #include <linux/percpu-refcount.h>
// /*AFLA*/ #include <linux/bit_spinlock.h>
// /*AFLA*/ #include <linux/shrinker.h>
// /*AFLA*/ #include <linux/resource.h>
// /*AFLA*/ #include <linux/page_ext.h>
// /*AFLA*/ #include <linux/err.h>
// /*AFLA*/ #include <linux/page_ref.h>
// /*AFLA*/ 
// /*AFLA*/ struct mempolicy;
// /*AFLA*/ struct anon_vma;
// /*AFLA*/ struct anon_vma_chain;
// /*AFLA*/ struct file_ra_state;
// /*AFLA*/ struct user_struct;
// /*AFLA*/ struct writeback_control;
// /*AFLA*/ struct bdi_writeback;
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_NEED_MULTIPLE_NODES	/* Don't use mapnrs, do it properly */
// /*AFLA*/ extern unsigned long max_mapnr;
// /*AFLA*/ 
// /*AFLA*/ static inline void set_max_mapnr(unsigned long limit)
// /*AFLA*/ {
// /*AFLA*/ 	max_mapnr = limit;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline void set_max_mapnr(unsigned long limit) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long totalram_pages;
// /*AFLA*/ extern void * high_memory;
// /*AFLA*/ extern int page_cluster;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SYSCTL
// /*AFLA*/ extern int sysctl_legacy_va_layout;
// /*AFLA*/ #else
// /*AFLA*/ #define sysctl_legacy_va_layout 0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HAVE_ARCH_MMAP_RND_BITS
// /*AFLA*/ extern const int mmap_rnd_bits_min;
// /*AFLA*/ extern const int mmap_rnd_bits_max;
// /*AFLA*/ extern int mmap_rnd_bits __read_mostly;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_HAVE_ARCH_MMAP_RND_COMPAT_BITS
// /*AFLA*/ extern const int mmap_rnd_compat_bits_min;
// /*AFLA*/ extern const int mmap_rnd_compat_bits_max;
// /*AFLA*/ extern int mmap_rnd_compat_bits __read_mostly;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #include <asm/page.h>
// /*AFLA*/ #include <asm/pgtable.h>
// /*AFLA*/ #include <asm/processor.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef __pa_symbol
// /*AFLA*/ #define __pa_symbol(x)  __pa(RELOC_HIDE((unsigned long)(x), 0))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef page_to_virt
// /*AFLA*/ #define page_to_virt(x)	__va(PFN_PHYS(page_to_pfn(x)))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * To prevent common memory management code establishing
// /*AFLA*/  * a zero page mapping on a read fault.
// /*AFLA*/  * This macro should be defined within <asm/pgtable.h>.
// /*AFLA*/  * s390 does this to prevent multiplexing of hardware bits
// /*AFLA*/  * related to the physical page in case of virtualization.
// /*AFLA*/  */
// /*AFLA*/ #ifndef mm_forbids_zeropage
// /*AFLA*/ #define mm_forbids_zeropage(X)	(0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Default maximum number of active map areas, this limits the number of vmas
// /*AFLA*/  * per mm struct. Users can overwrite this number by sysctl but there is a
// /*AFLA*/  * problem.
// /*AFLA*/  *
// /*AFLA*/  * When a program's coredump is generated as ELF format, a section is created
// /*AFLA*/  * per a vma. In ELF, the number of sections is represented in unsigned short.
// /*AFLA*/  * This means the number of sections should be smaller than 65535 at coredump.
// /*AFLA*/  * Because the kernel adds some informative sections to a image of program at
// /*AFLA*/  * generating coredump, we need some margin. The number of extra sections is
// /*AFLA*/  * 1-3 now and depends on arch. We use "5" as safe margin, here.
// /*AFLA*/  *
// /*AFLA*/  * ELF extended numbering allows more than 65535 sections, so 16-bit bound is
// /*AFLA*/  * not a hard limit any more. Although some userspace tools can be surprised by
// /*AFLA*/  * that.
// /*AFLA*/  */
// /*AFLA*/ #define MAPCOUNT_ELF_CORE_MARGIN	(5)
// /*AFLA*/ #define DEFAULT_MAX_MAP_COUNT	(USHRT_MAX - MAPCOUNT_ELF_CORE_MARGIN)
// /*AFLA*/ 
// /*AFLA*/ extern int sysctl_max_map_count;
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long sysctl_user_reserve_kbytes;
// /*AFLA*/ extern unsigned long sysctl_admin_reserve_kbytes;
// /*AFLA*/ 
// /*AFLA*/ extern int sysctl_overcommit_memory;
// /*AFLA*/ extern int sysctl_overcommit_ratio;
// /*AFLA*/ extern unsigned long sysctl_overcommit_kbytes;
// /*AFLA*/ 
// /*AFLA*/ extern int overcommit_ratio_handler(struct ctl_table *, int, void __user *,
// /*AFLA*/ 				    size_t *, loff_t *);
// /*AFLA*/ extern int overcommit_kbytes_handler(struct ctl_table *, int, void __user *,
// /*AFLA*/ 				    size_t *, loff_t *);
// /*AFLA*/ 
// /*AFLA*/ #define nth_page(page,n) pfn_to_page(page_to_pfn((page)) + (n))
// /*AFLA*/ 
// /*AFLA*/ /* to align the pointer to the (next) page boundary */
// /*AFLA*/ #define PAGE_ALIGN(addr) ALIGN(addr, PAGE_SIZE)
// /*AFLA*/ 
// /*AFLA*/ /* test whether an address (unsigned long or pointer) is aligned to PAGE_SIZE */
// /*AFLA*/ #define PAGE_ALIGNED(addr)	IS_ALIGNED((unsigned long)(addr), PAGE_SIZE)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Linux kernel virtual memory manager primitives.
// /*AFLA*/  * The idea being to have a "virtual" mm in the same way
// /*AFLA*/  * we have a virtual fs - giving a cleaner interface to the
// /*AFLA*/  * mm details, and allowing different kinds of memory mappings
// /*AFLA*/  * (from shared memory to executable loading to arbitrary
// /*AFLA*/  * mmap() functions).
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ extern struct kmem_cache *vm_area_cachep;
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_MMU
// /*AFLA*/ extern struct rb_root nommu_region_tree;
// /*AFLA*/ extern struct rw_semaphore nommu_region_sem;
// /*AFLA*/ 
// /*AFLA*/ extern unsigned int kobjsize(const void *objp);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * vm_flags in vm_area_struct, see mm_types.h.
// /*AFLA*/  * When changing, update also include/trace/events/mmflags.h
// /*AFLA*/  */
// /*AFLA*/ #define VM_NONE		0x00000000
// /*AFLA*/ 
// /*AFLA*/ #define VM_READ		0x00000001	/* currently active flags */
// /*AFLA*/ #define VM_WRITE	0x00000002
// /*AFLA*/ #define VM_EXEC		0x00000004
// /*AFLA*/ #define VM_SHARED	0x00000008
// /*AFLA*/ 
// /*AFLA*/ /* mprotect() hardcodes VM_MAYREAD >> 4 == VM_READ, and so for r/w/x bits. */
// /*AFLA*/ #define VM_MAYREAD	0x00000010	/* limits for mprotect() etc */
// /*AFLA*/ #define VM_MAYWRITE	0x00000020
// /*AFLA*/ #define VM_MAYEXEC	0x00000040
// /*AFLA*/ #define VM_MAYSHARE	0x00000080
// /*AFLA*/ 
// /*AFLA*/ #define VM_GROWSDOWN	0x00000100	/* general info on the segment */
// /*AFLA*/ #define VM_UFFD_MISSING	0x00000200	/* missing pages tracking */
// /*AFLA*/ #define VM_PFNMAP	0x00000400	/* Page-ranges managed without "struct page", just pure PFN */
// /*AFLA*/ #define VM_DENYWRITE	0x00000800	/* ETXTBSY on write attempts.. */
// /*AFLA*/ #define VM_UFFD_WP	0x00001000	/* wrprotect pages tracking */
// /*AFLA*/ 
// /*AFLA*/ #define VM_LOCKED	0x00002000
// /*AFLA*/ #define VM_IO           0x00004000	/* Memory mapped I/O or similar */
// /*AFLA*/ 
// /*AFLA*/ 					/* Used by sys_madvise() */
// /*AFLA*/ #define VM_SEQ_READ	0x00008000	/* App will access data sequentially */
// /*AFLA*/ #define VM_RAND_READ	0x00010000	/* App will not benefit from clustered reads */
// /*AFLA*/ 
// /*AFLA*/ #define VM_DONTCOPY	0x00020000      /* Do not copy this vma on fork */
// /*AFLA*/ #define VM_DONTEXPAND	0x00040000	/* Cannot expand with mremap() */
// /*AFLA*/ #define VM_LOCKONFAULT	0x00080000	/* Lock the pages covered when they are faulted in */
// /*AFLA*/ #define VM_ACCOUNT	0x00100000	/* Is a VM accounted object */
// /*AFLA*/ #define VM_NORESERVE	0x00200000	/* should the VM suppress accounting */
// /*AFLA*/ #define VM_HUGETLB	0x00400000	/* Huge TLB Page VM */
// /*AFLA*/ #define VM_ARCH_1	0x01000000	/* Architecture-specific flag */
// /*AFLA*/ #define VM_ARCH_2	0x02000000
// /*AFLA*/ #define VM_DONTDUMP	0x04000000	/* Do not include in the core dump */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MEM_SOFT_DIRTY
// /*AFLA*/ # define VM_SOFTDIRTY	0x08000000	/* Not soft dirty clean area */
// /*AFLA*/ #else
// /*AFLA*/ # define VM_SOFTDIRTY	0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define VM_MIXEDMAP	0x10000000	/* Can contain "struct page" and pure PFN pages */
// /*AFLA*/ #define VM_HUGEPAGE	0x20000000	/* MADV_HUGEPAGE marked this vma */
// /*AFLA*/ #define VM_NOHUGEPAGE	0x40000000	/* MADV_NOHUGEPAGE marked this vma */
// /*AFLA*/ #define VM_MERGEABLE	0x80000000	/* KSM may merge identical pages */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_ARCH_USES_HIGH_VMA_FLAGS
// /*AFLA*/ #define VM_HIGH_ARCH_BIT_0	32	/* bit only usable on 64-bit architectures */
// /*AFLA*/ #define VM_HIGH_ARCH_BIT_1	33	/* bit only usable on 64-bit architectures */
// /*AFLA*/ #define VM_HIGH_ARCH_BIT_2	34	/* bit only usable on 64-bit architectures */
// /*AFLA*/ #define VM_HIGH_ARCH_BIT_3	35	/* bit only usable on 64-bit architectures */
// /*AFLA*/ #define VM_HIGH_ARCH_0	BIT(VM_HIGH_ARCH_BIT_0)
// /*AFLA*/ #define VM_HIGH_ARCH_1	BIT(VM_HIGH_ARCH_BIT_1)
// /*AFLA*/ #define VM_HIGH_ARCH_2	BIT(VM_HIGH_ARCH_BIT_2)
// /*AFLA*/ #define VM_HIGH_ARCH_3	BIT(VM_HIGH_ARCH_BIT_3)
// /*AFLA*/ #endif /* CONFIG_ARCH_USES_HIGH_VMA_FLAGS */
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_X86)
// /*AFLA*/ # define VM_PAT		VM_ARCH_1	/* PAT reserves whole VMA at once (x86) */
// /*AFLA*/ #if defined (CONFIG_X86_INTEL_MEMORY_PROTECTION_KEYS)
// /*AFLA*/ # define VM_PKEY_SHIFT	VM_HIGH_ARCH_BIT_0
// /*AFLA*/ # define VM_PKEY_BIT0	VM_HIGH_ARCH_0	/* A protection key is a 4-bit value */
// /*AFLA*/ # define VM_PKEY_BIT1	VM_HIGH_ARCH_1
// /*AFLA*/ # define VM_PKEY_BIT2	VM_HIGH_ARCH_2
// /*AFLA*/ # define VM_PKEY_BIT3	VM_HIGH_ARCH_3
// /*AFLA*/ #endif
// /*AFLA*/ #elif defined(CONFIG_PPC)
// /*AFLA*/ # define VM_SAO		VM_ARCH_1	/* Strong Access Ordering (powerpc) */
// /*AFLA*/ #elif defined(CONFIG_PARISC)
// /*AFLA*/ # define VM_GROWSUP	VM_ARCH_1
// /*AFLA*/ #elif defined(CONFIG_METAG)
// /*AFLA*/ # define VM_GROWSUP	VM_ARCH_1
// /*AFLA*/ #elif defined(CONFIG_IA64)
// /*AFLA*/ # define VM_GROWSUP	VM_ARCH_1
// /*AFLA*/ #elif !defined(CONFIG_MMU)
// /*AFLA*/ # define VM_MAPPED_COPY	VM_ARCH_1	/* T if mapped copy of data (nommu mmap) */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_X86)
// /*AFLA*/ /* MPX specific bounds table or bounds directory */
// /*AFLA*/ # define VM_MPX		VM_ARCH_2
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef VM_GROWSUP
// /*AFLA*/ # define VM_GROWSUP	VM_NONE
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Bits set in the VMA until the stack is in its final location */
// /*AFLA*/ #define VM_STACK_INCOMPLETE_SETUP	(VM_RAND_READ | VM_SEQ_READ)
// /*AFLA*/ 
// /*AFLA*/ #ifndef VM_STACK_DEFAULT_FLAGS		/* arch can override this */
// /*AFLA*/ #define VM_STACK_DEFAULT_FLAGS VM_DATA_DEFAULT_FLAGS
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_STACK_GROWSUP
// /*AFLA*/ #define VM_STACK	VM_GROWSUP
// /*AFLA*/ #else
// /*AFLA*/ #define VM_STACK	VM_GROWSDOWN
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define VM_STACK_FLAGS	(VM_STACK | VM_STACK_DEFAULT_FLAGS | VM_ACCOUNT)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Special vmas that are non-mergable, non-mlock()able.
// /*AFLA*/  * Note: mm/huge_memory.c VM_NO_THP depends on this definition.
// /*AFLA*/  */
// /*AFLA*/ #define VM_SPECIAL (VM_IO | VM_DONTEXPAND | VM_PFNMAP | VM_MIXEDMAP)
// /*AFLA*/ 
// /*AFLA*/ /* This mask defines which mm->def_flags a process can inherit its parent */
// /*AFLA*/ #define VM_INIT_DEF_MASK	VM_NOHUGEPAGE
// /*AFLA*/ 
// /*AFLA*/ /* This mask is used to clear all the VMA flags used by mlock */
// /*AFLA*/ #define VM_LOCKED_CLEAR_MASK	(~(VM_LOCKED | VM_LOCKONFAULT))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * mapping from the currently active vm_flags protection bits (the
// /*AFLA*/  * low four bits) to a page protection mask..
// /*AFLA*/  */
// /*AFLA*/ extern pgprot_t protection_map[16];
// /*AFLA*/ 
// /*AFLA*/ #define FAULT_FLAG_WRITE	0x01	/* Fault was a write access */
// /*AFLA*/ #define FAULT_FLAG_MKWRITE	0x02	/* Fault was mkwrite of existing pte */
// /*AFLA*/ #define FAULT_FLAG_ALLOW_RETRY	0x04	/* Retry fault if blocking */
// /*AFLA*/ #define FAULT_FLAG_RETRY_NOWAIT	0x08	/* Don't drop mmap_sem and wait when retrying */
// /*AFLA*/ #define FAULT_FLAG_KILLABLE	0x10	/* The fault task is in SIGKILL killable region */
// /*AFLA*/ #define FAULT_FLAG_TRIED	0x20	/* Second try */
// /*AFLA*/ #define FAULT_FLAG_USER		0x40	/* The fault originated in userspace */
// /*AFLA*/ #define FAULT_FLAG_REMOTE	0x80	/* faulting for non current tsk/mm */
// /*AFLA*/ #define FAULT_FLAG_INSTRUCTION  0x100	/* The fault was during an instruction fetch */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * vm_fault is filled by the the pagefault handler and passed to the vma's
// /*AFLA*/  * ->fault function. The vma's ->fault is responsible for returning a bitmask
// /*AFLA*/  * of VM_FAULT_xxx flags that give details about how the fault was handled.
// /*AFLA*/  *
// /*AFLA*/  * MM layer fills up gfp_mask for page allocations but fault handler might
// /*AFLA*/  * alter it if its implementation requires a different allocation context.
// /*AFLA*/  *
// /*AFLA*/  * pgoff should be used in favour of virtual_address, if possible.
// /*AFLA*/  */
// /*AFLA*/ struct vm_fault {
// /*AFLA*/ 	unsigned int flags;		/* FAULT_FLAG_xxx flags */
// /*AFLA*/ 	gfp_t gfp_mask;			/* gfp mask to be used for allocations */
// /*AFLA*/ 	pgoff_t pgoff;			/* Logical page offset based on vma */
// /*AFLA*/ 	void __user *virtual_address;	/* Faulting virtual address */
// /*AFLA*/ 
// /*AFLA*/ 	struct page *cow_page;		/* Handler may choose to COW */
// /*AFLA*/ 	struct page *page;		/* ->fault handlers should return a
// /*AFLA*/ 					 * page here, unless VM_FAULT_NOPAGE
// /*AFLA*/ 					 * is set (which is also implied by
// /*AFLA*/ 					 * VM_FAULT_ERROR).
// /*AFLA*/ 					 */
// /*AFLA*/ 	void *entry;			/* ->fault handler can alternatively
// /*AFLA*/ 					 * return locked DAX entry. In that
// /*AFLA*/ 					 * case handler should return
// /*AFLA*/ 					 * VM_FAULT_DAX_LOCKED and fill in
// /*AFLA*/ 					 * entry here.
// /*AFLA*/ 					 */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Page fault context: passes though page fault handler instead of endless list
// /*AFLA*/  * of function arguments.
// /*AFLA*/  */
// /*AFLA*/ struct fault_env {
// /*AFLA*/ 	struct vm_area_struct *vma;	/* Target VMA */
// /*AFLA*/ 	unsigned long address;		/* Faulting virtual address */
// /*AFLA*/ 	unsigned int flags;		/* FAULT_FLAG_xxx flags */
// /*AFLA*/ 	pmd_t *pmd;			/* Pointer to pmd entry matching
// /*AFLA*/ 					 * the 'address'
// /*AFLA*/ 					 */
// /*AFLA*/ 	pte_t *pte;			/* Pointer to pte entry matching
// /*AFLA*/ 					 * the 'address'. NULL if the page
// /*AFLA*/ 					 * table hasn't been allocated.
// /*AFLA*/ 					 */
// /*AFLA*/ 	spinlock_t *ptl;		/* Page table lock.
// /*AFLA*/ 					 * Protects pte page table if 'pte'
// /*AFLA*/ 					 * is not NULL, otherwise pmd.
// /*AFLA*/ 					 */
// /*AFLA*/ 	pgtable_t prealloc_pte;		/* Pre-allocated pte page table.
// /*AFLA*/ 					 * vm_ops->map_pages() calls
// /*AFLA*/ 					 * alloc_set_pte() from atomic context.
// /*AFLA*/ 					 * do_fault_around() pre-allocates
// /*AFLA*/ 					 * page table to avoid allocation from
// /*AFLA*/ 					 * atomic context.
// /*AFLA*/ 					 */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These are the virtual MM functions - opening of an area, closing and
// /*AFLA*/  * unmapping it (needed to keep files on disk up-to-date etc), pointer
// /*AFLA*/  * to the functions called when a no-page or a wp-page exception occurs. 
// /*AFLA*/  */
// /*AFLA*/ struct vm_operations_struct {
// /*AFLA*/ 	void (*open)(struct vm_area_struct * area);
// /*AFLA*/ 	void (*close)(struct vm_area_struct * area);
// /*AFLA*/ 	int (*mremap)(struct vm_area_struct * area);
// /*AFLA*/ 	int (*fault)(struct vm_area_struct *vma, struct vm_fault *vmf);
// /*AFLA*/ 	int (*pmd_fault)(struct vm_area_struct *, unsigned long address,
// /*AFLA*/ 						pmd_t *, unsigned int flags);
// /*AFLA*/ 	void (*map_pages)(struct fault_env *fe,
// /*AFLA*/ 			pgoff_t start_pgoff, pgoff_t end_pgoff);
// /*AFLA*/ 
// /*AFLA*/ 	/* notification that a previously read-only page is about to become
// /*AFLA*/ 	 * writable, if an error is returned it will cause a SIGBUS */
// /*AFLA*/ 	int (*page_mkwrite)(struct vm_area_struct *vma, struct vm_fault *vmf);
// /*AFLA*/ 
// /*AFLA*/ 	/* same as page_mkwrite when using VM_PFNMAP|VM_MIXEDMAP */
// /*AFLA*/ 	int (*pfn_mkwrite)(struct vm_area_struct *vma, struct vm_fault *vmf);
// /*AFLA*/ 
// /*AFLA*/ 	/* called by access_process_vm when get_user_pages() fails, typically
// /*AFLA*/ 	 * for use by special VMAs that can switch between memory and hardware
// /*AFLA*/ 	 */
// /*AFLA*/ 	int (*access)(struct vm_area_struct *vma, unsigned long addr,
// /*AFLA*/ 		      void *buf, int len, int write);
// /*AFLA*/ 
// /*AFLA*/ 	/* Called by the /proc/PID/maps code to ask the vma whether it
// /*AFLA*/ 	 * has a special name.  Returning non-NULL will also cause this
// /*AFLA*/ 	 * vma to be dumped unconditionally. */
// /*AFLA*/ 	const char *(*name)(struct vm_area_struct *vma);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * set_policy() op must add a reference to any non-NULL @new mempolicy
// /*AFLA*/ 	 * to hold the policy upon return.  Caller should pass NULL @new to
// /*AFLA*/ 	 * remove a policy and fall back to surrounding context--i.e. do not
// /*AFLA*/ 	 * install a MPOL_DEFAULT policy, nor the task or system default
// /*AFLA*/ 	 * mempolicy.
// /*AFLA*/ 	 */
// /*AFLA*/ 	int (*set_policy)(struct vm_area_struct *vma, struct mempolicy *new);
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * get_policy() op must add reference [mpol_get()] to any policy at
// /*AFLA*/ 	 * (vma,addr) marked as MPOL_SHARED.  The shared policy infrastructure
// /*AFLA*/ 	 * in mm/mempolicy.c will do this automatically.
// /*AFLA*/ 	 * get_policy() must NOT add a ref if the policy at (vma,addr) is not
// /*AFLA*/ 	 * marked as MPOL_SHARED. vma policies are protected by the mmap_sem.
// /*AFLA*/ 	 * If no [shared/vma] mempolicy exists at the addr, get_policy() op
// /*AFLA*/ 	 * must return NULL--i.e., do not "fallback" to task or system default
// /*AFLA*/ 	 * policy.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct mempolicy *(*get_policy)(struct vm_area_struct *vma,
// /*AFLA*/ 					unsigned long addr);
// /*AFLA*/ #endif
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Called by vm_normal_page() for special PTEs to find the
// /*AFLA*/ 	 * page for @addr.  This is useful if the default behavior
// /*AFLA*/ 	 * (using pte_page()) would not find the correct page.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct page *(*find_special_page)(struct vm_area_struct *vma,
// /*AFLA*/ 					  unsigned long addr);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct mmu_gather;
// /*AFLA*/ struct inode;
// /*AFLA*/ 
// /*AFLA*/ #define page_private(page)		((page)->private)
// /*AFLA*/ #define set_page_private(page, v)	((page)->private = (v))
// /*AFLA*/ 
// /*AFLA*/ #if !defined(__HAVE_ARCH_PTE_DEVMAP) || !defined(CONFIG_TRANSPARENT_HUGEPAGE)
// /*AFLA*/ static inline int pmd_devmap(pmd_t pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * FIXME: take this include out, include page-flags.h in
// /*AFLA*/  * files which need it (119 of them)
// /*AFLA*/  */
// /*AFLA*/ #include <linux/page-flags.h>
// /*AFLA*/ #include <linux/huge_mm.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Methods to modify the page usage count.
// /*AFLA*/  *
// /*AFLA*/  * What counts for a page usage:
// /*AFLA*/  * - cache mapping   (page->mapping)
// /*AFLA*/  * - private data    (page->private)
// /*AFLA*/  * - page mapped in a task's page tables, each mapping
// /*AFLA*/  *   is counted separately
// /*AFLA*/  *
// /*AFLA*/  * Also, many kernel routines increase the page count before a critical
// /*AFLA*/  * routine so they can be sure the page doesn't go away from under them.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Drop a ref, return true if the refcount fell to zero (the page has no users)
// /*AFLA*/  */
// /*AFLA*/ static inline int put_page_testzero(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	VM_BUG_ON_PAGE(page_ref_count(page) == 0, page);
// /*AFLA*/ 	return page_ref_dec_and_test(page);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Try to grab a ref unless the page has a refcount of zero, return false if
// /*AFLA*/  * that is the case.
// /*AFLA*/  * This can be called when MMU is off so it must not access
// /*AFLA*/  * any of the virtual mappings.
// /*AFLA*/  */
// /*AFLA*/ static inline int get_page_unless_zero(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return page_ref_add_unless(page, 1, 0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int page_is_ram(unsigned long pfn);
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	REGION_INTERSECTS,
// /*AFLA*/ 	REGION_DISJOINT,
// /*AFLA*/ 	REGION_MIXED,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ int region_intersects(resource_size_t offset, size_t size, unsigned long flags,
// /*AFLA*/ 		      unsigned long desc);
// /*AFLA*/ 
// /*AFLA*/ /* Support for virtually mapped pages */
// /*AFLA*/ struct page *vmalloc_to_page(const void *addr);
// /*AFLA*/ unsigned long vmalloc_to_pfn(const void *addr);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Determine if an address is within the vmalloc range
// /*AFLA*/  *
// /*AFLA*/  * On nommu, vmalloc/vfree wrap through kmalloc/kfree directly, so there
// /*AFLA*/  * is no special casing required.
// /*AFLA*/  */
// /*AFLA*/ static inline bool is_vmalloc_addr(const void *x)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_MMU
// /*AFLA*/ 	unsigned long addr = (unsigned long)x;
// /*AFLA*/ 
// /*AFLA*/ 	return addr >= VMALLOC_START && addr < VMALLOC_END;
// /*AFLA*/ #else
// /*AFLA*/ 	return false;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ #ifdef CONFIG_MMU
// /*AFLA*/ extern int is_vmalloc_or_module_addr(const void *x);
// /*AFLA*/ #else
// /*AFLA*/ static inline int is_vmalloc_or_module_addr(const void *x)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern void kvfree(const void *addr);
// /*AFLA*/ 
// /*AFLA*/ static inline atomic_t *compound_mapcount_ptr(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return &page[1].compound_mapcount;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int compound_mapcount(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	VM_BUG_ON_PAGE(!PageCompound(page), page);
// /*AFLA*/ 	page = compound_head(page);
// /*AFLA*/ 	return atomic_read(compound_mapcount_ptr(page)) + 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The atomic page->_mapcount, starts from -1: so that transitions
// /*AFLA*/  * both from it and to it can be tracked, using atomic_inc_and_test
// /*AFLA*/  * and atomic_add_negative(-1).
// /*AFLA*/  */
// /*AFLA*/ static inline void page_mapcount_reset(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_set(&(page)->_mapcount, -1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ int __page_mapcount(struct page *page);
// /*AFLA*/ 
// /*AFLA*/ static inline int page_mapcount(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	VM_BUG_ON_PAGE(PageSlab(page), page);
// /*AFLA*/ 
// /*AFLA*/ 	if (unlikely(PageCompound(page)))
// /*AFLA*/ 		return __page_mapcount(page);
// /*AFLA*/ 	return atomic_read(&page->_mapcount) + 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_TRANSPARENT_HUGEPAGE
// /*AFLA*/ int total_mapcount(struct page *page);
// /*AFLA*/ int page_trans_huge_mapcount(struct page *page, int *total_mapcount);
// /*AFLA*/ #else
// /*AFLA*/ static inline int total_mapcount(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return page_mapcount(page);
// /*AFLA*/ }
// /*AFLA*/ static inline int page_trans_huge_mapcount(struct page *page,
// /*AFLA*/ 					   int *total_mapcount)
// /*AFLA*/ {
// /*AFLA*/ 	int mapcount = page_mapcount(page);
// /*AFLA*/ 	if (total_mapcount)
// /*AFLA*/ 		*total_mapcount = mapcount;
// /*AFLA*/ 	return mapcount;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline struct page *virt_to_head_page(const void *x)
// /*AFLA*/ {
// /*AFLA*/ 	struct page *page = virt_to_page(x);
// /*AFLA*/ 
// /*AFLA*/ 	return compound_head(page);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ void __put_page(struct page *page);
// /*AFLA*/ 
// /*AFLA*/ void put_pages_list(struct list_head *pages);
// /*AFLA*/ 
// /*AFLA*/ void split_page(struct page *page, unsigned int order);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Compound pages have a destructor function.  Provide a
// /*AFLA*/  * prototype for that function and accessor functions.
// /*AFLA*/  * These are _only_ valid on the head of a compound page.
// /*AFLA*/  */
// /*AFLA*/ typedef void compound_page_dtor(struct page *);
// /*AFLA*/ 
// /*AFLA*/ /* Keep the enum in sync with compound_page_dtors array in mm/page_alloc.c */
// /*AFLA*/ enum compound_dtor_id {
// /*AFLA*/ 	NULL_COMPOUND_DTOR,
// /*AFLA*/ 	COMPOUND_PAGE_DTOR,
// /*AFLA*/ #ifdef CONFIG_HUGETLB_PAGE
// /*AFLA*/ 	HUGETLB_PAGE_DTOR,
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_TRANSPARENT_HUGEPAGE
// /*AFLA*/ 	TRANSHUGE_PAGE_DTOR,
// /*AFLA*/ #endif
// /*AFLA*/ 	NR_COMPOUND_DTORS,
// /*AFLA*/ };
// /*AFLA*/ extern compound_page_dtor * const compound_page_dtors[];
// /*AFLA*/ 
// /*AFLA*/ static inline void set_compound_page_dtor(struct page *page,
// /*AFLA*/ 		enum compound_dtor_id compound_dtor)
// /*AFLA*/ {
// /*AFLA*/ 	VM_BUG_ON_PAGE(compound_dtor >= NR_COMPOUND_DTORS, page);
// /*AFLA*/ 	page[1].compound_dtor = compound_dtor;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline compound_page_dtor *get_compound_page_dtor(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	VM_BUG_ON_PAGE(page[1].compound_dtor >= NR_COMPOUND_DTORS, page);
// /*AFLA*/ 	return compound_page_dtors[page[1].compound_dtor];
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned int compound_order(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	if (!PageHead(page))
// /*AFLA*/ 		return 0;
// /*AFLA*/ 	return page[1].compound_order;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void set_compound_order(struct page *page, unsigned int order)
// /*AFLA*/ {
// /*AFLA*/ 	page[1].compound_order = order;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ void free_compound_page(struct page *page);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MMU
// /*AFLA*/ /*
// /*AFLA*/  * Do pte_mkwrite, but only if the vma says VM_WRITE.  We do this when
// /*AFLA*/  * servicing faults for write access.  In the normal case, do always want
// /*AFLA*/  * pte_mkwrite.  But get_user_pages can cause write faults for mappings
// /*AFLA*/  * that do not have writing enabled, when used by access_process_vm.
// /*AFLA*/  */
// /*AFLA*/ static inline pte_t maybe_mkwrite(pte_t pte, struct vm_area_struct *vma)
// /*AFLA*/ {
// /*AFLA*/ 	if (likely(vma->vm_flags & VM_WRITE))
// /*AFLA*/ 		pte = pte_mkwrite(pte);
// /*AFLA*/ 	return pte;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ int alloc_set_pte(struct fault_env *fe, struct mem_cgroup *memcg,
// /*AFLA*/ 		struct page *page);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Multiple processes may "see" the same page. E.g. for untouched
// /*AFLA*/  * mappings of /dev/null, all processes see the same page full of
// /*AFLA*/  * zeroes, and text pages of executables and shared libraries have
// /*AFLA*/  * only one copy in memory, at most, normally.
// /*AFLA*/  *
// /*AFLA*/  * For the non-reserved pages, page_count(page) denotes a reference count.
// /*AFLA*/  *   page_count() == 0 means the page is free. page->lru is then used for
// /*AFLA*/  *   freelist management in the buddy allocator.
// /*AFLA*/  *   page_count() > 0  means the page has been allocated.
// /*AFLA*/  *
// /*AFLA*/  * Pages are allocated by the slab allocator in order to provide memory
// /*AFLA*/  * to kmalloc and kmem_cache_alloc. In this case, the management of the
// /*AFLA*/  * page, and the fields in 'struct page' are the responsibility of mm/slab.c
// /*AFLA*/  * unless a particular usage is carefully commented. (the responsibility of
// /*AFLA*/  * freeing the kmalloc memory is the caller's, of course).
// /*AFLA*/  *
// /*AFLA*/  * A page may be used by anyone else who does a __get_free_page().
// /*AFLA*/  * In this case, page_count still tracks the references, and should only
// /*AFLA*/  * be used through the normal accessor functions. The top bits of page->flags
// /*AFLA*/  * and page->virtual store page management information, but all other fields
// /*AFLA*/  * are unused and could be used privately, carefully. The management of this
// /*AFLA*/  * page is the responsibility of the one who allocated it, and those who have
// /*AFLA*/  * subsequently been given references to it.
// /*AFLA*/  *
// /*AFLA*/  * The other pages (we may call them "pagecache pages") are completely
// /*AFLA*/  * managed by the Linux memory manager: I/O, buffers, swapping etc.
// /*AFLA*/  * The following discussion applies only to them.
// /*AFLA*/  *
// /*AFLA*/  * A pagecache page contains an opaque `private' member, which belongs to the
// /*AFLA*/  * page's address_space. Usually, this is the address of a circular list of
// /*AFLA*/  * the page's disk buffers. PG_private must be set to tell the VM to call
// /*AFLA*/  * into the filesystem to release these pages.
// /*AFLA*/  *
// /*AFLA*/  * A page may belong to an inode's memory mapping. In this case, page->mapping
// /*AFLA*/  * is the pointer to the inode, and page->index is the file offset of the page,
// /*AFLA*/  * in units of PAGE_SIZE.
// /*AFLA*/  *
// /*AFLA*/  * If pagecache pages are not associated with an inode, they are said to be
// /*AFLA*/  * anonymous pages. These may become associated with the swapcache, and in that
// /*AFLA*/  * case PG_swapcache is set, and page->private is an offset into the swapcache.
// /*AFLA*/  *
// /*AFLA*/  * In either case (swapcache or inode backed), the pagecache itself holds one
// /*AFLA*/  * reference to the page. Setting PG_private should also increment the
// /*AFLA*/  * refcount. The each user mapping also has a reference to the page.
// /*AFLA*/  *
// /*AFLA*/  * The pagecache pages are stored in a per-mapping radix tree, which is
// /*AFLA*/  * rooted at mapping->page_tree, and indexed by offset.
// /*AFLA*/  * Where 2.4 and early 2.6 kernels kept dirty/clean pages in per-address_space
// /*AFLA*/  * lists, we instead now tag pages as dirty/writeback in the radix tree.
// /*AFLA*/  *
// /*AFLA*/  * All pagecache pages may be subject to I/O:
// /*AFLA*/  * - inode pages may need to be read from disk,
// /*AFLA*/  * - inode pages which have been modified and are MAP_SHARED may need
// /*AFLA*/  *   to be written back to the inode on disk,
// /*AFLA*/  * - anonymous pages (including MAP_PRIVATE file mappings) which have been
// /*AFLA*/  *   modified may need to be swapped out to swap space and (later) to be read
// /*AFLA*/  *   back into memory.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The zone field is never updated after free_area_init_core()
// /*AFLA*/  * sets it, so none of the operations on it need to be atomic.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /* Page flags: | [SECTION] | [NODE] | ZONE | [LAST_CPUPID] | ... | FLAGS | */
// /*AFLA*/ #define SECTIONS_PGOFF		((sizeof(unsigned long)*8) - SECTIONS_WIDTH)
// /*AFLA*/ #define NODES_PGOFF		(SECTIONS_PGOFF - NODES_WIDTH)
// /*AFLA*/ #define ZONES_PGOFF		(NODES_PGOFF - ZONES_WIDTH)
// /*AFLA*/ #define LAST_CPUPID_PGOFF	(ZONES_PGOFF - LAST_CPUPID_WIDTH)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Define the bit shifts to access each section.  For non-existent
// /*AFLA*/  * sections we define the shift as 0; that plus a 0 mask ensures
// /*AFLA*/  * the compiler will optimise away reference to them.
// /*AFLA*/  */
// /*AFLA*/ #define SECTIONS_PGSHIFT	(SECTIONS_PGOFF * (SECTIONS_WIDTH != 0))
// /*AFLA*/ #define NODES_PGSHIFT		(NODES_PGOFF * (NODES_WIDTH != 0))
// /*AFLA*/ #define ZONES_PGSHIFT		(ZONES_PGOFF * (ZONES_WIDTH != 0))
// /*AFLA*/ #define LAST_CPUPID_PGSHIFT	(LAST_CPUPID_PGOFF * (LAST_CPUPID_WIDTH != 0))
// /*AFLA*/ 
// /*AFLA*/ /* NODE:ZONE or SECTION:ZONE is used to ID a zone for the buddy allocator */
// /*AFLA*/ #ifdef NODE_NOT_IN_PAGE_FLAGS
// /*AFLA*/ #define ZONEID_SHIFT		(SECTIONS_SHIFT + ZONES_SHIFT)
// /*AFLA*/ #define ZONEID_PGOFF		((SECTIONS_PGOFF < ZONES_PGOFF)? \
// /*AFLA*/ 						SECTIONS_PGOFF : ZONES_PGOFF)
// /*AFLA*/ #else
// /*AFLA*/ #define ZONEID_SHIFT		(NODES_SHIFT + ZONES_SHIFT)
// /*AFLA*/ #define ZONEID_PGOFF		((NODES_PGOFF < ZONES_PGOFF)? \
// /*AFLA*/ 						NODES_PGOFF : ZONES_PGOFF)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define ZONEID_PGSHIFT		(ZONEID_PGOFF * (ZONEID_SHIFT != 0))
// /*AFLA*/ 
// /*AFLA*/ #if SECTIONS_WIDTH+NODES_WIDTH+ZONES_WIDTH > BITS_PER_LONG - NR_PAGEFLAGS
// /*AFLA*/ #error SECTIONS_WIDTH+NODES_WIDTH+ZONES_WIDTH > BITS_PER_LONG - NR_PAGEFLAGS
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define ZONES_MASK		((1UL << ZONES_WIDTH) - 1)
// /*AFLA*/ #define NODES_MASK		((1UL << NODES_WIDTH) - 1)
// /*AFLA*/ #define SECTIONS_MASK		((1UL << SECTIONS_WIDTH) - 1)
// /*AFLA*/ #define LAST_CPUPID_MASK	((1UL << LAST_CPUPID_SHIFT) - 1)
// /*AFLA*/ #define ZONEID_MASK		((1UL << ZONEID_SHIFT) - 1)
// /*AFLA*/ 
// /*AFLA*/ static inline enum zone_type page_zonenum(const struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return (page->flags >> ZONES_PGSHIFT) & ZONES_MASK;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_ZONE_DEVICE
// /*AFLA*/ void get_zone_device_page(struct page *page);
// /*AFLA*/ void put_zone_device_page(struct page *page);
// /*AFLA*/ static inline bool is_zone_device_page(const struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return page_zonenum(page) == ZONE_DEVICE;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline void get_zone_device_page(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void put_zone_device_page(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline bool is_zone_device_page(const struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline void get_page(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	page = compound_head(page);
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Getting a normal page or the head of a compound page
// /*AFLA*/ 	 * requires to already have an elevated page->_refcount.
// /*AFLA*/ 	 */
// /*AFLA*/ 	VM_BUG_ON_PAGE(page_ref_count(page) <= 0, page);
// /*AFLA*/ 	page_ref_inc(page);
// /*AFLA*/ 
// /*AFLA*/ 	if (unlikely(is_zone_device_page(page)))
// /*AFLA*/ 		get_zone_device_page(page);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void put_page(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	page = compound_head(page);
// /*AFLA*/ 
// /*AFLA*/ 	if (put_page_testzero(page))
// /*AFLA*/ 		__put_page(page);
// /*AFLA*/ 
// /*AFLA*/ 	if (unlikely(is_zone_device_page(page)))
// /*AFLA*/ 		put_zone_device_page(page);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_SPARSEMEM) && !defined(CONFIG_SPARSEMEM_VMEMMAP)
// /*AFLA*/ #define SECTION_IN_PAGE_FLAGS
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The identification function is mainly used by the buddy allocator for
// /*AFLA*/  * determining if two pages could be buddies. We are not really identifying
// /*AFLA*/  * the zone since we could be using the section number id if we do not have
// /*AFLA*/  * node id available in page flags.
// /*AFLA*/  * We only guarantee that it will return the same value for two combinable
// /*AFLA*/  * pages in a zone.
// /*AFLA*/  */
// /*AFLA*/ static inline int page_zone_id(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return (page->flags >> ZONEID_PGSHIFT) & ZONEID_MASK;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int zone_to_nid(struct zone *zone)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ 	return zone->node;
// /*AFLA*/ #else
// /*AFLA*/ 	return 0;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef NODE_NOT_IN_PAGE_FLAGS
// /*AFLA*/ extern int page_to_nid(const struct page *page);
// /*AFLA*/ #else
// /*AFLA*/ static inline int page_to_nid(const struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return (page->flags >> NODES_PGSHIFT) & NODES_MASK;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NUMA_BALANCING
// /*AFLA*/ static inline int cpu_pid_to_cpupid(int cpu, int pid)
// /*AFLA*/ {
// /*AFLA*/ 	return ((cpu & LAST__CPU_MASK) << LAST__PID_SHIFT) | (pid & LAST__PID_MASK);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int cpupid_to_pid(int cpupid)
// /*AFLA*/ {
// /*AFLA*/ 	return cpupid & LAST__PID_MASK;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int cpupid_to_cpu(int cpupid)
// /*AFLA*/ {
// /*AFLA*/ 	return (cpupid >> LAST__PID_SHIFT) & LAST__CPU_MASK;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int cpupid_to_nid(int cpupid)
// /*AFLA*/ {
// /*AFLA*/ 	return cpu_to_node(cpupid_to_cpu(cpupid));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool cpupid_pid_unset(int cpupid)
// /*AFLA*/ {
// /*AFLA*/ 	return cpupid_to_pid(cpupid) == (-1 & LAST__PID_MASK);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool cpupid_cpu_unset(int cpupid)
// /*AFLA*/ {
// /*AFLA*/ 	return cpupid_to_cpu(cpupid) == (-1 & LAST__CPU_MASK);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool __cpupid_match_pid(pid_t task_pid, int cpupid)
// /*AFLA*/ {
// /*AFLA*/ 	return (task_pid & LAST__PID_MASK) == cpupid_to_pid(cpupid);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define cpupid_match_pid(task, cpupid) __cpupid_match_pid(task->pid, cpupid)
// /*AFLA*/ #ifdef LAST_CPUPID_NOT_IN_PAGE_FLAGS
// /*AFLA*/ static inline int page_cpupid_xchg_last(struct page *page, int cpupid)
// /*AFLA*/ {
// /*AFLA*/ 	return xchg(&page->_last_cpupid, cpupid & LAST_CPUPID_MASK);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int page_cpupid_last(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return page->_last_cpupid;
// /*AFLA*/ }
// /*AFLA*/ static inline void page_cpupid_reset_last(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	page->_last_cpupid = -1 & LAST_CPUPID_MASK;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline int page_cpupid_last(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return (page->flags >> LAST_CPUPID_PGSHIFT) & LAST_CPUPID_MASK;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int page_cpupid_xchg_last(struct page *page, int cpupid);
// /*AFLA*/ 
// /*AFLA*/ static inline void page_cpupid_reset_last(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	page->flags |= LAST_CPUPID_MASK << LAST_CPUPID_PGSHIFT;
// /*AFLA*/ }
// /*AFLA*/ #endif /* LAST_CPUPID_NOT_IN_PAGE_FLAGS */
// /*AFLA*/ #else /* !CONFIG_NUMA_BALANCING */
// /*AFLA*/ static inline int page_cpupid_xchg_last(struct page *page, int cpupid)
// /*AFLA*/ {
// /*AFLA*/ 	return page_to_nid(page); /* XXX */
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int page_cpupid_last(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return page_to_nid(page); /* XXX */
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int cpupid_to_nid(int cpupid)
// /*AFLA*/ {
// /*AFLA*/ 	return -1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int cpupid_to_pid(int cpupid)
// /*AFLA*/ {
// /*AFLA*/ 	return -1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int cpupid_to_cpu(int cpupid)
// /*AFLA*/ {
// /*AFLA*/ 	return -1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int cpu_pid_to_cpupid(int nid, int pid)
// /*AFLA*/ {
// /*AFLA*/ 	return -1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool cpupid_pid_unset(int cpupid)
// /*AFLA*/ {
// /*AFLA*/ 	return 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void page_cpupid_reset_last(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool cpupid_match_pid(struct task_struct *task, int cpupid)
// /*AFLA*/ {
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_NUMA_BALANCING */
// /*AFLA*/ 
// /*AFLA*/ static inline struct zone *page_zone(const struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return &NODE_DATA(page_to_nid(page))->node_zones[page_zonenum(page)];
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pg_data_t *page_pgdat(const struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return NODE_DATA(page_to_nid(page));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef SECTION_IN_PAGE_FLAGS
// /*AFLA*/ static inline void set_page_section(struct page *page, unsigned long section)
// /*AFLA*/ {
// /*AFLA*/ 	page->flags &= ~(SECTIONS_MASK << SECTIONS_PGSHIFT);
// /*AFLA*/ 	page->flags |= (section & SECTIONS_MASK) << SECTIONS_PGSHIFT;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long page_to_section(const struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return (page->flags >> SECTIONS_PGSHIFT) & SECTIONS_MASK;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline void set_page_zone(struct page *page, enum zone_type zone)
// /*AFLA*/ {
// /*AFLA*/ 	page->flags &= ~(ZONES_MASK << ZONES_PGSHIFT);
// /*AFLA*/ 	page->flags |= (zone & ZONES_MASK) << ZONES_PGSHIFT;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void set_page_node(struct page *page, unsigned long node)
// /*AFLA*/ {
// /*AFLA*/ 	page->flags &= ~(NODES_MASK << NODES_PGSHIFT);
// /*AFLA*/ 	page->flags |= (node & NODES_MASK) << NODES_PGSHIFT;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void set_page_links(struct page *page, enum zone_type zone,
// /*AFLA*/ 	unsigned long node, unsigned long pfn)
// /*AFLA*/ {
// /*AFLA*/ 	set_page_zone(page, zone);
// /*AFLA*/ 	set_page_node(page, node);
// /*AFLA*/ #ifdef SECTION_IN_PAGE_FLAGS
// /*AFLA*/ 	set_page_section(page, pfn_to_section_nr(pfn));
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MEMCG
// /*AFLA*/ static inline struct mem_cgroup *page_memcg(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return page->mem_cgroup;
// /*AFLA*/ }
// /*AFLA*/ static inline struct mem_cgroup *page_memcg_rcu(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	WARN_ON_ONCE(!rcu_read_lock_held());
// /*AFLA*/ 	return READ_ONCE(page->mem_cgroup);
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline struct mem_cgroup *page_memcg(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ static inline struct mem_cgroup *page_memcg_rcu(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	WARN_ON_ONCE(!rcu_read_lock_held());
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Some inline functions in vmstat.h depend on page_zone()
// /*AFLA*/  */
// /*AFLA*/ #include <linux/vmstat.h>
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void *lowmem_page_address(const struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return page_to_virt(page);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_HIGHMEM) && !defined(WANT_PAGE_VIRTUAL)
// /*AFLA*/ #define HASHED_PAGE_VIRTUAL
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if defined(WANT_PAGE_VIRTUAL)
// /*AFLA*/ static inline void *page_address(const struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return page->virtual;
// /*AFLA*/ }
// /*AFLA*/ static inline void set_page_address(struct page *page, void *address)
// /*AFLA*/ {
// /*AFLA*/ 	page->virtual = address;
// /*AFLA*/ }
// /*AFLA*/ #define page_address_init()  do { } while(0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if defined(HASHED_PAGE_VIRTUAL)
// /*AFLA*/ void *page_address(const struct page *page);
// /*AFLA*/ void set_page_address(struct page *page, void *virtual);
// /*AFLA*/ void page_address_init(void);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if !defined(HASHED_PAGE_VIRTUAL) && !defined(WANT_PAGE_VIRTUAL)
// /*AFLA*/ #define page_address(page) lowmem_page_address(page)
// /*AFLA*/ #define set_page_address(page, address)  do { } while(0)
// /*AFLA*/ #define page_address_init()  do { } while(0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern void *page_rmapping(struct page *page);
// /*AFLA*/ extern struct anon_vma *page_anon_vma(struct page *page);
// /*AFLA*/ extern struct address_space *page_mapping(struct page *page);
// /*AFLA*/ 
// /*AFLA*/ extern struct address_space *__page_file_mapping(struct page *);
// /*AFLA*/ 
// /*AFLA*/ static inline
// /*AFLA*/ struct address_space *page_file_mapping(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(PageSwapCache(page)))
// /*AFLA*/ 		return __page_file_mapping(page);
// /*AFLA*/ 
// /*AFLA*/ 	return page->mapping;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern pgoff_t __page_file_index(struct page *page);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Return the pagecache index of the passed page.  Regular pagecache pages
// /*AFLA*/  * use ->index whereas swapcache pages use swp_offset(->private)
// /*AFLA*/  */
// /*AFLA*/ static inline pgoff_t page_index(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(PageSwapCache(page)))
// /*AFLA*/ 		return __page_file_index(page);
// /*AFLA*/ 	return page->index;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ bool page_mapped(struct page *page);
// /*AFLA*/ struct address_space *page_mapping(struct page *page);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Return true only if the page has been allocated with
// /*AFLA*/  * ALLOC_NO_WATERMARKS and the low watermark was not
// /*AFLA*/  * met implying that the system is under some pressure.
// /*AFLA*/  */
// /*AFLA*/ static inline bool page_is_pfmemalloc(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Page index cannot be this large so this must be
// /*AFLA*/ 	 * a pfmemalloc page.
// /*AFLA*/ 	 */
// /*AFLA*/ 	return page->index == -1UL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Only to be called by the page allocator on a freshly allocated
// /*AFLA*/  * page.
// /*AFLA*/  */
// /*AFLA*/ static inline void set_page_pfmemalloc(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	page->index = -1UL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void clear_page_pfmemalloc(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	page->index = 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Different kinds of faults, as returned by handle_mm_fault().
// /*AFLA*/  * Used to decide whether a process gets delivered SIGBUS or
// /*AFLA*/  * just gets major/minor fault counters bumped up.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define VM_FAULT_OOM	0x0001
// /*AFLA*/ #define VM_FAULT_SIGBUS	0x0002
// /*AFLA*/ #define VM_FAULT_MAJOR	0x0004
// /*AFLA*/ #define VM_FAULT_WRITE	0x0008	/* Special case for get_user_pages */
// /*AFLA*/ #define VM_FAULT_HWPOISON 0x0010	/* Hit poisoned small page */
// /*AFLA*/ #define VM_FAULT_HWPOISON_LARGE 0x0020  /* Hit poisoned large page. Index encoded in upper bits */
// /*AFLA*/ #define VM_FAULT_SIGSEGV 0x0040
// /*AFLA*/ 
// /*AFLA*/ #define VM_FAULT_NOPAGE	0x0100	/* ->fault installed the pte, not return page */
// /*AFLA*/ #define VM_FAULT_LOCKED	0x0200	/* ->fault locked the returned page */
// /*AFLA*/ #define VM_FAULT_RETRY	0x0400	/* ->fault blocked, must retry */
// /*AFLA*/ #define VM_FAULT_FALLBACK 0x0800	/* huge page fault failed, fall back to small */
// /*AFLA*/ #define VM_FAULT_DAX_LOCKED 0x1000	/* ->fault has locked DAX entry */
// /*AFLA*/ 
// /*AFLA*/ #define VM_FAULT_HWPOISON_LARGE_MASK 0xf000 /* encodes hpage index for large hwpoison */
// /*AFLA*/ 
// /*AFLA*/ #define VM_FAULT_ERROR	(VM_FAULT_OOM | VM_FAULT_SIGBUS | VM_FAULT_SIGSEGV | \
// /*AFLA*/ 			 VM_FAULT_HWPOISON | VM_FAULT_HWPOISON_LARGE | \
// /*AFLA*/ 			 VM_FAULT_FALLBACK)
// /*AFLA*/ 
// /*AFLA*/ /* Encode hstate index for a hwpoisoned large page */
// /*AFLA*/ #define VM_FAULT_SET_HINDEX(x) ((x) << 12)
// /*AFLA*/ #define VM_FAULT_GET_HINDEX(x) (((x) >> 12) & 0xf)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Can be called by the pagefault handler when it gets a VM_FAULT_OOM.
// /*AFLA*/  */
// /*AFLA*/ extern void pagefault_out_of_memory(void);
// /*AFLA*/ 
// /*AFLA*/ #define offset_in_page(p)	((unsigned long)(p) & ~PAGE_MASK)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Flags passed to show_mem() and show_free_areas() to suppress output in
// /*AFLA*/  * various contexts.
// /*AFLA*/  */
// /*AFLA*/ #define SHOW_MEM_FILTER_NODES		(0x0001u)	/* disallowed nodes */
// /*AFLA*/ 
// /*AFLA*/ extern void show_free_areas(unsigned int flags);
// /*AFLA*/ extern bool skip_free_areas_node(unsigned int flags, int nid);
// /*AFLA*/ 
// /*AFLA*/ int shmem_zero_setup(struct vm_area_struct *);
// /*AFLA*/ #ifdef CONFIG_SHMEM
// /*AFLA*/ bool shmem_mapping(struct address_space *mapping);
// /*AFLA*/ #else
// /*AFLA*/ static inline bool shmem_mapping(struct address_space *mapping)
// /*AFLA*/ {
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern bool can_do_mlock(void);
// /*AFLA*/ extern int user_shm_lock(size_t, struct user_struct *);
// /*AFLA*/ extern void user_shm_unlock(size_t, struct user_struct *);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Parameter block passed down to zap_pte_range in exceptional cases.
// /*AFLA*/  */
// /*AFLA*/ struct zap_details {
// /*AFLA*/ 	struct address_space *check_mapping;	/* Check page->mapping if set */
// /*AFLA*/ 	pgoff_t	first_index;			/* Lowest page->index to unmap */
// /*AFLA*/ 	pgoff_t last_index;			/* Highest page->index to unmap */
// /*AFLA*/ 	bool ignore_dirty;			/* Ignore dirty pages */
// /*AFLA*/ 	bool check_swap_entries;		/* Check also swap entries */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct page *vm_normal_page(struct vm_area_struct *vma, unsigned long addr,
// /*AFLA*/ 		pte_t pte);
// /*AFLA*/ struct page *vm_normal_page_pmd(struct vm_area_struct *vma, unsigned long addr,
// /*AFLA*/ 				pmd_t pmd);
// /*AFLA*/ 
// /*AFLA*/ int zap_vma_ptes(struct vm_area_struct *vma, unsigned long address,
// /*AFLA*/ 		unsigned long size);
// /*AFLA*/ void zap_page_range(struct vm_area_struct *vma, unsigned long address,
// /*AFLA*/ 		unsigned long size, struct zap_details *);
// /*AFLA*/ void unmap_vmas(struct mmu_gather *tlb, struct vm_area_struct *start_vma,
// /*AFLA*/ 		unsigned long start, unsigned long end);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * mm_walk - callbacks for walk_page_range
// /*AFLA*/  * @pmd_entry: if set, called for each non-empty PMD (3rd-level) entry
// /*AFLA*/  *	       this handler is required to be able to handle
// /*AFLA*/  *	       pmd_trans_huge() pmds.  They may simply choose to
// /*AFLA*/  *	       split_huge_page() instead of handling it explicitly.
// /*AFLA*/  * @pte_entry: if set, called for each non-empty PTE (4th-level) entry
// /*AFLA*/  * @pte_hole: if set, called for each hole at all levels
// /*AFLA*/  * @hugetlb_entry: if set, called for each hugetlb entry
// /*AFLA*/  * @test_walk: caller specific callback function to determine whether
// /*AFLA*/  *             we walk over the current vma or not. Returning 0
// /*AFLA*/  *             value means "do page table walk over the current vma,"
// /*AFLA*/  *             and a negative one means "abort current page table walk
// /*AFLA*/  *             right now." 1 means "skip the current vma."
// /*AFLA*/  * @mm:        mm_struct representing the target process of page table walk
// /*AFLA*/  * @vma:       vma currently walked (NULL if walking outside vmas)
// /*AFLA*/  * @private:   private data for callbacks' usage
// /*AFLA*/  *
// /*AFLA*/  * (see the comment on walk_page_range() for more details)
// /*AFLA*/  */
// /*AFLA*/ struct mm_walk {
// /*AFLA*/ 	int (*pmd_entry)(pmd_t *pmd, unsigned long addr,
// /*AFLA*/ 			 unsigned long next, struct mm_walk *walk);
// /*AFLA*/ 	int (*pte_entry)(pte_t *pte, unsigned long addr,
// /*AFLA*/ 			 unsigned long next, struct mm_walk *walk);
// /*AFLA*/ 	int (*pte_hole)(unsigned long addr, unsigned long next,
// /*AFLA*/ 			struct mm_walk *walk);
// /*AFLA*/ 	int (*hugetlb_entry)(pte_t *pte, unsigned long hmask,
// /*AFLA*/ 			     unsigned long addr, unsigned long next,
// /*AFLA*/ 			     struct mm_walk *walk);
// /*AFLA*/ 	int (*test_walk)(unsigned long addr, unsigned long next,
// /*AFLA*/ 			struct mm_walk *walk);
// /*AFLA*/ 	struct mm_struct *mm;
// /*AFLA*/ 	struct vm_area_struct *vma;
// /*AFLA*/ 	void *private;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ int walk_page_range(unsigned long addr, unsigned long end,
// /*AFLA*/ 		struct mm_walk *walk);
// /*AFLA*/ int walk_page_vma(struct vm_area_struct *vma, struct mm_walk *walk);
// /*AFLA*/ void free_pgd_range(struct mmu_gather *tlb, unsigned long addr,
// /*AFLA*/ 		unsigned long end, unsigned long floor, unsigned long ceiling);
// /*AFLA*/ int copy_page_range(struct mm_struct *dst, struct mm_struct *src,
// /*AFLA*/ 			struct vm_area_struct *vma);
// /*AFLA*/ void unmap_mapping_range(struct address_space *mapping,
// /*AFLA*/ 		loff_t const holebegin, loff_t const holelen, int even_cows);
// /*AFLA*/ int follow_pfn(struct vm_area_struct *vma, unsigned long address,
// /*AFLA*/ 	unsigned long *pfn);
// /*AFLA*/ int follow_phys(struct vm_area_struct *vma, unsigned long address,
// /*AFLA*/ 		unsigned int flags, unsigned long *prot, resource_size_t *phys);
// /*AFLA*/ int generic_access_phys(struct vm_area_struct *vma, unsigned long addr,
// /*AFLA*/ 			void *buf, int len, int write);
// /*AFLA*/ 
// /*AFLA*/ static inline void unmap_shared_mapping_range(struct address_space *mapping,
// /*AFLA*/ 		loff_t const holebegin, loff_t const holelen)
// /*AFLA*/ {
// /*AFLA*/ 	unmap_mapping_range(mapping, holebegin, holelen, 0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void truncate_pagecache(struct inode *inode, loff_t new);
// /*AFLA*/ extern void truncate_setsize(struct inode *inode, loff_t newsize);
// /*AFLA*/ void pagecache_isize_extended(struct inode *inode, loff_t from, loff_t to);
// /*AFLA*/ void truncate_pagecache_range(struct inode *inode, loff_t offset, loff_t end);
// /*AFLA*/ int truncate_inode_page(struct address_space *mapping, struct page *page);
// /*AFLA*/ int generic_error_remove_page(struct address_space *mapping, struct page *page);
// /*AFLA*/ int invalidate_inode_page(struct page *page);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MMU
// /*AFLA*/ extern int handle_mm_fault(struct vm_area_struct *vma, unsigned long address,
// /*AFLA*/ 		unsigned int flags);
// /*AFLA*/ extern int fixup_user_fault(struct task_struct *tsk, struct mm_struct *mm,
// /*AFLA*/ 			    unsigned long address, unsigned int fault_flags,
// /*AFLA*/ 			    bool *unlocked);
// /*AFLA*/ #else
// /*AFLA*/ static inline int handle_mm_fault(struct vm_area_struct *vma,
// /*AFLA*/ 		unsigned long address, unsigned int flags)
// /*AFLA*/ {
// /*AFLA*/ 	/* should never happen if there's no MMU */
// /*AFLA*/ 	BUG();
// /*AFLA*/ 	return VM_FAULT_SIGBUS;
// /*AFLA*/ }
// /*AFLA*/ static inline int fixup_user_fault(struct task_struct *tsk,
// /*AFLA*/ 		struct mm_struct *mm, unsigned long address,
// /*AFLA*/ 		unsigned int fault_flags, bool *unlocked)
// /*AFLA*/ {
// /*AFLA*/ 	/* should never happen if there's no MMU */
// /*AFLA*/ 	BUG();
// /*AFLA*/ 	return -EFAULT;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern int access_process_vm(struct task_struct *tsk, unsigned long addr, void *buf, int len,
// /*AFLA*/ 		unsigned int gup_flags);
// /*AFLA*/ extern int access_remote_vm(struct mm_struct *mm, unsigned long addr,
// /*AFLA*/ 		void *buf, int len, unsigned int gup_flags);
// /*AFLA*/ 
// /*AFLA*/ long get_user_pages_remote(struct task_struct *tsk, struct mm_struct *mm,
// /*AFLA*/ 			    unsigned long start, unsigned long nr_pages,
// /*AFLA*/ 			    unsigned int gup_flags, struct page **pages,
// /*AFLA*/ 			    struct vm_area_struct **vmas);
// /*AFLA*/ long get_user_pages(unsigned long start, unsigned long nr_pages,
// /*AFLA*/ 			    unsigned int gup_flags, struct page **pages,
// /*AFLA*/ 			    struct vm_area_struct **vmas);
// /*AFLA*/ long get_user_pages_locked(unsigned long start, unsigned long nr_pages,
// /*AFLA*/ 		    unsigned int gup_flags, struct page **pages, int *locked);
// /*AFLA*/ long __get_user_pages_unlocked(struct task_struct *tsk, struct mm_struct *mm,
// /*AFLA*/ 			       unsigned long start, unsigned long nr_pages,
// /*AFLA*/ 			       struct page **pages, unsigned int gup_flags);
// /*AFLA*/ long get_user_pages_unlocked(unsigned long start, unsigned long nr_pages,
// /*AFLA*/ 		    struct page **pages, unsigned int gup_flags);
// /*AFLA*/ int get_user_pages_fast(unsigned long start, int nr_pages, int write,
// /*AFLA*/ 			struct page **pages);
// /*AFLA*/ 
// /*AFLA*/ /* Container for pinned pfns / pages */
// /*AFLA*/ struct frame_vector {
// /*AFLA*/ 	unsigned int nr_allocated;	/* Number of frames we have space for */
// /*AFLA*/ 	unsigned int nr_frames;	/* Number of frames stored in ptrs array */
// /*AFLA*/ 	bool got_ref;		/* Did we pin pages by getting page ref? */
// /*AFLA*/ 	bool is_pfns;		/* Does array contain pages or pfns? */
// /*AFLA*/ 	void *ptrs[0];		/* Array of pinned pfns / pages. Use
// /*AFLA*/ 				 * pfns_vector_pages() or pfns_vector_pfns()
// /*AFLA*/ 				 * for access */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct frame_vector *frame_vector_create(unsigned int nr_frames);
// /*AFLA*/ void frame_vector_destroy(struct frame_vector *vec);
// /*AFLA*/ int get_vaddr_frames(unsigned long start, unsigned int nr_pfns,
// /*AFLA*/ 		     unsigned int gup_flags, struct frame_vector *vec);
// /*AFLA*/ void put_vaddr_frames(struct frame_vector *vec);
// /*AFLA*/ int frame_vector_to_pages(struct frame_vector *vec);
// /*AFLA*/ void frame_vector_to_pfns(struct frame_vector *vec);
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned int frame_vector_count(struct frame_vector *vec)
// /*AFLA*/ {
// /*AFLA*/ 	return vec->nr_frames;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct page **frame_vector_pages(struct frame_vector *vec)
// /*AFLA*/ {
// /*AFLA*/ 	if (vec->is_pfns) {
// /*AFLA*/ 		int err = frame_vector_to_pages(vec);
// /*AFLA*/ 
// /*AFLA*/ 		if (err)
// /*AFLA*/ 			return ERR_PTR(err);
// /*AFLA*/ 	}
// /*AFLA*/ 	return (struct page **)(vec->ptrs);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long *frame_vector_pfns(struct frame_vector *vec)
// /*AFLA*/ {
// /*AFLA*/ 	if (!vec->is_pfns)
// /*AFLA*/ 		frame_vector_to_pfns(vec);
// /*AFLA*/ 	return (unsigned long *)(vec->ptrs);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ struct kvec;
// /*AFLA*/ int get_kernel_pages(const struct kvec *iov, int nr_pages, int write,
// /*AFLA*/ 			struct page **pages);
// /*AFLA*/ int get_kernel_page(unsigned long start, int write, struct page **pages);
// /*AFLA*/ struct page *get_dump_page(unsigned long addr);
// /*AFLA*/ 
// /*AFLA*/ extern int try_to_release_page(struct page * page, gfp_t gfp_mask);
// /*AFLA*/ extern void do_invalidatepage(struct page *page, unsigned int offset,
// /*AFLA*/ 			      unsigned int length);
// /*AFLA*/ 
// /*AFLA*/ int __set_page_dirty_nobuffers(struct page *page);
// /*AFLA*/ int __set_page_dirty_no_writeback(struct page *page);
// /*AFLA*/ int redirty_page_for_writepage(struct writeback_control *wbc,
// /*AFLA*/ 				struct page *page);
// /*AFLA*/ void account_page_dirtied(struct page *page, struct address_space *mapping);
// /*AFLA*/ void account_page_cleaned(struct page *page, struct address_space *mapping,
// /*AFLA*/ 			  struct bdi_writeback *wb);
// /*AFLA*/ int set_page_dirty(struct page *page);
// /*AFLA*/ int set_page_dirty_lock(struct page *page);
// /*AFLA*/ void cancel_dirty_page(struct page *page);
// /*AFLA*/ int clear_page_dirty_for_io(struct page *page);
// /*AFLA*/ 
// /*AFLA*/ int get_cmdline(struct task_struct *task, char *buffer, int buflen);
// /*AFLA*/ 
// /*AFLA*/ /* Is the vma a continuation of the stack vma above it? */
// /*AFLA*/ static inline int vma_growsdown(struct vm_area_struct *vma, unsigned long addr)
// /*AFLA*/ {
// /*AFLA*/ 	return vma && (vma->vm_end == addr) && (vma->vm_flags & VM_GROWSDOWN);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool vma_is_anonymous(struct vm_area_struct *vma)
// /*AFLA*/ {
// /*AFLA*/ 	return !vma->vm_ops;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int stack_guard_page_start(struct vm_area_struct *vma,
// /*AFLA*/ 					     unsigned long addr)
// /*AFLA*/ {
// /*AFLA*/ 	return (vma->vm_flags & VM_GROWSDOWN) &&
// /*AFLA*/ 		(vma->vm_start == addr) &&
// /*AFLA*/ 		!vma_growsdown(vma->vm_prev, addr);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Is the vma a continuation of the stack vma below it? */
// /*AFLA*/ static inline int vma_growsup(struct vm_area_struct *vma, unsigned long addr)
// /*AFLA*/ {
// /*AFLA*/ 	return vma && (vma->vm_start == addr) && (vma->vm_flags & VM_GROWSUP);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int stack_guard_page_end(struct vm_area_struct *vma,
// /*AFLA*/ 					   unsigned long addr)
// /*AFLA*/ {
// /*AFLA*/ 	return (vma->vm_flags & VM_GROWSUP) &&
// /*AFLA*/ 		(vma->vm_end == addr) &&
// /*AFLA*/ 		!vma_growsup(vma->vm_next, addr);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ int vma_is_stack_for_current(struct vm_area_struct *vma);
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long move_page_tables(struct vm_area_struct *vma,
// /*AFLA*/ 		unsigned long old_addr, struct vm_area_struct *new_vma,
// /*AFLA*/ 		unsigned long new_addr, unsigned long len,
// /*AFLA*/ 		bool need_rmap_locks);
// /*AFLA*/ extern unsigned long change_protection(struct vm_area_struct *vma, unsigned long start,
// /*AFLA*/ 			      unsigned long end, pgprot_t newprot,
// /*AFLA*/ 			      int dirty_accountable, int prot_numa);
// /*AFLA*/ extern int mprotect_fixup(struct vm_area_struct *vma,
// /*AFLA*/ 			  struct vm_area_struct **pprev, unsigned long start,
// /*AFLA*/ 			  unsigned long end, unsigned long newflags);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * doesn't attempt to fault and will return short.
// /*AFLA*/  */
// /*AFLA*/ int __get_user_pages_fast(unsigned long start, int nr_pages, int write,
// /*AFLA*/ 			  struct page **pages);
// /*AFLA*/ /*
// /*AFLA*/  * per-process(per-mm_struct) statistics.
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned long get_mm_counter(struct mm_struct *mm, int member)
// /*AFLA*/ {
// /*AFLA*/ 	long val = atomic_long_read(&mm->rss_stat.count[member]);
// /*AFLA*/ 
// /*AFLA*/ #ifdef SPLIT_RSS_COUNTING
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * counter is updated in asynchronous manner and may go to minus.
// /*AFLA*/ 	 * But it's never be expected number for users.
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (val < 0)
// /*AFLA*/ 		val = 0;
// /*AFLA*/ #endif
// /*AFLA*/ 	return (unsigned long)val;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void add_mm_counter(struct mm_struct *mm, int member, long value)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_long_add(value, &mm->rss_stat.count[member]);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void inc_mm_counter(struct mm_struct *mm, int member)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_long_inc(&mm->rss_stat.count[member]);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void dec_mm_counter(struct mm_struct *mm, int member)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_long_dec(&mm->rss_stat.count[member]);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Optimized variant when page is already known not to be PageAnon */
// /*AFLA*/ static inline int mm_counter_file(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	if (PageSwapBacked(page))
// /*AFLA*/ 		return MM_SHMEMPAGES;
// /*AFLA*/ 	return MM_FILEPAGES;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int mm_counter(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	if (PageAnon(page))
// /*AFLA*/ 		return MM_ANONPAGES;
// /*AFLA*/ 	return mm_counter_file(page);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long get_mm_rss(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	return get_mm_counter(mm, MM_FILEPAGES) +
// /*AFLA*/ 		get_mm_counter(mm, MM_ANONPAGES) +
// /*AFLA*/ 		get_mm_counter(mm, MM_SHMEMPAGES);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long get_mm_hiwater_rss(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	return max(mm->hiwater_rss, get_mm_rss(mm));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long get_mm_hiwater_vm(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	return max(mm->hiwater_vm, mm->total_vm);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void update_hiwater_rss(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long _rss = get_mm_rss(mm);
// /*AFLA*/ 
// /*AFLA*/ 	if ((mm)->hiwater_rss < _rss)
// /*AFLA*/ 		(mm)->hiwater_rss = _rss;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void update_hiwater_vm(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	if (mm->hiwater_vm < mm->total_vm)
// /*AFLA*/ 		mm->hiwater_vm = mm->total_vm;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void reset_mm_hiwater_rss(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	mm->hiwater_rss = get_mm_rss(mm);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void setmax_mm_hiwater_rss(unsigned long *maxrss,
// /*AFLA*/ 					 struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long hiwater_rss = get_mm_hiwater_rss(mm);
// /*AFLA*/ 
// /*AFLA*/ 	if (*maxrss < hiwater_rss)
// /*AFLA*/ 		*maxrss = hiwater_rss;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #if defined(SPLIT_RSS_COUNTING)
// /*AFLA*/ void sync_mm_rss(struct mm_struct *mm);
// /*AFLA*/ #else
// /*AFLA*/ static inline void sync_mm_rss(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_PTE_DEVMAP
// /*AFLA*/ static inline int pte_devmap(pte_t pte)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ int vma_wants_writenotify(struct vm_area_struct *vma, pgprot_t vm_page_prot);
// /*AFLA*/ 
// /*AFLA*/ extern pte_t *__get_locked_pte(struct mm_struct *mm, unsigned long addr,
// /*AFLA*/ 			       spinlock_t **ptl);
// /*AFLA*/ static inline pte_t *get_locked_pte(struct mm_struct *mm, unsigned long addr,
// /*AFLA*/ 				    spinlock_t **ptl)
// /*AFLA*/ {
// /*AFLA*/ 	pte_t *ptep;
// /*AFLA*/ 	__cond_lock(*ptl, ptep = __get_locked_pte(mm, addr, ptl));
// /*AFLA*/ 	return ptep;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef __PAGETABLE_PUD_FOLDED
// /*AFLA*/ static inline int __pud_alloc(struct mm_struct *mm, pgd_t *pgd,
// /*AFLA*/ 						unsigned long address)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ int __pud_alloc(struct mm_struct *mm, pgd_t *pgd, unsigned long address);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if defined(__PAGETABLE_PMD_FOLDED) || !defined(CONFIG_MMU)
// /*AFLA*/ static inline int __pmd_alloc(struct mm_struct *mm, pud_t *pud,
// /*AFLA*/ 						unsigned long address)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void mm_nr_pmds_init(struct mm_struct *mm) {}
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long mm_nr_pmds(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void mm_inc_nr_pmds(struct mm_struct *mm) {}
// /*AFLA*/ static inline void mm_dec_nr_pmds(struct mm_struct *mm) {}
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ int __pmd_alloc(struct mm_struct *mm, pud_t *pud, unsigned long address);
// /*AFLA*/ 
// /*AFLA*/ static inline void mm_nr_pmds_init(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_long_set(&mm->nr_pmds, 0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long mm_nr_pmds(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic_long_read(&mm->nr_pmds);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void mm_inc_nr_pmds(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_long_inc(&mm->nr_pmds);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void mm_dec_nr_pmds(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_long_dec(&mm->nr_pmds);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ int __pte_alloc(struct mm_struct *mm, pmd_t *pmd, unsigned long address);
// /*AFLA*/ int __pte_alloc_kernel(pmd_t *pmd, unsigned long address);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The following ifdef needed to get the 4level-fixup.h header to work.
// /*AFLA*/  * Remove it when 4level-fixup.h has been removed.
// /*AFLA*/  */
// /*AFLA*/ #if defined(CONFIG_MMU) && !defined(__ARCH_HAS_4LEVEL_HACK)
// /*AFLA*/ static inline pud_t *pud_alloc(struct mm_struct *mm, pgd_t *pgd, unsigned long address)
// /*AFLA*/ {
// /*AFLA*/ 	return (unlikely(pgd_none(*pgd)) && __pud_alloc(mm, pgd, address))?
// /*AFLA*/ 		NULL: pud_offset(pgd, address);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline pmd_t *pmd_alloc(struct mm_struct *mm, pud_t *pud, unsigned long address)
// /*AFLA*/ {
// /*AFLA*/ 	return (unlikely(pud_none(*pud)) && __pmd_alloc(mm, pud, address))?
// /*AFLA*/ 		NULL: pmd_offset(pud, address);
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_MMU && !__ARCH_HAS_4LEVEL_HACK */
// /*AFLA*/ 
// /*AFLA*/ #if USE_SPLIT_PTE_PTLOCKS
// /*AFLA*/ #if ALLOC_SPLIT_PTLOCKS
// /*AFLA*/ void __init ptlock_cache_init(void);
// /*AFLA*/ extern bool ptlock_alloc(struct page *page);
// /*AFLA*/ extern void ptlock_free(struct page *page);
// /*AFLA*/ 
// /*AFLA*/ static inline spinlock_t *ptlock_ptr(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return page->ptl;
// /*AFLA*/ }
// /*AFLA*/ #else /* ALLOC_SPLIT_PTLOCKS */
// /*AFLA*/ static inline void ptlock_cache_init(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool ptlock_alloc(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void ptlock_free(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline spinlock_t *ptlock_ptr(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return &page->ptl;
// /*AFLA*/ }
// /*AFLA*/ #endif /* ALLOC_SPLIT_PTLOCKS */
// /*AFLA*/ 
// /*AFLA*/ static inline spinlock_t *pte_lockptr(struct mm_struct *mm, pmd_t *pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return ptlock_ptr(pmd_page(*pmd));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool ptlock_init(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * prep_new_page() initialize page->private (and therefore page->ptl)
// /*AFLA*/ 	 * with 0. Make sure nobody took it in use in between.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * It can happen if arch try to use slab for page table allocation:
// /*AFLA*/ 	 * slab code uses page->slab_cache, which share storage with page->ptl.
// /*AFLA*/ 	 */
// /*AFLA*/ 	VM_BUG_ON_PAGE(*(unsigned long *)&page->ptl, page);
// /*AFLA*/ 	if (!ptlock_alloc(page))
// /*AFLA*/ 		return false;
// /*AFLA*/ 	spin_lock_init(ptlock_ptr(page));
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Reset page->mapping so free_pages_check won't complain. */
// /*AFLA*/ static inline void pte_lock_deinit(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	page->mapping = NULL;
// /*AFLA*/ 	ptlock_free(page);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else	/* !USE_SPLIT_PTE_PTLOCKS */
// /*AFLA*/ /*
// /*AFLA*/  * We use mm->page_table_lock to guard all pagetable pages of the mm.
// /*AFLA*/  */
// /*AFLA*/ static inline spinlock_t *pte_lockptr(struct mm_struct *mm, pmd_t *pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return &mm->page_table_lock;
// /*AFLA*/ }
// /*AFLA*/ static inline void ptlock_cache_init(void) {}
// /*AFLA*/ static inline bool ptlock_init(struct page *page) { return true; }
// /*AFLA*/ static inline void pte_lock_deinit(struct page *page) {}
// /*AFLA*/ #endif /* USE_SPLIT_PTE_PTLOCKS */
// /*AFLA*/ 
// /*AFLA*/ static inline void pgtable_init(void)
// /*AFLA*/ {
// /*AFLA*/ 	ptlock_cache_init();
// /*AFLA*/ 	pgtable_cache_init();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool pgtable_page_ctor(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	if (!ptlock_init(page))
// /*AFLA*/ 		return false;
// /*AFLA*/ 	inc_zone_page_state(page, NR_PAGETABLE);
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void pgtable_page_dtor(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	pte_lock_deinit(page);
// /*AFLA*/ 	dec_zone_page_state(page, NR_PAGETABLE);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define pte_offset_map_lock(mm, pmd, address, ptlp)	\
// /*AFLA*/ ({							\
// /*AFLA*/ 	spinlock_t *__ptl = pte_lockptr(mm, pmd);	\
// /*AFLA*/ 	pte_t *__pte = pte_offset_map(pmd, address);	\
// /*AFLA*/ 	*(ptlp) = __ptl;				\
// /*AFLA*/ 	spin_lock(__ptl);				\
// /*AFLA*/ 	__pte;						\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define pte_unmap_unlock(pte, ptl)	do {		\
// /*AFLA*/ 	spin_unlock(ptl);				\
// /*AFLA*/ 	pte_unmap(pte);					\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define pte_alloc(mm, pmd, address)			\
// /*AFLA*/ 	(unlikely(pmd_none(*(pmd))) && __pte_alloc(mm, pmd, address))
// /*AFLA*/ 
// /*AFLA*/ #define pte_alloc_map(mm, pmd, address)			\
// /*AFLA*/ 	(pte_alloc(mm, pmd, address) ? NULL : pte_offset_map(pmd, address))
// /*AFLA*/ 
// /*AFLA*/ #define pte_alloc_map_lock(mm, pmd, address, ptlp)	\
// /*AFLA*/ 	(pte_alloc(mm, pmd, address) ?			\
// /*AFLA*/ 		 NULL : pte_offset_map_lock(mm, pmd, address, ptlp))
// /*AFLA*/ 
// /*AFLA*/ #define pte_alloc_kernel(pmd, address)			\
// /*AFLA*/ 	((unlikely(pmd_none(*(pmd))) && __pte_alloc_kernel(pmd, address))? \
// /*AFLA*/ 		NULL: pte_offset_kernel(pmd, address))
// /*AFLA*/ 
// /*AFLA*/ #if USE_SPLIT_PMD_PTLOCKS
// /*AFLA*/ 
// /*AFLA*/ static struct page *pmd_to_page(pmd_t *pmd)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long mask = ~(PTRS_PER_PMD * sizeof(pmd_t) - 1);
// /*AFLA*/ 	return virt_to_page((void *)((unsigned long) pmd & mask));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline spinlock_t *pmd_lockptr(struct mm_struct *mm, pmd_t *pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return ptlock_ptr(pmd_to_page(pmd));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool pgtable_pmd_page_ctor(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_TRANSPARENT_HUGEPAGE
// /*AFLA*/ 	page->pmd_huge_pte = NULL;
// /*AFLA*/ #endif
// /*AFLA*/ 	return ptlock_init(page);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void pgtable_pmd_page_dtor(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_TRANSPARENT_HUGEPAGE
// /*AFLA*/ 	VM_BUG_ON_PAGE(page->pmd_huge_pte, page);
// /*AFLA*/ #endif
// /*AFLA*/ 	ptlock_free(page);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define pmd_huge_pte(mm, pmd) (pmd_to_page(pmd)->pmd_huge_pte)
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ static inline spinlock_t *pmd_lockptr(struct mm_struct *mm, pmd_t *pmd)
// /*AFLA*/ {
// /*AFLA*/ 	return &mm->page_table_lock;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool pgtable_pmd_page_ctor(struct page *page) { return true; }
// /*AFLA*/ static inline void pgtable_pmd_page_dtor(struct page *page) {}
// /*AFLA*/ 
// /*AFLA*/ #define pmd_huge_pte(mm, pmd) ((mm)->pmd_huge_pte)
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline spinlock_t *pmd_lock(struct mm_struct *mm, pmd_t *pmd)
// /*AFLA*/ {
// /*AFLA*/ 	spinlock_t *ptl = pmd_lockptr(mm, pmd);
// /*AFLA*/ 	spin_lock(ptl);
// /*AFLA*/ 	return ptl;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void free_area_init(unsigned long * zones_size);
// /*AFLA*/ extern void free_area_init_node(int nid, unsigned long * zones_size,
// /*AFLA*/ 		unsigned long zone_start_pfn, unsigned long *zholes_size);
// /*AFLA*/ extern void free_initmem(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Free reserved pages within range [PAGE_ALIGN(start), end & PAGE_MASK)
// /*AFLA*/  * into the buddy system. The freed pages will be poisoned with pattern
// /*AFLA*/  * "poison" if it's within range [0, UCHAR_MAX].
// /*AFLA*/  * Return pages freed into the buddy system.
// /*AFLA*/  */
// /*AFLA*/ extern unsigned long free_reserved_area(void *start, void *end,
// /*AFLA*/ 					int poison, char *s);
// /*AFLA*/ 
// /*AFLA*/ #ifdef	CONFIG_HIGHMEM
// /*AFLA*/ /*
// /*AFLA*/  * Free a highmem page into the buddy system, adjusting totalhigh_pages
// /*AFLA*/  * and totalram_pages.
// /*AFLA*/  */
// /*AFLA*/ extern void free_highmem_page(struct page *page);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern void adjust_managed_page_count(struct page *page, long count);
// /*AFLA*/ extern void mem_init_print_info(const char *str);
// /*AFLA*/ 
// /*AFLA*/ extern void reserve_bootmem_region(phys_addr_t start, phys_addr_t end);
// /*AFLA*/ 
// /*AFLA*/ /* Free the reserved page into the buddy system, so it gets managed. */
// /*AFLA*/ static inline void __free_reserved_page(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	ClearPageReserved(page);
// /*AFLA*/ 	init_page_count(page);
// /*AFLA*/ 	__free_page(page);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void free_reserved_page(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	__free_reserved_page(page);
// /*AFLA*/ 	adjust_managed_page_count(page, 1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void mark_page_reserved(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	SetPageReserved(page);
// /*AFLA*/ 	adjust_managed_page_count(page, -1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Default method to free all the __init memory into the buddy system.
// /*AFLA*/  * The freed pages will be poisoned with pattern "poison" if it's within
// /*AFLA*/  * range [0, UCHAR_MAX].
// /*AFLA*/  * Return pages freed into the buddy system.
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned long free_initmem_default(int poison)
// /*AFLA*/ {
// /*AFLA*/ 	extern char __init_begin[], __init_end[];
// /*AFLA*/ 
// /*AFLA*/ 	return free_reserved_area(&__init_begin, &__init_end,
// /*AFLA*/ 				  poison, "unused kernel");
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long get_num_physpages(void)
// /*AFLA*/ {
// /*AFLA*/ 	int nid;
// /*AFLA*/ 	unsigned long phys_pages = 0;
// /*AFLA*/ 
// /*AFLA*/ 	for_each_online_node(nid)
// /*AFLA*/ 		phys_pages += node_present_pages(nid);
// /*AFLA*/ 
// /*AFLA*/ 	return phys_pages;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HAVE_MEMBLOCK_NODE_MAP
// /*AFLA*/ /*
// /*AFLA*/  * With CONFIG_HAVE_MEMBLOCK_NODE_MAP set, an architecture may initialise its
// /*AFLA*/  * zones, allocate the backing mem_map and account for memory holes in a more
// /*AFLA*/  * architecture independent manner. This is a substitute for creating the
// /*AFLA*/  * zone_sizes[] and zholes_size[] arrays and passing them to
// /*AFLA*/  * free_area_init_node()
// /*AFLA*/  *
// /*AFLA*/  * An architecture is expected to register range of page frames backed by
// /*AFLA*/  * physical memory with memblock_add[_node]() before calling
// /*AFLA*/  * free_area_init_nodes() passing in the PFN each zone ends at. At a basic
// /*AFLA*/  * usage, an architecture is expected to do something like
// /*AFLA*/  *
// /*AFLA*/  * unsigned long max_zone_pfns[MAX_NR_ZONES] = {max_dma, max_normal_pfn,
// /*AFLA*/  * 							 max_highmem_pfn};
// /*AFLA*/  * for_each_valid_physical_page_range()
// /*AFLA*/  * 	memblock_add_node(base, size, nid)
// /*AFLA*/  * free_area_init_nodes(max_zone_pfns);
// /*AFLA*/  *
// /*AFLA*/  * free_bootmem_with_active_regions() calls free_bootmem_node() for each
// /*AFLA*/  * registered physical page range.  Similarly
// /*AFLA*/  * sparse_memory_present_with_active_regions() calls memory_present() for
// /*AFLA*/  * each range when SPARSEMEM is enabled.
// /*AFLA*/  *
// /*AFLA*/  * See mm/page_alloc.c for more information on each function exposed by
// /*AFLA*/  * CONFIG_HAVE_MEMBLOCK_NODE_MAP.
// /*AFLA*/  */
// /*AFLA*/ extern void free_area_init_nodes(unsigned long *max_zone_pfn);
// /*AFLA*/ unsigned long node_map_pfn_alignment(void);
// /*AFLA*/ unsigned long __absent_pages_in_range(int nid, unsigned long start_pfn,
// /*AFLA*/ 						unsigned long end_pfn);
// /*AFLA*/ extern unsigned long absent_pages_in_range(unsigned long start_pfn,
// /*AFLA*/ 						unsigned long end_pfn);
// /*AFLA*/ extern void get_pfn_range_for_nid(unsigned int nid,
// /*AFLA*/ 			unsigned long *start_pfn, unsigned long *end_pfn);
// /*AFLA*/ extern unsigned long find_min_pfn_with_active_regions(void);
// /*AFLA*/ extern void free_bootmem_with_active_regions(int nid,
// /*AFLA*/ 						unsigned long max_low_pfn);
// /*AFLA*/ extern void sparse_memory_present_with_active_regions(int nid);
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_HAVE_MEMBLOCK_NODE_MAP */
// /*AFLA*/ 
// /*AFLA*/ #if !defined(CONFIG_HAVE_MEMBLOCK_NODE_MAP) && \
// /*AFLA*/     !defined(CONFIG_HAVE_ARCH_EARLY_PFN_TO_NID)
// /*AFLA*/ static inline int __early_pfn_to_nid(unsigned long pfn,
// /*AFLA*/ 					struct mminit_pfnnid_cache *state)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ /* please see mm/page_alloc.c */
// /*AFLA*/ extern int __meminit early_pfn_to_nid(unsigned long pfn);
// /*AFLA*/ /* there is a per-arch backend function. */
// /*AFLA*/ extern int __meminit __early_pfn_to_nid(unsigned long pfn,
// /*AFLA*/ 					struct mminit_pfnnid_cache *state);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern void set_dma_reserve(unsigned long new_dma_reserve);
// /*AFLA*/ extern void memmap_init_zone(unsigned long, int, unsigned long,
// /*AFLA*/ 				unsigned long, enum memmap_context);
// /*AFLA*/ extern void setup_per_zone_wmarks(void);
// /*AFLA*/ extern int __meminit init_per_zone_wmark_min(void);
// /*AFLA*/ extern void mem_init(void);
// /*AFLA*/ extern void __init mmap_init(void);
// /*AFLA*/ extern void show_mem(unsigned int flags);
// /*AFLA*/ extern long si_mem_available(void);
// /*AFLA*/ extern void si_meminfo(struct sysinfo * val);
// /*AFLA*/ extern void si_meminfo_node(struct sysinfo *val, int nid);
// /*AFLA*/ #ifdef __HAVE_ARCH_RESERVED_KERNEL_PAGES
// /*AFLA*/ extern unsigned long arch_reserved_kernel_pages(void);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern __printf(2, 3)
// /*AFLA*/ void warn_alloc(gfp_t gfp_mask, const char *fmt, ...);
// /*AFLA*/ 
// /*AFLA*/ extern void setup_per_cpu_pageset(void);
// /*AFLA*/ 
// /*AFLA*/ extern void zone_pcp_update(struct zone *zone);
// /*AFLA*/ extern void zone_pcp_reset(struct zone *zone);
// /*AFLA*/ 
// /*AFLA*/ /* page_alloc.c */
// /*AFLA*/ extern int min_free_kbytes;
// /*AFLA*/ extern int watermark_scale_factor;
// /*AFLA*/ 
// /*AFLA*/ /* nommu.c */
// /*AFLA*/ extern atomic_long_t mmap_pages_allocated;
// /*AFLA*/ extern int nommu_shrink_inode_mappings(struct inode *, size_t, size_t);
// /*AFLA*/ 
// /*AFLA*/ /* interval_tree.c */
// /*AFLA*/ void vma_interval_tree_insert(struct vm_area_struct *node,
// /*AFLA*/ 			      struct rb_root *root);
// /*AFLA*/ void vma_interval_tree_insert_after(struct vm_area_struct *node,
// /*AFLA*/ 				    struct vm_area_struct *prev,
// /*AFLA*/ 				    struct rb_root *root);
// /*AFLA*/ void vma_interval_tree_remove(struct vm_area_struct *node,
// /*AFLA*/ 			      struct rb_root *root);
// /*AFLA*/ struct vm_area_struct *vma_interval_tree_iter_first(struct rb_root *root,
// /*AFLA*/ 				unsigned long start, unsigned long last);
// /*AFLA*/ struct vm_area_struct *vma_interval_tree_iter_next(struct vm_area_struct *node,
// /*AFLA*/ 				unsigned long start, unsigned long last);
// /*AFLA*/ 
// /*AFLA*/ #define vma_interval_tree_foreach(vma, root, start, last)		\
// /*AFLA*/ 	for (vma = vma_interval_tree_iter_first(root, start, last);	\
// /*AFLA*/ 	     vma; vma = vma_interval_tree_iter_next(vma, start, last))
// /*AFLA*/ 
// /*AFLA*/ void anon_vma_interval_tree_insert(struct anon_vma_chain *node,
// /*AFLA*/ 				   struct rb_root *root);
// /*AFLA*/ void anon_vma_interval_tree_remove(struct anon_vma_chain *node,
// /*AFLA*/ 				   struct rb_root *root);
// /*AFLA*/ struct anon_vma_chain *anon_vma_interval_tree_iter_first(
// /*AFLA*/ 	struct rb_root *root, unsigned long start, unsigned long last);
// /*AFLA*/ struct anon_vma_chain *anon_vma_interval_tree_iter_next(
// /*AFLA*/ 	struct anon_vma_chain *node, unsigned long start, unsigned long last);
// /*AFLA*/ #ifdef CONFIG_DEBUG_VM_RB
// /*AFLA*/ void anon_vma_interval_tree_verify(struct anon_vma_chain *node);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define anon_vma_interval_tree_foreach(avc, root, start, last)		 \
// /*AFLA*/ 	for (avc = anon_vma_interval_tree_iter_first(root, start, last); \
// /*AFLA*/ 	     avc; avc = anon_vma_interval_tree_iter_next(avc, start, last))
// /*AFLA*/ 
// /*AFLA*/ /* mmap.c */
// /*AFLA*/ extern int __vm_enough_memory(struct mm_struct *mm, long pages, int cap_sys_admin);
// /*AFLA*/ extern int __vma_adjust(struct vm_area_struct *vma, unsigned long start,
// /*AFLA*/ 	unsigned long end, pgoff_t pgoff, struct vm_area_struct *insert,
// /*AFLA*/ 	struct vm_area_struct *expand);
// /*AFLA*/ static inline int vma_adjust(struct vm_area_struct *vma, unsigned long start,
// /*AFLA*/ 	unsigned long end, pgoff_t pgoff, struct vm_area_struct *insert)
// /*AFLA*/ {
// /*AFLA*/ 	return __vma_adjust(vma, start, end, pgoff, insert, NULL);
// /*AFLA*/ }
// /*AFLA*/ extern struct vm_area_struct *vma_merge(struct mm_struct *,
// /*AFLA*/ 	struct vm_area_struct *prev, unsigned long addr, unsigned long end,
// /*AFLA*/ 	unsigned long vm_flags, struct anon_vma *, struct file *, pgoff_t,
// /*AFLA*/ 	struct mempolicy *, struct vm_userfaultfd_ctx);
// /*AFLA*/ extern struct anon_vma *find_mergeable_anon_vma(struct vm_area_struct *);
// /*AFLA*/ extern int split_vma(struct mm_struct *,
// /*AFLA*/ 	struct vm_area_struct *, unsigned long addr, int new_below);
// /*AFLA*/ extern int insert_vm_struct(struct mm_struct *, struct vm_area_struct *);
// /*AFLA*/ extern void __vma_link_rb(struct mm_struct *, struct vm_area_struct *,
// /*AFLA*/ 	struct rb_node **, struct rb_node *);
// /*AFLA*/ extern void unlink_file_vma(struct vm_area_struct *);
// /*AFLA*/ extern struct vm_area_struct *copy_vma(struct vm_area_struct **,
// /*AFLA*/ 	unsigned long addr, unsigned long len, pgoff_t pgoff,
// /*AFLA*/ 	bool *need_rmap_locks);
// /*AFLA*/ extern void exit_mmap(struct mm_struct *);
// /*AFLA*/ 
// /*AFLA*/ static inline int check_data_rlimit(unsigned long rlim,
// /*AFLA*/ 				    unsigned long new,
// /*AFLA*/ 				    unsigned long start,
// /*AFLA*/ 				    unsigned long end_data,
// /*AFLA*/ 				    unsigned long start_data)
// /*AFLA*/ {
// /*AFLA*/ 	if (rlim < RLIM_INFINITY) {
// /*AFLA*/ 		if (((new - start) + (end_data - start_data)) > rlim)
// /*AFLA*/ 			return -ENOSPC;
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int mm_take_all_locks(struct mm_struct *mm);
// /*AFLA*/ extern void mm_drop_all_locks(struct mm_struct *mm);
// /*AFLA*/ 
// /*AFLA*/ extern void set_mm_exe_file(struct mm_struct *mm, struct file *new_exe_file);
// /*AFLA*/ extern struct file *get_mm_exe_file(struct mm_struct *mm);
// /*AFLA*/ extern struct file *get_task_exe_file(struct task_struct *task);
// /*AFLA*/ 
// /*AFLA*/ extern bool may_expand_vm(struct mm_struct *, vm_flags_t, unsigned long npages);
// /*AFLA*/ extern void vm_stat_account(struct mm_struct *, vm_flags_t, long npages);
// /*AFLA*/ 
// /*AFLA*/ extern bool vma_is_special_mapping(const struct vm_area_struct *vma,
// /*AFLA*/ 				   const struct vm_special_mapping *sm);
// /*AFLA*/ extern struct vm_area_struct *_install_special_mapping(struct mm_struct *mm,
// /*AFLA*/ 				   unsigned long addr, unsigned long len,
// /*AFLA*/ 				   unsigned long flags,
// /*AFLA*/ 				   const struct vm_special_mapping *spec);
// /*AFLA*/ /* This is an obsolete alternative to _install_special_mapping. */
// /*AFLA*/ extern int install_special_mapping(struct mm_struct *mm,
// /*AFLA*/ 				   unsigned long addr, unsigned long len,
// /*AFLA*/ 				   unsigned long flags, struct page **pages);
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long get_unmapped_area(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long mmap_region(struct file *file, unsigned long addr,
// /*AFLA*/ 	unsigned long len, vm_flags_t vm_flags, unsigned long pgoff);
// /*AFLA*/ extern unsigned long do_mmap(struct file *file, unsigned long addr,
// /*AFLA*/ 	unsigned long len, unsigned long prot, unsigned long flags,
// /*AFLA*/ 	vm_flags_t vm_flags, unsigned long pgoff, unsigned long *populate);
// /*AFLA*/ extern int do_munmap(struct mm_struct *, unsigned long, size_t);
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long
// /*AFLA*/ do_mmap_pgoff(struct file *file, unsigned long addr,
// /*AFLA*/ 	unsigned long len, unsigned long prot, unsigned long flags,
// /*AFLA*/ 	unsigned long pgoff, unsigned long *populate)
// /*AFLA*/ {
// /*AFLA*/ 	return do_mmap(file, addr, len, prot, flags, 0, pgoff, populate);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MMU
// /*AFLA*/ extern int __mm_populate(unsigned long addr, unsigned long len,
// /*AFLA*/ 			 int ignore_errors);
// /*AFLA*/ static inline void mm_populate(unsigned long addr, unsigned long len)
// /*AFLA*/ {
// /*AFLA*/ 	/* Ignore errors */
// /*AFLA*/ 	(void) __mm_populate(addr, len, 1);
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline void mm_populate(unsigned long addr, unsigned long len) {}
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* These take the mm semaphore themselves */
// /*AFLA*/ extern int __must_check vm_brk(unsigned long, unsigned long);
// /*AFLA*/ extern int vm_munmap(unsigned long, size_t);
// /*AFLA*/ extern unsigned long __must_check vm_mmap(struct file *, unsigned long,
// /*AFLA*/         unsigned long, unsigned long,
// /*AFLA*/         unsigned long, unsigned long);
// /*AFLA*/ 
// /*AFLA*/ struct vm_unmapped_area_info {
// /*AFLA*/ #define VM_UNMAPPED_AREA_TOPDOWN 1
// /*AFLA*/ 	unsigned long flags;
// /*AFLA*/ 	unsigned long length;
// /*AFLA*/ 	unsigned long low_limit;
// /*AFLA*/ 	unsigned long high_limit;
// /*AFLA*/ 	unsigned long align_mask;
// /*AFLA*/ 	unsigned long align_offset;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long unmapped_area(struct vm_unmapped_area_info *info);
// /*AFLA*/ extern unsigned long unmapped_area_topdown(struct vm_unmapped_area_info *info);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Search for an unmapped address range.
// /*AFLA*/  *
// /*AFLA*/  * We are looking for a range that:
// /*AFLA*/  * - does not intersect with any VMA;
// /*AFLA*/  * - is contained within the [low_limit, high_limit) interval;
// /*AFLA*/  * - is at least the desired size.
// /*AFLA*/  * - satisfies (begin_addr & align_mask) == (align_offset & align_mask)
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned long
// /*AFLA*/ vm_unmapped_area(struct vm_unmapped_area_info *info)
// /*AFLA*/ {
// /*AFLA*/ 	if (info->flags & VM_UNMAPPED_AREA_TOPDOWN)
// /*AFLA*/ 		return unmapped_area_topdown(info);
// /*AFLA*/ 	else
// /*AFLA*/ 		return unmapped_area(info);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* truncate.c */
// /*AFLA*/ extern void truncate_inode_pages(struct address_space *, loff_t);
// /*AFLA*/ extern void truncate_inode_pages_range(struct address_space *,
// /*AFLA*/ 				       loff_t lstart, loff_t lend);
// /*AFLA*/ extern void truncate_inode_pages_final(struct address_space *);
// /*AFLA*/ 
// /*AFLA*/ /* generic vm_area_ops exported for stackable file systems */
// /*AFLA*/ extern int filemap_fault(struct vm_area_struct *, struct vm_fault *);
// /*AFLA*/ extern void filemap_map_pages(struct fault_env *fe,
// /*AFLA*/ 		pgoff_t start_pgoff, pgoff_t end_pgoff);
// /*AFLA*/ extern int filemap_page_mkwrite(struct vm_area_struct *vma, struct vm_fault *vmf);
// /*AFLA*/ 
// /*AFLA*/ /* mm/page-writeback.c */
// /*AFLA*/ int write_one_page(struct page *page, int wait);
// /*AFLA*/ void task_dirty_inc(struct task_struct *tsk);
// /*AFLA*/ 
// /*AFLA*/ /* readahead.c */
// /*AFLA*/ #define VM_MAX_READAHEAD	128	/* kbytes */
// /*AFLA*/ #define VM_MIN_READAHEAD	16	/* kbytes (includes current page) */
// /*AFLA*/ 
// /*AFLA*/ int force_page_cache_readahead(struct address_space *mapping, struct file *filp,
// /*AFLA*/ 			pgoff_t offset, unsigned long nr_to_read);
// /*AFLA*/ 
// /*AFLA*/ void page_cache_sync_readahead(struct address_space *mapping,
// /*AFLA*/ 			       struct file_ra_state *ra,
// /*AFLA*/ 			       struct file *filp,
// /*AFLA*/ 			       pgoff_t offset,
// /*AFLA*/ 			       unsigned long size);
// /*AFLA*/ 
// /*AFLA*/ void page_cache_async_readahead(struct address_space *mapping,
// /*AFLA*/ 				struct file_ra_state *ra,
// /*AFLA*/ 				struct file *filp,
// /*AFLA*/ 				struct page *pg,
// /*AFLA*/ 				pgoff_t offset,
// /*AFLA*/ 				unsigned long size);
// /*AFLA*/ 
// /*AFLA*/ /* Generic expand stack which grows the stack according to GROWS{UP,DOWN} */
// /*AFLA*/ extern int expand_stack(struct vm_area_struct *vma, unsigned long address);
// /*AFLA*/ 
// /*AFLA*/ /* CONFIG_STACK_GROWSUP still needs to to grow downwards at some places */
// /*AFLA*/ extern int expand_downwards(struct vm_area_struct *vma,
// /*AFLA*/ 		unsigned long address);
// /*AFLA*/ #if VM_GROWSUP
// /*AFLA*/ extern int expand_upwards(struct vm_area_struct *vma, unsigned long address);
// /*AFLA*/ #else
// /*AFLA*/   #define expand_upwards(vma, address) (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Look up the first VMA which satisfies  addr < vm_end,  NULL if none. */
// /*AFLA*/ extern struct vm_area_struct * find_vma(struct mm_struct * mm, unsigned long addr);
// /*AFLA*/ extern struct vm_area_struct * find_vma_prev(struct mm_struct * mm, unsigned long addr,
// /*AFLA*/ 					     struct vm_area_struct **pprev);
// /*AFLA*/ 
// /*AFLA*/ /* Look up the first VMA which intersects the interval start_addr..end_addr-1,
// /*AFLA*/    NULL if none.  Assume start_addr < end_addr. */
// /*AFLA*/ static inline struct vm_area_struct * find_vma_intersection(struct mm_struct * mm, unsigned long start_addr, unsigned long end_addr)
// /*AFLA*/ {
// /*AFLA*/ 	struct vm_area_struct * vma = find_vma(mm,start_addr);
// /*AFLA*/ 
// /*AFLA*/ 	if (vma && end_addr <= vma->vm_start)
// /*AFLA*/ 		vma = NULL;
// /*AFLA*/ 	return vma;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long vma_pages(struct vm_area_struct *vma)
// /*AFLA*/ {
// /*AFLA*/ 	return (vma->vm_end - vma->vm_start) >> PAGE_SHIFT;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Look up the first VMA which exactly match the interval vm_start ... vm_end */
// /*AFLA*/ static inline struct vm_area_struct *find_exact_vma(struct mm_struct *mm,
// /*AFLA*/ 				unsigned long vm_start, unsigned long vm_end)
// /*AFLA*/ {
// /*AFLA*/ 	struct vm_area_struct *vma = find_vma(mm, vm_start);
// /*AFLA*/ 
// /*AFLA*/ 	if (vma && (vma->vm_start != vm_start || vma->vm_end != vm_end))
// /*AFLA*/ 		vma = NULL;
// /*AFLA*/ 
// /*AFLA*/ 	return vma;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MMU
// /*AFLA*/ pgprot_t vm_get_page_prot(unsigned long vm_flags);
// /*AFLA*/ void vma_set_page_prot(struct vm_area_struct *vma);
// /*AFLA*/ #else
// /*AFLA*/ static inline pgprot_t vm_get_page_prot(unsigned long vm_flags)
// /*AFLA*/ {
// /*AFLA*/ 	return __pgprot(0);
// /*AFLA*/ }
// /*AFLA*/ static inline void vma_set_page_prot(struct vm_area_struct *vma)
// /*AFLA*/ {
// /*AFLA*/ 	vma->vm_page_prot = vm_get_page_prot(vma->vm_flags);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NUMA_BALANCING
// /*AFLA*/ unsigned long change_prot_numa(struct vm_area_struct *vma,
// /*AFLA*/ 			unsigned long start, unsigned long end);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct vm_area_struct *find_extend_vma(struct mm_struct *, unsigned long addr);
// /*AFLA*/ int remap_pfn_range(struct vm_area_struct *, unsigned long addr,
// /*AFLA*/ 			unsigned long pfn, unsigned long size, pgprot_t);
// /*AFLA*/ int vm_insert_page(struct vm_area_struct *, unsigned long addr, struct page *);
// /*AFLA*/ int vm_insert_pfn(struct vm_area_struct *vma, unsigned long addr,
// /*AFLA*/ 			unsigned long pfn);
// /*AFLA*/ int vm_insert_pfn_prot(struct vm_area_struct *vma, unsigned long addr,
// /*AFLA*/ 			unsigned long pfn, pgprot_t pgprot);
// /*AFLA*/ int vm_insert_mixed(struct vm_area_struct *vma, unsigned long addr,
// /*AFLA*/ 			pfn_t pfn);
// /*AFLA*/ int vm_iomap_memory(struct vm_area_struct *vma, phys_addr_t start, unsigned long len);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ struct page *follow_page_mask(struct vm_area_struct *vma,
// /*AFLA*/ 			      unsigned long address, unsigned int foll_flags,
// /*AFLA*/ 			      unsigned int *page_mask);
// /*AFLA*/ 
// /*AFLA*/ static inline struct page *follow_page(struct vm_area_struct *vma,
// /*AFLA*/ 		unsigned long address, unsigned int foll_flags)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned int unused_page_mask;
// /*AFLA*/ 	return follow_page_mask(vma, address, foll_flags, &unused_page_mask);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define FOLL_WRITE	0x01	/* check pte is writable */
// /*AFLA*/ #define FOLL_TOUCH	0x02	/* mark page accessed */
// /*AFLA*/ #define FOLL_GET	0x04	/* do get_page on page */
// /*AFLA*/ #define FOLL_DUMP	0x08	/* give error on hole if it would be zero */
// /*AFLA*/ #define FOLL_FORCE	0x10	/* get_user_pages read/write w/o permission */
// /*AFLA*/ #define FOLL_NOWAIT	0x20	/* if a disk transfer is needed, start the IO
// /*AFLA*/ 				 * and return without waiting upon it */
// /*AFLA*/ #define FOLL_POPULATE	0x40	/* fault in page */
// /*AFLA*/ #define FOLL_SPLIT	0x80	/* don't return transhuge pages, split them */
// /*AFLA*/ #define FOLL_HWPOISON	0x100	/* check page is hwpoisoned */
// /*AFLA*/ #define FOLL_NUMA	0x200	/* force NUMA hinting page fault */
// /*AFLA*/ #define FOLL_MIGRATION	0x400	/* wait for page to replace migration entry */
// /*AFLA*/ #define FOLL_TRIED	0x800	/* a retry, previous pass started an IO */
// /*AFLA*/ #define FOLL_MLOCK	0x1000	/* lock present pages */
// /*AFLA*/ #define FOLL_REMOTE	0x2000	/* we are working on non-current tsk/mm */
// /*AFLA*/ #define FOLL_COW	0x4000	/* internal GUP flag */
// /*AFLA*/ 
// /*AFLA*/ typedef int (*pte_fn_t)(pte_t *pte, pgtable_t token, unsigned long addr,
// /*AFLA*/ 			void *data);
// /*AFLA*/ extern int apply_to_page_range(struct mm_struct *mm, unsigned long address,
// /*AFLA*/ 			       unsigned long size, pte_fn_t fn, void *data);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PAGE_POISONING
// /*AFLA*/ extern bool page_poisoning_enabled(void);
// /*AFLA*/ extern void kernel_poison_pages(struct page *page, int numpages, int enable);
// /*AFLA*/ extern bool page_is_poisoned(struct page *page);
// /*AFLA*/ #else
// /*AFLA*/ static inline bool page_poisoning_enabled(void) { return false; }
// /*AFLA*/ static inline void kernel_poison_pages(struct page *page, int numpages,
// /*AFLA*/ 					int enable) { }
// /*AFLA*/ static inline bool page_is_poisoned(struct page *page) { return false; }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_PAGEALLOC
// /*AFLA*/ extern bool _debug_pagealloc_enabled;
// /*AFLA*/ extern void __kernel_map_pages(struct page *page, int numpages, int enable);
// /*AFLA*/ 
// /*AFLA*/ static inline bool debug_pagealloc_enabled(void)
// /*AFLA*/ {
// /*AFLA*/ 	return _debug_pagealloc_enabled;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ kernel_map_pages(struct page *page, int numpages, int enable)
// /*AFLA*/ {
// /*AFLA*/ 	if (!debug_pagealloc_enabled())
// /*AFLA*/ 		return;
// /*AFLA*/ 
// /*AFLA*/ 	__kernel_map_pages(page, numpages, enable);
// /*AFLA*/ }
// /*AFLA*/ #ifdef CONFIG_HIBERNATION
// /*AFLA*/ extern bool kernel_page_present(struct page *page);
// /*AFLA*/ #endif	/* CONFIG_HIBERNATION */
// /*AFLA*/ #else	/* CONFIG_DEBUG_PAGEALLOC */
// /*AFLA*/ static inline void
// /*AFLA*/ kernel_map_pages(struct page *page, int numpages, int enable) {}
// /*AFLA*/ #ifdef CONFIG_HIBERNATION
// /*AFLA*/ static inline bool kernel_page_present(struct page *page) { return true; }
// /*AFLA*/ #endif	/* CONFIG_HIBERNATION */
// /*AFLA*/ static inline bool debug_pagealloc_enabled(void)
// /*AFLA*/ {
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ #endif	/* CONFIG_DEBUG_PAGEALLOC */
// /*AFLA*/ 
// /*AFLA*/ #ifdef __HAVE_ARCH_GATE_AREA
// /*AFLA*/ extern struct vm_area_struct *get_gate_vma(struct mm_struct *mm);
// /*AFLA*/ extern int in_gate_area_no_mm(unsigned long addr);
// /*AFLA*/ extern int in_gate_area(struct mm_struct *mm, unsigned long addr);
// /*AFLA*/ #else
// /*AFLA*/ static inline struct vm_area_struct *get_gate_vma(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ static inline int in_gate_area_no_mm(unsigned long addr) { return 0; }
// /*AFLA*/ static inline int in_gate_area(struct mm_struct *mm, unsigned long addr)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif	/* __HAVE_ARCH_GATE_AREA */
// /*AFLA*/ 
// /*AFLA*/ extern bool process_shares_mm(struct task_struct *p, struct mm_struct *mm);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SYSCTL
// /*AFLA*/ extern int sysctl_drop_caches;
// /*AFLA*/ int drop_caches_sysctl_handler(struct ctl_table *, int,
// /*AFLA*/ 					void __user *, size_t *, loff_t *);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ void drop_slab(void);
// /*AFLA*/ void drop_slab_node(int nid);
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_MMU
// /*AFLA*/ #define randomize_va_space 0
// /*AFLA*/ #else
// /*AFLA*/ extern int randomize_va_space;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ const char * arch_vma_name(struct vm_area_struct *vma);
// /*AFLA*/ void print_vma_addr(char *prefix, unsigned long rip);
// /*AFLA*/ 
// /*AFLA*/ void sparse_mem_maps_populate_node(struct page **map_map,
// /*AFLA*/ 				   unsigned long pnum_begin,
// /*AFLA*/ 				   unsigned long pnum_end,
// /*AFLA*/ 				   unsigned long map_count,
// /*AFLA*/ 				   int nodeid);
// /*AFLA*/ 
// /*AFLA*/ struct page *sparse_mem_map_populate(unsigned long pnum, int nid);
// /*AFLA*/ pgd_t *vmemmap_pgd_populate(unsigned long addr, int node);
// /*AFLA*/ pud_t *vmemmap_pud_populate(pgd_t *pgd, unsigned long addr, int node);
// /*AFLA*/ pmd_t *vmemmap_pmd_populate(pud_t *pud, unsigned long addr, int node);
// /*AFLA*/ pte_t *vmemmap_pte_populate(pmd_t *pmd, unsigned long addr, int node);
// /*AFLA*/ void *vmemmap_alloc_block(unsigned long size, int node);
// /*AFLA*/ struct vmem_altmap;
// /*AFLA*/ void *__vmemmap_alloc_block_buf(unsigned long size, int node,
// /*AFLA*/ 		struct vmem_altmap *altmap);
// /*AFLA*/ static inline void *vmemmap_alloc_block_buf(unsigned long size, int node)
// /*AFLA*/ {
// /*AFLA*/ 	return __vmemmap_alloc_block_buf(size, node, NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ void vmemmap_verify(pte_t *, int, unsigned long, unsigned long);
// /*AFLA*/ int vmemmap_populate_basepages(unsigned long start, unsigned long end,
// /*AFLA*/ 			       int node);
// /*AFLA*/ int vmemmap_populate(unsigned long start, unsigned long end, int node);
// /*AFLA*/ void vmemmap_populate_print_last(void);
// /*AFLA*/ #ifdef CONFIG_MEMORY_HOTPLUG
// /*AFLA*/ void vmemmap_free(unsigned long start, unsigned long end);
// /*AFLA*/ #endif
// /*AFLA*/ void register_page_bootmem_memmap(unsigned long section_nr, struct page *map,
// /*AFLA*/ 				  unsigned long size);
// /*AFLA*/ 
// /*AFLA*/ enum mf_flags {
// /*AFLA*/ 	MF_COUNT_INCREASED = 1 << 0,
// /*AFLA*/ 	MF_ACTION_REQUIRED = 1 << 1,
// /*AFLA*/ 	MF_MUST_KILL = 1 << 2,
// /*AFLA*/ 	MF_SOFT_OFFLINE = 1 << 3,
// /*AFLA*/ };
// /*AFLA*/ extern int memory_failure(unsigned long pfn, int trapno, int flags);
// /*AFLA*/ extern void memory_failure_queue(unsigned long pfn, int trapno, int flags);
// /*AFLA*/ extern int unpoison_memory(unsigned long pfn);
// /*AFLA*/ extern int get_hwpoison_page(struct page *page);
// /*AFLA*/ #define put_hwpoison_page(page)	put_page(page)
// /*AFLA*/ extern int sysctl_memory_failure_early_kill;
// /*AFLA*/ extern int sysctl_memory_failure_recovery;
// /*AFLA*/ extern void shake_page(struct page *p, int access);
// /*AFLA*/ extern atomic_long_t num_poisoned_pages;
// /*AFLA*/ extern int soft_offline_page(struct page *page, int flags);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Error handlers for various types of pages.
// /*AFLA*/  */
// /*AFLA*/ enum mf_result {
// /*AFLA*/ 	MF_IGNORED,	/* Error: cannot be handled */
// /*AFLA*/ 	MF_FAILED,	/* Error: handling failed */
// /*AFLA*/ 	MF_DELAYED,	/* Will be handled later */
// /*AFLA*/ 	MF_RECOVERED,	/* Successfully recovered */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ enum mf_action_page_type {
// /*AFLA*/ 	MF_MSG_KERNEL,
// /*AFLA*/ 	MF_MSG_KERNEL_HIGH_ORDER,
// /*AFLA*/ 	MF_MSG_SLAB,
// /*AFLA*/ 	MF_MSG_DIFFERENT_COMPOUND,
// /*AFLA*/ 	MF_MSG_POISONED_HUGE,
// /*AFLA*/ 	MF_MSG_HUGE,
// /*AFLA*/ 	MF_MSG_FREE_HUGE,
// /*AFLA*/ 	MF_MSG_UNMAP_FAILED,
// /*AFLA*/ 	MF_MSG_DIRTY_SWAPCACHE,
// /*AFLA*/ 	MF_MSG_CLEAN_SWAPCACHE,
// /*AFLA*/ 	MF_MSG_DIRTY_MLOCKED_LRU,
// /*AFLA*/ 	MF_MSG_CLEAN_MLOCKED_LRU,
// /*AFLA*/ 	MF_MSG_DIRTY_UNEVICTABLE_LRU,
// /*AFLA*/ 	MF_MSG_CLEAN_UNEVICTABLE_LRU,
// /*AFLA*/ 	MF_MSG_DIRTY_LRU,
// /*AFLA*/ 	MF_MSG_CLEAN_LRU,
// /*AFLA*/ 	MF_MSG_TRUNCATED_LRU,
// /*AFLA*/ 	MF_MSG_BUDDY,
// /*AFLA*/ 	MF_MSG_BUDDY_2ND,
// /*AFLA*/ 	MF_MSG_UNKNOWN,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_TRANSPARENT_HUGEPAGE) || defined(CONFIG_HUGETLBFS)
// /*AFLA*/ extern void clear_huge_page(struct page *page,
// /*AFLA*/ 			    unsigned long addr,
// /*AFLA*/ 			    unsigned int pages_per_huge_page);
// /*AFLA*/ extern void copy_user_huge_page(struct page *dst, struct page *src,
// /*AFLA*/ 				unsigned long addr, struct vm_area_struct *vma,
// /*AFLA*/ 				unsigned int pages_per_huge_page);
// /*AFLA*/ #endif /* CONFIG_TRANSPARENT_HUGEPAGE || CONFIG_HUGETLBFS */
// /*AFLA*/ 
// /*AFLA*/ extern struct page_ext_operations debug_guardpage_ops;
// /*AFLA*/ extern struct page_ext_operations page_poisoning_ops;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_PAGEALLOC
// /*AFLA*/ extern unsigned int _debug_guardpage_minorder;
// /*AFLA*/ extern bool _debug_guardpage_enabled;
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned int debug_guardpage_minorder(void)
// /*AFLA*/ {
// /*AFLA*/ 	return _debug_guardpage_minorder;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool debug_guardpage_enabled(void)
// /*AFLA*/ {
// /*AFLA*/ 	return _debug_guardpage_enabled;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool page_is_guard(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	struct page_ext *page_ext;
// /*AFLA*/ 
// /*AFLA*/ 	if (!debug_guardpage_enabled())
// /*AFLA*/ 		return false;
// /*AFLA*/ 
// /*AFLA*/ 	page_ext = lookup_page_ext(page);
// /*AFLA*/ 	if (unlikely(!page_ext))
// /*AFLA*/ 		return false;
// /*AFLA*/ 
// /*AFLA*/ 	return test_bit(PAGE_EXT_DEBUG_GUARD, &page_ext->flags);
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline unsigned int debug_guardpage_minorder(void) { return 0; }
// /*AFLA*/ static inline bool debug_guardpage_enabled(void) { return false; }
// /*AFLA*/ static inline bool page_is_guard(struct page *page) { return false; }
// /*AFLA*/ #endif /* CONFIG_DEBUG_PAGEALLOC */
// /*AFLA*/ 
// /*AFLA*/ #if MAX_NUMNODES > 1
// /*AFLA*/ void __init setup_nr_node_ids(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline void setup_nr_node_ids(void) {}
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* __KERNEL__ */
// /*AFLA*/ #endif /* _LINUX_MM_H */
