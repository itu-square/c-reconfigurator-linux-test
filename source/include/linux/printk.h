// /*AFLA*/ #ifndef __KERNEL_PRINTK__
// /*AFLA*/ #define __KERNEL_PRINTK__
// /*AFLA*/ 
// /*AFLA*/ #include <stdarg.h>
// /*AFLA*/ #include <linux/init.h>
// /*AFLA*/ #include <linux/kern_levels.h>
// /*AFLA*/ #include <linux/linkage.h>
// /*AFLA*/ #include <linux/cache.h>
// /*AFLA*/ 
// /*AFLA*/ extern const char linux_banner[];
// /*AFLA*/ extern const char linux_proc_banner[];
// /*AFLA*/ 
// /*AFLA*/ static inline int printk_get_level(const char *buffer)
// /*AFLA*/ {
// /*AFLA*/ 	if (buffer[0] == KERN_SOH_ASCII && buffer[1]) {
// /*AFLA*/ 		switch (buffer[1]) {
// /*AFLA*/ 		case '0' ... '7':
// /*AFLA*/ 		case 'd':	/* KERN_DEFAULT */
// /*AFLA*/ 		case 'c':	/* KERN_CONT */
// /*AFLA*/ 			return buffer[1];
// /*AFLA*/ 		}
// /*AFLA*/ 	}
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline const char *printk_skip_level(const char *buffer)
// /*AFLA*/ {
// /*AFLA*/ 	if (printk_get_level(buffer))
// /*AFLA*/ 		return buffer + 2;
// /*AFLA*/ 
// /*AFLA*/ 	return buffer;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define CONSOLE_EXT_LOG_MAX	8192
// /*AFLA*/ 
// /*AFLA*/ /* printk's without a loglevel use this.. */
// /*AFLA*/ #define MESSAGE_LOGLEVEL_DEFAULT CONFIG_MESSAGE_LOGLEVEL_DEFAULT
// /*AFLA*/ 
// /*AFLA*/ /* We show everything that is MORE important than this.. */
// /*AFLA*/ #define CONSOLE_LOGLEVEL_SILENT  0 /* Mum's the word */
// /*AFLA*/ #define CONSOLE_LOGLEVEL_MIN	 1 /* Minimum loglevel we let people use */
// /*AFLA*/ #define CONSOLE_LOGLEVEL_QUIET	 4 /* Shhh ..., when booted with "quiet" */
// /*AFLA*/ #define CONSOLE_LOGLEVEL_DEFAULT 7 /* anything MORE serious than KERN_DEBUG */
// /*AFLA*/ #define CONSOLE_LOGLEVEL_DEBUG	10 /* issue debug messages */
// /*AFLA*/ #define CONSOLE_LOGLEVEL_MOTORMOUTH 15	/* You can't shut this one up */
// /*AFLA*/ 
// /*AFLA*/ extern int console_printk[];
// /*AFLA*/ 
// /*AFLA*/ #define console_loglevel (console_printk[0])
// /*AFLA*/ #define default_message_loglevel (console_printk[1])
// /*AFLA*/ #define minimum_console_loglevel (console_printk[2])
// /*AFLA*/ #define default_console_loglevel (console_printk[3])
// /*AFLA*/ 
// /*AFLA*/ static inline void console_silent(void)
// /*AFLA*/ {
// /*AFLA*/ 	console_loglevel = CONSOLE_LOGLEVEL_SILENT;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void console_verbose(void)
// /*AFLA*/ {
// /*AFLA*/ 	if (console_loglevel)
// /*AFLA*/ 		console_loglevel = CONSOLE_LOGLEVEL_MOTORMOUTH;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* strlen("ratelimit") + 1 */
// /*AFLA*/ #define DEVKMSG_STR_MAX_SIZE 10
// /*AFLA*/ extern char devkmsg_log_str[];
// /*AFLA*/ struct ctl_table;
// /*AFLA*/ 
// /*AFLA*/ struct va_format {
// /*AFLA*/ 	const char *fmt;
// /*AFLA*/ 	va_list *va;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * FW_BUG
// /*AFLA*/  * Add this to a message where you are sure the firmware is buggy or behaves
// /*AFLA*/  * really stupid or out of spec. Be aware that the responsible BIOS developer
// /*AFLA*/  * should be able to fix this issue or at least get a concrete idea of the
// /*AFLA*/  * problem by reading your message without the need of looking at the kernel
// /*AFLA*/  * code.
// /*AFLA*/  *
// /*AFLA*/  * Use it for definite and high priority BIOS bugs.
// /*AFLA*/  *
// /*AFLA*/  * FW_WARN
// /*AFLA*/  * Use it for not that clear (e.g. could the kernel messed up things already?)
// /*AFLA*/  * and medium priority BIOS bugs.
// /*AFLA*/  *
// /*AFLA*/  * FW_INFO
// /*AFLA*/  * Use this one if you want to tell the user or vendor about something
// /*AFLA*/  * suspicious, but generally harmless related to the firmware.
// /*AFLA*/  *
// /*AFLA*/  * Use it for information or very low priority BIOS bugs.
// /*AFLA*/  */
// /*AFLA*/ #define FW_BUG		"[Firmware Bug]: "
// /*AFLA*/ #define FW_WARN		"[Firmware Warn]: "
// /*AFLA*/ #define FW_INFO		"[Firmware Info]: "
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * HW_ERR
// /*AFLA*/  * Add this to a message for hardware errors, so that user can report
// /*AFLA*/  * it to hardware vendor instead of LKML or software vendor.
// /*AFLA*/  */
// /*AFLA*/ #define HW_ERR		"[Hardware Error]: "
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * DEPRECATED
// /*AFLA*/  * Add this to a message whenever you want to warn user space about the use
// /*AFLA*/  * of a deprecated aspect of an API so they can stop using it
// /*AFLA*/  */
// /*AFLA*/ #define DEPRECATED	"[Deprecated]: "
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Dummy printk for disabled debugging statements to use whilst maintaining
// /*AFLA*/  * gcc's format checking.
// /*AFLA*/  */
// /*AFLA*/ #define no_printk(fmt, ...)				\
// /*AFLA*/ ({							\
// /*AFLA*/ 	do {						\
// /*AFLA*/ 		if (0)					\
// /*AFLA*/ 			printk(fmt, ##__VA_ARGS__);	\
// /*AFLA*/ 	} while (0);					\
// /*AFLA*/ 	0;						\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_EARLY_PRINTK
// /*AFLA*/ extern asmlinkage __printf(1, 2)
// /*AFLA*/ void early_printk(const char *fmt, ...);
// /*AFLA*/ #else
// /*AFLA*/ static inline __printf(1, 2) __cold
// /*AFLA*/ void early_printk(const char *s, ...) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PRINTK_NMI
// /*AFLA*/ extern void printk_nmi_init(void);
// /*AFLA*/ extern void printk_nmi_enter(void);
// /*AFLA*/ extern void printk_nmi_exit(void);
// /*AFLA*/ extern void printk_nmi_flush(void);
// /*AFLA*/ extern void printk_nmi_flush_on_panic(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline void printk_nmi_init(void) { }
// /*AFLA*/ static inline void printk_nmi_enter(void) { }
// /*AFLA*/ static inline void printk_nmi_exit(void) { }
// /*AFLA*/ static inline void printk_nmi_flush(void) { }
// /*AFLA*/ static inline void printk_nmi_flush_on_panic(void) { }
// /*AFLA*/ #endif /* PRINTK_NMI */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PRINTK
// /*AFLA*/ asmlinkage __printf(5, 0)
// /*AFLA*/ int vprintk_emit(int facility, int level,
// /*AFLA*/ 		 const char *dict, size_t dictlen,
// /*AFLA*/ 		 const char *fmt, va_list args);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage __printf(1, 0)
// /*AFLA*/ int vprintk(const char *fmt, va_list args);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage __printf(5, 6) __cold
// /*AFLA*/ int printk_emit(int facility, int level,
// /*AFLA*/ 		const char *dict, size_t dictlen,
// /*AFLA*/ 		const char *fmt, ...);
// /*AFLA*/ 
// /*AFLA*/ asmlinkage __printf(1, 2) __cold
// /*AFLA*/ int printk(const char *fmt, ...);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Special printk facility for scheduler/timekeeping use only, _DO_NOT_USE_ !
// /*AFLA*/  */
// /*AFLA*/ __printf(1, 2) __cold int printk_deferred(const char *fmt, ...);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Please don't use printk_ratelimit(), because it shares ratelimiting state
// /*AFLA*/  * with all other unrelated printk_ratelimit() callsites.  Instead use
// /*AFLA*/  * printk_ratelimited() or plain old __ratelimit().
// /*AFLA*/  */
// /*AFLA*/ extern int __printk_ratelimit(const char *func);
// /*AFLA*/ #define printk_ratelimit() __printk_ratelimit(__func__)
// /*AFLA*/ extern bool printk_timed_ratelimit(unsigned long *caller_jiffies,
// /*AFLA*/ 				   unsigned int interval_msec);
// /*AFLA*/ 
// /*AFLA*/ extern int printk_delay_msec;
// /*AFLA*/ extern int dmesg_restrict;
// /*AFLA*/ extern int kptr_restrict;
// /*AFLA*/ 
// /*AFLA*/ extern int
// /*AFLA*/ devkmsg_sysctl_set_loglvl(struct ctl_table *table, int write, void __user *buf,
// /*AFLA*/ 			  size_t *lenp, loff_t *ppos);
// /*AFLA*/ 
// /*AFLA*/ extern void wake_up_klogd(void);
// /*AFLA*/ 
// /*AFLA*/ char *log_buf_addr_get(void);
// /*AFLA*/ u32 log_buf_len_get(void);
// /*AFLA*/ void log_buf_kexec_setup(void);
// /*AFLA*/ void __init setup_log_buf(int early);
// /*AFLA*/ __printf(1, 2) void dump_stack_set_arch_desc(const char *fmt, ...);
// /*AFLA*/ void dump_stack_print_info(const char *log_lvl);
// /*AFLA*/ void show_regs_print_info(const char *log_lvl);
// /*AFLA*/ #else
// /*AFLA*/ static inline __printf(1, 0)
// /*AFLA*/ int vprintk(const char *s, va_list args)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline __printf(1, 2) __cold
// /*AFLA*/ int printk(const char *s, ...)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline __printf(1, 2) __cold
// /*AFLA*/ int printk_deferred(const char *s, ...)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline int printk_ratelimit(void)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline bool printk_timed_ratelimit(unsigned long *caller_jiffies,
// /*AFLA*/ 					  unsigned int interval_msec)
// /*AFLA*/ {
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void wake_up_klogd(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline char *log_buf_addr_get(void)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u32 log_buf_len_get(void)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void log_buf_kexec_setup(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void setup_log_buf(int early)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline __printf(1, 2) void dump_stack_set_arch_desc(const char *fmt, ...)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void dump_stack_print_info(const char *log_lvl)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void show_regs_print_info(const char *log_lvl)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern asmlinkage void dump_stack(void) __cold;
// /*AFLA*/ 
// /*AFLA*/ #ifndef pr_fmt
// /*AFLA*/ #define pr_fmt(fmt) fmt
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These can be used to print at the various log levels.
// /*AFLA*/  * All of these will print unconditionally, although note that pr_debug()
// /*AFLA*/  * and other debug macros are compiled out unless either DEBUG is defined
// /*AFLA*/  * or CONFIG_DYNAMIC_DEBUG is set.
// /*AFLA*/  */
// /*AFLA*/ #define pr_emerg(fmt, ...) \
// /*AFLA*/ 	printk(KERN_EMERG pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #define pr_alert(fmt, ...) \
// /*AFLA*/ 	printk(KERN_ALERT pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #define pr_crit(fmt, ...) \
// /*AFLA*/ 	printk(KERN_CRIT pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #define pr_err(fmt, ...) \
// /*AFLA*/ 	printk(KERN_ERR pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #define pr_warning(fmt, ...) \
// /*AFLA*/ 	printk(KERN_WARNING pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #define pr_warn pr_warning
// /*AFLA*/ #define pr_notice(fmt, ...) \
// /*AFLA*/ 	printk(KERN_NOTICE pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #define pr_info(fmt, ...) \
// /*AFLA*/ 	printk(KERN_INFO pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ /*
// /*AFLA*/  * Like KERN_CONT, pr_cont() should only be used when continuing
// /*AFLA*/  * a line with no newline ('\n') enclosed. Otherwise it defaults
// /*AFLA*/  * back to KERN_DEFAULT.
// /*AFLA*/  */
// /*AFLA*/ #define pr_cont(fmt, ...) \
// /*AFLA*/ 	printk(KERN_CONT fmt, ##__VA_ARGS__)
// /*AFLA*/ 
// /*AFLA*/ /* pr_devel() should produce zero code unless DEBUG is defined */
// /*AFLA*/ #ifdef DEBUG
// /*AFLA*/ #define pr_devel(fmt, ...) \
// /*AFLA*/ 	printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #else
// /*AFLA*/ #define pr_devel(fmt, ...) \
// /*AFLA*/ 	no_printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /* If you are writing a driver, please use dev_dbg instead */
// /*AFLA*/ #if defined(CONFIG_DYNAMIC_DEBUG)
// /*AFLA*/ #include <linux/dynamic_debug.h>
// /*AFLA*/ 
// /*AFLA*/ /* dynamic_pr_debug() uses pr_fmt() internally so we don't need it here */
// /*AFLA*/ #define pr_debug(fmt, ...) \
// /*AFLA*/ 	dynamic_pr_debug(fmt, ##__VA_ARGS__)
// /*AFLA*/ #elif defined(DEBUG)
// /*AFLA*/ #define pr_debug(fmt, ...) \
// /*AFLA*/ 	printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #else
// /*AFLA*/ #define pr_debug(fmt, ...) \
// /*AFLA*/ 	no_printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Print a one-time message (analogous to WARN_ONCE() et al):
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PRINTK
// /*AFLA*/ #define printk_once(fmt, ...)					\
// /*AFLA*/ ({								\
// /*AFLA*/ 	static bool __print_once __read_mostly;			\
// /*AFLA*/ 	bool __ret_print_once = !__print_once;			\
// /*AFLA*/ 								\
// /*AFLA*/ 	if (!__print_once) {					\
// /*AFLA*/ 		__print_once = true;				\
// /*AFLA*/ 		printk(fmt, ##__VA_ARGS__);			\
// /*AFLA*/ 	}							\
// /*AFLA*/ 	unlikely(__ret_print_once);				\
// /*AFLA*/ })
// /*AFLA*/ #define printk_deferred_once(fmt, ...)				\
// /*AFLA*/ ({								\
// /*AFLA*/ 	static bool __print_once __read_mostly;			\
// /*AFLA*/ 	bool __ret_print_once = !__print_once;			\
// /*AFLA*/ 								\
// /*AFLA*/ 	if (!__print_once) {					\
// /*AFLA*/ 		__print_once = true;				\
// /*AFLA*/ 		printk_deferred(fmt, ##__VA_ARGS__);		\
// /*AFLA*/ 	}							\
// /*AFLA*/ 	unlikely(__ret_print_once);				\
// /*AFLA*/ })
// /*AFLA*/ #else
// /*AFLA*/ #define printk_once(fmt, ...)					\
// /*AFLA*/ 	no_printk(fmt, ##__VA_ARGS__)
// /*AFLA*/ #define printk_deferred_once(fmt, ...)				\
// /*AFLA*/ 	no_printk(fmt, ##__VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define pr_emerg_once(fmt, ...)					\
// /*AFLA*/ 	printk_once(KERN_EMERG pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #define pr_alert_once(fmt, ...)					\
// /*AFLA*/ 	printk_once(KERN_ALERT pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #define pr_crit_once(fmt, ...)					\
// /*AFLA*/ 	printk_once(KERN_CRIT pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #define pr_err_once(fmt, ...)					\
// /*AFLA*/ 	printk_once(KERN_ERR pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #define pr_warn_once(fmt, ...)					\
// /*AFLA*/ 	printk_once(KERN_WARNING pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #define pr_notice_once(fmt, ...)				\
// /*AFLA*/ 	printk_once(KERN_NOTICE pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #define pr_info_once(fmt, ...)					\
// /*AFLA*/ 	printk_once(KERN_INFO pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #define pr_cont_once(fmt, ...)					\
// /*AFLA*/ 	printk_once(KERN_CONT pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ 
// /*AFLA*/ #if defined(DEBUG)
// /*AFLA*/ #define pr_devel_once(fmt, ...)					\
// /*AFLA*/ 	printk_once(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #else
// /*AFLA*/ #define pr_devel_once(fmt, ...)					\
// /*AFLA*/ 	no_printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* If you are writing a driver, please use dev_dbg instead */
// /*AFLA*/ #if defined(DEBUG)
// /*AFLA*/ #define pr_debug_once(fmt, ...)					\
// /*AFLA*/ 	printk_once(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #else
// /*AFLA*/ #define pr_debug_once(fmt, ...)					\
// /*AFLA*/ 	no_printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * ratelimited messages with local ratelimit_state,
// /*AFLA*/  * no local ratelimit_state used in the !PRINTK case
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_PRINTK
// /*AFLA*/ #define printk_ratelimited(fmt, ...)					\
// /*AFLA*/ ({									\
// /*AFLA*/ 	static DEFINE_RATELIMIT_STATE(_rs,				\
// /*AFLA*/ 				      DEFAULT_RATELIMIT_INTERVAL,	\
// /*AFLA*/ 				      DEFAULT_RATELIMIT_BURST);		\
// /*AFLA*/ 									\
// /*AFLA*/ 	if (__ratelimit(&_rs))						\
// /*AFLA*/ 		printk(fmt, ##__VA_ARGS__);				\
// /*AFLA*/ })
// /*AFLA*/ #else
// /*AFLA*/ #define printk_ratelimited(fmt, ...)					\
// /*AFLA*/ 	no_printk(fmt, ##__VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define pr_emerg_ratelimited(fmt, ...)					\
// /*AFLA*/ 	printk_ratelimited(KERN_EMERG pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #define pr_alert_ratelimited(fmt, ...)					\
// /*AFLA*/ 	printk_ratelimited(KERN_ALERT pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #define pr_crit_ratelimited(fmt, ...)					\
// /*AFLA*/ 	printk_ratelimited(KERN_CRIT pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #define pr_err_ratelimited(fmt, ...)					\
// /*AFLA*/ 	printk_ratelimited(KERN_ERR pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #define pr_warn_ratelimited(fmt, ...)					\
// /*AFLA*/ 	printk_ratelimited(KERN_WARNING pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #define pr_notice_ratelimited(fmt, ...)					\
// /*AFLA*/ 	printk_ratelimited(KERN_NOTICE pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #define pr_info_ratelimited(fmt, ...)					\
// /*AFLA*/ 	printk_ratelimited(KERN_INFO pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ /* no pr_cont_ratelimited, don't do that... */
// /*AFLA*/ 
// /*AFLA*/ #if defined(DEBUG)
// /*AFLA*/ #define pr_devel_ratelimited(fmt, ...)					\
// /*AFLA*/ 	printk_ratelimited(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #else
// /*AFLA*/ #define pr_devel_ratelimited(fmt, ...)					\
// /*AFLA*/ 	no_printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* If you are writing a driver, please use dev_dbg instead */
// /*AFLA*/ #if defined(CONFIG_DYNAMIC_DEBUG)
// /*AFLA*/ /* descriptor check is first to prevent flooding with "callbacks suppressed" */
// /*AFLA*/ #define pr_debug_ratelimited(fmt, ...)					\
// /*AFLA*/ do {									\
// /*AFLA*/ 	static DEFINE_RATELIMIT_STATE(_rs,				\
// /*AFLA*/ 				      DEFAULT_RATELIMIT_INTERVAL,	\
// /*AFLA*/ 				      DEFAULT_RATELIMIT_BURST);		\
// /*AFLA*/ 	DEFINE_DYNAMIC_DEBUG_METADATA(descriptor, pr_fmt(fmt));		\
// /*AFLA*/ 	if (unlikely(descriptor.flags & _DPRINTK_FLAGS_PRINT) &&	\
// /*AFLA*/ 	    __ratelimit(&_rs))						\
// /*AFLA*/ 		__dynamic_pr_debug(&descriptor, pr_fmt(fmt), ##__VA_ARGS__);	\
// /*AFLA*/ } while (0)
// /*AFLA*/ #elif defined(DEBUG)
// /*AFLA*/ #define pr_debug_ratelimited(fmt, ...)					\
// /*AFLA*/ 	printk_ratelimited(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #else
// /*AFLA*/ #define pr_debug_ratelimited(fmt, ...) \
// /*AFLA*/ 	no_printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern const struct file_operations kmsg_fops;
// /*AFLA*/ 
// /*AFLA*/ enum {
// /*AFLA*/ 	DUMP_PREFIX_NONE,
// /*AFLA*/ 	DUMP_PREFIX_ADDRESS,
// /*AFLA*/ 	DUMP_PREFIX_OFFSET
// /*AFLA*/ };
// /*AFLA*/ extern int hex_dump_to_buffer(const void *buf, size_t len, int rowsize,
// /*AFLA*/ 			      int groupsize, char *linebuf, size_t linebuflen,
// /*AFLA*/ 			      bool ascii);
// /*AFLA*/ #ifdef CONFIG_PRINTK
// /*AFLA*/ extern void print_hex_dump(const char *level, const char *prefix_str,
// /*AFLA*/ 			   int prefix_type, int rowsize, int groupsize,
// /*AFLA*/ 			   const void *buf, size_t len, bool ascii);
// /*AFLA*/ #if defined(CONFIG_DYNAMIC_DEBUG)
// /*AFLA*/ #define print_hex_dump_bytes(prefix_str, prefix_type, buf, len)	\
// /*AFLA*/ 	dynamic_hex_dump(prefix_str, prefix_type, 16, 1, buf, len, true)
// /*AFLA*/ #else
// /*AFLA*/ extern void print_hex_dump_bytes(const char *prefix_str, int prefix_type,
// /*AFLA*/ 				 const void *buf, size_t len);
// /*AFLA*/ #endif /* defined(CONFIG_DYNAMIC_DEBUG) */
// /*AFLA*/ #else
// /*AFLA*/ static inline void print_hex_dump(const char *level, const char *prefix_str,
// /*AFLA*/ 				  int prefix_type, int rowsize, int groupsize,
// /*AFLA*/ 				  const void *buf, size_t len, bool ascii)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void print_hex_dump_bytes(const char *prefix_str, int prefix_type,
// /*AFLA*/ 					const void *buf, size_t len)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_DYNAMIC_DEBUG)
// /*AFLA*/ #define print_hex_dump_debug(prefix_str, prefix_type, rowsize,	\
// /*AFLA*/ 			     groupsize, buf, len, ascii)	\
// /*AFLA*/ 	dynamic_hex_dump(prefix_str, prefix_type, rowsize,	\
// /*AFLA*/ 			 groupsize, buf, len, ascii)
// /*AFLA*/ #elif defined(DEBUG)
// /*AFLA*/ #define print_hex_dump_debug(prefix_str, prefix_type, rowsize,		\
// /*AFLA*/ 			     groupsize, buf, len, ascii)		\
// /*AFLA*/ 	print_hex_dump(KERN_DEBUG, prefix_str, prefix_type, rowsize,	\
// /*AFLA*/ 		       groupsize, buf, len, ascii)
// /*AFLA*/ #else
// /*AFLA*/ static inline void print_hex_dump_debug(const char *prefix_str, int prefix_type,
// /*AFLA*/ 					int rowsize, int groupsize,
// /*AFLA*/ 					const void *buf, size_t len, bool ascii)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif
