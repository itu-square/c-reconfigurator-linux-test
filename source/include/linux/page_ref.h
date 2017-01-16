// /*AFLA*/ #ifndef _LINUX_PAGE_REF_H
// /*AFLA*/ #define _LINUX_PAGE_REF_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <linux/mm_types.h>
// /*AFLA*/ #include <linux/page-flags.h>
// /*AFLA*/ #include <linux/tracepoint-defs.h>
// /*AFLA*/ 
// /*AFLA*/ extern struct tracepoint __tracepoint_page_ref_set;
// /*AFLA*/ extern struct tracepoint __tracepoint_page_ref_mod;
// /*AFLA*/ extern struct tracepoint __tracepoint_page_ref_mod_and_test;
// /*AFLA*/ extern struct tracepoint __tracepoint_page_ref_mod_and_return;
// /*AFLA*/ extern struct tracepoint __tracepoint_page_ref_mod_unless;
// /*AFLA*/ extern struct tracepoint __tracepoint_page_ref_freeze;
// /*AFLA*/ extern struct tracepoint __tracepoint_page_ref_unfreeze;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_PAGE_REF
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Ideally we would want to use the trace_<tracepoint>_enabled() helper
// /*AFLA*/  * functions. But due to include header file issues, that is not
// /*AFLA*/  * feasible. Instead we have to open code the static key functions.
// /*AFLA*/  *
// /*AFLA*/  * See trace_##name##_enabled(void) in include/linux/tracepoint.h
// /*AFLA*/  */
// /*AFLA*/ #define page_ref_tracepoint_active(t) static_key_false(&(t).key)
// /*AFLA*/ 
// /*AFLA*/ extern void __page_ref_set(struct page *page, int v);
// /*AFLA*/ extern void __page_ref_mod(struct page *page, int v);
// /*AFLA*/ extern void __page_ref_mod_and_test(struct page *page, int v, int ret);
// /*AFLA*/ extern void __page_ref_mod_and_return(struct page *page, int v, int ret);
// /*AFLA*/ extern void __page_ref_mod_unless(struct page *page, int v, int u);
// /*AFLA*/ extern void __page_ref_freeze(struct page *page, int v, int ret);
// /*AFLA*/ extern void __page_ref_unfreeze(struct page *page, int v);
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ #define page_ref_tracepoint_active(t) false
// /*AFLA*/ 
// /*AFLA*/ static inline void __page_ref_set(struct page *page, int v)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void __page_ref_mod(struct page *page, int v)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void __page_ref_mod_and_test(struct page *page, int v, int ret)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void __page_ref_mod_and_return(struct page *page, int v, int ret)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void __page_ref_mod_unless(struct page *page, int v, int u)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void __page_ref_freeze(struct page *page, int v, int ret)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void __page_ref_unfreeze(struct page *page, int v)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline int page_ref_count(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic_read(&page->_refcount);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int page_count(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic_read(&compound_head(page)->_refcount);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void set_page_count(struct page *page, int v)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_set(&page->_refcount, v);
// /*AFLA*/ 	if (page_ref_tracepoint_active(__tracepoint_page_ref_set))
// /*AFLA*/ 		__page_ref_set(page, v);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Setup the page count before being freed into the page allocator for
// /*AFLA*/  * the first time (boot or memory hotplug)
// /*AFLA*/  */
// /*AFLA*/ static inline void init_page_count(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	set_page_count(page, 1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void page_ref_add(struct page *page, int nr)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_add(nr, &page->_refcount);
// /*AFLA*/ 	if (page_ref_tracepoint_active(__tracepoint_page_ref_mod))
// /*AFLA*/ 		__page_ref_mod(page, nr);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void page_ref_sub(struct page *page, int nr)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_sub(nr, &page->_refcount);
// /*AFLA*/ 	if (page_ref_tracepoint_active(__tracepoint_page_ref_mod))
// /*AFLA*/ 		__page_ref_mod(page, -nr);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void page_ref_inc(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_inc(&page->_refcount);
// /*AFLA*/ 	if (page_ref_tracepoint_active(__tracepoint_page_ref_mod))
// /*AFLA*/ 		__page_ref_mod(page, 1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void page_ref_dec(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	atomic_dec(&page->_refcount);
// /*AFLA*/ 	if (page_ref_tracepoint_active(__tracepoint_page_ref_mod))
// /*AFLA*/ 		__page_ref_mod(page, -1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int page_ref_sub_and_test(struct page *page, int nr)
// /*AFLA*/ {
// /*AFLA*/ 	int ret = atomic_sub_and_test(nr, &page->_refcount);
// /*AFLA*/ 
// /*AFLA*/ 	if (page_ref_tracepoint_active(__tracepoint_page_ref_mod_and_test))
// /*AFLA*/ 		__page_ref_mod_and_test(page, -nr, ret);
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int page_ref_inc_return(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	int ret = atomic_inc_return(&page->_refcount);
// /*AFLA*/ 
// /*AFLA*/ 	if (page_ref_tracepoint_active(__tracepoint_page_ref_mod_and_return))
// /*AFLA*/ 		__page_ref_mod_and_return(page, 1, ret);
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int page_ref_dec_and_test(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	int ret = atomic_dec_and_test(&page->_refcount);
// /*AFLA*/ 
// /*AFLA*/ 	if (page_ref_tracepoint_active(__tracepoint_page_ref_mod_and_test))
// /*AFLA*/ 		__page_ref_mod_and_test(page, -1, ret);
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int page_ref_dec_return(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	int ret = atomic_dec_return(&page->_refcount);
// /*AFLA*/ 
// /*AFLA*/ 	if (page_ref_tracepoint_active(__tracepoint_page_ref_mod_and_return))
// /*AFLA*/ 		__page_ref_mod_and_return(page, -1, ret);
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int page_ref_add_unless(struct page *page, int nr, int u)
// /*AFLA*/ {
// /*AFLA*/ 	int ret = atomic_add_unless(&page->_refcount, nr, u);
// /*AFLA*/ 
// /*AFLA*/ 	if (page_ref_tracepoint_active(__tracepoint_page_ref_mod_unless))
// /*AFLA*/ 		__page_ref_mod_unless(page, nr, ret);
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int page_ref_freeze(struct page *page, int count)
// /*AFLA*/ {
// /*AFLA*/ 	int ret = likely(atomic_cmpxchg(&page->_refcount, count, 0) == count);
// /*AFLA*/ 
// /*AFLA*/ 	if (page_ref_tracepoint_active(__tracepoint_page_ref_freeze))
// /*AFLA*/ 		__page_ref_freeze(page, count, ret);
// /*AFLA*/ 	return ret;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void page_ref_unfreeze(struct page *page, int count)
// /*AFLA*/ {
// /*AFLA*/ 	VM_BUG_ON_PAGE(page_count(page) != 0, page);
// /*AFLA*/ 	VM_BUG_ON(count == 0);
// /*AFLA*/ 
// /*AFLA*/ 	atomic_set(&page->_refcount, count);
// /*AFLA*/ 	if (page_ref_tracepoint_active(__tracepoint_page_ref_unfreeze))
// /*AFLA*/ 		__page_ref_unfreeze(page, count);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif
