// /*AFLA*/ #ifndef _LINUX_LINKAGE_H
// /*AFLA*/ #define _LINUX_LINKAGE_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/stringify.h>
// /*AFLA*/ #include <linux/export.h>
// /*AFLA*/ #include <asm/linkage.h>
// /*AFLA*/ 
// /*AFLA*/ /* Some toolchains use other characters (e.g. '`') to mark new line in macro */
// /*AFLA*/ #ifndef ASM_NL
// /*AFLA*/ #define ASM_NL		 ;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef __cplusplus
// /*AFLA*/ #define CPP_ASMLINKAGE extern "C"
// /*AFLA*/ #else
// /*AFLA*/ #define CPP_ASMLINKAGE
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef asmlinkage
// /*AFLA*/ #define asmlinkage CPP_ASMLINKAGE
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef cond_syscall
// /*AFLA*/ #define cond_syscall(x)	asm(				\
// /*AFLA*/ 	".weak " VMLINUX_SYMBOL_STR(x) "\n\t"		\
// /*AFLA*/ 	".set  " VMLINUX_SYMBOL_STR(x) ","		\
// /*AFLA*/ 		 VMLINUX_SYMBOL_STR(sys_ni_syscall))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef SYSCALL_ALIAS
// /*AFLA*/ #define SYSCALL_ALIAS(alias, name) asm(			\
// /*AFLA*/ 	".globl " VMLINUX_SYMBOL_STR(alias) "\n\t"	\
// /*AFLA*/ 	".set   " VMLINUX_SYMBOL_STR(alias) ","		\
// /*AFLA*/ 		  VMLINUX_SYMBOL_STR(name))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define __page_aligned_data	__section(.data..page_aligned) __aligned(PAGE_SIZE)
// /*AFLA*/ #define __page_aligned_bss	__section(.bss..page_aligned) __aligned(PAGE_SIZE)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * For assembly routines.
// /*AFLA*/  *
// /*AFLA*/  * Note when using these that you must specify the appropriate
// /*AFLA*/  * alignment directives yourself
// /*AFLA*/  */
// /*AFLA*/ #define __PAGE_ALIGNED_DATA	.section ".data..page_aligned", "aw"
// /*AFLA*/ #define __PAGE_ALIGNED_BSS	.section ".bss..page_aligned", "aw"
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * This is used by architectures to keep arguments on the stack
// /*AFLA*/  * untouched by the compiler by keeping them live until the end.
// /*AFLA*/  * The argument stack may be owned by the assembly-language
// /*AFLA*/  * caller, not the callee, and gcc doesn't always understand
// /*AFLA*/  * that.
// /*AFLA*/  *
// /*AFLA*/  * We have the return value, and a maximum of six arguments.
// /*AFLA*/  *
// /*AFLA*/  * This should always be followed by a "return ret" for the
// /*AFLA*/  * protection to work (ie no more work that the compiler might
// /*AFLA*/  * end up needing stack temporaries for).
// /*AFLA*/  */
// /*AFLA*/ /* Assembly files may be compiled with -traditional .. */
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ #ifndef asmlinkage_protect
// /*AFLA*/ # define asmlinkage_protect(n, ret, args...)	do { } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ALIGN
// /*AFLA*/ #define __ALIGN		.align 4,0x90
// /*AFLA*/ #define __ALIGN_STR	".align 4,0x90"
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ #ifndef LINKER_SCRIPT
// /*AFLA*/ #define ALIGN __ALIGN
// /*AFLA*/ #define ALIGN_STR __ALIGN_STR
// /*AFLA*/ 
// /*AFLA*/ #ifndef ENTRY
// /*AFLA*/ #define ENTRY(name) \
// /*AFLA*/ 	.globl name ASM_NL \
// /*AFLA*/ 	ALIGN ASM_NL \
// /*AFLA*/ 	name:
// /*AFLA*/ #endif
// /*AFLA*/ #endif /* LINKER_SCRIPT */
// /*AFLA*/ 
// /*AFLA*/ #ifndef WEAK
// /*AFLA*/ #define WEAK(name)	   \
// /*AFLA*/ 	.weak name ASM_NL   \
// /*AFLA*/ 	name:
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef END
// /*AFLA*/ #define END(name) \
// /*AFLA*/ 	.size name, .-name
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* If symbol 'name' is treated as a subroutine (gets called, and returns)
// /*AFLA*/  * then please use ENDPROC to mark 'name' as STT_FUNC for the benefit of
// /*AFLA*/  * static analysis tools such as stack depth analyzer.
// /*AFLA*/  */
// /*AFLA*/ #ifndef ENDPROC
// /*AFLA*/ #define ENDPROC(name) \
// /*AFLA*/ 	.type name, @function ASM_NL \
// /*AFLA*/ 	END(name)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif
