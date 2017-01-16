// /*AFLA*/ /*
// /*AFLA*/  * Macros for manipulating and testing page->flags
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef PAGE_FLAGS_H
// /*AFLA*/ #define PAGE_FLAGS_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/bug.h>
// /*AFLA*/ #include <linux/mmdebug.h>
// /*AFLA*/ #ifndef __GENERATING_BOUNDS_H
// /*AFLA*/ #include <linux/mm_types.h>
// /*AFLA*/ #include <generated/bounds.h>
// /*AFLA*/ #endif /* !__GENERATING_BOUNDS_H */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Various page->flags bits:
// /*AFLA*/  *
// /*AFLA*/  * PG_reserved is set for special pages, which can never be swapped out. Some
// /*AFLA*/  * of them might not even exist (eg empty_bad_page)...
// /*AFLA*/  *
// /*AFLA*/  * The PG_private bitflag is set on pagecache pages if they contain filesystem
// /*AFLA*/  * specific data (which is normally at page->private). It can be used by
// /*AFLA*/  * private allocations for its own usage.
// /*AFLA*/  *
// /*AFLA*/  * During initiation of disk I/O, PG_locked is set. This bit is set before I/O
// /*AFLA*/  * and cleared when writeback _starts_ or when read _completes_. PG_writeback
// /*AFLA*/  * is set before writeback starts and cleared when it finishes.
// /*AFLA*/  *
// /*AFLA*/  * PG_locked also pins a page in pagecache, and blocks truncation of the file
// /*AFLA*/  * while it is held.
// /*AFLA*/  *
// /*AFLA*/  * page_waitqueue(page) is a wait queue of all tasks waiting for the page
// /*AFLA*/  * to become unlocked.
// /*AFLA*/  *
// /*AFLA*/  * PG_uptodate tells whether the page's contents is valid.  When a read
// /*AFLA*/  * completes, the page becomes uptodate, unless a disk I/O error happened.
// /*AFLA*/  *
// /*AFLA*/  * PG_referenced, PG_reclaim are used for page reclaim for anonymous and
// /*AFLA*/  * file-backed pagecache (see mm/vmscan.c).
// /*AFLA*/  *
// /*AFLA*/  * PG_error is set to indicate that an I/O error occurred on this page.
// /*AFLA*/  *
// /*AFLA*/  * PG_arch_1 is an architecture specific page state bit.  The generic code
// /*AFLA*/  * guarantees that this bit is cleared for a page when it first is entered into
// /*AFLA*/  * the page cache.
// /*AFLA*/  *
// /*AFLA*/  * PG_highmem pages are not permanently mapped into the kernel virtual address
// /*AFLA*/  * space, they need to be kmapped separately for doing IO on the pages.  The
// /*AFLA*/  * struct page (these bits with information) are always mapped into kernel
// /*AFLA*/  * address space...
// /*AFLA*/  *
// /*AFLA*/  * PG_hwpoison indicates that a page got corrupted in hardware and contains
// /*AFLA*/  * data with incorrect ECC bits that triggered a machine check. Accessing is
// /*AFLA*/  * not safe since it may cause another machine check. Don't touch!
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Don't use the *_dontuse flags.  Use the macros.  Otherwise you'll break
// /*AFLA*/  * locked- and dirty-page accounting.
// /*AFLA*/  *
// /*AFLA*/  * The page flags field is split into two parts, the main flags area
// /*AFLA*/  * which extends from the low bits upwards, and the fields area which
// /*AFLA*/  * extends from the high bits downwards.
// /*AFLA*/  *
// /*AFLA*/  *  | FIELD | ... | FLAGS |
// /*AFLA*/  *  N-1           ^       0
// /*AFLA*/  *               (NR_PAGEFLAGS)
// /*AFLA*/  *
// /*AFLA*/  * The fields area is reserved for fields mapping zone, node (for NUMA) and
// /*AFLA*/  * SPARSEMEM section (for variants of SPARSEMEM that require section ids like
// /*AFLA*/  * SPARSEMEM_EXTREME with !SPARSEMEM_VMEMMAP).
// /*AFLA*/  */
// /*AFLA*/ enum pageflags {
// /*AFLA*/ 	PG_locked,		/* Page is locked. Don't touch. */
// /*AFLA*/ 	PG_error,
// /*AFLA*/ 	PG_referenced,
// /*AFLA*/ 	PG_uptodate,
// /*AFLA*/ 	PG_dirty,
// /*AFLA*/ 	PG_lru,
// /*AFLA*/ 	PG_active,
// /*AFLA*/ 	PG_slab,
// /*AFLA*/ 	PG_owner_priv_1,	/* Owner use. If pagecache, fs may use*/
// /*AFLA*/ 	PG_arch_1,
// /*AFLA*/ 	PG_reserved,
// /*AFLA*/ 	PG_private,		/* If pagecache, has fs-private data */
// /*AFLA*/ 	PG_private_2,		/* If pagecache, has fs aux data */
// /*AFLA*/ 	PG_writeback,		/* Page is under writeback */
// /*AFLA*/ 	PG_head,		/* A head page */
// /*AFLA*/ 	PG_swapcache,		/* Swap page: swp_entry_t in private */
// /*AFLA*/ 	PG_mappedtodisk,	/* Has blocks allocated on-disk */
// /*AFLA*/ 	PG_reclaim,		/* To be reclaimed asap */
// /*AFLA*/ 	PG_swapbacked,		/* Page is backed by RAM/swap */
// /*AFLA*/ 	PG_unevictable,		/* Page is "unevictable"  */
// /*AFLA*/ #ifdef CONFIG_MMU
// /*AFLA*/ 	PG_mlocked,		/* Page is vma mlocked */
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_ARCH_USES_PG_UNCACHED
// /*AFLA*/ 	PG_uncached,		/* Page has been mapped as uncached */
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_MEMORY_FAILURE
// /*AFLA*/ 	PG_hwpoison,		/* hardware poisoned page. Don't touch */
// /*AFLA*/ #endif
// /*AFLA*/ #if defined(CONFIG_IDLE_PAGE_TRACKING) && defined(CONFIG_64BIT)
// /*AFLA*/ 	PG_young,
// /*AFLA*/ 	PG_idle,
// /*AFLA*/ #endif
// /*AFLA*/ 	__NR_PAGEFLAGS,
// /*AFLA*/ 
// /*AFLA*/ 	/* Filesystems */
// /*AFLA*/ 	PG_checked = PG_owner_priv_1,
// /*AFLA*/ 
// /*AFLA*/ 	/* Two page bits are conscripted by FS-Cache to maintain local caching
// /*AFLA*/ 	 * state.  These bits are set on pages belonging to the netfs's inodes
// /*AFLA*/ 	 * when those inodes are being locally cached.
// /*AFLA*/ 	 */
// /*AFLA*/ 	PG_fscache = PG_private_2,	/* page backed by cache */
// /*AFLA*/ 
// /*AFLA*/ 	/* XEN */
// /*AFLA*/ 	/* Pinned in Xen as a read-only pagetable page. */
// /*AFLA*/ 	PG_pinned = PG_owner_priv_1,
// /*AFLA*/ 	/* Pinned as part of domain save (see xen_mm_pin_all()). */
// /*AFLA*/ 	PG_savepinned = PG_dirty,
// /*AFLA*/ 	/* Has a grant mapping of another (foreign) domain's page. */
// /*AFLA*/ 	PG_foreign = PG_owner_priv_1,
// /*AFLA*/ 
// /*AFLA*/ 	/* SLOB */
// /*AFLA*/ 	PG_slob_free = PG_private,
// /*AFLA*/ 
// /*AFLA*/ 	/* Compound pages. Stored in first tail page's flags */
// /*AFLA*/ 	PG_double_map = PG_private_2,
// /*AFLA*/ 
// /*AFLA*/ 	/* non-lru isolated movable page */
// /*AFLA*/ 	PG_isolated = PG_reclaim,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifndef __GENERATING_BOUNDS_H
// /*AFLA*/ 
// /*AFLA*/ struct page;	/* forward declaration */
// /*AFLA*/ 
// /*AFLA*/ static inline struct page *compound_head(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long head = READ_ONCE(page->compound_head);
// /*AFLA*/ 
// /*AFLA*/ 	if (unlikely(head & 1))
// /*AFLA*/ 		return (struct page *) (head - 1);
// /*AFLA*/ 	return page;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline int PageTail(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return READ_ONCE(page->compound_head) & 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline int PageCompound(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return test_bit(PG_head, &page->flags) || PageTail(page);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Page flags policies wrt compound pages
// /*AFLA*/  *
// /*AFLA*/  * PF_ANY:
// /*AFLA*/  *     the page flag is relevant for small, head and tail pages.
// /*AFLA*/  *
// /*AFLA*/  * PF_HEAD:
// /*AFLA*/  *     for compound page all operations related to the page flag applied to
// /*AFLA*/  *     head page.
// /*AFLA*/  *
// /*AFLA*/  * PF_NO_TAIL:
// /*AFLA*/  *     modifications of the page flag must be done on small or head pages,
// /*AFLA*/  *     checks can be done on tail pages too.
// /*AFLA*/  *
// /*AFLA*/  * PF_NO_COMPOUND:
// /*AFLA*/  *     the page flag is not relevant for compound pages.
// /*AFLA*/  */
// /*AFLA*/ #define PF_ANY(page, enforce)	page
// /*AFLA*/ #define PF_HEAD(page, enforce)	compound_head(page)
// /*AFLA*/ #define PF_NO_TAIL(page, enforce) ({					\
// /*AFLA*/ 		VM_BUG_ON_PGFLAGS(enforce && PageTail(page), page);	\
// /*AFLA*/ 		compound_head(page);})
// /*AFLA*/ #define PF_NO_COMPOUND(page, enforce) ({				\
// /*AFLA*/ 		VM_BUG_ON_PGFLAGS(enforce && PageCompound(page), page);	\
// /*AFLA*/ 		page;})
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Macros to create function definitions for page flags
// /*AFLA*/  */
// /*AFLA*/ #define TESTPAGEFLAG(uname, lname, policy)				\
// /*AFLA*/ static __always_inline int Page##uname(struct page *page)		\
// /*AFLA*/ 	{ return test_bit(PG_##lname, &policy(page, 0)->flags); }
// /*AFLA*/ 
// /*AFLA*/ #define SETPAGEFLAG(uname, lname, policy)				\
// /*AFLA*/ static __always_inline void SetPage##uname(struct page *page)		\
// /*AFLA*/ 	{ set_bit(PG_##lname, &policy(page, 1)->flags); }
// /*AFLA*/ 
// /*AFLA*/ #define CLEARPAGEFLAG(uname, lname, policy)				\
// /*AFLA*/ static __always_inline void ClearPage##uname(struct page *page)		\
// /*AFLA*/ 	{ clear_bit(PG_##lname, &policy(page, 1)->flags); }
// /*AFLA*/ 
// /*AFLA*/ #define __SETPAGEFLAG(uname, lname, policy)				\
// /*AFLA*/ static __always_inline void __SetPage##uname(struct page *page)		\
// /*AFLA*/ 	{ __set_bit(PG_##lname, &policy(page, 1)->flags); }
// /*AFLA*/ 
// /*AFLA*/ #define __CLEARPAGEFLAG(uname, lname, policy)				\
// /*AFLA*/ static __always_inline void __ClearPage##uname(struct page *page)	\
// /*AFLA*/ 	{ __clear_bit(PG_##lname, &policy(page, 1)->flags); }
// /*AFLA*/ 
// /*AFLA*/ #define TESTSETFLAG(uname, lname, policy)				\
// /*AFLA*/ static __always_inline int TestSetPage##uname(struct page *page)	\
// /*AFLA*/ 	{ return test_and_set_bit(PG_##lname, &policy(page, 1)->flags); }
// /*AFLA*/ 
// /*AFLA*/ #define TESTCLEARFLAG(uname, lname, policy)				\
// /*AFLA*/ static __always_inline int TestClearPage##uname(struct page *page)	\
// /*AFLA*/ 	{ return test_and_clear_bit(PG_##lname, &policy(page, 1)->flags); }
// /*AFLA*/ 
// /*AFLA*/ #define PAGEFLAG(uname, lname, policy)					\
// /*AFLA*/ 	TESTPAGEFLAG(uname, lname, policy)				\
// /*AFLA*/ 	SETPAGEFLAG(uname, lname, policy)				\
// /*AFLA*/ 	CLEARPAGEFLAG(uname, lname, policy)
// /*AFLA*/ 
// /*AFLA*/ #define __PAGEFLAG(uname, lname, policy)				\
// /*AFLA*/ 	TESTPAGEFLAG(uname, lname, policy)				\
// /*AFLA*/ 	__SETPAGEFLAG(uname, lname, policy)				\
// /*AFLA*/ 	__CLEARPAGEFLAG(uname, lname, policy)
// /*AFLA*/ 
// /*AFLA*/ #define TESTSCFLAG(uname, lname, policy)				\
// /*AFLA*/ 	TESTSETFLAG(uname, lname, policy)				\
// /*AFLA*/ 	TESTCLEARFLAG(uname, lname, policy)
// /*AFLA*/ 
// /*AFLA*/ #define TESTPAGEFLAG_FALSE(uname)					\
// /*AFLA*/ static inline int Page##uname(const struct page *page) { return 0; }
// /*AFLA*/ 
// /*AFLA*/ #define SETPAGEFLAG_NOOP(uname)						\
// /*AFLA*/ static inline void SetPage##uname(struct page *page) {  }
// /*AFLA*/ 
// /*AFLA*/ #define CLEARPAGEFLAG_NOOP(uname)					\
// /*AFLA*/ static inline void ClearPage##uname(struct page *page) {  }
// /*AFLA*/ 
// /*AFLA*/ #define __CLEARPAGEFLAG_NOOP(uname)					\
// /*AFLA*/ static inline void __ClearPage##uname(struct page *page) {  }
// /*AFLA*/ 
// /*AFLA*/ #define TESTSETFLAG_FALSE(uname)					\
// /*AFLA*/ static inline int TestSetPage##uname(struct page *page) { return 0; }
// /*AFLA*/ 
// /*AFLA*/ #define TESTCLEARFLAG_FALSE(uname)					\
// /*AFLA*/ static inline int TestClearPage##uname(struct page *page) { return 0; }
// /*AFLA*/ 
// /*AFLA*/ #define PAGEFLAG_FALSE(uname) TESTPAGEFLAG_FALSE(uname)			\
// /*AFLA*/ 	SETPAGEFLAG_NOOP(uname) CLEARPAGEFLAG_NOOP(uname)
// /*AFLA*/ 
// /*AFLA*/ #define TESTSCFLAG_FALSE(uname)						\
// /*AFLA*/ 	TESTSETFLAG_FALSE(uname) TESTCLEARFLAG_FALSE(uname)
// /*AFLA*/ 
// /*AFLA*/ __PAGEFLAG(Locked, locked, PF_NO_TAIL)
// /*AFLA*/ PAGEFLAG(Error, error, PF_NO_COMPOUND) TESTCLEARFLAG(Error, error, PF_NO_COMPOUND)
// /*AFLA*/ PAGEFLAG(Referenced, referenced, PF_HEAD)
// /*AFLA*/ 	TESTCLEARFLAG(Referenced, referenced, PF_HEAD)
// /*AFLA*/ 	__SETPAGEFLAG(Referenced, referenced, PF_HEAD)
// /*AFLA*/ PAGEFLAG(Dirty, dirty, PF_HEAD) TESTSCFLAG(Dirty, dirty, PF_HEAD)
// /*AFLA*/ 	__CLEARPAGEFLAG(Dirty, dirty, PF_HEAD)
// /*AFLA*/ PAGEFLAG(LRU, lru, PF_HEAD) __CLEARPAGEFLAG(LRU, lru, PF_HEAD)
// /*AFLA*/ PAGEFLAG(Active, active, PF_HEAD) __CLEARPAGEFLAG(Active, active, PF_HEAD)
// /*AFLA*/ 	TESTCLEARFLAG(Active, active, PF_HEAD)
// /*AFLA*/ __PAGEFLAG(Slab, slab, PF_NO_TAIL)
// /*AFLA*/ __PAGEFLAG(SlobFree, slob_free, PF_NO_TAIL)
// /*AFLA*/ PAGEFLAG(Checked, checked, PF_NO_COMPOUND)	   /* Used by some filesystems */
// /*AFLA*/ 
// /*AFLA*/ /* Xen */
// /*AFLA*/ PAGEFLAG(Pinned, pinned, PF_NO_COMPOUND)
// /*AFLA*/ 	TESTSCFLAG(Pinned, pinned, PF_NO_COMPOUND)
// /*AFLA*/ PAGEFLAG(SavePinned, savepinned, PF_NO_COMPOUND);
// /*AFLA*/ PAGEFLAG(Foreign, foreign, PF_NO_COMPOUND);
// /*AFLA*/ 
// /*AFLA*/ PAGEFLAG(Reserved, reserved, PF_NO_COMPOUND)
// /*AFLA*/ 	__CLEARPAGEFLAG(Reserved, reserved, PF_NO_COMPOUND)
// /*AFLA*/ PAGEFLAG(SwapBacked, swapbacked, PF_NO_TAIL)
// /*AFLA*/ 	__CLEARPAGEFLAG(SwapBacked, swapbacked, PF_NO_TAIL)
// /*AFLA*/ 	__SETPAGEFLAG(SwapBacked, swapbacked, PF_NO_TAIL)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Private page markings that may be used by the filesystem that owns the page
// /*AFLA*/  * for its own purposes.
// /*AFLA*/  * - PG_private and PG_private_2 cause releasepage() and co to be invoked
// /*AFLA*/  */
// /*AFLA*/ PAGEFLAG(Private, private, PF_ANY) __SETPAGEFLAG(Private, private, PF_ANY)
// /*AFLA*/ 	__CLEARPAGEFLAG(Private, private, PF_ANY)
// /*AFLA*/ PAGEFLAG(Private2, private_2, PF_ANY) TESTSCFLAG(Private2, private_2, PF_ANY)
// /*AFLA*/ PAGEFLAG(OwnerPriv1, owner_priv_1, PF_ANY)
// /*AFLA*/ 	TESTCLEARFLAG(OwnerPriv1, owner_priv_1, PF_ANY)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Only test-and-set exist for PG_writeback.  The unconditional operators are
// /*AFLA*/  * risky: they bypass page accounting.
// /*AFLA*/  */
// /*AFLA*/ TESTPAGEFLAG(Writeback, writeback, PF_NO_COMPOUND)
// /*AFLA*/ 	TESTSCFLAG(Writeback, writeback, PF_NO_COMPOUND)
// /*AFLA*/ PAGEFLAG(MappedToDisk, mappedtodisk, PF_NO_TAIL)
// /*AFLA*/ 
// /*AFLA*/ /* PG_readahead is only used for reads; PG_reclaim is only for writes */
// /*AFLA*/ PAGEFLAG(Reclaim, reclaim, PF_NO_TAIL)
// /*AFLA*/ 	TESTCLEARFLAG(Reclaim, reclaim, PF_NO_TAIL)
// /*AFLA*/ PAGEFLAG(Readahead, reclaim, PF_NO_COMPOUND)
// /*AFLA*/ 	TESTCLEARFLAG(Readahead, reclaim, PF_NO_COMPOUND)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HIGHMEM
// /*AFLA*/ /*
// /*AFLA*/  * Must use a macro here due to header dependency issues. page_zone() is not
// /*AFLA*/  * available at this point.
// /*AFLA*/  */
// /*AFLA*/ #define PageHighMem(__p) is_highmem_idx(page_zonenum(__p))
// /*AFLA*/ #else
// /*AFLA*/ PAGEFLAG_FALSE(HighMem)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SWAP
// /*AFLA*/ PAGEFLAG(SwapCache, swapcache, PF_NO_COMPOUND)
// /*AFLA*/ #else
// /*AFLA*/ PAGEFLAG_FALSE(SwapCache)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ PAGEFLAG(Unevictable, unevictable, PF_HEAD)
// /*AFLA*/ 	__CLEARPAGEFLAG(Unevictable, unevictable, PF_HEAD)
// /*AFLA*/ 	TESTCLEARFLAG(Unevictable, unevictable, PF_HEAD)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MMU
// /*AFLA*/ PAGEFLAG(Mlocked, mlocked, PF_NO_TAIL)
// /*AFLA*/ 	__CLEARPAGEFLAG(Mlocked, mlocked, PF_NO_TAIL)
// /*AFLA*/ 	TESTSCFLAG(Mlocked, mlocked, PF_NO_TAIL)
// /*AFLA*/ #else
// /*AFLA*/ PAGEFLAG_FALSE(Mlocked) __CLEARPAGEFLAG_NOOP(Mlocked)
// /*AFLA*/ 	TESTSCFLAG_FALSE(Mlocked)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_ARCH_USES_PG_UNCACHED
// /*AFLA*/ PAGEFLAG(Uncached, uncached, PF_NO_COMPOUND)
// /*AFLA*/ #else
// /*AFLA*/ PAGEFLAG_FALSE(Uncached)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MEMORY_FAILURE
// /*AFLA*/ PAGEFLAG(HWPoison, hwpoison, PF_ANY)
// /*AFLA*/ TESTSCFLAG(HWPoison, hwpoison, PF_ANY)
// /*AFLA*/ #define __PG_HWPOISON (1UL << PG_hwpoison)
// /*AFLA*/ #else
// /*AFLA*/ PAGEFLAG_FALSE(HWPoison)
// /*AFLA*/ #define __PG_HWPOISON 0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_IDLE_PAGE_TRACKING) && defined(CONFIG_64BIT)
// /*AFLA*/ TESTPAGEFLAG(Young, young, PF_ANY)
// /*AFLA*/ SETPAGEFLAG(Young, young, PF_ANY)
// /*AFLA*/ TESTCLEARFLAG(Young, young, PF_ANY)
// /*AFLA*/ PAGEFLAG(Idle, idle, PF_ANY)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * On an anonymous page mapped into a user virtual memory area,
// /*AFLA*/  * page->mapping points to its anon_vma, not to a struct address_space;
// /*AFLA*/  * with the PAGE_MAPPING_ANON bit set to distinguish it.  See rmap.h.
// /*AFLA*/  *
// /*AFLA*/  * On an anonymous page in a VM_MERGEABLE area, if CONFIG_KSM is enabled,
// /*AFLA*/  * the PAGE_MAPPING_MOVABLE bit may be set along with the PAGE_MAPPING_ANON
// /*AFLA*/  * bit; and then page->mapping points, not to an anon_vma, but to a private
// /*AFLA*/  * structure which KSM associates with that merged page.  See ksm.h.
// /*AFLA*/  *
// /*AFLA*/  * PAGE_MAPPING_KSM without PAGE_MAPPING_ANON is used for non-lru movable
// /*AFLA*/  * page and then page->mapping points a struct address_space.
// /*AFLA*/  *
// /*AFLA*/  * Please note that, confusingly, "page_mapping" refers to the inode
// /*AFLA*/  * address_space which maps the page from disk; whereas "page_mapped"
// /*AFLA*/  * refers to user virtual address space into which the page is mapped.
// /*AFLA*/  */
// /*AFLA*/ #define PAGE_MAPPING_ANON	0x1
// /*AFLA*/ #define PAGE_MAPPING_MOVABLE	0x2
// /*AFLA*/ #define PAGE_MAPPING_KSM	(PAGE_MAPPING_ANON | PAGE_MAPPING_MOVABLE)
// /*AFLA*/ #define PAGE_MAPPING_FLAGS	(PAGE_MAPPING_ANON | PAGE_MAPPING_MOVABLE)
// /*AFLA*/ 
// /*AFLA*/ static __always_inline int PageMappingFlags(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return ((unsigned long)page->mapping & PAGE_MAPPING_FLAGS) != 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline int PageAnon(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	page = compound_head(page);
// /*AFLA*/ 	return ((unsigned long)page->mapping & PAGE_MAPPING_ANON) != 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline int __PageMovable(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return ((unsigned long)page->mapping & PAGE_MAPPING_FLAGS) ==
// /*AFLA*/ 				PAGE_MAPPING_MOVABLE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_KSM
// /*AFLA*/ /*
// /*AFLA*/  * A KSM page is one of those write-protected "shared pages" or "merged pages"
// /*AFLA*/  * which KSM maps into multiple mms, wherever identical anonymous page content
// /*AFLA*/  * is found in VM_MERGEABLE vmas.  It's a PageAnon page, pointing not to any
// /*AFLA*/  * anon_vma, but to that page's node of the stable tree.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline int PageKsm(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	page = compound_head(page);
// /*AFLA*/ 	return ((unsigned long)page->mapping & PAGE_MAPPING_FLAGS) ==
// /*AFLA*/ 				PAGE_MAPPING_KSM;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ TESTPAGEFLAG_FALSE(Ksm)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ u64 stable_page_flags(struct page *page);
// /*AFLA*/ 
// /*AFLA*/ static inline int PageUptodate(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	int ret;
// /*AFLA*/ 	page = compound_head(page);
// /*AFLA*/ 	ret = test_bit(PG_uptodate, &(page)->flags);
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Must ensure that the data we read out of the page is loaded
// /*AFLA*/ 	 * _after_ we've loaded page->flags to check for PageUptodate.
// /*AFLA*/ 	 * We can skip the barrier if the page is not uptodate, because
// /*AFLA*/ 	 * we wouldn't be reading anything from it.
// /*AFLA*/ 	 *
// /*AFLA*/ 	 * See SetPageUptodate() for the other side of the story.
// /*AFLA*/ 	 */
// /*AFLA*/ 	if (ret)
// /*AFLA*/ 		smp_rmb();
// /*AFLA*/ 
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void __SetPageUptodate(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	VM_BUG_ON_PAGE(PageTail(page), page);
// /*AFLA*/ 	smp_wmb();
// /*AFLA*/ 	__set_bit(PG_uptodate, &page->flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void SetPageUptodate(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	VM_BUG_ON_PAGE(PageTail(page), page);
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * Memory barrier must be issued before setting the PG_uptodate bit,
// /*AFLA*/ 	 * so that all previous stores issued in order to bring the page
// /*AFLA*/ 	 * uptodate are actually visible before PageUptodate becomes true.
// /*AFLA*/ 	 */
// /*AFLA*/ 	smp_wmb();
// /*AFLA*/ 	set_bit(PG_uptodate, &page->flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ CLEARPAGEFLAG(Uptodate, uptodate, PF_NO_TAIL)
// /*AFLA*/ 
// /*AFLA*/ int test_clear_page_writeback(struct page *page);
// /*AFLA*/ int __test_set_page_writeback(struct page *page, bool keep_write);
// /*AFLA*/ 
// /*AFLA*/ #define test_set_page_writeback(page)			\
// /*AFLA*/ 	__test_set_page_writeback(page, false)
// /*AFLA*/ #define test_set_page_writeback_keepwrite(page)	\
// /*AFLA*/ 	__test_set_page_writeback(page, true)
// /*AFLA*/ 
// /*AFLA*/ static inline void set_page_writeback(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	test_set_page_writeback(page);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void set_page_writeback_keepwrite(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	test_set_page_writeback_keepwrite(page);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ __PAGEFLAG(Head, head, PF_ANY) CLEARPAGEFLAG(Head, head, PF_ANY)
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void set_compound_head(struct page *page, struct page *head)
// /*AFLA*/ {
// /*AFLA*/ 	WRITE_ONCE(page->compound_head, (unsigned long)head + 1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void clear_compound_head(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	WRITE_ONCE(page->compound_head, 0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_TRANSPARENT_HUGEPAGE
// /*AFLA*/ static inline void ClearPageCompound(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	BUG_ON(!PageHead(page));
// /*AFLA*/ 	ClearPageHead(page);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define PG_head_mask ((1UL << PG_head))
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HUGETLB_PAGE
// /*AFLA*/ int PageHuge(struct page *page);
// /*AFLA*/ int PageHeadHuge(struct page *page);
// /*AFLA*/ bool page_huge_active(struct page *page);
// /*AFLA*/ #else
// /*AFLA*/ TESTPAGEFLAG_FALSE(Huge)
// /*AFLA*/ TESTPAGEFLAG_FALSE(HeadHuge)
// /*AFLA*/ 
// /*AFLA*/ static inline bool page_huge_active(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_TRANSPARENT_HUGEPAGE
// /*AFLA*/ /*
// /*AFLA*/  * PageHuge() only returns true for hugetlbfs pages, but not for
// /*AFLA*/  * normal or transparent huge pages.
// /*AFLA*/  *
// /*AFLA*/  * PageTransHuge() returns true for both transparent huge and
// /*AFLA*/  * hugetlbfs pages, but not normal pages. PageTransHuge() can only be
// /*AFLA*/  * called only in the core VM paths where hugetlbfs pages can't exist.
// /*AFLA*/  */
// /*AFLA*/ static inline int PageTransHuge(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	VM_BUG_ON_PAGE(PageTail(page), page);
// /*AFLA*/ 	return PageHead(page);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * PageTransCompound returns true for both transparent huge pages
// /*AFLA*/  * and hugetlbfs pages, so it should only be called when it's known
// /*AFLA*/  * that hugetlbfs pages aren't involved.
// /*AFLA*/  */
// /*AFLA*/ static inline int PageTransCompound(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return PageCompound(page);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * PageTransCompoundMap is the same as PageTransCompound, but it also
// /*AFLA*/  * guarantees the primary MMU has the entire compound page mapped
// /*AFLA*/  * through pmd_trans_huge, which in turn guarantees the secondary MMUs
// /*AFLA*/  * can also map the entire compound page. This allows the secondary
// /*AFLA*/  * MMUs to call get_user_pages() only once for each compound page and
// /*AFLA*/  * to immediately map the entire compound page with a single secondary
// /*AFLA*/  * MMU fault. If there will be a pmd split later, the secondary MMUs
// /*AFLA*/  * will get an update through the MMU notifier invalidation through
// /*AFLA*/  * split_huge_pmd().
// /*AFLA*/  *
// /*AFLA*/  * Unlike PageTransCompound, this is safe to be called only while
// /*AFLA*/  * split_huge_pmd() cannot run from under us, like if protected by the
// /*AFLA*/  * MMU notifier, otherwise it may result in page->_mapcount < 0 false
// /*AFLA*/  * positives.
// /*AFLA*/  */
// /*AFLA*/ static inline int PageTransCompoundMap(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return PageTransCompound(page) && atomic_read(&page->_mapcount) < 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * PageTransTail returns true for both transparent huge pages
// /*AFLA*/  * and hugetlbfs pages, so it should only be called when it's known
// /*AFLA*/  * that hugetlbfs pages aren't involved.
// /*AFLA*/  */
// /*AFLA*/ static inline int PageTransTail(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return PageTail(page);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * PageDoubleMap indicates that the compound page is mapped with PTEs as well
// /*AFLA*/  * as PMDs.
// /*AFLA*/  *
// /*AFLA*/  * This is required for optimization of rmap operations for THP: we can postpone
// /*AFLA*/  * per small page mapcount accounting (and its overhead from atomic operations)
// /*AFLA*/  * until the first PMD split.
// /*AFLA*/  *
// /*AFLA*/  * For the page PageDoubleMap means ->_mapcount in all sub-pages is offset up
// /*AFLA*/  * by one. This reference will go away with last compound_mapcount.
// /*AFLA*/  *
// /*AFLA*/  * See also __split_huge_pmd_locked() and page_remove_anon_compound_rmap().
// /*AFLA*/  */
// /*AFLA*/ static inline int PageDoubleMap(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return PageHead(page) && test_bit(PG_double_map, &page[1].flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void SetPageDoubleMap(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	VM_BUG_ON_PAGE(!PageHead(page), page);
// /*AFLA*/ 	set_bit(PG_double_map, &page[1].flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void ClearPageDoubleMap(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	VM_BUG_ON_PAGE(!PageHead(page), page);
// /*AFLA*/ 	clear_bit(PG_double_map, &page[1].flags);
// /*AFLA*/ }
// /*AFLA*/ static inline int TestSetPageDoubleMap(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	VM_BUG_ON_PAGE(!PageHead(page), page);
// /*AFLA*/ 	return test_and_set_bit(PG_double_map, &page[1].flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int TestClearPageDoubleMap(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	VM_BUG_ON_PAGE(!PageHead(page), page);
// /*AFLA*/ 	return test_and_clear_bit(PG_double_map, &page[1].flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ TESTPAGEFLAG_FALSE(TransHuge)
// /*AFLA*/ TESTPAGEFLAG_FALSE(TransCompound)
// /*AFLA*/ TESTPAGEFLAG_FALSE(TransCompoundMap)
// /*AFLA*/ TESTPAGEFLAG_FALSE(TransTail)
// /*AFLA*/ PAGEFLAG_FALSE(DoubleMap)
// /*AFLA*/ 	TESTSETFLAG_FALSE(DoubleMap)
// /*AFLA*/ 	TESTCLEARFLAG_FALSE(DoubleMap)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * For pages that are never mapped to userspace, page->mapcount may be
// /*AFLA*/  * used for storing extra information about page type. Any value used
// /*AFLA*/  * for this purpose must be <= -2, but it's better start not too close
// /*AFLA*/  * to -2 so that an underflow of the page_mapcount() won't be mistaken
// /*AFLA*/  * for a special page.
// /*AFLA*/  */
// /*AFLA*/ #define PAGE_MAPCOUNT_OPS(uname, lname)					\
// /*AFLA*/ static __always_inline int Page##uname(struct page *page)		\
// /*AFLA*/ {									\
// /*AFLA*/ 	return atomic_read(&page->_mapcount) ==				\
// /*AFLA*/ 				PAGE_##lname##_MAPCOUNT_VALUE;		\
// /*AFLA*/ }									\
// /*AFLA*/ static __always_inline void __SetPage##uname(struct page *page)		\
// /*AFLA*/ {									\
// /*AFLA*/ 	VM_BUG_ON_PAGE(atomic_read(&page->_mapcount) != -1, page);	\
// /*AFLA*/ 	atomic_set(&page->_mapcount, PAGE_##lname##_MAPCOUNT_VALUE);	\
// /*AFLA*/ }									\
// /*AFLA*/ static __always_inline void __ClearPage##uname(struct page *page)	\
// /*AFLA*/ {									\
// /*AFLA*/ 	VM_BUG_ON_PAGE(!Page##uname(page), page);			\
// /*AFLA*/ 	atomic_set(&page->_mapcount, -1);				\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * PageBuddy() indicate that the page is free and in the buddy system
// /*AFLA*/  * (see mm/page_alloc.c).
// /*AFLA*/  */
// /*AFLA*/ #define PAGE_BUDDY_MAPCOUNT_VALUE		(-128)
// /*AFLA*/ PAGE_MAPCOUNT_OPS(Buddy, BUDDY)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * PageBalloon() is set on pages that are on the balloon page list
// /*AFLA*/  * (see mm/balloon_compaction.c).
// /*AFLA*/  */
// /*AFLA*/ #define PAGE_BALLOON_MAPCOUNT_VALUE		(-256)
// /*AFLA*/ PAGE_MAPCOUNT_OPS(Balloon, BALLOON)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * If kmemcg is enabled, the buddy allocator will set PageKmemcg() on
// /*AFLA*/  * pages allocated with __GFP_ACCOUNT. It gets cleared on page free.
// /*AFLA*/  */
// /*AFLA*/ #define PAGE_KMEMCG_MAPCOUNT_VALUE		(-512)
// /*AFLA*/ PAGE_MAPCOUNT_OPS(Kmemcg, KMEMCG)
// /*AFLA*/ 
// /*AFLA*/ extern bool is_free_buddy_page(struct page *page);
// /*AFLA*/ 
// /*AFLA*/ __PAGEFLAG(Isolated, isolated, PF_ANY);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * If network-based swap is enabled, sl*b must keep track of whether pages
// /*AFLA*/  * were allocated from pfmemalloc reserves.
// /*AFLA*/  */
// /*AFLA*/ static inline int PageSlabPfmemalloc(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	VM_BUG_ON_PAGE(!PageSlab(page), page);
// /*AFLA*/ 	return PageActive(page);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void SetPageSlabPfmemalloc(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	VM_BUG_ON_PAGE(!PageSlab(page), page);
// /*AFLA*/ 	SetPageActive(page);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __ClearPageSlabPfmemalloc(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	VM_BUG_ON_PAGE(!PageSlab(page), page);
// /*AFLA*/ 	__ClearPageActive(page);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void ClearPageSlabPfmemalloc(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	VM_BUG_ON_PAGE(!PageSlab(page), page);
// /*AFLA*/ 	ClearPageActive(page);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_MMU
// /*AFLA*/ #define __PG_MLOCKED		(1UL << PG_mlocked)
// /*AFLA*/ #else
// /*AFLA*/ #define __PG_MLOCKED		0
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Flags checked when a page is freed.  Pages being freed should not have
// /*AFLA*/  * these flags set.  It they are, there is a problem.
// /*AFLA*/  */
// /*AFLA*/ #define PAGE_FLAGS_CHECK_AT_FREE \
// /*AFLA*/ 	(1UL << PG_lru	 | 1UL << PG_locked    | \
// /*AFLA*/ 	 1UL << PG_private | 1UL << PG_private_2 | \
// /*AFLA*/ 	 1UL << PG_writeback | 1UL << PG_reserved | \
// /*AFLA*/ 	 1UL << PG_slab	 | 1UL << PG_swapcache | 1UL << PG_active | \
// /*AFLA*/ 	 1UL << PG_unevictable | __PG_MLOCKED)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Flags checked when a page is prepped for return by the page allocator.
// /*AFLA*/  * Pages being prepped should not have these flags set.  It they are set,
// /*AFLA*/  * there has been a kernel bug or struct page corruption.
// /*AFLA*/  *
// /*AFLA*/  * __PG_HWPOISON is exceptional because it needs to be kept beyond page's
// /*AFLA*/  * alloc-free cycle to prevent from reusing the page.
// /*AFLA*/  */
// /*AFLA*/ #define PAGE_FLAGS_CHECK_AT_PREP	\
// /*AFLA*/ 	(((1UL << NR_PAGEFLAGS) - 1) & ~__PG_HWPOISON)
// /*AFLA*/ 
// /*AFLA*/ #define PAGE_FLAGS_PRIVATE				\
// /*AFLA*/ 	(1UL << PG_private | 1UL << PG_private_2)
// /*AFLA*/ /**
// /*AFLA*/  * page_has_private - Determine if page has private stuff
// /*AFLA*/  * @page: The page to be checked
// /*AFLA*/  *
// /*AFLA*/  * Determine if a page has private stuff, indicating that release routines
// /*AFLA*/  * should be invoked upon it.
// /*AFLA*/  */
// /*AFLA*/ static inline int page_has_private(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return !!(page->flags & PAGE_FLAGS_PRIVATE);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #undef PF_ANY
// /*AFLA*/ #undef PF_HEAD
// /*AFLA*/ #undef PF_NO_TAIL
// /*AFLA*/ #undef PF_NO_COMPOUND
// /*AFLA*/ #endif /* !__GENERATING_BOUNDS_H */
// /*AFLA*/ 
// /*AFLA*/ #endif	/* PAGE_FLAGS_H */
