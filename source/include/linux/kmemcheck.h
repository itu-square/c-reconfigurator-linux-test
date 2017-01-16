// /*AFLA*/ #ifndef LINUX_KMEMCHECK_H
// /*AFLA*/ #define LINUX_KMEMCHECK_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/mm_types.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_KMEMCHECK
// /*AFLA*/ extern int kmemcheck_enabled;
// /*AFLA*/ 
// /*AFLA*/ /* The slab-related functions. */
// /*AFLA*/ void kmemcheck_alloc_shadow(struct page *page, int order, gfp_t flags, int node);
// /*AFLA*/ void kmemcheck_free_shadow(struct page *page, int order);
// /*AFLA*/ void kmemcheck_slab_alloc(struct kmem_cache *s, gfp_t gfpflags, void *object,
// /*AFLA*/ 			  size_t size);
// /*AFLA*/ void kmemcheck_slab_free(struct kmem_cache *s, void *object, size_t size);
// /*AFLA*/ 
// /*AFLA*/ void kmemcheck_pagealloc_alloc(struct page *p, unsigned int order,
// /*AFLA*/ 			       gfp_t gfpflags);
// /*AFLA*/ 
// /*AFLA*/ void kmemcheck_show_pages(struct page *p, unsigned int n);
// /*AFLA*/ void kmemcheck_hide_pages(struct page *p, unsigned int n);
// /*AFLA*/ 
// /*AFLA*/ bool kmemcheck_page_is_tracked(struct page *p);
// /*AFLA*/ 
// /*AFLA*/ void kmemcheck_mark_unallocated(void *address, unsigned int n);
// /*AFLA*/ void kmemcheck_mark_uninitialized(void *address, unsigned int n);
// /*AFLA*/ void kmemcheck_mark_initialized(void *address, unsigned int n);
// /*AFLA*/ void kmemcheck_mark_freed(void *address, unsigned int n);
// /*AFLA*/ 
// /*AFLA*/ void kmemcheck_mark_unallocated_pages(struct page *p, unsigned int n);
// /*AFLA*/ void kmemcheck_mark_uninitialized_pages(struct page *p, unsigned int n);
// /*AFLA*/ void kmemcheck_mark_initialized_pages(struct page *p, unsigned int n);
// /*AFLA*/ 
// /*AFLA*/ int kmemcheck_show_addr(unsigned long address);
// /*AFLA*/ int kmemcheck_hide_addr(unsigned long address);
// /*AFLA*/ 
// /*AFLA*/ bool kmemcheck_is_obj_initialized(unsigned long addr, size_t size);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Bitfield annotations
// /*AFLA*/  *
// /*AFLA*/  * How to use: If you have a struct using bitfields, for example
// /*AFLA*/  *
// /*AFLA*/  *     struct a {
// /*AFLA*/  *             int x:8, y:8;
// /*AFLA*/  *     };
// /*AFLA*/  *
// /*AFLA*/  * then this should be rewritten as
// /*AFLA*/  *
// /*AFLA*/  *     struct a {
// /*AFLA*/  *             kmemcheck_bitfield_begin(flags);
// /*AFLA*/  *             int x:8, y:8;
// /*AFLA*/  *             kmemcheck_bitfield_end(flags);
// /*AFLA*/  *     };
// /*AFLA*/  *
// /*AFLA*/  * Now the "flags_begin" and "flags_end" members may be used to refer to the
// /*AFLA*/  * beginning and end, respectively, of the bitfield (and things like
// /*AFLA*/  * &x.flags_begin is allowed). As soon as the struct is allocated, the bit-
// /*AFLA*/  * fields should be annotated:
// /*AFLA*/  *
// /*AFLA*/  *     struct a *a = kmalloc(sizeof(struct a), GFP_KERNEL);
// /*AFLA*/  *     kmemcheck_annotate_bitfield(a, flags);
// /*AFLA*/  */
// /*AFLA*/ #define kmemcheck_bitfield_begin(name)	\
// /*AFLA*/ 	int name##_begin[0];
// /*AFLA*/ 
// /*AFLA*/ #define kmemcheck_bitfield_end(name)	\
// /*AFLA*/ 	int name##_end[0];
// /*AFLA*/ 
// /*AFLA*/ #define kmemcheck_annotate_bitfield(ptr, name)				\
// /*AFLA*/ 	do {								\
// /*AFLA*/ 		int _n;							\
// /*AFLA*/ 									\
// /*AFLA*/ 		if (!ptr)						\
// /*AFLA*/ 			break;						\
// /*AFLA*/ 									\
// /*AFLA*/ 		_n = (long) &((ptr)->name##_end)			\
// /*AFLA*/ 			- (long) &((ptr)->name##_begin);		\
// /*AFLA*/ 		BUILD_BUG_ON(_n < 0);					\
// /*AFLA*/ 									\
// /*AFLA*/ 		kmemcheck_mark_initialized(&((ptr)->name##_begin), _n);	\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #define kmemcheck_annotate_variable(var)				\
// /*AFLA*/ 	do {								\
// /*AFLA*/ 		kmemcheck_mark_initialized(&(var), sizeof(var));	\
// /*AFLA*/ 	} while (0)							\
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ #define kmemcheck_enabled 0
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ kmemcheck_alloc_shadow(struct page *page, int order, gfp_t flags, int node)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ kmemcheck_free_shadow(struct page *page, int order)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ kmemcheck_slab_alloc(struct kmem_cache *s, gfp_t gfpflags, void *object,
// /*AFLA*/ 		     size_t size)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void kmemcheck_slab_free(struct kmem_cache *s, void *object,
// /*AFLA*/ 				       size_t size)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void kmemcheck_pagealloc_alloc(struct page *p,
// /*AFLA*/ 	unsigned int order, gfp_t gfpflags)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool kmemcheck_page_is_tracked(struct page *p)
// /*AFLA*/ {
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void kmemcheck_mark_unallocated(void *address, unsigned int n)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void kmemcheck_mark_uninitialized(void *address, unsigned int n)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void kmemcheck_mark_initialized(void *address, unsigned int n)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void kmemcheck_mark_freed(void *address, unsigned int n)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void kmemcheck_mark_unallocated_pages(struct page *p,
// /*AFLA*/ 						    unsigned int n)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void kmemcheck_mark_uninitialized_pages(struct page *p,
// /*AFLA*/ 						      unsigned int n)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void kmemcheck_mark_initialized_pages(struct page *p,
// /*AFLA*/ 						    unsigned int n)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool kmemcheck_is_obj_initialized(unsigned long addr, size_t size)
// /*AFLA*/ {
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define kmemcheck_bitfield_begin(name)
// /*AFLA*/ #define kmemcheck_bitfield_end(name)
// /*AFLA*/ #define kmemcheck_annotate_bitfield(ptr, name)	\
// /*AFLA*/ 	do {					\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #define kmemcheck_annotate_variable(var)	\
// /*AFLA*/ 	do {					\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_KMEMCHECK */
// /*AFLA*/ 
// /*AFLA*/ #endif /* LINUX_KMEMCHECK_H */
