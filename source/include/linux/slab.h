// /*AFLA*/ /*
// /*AFLA*/  * Written by Mark Hemment, 1996 (markhe@nextd.demon.co.uk).
// /*AFLA*/  *
// /*AFLA*/  * (C) SGI 2006, Christoph Lameter
// /*AFLA*/  * 	Cleaned up and restructured to ease the addition of alternative
// /*AFLA*/  * 	implementations of SLAB allocators.
// /*AFLA*/  * (C) Linux Foundation 2008-2013
// /*AFLA*/  *      Unified interface for all slab allocators
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_SLAB_H
// /*AFLA*/ #define	_LINUX_SLAB_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/gfp.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/workqueue.h>
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Flags to pass to kmem_cache_create().
// /*AFLA*/  * The ones marked DEBUG are only valid if CONFIG_DEBUG_SLAB is set.
// /*AFLA*/  */
// /*AFLA*/ #define SLAB_CONSISTENCY_CHECKS	0x00000100UL	/* DEBUG: Perform (expensive) checks on alloc/free */
// /*AFLA*/ #define SLAB_RED_ZONE		0x00000400UL	/* DEBUG: Red zone objs in a cache */
// /*AFLA*/ #define SLAB_POISON		0x00000800UL	/* DEBUG: Poison objects */
// /*AFLA*/ #define SLAB_HWCACHE_ALIGN	0x00002000UL	/* Align objs on cache lines */
// /*AFLA*/ #define SLAB_CACHE_DMA		0x00004000UL	/* Use GFP_DMA memory */
// /*AFLA*/ #define SLAB_STORE_USER		0x00010000UL	/* DEBUG: Store the last owner for bug hunting */
// /*AFLA*/ #define SLAB_PANIC		0x00040000UL	/* Panic if kmem_cache_create() fails */
// /*AFLA*/ /*
// /*AFLA*/  * SLAB_DESTROY_BY_RCU - **WARNING** READ THIS!
// /*AFLA*/  *
// /*AFLA*/  * This delays freeing the SLAB page by a grace period, it does _NOT_
// /*AFLA*/  * delay object freeing. This means that if you do kmem_cache_free()
// /*AFLA*/  * that memory location is free to be reused at any time. Thus it may
// /*AFLA*/  * be possible to see another object there in the same RCU grace period.
// /*AFLA*/  *
// /*AFLA*/  * This feature only ensures the memory location backing the object
// /*AFLA*/  * stays valid, the trick to using this is relying on an independent
// /*AFLA*/  * object validation pass. Something like:
// /*AFLA*/  *
// /*AFLA*/  *  rcu_read_lock()
// /*AFLA*/  * again:
// /*AFLA*/  *  obj = lockless_lookup(key);
// /*AFLA*/  *  if (obj) {
// /*AFLA*/  *    if (!try_get_ref(obj)) // might fail for free objects
// /*AFLA*/  *      goto again;
// /*AFLA*/  *
// /*AFLA*/  *    if (obj->key != key) { // not the object we expected
// /*AFLA*/  *      put_ref(obj);
// /*AFLA*/  *      goto again;
// /*AFLA*/  *    }
// /*AFLA*/  *  }
// /*AFLA*/  *  rcu_read_unlock();
// /*AFLA*/  *
// /*AFLA*/  * This is useful if we need to approach a kernel structure obliquely,
// /*AFLA*/  * from its address obtained without the usual locking. We can lock
// /*AFLA*/  * the structure to stabilize it and check it's still at the given address,
// /*AFLA*/  * only if we can be sure that the memory has not been meanwhile reused
// /*AFLA*/  * for some other kind of object (which our subsystem's lock might corrupt).
// /*AFLA*/  *
// /*AFLA*/  * rcu_read_lock before reading the address, then rcu_read_unlock after
// /*AFLA*/  * taking the spinlock within the structure expected at that address.
// /*AFLA*/  */
// /*AFLA*/ #define SLAB_DESTROY_BY_RCU	0x00080000UL	/* Defer freeing slabs to RCU */
// /*AFLA*/ #define SLAB_MEM_SPREAD		0x00100000UL	/* Spread some memory over cpuset */
// /*AFLA*/ #define SLAB_TRACE		0x00200000UL	/* Trace allocations and frees */
// /*AFLA*/ 
// /*AFLA*/ /* Flag to prevent checks on free */
// /*AFLA*/ #ifdef CONFIG_DEBUG_OBJECTS
// /*AFLA*/ # define SLAB_DEBUG_OBJECTS	0x00400000UL
// /*AFLA*/ #else
// /*AFLA*/ # define SLAB_DEBUG_OBJECTS	0x00000000UL
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define SLAB_NOLEAKTRACE	0x00800000UL	/* Avoid kmemleak tracing */
// /*AFLA*/ 
// /*AFLA*/ /* Don't track use of uninitialized memory */
// /*AFLA*/ #ifdef CONFIG_KMEMCHECK
// /*AFLA*/ # define SLAB_NOTRACK		0x01000000UL
// /*AFLA*/ #else
// /*AFLA*/ # define SLAB_NOTRACK		0x00000000UL
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_FAILSLAB
// /*AFLA*/ # define SLAB_FAILSLAB		0x02000000UL	/* Fault injection mark */
// /*AFLA*/ #else
// /*AFLA*/ # define SLAB_FAILSLAB		0x00000000UL
// /*AFLA*/ #endif
// /*AFLA*/ #if defined(CONFIG_MEMCG) && !defined(CONFIG_SLOB)
// /*AFLA*/ # define SLAB_ACCOUNT		0x04000000UL	/* Account to memcg */
// /*AFLA*/ #else
// /*AFLA*/ # define SLAB_ACCOUNT		0x00000000UL
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_KASAN
// /*AFLA*/ #define SLAB_KASAN		0x08000000UL
// /*AFLA*/ #else
// /*AFLA*/ #define SLAB_KASAN		0x00000000UL
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* The following flags affect the page allocator grouping pages by mobility */
// /*AFLA*/ #define SLAB_RECLAIM_ACCOUNT	0x00020000UL		/* Objects are reclaimable */
// /*AFLA*/ #define SLAB_TEMPORARY		SLAB_RECLAIM_ACCOUNT	/* Objects are short-lived */
// /*AFLA*/ /*
// /*AFLA*/  * ZERO_SIZE_PTR will be returned for zero sized kmalloc requests.
// /*AFLA*/  *
// /*AFLA*/  * Dereferencing ZERO_SIZE_PTR will lead to a distinct access fault.
// /*AFLA*/  *
// /*AFLA*/  * ZERO_SIZE_PTR can be passed to kfree though in the same way that NULL can.
// /*AFLA*/  * Both make kfree a no-op.
// /*AFLA*/  */
// /*AFLA*/ #define ZERO_SIZE_PTR ((void *)16)
// /*AFLA*/ 
// /*AFLA*/ #define ZERO_OR_NULL_PTR(x) ((unsigned long)(x) <= \
// /*AFLA*/ 				(unsigned long)ZERO_SIZE_PTR)
// /*AFLA*/ 
// /*AFLA*/ #include <linux/kmemleak.h>
// /*AFLA*/ #include <linux/kasan.h>
// /*AFLA*/ 
// /*AFLA*/ struct mem_cgroup;
// /*AFLA*/ /*
// /*AFLA*/  * struct kmem_cache related prototypes
// /*AFLA*/  */
// /*AFLA*/ void __init kmem_cache_init(void);
// /*AFLA*/ bool slab_is_available(void);
// /*AFLA*/ 
// /*AFLA*/ struct kmem_cache *kmem_cache_create(const char *, size_t, size_t,
// /*AFLA*/ 			unsigned long,
// /*AFLA*/ 			void (*)(void *));
// /*AFLA*/ void kmem_cache_destroy(struct kmem_cache *);
// /*AFLA*/ int kmem_cache_shrink(struct kmem_cache *);
// /*AFLA*/ 
// /*AFLA*/ void memcg_create_kmem_cache(struct mem_cgroup *, struct kmem_cache *);
// /*AFLA*/ void memcg_deactivate_kmem_caches(struct mem_cgroup *);
// /*AFLA*/ void memcg_destroy_kmem_caches(struct mem_cgroup *);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Please use this macro to create slab caches. Simply specify the
// /*AFLA*/  * name of the structure and maybe some flags that are listed above.
// /*AFLA*/  *
// /*AFLA*/  * The alignment of the struct determines object alignment. If you
// /*AFLA*/  * f.e. add ____cacheline_aligned_in_smp to the struct declaration
// /*AFLA*/  * then the objects will be properly aligned in SMP configurations.
// /*AFLA*/  */
// /*AFLA*/ #define KMEM_CACHE(__struct, __flags) kmem_cache_create(#__struct,\
// /*AFLA*/ 		sizeof(struct __struct), __alignof__(struct __struct),\
// /*AFLA*/ 		(__flags), NULL)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Common kmalloc functions provided by all allocators
// /*AFLA*/  */
// /*AFLA*/ void * __must_check __krealloc(const void *, size_t, gfp_t);
// /*AFLA*/ void * __must_check krealloc(const void *, size_t, gfp_t);
// /*AFLA*/ void kfree(const void *);
// /*AFLA*/ void kzfree(const void *);
// /*AFLA*/ size_t ksize(const void *);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_HAVE_HARDENED_USERCOPY_ALLOCATOR
// /*AFLA*/ const char *__check_heap_object(const void *ptr, unsigned long n,
// /*AFLA*/ 				struct page *page);
// /*AFLA*/ #else
// /*AFLA*/ static inline const char *__check_heap_object(const void *ptr,
// /*AFLA*/ 					      unsigned long n,
// /*AFLA*/ 					      struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Some archs want to perform DMA into kmalloc caches and need a guaranteed
// /*AFLA*/  * alignment larger than the alignment of a 64-bit integer.
// /*AFLA*/  * Setting ARCH_KMALLOC_MINALIGN in arch headers allows that.
// /*AFLA*/  */
// /*AFLA*/ #if defined(ARCH_DMA_MINALIGN) && ARCH_DMA_MINALIGN > 8
// /*AFLA*/ #define ARCH_KMALLOC_MINALIGN ARCH_DMA_MINALIGN
// /*AFLA*/ #define KMALLOC_MIN_SIZE ARCH_DMA_MINALIGN
// /*AFLA*/ #define KMALLOC_SHIFT_LOW ilog2(ARCH_DMA_MINALIGN)
// /*AFLA*/ #else
// /*AFLA*/ #define ARCH_KMALLOC_MINALIGN __alignof__(unsigned long long)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Setting ARCH_SLAB_MINALIGN in arch headers allows a different alignment.
// /*AFLA*/  * Intended for arches that get misalignment faults even for 64 bit integer
// /*AFLA*/  * aligned buffers.
// /*AFLA*/  */
// /*AFLA*/ #ifndef ARCH_SLAB_MINALIGN
// /*AFLA*/ #define ARCH_SLAB_MINALIGN __alignof__(unsigned long long)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * kmalloc and friends return ARCH_KMALLOC_MINALIGN aligned
// /*AFLA*/  * pointers. kmem_cache_alloc and friends return ARCH_SLAB_MINALIGN
// /*AFLA*/  * aligned pointers.
// /*AFLA*/  */
// /*AFLA*/ #define __assume_kmalloc_alignment __assume_aligned(ARCH_KMALLOC_MINALIGN)
// /*AFLA*/ #define __assume_slab_alignment __assume_aligned(ARCH_SLAB_MINALIGN)
// /*AFLA*/ #define __assume_page_alignment __assume_aligned(PAGE_SIZE)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Kmalloc array related definitions
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SLAB
// /*AFLA*/ /*
// /*AFLA*/  * The largest kmalloc size supported by the SLAB allocators is
// /*AFLA*/  * 32 megabyte (2^25) or the maximum allocatable page order if that is
// /*AFLA*/  * less than 32 MB.
// /*AFLA*/  *
// /*AFLA*/  * WARNING: Its not easy to increase this value since the allocators have
// /*AFLA*/  * to do various tricks to work around compiler limitations in order to
// /*AFLA*/  * ensure proper constant folding.
// /*AFLA*/  */
// /*AFLA*/ #define KMALLOC_SHIFT_HIGH	((MAX_ORDER + PAGE_SHIFT - 1) <= 25 ? \
// /*AFLA*/ 				(MAX_ORDER + PAGE_SHIFT - 1) : 25)
// /*AFLA*/ #define KMALLOC_SHIFT_MAX	KMALLOC_SHIFT_HIGH
// /*AFLA*/ #ifndef KMALLOC_SHIFT_LOW
// /*AFLA*/ #define KMALLOC_SHIFT_LOW	5
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SLUB
// /*AFLA*/ /*
// /*AFLA*/  * SLUB directly allocates requests fitting in to an order-1 page
// /*AFLA*/  * (PAGE_SIZE*2).  Larger requests are passed to the page allocator.
// /*AFLA*/  */
// /*AFLA*/ #define KMALLOC_SHIFT_HIGH	(PAGE_SHIFT + 1)
// /*AFLA*/ #define KMALLOC_SHIFT_MAX	(MAX_ORDER + PAGE_SHIFT)
// /*AFLA*/ #ifndef KMALLOC_SHIFT_LOW
// /*AFLA*/ #define KMALLOC_SHIFT_LOW	3
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SLOB
// /*AFLA*/ /*
// /*AFLA*/  * SLOB passes all requests larger than one page to the page allocator.
// /*AFLA*/  * No kmalloc array is necessary since objects of different sizes can
// /*AFLA*/  * be allocated from the same page.
// /*AFLA*/  */
// /*AFLA*/ #define KMALLOC_SHIFT_HIGH	PAGE_SHIFT
// /*AFLA*/ #define KMALLOC_SHIFT_MAX	30
// /*AFLA*/ #ifndef KMALLOC_SHIFT_LOW
// /*AFLA*/ #define KMALLOC_SHIFT_LOW	3
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Maximum allocatable size */
// /*AFLA*/ #define KMALLOC_MAX_SIZE	(1UL << KMALLOC_SHIFT_MAX)
// /*AFLA*/ /* Maximum size for which we actually use a slab cache */
// /*AFLA*/ #define KMALLOC_MAX_CACHE_SIZE	(1UL << KMALLOC_SHIFT_HIGH)
// /*AFLA*/ /* Maximum order allocatable via the slab allocagtor */
// /*AFLA*/ #define KMALLOC_MAX_ORDER	(KMALLOC_SHIFT_MAX - PAGE_SHIFT)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Kmalloc subsystem.
// /*AFLA*/  */
// /*AFLA*/ #ifndef KMALLOC_MIN_SIZE
// /*AFLA*/ #define KMALLOC_MIN_SIZE (1 << KMALLOC_SHIFT_LOW)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This restriction comes from byte sized index implementation.
// /*AFLA*/  * Page size is normally 2^12 bytes and, in this case, if we want to use
// /*AFLA*/  * byte sized index which can represent 2^8 entries, the size of the object
// /*AFLA*/  * should be equal or greater to 2^12 / 2^8 = 2^4 = 16.
// /*AFLA*/  * If minimum size of kmalloc is less than 16, we use it as minimum object
// /*AFLA*/  * size and give up to use byte sized index.
// /*AFLA*/  */
// /*AFLA*/ #define SLAB_OBJ_MIN_SIZE      (KMALLOC_MIN_SIZE < 16 ? \
// /*AFLA*/                                (KMALLOC_MIN_SIZE) : 16)
// /*AFLA*/ 
// /*AFLA*/ #ifndef CONFIG_SLOB
// /*AFLA*/ extern struct kmem_cache *kmalloc_caches[KMALLOC_SHIFT_HIGH + 1];
// /*AFLA*/ #ifdef CONFIG_ZONE_DMA
// /*AFLA*/ extern struct kmem_cache *kmalloc_dma_caches[KMALLOC_SHIFT_HIGH + 1];
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Figure out which kmalloc slab an allocation of a certain size
// /*AFLA*/  * belongs to.
// /*AFLA*/  * 0 = zero alloc
// /*AFLA*/  * 1 =  65 .. 96 bytes
// /*AFLA*/  * 2 = 129 .. 192 bytes
// /*AFLA*/  * n = 2^(n-1)+1 .. 2^n
// /*AFLA*/  */
// /*AFLA*/ static __always_inline int kmalloc_index(size_t size)
// /*AFLA*/ {
// /*AFLA*/ 	if (!size)
// /*AFLA*/ 		return 0;
// /*AFLA*/ 
// /*AFLA*/ 	if (size <= KMALLOC_MIN_SIZE)
// /*AFLA*/ 		return KMALLOC_SHIFT_LOW;
// /*AFLA*/ 
// /*AFLA*/ 	if (KMALLOC_MIN_SIZE <= 32 && size > 64 && size <= 96)
// /*AFLA*/ 		return 1;
// /*AFLA*/ 	if (KMALLOC_MIN_SIZE <= 64 && size > 128 && size <= 192)
// /*AFLA*/ 		return 2;
// /*AFLA*/ 	if (size <=          8) return 3;
// /*AFLA*/ 	if (size <=         16) return 4;
// /*AFLA*/ 	if (size <=         32) return 5;
// /*AFLA*/ 	if (size <=         64) return 6;
// /*AFLA*/ 	if (size <=        128) return 7;
// /*AFLA*/ 	if (size <=        256) return 8;
// /*AFLA*/ 	if (size <=        512) return 9;
// /*AFLA*/ 	if (size <=       1024) return 10;
// /*AFLA*/ 	if (size <=   2 * 1024) return 11;
// /*AFLA*/ 	if (size <=   4 * 1024) return 12;
// /*AFLA*/ 	if (size <=   8 * 1024) return 13;
// /*AFLA*/ 	if (size <=  16 * 1024) return 14;
// /*AFLA*/ 	if (size <=  32 * 1024) return 15;
// /*AFLA*/ 	if (size <=  64 * 1024) return 16;
// /*AFLA*/ 	if (size <= 128 * 1024) return 17;
// /*AFLA*/ 	if (size <= 256 * 1024) return 18;
// /*AFLA*/ 	if (size <= 512 * 1024) return 19;
// /*AFLA*/ 	if (size <= 1024 * 1024) return 20;
// /*AFLA*/ 	if (size <=  2 * 1024 * 1024) return 21;
// /*AFLA*/ 	if (size <=  4 * 1024 * 1024) return 22;
// /*AFLA*/ 	if (size <=  8 * 1024 * 1024) return 23;
// /*AFLA*/ 	if (size <=  16 * 1024 * 1024) return 24;
// /*AFLA*/ 	if (size <=  32 * 1024 * 1024) return 25;
// /*AFLA*/ 	if (size <=  64 * 1024 * 1024) return 26;
// /*AFLA*/ 	BUG();
// /*AFLA*/ 
// /*AFLA*/ 	/* Will never be reached. Needed because the compiler may complain */
// /*AFLA*/ 	return -1;
// /*AFLA*/ }
// /*AFLA*/ #endif /* !CONFIG_SLOB */
// /*AFLA*/ 
// /*AFLA*/ void *__kmalloc(size_t size, gfp_t flags) __assume_kmalloc_alignment __malloc;
// /*AFLA*/ void *kmem_cache_alloc(struct kmem_cache *, gfp_t flags) __assume_slab_alignment __malloc;
// /*AFLA*/ void kmem_cache_free(struct kmem_cache *, void *);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Bulk allocation and freeing operations. These are accelerated in an
// /*AFLA*/  * allocator specific way to avoid taking locks repeatedly or building
// /*AFLA*/  * metadata structures unnecessarily.
// /*AFLA*/  *
// /*AFLA*/  * Note that interrupts must be enabled when calling these functions.
// /*AFLA*/  */
// /*AFLA*/ void kmem_cache_free_bulk(struct kmem_cache *, size_t, void **);
// /*AFLA*/ int kmem_cache_alloc_bulk(struct kmem_cache *, gfp_t, size_t, void **);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Caller must not use kfree_bulk() on memory not originally allocated
// /*AFLA*/  * by kmalloc(), because the SLOB allocator cannot handle this.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline void kfree_bulk(size_t size, void **p)
// /*AFLA*/ {
// /*AFLA*/ 	kmem_cache_free_bulk(NULL, size, p);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ void *__kmalloc_node(size_t size, gfp_t flags, int node) __assume_kmalloc_alignment __malloc;
// /*AFLA*/ void *kmem_cache_alloc_node(struct kmem_cache *, gfp_t flags, int node) __assume_slab_alignment __malloc;
// /*AFLA*/ #else
// /*AFLA*/ static __always_inline void *__kmalloc_node(size_t size, gfp_t flags, int node)
// /*AFLA*/ {
// /*AFLA*/ 	return __kmalloc(size, flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void *kmem_cache_alloc_node(struct kmem_cache *s, gfp_t flags, int node)
// /*AFLA*/ {
// /*AFLA*/ 	return kmem_cache_alloc(s, flags);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_TRACING
// /*AFLA*/ extern void *kmem_cache_alloc_trace(struct kmem_cache *, gfp_t, size_t) __assume_slab_alignment __malloc;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ extern void *kmem_cache_alloc_node_trace(struct kmem_cache *s,
// /*AFLA*/ 					   gfp_t gfpflags,
// /*AFLA*/ 					   int node, size_t size) __assume_slab_alignment __malloc;
// /*AFLA*/ #else
// /*AFLA*/ static __always_inline void *
// /*AFLA*/ kmem_cache_alloc_node_trace(struct kmem_cache *s,
// /*AFLA*/ 			      gfp_t gfpflags,
// /*AFLA*/ 			      int node, size_t size)
// /*AFLA*/ {
// /*AFLA*/ 	return kmem_cache_alloc_trace(s, gfpflags, size);
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_NUMA */
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_TRACING */
// /*AFLA*/ static __always_inline void *kmem_cache_alloc_trace(struct kmem_cache *s,
// /*AFLA*/ 		gfp_t flags, size_t size)
// /*AFLA*/ {
// /*AFLA*/ 	void *ret = kmem_cache_alloc(s, flags);
// /*AFLA*/ 
// /*AFLA*/ 	kasan_kmalloc(s, ret, size, flags);
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void *
// /*AFLA*/ kmem_cache_alloc_node_trace(struct kmem_cache *s,
// /*AFLA*/ 			      gfp_t gfpflags,
// /*AFLA*/ 			      int node, size_t size)
// /*AFLA*/ {
// /*AFLA*/ 	void *ret = kmem_cache_alloc_node(s, gfpflags, node);
// /*AFLA*/ 
// /*AFLA*/ 	kasan_kmalloc(s, ret, size, gfpflags);
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_TRACING */
// /*AFLA*/ 
// /*AFLA*/ extern void *kmalloc_order(size_t size, gfp_t flags, unsigned int order) __assume_page_alignment __malloc;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_TRACING
// /*AFLA*/ extern void *kmalloc_order_trace(size_t size, gfp_t flags, unsigned int order) __assume_page_alignment __malloc;
// /*AFLA*/ #else
// /*AFLA*/ static __always_inline void *
// /*AFLA*/ kmalloc_order_trace(size_t size, gfp_t flags, unsigned int order)
// /*AFLA*/ {
// /*AFLA*/ 	return kmalloc_order(size, flags, order);
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void *kmalloc_large(size_t size, gfp_t flags)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned int order = get_order(size);
// /*AFLA*/ 	return kmalloc_order_trace(size, flags, order);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * kmalloc - allocate memory
// /*AFLA*/  * @size: how many bytes of memory are required.
// /*AFLA*/  * @flags: the type of memory to allocate.
// /*AFLA*/  *
// /*AFLA*/  * kmalloc is the normal method of allocating memory
// /*AFLA*/  * for objects smaller than page size in the kernel.
// /*AFLA*/  *
// /*AFLA*/  * The @flags argument may be one of:
// /*AFLA*/  *
// /*AFLA*/  * %GFP_USER - Allocate memory on behalf of user.  May sleep.
// /*AFLA*/  *
// /*AFLA*/  * %GFP_KERNEL - Allocate normal kernel ram.  May sleep.
// /*AFLA*/  *
// /*AFLA*/  * %GFP_ATOMIC - Allocation will not sleep.  May use emergency pools.
// /*AFLA*/  *   For example, use this inside interrupt handlers.
// /*AFLA*/  *
// /*AFLA*/  * %GFP_HIGHUSER - Allocate pages from high memory.
// /*AFLA*/  *
// /*AFLA*/  * %GFP_NOIO - Do not do any I/O at all while trying to get memory.
// /*AFLA*/  *
// /*AFLA*/  * %GFP_NOFS - Do not make any fs calls while trying to get memory.
// /*AFLA*/  *
// /*AFLA*/  * %GFP_NOWAIT - Allocation will not sleep.
// /*AFLA*/  *
// /*AFLA*/  * %__GFP_THISNODE - Allocate node-local memory only.
// /*AFLA*/  *
// /*AFLA*/  * %GFP_DMA - Allocation suitable for DMA.
// /*AFLA*/  *   Should only be used for kmalloc() caches. Otherwise, use a
// /*AFLA*/  *   slab created with SLAB_DMA.
// /*AFLA*/  *
// /*AFLA*/  * Also it is possible to set different flags by OR'ing
// /*AFLA*/  * in one or more of the following additional @flags:
// /*AFLA*/  *
// /*AFLA*/  * %__GFP_COLD - Request cache-cold pages instead of
// /*AFLA*/  *   trying to return cache-warm pages.
// /*AFLA*/  *
// /*AFLA*/  * %__GFP_HIGH - This allocation has high priority and may use emergency pools.
// /*AFLA*/  *
// /*AFLA*/  * %__GFP_NOFAIL - Indicate that this allocation is in no way allowed to fail
// /*AFLA*/  *   (think twice before using).
// /*AFLA*/  *
// /*AFLA*/  * %__GFP_NORETRY - If memory is not immediately available,
// /*AFLA*/  *   then give up at once.
// /*AFLA*/  *
// /*AFLA*/  * %__GFP_NOWARN - If allocation fails, don't issue any warnings.
// /*AFLA*/  *
// /*AFLA*/  * %__GFP_REPEAT - If allocation fails initially, try once more before failing.
// /*AFLA*/  *
// /*AFLA*/  * There are other flags available as well, but these are not intended
// /*AFLA*/  * for general use, and so are not documented here. For a full list of
// /*AFLA*/  * potential flags, always refer to linux/gfp.h.
// /*AFLA*/  */
// /*AFLA*/ static __always_inline void *kmalloc(size_t size, gfp_t flags)
// /*AFLA*/ {
// /*AFLA*/ 	if (__builtin_constant_p(size)) {
// /*AFLA*/ 		if (size > KMALLOC_MAX_CACHE_SIZE)
// /*AFLA*/ 			return kmalloc_large(size, flags);
// /*AFLA*/ #ifndef CONFIG_SLOB
// /*AFLA*/ 		if (!(flags & GFP_DMA)) {
// /*AFLA*/ 			int index = kmalloc_index(size);
// /*AFLA*/ 
// /*AFLA*/ 			if (!index)
// /*AFLA*/ 				return ZERO_SIZE_PTR;
// /*AFLA*/ 
// /*AFLA*/ 			return kmem_cache_alloc_trace(kmalloc_caches[index],
// /*AFLA*/ 					flags, size);
// /*AFLA*/ 		}
// /*AFLA*/ #endif
// /*AFLA*/ 	}
// /*AFLA*/ 	return __kmalloc(size, flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Determine size used for the nth kmalloc cache.
// /*AFLA*/  * return size or 0 if a kmalloc cache for that
// /*AFLA*/  * size does not exist
// /*AFLA*/  */
// /*AFLA*/ static __always_inline int kmalloc_size(int n)
// /*AFLA*/ {
// /*AFLA*/ #ifndef CONFIG_SLOB
// /*AFLA*/ 	if (n > 2)
// /*AFLA*/ 		return 1 << n;
// /*AFLA*/ 
// /*AFLA*/ 	if (n == 1 && KMALLOC_MIN_SIZE <= 32)
// /*AFLA*/ 		return 96;
// /*AFLA*/ 
// /*AFLA*/ 	if (n == 2 && KMALLOC_MIN_SIZE <= 64)
// /*AFLA*/ 		return 192;
// /*AFLA*/ #endif
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void *kmalloc_node(size_t size, gfp_t flags, int node)
// /*AFLA*/ {
// /*AFLA*/ #ifndef CONFIG_SLOB
// /*AFLA*/ 	if (__builtin_constant_p(size) &&
// /*AFLA*/ 		size <= KMALLOC_MAX_CACHE_SIZE && !(flags & GFP_DMA)) {
// /*AFLA*/ 		int i = kmalloc_index(size);
// /*AFLA*/ 
// /*AFLA*/ 		if (!i)
// /*AFLA*/ 			return ZERO_SIZE_PTR;
// /*AFLA*/ 
// /*AFLA*/ 		return kmem_cache_alloc_node_trace(kmalloc_caches[i],
// /*AFLA*/ 						flags, node, size);
// /*AFLA*/ 	}
// /*AFLA*/ #endif
// /*AFLA*/ 	return __kmalloc_node(size, flags, node);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ struct memcg_cache_array {
// /*AFLA*/ 	struct rcu_head rcu;
// /*AFLA*/ 	struct kmem_cache *entries[0];
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This is the main placeholder for memcg-related information in kmem caches.
// /*AFLA*/  * Both the root cache and the child caches will have it. For the root cache,
// /*AFLA*/  * this will hold a dynamically allocated array large enough to hold
// /*AFLA*/  * information about the currently limited memcgs in the system. To allow the
// /*AFLA*/  * array to be accessed without taking any locks, on relocation we free the old
// /*AFLA*/  * version only after a grace period.
// /*AFLA*/  *
// /*AFLA*/  * Child caches will hold extra metadata needed for its operation. Fields are:
// /*AFLA*/  *
// /*AFLA*/  * @memcg: pointer to the memcg this cache belongs to
// /*AFLA*/  * @root_cache: pointer to the global, root cache, this cache was derived from
// /*AFLA*/  *
// /*AFLA*/  * Both root and child caches of the same kind are linked into a list chained
// /*AFLA*/  * through @list.
// /*AFLA*/  */
// /*AFLA*/ struct memcg_cache_params {
// /*AFLA*/ 	bool is_root_cache;
// /*AFLA*/ 	struct list_head list;
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct memcg_cache_array __rcu *memcg_caches;
// /*AFLA*/ 		struct {
// /*AFLA*/ 			struct mem_cgroup *memcg;
// /*AFLA*/ 			struct kmem_cache *root_cache;
// /*AFLA*/ 		};
// /*AFLA*/ 	};
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ int memcg_update_all_caches(int num_memcgs);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * kmalloc_array - allocate memory for an array.
// /*AFLA*/  * @n: number of elements.
// /*AFLA*/  * @size: element size.
// /*AFLA*/  * @flags: the type of memory to allocate (see kmalloc).
// /*AFLA*/  */
// /*AFLA*/ static inline void *kmalloc_array(size_t n, size_t size, gfp_t flags)
// /*AFLA*/ {
// /*AFLA*/ 	if (size != 0 && n > SIZE_MAX / size)
// /*AFLA*/ 		return NULL;
// /*AFLA*/ 	if (__builtin_constant_p(n) && __builtin_constant_p(size))
// /*AFLA*/ 		return kmalloc(n * size, flags);
// /*AFLA*/ 	return __kmalloc(n * size, flags);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * kcalloc - allocate memory for an array. The memory is set to zero.
// /*AFLA*/  * @n: number of elements.
// /*AFLA*/  * @size: element size.
// /*AFLA*/  * @flags: the type of memory to allocate (see kmalloc).
// /*AFLA*/  */
// /*AFLA*/ static inline void *kcalloc(size_t n, size_t size, gfp_t flags)
// /*AFLA*/ {
// /*AFLA*/ 	return kmalloc_array(n, size, flags | __GFP_ZERO);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * kmalloc_track_caller is a special version of kmalloc that records the
// /*AFLA*/  * calling function of the routine calling it for slab leak tracking instead
// /*AFLA*/  * of just the calling function (confusing, eh?).
// /*AFLA*/  * It's useful when the call to kmalloc comes from a widely-used standard
// /*AFLA*/  * allocator where we care about the real place the memory allocation
// /*AFLA*/  * request comes from.
// /*AFLA*/  */
// /*AFLA*/ extern void *__kmalloc_track_caller(size_t, gfp_t, unsigned long);
// /*AFLA*/ #define kmalloc_track_caller(size, flags) \
// /*AFLA*/ 	__kmalloc_track_caller(size, flags, _RET_IP_)
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_NUMA
// /*AFLA*/ extern void *__kmalloc_node_track_caller(size_t, gfp_t, int, unsigned long);
// /*AFLA*/ #define kmalloc_node_track_caller(size, flags, node) \
// /*AFLA*/ 	__kmalloc_node_track_caller(size, flags, node, \
// /*AFLA*/ 			_RET_IP_)
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_NUMA */
// /*AFLA*/ 
// /*AFLA*/ #define kmalloc_node_track_caller(size, flags, node) \
// /*AFLA*/ 	kmalloc_track_caller(size, flags)
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_NUMA */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Shortcuts
// /*AFLA*/  */
// /*AFLA*/ static inline void *kmem_cache_zalloc(struct kmem_cache *k, gfp_t flags)
// /*AFLA*/ {
// /*AFLA*/ 	return kmem_cache_alloc(k, flags | __GFP_ZERO);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * kzalloc - allocate memory. The memory is set to zero.
// /*AFLA*/  * @size: how many bytes of memory are required.
// /*AFLA*/  * @flags: the type of memory to allocate (see kmalloc).
// /*AFLA*/  */
// /*AFLA*/ static inline void *kzalloc(size_t size, gfp_t flags)
// /*AFLA*/ {
// /*AFLA*/ 	return kmalloc(size, flags | __GFP_ZERO);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * kzalloc_node - allocate zeroed memory from a particular memory node.
// /*AFLA*/  * @size: how many bytes of memory are required.
// /*AFLA*/  * @flags: the type of memory to allocate (see kmalloc).
// /*AFLA*/  * @node: memory node from which to allocate
// /*AFLA*/  */
// /*AFLA*/ static inline void *kzalloc_node(size_t size, gfp_t flags, int node)
// /*AFLA*/ {
// /*AFLA*/ 	return kmalloc_node(size, flags | __GFP_ZERO, node);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ unsigned int kmem_cache_size(struct kmem_cache *s);
// /*AFLA*/ void __init kmem_cache_init_late(void);
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_SMP) && defined(CONFIG_SLAB)
// /*AFLA*/ int slab_prepare_cpu(unsigned int cpu);
// /*AFLA*/ int slab_dead_cpu(unsigned int cpu);
// /*AFLA*/ #else
// /*AFLA*/ #define slab_prepare_cpu	NULL
// /*AFLA*/ #define slab_dead_cpu		NULL
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif	/* _LINUX_SLAB_H */
