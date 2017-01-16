// /*AFLA*/ #ifndef _DYNAMIC_DEBUG_H
// /*AFLA*/ #define _DYNAMIC_DEBUG_H
// /*AFLA*/ 
// /*AFLA*/ #if defined(CC_HAVE_ASM_GOTO) && defined(CONFIG_JUMP_LABEL)
// /*AFLA*/ #include <linux/jump_label.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * An instance of this structure is created in a special
// /*AFLA*/  * ELF section at every dynamic debug callsite.  At runtime,
// /*AFLA*/  * the special section is treated as an array of these.
// /*AFLA*/  */
// /*AFLA*/ struct _ddebug {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * These fields are used to drive the user interface
// /*AFLA*/ 	 * for selecting and displaying debug callsites.
// /*AFLA*/ 	 */
// /*AFLA*/ 	const char *modname;
// /*AFLA*/ 	const char *function;
// /*AFLA*/ 	const char *filename;
// /*AFLA*/ 	const char *format;
// /*AFLA*/ 	unsigned int lineno:18;
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * The flags field controls the behaviour at the callsite.
// /*AFLA*/ 	 * The bits here are changed dynamically when the user
// /*AFLA*/ 	 * writes commands to <debugfs>/dynamic_debug/control
// /*AFLA*/ 	 */
// /*AFLA*/ #define _DPRINTK_FLAGS_NONE	0
// /*AFLA*/ #define _DPRINTK_FLAGS_PRINT	(1<<0) /* printk() a message using the format */
// /*AFLA*/ #define _DPRINTK_FLAGS_INCL_MODNAME	(1<<1)
// /*AFLA*/ #define _DPRINTK_FLAGS_INCL_FUNCNAME	(1<<2)
// /*AFLA*/ #define _DPRINTK_FLAGS_INCL_LINENO	(1<<3)
// /*AFLA*/ #define _DPRINTK_FLAGS_INCL_TID		(1<<4)
// /*AFLA*/ #if defined DEBUG
// /*AFLA*/ #define _DPRINTK_FLAGS_DEFAULT _DPRINTK_FLAGS_PRINT
// /*AFLA*/ #else
// /*AFLA*/ #define _DPRINTK_FLAGS_DEFAULT 0
// /*AFLA*/ #endif
// /*AFLA*/ 	unsigned int flags:8;
// /*AFLA*/ #ifdef HAVE_JUMP_LABEL
// /*AFLA*/ 	union {
// /*AFLA*/ 		struct static_key_true dd_key_true;
// /*AFLA*/ 		struct static_key_false dd_key_false;
// /*AFLA*/ 	} key;
// /*AFLA*/ #endif
// /*AFLA*/ } __attribute__((aligned(8)));
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ int ddebug_add_module(struct _ddebug *tab, unsigned int n,
// /*AFLA*/ 				const char *modname);
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_DYNAMIC_DEBUG)
// /*AFLA*/ extern int ddebug_remove_module(const char *mod_name);
// /*AFLA*/ extern __printf(2, 3)
// /*AFLA*/ void __dynamic_pr_debug(struct _ddebug *descriptor, const char *fmt, ...);
// /*AFLA*/ 
// /*AFLA*/ extern int ddebug_dyndbg_module_param_cb(char *param, char *val,
// /*AFLA*/ 					const char *modname);
// /*AFLA*/ 
// /*AFLA*/ struct device;
// /*AFLA*/ 
// /*AFLA*/ extern __printf(3, 4)
// /*AFLA*/ void __dynamic_dev_dbg(struct _ddebug *descriptor, const struct device *dev,
// /*AFLA*/ 		       const char *fmt, ...);
// /*AFLA*/ 
// /*AFLA*/ struct net_device;
// /*AFLA*/ 
// /*AFLA*/ extern __printf(3, 4)
// /*AFLA*/ void __dynamic_netdev_dbg(struct _ddebug *descriptor,
// /*AFLA*/ 			  const struct net_device *dev,
// /*AFLA*/ 			  const char *fmt, ...);
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_DYNAMIC_DEBUG_METADATA_KEY(name, fmt, key, init)	\
// /*AFLA*/ 	static struct _ddebug  __aligned(8)			\
// /*AFLA*/ 	__attribute__((section("__verbose"))) name = {		\
// /*AFLA*/ 		.modname = KBUILD_MODNAME,			\
// /*AFLA*/ 		.function = __func__,				\
// /*AFLA*/ 		.filename = __FILE__,				\
// /*AFLA*/ 		.format = (fmt),				\
// /*AFLA*/ 		.lineno = __LINE__,				\
// /*AFLA*/ 		.flags = _DPRINTK_FLAGS_DEFAULT,		\
// /*AFLA*/ 		dd_key_init(key, init)				\
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ #ifdef HAVE_JUMP_LABEL
// /*AFLA*/ 
// /*AFLA*/ #define dd_key_init(key, init) key = (init)
// /*AFLA*/ 
// /*AFLA*/ #ifdef DEBUG
// /*AFLA*/ #define DEFINE_DYNAMIC_DEBUG_METADATA(name, fmt) \
// /*AFLA*/ 	DEFINE_DYNAMIC_DEBUG_METADATA_KEY(name, fmt, .key.dd_key_true, \
// /*AFLA*/ 					  (STATIC_KEY_TRUE_INIT))
// /*AFLA*/ 
// /*AFLA*/ #define DYNAMIC_DEBUG_BRANCH(descriptor) \
// /*AFLA*/ 	static_branch_likely(&descriptor.key.dd_key_true)
// /*AFLA*/ #else
// /*AFLA*/ #define DEFINE_DYNAMIC_DEBUG_METADATA(name, fmt) \
// /*AFLA*/ 	DEFINE_DYNAMIC_DEBUG_METADATA_KEY(name, fmt, .key.dd_key_false, \
// /*AFLA*/ 					  (STATIC_KEY_FALSE_INIT))
// /*AFLA*/ 
// /*AFLA*/ #define DYNAMIC_DEBUG_BRANCH(descriptor) \
// /*AFLA*/ 	static_branch_unlikely(&descriptor.key.dd_key_false)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ #define dd_key_init(key, init)
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_DYNAMIC_DEBUG_METADATA(name, fmt) \
// /*AFLA*/ 	DEFINE_DYNAMIC_DEBUG_METADATA_KEY(name, fmt, 0, 0)
// /*AFLA*/ 
// /*AFLA*/ #ifdef DEBUG
// /*AFLA*/ #define DYNAMIC_DEBUG_BRANCH(descriptor) \
// /*AFLA*/ 	likely(descriptor.flags & _DPRINTK_FLAGS_PRINT)
// /*AFLA*/ #else
// /*AFLA*/ #define DYNAMIC_DEBUG_BRANCH(descriptor) \
// /*AFLA*/ 	unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define dynamic_pr_debug(fmt, ...)				\
// /*AFLA*/ do {								\
// /*AFLA*/ 	DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, fmt);		\
// /*AFLA*/ 	if (DYNAMIC_DEBUG_BRANCH(descriptor))			\
// /*AFLA*/ 		__dynamic_pr_debug(&descriptor, pr_fmt(fmt),	\
// /*AFLA*/ 				   ##__VA_ARGS__);		\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define dynamic_dev_dbg(dev, fmt, ...)				\
// /*AFLA*/ do {								\
// /*AFLA*/ 	DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, fmt);		\
// /*AFLA*/ 	if (DYNAMIC_DEBUG_BRANCH(descriptor))			\
// /*AFLA*/ 		__dynamic_dev_dbg(&descriptor, dev, fmt,	\
// /*AFLA*/ 				  ##__VA_ARGS__);		\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define dynamic_netdev_dbg(dev, fmt, ...)			\
// /*AFLA*/ do {								\
// /*AFLA*/ 	DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, fmt);		\
// /*AFLA*/ 	if (DYNAMIC_DEBUG_BRANCH(descriptor))			\
// /*AFLA*/ 		__dynamic_netdev_dbg(&descriptor, dev, fmt,	\
// /*AFLA*/ 				     ##__VA_ARGS__);		\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #define dynamic_hex_dump(prefix_str, prefix_type, rowsize,	\
// /*AFLA*/ 			 groupsize, buf, len, ascii)		\
// /*AFLA*/ do {								\
// /*AFLA*/ 	DEFINE_DYNAMIC_DEBUG_METADATA(descriptor,		\
// /*AFLA*/ 		__builtin_constant_p(prefix_str) ? prefix_str : "hexdump");\
// /*AFLA*/ 	if (DYNAMIC_DEBUG_BRANCH(descriptor))			\
// /*AFLA*/ 		print_hex_dump(KERN_DEBUG, prefix_str,		\
// /*AFLA*/ 			       prefix_type, rowsize, groupsize,	\
// /*AFLA*/ 			       buf, len, ascii);		\
// /*AFLA*/ } while (0)
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ #include <linux/string.h>
// /*AFLA*/ #include <linux/errno.h>
// /*AFLA*/ 
// /*AFLA*/ static inline int ddebug_remove_module(const char *mod)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int ddebug_dyndbg_module_param_cb(char *param, char *val,
// /*AFLA*/ 						const char *modname)
// /*AFLA*/ {
// /*AFLA*/ 	if (strstr(param, "dyndbg")) {
// /*AFLA*/ 		/* avoid pr_warn(), which wants pr_fmt() fully defined */
// /*AFLA*/ 		printk(KERN_WARNING "dyndbg param is supported only in "
// /*AFLA*/ 			"CONFIG_DYNAMIC_DEBUG builds\n");
// /*AFLA*/ 		return 0; /* allow and ignore */
// /*AFLA*/ 	}
// /*AFLA*/ 	return -EINVAL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define dynamic_pr_debug(fmt, ...)					\
// /*AFLA*/ 	do { if (0) printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__); } while (0)
// /*AFLA*/ #define dynamic_dev_dbg(dev, fmt, ...)					\
// /*AFLA*/ 	do { if (0) dev_printk(KERN_DEBUG, dev, fmt, ##__VA_ARGS__); } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif
