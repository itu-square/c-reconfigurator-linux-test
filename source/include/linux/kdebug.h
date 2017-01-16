// /*AFLA*/ #ifndef _LINUX_KDEBUG_H
// /*AFLA*/ #define _LINUX_KDEBUG_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/kdebug.h>
// /*AFLA*/ 
// /*AFLA*/ struct notifier_block;
// /*AFLA*/ 
// /*AFLA*/ struct die_args {
// /*AFLA*/ 	struct pt_regs *regs;
// /*AFLA*/ 	const char *str;
// /*AFLA*/ 	long err;
// /*AFLA*/ 	int trapnr;
// /*AFLA*/ 	int signr;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ int register_die_notifier(struct notifier_block *nb);
// /*AFLA*/ int unregister_die_notifier(struct notifier_block *nb);
// /*AFLA*/ 
// /*AFLA*/ int notify_die(enum die_val val, const char *str,
// /*AFLA*/ 	       struct pt_regs *regs, long err, int trap, int sig);
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_KDEBUG_H */
