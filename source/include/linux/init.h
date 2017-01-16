// /*AFLA*/ #ifndef _LINUX_INIT_H
// /*AFLA*/ #define _LINUX_INIT_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ /* These macros are used to mark some functions or 
// /*AFLA*/  * initialized data (doesn't apply to uninitialized data)
// /*AFLA*/  * as `initialization' functions. The kernel can take this
// /*AFLA*/  * as hint that the function is used only during the initialization
// /*AFLA*/  * phase and free up used memory resources after
// /*AFLA*/  *
// /*AFLA*/  * Usage:
// /*AFLA*/  * For functions:
// /*AFLA*/  * 
// /*AFLA*/  * You should add __init immediately before the function name, like:
// /*AFLA*/  *
// /*AFLA*/  * static void __init initme(int x, int y)
// /*AFLA*/  * {
// /*AFLA*/  *    extern int z; z = x * y;
// /*AFLA*/  * }
// /*AFLA*/  *
// /*AFLA*/  * If the function has a prototype somewhere, you can also add
// /*AFLA*/  * __init between closing brace of the prototype and semicolon:
// /*AFLA*/  *
// /*AFLA*/  * extern int initialize_foobar_device(int, int, int) __init;
// /*AFLA*/  *
// /*AFLA*/  * For initialized data:
// /*AFLA*/  * You should insert __initdata or __initconst between the variable name
// /*AFLA*/  * and equal sign followed by value, e.g.:
// /*AFLA*/  *
// /*AFLA*/  * static int init_variable __initdata = 0;
// /*AFLA*/  * static const char linux_logo[] __initconst = { 0x32, 0x36, ... };
// /*AFLA*/  *
// /*AFLA*/  * Don't forget to initialize data not at file scope, i.e. within a function,
// /*AFLA*/  * as gcc otherwise puts the data into the bss section and not into the init
// /*AFLA*/  * section.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /* These are for everybody (although not all archs will actually
// /*AFLA*/    discard it in modules) */
// /*AFLA*/ #define __init		__section(.init.text) __cold notrace __latent_entropy
// /*AFLA*/ #define __initdata	__section(.init.data)
// /*AFLA*/ #define __initconst	__section(.init.rodata)
// /*AFLA*/ #define __exitdata	__section(.exit.data)
// /*AFLA*/ #define __exit_call	__used __section(.exitcall.exit)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * modpost check for section mismatches during the kernel build.
// /*AFLA*/  * A section mismatch happens when there are references from a
// /*AFLA*/  * code or data section to an init section (both code or data).
// /*AFLA*/  * The init sections are (for most archs) discarded by the kernel
// /*AFLA*/  * when early init has completed so all such references are potential bugs.
// /*AFLA*/  * For exit sections the same issue exists.
// /*AFLA*/  *
// /*AFLA*/  * The following markers are used for the cases where the reference to
// /*AFLA*/  * the *init / *exit section (code or data) is valid and will teach
// /*AFLA*/  * modpost not to issue a warning.  Intended semantics is that a code or
// /*AFLA*/  * data tagged __ref* can reference code or data from init section without
// /*AFLA*/  * producing a warning (of course, no warning does not mean code is
// /*AFLA*/  * correct, so optimally document why the __ref is needed and why it's OK).
// /*AFLA*/  *
// /*AFLA*/  * The markers follow same syntax rules as __init / __initdata.
// /*AFLA*/  */
// /*AFLA*/ #define __ref            __section(.ref.text) noinline
// /*AFLA*/ #define __refdata        __section(.ref.data)
// /*AFLA*/ #define __refconst       __section(.ref.rodata)
// /*AFLA*/ 
// /*AFLA*/ #ifdef MODULE
// /*AFLA*/ #define __exitused
// /*AFLA*/ #else
// /*AFLA*/ #define __exitused  __used
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define __exit          __section(.exit.text) __exitused __cold notrace
// /*AFLA*/ 
// /*AFLA*/ /* Used for MEMORY_HOTPLUG */
// /*AFLA*/ #define __meminit        __section(.meminit.text) __cold notrace \
// /*AFLA*/ 						  __latent_entropy
// /*AFLA*/ #define __meminitdata    __section(.meminit.data)
// /*AFLA*/ #define __meminitconst   __section(.meminit.rodata)
// /*AFLA*/ #define __memexit        __section(.memexit.text) __exitused __cold notrace
// /*AFLA*/ #define __memexitdata    __section(.memexit.data)
// /*AFLA*/ #define __memexitconst   __section(.memexit.rodata)
// /*AFLA*/ 
// /*AFLA*/ /* For assembly routines */
// /*AFLA*/ #define __HEAD		.section	".head.text","ax"
// /*AFLA*/ #define __INIT		.section	".init.text","ax"
// /*AFLA*/ #define __FINIT		.previous
// /*AFLA*/ 
// /*AFLA*/ #define __INITDATA	.section	".init.data","aw",%progbits
// /*AFLA*/ #define __INITRODATA	.section	".init.rodata","a",%progbits
// /*AFLA*/ #define __FINITDATA	.previous
// /*AFLA*/ 
// /*AFLA*/ #define __MEMINIT        .section	".meminit.text", "ax"
// /*AFLA*/ #define __MEMINITDATA    .section	".meminit.data", "aw"
// /*AFLA*/ #define __MEMINITRODATA  .section	".meminit.rodata", "a"
// /*AFLA*/ 
// /*AFLA*/ /* silence warnings when references are OK */
// /*AFLA*/ #define __REF            .section       ".ref.text", "ax"
// /*AFLA*/ #define __REFDATA        .section       ".ref.data", "aw"
// /*AFLA*/ #define __REFCONST       .section       ".ref.rodata", "a"
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ /*
// /*AFLA*/  * Used for initialization calls..
// /*AFLA*/  */
// /*AFLA*/ typedef int (*initcall_t)(void);
// /*AFLA*/ typedef void (*exitcall_t)(void);
// /*AFLA*/ 
// /*AFLA*/ extern initcall_t __con_initcall_start[], __con_initcall_end[];
// /*AFLA*/ extern initcall_t __security_initcall_start[], __security_initcall_end[];
// /*AFLA*/ 
// /*AFLA*/ /* Used for contructor calls. */
// /*AFLA*/ typedef void (*ctor_fn_t)(void);
// /*AFLA*/ 
// /*AFLA*/ /* Defined in init/main.c */
// /*AFLA*/ extern int do_one_initcall(initcall_t fn);
// /*AFLA*/ extern char __initdata boot_command_line[];
// /*AFLA*/ extern char *saved_command_line;
// /*AFLA*/ extern unsigned int reset_devices;
// /*AFLA*/ 
// /*AFLA*/ /* used by init/main.c */
// /*AFLA*/ void setup_arch(char **);
// /*AFLA*/ void prepare_namespace(void);
// /*AFLA*/ void __init load_default_modules(void);
// /*AFLA*/ int __init init_rootfs(void);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_RODATA
// /*AFLA*/ void mark_rodata_ro(void);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern void (*late_time_init)(void);
// /*AFLA*/ 
// /*AFLA*/ extern bool initcall_debug;
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/   
// /*AFLA*/ #ifndef MODULE
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * initcalls are now grouped by functionality into separate
// /*AFLA*/  * subsections. Ordering inside the subsections is determined
// /*AFLA*/  * by link order. 
// /*AFLA*/  * For backwards compatibility, initcall() puts the call in 
// /*AFLA*/  * the device init subsection.
// /*AFLA*/  *
// /*AFLA*/  * The `id' arg to __define_initcall() is needed so that multiple initcalls
// /*AFLA*/  * can point at the same handler without causing duplicate-symbol build errors.
// /*AFLA*/  *
// /*AFLA*/  * Initcalls are run by placing pointers in initcall sections that the
// /*AFLA*/  * kernel iterates at runtime. The linker can do dead code / data elimination
// /*AFLA*/  * and remove that completely, so the initcall sections have to be marked
// /*AFLA*/  * as KEEP() in the linker script.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define __define_initcall(fn, id) \
// /*AFLA*/ 	static initcall_t __initcall_##fn##id __used \
// /*AFLA*/ 	__attribute__((__section__(".initcall" #id ".init"))) = fn;
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Early initcalls run before initializing SMP.
// /*AFLA*/  *
// /*AFLA*/  * Only for built-in code, not modules.
// /*AFLA*/  */
// /*AFLA*/ #define early_initcall(fn)		__define_initcall(fn, early)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * A "pure" initcall has no dependencies on anything else, and purely
// /*AFLA*/  * initializes variables that couldn't be statically initialized.
// /*AFLA*/  *
// /*AFLA*/  * This only exists for built-in code, not for modules.
// /*AFLA*/  * Keep main.c:initcall_level_names[] in sync.
// /*AFLA*/  */
// /*AFLA*/ #define pure_initcall(fn)		__define_initcall(fn, 0)
// /*AFLA*/ 
// /*AFLA*/ #define core_initcall(fn)		__define_initcall(fn, 1)
// /*AFLA*/ #define core_initcall_sync(fn)		__define_initcall(fn, 1s)
// /*AFLA*/ #define postcore_initcall(fn)		__define_initcall(fn, 2)
// /*AFLA*/ #define postcore_initcall_sync(fn)	__define_initcall(fn, 2s)
// /*AFLA*/ #define arch_initcall(fn)		__define_initcall(fn, 3)
// /*AFLA*/ #define arch_initcall_sync(fn)		__define_initcall(fn, 3s)
// /*AFLA*/ #define subsys_initcall(fn)		__define_initcall(fn, 4)
// /*AFLA*/ #define subsys_initcall_sync(fn)	__define_initcall(fn, 4s)
// /*AFLA*/ #define fs_initcall(fn)			__define_initcall(fn, 5)
// /*AFLA*/ #define fs_initcall_sync(fn)		__define_initcall(fn, 5s)
// /*AFLA*/ #define rootfs_initcall(fn)		__define_initcall(fn, rootfs)
// /*AFLA*/ #define device_initcall(fn)		__define_initcall(fn, 6)
// /*AFLA*/ #define device_initcall_sync(fn)	__define_initcall(fn, 6s)
// /*AFLA*/ #define late_initcall(fn)		__define_initcall(fn, 7)
// /*AFLA*/ #define late_initcall_sync(fn)		__define_initcall(fn, 7s)
// /*AFLA*/ 
// /*AFLA*/ #define __initcall(fn) device_initcall(fn)
// /*AFLA*/ 
// /*AFLA*/ #define __exitcall(fn)						\
// /*AFLA*/ 	static exitcall_t __exitcall_##fn __exit_call = fn
// /*AFLA*/ 
// /*AFLA*/ #define console_initcall(fn)					\
// /*AFLA*/ 	static initcall_t __initcall_##fn			\
// /*AFLA*/ 	__used __section(.con_initcall.init) = fn
// /*AFLA*/ 
// /*AFLA*/ #define security_initcall(fn)					\
// /*AFLA*/ 	static initcall_t __initcall_##fn			\
// /*AFLA*/ 	__used __section(.security_initcall.init) = fn
// /*AFLA*/ 
// /*AFLA*/ struct obs_kernel_param {
// /*AFLA*/ 	const char *str;
// /*AFLA*/ 	int (*setup_func)(char *);
// /*AFLA*/ 	int early;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Only for really core code.  See moduleparam.h for the normal way.
// /*AFLA*/  *
// /*AFLA*/  * Force the alignment so the compiler doesn't space elements of the
// /*AFLA*/  * obs_kernel_param "array" too far apart in .init.setup.
// /*AFLA*/  */
// /*AFLA*/ #define __setup_param(str, unique_id, fn, early)			\
// /*AFLA*/ 	static const char __setup_str_##unique_id[] __initconst		\
// /*AFLA*/ 		__aligned(1) = str; 					\
// /*AFLA*/ 	static struct obs_kernel_param __setup_##unique_id		\
// /*AFLA*/ 		__used __section(.init.setup)				\
// /*AFLA*/ 		__attribute__((aligned((sizeof(long)))))		\
// /*AFLA*/ 		= { __setup_str_##unique_id, fn, early }
// /*AFLA*/ 
// /*AFLA*/ #define __setup(str, fn)						\
// /*AFLA*/ 	__setup_param(str, fn, fn, 0)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * NOTE: fn is as per module_param, not __setup!
// /*AFLA*/  * Emits warning if fn returns non-zero.
// /*AFLA*/  */
// /*AFLA*/ #define early_param(str, fn)						\
// /*AFLA*/ 	__setup_param(str, fn, fn, 1)
// /*AFLA*/ 
// /*AFLA*/ #define early_param_on_off(str_on, str_off, var, config)		\
// /*AFLA*/ 									\
// /*AFLA*/ 	int var = IS_ENABLED(config);					\
// /*AFLA*/ 									\
// /*AFLA*/ 	static int __init parse_##var##_on(char *arg)			\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		var = 1;						\
// /*AFLA*/ 		return 0;						\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	__setup_param(str_on, parse_##var##_on, parse_##var##_on, 1);	\
// /*AFLA*/ 									\
// /*AFLA*/ 	static int __init parse_##var##_off(char *arg)			\
// /*AFLA*/ 	{								\
// /*AFLA*/ 		var = 0;						\
// /*AFLA*/ 		return 0;						\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	__setup_param(str_off, parse_##var##_off, parse_##var##_off, 1)
// /*AFLA*/ 
// /*AFLA*/ /* Relies on boot_command_line being set */
// /*AFLA*/ void __init parse_early_param(void);
// /*AFLA*/ void __init parse_early_options(char *cmdline);
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #else /* MODULE */
// /*AFLA*/ 
// /*AFLA*/ #define __setup_param(str, unique_id, fn)	/* nothing */
// /*AFLA*/ #define __setup(str, func) 			/* nothing */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* Data marked not to be saved by software suspend */
// /*AFLA*/ #define __nosavedata __section(.data..nosave)
// /*AFLA*/ 
// /*AFLA*/ #ifdef MODULE
// /*AFLA*/ #define __exit_p(x) x
// /*AFLA*/ #else
// /*AFLA*/ #define __exit_p(x) NULL
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_INIT_H */
