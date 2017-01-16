// /*AFLA*/ #ifndef _ASM_X86_SPARSEMEM_H
// /*AFLA*/ #define _ASM_X86_SPARSEMEM_H
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_SPARSEMEM
// /*AFLA*/ /*
// /*AFLA*/  * generic non-linear memory support:
// /*AFLA*/  *
// /*AFLA*/  * 1) we will not split memory into more chunks than will fit into the flags
// /*AFLA*/  *    field of the struct page
// /*AFLA*/  *
// /*AFLA*/  * SECTION_SIZE_BITS		2^n: size of each section
// /*AFLA*/  * MAX_PHYSADDR_BITS		2^n: max size of physical address space
// /*AFLA*/  * MAX_PHYSMEM_BITS		2^n: how much memory we can have in that space
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ # ifdef CONFIG_X86_PAE
// /*AFLA*/ #  define SECTION_SIZE_BITS	29
// /*AFLA*/ #  define MAX_PHYSADDR_BITS	36
// /*AFLA*/ #  define MAX_PHYSMEM_BITS	36
// /*AFLA*/ # else
// /*AFLA*/ #  define SECTION_SIZE_BITS	26
// /*AFLA*/ #  define MAX_PHYSADDR_BITS	32
// /*AFLA*/ #  define MAX_PHYSMEM_BITS	32
// /*AFLA*/ # endif
// /*AFLA*/ #else /* CONFIG_X86_32 */
// /*AFLA*/ # define SECTION_SIZE_BITS	27 /* matt - 128 is convenient right now */
// /*AFLA*/ # define MAX_PHYSADDR_BITS	44
// /*AFLA*/ # define MAX_PHYSMEM_BITS	46
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_SPARSEMEM */
// /*AFLA*/ #endif /* _ASM_X86_SPARSEMEM_H */
