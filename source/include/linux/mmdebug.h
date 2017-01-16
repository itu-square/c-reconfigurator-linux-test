// /*AFLA*/ #ifndef LINUX_MM_DEBUG_H
// /*AFLA*/ #define LINUX_MM_DEBUG_H 1
// /*AFLA*/ 
// /*AFLA*/ #include <linux/bug.h>
// /*AFLA*/ #include <linux/stringify.h>
// /*AFLA*/ 
// /*AFLA*/ struct page;
// /*AFLA*/ struct vm_area_struct;
// /*AFLA*/ struct mm_struct;
// /*AFLA*/ 
// /*AFLA*/ extern void dump_page(struct page *page, const char *reason);
// /*AFLA*/ extern void __dump_page(struct page *page, const char *reason);
// /*AFLA*/ void dump_vma(const struct vm_area_struct *vma);
// /*AFLA*/ void dump_mm(const struct mm_struct *mm);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_VM
// /*AFLA*/ #define VM_BUG_ON(cond) BUG_ON(cond)
// /*AFLA*/ #define VM_BUG_ON_PAGE(cond, page)					\
// /*AFLA*/ 	do {								\
// /*AFLA*/ 		if (unlikely(cond)) {					\
// /*AFLA*/ 			dump_page(page, "VM_BUG_ON_PAGE(" __stringify(cond)")");\
// /*AFLA*/ 			BUG();						\
// /*AFLA*/ 		}							\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ #define VM_BUG_ON_VMA(cond, vma)					\
// /*AFLA*/ 	do {								\
// /*AFLA*/ 		if (unlikely(cond)) {					\
// /*AFLA*/ 			dump_vma(vma);					\
// /*AFLA*/ 			BUG();						\
// /*AFLA*/ 		}							\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ #define VM_BUG_ON_MM(cond, mm)						\
// /*AFLA*/ 	do {								\
// /*AFLA*/ 		if (unlikely(cond)) {					\
// /*AFLA*/ 			dump_mm(mm);					\
// /*AFLA*/ 			BUG();						\
// /*AFLA*/ 		}							\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ #define VM_WARN_ON(cond) WARN_ON(cond)
// /*AFLA*/ #define VM_WARN_ON_ONCE(cond) WARN_ON_ONCE(cond)
// /*AFLA*/ #define VM_WARN_ONCE(cond, format...) WARN_ONCE(cond, format)
// /*AFLA*/ #define VM_WARN(cond, format...) WARN(cond, format)
// /*AFLA*/ #else
// /*AFLA*/ #define VM_BUG_ON(cond) BUILD_BUG_ON_INVALID(cond)
// /*AFLA*/ #define VM_BUG_ON_PAGE(cond, page) VM_BUG_ON(cond)
// /*AFLA*/ #define VM_BUG_ON_VMA(cond, vma) VM_BUG_ON(cond)
// /*AFLA*/ #define VM_BUG_ON_MM(cond, mm) VM_BUG_ON(cond)
// /*AFLA*/ #define VM_WARN_ON(cond) BUILD_BUG_ON_INVALID(cond)
// /*AFLA*/ #define VM_WARN_ON_ONCE(cond) BUILD_BUG_ON_INVALID(cond)
// /*AFLA*/ #define VM_WARN_ONCE(cond, format...) BUILD_BUG_ON_INVALID(cond)
// /*AFLA*/ #define VM_WARN(cond, format...) BUILD_BUG_ON_INVALID(cond)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_VIRTUAL
// /*AFLA*/ #define VIRTUAL_BUG_ON(cond) BUG_ON(cond)
// /*AFLA*/ #else
// /*AFLA*/ #define VIRTUAL_BUG_ON(cond) do { } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_VM_PGFLAGS
// /*AFLA*/ #define VM_BUG_ON_PGFLAGS(cond, page) VM_BUG_ON_PAGE(cond, page)
// /*AFLA*/ #else
// /*AFLA*/ #define VM_BUG_ON_PGFLAGS(cond, page) BUILD_BUG_ON_INVALID(cond)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif
