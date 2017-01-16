// /*AFLA*/ #ifndef _ASM_X86_SECTIONS_H
// /*AFLA*/ #define _ASM_X86_SECTIONS_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm-generic/sections.h>
// /*AFLA*/ #include <asm/extable.h>
// /*AFLA*/ 
// /*AFLA*/ extern char __brk_base[], __brk_limit[];
// /*AFLA*/ extern struct exception_table_entry __stop___ex_table[];
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_X86_64)
// /*AFLA*/ extern char __end_rodata_hpage_align[];
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif	/* _ASM_X86_SECTIONS_H */
