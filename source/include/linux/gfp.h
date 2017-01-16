// /*AFLA*/ #ifndef __LINUX_GFP_H
// /*AFLA*/ #define __LINUX_GFP_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/mmdebug.h>
// /*AFLA*/ #include <linux/mmzone.h>
// /*AFLA*/ #include <linux/stddef.h>
// /*AFLA*/ #include <linux/linkage.h>
// /*AFLA*/ #include <linux/topology.h>
// /*AFLA*/ 
// /*AFLA*/ struct vm_area_struct;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * In case of changes, please don't forget to update
// /*AFLA*/  * include/trace/events/mmflags.h and tools/perf/builtin-kmem.c
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /* Plain integer GFP bitmasks. Do not use this directly. */
// /*AFLA*/ #define ___GFP_DMA		0x01u
// /*AFLA*/ #define ___GFP_HIGHMEM		0x02u
// /*AFLA*/ #define ___GFP_DMA32		0x04u
// /*AFLA*/ #define ___GFP_MOVABLE		0x08u
// /*AFLA*/ #define ___GFP_RECLAIMABLE	0x10u
// /*AFLA*/ #define ___GFP_HIGH		0x20u
// /*AFLA*/ #define ___GFP_IO		0x40u
// /*AFLA*/ #define ___GFP_FS		0x80u
// /*AFLA*/ #define ___GFP_COLD		0x100u
// /*AFLA*/ #define ___GFP_NOWARN		0x200u
// /*AFLA*/ #define ___GFP_REPEAT		0x400u
// /*AFLA*/ #define ___GFP_NOFAIL		0x800u
// /*AFLA*/ #define ___GFP_NORETRY		0x1000u
// /*AFLA*/ #define ___GFP_MEMALLOC		0x2000u
// /*AFLA*/ #define ___GFP_COMP		0x4000u
// /*AFLA*/ #define ___GFP_ZERO		0x8000u
// /*AFLA*/ #define ___GFP_NOMEMALLOC	0x10000u
// /*AFLA*/ #define ___GFP_HARDWALL		0x20000u
// /*AFLA*/ #define ___GFP_THISNODE		0x40000u
// /*AFLA*/ #define ___GFP_ATOMIC		0x80000u
// /*AFLA*/ #define ___GFP_ACCOUNT		0x100000u
// /*AFLA*/ #define ___GFP_NOTRACK		0x200000u
// /*AFLA*/ #define ___GFP_DIRECT_RECLAIM	0x400000u
// /*AFLA*/ #define ___GFP_OTHER_NODE	0x800000u
// /*AFLA*/ #define ___GFP_WRITE		0x1000000u
// /*AFLA*/ #define ___GFP_KSWAPD_RECLAIM	0x2000000u
// /*AFLA*/ /* If the above are modified, __GFP_BITS_SHIFT may need updating */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Physical address zone modifiers (see linux/mmzone.h - low four bits)
// /*AFLA*/  *
// /*AFLA*/  * Do not put any conditional on these. If necessary modify the definitions
// /*AFLA*/  * without the underscores and use them consistently. The definitions here may
// /*AFLA*/  * be used in bit comparisons.
// /*AFLA*/  */
// /*AFLA*/ #define __GFP_DMA	((__force gfp_t)___GFP_DMA)
// /*AFLA*/ #define __GFP_HIGHMEM	((__force gfp_t)___GFP_HIGHMEM)
// /*AFLA*/ #define __GFP_DMA32	((__force gfp_t)___GFP_DMA32)
// /*AFLA*/ #define __GFP_MOVABLE	((__force gfp_t)___GFP_MOVABLE)  /* ZONE_MOVABLE allowed */
// /*AFLA*/ #define GFP_ZONEMASK	(__GFP_DMA|__GFP_HIGHMEM|__GFP_DMA32|__GFP_MOVABLE)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Page mobility and placement hints
// /*AFLA*/  *
// /*AFLA*/  * These flags provide hints about how mobile the page is. Pages with similar
// /*AFLA*/  * mobility are placed within the same pageblocks to minimise problems due
// /*AFLA*/  * to external fragmentation.
// /*AFLA*/  *
// /*AFLA*/  * __GFP_MOVABLE (also a zone modifier) indicates that the page can be
// /*AFLA*/  *   moved by page migration during memory compaction or can be reclaimed.
// /*AFLA*/  *
// /*AFLA*/  * __GFP_RECLAIMABLE is used for slab allocations that specify
// /*AFLA*/  *   SLAB_RECLAIM_ACCOUNT and whose pages can be freed via shrinkers.
// /*AFLA*/  *
// /*AFLA*/  * __GFP_WRITE indicates the caller intends to dirty the page. Where possible,
// /*AFLA*/  *   these pages will be spread between local zones to avoid all the dirty
// /*AFLA*/  *   pages being in one zone (fair zone allocation policy).
// /*AFLA*/  *
// /*AFLA*/  * __GFP_HARDWALL enforces the cpuset memory allocation policy.
// /*AFLA*/  *
// /*AFLA*/  * __GFP_THISNODE forces the allocation to be satisified from the requested
// /*AFLA*/  *   node with no fallbacks or placement policy enforcements.
// /*AFLA*/  *
// /*AFLA*/  * __GFP_ACCOUNT causes the allocation to be accounted to kmemcg.
// /*AFLA*/  */
// /*AFLA*/ #define __GFP_RECLAIMABLE ((__force gfp_t)___GFP_RECLAIMABLE)
// /*AFLA*/ #define __GFP_WRITE	((__force gfp_t)___GFP_WRITE)
// /*AFLA*/ #define __GFP_HARDWALL   ((__force gfp_t)___GFP_HARDWALL)
// /*AFLA*/ #define __GFP_THISNODE	((__force gfp_t)___GFP_THISNODE)
// /*AFLA*/ #define __GFP_ACCOUNT	((__force gfp_t)___GFP_ACCOUNT)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Watermark modifiers -- controls access to emergency reserves
// /*AFLA*/  *
// /*AFLA*/  * __GFP_HIGH indicates that the caller is high-priority and that granting
// /*AFLA*/  *   the request is necessary before the system can make forward progress.
// /*AFLA*/  *   For example, creating an IO context to clean pages.
// /*AFLA*/  *
// /*AFLA*/  * __GFP_ATOMIC indicates that the caller cannot reclaim or sleep and is
// /*AFLA*/  *   high priority. Users are typically interrupt handlers. This may be
// /*AFLA*/  *   used in conjunction with __GFP_HIGH
// /*AFLA*/  *
// /*AFLA*/  * __GFP_MEMALLOC allows access to all memory. This should only be used when
// /*AFLA*/  *   the caller guarantees the allocation will allow more memory to be freed
// /*AFLA*/  *   very shortly e.g. process exiting or swapping. Users either should
// /*AFLA*/  *   be the MM or co-ordinating closely with the VM (e.g. swap over NFS).
// /*AFLA*/  *
// /*AFLA*/  * __GFP_NOMEMALLOC is used to explicitly forbid access to emergency reserves.
// /*AFLA*/  *   This takes precedence over the __GFP_MEMALLOC flag if both are set.
// /*AFLA*/  */
// /*AFLA*/ #define __GFP_ATOMIC	((__force gfp_t)___GFP_ATOMIC)
// /*AFLA*/ #define __GFP_HIGH	((__force gfp_t)___GFP_HIGH)
// /*AFLA*/ #define __GFP_MEMALLOC	((__force gfp_t)___GFP_MEMALLOC)
// /*AFLA*/ #define __GFP_NOMEMALLOC ((__force gfp_t)___GFP_NOMEMALLOC)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Reclaim modifiers
// /*AFLA*/  *
// /*AFLA*/  * __GFP_IO can start physical IO.
// /*AFLA*/  *
// /*AFLA*/  * __GFP_FS can call down to the low-level FS. Clearing the flag avoids the
// /*AFLA*/  *   allocator recursing into the filesystem which might already be holding
// /*AFLA*/  *   locks.
// /*AFLA*/  *
// /*AFLA*/  * __GFP_DIRECT_RECLAIM indicates that the caller may enter direct reclaim.
// /*AFLA*/  *   This flag can be cleared to avoid unnecessary delays when a fallback
// /*AFLA*/  *   option is available.
// /*AFLA*/  *
// /*AFLA*/  * __GFP_KSWAPD_RECLAIM indicates that the caller wants to wake kswapd when
// /*AFLA*/  *   the low watermark is reached and have it reclaim pages until the high
// /*AFLA*/  *   watermark is reached. A caller may wish to clear this flag when fallback
// /*AFLA*/  *   options are available and the reclaim is likely to disrupt the system. The
// /*AFLA*/  *   canonical example is THP allocation where a fallback is cheap but
// /*AFLA*/  *   reclaim/compaction may cause indirect stalls.
// /*AFLA*/  *
// /*AFLA*/  * __GFP_RECLAIM is shorthand to allow/forbid both direct and kswapd reclaim.
// /*AFLA*/  *
// /*AFLA*/  * __GFP_REPEAT: Try hard to allocate the memory, but the allocation attempt
// /*AFLA*/  *   _might_ fail.  This depends upon the particular VM implementation.
// /*AFLA*/  *
// /*AFLA*/  * __GFP_NOFAIL: The VM implementation _must_ retry infinitely: the caller
// /*AFLA*/  *   cannot handle allocation failures. New users should be evaluated carefully
// /*AFLA*/  *   (and the flag should be used only when there is no reasonable failure
// /*AFLA*/  *   policy) but it is definitely preferable to use the flag rather than
// /*AFLA*/  *   opencode endless loop around allocator.
// /*AFLA*/  *
// /*AFLA*/  * __GFP_NORETRY: The VM implementation must not retry indefinitely and will
// /*AFLA*/  *   return NULL when direct reclaim and memory compaction have failed to allow
// /*AFLA*/  *   the allocation to succeed.  The OOM killer is not called with the current
// /*AFLA*/  *   implementation.
// /*AFLA*/  */
// /*AFLA*/ #define __GFP_IO	((__force gfp_t)___GFP_IO)
// /*AFLA*/ #define __GFP_FS	((__force gfp_t)___GFP_FS)
// /*AFLA*/ #define __GFP_DIRECT_RECLAIM	((__force gfp_t)___GFP_DIRECT_RECLAIM) /* Caller can reclaim */
// /*AFLA*/ #define __GFP_KSWAPD_RECLAIM	((__force gfp_t)___GFP_KSWAPD_RECLAIM) /* kswapd can wake */
// /*AFLA*/ #define __GFP_RECLAIM ((__force gfp_t)(___GFP_DIRECT_RECLAIM|___GFP_KSWAPD_RECLAIM))
// /*AFLA*/ #define __GFP_REPEAT	((__force gfp_t)___GFP_REPEAT)
// /*AFLA*/ #define __GFP_NOFAIL	((__force gfp_t)___GFP_NOFAIL)
// /*AFLA*/ #define __GFP_NORETRY	((__force gfp_t)___GFP_NORETRY)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Action modifiers
// /*AFLA*/  *
// /*AFLA*/  * __GFP_COLD indicates that the caller does not expect to be used in the near
// /*AFLA*/  *   future. Where possible, a cache-cold page will be returned.
// /*AFLA*/  *
// /*AFLA*/  * __GFP_NOWARN suppresses allocation failure reports.
// /*AFLA*/  *
// /*AFLA*/  * __GFP_COMP address compound page metadata.
// /*AFLA*/  *
// /*AFLA*/  * __GFP_ZERO returns a zeroed page on success.
// /*AFLA*/  *
// /*AFLA*/  * __GFP_NOTRACK avoids tracking with kmemcheck.
// /*AFLA*/  *
// /*AFLA*/  * __GFP_NOTRACK_FALSE_POSITIVE is an alias of __GFP_NOTRACK. It's a means of
// /*AFLA*/  *   distinguishing in the source between false positives and allocations that
// /*AFLA*/  *   cannot be supported (e.g. page tables).
// /*AFLA*/  *
// /*AFLA*/  * __GFP_OTHER_NODE is for allocations that are on a remote node but that
// /*AFLA*/  *   should not be accounted for as a remote allocation in vmstat. A
// /*AFLA*/  *   typical user would be khugepaged collapsing a huge page on a remote
// /*AFLA*/  *   node.
// /*AFLA*/  */
// /*AFLA*/ #define __GFP_COLD	((__force gfp_t)___GFP_COLD)
// /*AFLA*/ #define __GFP_NOWARN	((__force gfp_t)___GFP_NOWARN)
// /*AFLA*/ #define __GFP_COMP	((__force gfp_t)___GFP_COMP)
// /*AFLA*/ #define __GFP_ZERO	((__force gfp_t)___GFP_ZERO)
// /*AFLA*/ #define __GFP_NOTRACK	((__force gfp_t)___GFP_NOTRACK)
// /*AFLA*/ #define __GFP_NOTRACK_FALSE_POSITIVE (__GFP_NOTRACK)
// /*AFLA*/ #define __GFP_OTHER_NODE ((__force gfp_t)___GFP_OTHER_NODE)
// /*AFLA*/ 
// /*AFLA*/ /* Room for N __GFP_FOO bits */
// /*AFLA*/ #define __GFP_BITS_SHIFT 26
// /*AFLA*/ #define __GFP_BITS_MASK ((__force gfp_t)((1 << __GFP_BITS_SHIFT) - 1))
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Useful GFP flag combinations that are commonly used. It is recommended
// /*AFLA*/  * that subsystems start with one of these combinations and then set/clear
// /*AFLA*/  * __GFP_FOO flags as necessary.
// /*AFLA*/  *
// /*AFLA*/  * GFP_ATOMIC users can not sleep and need the allocation to succeed. A lower
// /*AFLA*/  *   watermark is applied to allow access to "atomic reserves"
// /*AFLA*/  *
// /*AFLA*/  * GFP_KERNEL is typical for kernel-internal allocations. The caller requires
// /*AFLA*/  *   ZONE_NORMAL or a lower zone for direct access but can direct reclaim.
// /*AFLA*/  *
// /*AFLA*/  * GFP_KERNEL_ACCOUNT is the same as GFP_KERNEL, except the allocation is
// /*AFLA*/  *   accounted to kmemcg.
// /*AFLA*/  *
// /*AFLA*/  * GFP_NOWAIT is for kernel allocations that should not stall for direct
// /*AFLA*/  *   reclaim, start physical IO or use any filesystem callback.
// /*AFLA*/  *
// /*AFLA*/  * GFP_NOIO will use direct reclaim to discard clean pages or slab pages
// /*AFLA*/  *   that do not require the starting of any physical IO.
// /*AFLA*/  *
// /*AFLA*/  * GFP_NOFS will use direct reclaim but will not use any filesystem interfaces.
// /*AFLA*/  *
// /*AFLA*/  * GFP_USER is for userspace allocations that also need to be directly
// /*AFLA*/  *   accessibly by the kernel or hardware. It is typically used by hardware
// /*AFLA*/  *   for buffers that are mapped to userspace (e.g. graphics) that hardware
// /*AFLA*/  *   still must DMA to. cpuset limits are enforced for these allocations.
// /*AFLA*/  *
// /*AFLA*/  * GFP_DMA exists for historical reasons and should be avoided where possible.
// /*AFLA*/  *   The flags indicates that the caller requires that the lowest zone be
// /*AFLA*/  *   used (ZONE_DMA or 16M on x86-64). Ideally, this would be removed but
// /*AFLA*/  *   it would require careful auditing as some users really require it and
// /*AFLA*/  *   others use the flag to avoid lowmem reserves in ZONE_DMA and treat the
// /*AFLA*/  *   lowest zone as a type of emergency reserve.
// /*AFLA*/  *
// /*AFLA*/  * GFP_DMA32 is similar to GFP_DMA except that the caller requires a 32-bit
// /*AFLA*/  *   address.
// /*AFLA*/  *
// /*AFLA*/  * GFP_HIGHUSER is for userspace allocations that may be mapped to userspace,
// /*AFLA*/  *   do not need to be directly accessible by the kernel but that cannot
// /*AFLA*/  *   move once in use. An example may be a hardware allocation that maps
// /*AFLA*/  *   data directly into userspace but has no addressing limitations.
// /*AFLA*/  *
// /*AFLA*/  * GFP_HIGHUSER_MOVABLE is for userspace allocations that the kernel does not
// /*AFLA*/  *   need direct access to but can use kmap() when access is required. They
// /*AFLA*/  *   are expected to be movable via page reclaim or page migration. Typically,
// /*AFLA*/  *   pages on the LRU would also be allocated with GFP_HIGHUSER_MOVABLE.
// /*AFLA*/  *
// /*AFLA*/  * GFP_TRANSHUGE and GFP_TRANSHUGE_LIGHT are used for THP allocations. They are
// /*AFLA*/  *   compound allocations that will generally fail quickly if memory is not
// /*AFLA*/  *   available and will not wake kswapd/kcompactd on failure. The _LIGHT
// /*AFLA*/  *   version does not attempt reclaim/compaction at all and is by default used
// /*AFLA*/  *   in page fault path, while the non-light is used by khugepaged.
// /*AFLA*/  */
// /*AFLA*/ #define GFP_ATOMIC	(__GFP_HIGH|__GFP_ATOMIC|__GFP_KSWAPD_RECLAIM)
// /*AFLA*/ #define GFP_KERNEL	(__GFP_RECLAIM | __GFP_IO | __GFP_FS)
// /*AFLA*/ #define GFP_KERNEL_ACCOUNT (GFP_KERNEL | __GFP_ACCOUNT)
// /*AFLA*/ #define GFP_NOWAIT	(__GFP_KSWAPD_RECLAIM)
// /*AFLA*/ #define GFP_NOIO	(__GFP_RECLAIM)
// /*AFLA*/ #define GFP_NOFS	(__GFP_RECLAIM | __GFP_IO)
// /*AFLA*/ #define GFP_TEMPORARY	(__GFP_RECLAIM | __GFP_IO | __GFP_FS | \
// /*AFLA*/ 			 __GFP_RECLAIMABLE)
// /*AFLA*/ #define GFP_USER	(__GFP_RECLAIM | __GFP_IO | __GFP_FS | __GFP_HARDWALL)
// /*AFLA*/ #define GFP_DMA		__GFP_DMA
// /*AFLA*/ #define GFP_DMA32	__GFP_DMA32
// /*AFLA*/ #define GFP_HIGHUSER	(GFP_USER | __GFP_HIGHMEM)
// /*AFLA*/ #define GFP_HIGHUSER_MOVABLE	(GFP_HIGHUSER | __GFP_MOVABLE)
// /*AFLA*/ #define GFP_TRANSHUGE_LIGHT	((GFP_HIGHUSER_MOVABLE | __GFP_COMP | \
// /*AFLA*/ 			 __GFP_NOMEMALLOC | __GFP_NOWARN) & ~__GFP_RECLAIM)
// /*AFLA*/ #define GFP_TRANSHUGE	(GFP_TRANSHUGE_LIGHT | __GFP_DIRECT_RECLAIM)
// /*AFLA*/ 
// /*AFLA*/ /* Convert GFP flags to their corresponding migrate type */
// /*AFLA*/ #define GFP_MOVABLE_MASK (__GFP_RECLAIMABLE|__GFP_MOVABLE)
// /*AFLA*/ #define GFP_MOVABLE_SHIFT 3
// /*AFLA*/ 
// /*AFLA*/ static inline int gfpflags_to_migratetype(const gfp_t gfp_flags)
// /*AFLA*/ {
// /*AFLA*/ 	VM_WARN_ON((gfp_flags & GFP_MOVABLE_MASK) == GFP_MOVABLE_MASK);
// /*AFLA*/ 	BUILD_BUG_ON((1UL << GFP_MOVABLE_SHIFT) != ___GFP_MOVABLE);
// /*AFLA*/ 	BUILD_BUG_ON((___GFP_MOVABLE >> GFP_MOVABLE_SHIFT) != MIGRATE_MOVABLE);
// /*AFLA*/ 
// /*AFLA*/ 	if (unlikely(page_group_by_mobility_disabled))
// /*AFLA*/ 		return MIGRATE_UNMOVABLE;
// /*AFLA*/ 
// /*AFLA*/ 	/* Group based on mobility */
// /*AFLA*/ 	return (gfp_flags & GFP_MOVABLE_MASK) >> GFP_MOVABLE_SHIFT;
// /*AFLA*/ }
// /*AFLA*/ #undef GFP_MOVABLE_MASK
// /*AFLA*/ #undef GFP_MOVABLE_SHIFT
// /*AFLA*/ 
// /*AFLA*/ static inline bool gfpflags_allow_blocking(const gfp_t gfp_flags)
// /*AFLA*/ {
// /*AFLA*/ 	return !!(gfp_flags & __GFP_DIRECT_RECLAIM);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HIGHMEM
// /*AFLA*/ #define OPT_ZONE_HIGHMEM ZONE_HIGHMEM
// /*AFLA*/ #else
// /*AFLA*/ #define OPT_ZONE_HIGHMEM ZONE_NORMAL
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_ZONE_DMA
// /*AFLA*/ #define OPT_ZONE_DMA ZONE_DMA
// /*AFLA*/ #else
// /*AFLA*/ #define OPT_ZONE_DMA ZONE_NORMAL
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_ZONE_DMA32
// /*AFLA*/ #define OPT_ZONE_DMA32 ZONE_DMA32
// /*AFLA*/ #else
// /*AFLA*/ #define OPT_ZONE_DMA32 ZONE_NORMAL
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * GFP_ZONE_TABLE is a word size bitstring that is used for looking up the
// /*AFLA*/  * zone to use given the lowest 4 bits of gfp_t. Entries are ZONE_SHIFT long
// /*AFLA*/  * and there are 16 of them to cover all possible combinations of
// /*AFLA*/  * __GFP_DMA, __GFP_DMA32, __GFP_MOVABLE and __GFP_HIGHMEM.
// /*AFLA*/  *
// /*AFLA*/  * The zone fallback order is MOVABLE=>HIGHMEM=>NORMAL=>DMA32=>DMA.
// /*AFLA*/  * But GFP_MOVABLE is not only a zone specifier but also an allocation
// /*AFLA*/  * policy. Therefore __GFP_MOVABLE plus another zone selector is valid.
// /*AFLA*/  * Only 1 bit of the lowest 3 bits (DMA,DMA32,HIGHMEM) can be set to "1".
// /*AFLA*/  *
// /*AFLA*/  *       bit       result
// /*AFLA*/  *       =================
// /*AFLA*/  *       0x0    => NORMAL
// /*AFLA*/  *       0x1    => DMA or NORMAL
// /*AFLA*/  *       0x2    => HIGHMEM or NORMAL
// /*AFLA*/  *       0x3    => BAD (DMA+HIGHMEM)
// /*AFLA*/  *       0x4    => DMA32 or DMA or NORMAL
// /*AFLA*/  *       0x5    => BAD (DMA+DMA32)
// /*AFLA*/  *       0x6    => BAD (HIGHMEM+DMA32)
// /*AFLA*/  *       0x7    => BAD (HIGHMEM+DMA32+DMA)
// /*AFLA*/  *       0x8    => NORMAL (MOVABLE+0)
// /*AFLA*/  *       0x9    => DMA or NORMAL (MOVABLE+DMA)
// /*AFLA*/  *       0xa    => MOVABLE (Movable is valid only if HIGHMEM is set too)
// /*AFLA*/  *       0xb    => BAD (MOVABLE+HIGHMEM+DMA)
// /*AFLA*/  *       0xc    => DMA32 (MOVABLE+DMA32)
// /*AFLA*/  *       0xd    => BAD (MOVABLE+DMA32+DMA)
// /*AFLA*/  *       0xe    => BAD (MOVABLE+DMA32+HIGHMEM)
// /*AFLA*/  *       0xf    => BAD (MOVABLE+DMA32+HIGHMEM+DMA)
// /*AFLA*/  *
// /*AFLA*/  * GFP_ZONES_SHIFT must be <= 2 on 32 bit platforms.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_ZONE_DEVICE) && (MAX_NR_ZONES-1) <= 4
// /*AFLA*/ /* ZONE_DEVICE is not a valid GFP zone specifier */
// /*AFLA*/ #define GFP_ZONES_SHIFT 2
// /*AFLA*/ #else
// /*AFLA*/ #define GFP_ZONES_SHIFT ZONES_SHIFT
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if 16 * GFP_ZONES_SHIFT > BITS_PER_LONG
// /*AFLA*/ #error GFP_ZONES_SHIFT too large to create GFP_ZONE_TABLE integer
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define GFP_ZONE_TABLE ( \
// /*AFLA*/ 	(ZONE_NORMAL << 0 * GFP_ZONES_SHIFT)				       \
// /*AFLA*/ 	| (OPT_ZONE_DMA << ___GFP_DMA * GFP_ZONES_SHIFT)		       \
// /*AFLA*/ 	| (OPT_ZONE_HIGHMEM << ___GFP_HIGHMEM * GFP_ZONES_SHIFT)	       \
// /*AFLA*/ 	| (OPT_ZONE_DMA32 << ___GFP_DMA32 * GFP_ZONES_SHIFT)		       \
// /*AFLA*/ 	| (ZONE_NORMAL << ___GFP_MOVABLE * GFP_ZONES_SHIFT)		       \
// /*AFLA*/ 	| (OPT_ZONE_DMA << (___GFP_MOVABLE | ___GFP_DMA) * GFP_ZONES_SHIFT)    \
// /*AFLA*/ 	| (ZONE_MOVABLE << (___GFP_MOVABLE | ___GFP_HIGHMEM) * GFP_ZONES_SHIFT)\
// /*AFLA*/ 	| (OPT_ZONE_DMA32 << (___GFP_MOVABLE | ___GFP_DMA32) * GFP_ZONES_SHIFT)\
// /*AFLA*/ )
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * GFP_ZONE_BAD is a bitmap for all combinations of __GFP_DMA, __GFP_DMA32
// /*AFLA*/  * __GFP_HIGHMEM and __GFP_MOVABLE that are not permitted. One flag per
// /*AFLA*/  * entry starting with bit 0. Bit is set if the combination is not
// /*AFLA*/  * allowed.
// /*AFLA*/  */
// /*AFLA*/ #define GFP_ZONE_BAD ( \
// /*AFLA*/ 	1 << (___GFP_DMA | ___GFP_HIGHMEM)				      \
// /*AFLA*/ 	| 1 << (___GFP_DMA | ___GFP_DMA32)				      \
// /*AFLA*/ 	| 1 << (___GFP_DMA32 | ___GFP_HIGHMEM)				      \
// /*AFLA*/ 	| 1 << (___GFP_DMA | ___GFP_DMA32 | ___GFP_HIGHMEM)		      \
// /*AFLA*/ 	| 1 << (___GFP_MOVABLE | ___GFP_HIGHMEM | ___GFP_DMA)		      \
// /*AFLA*/ 	| 1 << (___GFP_MOVABLE | ___GFP_DMA32 | ___GFP_DMA)		      \
// /*AFLA*/ 	| 1 << (___GFP_MOVABLE | ___GFP_DMA32 | ___GFP_HIGHMEM)		      \
// /*AFLA*/ 	| 1 << (___GFP_MOVABLE | ___GFP_DMA32 | ___GFP_DMA | ___GFP_HIGHMEM)  \
// /*AFLA*/ )
// /*AFLA*/ 
// /*AFLA*/ static inline enum zone_type gfp_zone(gfp_t flags)
// /*AFLA*/ {
// /*AFLA*/ 	enum zone_type z;
// /*AFLA*/ 	int bit = (__force int) (flags & GFP_ZONEMASK);
// /*AFLA*/ 
// /*AFLA*/ 	z = (GFP_ZONE_TABLE >> (bit * GFP_ZONES_SHIFT)) &
// /*AFLA*/ 					 ((1 << GFP_ZONES_SHIFT) - 1);
// /*AFLA*/ 	VM_BUG_ON((GFP_ZONE_BAD >> bit) & 1);
// /*AFLA*/ 	return z;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * There is only one page-allocator function, and two main namespaces to
// /*AFLA*/  * it. The alloc_page*() variants return 'struct page *' and as such
// /*AFLA*/  * can allocate highmem pages, the *get*page*() variants return
// /*AFLA*/  * virtual kernel addresses to the allocated page(s).
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ static inline int gfp_zonelist(gfp_t flags)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ 	if (unlikely(flags & __GFP_THISNODE))
// /*AFLA*/ 		return ZONELIST_NOFALLBACK;
// /*AFLA*/ #endif
// /*AFLA*/ 	return ZONELIST_FALLBACK;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We get the zone list from the current node and the gfp_mask.
// /*AFLA*/  * This zone list contains a maximum of MAXNODES*MAX_NR_ZONES zones.
// /*AFLA*/  * There are two zonelists per node, one for all zones with memory and
// /*AFLA*/  * one containing just zones from the node the zonelist belongs to.
// /*AFLA*/  *
// /*AFLA*/  * For the normal case of non-DISCONTIGMEM systems the NODE_DATA() gets
// /*AFLA*/  * optimized to &contig_page_data at compile-time.
// /*AFLA*/  */
// /*AFLA*/ static inline struct zonelist *node_zonelist(int nid, gfp_t flags)
// /*AFLA*/ {
// /*AFLA*/ 	return NODE_DATA(nid)->node_zonelists + gfp_zonelist(flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifndef HAVE_ARCH_FREE_PAGE
// /*AFLA*/ static inline void arch_free_page(struct page *page, int order) { }
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef HAVE_ARCH_ALLOC_PAGE
// /*AFLA*/ static inline void arch_alloc_page(struct page *page, int order) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct page *
// /*AFLA*/ __alloc_pages_nodemask(gfp_t gfp_mask, unsigned int order,
// /*AFLA*/ 		       struct zonelist *zonelist, nodemask_t *nodemask);
// /*AFLA*/ 
// /*AFLA*/ static inline struct page *
// /*AFLA*/ __alloc_pages(gfp_t gfp_mask, unsigned int order,
// /*AFLA*/ 		struct zonelist *zonelist)
// /*AFLA*/ {
// /*AFLA*/ 	return __alloc_pages_nodemask(gfp_mask, order, zonelist, NULL);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Allocate pages, preferring the node given as nid. The node must be valid and
// /*AFLA*/  * online. For more general interface, see alloc_pages_node().
// /*AFLA*/  */
// /*AFLA*/ static inline struct page *
// /*AFLA*/ __alloc_pages_node(int nid, gfp_t gfp_mask, unsigned int order)
// /*AFLA*/ {
// /*AFLA*/ 	VM_BUG_ON(nid < 0 || nid >= MAX_NUMNODES);
// /*AFLA*/ 	VM_WARN_ON(!node_online(nid));
// /*AFLA*/ 
// /*AFLA*/ 	return __alloc_pages(gfp_mask, order, node_zonelist(nid, gfp_mask));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Allocate pages, preferring the node given as nid. When nid == NUMA_NO_NODE,
// /*AFLA*/  * prefer the current CPU's closest node. Otherwise node must be valid and
// /*AFLA*/  * online.
// /*AFLA*/  */
// /*AFLA*/ static inline struct page *alloc_pages_node(int nid, gfp_t gfp_mask,
// /*AFLA*/ 						unsigned int order)
// /*AFLA*/ {
// /*AFLA*/ 	if (nid == NUMA_NO_NODE)
// /*AFLA*/ 		nid = numa_mem_id();
// /*AFLA*/ 
// /*AFLA*/ 	return __alloc_pages_node(nid, gfp_mask, order);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ extern struct page *alloc_pages_current(gfp_t gfp_mask, unsigned order);
// /*AFLA*/ 
// /*AFLA*/ static inline struct page *
// /*AFLA*/ alloc_pages(gfp_t gfp_mask, unsigned int order)
// /*AFLA*/ {
// /*AFLA*/ 	return alloc_pages_current(gfp_mask, order);
// /*AFLA*/ }
// /*AFLA*/ extern struct page *alloc_pages_vma(gfp_t gfp_mask, int order,
// /*AFLA*/ 			struct vm_area_struct *vma, unsigned long addr,
// /*AFLA*/ 			int node, bool hugepage);
// /*AFLA*/ #define alloc_hugepage_vma(gfp_mask, vma, addr, order)	\
// /*AFLA*/ 	alloc_pages_vma(gfp_mask, order, vma, addr, numa_node_id(), true)
// /*AFLA*/ #else
// /*AFLA*/ #define alloc_pages(gfp_mask, order) \
// /*AFLA*/ 		alloc_pages_node(numa_node_id(), gfp_mask, order)
// /*AFLA*/ #define alloc_pages_vma(gfp_mask, order, vma, addr, node, false)\
// /*AFLA*/ 	alloc_pages(gfp_mask, order)
// /*AFLA*/ #define alloc_hugepage_vma(gfp_mask, vma, addr, order)	\
// /*AFLA*/ 	alloc_pages(gfp_mask, order)
// /*AFLA*/ #endif
// /*AFLA*/ #define alloc_page(gfp_mask) alloc_pages(gfp_mask, 0)
// /*AFLA*/ #define alloc_page_vma(gfp_mask, vma, addr)			\
// /*AFLA*/ 	alloc_pages_vma(gfp_mask, 0, vma, addr, numa_node_id(), false)
// /*AFLA*/ #define alloc_page_vma_node(gfp_mask, vma, addr, node)		\
// /*AFLA*/ 	alloc_pages_vma(gfp_mask, 0, vma, addr, node, false)
// /*AFLA*/ 
// /*AFLA*/ extern unsigned long __get_free_pages(gfp_t gfp_mask, unsigned int order);
// /*AFLA*/ extern unsigned long get_zeroed_page(gfp_t gfp_mask);
// /*AFLA*/ 
// /*AFLA*/ void *alloc_pages_exact(size_t size, gfp_t gfp_mask);
// /*AFLA*/ void free_pages_exact(void *virt, size_t size);
// /*AFLA*/ void * __meminit alloc_pages_exact_nid(int nid, size_t size, gfp_t gfp_mask);
// /*AFLA*/ 
// /*AFLA*/ #define __get_free_page(gfp_mask) \
// /*AFLA*/ 		__get_free_pages((gfp_mask), 0)
// /*AFLA*/ 
// /*AFLA*/ #define __get_dma_pages(gfp_mask, order) \
// /*AFLA*/ 		__get_free_pages((gfp_mask) | GFP_DMA, (order))
// /*AFLA*/ 
// /*AFLA*/ extern void __free_pages(struct page *page, unsigned int order);
// /*AFLA*/ extern void free_pages(unsigned long addr, unsigned int order);
// /*AFLA*/ extern void free_hot_cold_page(struct page *page, bool cold);
// /*AFLA*/ extern void free_hot_cold_page_list(struct list_head *list, bool cold);
// /*AFLA*/ 
// /*AFLA*/ struct page_frag_cache;
// /*AFLA*/ extern void *__alloc_page_frag(struct page_frag_cache *nc,
// /*AFLA*/ 			       unsigned int fragsz, gfp_t gfp_mask);
// /*AFLA*/ extern void __free_page_frag(void *addr);
// /*AFLA*/ 
// /*AFLA*/ #define __free_page(page) __free_pages((page), 0)
// /*AFLA*/ #define free_page(addr) free_pages((addr), 0)
// /*AFLA*/ 
// /*AFLA*/ void page_alloc_init(void);
// /*AFLA*/ void drain_zone_pages(struct zone *zone, struct per_cpu_pages *pcp);
// /*AFLA*/ void drain_all_pages(struct zone *zone);
// /*AFLA*/ void drain_local_pages(struct zone *zone);
// /*AFLA*/ 
// /*AFLA*/ void page_alloc_init_late(void);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * gfp_allowed_mask is set to GFP_BOOT_MASK during early boot to restrict what
// /*AFLA*/  * GFP flags are used before interrupts are enabled. Once interrupts are
// /*AFLA*/  * enabled, it is set to __GFP_BITS_MASK while the system is running. During
// /*AFLA*/  * hibernation, it is used by PM to avoid I/O during memory allocation while
// /*AFLA*/  * devices are suspended.
// /*AFLA*/  */
// /*AFLA*/ extern gfp_t gfp_allowed_mask;
// /*AFLA*/ 
// /*AFLA*/ /* Returns true if the gfp_mask allows use of ALLOC_NO_WATERMARK */
// /*AFLA*/ bool gfp_pfmemalloc_allowed(gfp_t gfp_mask);
// /*AFLA*/ 
// /*AFLA*/ extern void pm_restrict_gfp_mask(void);
// /*AFLA*/ extern void pm_restore_gfp_mask(void);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PM_SLEEP
// /*AFLA*/ extern bool pm_suspended_storage(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline bool pm_suspended_storage(void)
// /*AFLA*/ {
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_PM_SLEEP */
// /*AFLA*/ 
// /*AFLA*/ #if (defined(CONFIG_MEMORY_ISOLATION) && defined(CONFIG_COMPACTION)) || defined(CONFIG_CMA)
// /*AFLA*/ /* The below functions must be run on a range from a single zone. */
// /*AFLA*/ extern int alloc_contig_range(unsigned long start, unsigned long end,
// /*AFLA*/ 			      unsigned migratetype);
// /*AFLA*/ extern void free_contig_range(unsigned long pfn, unsigned nr_pages);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_CMA
// /*AFLA*/ /* CMA stuff */
// /*AFLA*/ extern void init_cma_reserved_pageblock(struct page *page);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_GFP_H */
