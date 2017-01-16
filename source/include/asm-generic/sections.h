// /*AFLA*/ #ifndef _ASM_GENERIC_SECTIONS_H_
// /*AFLA*/ #define _ASM_GENERIC_SECTIONS_H_
// /*AFLA*/ 
// /*AFLA*/ /* References to section boundaries */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Usage guidelines:
// /*AFLA*/  * _text, _data: architecture specific, don't use them in arch-independent code
// /*AFLA*/  * [_stext, _etext]: contains .text.* sections, may also contain .rodata.*
// /*AFLA*/  *                   and/or .init.* sections
// /*AFLA*/  * [_sdata, _edata]: contains .data.* sections, may also contain .rodata.*
// /*AFLA*/  *                   and/or .init.* sections.
// /*AFLA*/  * [__start_rodata, __end_rodata]: contains .rodata.* sections
// /*AFLA*/  * [__start_data_ro_after_init, __end_data_ro_after_init]:
// /*AFLA*/  *		     contains data.ro_after_init section
// /*AFLA*/  * [__init_begin, __init_end]: contains .init.* sections, but .init.text.*
// /*AFLA*/  *                   may be out of this range on some architectures.
// /*AFLA*/  * [_sinittext, _einittext]: contains .init.text.* sections
// /*AFLA*/  * [__bss_start, __bss_stop]: contains BSS sections
// /*AFLA*/  *
// /*AFLA*/  * Following global variables are optional and may be unavailable on some
// /*AFLA*/  * architectures and/or kernel configurations.
// /*AFLA*/  *	_text, _data
// /*AFLA*/  *	__kprobes_text_start, __kprobes_text_end
// /*AFLA*/  *	__entry_text_start, __entry_text_end
// /*AFLA*/  *	__ctors_start, __ctors_end
// /*AFLA*/  */
// /*AFLA*/ extern char _text[], _stext[], _etext[];
// /*AFLA*/ extern char _data[], _sdata[], _edata[];
// /*AFLA*/ extern char __bss_start[], __bss_stop[];
// /*AFLA*/ extern char __init_begin[], __init_end[];
// /*AFLA*/ extern char _sinittext[], _einittext[];
// /*AFLA*/ extern char __start_data_ro_after_init[], __end_data_ro_after_init[];
// /*AFLA*/ extern char _end[];
// /*AFLA*/ extern char __per_cpu_load[], __per_cpu_start[], __per_cpu_end[];
// /*AFLA*/ extern char __kprobes_text_start[], __kprobes_text_end[];
// /*AFLA*/ extern char __entry_text_start[], __entry_text_end[];
// /*AFLA*/ extern char __start_rodata[], __end_rodata[];
// /*AFLA*/ 
// /*AFLA*/ /* Start and end of .ctors section - used for constructor calls. */
// /*AFLA*/ extern char __ctors_start[], __ctors_end[];
// /*AFLA*/ 
// /*AFLA*/ extern __visible const void __nosave_begin, __nosave_end;
// /*AFLA*/ 
// /*AFLA*/ /* function descriptor handling (if any).  Override
// /*AFLA*/  * in asm/sections.h */
// /*AFLA*/ #ifndef dereference_function_descriptor
// /*AFLA*/ #define dereference_function_descriptor(p) (p)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* random extra sections (if any).  Override
// /*AFLA*/  * in asm/sections.h */
// /*AFLA*/ #ifndef arch_is_kernel_text
// /*AFLA*/ static inline int arch_is_kernel_text(unsigned long addr)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef arch_is_kernel_data
// /*AFLA*/ static inline int arch_is_kernel_data(unsigned long addr)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * memory_contains - checks if an object is contained within a memory region
// /*AFLA*/  * @begin: virtual address of the beginning of the memory region
// /*AFLA*/  * @end: virtual address of the end of the memory region
// /*AFLA*/  * @virt: virtual address of the memory object
// /*AFLA*/  * @size: size of the memory object
// /*AFLA*/  *
// /*AFLA*/  * Returns: true if the object specified by @virt and @size is entirely
// /*AFLA*/  * contained within the memory region defined by @begin and @end, false
// /*AFLA*/  * otherwise.
// /*AFLA*/  */
// /*AFLA*/ static inline bool memory_contains(void *begin, void *end, void *virt,
// /*AFLA*/ 				   size_t size)
// /*AFLA*/ {
// /*AFLA*/ 	return virt >= begin && virt + size <= end;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * memory_intersects - checks if the region occupied by an object intersects
// /*AFLA*/  *                     with another memory region
// /*AFLA*/  * @begin: virtual address of the beginning of the memory regien
// /*AFLA*/  * @end: virtual address of the end of the memory region
// /*AFLA*/  * @virt: virtual address of the memory object
// /*AFLA*/  * @size: size of the memory object
// /*AFLA*/  *
// /*AFLA*/  * Returns: true if an object's memory region, specified by @virt and @size,
// /*AFLA*/  * intersects with the region specified by @begin and @end, false otherwise.
// /*AFLA*/  */
// /*AFLA*/ static inline bool memory_intersects(void *begin, void *end, void *virt,
// /*AFLA*/ 				     size_t size)
// /*AFLA*/ {
// /*AFLA*/ 	void *vend = virt + size;
// /*AFLA*/ 
// /*AFLA*/ 	return (virt >= begin && virt < end) || (vend >= begin && vend < end);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * init_section_contains - checks if an object is contained within the init
// /*AFLA*/  *                         section
// /*AFLA*/  * @virt: virtual address of the memory object
// /*AFLA*/  * @size: size of the memory object
// /*AFLA*/  *
// /*AFLA*/  * Returns: true if the object specified by @virt and @size is entirely
// /*AFLA*/  * contained within the init section, false otherwise.
// /*AFLA*/  */
// /*AFLA*/ static inline bool init_section_contains(void *virt, size_t size)
// /*AFLA*/ {
// /*AFLA*/ 	return memory_contains(__init_begin, __init_end, virt, size);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * init_section_intersects - checks if the region occupied by an object
// /*AFLA*/  *                           intersects with the init section
// /*AFLA*/  * @virt: virtual address of the memory object
// /*AFLA*/  * @size: size of the memory object
// /*AFLA*/  *
// /*AFLA*/  * Returns: true if an object's memory region, specified by @virt and @size,
// /*AFLA*/  * intersects with the init section, false otherwise.
// /*AFLA*/  */
// /*AFLA*/ static inline bool init_section_intersects(void *virt, size_t size)
// /*AFLA*/ {
// /*AFLA*/ 	return memory_intersects(__init_begin, __init_end, virt, size);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_GENERIC_SECTIONS_H_ */
