// /*AFLA*/ /* Rewritten and vastly simplified by Rusty Russell for in-kernel
// /*AFLA*/  * module loader:
// /*AFLA*/  *   Copyright 2002 Rusty Russell <rusty@rustcorp.com.au> IBM Corporation
// /*AFLA*/  */
// /*AFLA*/ #ifndef _LINUX_KALLSYMS_H
// /*AFLA*/ #define _LINUX_KALLSYMS_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/errno.h>
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ #include <linux/stddef.h>
// /*AFLA*/ 
// /*AFLA*/ #define KSYM_NAME_LEN 128
// /*AFLA*/ #define KSYM_SYMBOL_LEN (sizeof("%s+%#lx/%#lx [%s]") + (KSYM_NAME_LEN - 1) + \
// /*AFLA*/ 			 2*(BITS_PER_LONG*3/10) + (MODULE_NAME_LEN - 1) + 1)
// /*AFLA*/ 
// /*AFLA*/ struct module;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_KALLSYMS
// /*AFLA*/ /* Lookup the address for a symbol. Returns 0 if not found. */
// /*AFLA*/ unsigned long kallsyms_lookup_name(const char *name);
// /*AFLA*/ 
// /*AFLA*/ /* Call a function on each kallsyms symbol in the core kernel */
// /*AFLA*/ int kallsyms_on_each_symbol(int (*fn)(void *, const char *, struct module *,
// /*AFLA*/ 				      unsigned long),
// /*AFLA*/ 			    void *data);
// /*AFLA*/ 
// /*AFLA*/ extern int kallsyms_lookup_size_offset(unsigned long addr,
// /*AFLA*/ 				  unsigned long *symbolsize,
// /*AFLA*/ 				  unsigned long *offset);
// /*AFLA*/ 
// /*AFLA*/ /* Lookup an address.  modname is set to NULL if it's in the kernel. */
// /*AFLA*/ const char *kallsyms_lookup(unsigned long addr,
// /*AFLA*/ 			    unsigned long *symbolsize,
// /*AFLA*/ 			    unsigned long *offset,
// /*AFLA*/ 			    char **modname, char *namebuf);
// /*AFLA*/ 
// /*AFLA*/ /* Look up a kernel symbol and return it in a text buffer. */
// /*AFLA*/ extern int sprint_symbol(char *buffer, unsigned long address);
// /*AFLA*/ extern int sprint_symbol_no_offset(char *buffer, unsigned long address);
// /*AFLA*/ extern int sprint_backtrace(char *buffer, unsigned long address);
// /*AFLA*/ 
// /*AFLA*/ /* Look up a kernel symbol and print it to the kernel messages. */
// /*AFLA*/ extern void __print_symbol(const char *fmt, unsigned long address);
// /*AFLA*/ 
// /*AFLA*/ int lookup_symbol_name(unsigned long addr, char *symname);
// /*AFLA*/ int lookup_symbol_attrs(unsigned long addr, unsigned long *size, unsigned long *offset, char *modname, char *name);
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_KALLSYMS */
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long kallsyms_lookup_name(const char *name)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int kallsyms_on_each_symbol(int (*fn)(void *, const char *,
// /*AFLA*/ 						    struct module *,
// /*AFLA*/ 						    unsigned long),
// /*AFLA*/ 					  void *data)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int kallsyms_lookup_size_offset(unsigned long addr,
// /*AFLA*/ 					      unsigned long *symbolsize,
// /*AFLA*/ 					      unsigned long *offset)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline const char *kallsyms_lookup(unsigned long addr,
// /*AFLA*/ 					  unsigned long *symbolsize,
// /*AFLA*/ 					  unsigned long *offset,
// /*AFLA*/ 					  char **modname, char *namebuf)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sprint_symbol(char *buffer, unsigned long addr)
// /*AFLA*/ {
// /*AFLA*/ 	*buffer = '\0';
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sprint_symbol_no_offset(char *buffer, unsigned long addr)
// /*AFLA*/ {
// /*AFLA*/ 	*buffer = '\0';
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sprint_backtrace(char *buffer, unsigned long addr)
// /*AFLA*/ {
// /*AFLA*/ 	*buffer = '\0';
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int lookup_symbol_name(unsigned long addr, char *symname)
// /*AFLA*/ {
// /*AFLA*/ 	return -ERANGE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int lookup_symbol_attrs(unsigned long addr, unsigned long *size, unsigned long *offset, char *modname, char *name)
// /*AFLA*/ {
// /*AFLA*/ 	return -ERANGE;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Stupid that this does nothing, but I didn't create this mess. */
// /*AFLA*/ #define __print_symbol(fmt, addr)
// /*AFLA*/ #endif /*CONFIG_KALLSYMS*/
// /*AFLA*/ 
// /*AFLA*/ /* This macro allows us to keep printk typechecking */
// /*AFLA*/ static __printf(1, 2)
// /*AFLA*/ void __check_printsym_format(const char *fmt, ...)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void print_symbol(const char *fmt, unsigned long addr)
// /*AFLA*/ {
// /*AFLA*/ 	__check_printsym_format(fmt, "");
// /*AFLA*/ 	__print_symbol(fmt, (unsigned long)
// /*AFLA*/ 		       __builtin_extract_return_addr((void *)addr));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void print_ip_sym(unsigned long ip)
// /*AFLA*/ {
// /*AFLA*/ 	printk("[<%p>] %pS\n", (void *) ip, (void *) ip);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /*_LINUX_KALLSYMS_H*/
