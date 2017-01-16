// /*AFLA*/ #ifndef _LINUX_KASAN_H
// /*AFLA*/ #define _LINUX_KASAN_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/sched.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ struct kmem_cache;
// /*AFLA*/ struct page;
// /*AFLA*/ struct vm_struct;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_KASAN
// /*AFLA*/ 
// /*AFLA*/ #define KASAN_SHADOW_SCALE_SHIFT 3
// /*AFLA*/ 
// /*AFLA*/ #include <asm/kasan.h>
// /*AFLA*/ #include <asm/pgtable.h>
// /*AFLA*/ 
// /*AFLA*/ extern unsigned char kasan_zero_page[PAGE_SIZE];
// /*AFLA*/ extern pte_t kasan_zero_pte[PTRS_PER_PTE];
// /*AFLA*/ extern pmd_t kasan_zero_pmd[PTRS_PER_PMD];
// /*AFLA*/ extern pud_t kasan_zero_pud[PTRS_PER_PUD];
// /*AFLA*/ 
// /*AFLA*/ void kasan_populate_zero_shadow(const void *shadow_start,
// /*AFLA*/ 				const void *shadow_end);
// /*AFLA*/ 
// /*AFLA*/ static inline void *kasan_mem_to_shadow(const void *addr)
// /*AFLA*/ {
// /*AFLA*/ 	return (void *)((unsigned long)addr >> KASAN_SHADOW_SCALE_SHIFT)
// /*AFLA*/ 		+ KASAN_SHADOW_OFFSET;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Enable reporting bugs after kasan_disable_current() */
// /*AFLA*/ static inline void kasan_enable_current(void)
// /*AFLA*/ {
// /*AFLA*/ 	current->kasan_depth++;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Disable reporting bugs for current task */
// /*AFLA*/ static inline void kasan_disable_current(void)
// /*AFLA*/ {
// /*AFLA*/ 	current->kasan_depth--;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ void kasan_unpoison_shadow(const void *address, size_t size);
// /*AFLA*/ 
// /*AFLA*/ void kasan_unpoison_task_stack(struct task_struct *task);
// /*AFLA*/ void kasan_unpoison_stack_above_sp_to(const void *watermark);
// /*AFLA*/ 
// /*AFLA*/ void kasan_alloc_pages(struct page *page, unsigned int order);
// /*AFLA*/ void kasan_free_pages(struct page *page, unsigned int order);
// /*AFLA*/ 
// /*AFLA*/ void kasan_cache_create(struct kmem_cache *cache, size_t *size,
// /*AFLA*/ 			unsigned long *flags);
// /*AFLA*/ void kasan_cache_shrink(struct kmem_cache *cache);
// /*AFLA*/ void kasan_cache_destroy(struct kmem_cache *cache);
// /*AFLA*/ 
// /*AFLA*/ void kasan_poison_slab(struct page *page);
// /*AFLA*/ void kasan_unpoison_object_data(struct kmem_cache *cache, void *object);
// /*AFLA*/ void kasan_poison_object_data(struct kmem_cache *cache, void *object);
// /*AFLA*/ void kasan_init_slab_obj(struct kmem_cache *cache, const void *object);
// /*AFLA*/ 
// /*AFLA*/ void kasan_kmalloc_large(const void *ptr, size_t size, gfp_t flags);
// /*AFLA*/ void kasan_kfree_large(const void *ptr);
// /*AFLA*/ void kasan_poison_kfree(void *ptr);
// /*AFLA*/ void kasan_kmalloc(struct kmem_cache *s, const void *object, size_t size,
// /*AFLA*/ 		  gfp_t flags);
// /*AFLA*/ void kasan_krealloc(const void *object, size_t new_size, gfp_t flags);
// /*AFLA*/ 
// /*AFLA*/ void kasan_slab_alloc(struct kmem_cache *s, void *object, gfp_t flags);
// /*AFLA*/ bool kasan_slab_free(struct kmem_cache *s, void *object);
// /*AFLA*/ 
// /*AFLA*/ struct kasan_cache {
// /*AFLA*/ 	int alloc_meta_offset;
// /*AFLA*/ 	int free_meta_offset;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ int kasan_module_alloc(void *addr, size_t size);
// /*AFLA*/ void kasan_free_shadow(const struct vm_struct *vm);
// /*AFLA*/ 
// /*AFLA*/ size_t ksize(const void *);
// /*AFLA*/ static inline void kasan_unpoison_slab(const void *ptr) { ksize(ptr); }
// /*AFLA*/ size_t kasan_metadata_size(struct kmem_cache *cache);
// /*AFLA*/ 
// /*AFLA*/ #else /* CONFIG_KASAN */
// /*AFLA*/ 
// /*AFLA*/ static inline void kasan_unpoison_shadow(const void *address, size_t size) {}
// /*AFLA*/ 
// /*AFLA*/ static inline void kasan_unpoison_task_stack(struct task_struct *task) {}
// /*AFLA*/ static inline void kasan_unpoison_stack_above_sp_to(const void *watermark) {}
// /*AFLA*/ 
// /*AFLA*/ static inline void kasan_enable_current(void) {}
// /*AFLA*/ static inline void kasan_disable_current(void) {}
// /*AFLA*/ 
// /*AFLA*/ static inline void kasan_alloc_pages(struct page *page, unsigned int order) {}
// /*AFLA*/ static inline void kasan_free_pages(struct page *page, unsigned int order) {}
// /*AFLA*/ 
// /*AFLA*/ static inline void kasan_cache_create(struct kmem_cache *cache,
// /*AFLA*/ 				      size_t *size,
// /*AFLA*/ 				      unsigned long *flags) {}
// /*AFLA*/ static inline void kasan_cache_shrink(struct kmem_cache *cache) {}
// /*AFLA*/ static inline void kasan_cache_destroy(struct kmem_cache *cache) {}
// /*AFLA*/ 
// /*AFLA*/ static inline void kasan_poison_slab(struct page *page) {}
// /*AFLA*/ static inline void kasan_unpoison_object_data(struct kmem_cache *cache,
// /*AFLA*/ 					void *object) {}
// /*AFLA*/ static inline void kasan_poison_object_data(struct kmem_cache *cache,
// /*AFLA*/ 					void *object) {}
// /*AFLA*/ static inline void kasan_init_slab_obj(struct kmem_cache *cache,
// /*AFLA*/ 				const void *object) {}
// /*AFLA*/ 
// /*AFLA*/ static inline void kasan_kmalloc_large(void *ptr, size_t size, gfp_t flags) {}
// /*AFLA*/ static inline void kasan_kfree_large(const void *ptr) {}
// /*AFLA*/ static inline void kasan_poison_kfree(void *ptr) {}
// /*AFLA*/ static inline void kasan_kmalloc(struct kmem_cache *s, const void *object,
// /*AFLA*/ 				size_t size, gfp_t flags) {}
// /*AFLA*/ static inline void kasan_krealloc(const void *object, size_t new_size,
// /*AFLA*/ 				 gfp_t flags) {}
// /*AFLA*/ 
// /*AFLA*/ static inline void kasan_slab_alloc(struct kmem_cache *s, void *object,
// /*AFLA*/ 				   gfp_t flags) {}
// /*AFLA*/ static inline bool kasan_slab_free(struct kmem_cache *s, void *object)
// /*AFLA*/ {
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int kasan_module_alloc(void *addr, size_t size) { return 0; }
// /*AFLA*/ static inline void kasan_free_shadow(const struct vm_struct *vm) {}
// /*AFLA*/ 
// /*AFLA*/ static inline void kasan_unpoison_slab(const void *ptr) { }
// /*AFLA*/ static inline size_t kasan_metadata_size(struct kmem_cache *cache) { return 0; }
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_KASAN */
// /*AFLA*/ 
// /*AFLA*/ #endif /* LINUX_KASAN_H */
