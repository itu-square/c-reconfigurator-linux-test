// /*AFLA*/ #ifndef _LINUX_MM_TYPES_H
// /*AFLA*/ #define _LINUX_MM_TYPES_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/auxvec.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/threads.h>
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/spinlock.h>
// /*AFLA*/ #include <linux/rbtree.h>
// /*AFLA*/ #include <linux/rwsem.h>
// /*AFLA*/ #include <linux/completion.h>
// /*AFLA*/ #include <linux/cpumask.h>
// /*AFLA*/ #include <linux/uprobes.h>
// /*AFLA*/ #include <linux/page-flags-layout.h>
// /*AFLA*/ #include <linux/workqueue.h>
// /*AFLA*/ #include <asm/page.h>
// /*AFLA*/ #include <asm/mmu.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef AT_VECTOR_SIZE_ARCH
// /*AFLA*/ #define AT_VECTOR_SIZE_ARCH 0
// /*AFLA*/ #endif
// /*AFLA*/ #define AT_VECTOR_SIZE (2*(AT_VECTOR_SIZE_ARCH + AT_VECTOR_SIZE_BASE + 1))
// /*AFLA*/ 
// /*AFLA*/ struct address_space;
// /*AFLA*/ struct mem_cgroup;
// /*AFLA*/ 
// /*AFLA*/ #define USE_SPLIT_PTE_PTLOCKS	(NR_CPUS >= CONFIG_SPLIT_PTLOCK_CPUS)
// /*AFLA*/ #define USE_SPLIT_PMD_PTLOCKS	(USE_SPLIT_PTE_PTLOCKS && \
// /*AFLA*/ 		IS_ENABLED(CONFIG_ARCH_ENABLE_SPLIT_PMD_PTLOCK))
// /*AFLA*/ #define ALLOC_SPLIT_PTLOCKS	(SPINLOCK_SIZE > BITS_PER_LONG/8)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Each physical page in the system has a struct page associated with
// /*AFLA*/  * it to keep track of whatever it is we are using the page for at the
// /*AFLA*/  * moment. Note that we have no way to track which tasks are using
// /*AFLA*/  * a page, though if it is a pagecache page, rmap structures can tell us
// /*AFLA*/  * who is mapping it.
// /*AFLA*/  *
// /*AFLA*/  * The objects in struct page are organized in double word blocks in
// /*AFLA*/  * order to allows us to use atomic double word operations on portions
// /*AFLA*/  * of struct page. That is currently only used by slub but the arrangement
// /*AFLA*/  * allows the use of atomic double word operations on the flags/mapping
// /*AFLA*/  * and lru list pointers also.
// /*AFLA*/  */
// /*AFLA*/ struct page {
// /*AFLA*/ 	/* First double word block */
// /*AFLA*/ 	unsigned long flags;		/* Atomic flags, some possibly
// /*AFLA*/ 					 * updated asynchronously */
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct address_space *mapping;	/* If low bit clear, points to
// /*AFLA*/ 						 * inode address_space, or NULL.
// /*AFLA*/ 						 * If page mapped as anonymous
// /*AFLA*/ 						 * memory, low bit is set, and
// /*AFLA*/ 						 * it points to anon_vma object:
// /*AFLA*/ 						 * see PAGE_MAPPING_ANON below.
// /*AFLA*/ 						 */
// /*AFLA*/ 		void *s_mem;			/* slab first object */
// /*AFLA*/ 		atomic_t compound_mapcount;	/* first tail page */
// /*AFLA*/ 		/* page_deferred_list().next	 -- second tail page */
// /*AFLA*/ 	};
// /*AFLA*/ 
// /*AFLA*/ 	/* Second double word */
// /*AFLA*/ 	union {
// /*AFLA*/ 		pgoff_t index;		/* Our offset within mapping. */
// /*AFLA*/ 		void *freelist;		/* sl[aou]b first free object */
// /*AFLA*/ 		/* page_deferred_list().prev	-- second tail page */
// /*AFLA*/ 	};
// /*AFLA*/ 
// /*AFLA*/ 	union {
// /*AFLA*/ #if defined(CONFIG_HAVE_CMPXCHG_DOUBLE) && \
// /*AFLA*/ 	defined(CONFIG_HAVE_ALIGNED_STRUCT_PAGE)
// /*AFLA*/ 		/* Used for cmpxchg_double in slub */
// /*AFLA*/ 		unsigned long counters;
// /*AFLA*/ #else
// /*AFLA*/ 		/*
// /*AFLA*/ 		 * Keep _refcount separate from slub cmpxchg_double data.
// /*AFLA*/ 		 * As the rest of the double word is protected by slab_lock
// /*AFLA*/ 		 * but _refcount is not.
// /*AFLA*/ 		 */
// /*AFLA*/ 		unsigned counters;
// /*AFLA*/ #endif
// /*AFLA*/ 		struct {
// /*AFLA*/ 
// /*AFLA*/ 			union {
// /*AFLA*/ 				/*
// /*AFLA*/ 				 * Count of ptes mapped in mms, to show when
// /*AFLA*/ 				 * page is mapped & limit reverse map searches.
// /*AFLA*/ 				 *
// /*AFLA*/ 				 * Extra information about page type may be
// /*AFLA*/ 				 * stored here for pages that are never mapped,
// /*AFLA*/ 				 * in which case the value MUST BE <= -2.
// /*AFLA*/ 				 * See page-flags.h for more details.
// /*AFLA*/ 				 */
// /*AFLA*/ 				atomic_t _mapcount;
// /*AFLA*/ 
// /*AFLA*/ 				unsigned int active;		/* SLAB */
// /*AFLA*/ 				struct {			/* SLUB */
// /*AFLA*/ 					unsigned inuse:16;
// /*AFLA*/ 					unsigned objects:15;
// /*AFLA*/ 					unsigned frozen:1;
// /*AFLA*/ 				};
// /*AFLA*/ 				int units;			/* SLOB */
// /*AFLA*/ 			};
// /*AFLA*/ 			/*
// /*AFLA*/ 			 * Usage count, *USE WRAPPER FUNCTION* when manual
// /*AFLA*/ 			 * accounting. See page_ref.h
// /*AFLA*/ 			 */
// /*AFLA*/ 			atomic_t _refcount;
// /*AFLA*/ 		};
// /*AFLA*/ 	};
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Third double word block
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * WARNING: bit 0 of the first word encode PageTail(). That means
// /*AFLA*/ 	 * the rest users of the storage space MUST NOT use the bit to
// /*AFLA*/ 	 * avoid collision and false-positive PageTail().
// /*AFLA*/ 	 */
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct list_head lru;	/* Pageout list, eg. active_list
// /*AFLA*/ 					 * protected by zone_lru_lock !
// /*AFLA*/ 					 * Can be used as a generic list
// /*AFLA*/ 					 * by the page owner.
// /*AFLA*/ 					 */
// /*AFLA*/ 		struct dev_pagemap *pgmap; /* ZONE_DEVICE pages are never on an
// /*AFLA*/ 					    * lru or handled by a slab
// /*AFLA*/ 					    * allocator, this points to the
// /*AFLA*/ 					    * hosting device page map.
// /*AFLA*/ 					    */
// /*AFLA*/ 		struct {		/* slub per cpu partial pages */
// /*AFLA*/ 			struct page *next;	/* Next partial slab */
// /*AFLA*/ #ifdef CONFIG_64BIT
// /*AFLA*/ 			int pages;	/* Nr of partial slabs left */
// /*AFLA*/ 			int pobjects;	/* Approximate # of objects */
// /*AFLA*/ #else
// /*AFLA*/ 			short int pages;
// /*AFLA*/ 			short int pobjects;
// /*AFLA*/ #endif
// /*AFLA*/ 		};
// /*AFLA*/ 
// /*AFLA*/ 		struct rcu_head rcu_head;	/* Used by SLAB
// /*AFLA*/ 						 * when destroying via RCU
// /*AFLA*/ 						 */
// /*AFLA*/ 		/* Tail pages of compound page */
// /*AFLA*/ 		struct {
// /*AFLA*/ 			unsigned long compound_head; /* If bit zero is set */
// /*AFLA*/ 
// /*AFLA*/ 			/* First tail page only */
// /*AFLA*/ #ifdef CONFIG_64BIT
// /*AFLA*/ 			/*
// /*AFLA*/ 			 * On 64 bit system we have enough space in struct page
// /*AFLA*/ 			 * to encode compound_dtor and compound_order with
// /*AFLA*/ 			 * unsigned int. It can help compiler generate better or
// /*AFLA*/ 			 * smaller code on some archtectures.
// /*AFLA*/ 			 */
// /*AFLA*/ 			unsigned int compound_dtor;
// /*AFLA*/ 			unsigned int compound_order;
// /*AFLA*/ #else
// /*AFLA*/ 			unsigned short int compound_dtor;
// /*AFLA*/ 			unsigned short int compound_order;
// /*AFLA*/ #endif
// /*AFLA*/ 		};
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_TRANSPARENT_HUGEPAGE) && USE_SPLIT_PMD_PTLOCKS
// /*AFLA*/ 		struct {
// /*AFLA*/ 			unsigned long __pad;	/* do not overlay pmd_huge_pte
// /*AFLA*/ 						 * with compound_head to avoid
// /*AFLA*/ 						 * possible bit 0 collision.
// /*AFLA*/ 						 */
// /*AFLA*/ 			pgtable_t pmd_huge_pte; /* protected by page->ptl */
// /*AFLA*/ 		};
// /*AFLA*/ #endif
// /*AFLA*/ 	};
// /*AFLA*/ 
// /*AFLA*/ 	/* Remainder is not double word aligned */
// /*AFLA*/ 	union {
// /*AFLA*/ 		unsigned long private;		/* Mapping-private opaque data:
// /*AFLA*/ 					 	 * usually used for buffer_heads
// /*AFLA*/ 						 * if PagePrivate set; used for
// /*AFLA*/ 						 * swp_entry_t if PageSwapCache;
// /*AFLA*/ 						 * indicates order in the buddy
// /*AFLA*/ 						 * system if PG_buddy is set.
// /*AFLA*/ 						 */
// /*AFLA*/ #if USE_SPLIT_PTE_PTLOCKS
// /*AFLA*/ #if ALLOC_SPLIT_PTLOCKS
// /*AFLA*/ 		spinlock_t *ptl;
// /*AFLA*/ #else
// /*AFLA*/ 		spinlock_t ptl;
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ 		struct kmem_cache *slab_cache;	/* SL[AU]B: Pointer to slab */
// /*AFLA*/ 	};
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MEMCG
// /*AFLA*/ 	struct mem_cgroup *mem_cgroup;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * On machines where all RAM is mapped into kernel address space,
// /*AFLA*/ 	 * we can simply calculate the virtual address. On machines with
// /*AFLA*/ 	 * highmem some memory is mapped into kernel virtual memory
// /*AFLA*/ 	 * dynamically, so we need a place to store that address.
// /*AFLA*/ 	 * Note that this field could be 16 bits on x86 ... ;)
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * Architectures with slow multiplication can define
// /*AFLA*/ 	 * WANT_PAGE_VIRTUAL in asm/page.h
// /*AFLA*/ 	 */
// /*AFLA*/ #if defined(WANT_PAGE_VIRTUAL)
// /*AFLA*/ 	void *virtual;			/* Kernel virtual address (NULL if
// /*AFLA*/ 					   not kmapped, ie. highmem) */
// /*AFLA*/ #endif /* WANT_PAGE_VIRTUAL */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_KMEMCHECK
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * kmemcheck wants to track the status of each byte in a page; this
// /*AFLA*/ 	 * is a pointer to such a status block. NULL if not tracked.
// /*AFLA*/ 	 */
// /*AFLA*/ 	void *shadow;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef LAST_CPUPID_NOT_IN_PAGE_FLAGS
// /*AFLA*/ 	int _last_cpupid;
// /*AFLA*/ #endif
// /*AFLA*/ }
// /*AFLA*/ /*
// /*AFLA*/  * The struct page can be forced to be double word aligned so that atomic ops
// /*AFLA*/  * on double words work. The SLUB allocator can make use of such a feature.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_HAVE_ALIGNED_STRUCT_PAGE
// /*AFLA*/ 	__aligned(2 * sizeof(unsigned long))
// /*AFLA*/ #endif
// /*AFLA*/ ;
// /*AFLA*/ 
// /*AFLA*/ struct page_frag {
// /*AFLA*/ 	struct page *page;
// /*AFLA*/ #if (BITS_PER_LONG > 32) || (PAGE_SIZE >= 65536)
// /*AFLA*/ 	__u32 offset;
// /*AFLA*/ 	__u32 size;
// /*AFLA*/ #else
// /*AFLA*/ 	__u16 offset;
// /*AFLA*/ 	__u16 size;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define PAGE_FRAG_CACHE_MAX_SIZE	__ALIGN_MASK(32768, ~PAGE_MASK)
// /*AFLA*/ #define PAGE_FRAG_CACHE_MAX_ORDER	get_order(PAGE_FRAG_CACHE_MAX_SIZE)
// /*AFLA*/ 
// /*AFLA*/ struct page_frag_cache {
// /*AFLA*/ 	void * va;
// /*AFLA*/ #if (PAGE_SIZE < PAGE_FRAG_CACHE_MAX_SIZE)
// /*AFLA*/ 	__u16 offset;
// /*AFLA*/ 	__u16 size;
// /*AFLA*/ #else
// /*AFLA*/ 	__u32 offset;
// /*AFLA*/ #endif
// /*AFLA*/ 	/* we maintain a pagecount bias, so that we dont dirty cache line
// /*AFLA*/ 	 * containing page->_refcount every time we allocate a fragment.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned int		pagecnt_bias;
// /*AFLA*/ 	bool pfmemalloc;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ typedef unsigned long vm_flags_t;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * A region containing a mapping of a non-memory backed file under NOMMU
// /*AFLA*/  * conditions.  These are held in a global tree and are pinned by the VMAs that
// /*AFLA*/  * map parts of them.
// /*AFLA*/  */
// /*AFLA*/ struct vm_region {
// /*AFLA*/ 	struct rb_node	vm_rb;		/* link in global region tree */
// /*AFLA*/ 	vm_flags_t	vm_flags;	/* VMA vm_flags */
// /*AFLA*/ 	unsigned long	vm_start;	/* start address of region */
// /*AFLA*/ 	unsigned long	vm_end;		/* region initialised to here */
// /*AFLA*/ 	unsigned long	vm_top;		/* region allocated to here */
// /*AFLA*/ 	unsigned long	vm_pgoff;	/* the offset in vm_file corresponding to vm_start */
// /*AFLA*/ 	struct file	*vm_file;	/* the backing file or NULL */
// /*AFLA*/ 
// /*AFLA*/ 	int		vm_usage;	/* region usage count (access under nommu_region_sem) */
// /*AFLA*/ 	bool		vm_icache_flushed : 1; /* true if the icache has been flushed for
// /*AFLA*/ 						* this region */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_USERFAULTFD
// /*AFLA*/ #define NULL_VM_UFFD_CTX ((struct vm_userfaultfd_ctx) { NULL, })
// /*AFLA*/ struct vm_userfaultfd_ctx {
// /*AFLA*/ 	struct userfaultfd_ctx *ctx;
// /*AFLA*/ };
// /*AFLA*/ #else /* CONFIG_USERFAULTFD */
// /*AFLA*/ #define NULL_VM_UFFD_CTX ((struct vm_userfaultfd_ctx) {})
// /*AFLA*/ struct vm_userfaultfd_ctx {};
// /*AFLA*/ #endif /* CONFIG_USERFAULTFD */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This struct defines a memory VMM memory area. There is one of these
// /*AFLA*/  * per VM-area/task.  A VM area is any part of the process virtual memory
// /*AFLA*/  * space that has a special rule for the page-fault handlers (ie a shared
// /*AFLA*/  * library, the executable area etc).
// /*AFLA*/  */
// /*AFLA*/ struct vm_area_struct {
// /*AFLA*/ 	/* The first cache line has the info for VMA tree walking. */
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long vm_start;		/* Our start address within vm_mm. */
// /*AFLA*/ 	unsigned long vm_end;		/* The first byte after our end address
// /*AFLA*/ 					   within vm_mm. */
// /*AFLA*/ 
// /*AFLA*/ 	/* linked list of VM areas per task, sorted by address */
// /*AFLA*/ 	struct vm_area_struct *vm_next, *vm_prev;
// /*AFLA*/ 
// /*AFLA*/ 	struct rb_node vm_rb;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Largest free memory gap in bytes to the left of this VMA.
// /*AFLA*/ 	 * Either between this VMA and vma->vm_prev, or between one of the
// /*AFLA*/ 	 * VMAs below us in the VMA rbtree and its ->vm_prev. This helps
// /*AFLA*/ 	 * get_unmapped_area find a free area of the right size.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long rb_subtree_gap;
// /*AFLA*/ 
// /*AFLA*/ 	/* Second cache line starts here. */
// /*AFLA*/ 
// /*AFLA*/ 	struct mm_struct *vm_mm;	/* The address space we belong to. */
// /*AFLA*/ 	pgprot_t vm_page_prot;		/* Access permissions of this VMA. */
// /*AFLA*/ 	unsigned long vm_flags;		/* Flags, see mm.h. */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * For areas with an address space and backing store,
// /*AFLA*/ 	 * linkage into the address_space->i_mmap interval tree.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct {
// /*AFLA*/ 		struct rb_node rb;
// /*AFLA*/ 		unsigned long rb_subtree_last;
// /*AFLA*/ 	} shared;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * A file's MAP_PRIVATE vma can be in both i_mmap tree and anon_vma
// /*AFLA*/ 	 * list, after a COW of one of the file pages.	A MAP_SHARED vma
// /*AFLA*/ 	 * can only be in the i_mmap tree.  An anonymous MAP_PRIVATE, stack
// /*AFLA*/ 	 * or brk vma (with NULL file) can only be in an anon_vma list.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct list_head anon_vma_chain; /* Serialized by mmap_sem &
// /*AFLA*/ 					  * page_table_lock */
// /*AFLA*/ 	struct anon_vma *anon_vma;	/* Serialized by page_table_lock */
// /*AFLA*/ 
// /*AFLA*/ 	/* Function pointers to deal with this struct. */
// /*AFLA*/ 	const struct vm_operations_struct *vm_ops;
// /*AFLA*/ 
// /*AFLA*/ 	/* Information about our backing store: */
// /*AFLA*/ 	unsigned long vm_pgoff;		/* Offset (within vm_file) in PAGE_SIZE
// /*AFLA*/ 					   units */
// /*AFLA*/ 	struct file * vm_file;		/* File we map to (can be NULL). */
// /*AFLA*/ 	void * vm_private_data;		/* was vm_pte (shared mem) */
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_MMU
// /*AFLA*/ 	struct vm_region *vm_region;	/* NOMMU mapping region */
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ 	struct mempolicy *vm_policy;	/* NUMA policy for the VMA */
// /*AFLA*/ #endif
// /*AFLA*/ 	struct vm_userfaultfd_ctx vm_userfaultfd_ctx;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct core_thread {
// /*AFLA*/ 	struct task_struct *task;
// /*AFLA*/ 	struct core_thread *next;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct core_state {
// /*AFLA*/ 	atomic_t nr_threads;
// /*AFLA*/ 	struct core_thread dumper;
// /*AFLA*/ 	struct completion startup;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	MM_FILEPAGES,	/* Resident file mapping pages */
// /*AFLA*/ 	MM_ANONPAGES,	/* Resident anonymous pages */
// /*AFLA*/ 	MM_SWAPENTS,	/* Anonymous swap entries */
// /*AFLA*/ 	MM_SHMEMPAGES,	/* Resident shared memory pages */
// /*AFLA*/ 	NR_MM_COUNTERS
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #if USE_SPLIT_PTE_PTLOCKS && defined(CONFIG_MMU)
// /*AFLA*/ #define SPLIT_RSS_COUNTING
// /*AFLA*/ /* per-thread cached information, */
// /*AFLA*/ struct task_rss_stat {
// /*AFLA*/ 	int events;	/* for synchronization threshold */
// /*AFLA*/ 	int count[NR_MM_COUNTERS];
// /*AFLA*/ };
// /*AFLA*/ #endif /* USE_SPLIT_PTE_PTLOCKS */
// /*AFLA*/ 
// /*AFLA*/ struct mm_rss_stat {
// /*AFLA*/ 	atomic_long_t count[NR_MM_COUNTERS];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct kioctx_table;
// /*AFLA*/ struct mm_struct {
// /*AFLA*/ 	struct vm_area_struct *mmap;		/* list of VMAs */
// /*AFLA*/ 	struct rb_root mm_rb;
// /*AFLA*/ 	u32 vmacache_seqnum;                   /* per-thread vmacache */
// /*AFLA*/ #ifdef CONFIG_MMU
// /*AFLA*/ 	unsigned long (*get_unmapped_area) (struct file *filp,
// /*AFLA*/ 				unsigned long addr, unsigned long len,
// /*AFLA*/ 				unsigned long pgoff, unsigned long flags);
// /*AFLA*/ #endif
// /*AFLA*/ 	unsigned long mmap_base;		/* base of mmap area */
// /*AFLA*/ 	unsigned long mmap_legacy_base;         /* base of mmap area in bottom-up allocations */
// /*AFLA*/ 	unsigned long task_size;		/* size of task vm space */
// /*AFLA*/ 	unsigned long highest_vm_end;		/* highest vma end address */
// /*AFLA*/ 	pgd_t * pgd;
// /*AFLA*/ 	atomic_t mm_users;			/* How many users with user space? */
// /*AFLA*/ 	atomic_t mm_count;			/* How many references to "struct mm_struct" (users count as 1) */
// /*AFLA*/ 	atomic_long_t nr_ptes;			/* PTE page table pages */
// /*AFLA*/ #if CONFIG_PGTABLE_LEVELS > 2
// /*AFLA*/ 	atomic_long_t nr_pmds;			/* PMD page table pages */
// /*AFLA*/ #endif
// /*AFLA*/ 	int map_count;				/* number of VMAs */
// /*AFLA*/ 
// /*AFLA*/ 	spinlock_t page_table_lock;		/* Protects page tables and some counters */
// /*AFLA*/ 	struct rw_semaphore mmap_sem;
// /*AFLA*/ 
// /*AFLA*/ 	struct list_head mmlist;		/* List of maybe swapped mm's.	These are globally strung
// /*AFLA*/ 						 * together off init_mm.mmlist, and are protected
// /*AFLA*/ 						 * by mmlist_lock
// /*AFLA*/ 						 */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long hiwater_rss;	/* High-watermark of RSS usage */
// /*AFLA*/ 	unsigned long hiwater_vm;	/* High-water virtual memory usage */
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long total_vm;		/* Total pages mapped */
// /*AFLA*/ 	unsigned long locked_vm;	/* Pages that have PG_mlocked set */
// /*AFLA*/ 	unsigned long pinned_vm;	/* Refcount permanently increased */
// /*AFLA*/ 	unsigned long data_vm;		/* VM_WRITE & ~VM_SHARED & ~VM_STACK */
// /*AFLA*/ 	unsigned long exec_vm;		/* VM_EXEC & ~VM_WRITE & ~VM_STACK */
// /*AFLA*/ 	unsigned long stack_vm;		/* VM_STACK */
// /*AFLA*/ 	unsigned long def_flags;
// /*AFLA*/ 	unsigned long start_code, end_code, start_data, end_data;
// /*AFLA*/ 	unsigned long start_brk, brk, start_stack;
// /*AFLA*/ 	unsigned long arg_start, arg_end, env_start, env_end;
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long saved_auxv[AT_VECTOR_SIZE]; /* for /proc/PID/auxv */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Special counters, in some configurations protected by the
// /*AFLA*/ 	 * page_table_lock, in other configurations by being atomic.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct mm_rss_stat rss_stat;
// /*AFLA*/ 
// /*AFLA*/ 	struct linux_binfmt *binfmt;
// /*AFLA*/ 
// /*AFLA*/ 	cpumask_var_t cpu_vm_mask_var;
// /*AFLA*/ 
// /*AFLA*/ 	/* Architecture-specific MM context */
// /*AFLA*/ 	mm_context_t context;
// /*AFLA*/ 
// /*AFLA*/ 	unsigned long flags; /* Must use atomic bitops to access the bits */
// /*AFLA*/ 
// /*AFLA*/ 	struct core_state *core_state; /* coredumping support */
// /*AFLA*/ #ifdef CONFIG_AIO
// /*AFLA*/ 	spinlock_t			ioctx_lock;
// /*AFLA*/ 	struct kioctx_table __rcu	*ioctx_table;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_MEMCG
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * "owner" points to a task that is regarded as the canonical
// /*AFLA*/ 	 * user/owner of this mm. All of the following must be true in
// /*AFLA*/ 	 * order for it to be changed:
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * current == mm->owner
// /*AFLA*/ 	 * current->mm != mm
// /*AFLA*/ 	 * new_owner->mm == mm
// /*AFLA*/ 	 * new_owner->alloc_lock is held
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct task_struct __rcu *owner;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 	/* store ref to file /proc/<pid>/exe symlink points to */
// /*AFLA*/ 	struct file __rcu *exe_file;
// /*AFLA*/ #ifdef CONFIG_MMU_NOTIFIER
// /*AFLA*/ 	struct mmu_notifier_mm *mmu_notifier_mm;
// /*AFLA*/ #endif
// /*AFLA*/ #if defined(CONFIG_TRANSPARENT_HUGEPAGE) && !USE_SPLIT_PMD_PTLOCKS
// /*AFLA*/ 	pgtable_t pmd_huge_pte; /* protected by page_table_lock */
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_CPUMASK_OFFSTACK
// /*AFLA*/ 	struct cpumask cpumask_allocation;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_NUMA_BALANCING
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * numa_next_scan is the next time that the PTEs will be marked
// /*AFLA*/ 	 * pte_numa. NUMA hinting faults will gather statistics and migrate
// /*AFLA*/ 	 * pages to new nodes if necessary.
// /*AFLA*/ 	 */
// /*AFLA*/ 	unsigned long numa_next_scan;
// /*AFLA*/ 
// /*AFLA*/ 	/* Restart point for scanning and setting pte_numa */
// /*AFLA*/ 	unsigned long numa_scan_offset;
// /*AFLA*/ 
// /*AFLA*/ 	/* numa_scan_seq prevents two threads setting pte_numa */
// /*AFLA*/ 	int numa_scan_seq;
// /*AFLA*/ #endif
// /*AFLA*/ #if defined(CONFIG_NUMA_BALANCING) || defined(CONFIG_COMPACTION)
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * An operation with batched TLB flushing is going on. Anything that
// /*AFLA*/ 	 * can move process memory needs to flush the TLB when moving a
// /*AFLA*/ 	 * PROT_NONE or PROT_NUMA mapped page.
// /*AFLA*/ 	 */
// /*AFLA*/ 	bool tlb_flush_pending;
// /*AFLA*/ #endif
// /*AFLA*/ 	struct uprobes_state uprobes_state;
// /*AFLA*/ #ifdef CONFIG_X86_INTEL_MPX
// /*AFLA*/ 	/* address of the bounds directory */
// /*AFLA*/ 	void __user *bd_addr;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_HUGETLB_PAGE
// /*AFLA*/ 	atomic_long_t hugetlb_usage;
// /*AFLA*/ #endif
// /*AFLA*/ 	struct work_struct async_put_work;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ static inline void mm_init_cpumask(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_CPUMASK_OFFSTACK
// /*AFLA*/ 	mm->cpu_vm_mask_var = &mm->cpumask_allocation;
// /*AFLA*/ #endif
// /*AFLA*/ 	cpumask_clear(mm->cpu_vm_mask_var);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Future-safe accessor for struct mm_struct's cpu_vm_mask. */
// /*AFLA*/ static inline cpumask_t *mm_cpumask(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	return mm->cpu_vm_mask_var;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_NUMA_BALANCING) || defined(CONFIG_COMPACTION)
// /*AFLA*/ /*
// /*AFLA*/  * Memory barriers to keep this state in sync are graciously provided by
// /*AFLA*/  * the page table locks, outside of which no page table modifications happen.
// /*AFLA*/  * The barriers below prevent the compiler from re-ordering the instructions
// /*AFLA*/  * around the memory barriers that are already present in the code.
// /*AFLA*/  */
// /*AFLA*/ static inline bool mm_tlb_flush_pending(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	barrier();
// /*AFLA*/ 	return mm->tlb_flush_pending;
// /*AFLA*/ }
// /*AFLA*/ static inline void set_tlb_flush_pending(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	mm->tlb_flush_pending = true;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Guarantee that the tlb_flush_pending store does not leak into the
// /*AFLA*/ 	 * critical section updating the page tables
// /*AFLA*/ 	 */
// /*AFLA*/ 	smp_mb__before_spinlock();
// /*AFLA*/ }
// /*AFLA*/ /* Clearing is done after a TLB flush, which also provides a barrier. */
// /*AFLA*/ static inline void clear_tlb_flush_pending(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	barrier();
// /*AFLA*/ 	mm->tlb_flush_pending = false;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline bool mm_tlb_flush_pending(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ static inline void set_tlb_flush_pending(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void clear_tlb_flush_pending(struct mm_struct *mm)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct vm_fault;
// /*AFLA*/ 
// /*AFLA*/ struct vm_special_mapping {
// /*AFLA*/ 	const char *name;	/* The name, e.g. "[vdso]". */
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If .fault is not provided, this points to a
// /*AFLA*/ 	 * NULL-terminated array of pages that back the special mapping.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * This must not be NULL unless .fault is provided.
// /*AFLA*/ 	 */
// /*AFLA*/ 	struct page **pages;
// /*AFLA*/ 
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * If non-NULL, then this is called to resolve page faults
// /*AFLA*/ 	 * on the special mapping.  If used, .pages is not checked.
// /*AFLA*/ 	 */
// /*AFLA*/ 	int (*fault)(const struct vm_special_mapping *sm,
// /*AFLA*/ 		     struct vm_area_struct *vma,
// /*AFLA*/ 		     struct vm_fault *vmf);
// /*AFLA*/ 
// /*AFLA*/ 	int (*mremap)(const struct vm_special_mapping *sm,
// /*AFLA*/ 		     struct vm_area_struct *new_vma);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ enum tlb_flush_reason {
// /*AFLA*/ 	TLB_FLUSH_ON_TASK_SWITCH,
// /*AFLA*/ 	TLB_REMOTE_SHOOTDOWN,
// /*AFLA*/ 	TLB_LOCAL_SHOOTDOWN,
// /*AFLA*/ 	TLB_LOCAL_MM_SHOOTDOWN,
// /*AFLA*/ 	TLB_REMOTE_SEND_IPI,
// /*AFLA*/ 	NR_TLB_FLUSH_REASONS,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/  /*
// /*AFLA*/   * A swap entry has to fit into a "unsigned long", as the entry is hidden
// /*AFLA*/   * in the "index" field of the swapper address space.
// /*AFLA*/   */
// /*AFLA*/ typedef struct {
// /*AFLA*/ 	unsigned long val;
// /*AFLA*/ } swp_entry_t;
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_MM_TYPES_H */
