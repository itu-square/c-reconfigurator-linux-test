// /*AFLA*/ #ifndef _ASM_KASLR_H_
// /*AFLA*/ #define _ASM_KASLR_H_
// /*AFLA*/ 
// /*AFLA*/ unsigned long kaslr_get_random_long(const char *purpose);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_RANDOMIZE_MEMORY
// /*AFLA*/ extern unsigned long page_offset_base;
// /*AFLA*/ extern unsigned long vmalloc_base;
// /*AFLA*/ extern unsigned long vmemmap_base;
// /*AFLA*/ 
// /*AFLA*/ void kernel_randomize_memory(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline void kernel_randomize_memory(void) { }
// /*AFLA*/ #endif /* CONFIG_RANDOMIZE_MEMORY */
// /*AFLA*/ 
// /*AFLA*/ #endif
