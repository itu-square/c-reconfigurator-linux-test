// /*AFLA*/ #ifndef _ASM_X86_ASM_H
// /*AFLA*/ #define _ASM_X86_ASM_H
// /*AFLA*/ 
// /*AFLA*/ #ifdef __ASSEMBLY__
// /*AFLA*/ # define __ASM_FORM(x)	x
// /*AFLA*/ # define __ASM_FORM_RAW(x)     x
// /*AFLA*/ # define __ASM_FORM_COMMA(x) x,
// /*AFLA*/ #else
// /*AFLA*/ # define __ASM_FORM(x)	" " #x " "
// /*AFLA*/ # define __ASM_FORM_RAW(x)     #x
// /*AFLA*/ # define __ASM_FORM_COMMA(x) " " #x ","
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ # define __ASM_SEL(a,b) __ASM_FORM(a)
// /*AFLA*/ # define __ASM_SEL_RAW(a,b) __ASM_FORM_RAW(a)
// /*AFLA*/ #else
// /*AFLA*/ # define __ASM_SEL(a,b) __ASM_FORM(b)
// /*AFLA*/ # define __ASM_SEL_RAW(a,b) __ASM_FORM_RAW(b)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define __ASM_SIZE(inst, ...)	__ASM_SEL(inst##l##__VA_ARGS__, \
// /*AFLA*/ 					  inst##q##__VA_ARGS__)
// /*AFLA*/ #define __ASM_REG(reg)         __ASM_SEL_RAW(e##reg, r##reg)
// /*AFLA*/ 
// /*AFLA*/ #define _ASM_PTR	__ASM_SEL(.long, .quad)
// /*AFLA*/ #define _ASM_ALIGN	__ASM_SEL(.balign 4, .balign 8)
// /*AFLA*/ 
// /*AFLA*/ #define _ASM_MOV	__ASM_SIZE(mov)
// /*AFLA*/ #define _ASM_INC	__ASM_SIZE(inc)
// /*AFLA*/ #define _ASM_DEC	__ASM_SIZE(dec)
// /*AFLA*/ #define _ASM_ADD	__ASM_SIZE(add)
// /*AFLA*/ #define _ASM_SUB	__ASM_SIZE(sub)
// /*AFLA*/ #define _ASM_XADD	__ASM_SIZE(xadd)
// /*AFLA*/ 
// /*AFLA*/ #define _ASM_AX		__ASM_REG(ax)
// /*AFLA*/ #define _ASM_BX		__ASM_REG(bx)
// /*AFLA*/ #define _ASM_CX		__ASM_REG(cx)
// /*AFLA*/ #define _ASM_DX		__ASM_REG(dx)
// /*AFLA*/ #define _ASM_SP		__ASM_REG(sp)
// /*AFLA*/ #define _ASM_BP		__ASM_REG(bp)
// /*AFLA*/ #define _ASM_SI		__ASM_REG(si)
// /*AFLA*/ #define _ASM_DI		__ASM_REG(di)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Macros to generate condition code outputs from inline assembly,
// /*AFLA*/  * The output operand must be type "bool".
// /*AFLA*/  */
// /*AFLA*/ #ifdef __GCC_ASM_FLAG_OUTPUTS__
// /*AFLA*/ # define CC_SET(c) "\n\t/* output condition code " #c "*/\n"
// /*AFLA*/ # define CC_OUT(c) "=@cc" #c
// /*AFLA*/ #else
// /*AFLA*/ # define CC_SET(c) "\n\tset" #c " %[_cc_" #c "]\n"
// /*AFLA*/ # define CC_OUT(c) [_cc_ ## c] "=qm"
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Exception table entry */
// /*AFLA*/ #ifdef __ASSEMBLY__
// /*AFLA*/ # define _ASM_EXTABLE_HANDLE(from, to, handler)			\
// /*AFLA*/ 	.pushsection "__ex_table","a" ;				\
// /*AFLA*/ 	.balign 4 ;						\
// /*AFLA*/ 	.long (from) - . ;					\
// /*AFLA*/ 	.long (to) - . ;					\
// /*AFLA*/ 	.long (handler) - . ;					\
// /*AFLA*/ 	.popsection
// /*AFLA*/ 
// /*AFLA*/ # define _ASM_EXTABLE(from, to)					\
// /*AFLA*/ 	_ASM_EXTABLE_HANDLE(from, to, ex_handler_default)
// /*AFLA*/ 
// /*AFLA*/ # define _ASM_EXTABLE_FAULT(from, to)				\
// /*AFLA*/ 	_ASM_EXTABLE_HANDLE(from, to, ex_handler_fault)
// /*AFLA*/ 
// /*AFLA*/ # define _ASM_EXTABLE_EX(from, to)				\
// /*AFLA*/ 	_ASM_EXTABLE_HANDLE(from, to, ex_handler_ext)
// /*AFLA*/ 
// /*AFLA*/ # define _ASM_NOKPROBE(entry)					\
// /*AFLA*/ 	.pushsection "_kprobe_blacklist","aw" ;			\
// /*AFLA*/ 	_ASM_ALIGN ;						\
// /*AFLA*/ 	_ASM_PTR (entry);					\
// /*AFLA*/ 	.popsection
// /*AFLA*/ 
// /*AFLA*/ .macro ALIGN_DESTINATION
// /*AFLA*/ 	/* check for bad alignment of destination */
// /*AFLA*/ 	movl %edi,%ecx
// /*AFLA*/ 	andl $7,%ecx
// /*AFLA*/ 	jz 102f				/* already aligned */
// /*AFLA*/ 	subl $8,%ecx
// /*AFLA*/ 	negl %ecx
// /*AFLA*/ 	subl %ecx,%edx
// /*AFLA*/ 100:	movb (%rsi),%al
// /*AFLA*/ 101:	movb %al,(%rdi)
// /*AFLA*/ 	incq %rsi
// /*AFLA*/ 	incq %rdi
// /*AFLA*/ 	decl %ecx
// /*AFLA*/ 	jnz 100b
// /*AFLA*/ 102:
// /*AFLA*/ 	.section .fixup,"ax"
// /*AFLA*/ 103:	addl %ecx,%edx			/* ecx is zerorest also */
// /*AFLA*/ 	jmp copy_user_handle_tail
// /*AFLA*/ 	.previous
// /*AFLA*/ 
// /*AFLA*/ 	_ASM_EXTABLE(100b,103b)
// /*AFLA*/ 	_ASM_EXTABLE(101b,103b)
// /*AFLA*/ 	.endm
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ # define _EXPAND_EXTABLE_HANDLE(x) #x
// /*AFLA*/ # define _ASM_EXTABLE_HANDLE(from, to, handler)			\
// /*AFLA*/ 	" .pushsection \"__ex_table\",\"a\"\n"			\
// /*AFLA*/ 	" .balign 4\n"						\
// /*AFLA*/ 	" .long (" #from ") - .\n"				\
// /*AFLA*/ 	" .long (" #to ") - .\n"				\
// /*AFLA*/ 	" .long (" _EXPAND_EXTABLE_HANDLE(handler) ") - .\n"	\
// /*AFLA*/ 	" .popsection\n"
// /*AFLA*/ 
// /*AFLA*/ # define _ASM_EXTABLE(from, to)					\
// /*AFLA*/ 	_ASM_EXTABLE_HANDLE(from, to, ex_handler_default)
// /*AFLA*/ 
// /*AFLA*/ # define _ASM_EXTABLE_FAULT(from, to)				\
// /*AFLA*/ 	_ASM_EXTABLE_HANDLE(from, to, ex_handler_fault)
// /*AFLA*/ 
// /*AFLA*/ # define _ASM_EXTABLE_EX(from, to)				\
// /*AFLA*/ 	_ASM_EXTABLE_HANDLE(from, to, ex_handler_ext)
// /*AFLA*/ 
// /*AFLA*/ /* For C file, we already have NOKPROBE_SYMBOL macro */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_ASM_H */
