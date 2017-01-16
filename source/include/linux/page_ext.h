// /*AFLA*/ #ifndef __LINUX_PAGE_EXT_H
// /*AFLA*/ #define __LINUX_PAGE_EXT_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/stacktrace.h>
// /*AFLA*/ #include <linux/stackdepot.h>
// /*AFLA*/ 
// /*AFLA*/ struct pglist_data;
// /*AFLA*/ struct page_ext_operations {
// /*AFLA*/ 	size_t offset;
// /*AFLA*/ 	size_t size;
// /*AFLA*/ 	bool (*need)(void);
// /*AFLA*/ 	void (*init)(void);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PAGE_EXTENSION
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * page_ext->flags bits:
// /*AFLA*/  *
// /*AFLA*/  * PAGE_EXT_DEBUG_POISON is set for poisoned pages. This is used to
// /*AFLA*/  * implement generic debug pagealloc feature. The pages are filled with
// /*AFLA*/  * poison patterns and set this flag after free_pages(). The poisoned
// /*AFLA*/  * pages are verified whether the patterns are not corrupted and clear
// /*AFLA*/  * the flag before alloc_pages().
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ enum page_ext_flags {
// /*AFLA*/ 	PAGE_EXT_DEBUG_POISON,		/* Page is poisoned */
// /*AFLA*/ 	PAGE_EXT_DEBUG_GUARD,
// /*AFLA*/ 	PAGE_EXT_OWNER,
// /*AFLA*/ #if defined(CONFIG_IDLE_PAGE_TRACKING) && !defined(CONFIG_64BIT)
// /*AFLA*/ 	PAGE_EXT_YOUNG,
// /*AFLA*/ 	PAGE_EXT_IDLE,
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Page Extension can be considered as an extended mem_map.
// /*AFLA*/  * A page_ext page is associated with every page descriptor. The
// /*AFLA*/  * page_ext helps us add more information about the page.
// /*AFLA*/  * All page_ext are allocated at boot or memory hotplug event,
// /*AFLA*/  * then the page_ext for pfn always exists.
// /*AFLA*/  */
// /*AFLA*/ struct page_ext {
// /*AFLA*/ 	unsigned long flags;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern void pgdat_page_ext_init(struct pglist_data *pgdat);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SPARSEMEM
// /*AFLA*/ static inline void page_ext_init_flatmem(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ extern void page_ext_init(void);
// /*AFLA*/ #else
// /*AFLA*/ extern void page_ext_init_flatmem(void);
// /*AFLA*/ static inline void page_ext_init(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct page_ext *lookup_page_ext(struct page *page);
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_PAGE_EXTENSION */
// /*AFLA*/ struct page_ext;
// /*AFLA*/ 
// /*AFLA*/ static inline void pgdat_page_ext_init(struct pglist_data *pgdat)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct page_ext *lookup_page_ext(struct page *page)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void page_ext_init(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void page_ext_init_flatmem(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_PAGE_EXTENSION */
// /*AFLA*/ #endif /* __LINUX_PAGE_EXT_H */
