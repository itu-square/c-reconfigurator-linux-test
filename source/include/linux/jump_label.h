// /*AFLA*/ #ifndef _LINUX_JUMP_LABEL_H
// /*AFLA*/ #define _LINUX_JUMP_LABEL_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Jump label support
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) 2009-2012 Jason Baron <jbaron@redhat.com>
// /*AFLA*/  * Copyright (C) 2011-2012 Red Hat, Inc., Peter Zijlstra
// /*AFLA*/  *
// /*AFLA*/  * DEPRECATED API:
// /*AFLA*/  *
// /*AFLA*/  * The use of 'struct static_key' directly, is now DEPRECATED. In addition
// /*AFLA*/  * static_key_{true,false}() is also DEPRECATED. IE DO NOT use the following:
// /*AFLA*/  *
// /*AFLA*/  * struct static_key false = STATIC_KEY_INIT_FALSE;
// /*AFLA*/  * struct static_key true = STATIC_KEY_INIT_TRUE;
// /*AFLA*/  * static_key_true()
// /*AFLA*/  * static_key_false()
// /*AFLA*/  *
// /*AFLA*/  * The updated API replacements are:
// /*AFLA*/  *
// /*AFLA*/  * DEFINE_STATIC_KEY_TRUE(key);
// /*AFLA*/  * DEFINE_STATIC_KEY_FALSE(key);
// /*AFLA*/  * DEFINE_STATIC_KEY_ARRAY_TRUE(keys, count);
// /*AFLA*/  * DEFINE_STATIC_KEY_ARRAY_FALSE(keys, count);
// /*AFLA*/  * static_branch_likely()
// /*AFLA*/  * static_branch_unlikely()
// /*AFLA*/  *
// /*AFLA*/  * Jump labels provide an interface to generate dynamic branches using
// /*AFLA*/  * self-modifying code. Assuming toolchain and architecture support, if we
// /*AFLA*/  * define a "key" that is initially false via "DEFINE_STATIC_KEY_FALSE(key)",
// /*AFLA*/  * an "if (static_branch_unlikely(&key))" statement is an unconditional branch
// /*AFLA*/  * (which defaults to false - and the true block is placed out of line).
// /*AFLA*/  * Similarly, we can define an initially true key via
// /*AFLA*/  * "DEFINE_STATIC_KEY_TRUE(key)", and use it in the same
// /*AFLA*/  * "if (static_branch_unlikely(&key))", in which case we will generate an
// /*AFLA*/  * unconditional branch to the out-of-line true branch. Keys that are
// /*AFLA*/  * initially true or false can be using in both static_branch_unlikely()
// /*AFLA*/  * and static_branch_likely() statements.
// /*AFLA*/  *
// /*AFLA*/  * At runtime we can change the branch target by setting the key
// /*AFLA*/  * to true via a call to static_branch_enable(), or false using
// /*AFLA*/  * static_branch_disable(). If the direction of the branch is switched by
// /*AFLA*/  * these calls then we run-time modify the branch target via a
// /*AFLA*/  * no-op -> jump or jump -> no-op conversion. For example, for an
// /*AFLA*/  * initially false key that is used in an "if (static_branch_unlikely(&key))"
// /*AFLA*/  * statement, setting the key to true requires us to patch in a jump
// /*AFLA*/  * to the out-of-line of true branch.
// /*AFLA*/  *
// /*AFLA*/  * In addition to static_branch_{enable,disable}, we can also reference count
// /*AFLA*/  * the key or branch direction via static_branch_{inc,dec}. Thus,
// /*AFLA*/  * static_branch_inc() can be thought of as a 'make more true' and
// /*AFLA*/  * static_branch_dec() as a 'make more false'.
// /*AFLA*/  *
// /*AFLA*/  * Since this relies on modifying code, the branch modifying functions
// /*AFLA*/  * must be considered absolute slow paths (machine wide synchronization etc.).
// /*AFLA*/  * OTOH, since the affected branches are unconditional, their runtime overhead
// /*AFLA*/  * will be absolutely minimal, esp. in the default (off) case where the total
// /*AFLA*/  * effect is a single NOP of appropriate size. The on case will patch in a jump
// /*AFLA*/  * to the out-of-line block.
// /*AFLA*/  *
// /*AFLA*/  * When the control is directly exposed to userspace, it is prudent to delay the
// /*AFLA*/  * decrement to avoid high frequency code modifications which can (and do)
// /*AFLA*/  * cause significant performance degradation. Struct static_key_deferred and
// /*AFLA*/  * static_key_slow_dec_deferred() provide for this.
// /*AFLA*/  *
// /*AFLA*/  * Lacking toolchain and or architecture support, static keys fall back to a
// /*AFLA*/  * simple conditional branch.
// /*AFLA*/  *
// /*AFLA*/  * Additional babbling in: Documentation/static-keys.txt
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #if defined(CC_HAVE_ASM_GOTO) && defined(CONFIG_JUMP_LABEL)
// /*AFLA*/ # define HAVE_JUMP_LABEL
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ 
// /*AFLA*/ extern bool static_key_initialized;
// /*AFLA*/ 
// /*AFLA*/ #define STATIC_KEY_CHECK_USE() WARN(!static_key_initialized,		      \
// /*AFLA*/ 				    "%s used before call to jump_label_init", \
// /*AFLA*/ 				    __func__)
// /*AFLA*/ 
// /*AFLA*/ #ifdef HAVE_JUMP_LABEL
// /*AFLA*/ 
// /*AFLA*/ struct static_key {
// /*AFLA*/ 	atomic_t enabled;
// /*AFLA*/ /* Set lsb bit to 1 if branch is default true, 0 ot */
// /*AFLA*/ 	struct jump_entry *entries;
// /*AFLA*/ #ifdef CONFIG_MODULES
// /*AFLA*/ 	struct static_key_mod *next;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ struct static_key {
// /*AFLA*/ 	atomic_t enabled;
// /*AFLA*/ };
// /*AFLA*/ #endif	/* HAVE_JUMP_LABEL */
// /*AFLA*/ #endif /* __ASSEMBLY__ */
// /*AFLA*/ 
// /*AFLA*/ #ifdef HAVE_JUMP_LABEL
// /*AFLA*/ #include <asm/jump_label.h>
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __ASSEMBLY__
// /*AFLA*/ 
// /*AFLA*/ enum jump_label_type {
// /*AFLA*/ 	JUMP_LABEL_NOP = 0,
// /*AFLA*/ 	JUMP_LABEL_JMP,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct module;
// /*AFLA*/ 
// /*AFLA*/ #ifdef HAVE_JUMP_LABEL
// /*AFLA*/ 
// /*AFLA*/ #define JUMP_TYPE_FALSE	0UL
// /*AFLA*/ #define JUMP_TYPE_TRUE	1UL
// /*AFLA*/ #define JUMP_TYPE_MASK	1UL
// /*AFLA*/ 
// /*AFLA*/ static __always_inline bool static_key_false(struct static_key *key)
// /*AFLA*/ {
// /*AFLA*/ 	return arch_static_branch(key, false);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline bool static_key_true(struct static_key *key)
// /*AFLA*/ {
// /*AFLA*/ 	return !arch_static_branch(key, true);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern struct jump_entry __start___jump_table[];
// /*AFLA*/ extern struct jump_entry __stop___jump_table[];
// /*AFLA*/ 
// /*AFLA*/ extern void jump_label_init(void);
// /*AFLA*/ extern void jump_label_lock(void);
// /*AFLA*/ extern void jump_label_unlock(void);
// /*AFLA*/ extern void arch_jump_label_transform(struct jump_entry *entry,
// /*AFLA*/ 				      enum jump_label_type type);
// /*AFLA*/ extern void arch_jump_label_transform_static(struct jump_entry *entry,
// /*AFLA*/ 					     enum jump_label_type type);
// /*AFLA*/ extern int jump_label_text_reserved(void *start, void *end);
// /*AFLA*/ extern void static_key_slow_inc(struct static_key *key);
// /*AFLA*/ extern void static_key_slow_dec(struct static_key *key);
// /*AFLA*/ extern void jump_label_apply_nops(struct module *mod);
// /*AFLA*/ extern int static_key_count(struct static_key *key);
// /*AFLA*/ extern void static_key_enable(struct static_key *key);
// /*AFLA*/ extern void static_key_disable(struct static_key *key);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * We should be using ATOMIC_INIT() for initializing .enabled, but
// /*AFLA*/  * the inclusion of atomic.h is problematic for inclusion of jump_label.h
// /*AFLA*/  * in 'low-level' headers. Thus, we are initializing .enabled with a
// /*AFLA*/  * raw value, but have added a BUILD_BUG_ON() to catch any issues in
// /*AFLA*/  * jump_label_init() see: kernel/jump_label.c.
// /*AFLA*/  */
// /*AFLA*/ #define STATIC_KEY_INIT_TRUE					\
// /*AFLA*/ 	{ .enabled = { 1 },					\
// /*AFLA*/ 	  .entries = (void *)JUMP_TYPE_TRUE }
// /*AFLA*/ #define STATIC_KEY_INIT_FALSE					\
// /*AFLA*/ 	{ .enabled = { 0 },					\
// /*AFLA*/ 	  .entries = (void *)JUMP_TYPE_FALSE }
// /*AFLA*/ 
// /*AFLA*/ #else  /* !HAVE_JUMP_LABEL */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/atomic.h>
// /*AFLA*/ #include <linux/bug.h>
// /*AFLA*/ 
// /*AFLA*/ static inline int static_key_count(struct static_key *key)
// /*AFLA*/ {
// /*AFLA*/ 	return atomic_read(&key->enabled);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void jump_label_init(void)
// /*AFLA*/ {
// /*AFLA*/ 	static_key_initialized = true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline bool static_key_false(struct static_key *key)
// /*AFLA*/ {
// /*AFLA*/ 	if (unlikely(static_key_count(key) > 0))
// /*AFLA*/ 		return true;
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline bool static_key_true(struct static_key *key)
// /*AFLA*/ {
// /*AFLA*/ 	if (likely(static_key_count(key) > 0))
// /*AFLA*/ 		return true;
// /*AFLA*/ 	return false;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void static_key_slow_inc(struct static_key *key)
// /*AFLA*/ {
// /*AFLA*/ 	STATIC_KEY_CHECK_USE();
// /*AFLA*/ 	atomic_inc(&key->enabled);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void static_key_slow_dec(struct static_key *key)
// /*AFLA*/ {
// /*AFLA*/ 	STATIC_KEY_CHECK_USE();
// /*AFLA*/ 	atomic_dec(&key->enabled);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int jump_label_text_reserved(void *start, void *end)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void jump_label_lock(void) {}
// /*AFLA*/ static inline void jump_label_unlock(void) {}
// /*AFLA*/ 
// /*AFLA*/ static inline int jump_label_apply_nops(struct module *mod)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void static_key_enable(struct static_key *key)
// /*AFLA*/ {
// /*AFLA*/ 	int count = static_key_count(key);
// /*AFLA*/ 
// /*AFLA*/ 	WARN_ON_ONCE(count < 0 || count > 1);
// /*AFLA*/ 
// /*AFLA*/ 	if (!count)
// /*AFLA*/ 		static_key_slow_inc(key);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void static_key_disable(struct static_key *key)
// /*AFLA*/ {
// /*AFLA*/ 	int count = static_key_count(key);
// /*AFLA*/ 
// /*AFLA*/ 	WARN_ON_ONCE(count < 0 || count > 1);
// /*AFLA*/ 
// /*AFLA*/ 	if (count)
// /*AFLA*/ 		static_key_slow_dec(key);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define STATIC_KEY_INIT_TRUE	{ .enabled = ATOMIC_INIT(1) }
// /*AFLA*/ #define STATIC_KEY_INIT_FALSE	{ .enabled = ATOMIC_INIT(0) }
// /*AFLA*/ 
// /*AFLA*/ #endif	/* HAVE_JUMP_LABEL */
// /*AFLA*/ 
// /*AFLA*/ #define STATIC_KEY_INIT STATIC_KEY_INIT_FALSE
// /*AFLA*/ #define jump_label_enabled static_key_enabled
// /*AFLA*/ 
// /*AFLA*/ /* -------------------------------------------------------------------------- */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Two type wrappers around static_key, such that we can use compile time
// /*AFLA*/  * type differentiation to emit the right code.
// /*AFLA*/  *
// /*AFLA*/  * All the below code is macros in order to play type games.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ struct static_key_true {
// /*AFLA*/ 	struct static_key key;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct static_key_false {
// /*AFLA*/ 	struct static_key key;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define STATIC_KEY_TRUE_INIT  (struct static_key_true) { .key = STATIC_KEY_INIT_TRUE,  }
// /*AFLA*/ #define STATIC_KEY_FALSE_INIT (struct static_key_false){ .key = STATIC_KEY_INIT_FALSE, }
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_STATIC_KEY_TRUE(name)	\
// /*AFLA*/ 	struct static_key_true name = STATIC_KEY_TRUE_INIT
// /*AFLA*/ 
// /*AFLA*/ #define DECLARE_STATIC_KEY_TRUE(name)	\
// /*AFLA*/ 	extern struct static_key_true name
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_STATIC_KEY_FALSE(name)	\
// /*AFLA*/ 	struct static_key_false name = STATIC_KEY_FALSE_INIT
// /*AFLA*/ 
// /*AFLA*/ #define DECLARE_STATIC_KEY_FALSE(name)	\
// /*AFLA*/ 	extern struct static_key_false name
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_STATIC_KEY_ARRAY_TRUE(name, count)		\
// /*AFLA*/ 	struct static_key_true name[count] = {			\
// /*AFLA*/ 		[0 ... (count) - 1] = STATIC_KEY_TRUE_INIT,	\
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_STATIC_KEY_ARRAY_FALSE(name, count)		\
// /*AFLA*/ 	struct static_key_false name[count] = {			\
// /*AFLA*/ 		[0 ... (count) - 1] = STATIC_KEY_FALSE_INIT,	\
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ extern bool ____wrong_branch_error(void);
// /*AFLA*/ 
// /*AFLA*/ #define static_key_enabled(x)							\
// /*AFLA*/ ({										\
// /*AFLA*/ 	if (!__builtin_types_compatible_p(typeof(*x), struct static_key) &&	\
// /*AFLA*/ 	    !__builtin_types_compatible_p(typeof(*x), struct static_key_true) &&\
// /*AFLA*/ 	    !__builtin_types_compatible_p(typeof(*x), struct static_key_false))	\
// /*AFLA*/ 		____wrong_branch_error();					\
// /*AFLA*/ 	static_key_count((struct static_key *)x) > 0;				\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #ifdef HAVE_JUMP_LABEL
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Combine the right initial value (type) with the right branch order
// /*AFLA*/  * to generate the desired result.
// /*AFLA*/  *
// /*AFLA*/  *
// /*AFLA*/  * type\branch|	likely (1)	      |	unlikely (0)
// /*AFLA*/  * -----------+-----------------------+------------------
// /*AFLA*/  *            |                       |
// /*AFLA*/  *  true (1)  |	   ...		      |	   ...
// /*AFLA*/  *            |    NOP		      |	   JMP L
// /*AFLA*/  *            |    <br-stmts>	      |	1: ...
// /*AFLA*/  *            |	L: ...		      |
// /*AFLA*/  *            |			      |
// /*AFLA*/  *            |			      |	L: <br-stmts>
// /*AFLA*/  *            |			      |	   jmp 1b
// /*AFLA*/  *            |                       |
// /*AFLA*/  * -----------+-----------------------+------------------
// /*AFLA*/  *            |                       |
// /*AFLA*/  *  false (0) |	   ...		      |	   ...
// /*AFLA*/  *            |    JMP L	      |	   NOP
// /*AFLA*/  *            |    <br-stmts>	      |	1: ...
// /*AFLA*/  *            |	L: ...		      |
// /*AFLA*/  *            |			      |
// /*AFLA*/  *            |			      |	L: <br-stmts>
// /*AFLA*/  *            |			      |	   jmp 1b
// /*AFLA*/  *            |                       |
// /*AFLA*/  * -----------+-----------------------+------------------
// /*AFLA*/  *
// /*AFLA*/  * The initial value is encoded in the LSB of static_key::entries,
// /*AFLA*/  * type: 0 = false, 1 = true.
// /*AFLA*/  *
// /*AFLA*/  * The branch type is encoded in the LSB of jump_entry::key,
// /*AFLA*/  * branch: 0 = unlikely, 1 = likely.
// /*AFLA*/  *
// /*AFLA*/  * This gives the following logic table:
// /*AFLA*/  *
// /*AFLA*/  *	enabled	type	branch	  instuction
// /*AFLA*/  * -----------------------------+-----------
// /*AFLA*/  *	0	0	0	| NOP
// /*AFLA*/  *	0	0	1	| JMP
// /*AFLA*/  *	0	1	0	| NOP
// /*AFLA*/  *	0	1	1	| JMP
// /*AFLA*/  *
// /*AFLA*/  *	1	0	0	| JMP
// /*AFLA*/  *	1	0	1	| NOP
// /*AFLA*/  *	1	1	0	| JMP
// /*AFLA*/  *	1	1	1	| NOP
// /*AFLA*/  *
// /*AFLA*/  * Which gives the following functions:
// /*AFLA*/  *
// /*AFLA*/  *   dynamic: instruction = enabled ^ branch
// /*AFLA*/  *   static:  instruction = type ^ branch
// /*AFLA*/  *
// /*AFLA*/  * See jump_label_type() / jump_label_init_type().
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define static_branch_likely(x)							\
// /*AFLA*/ ({										\
// /*AFLA*/ 	bool branch;								\
// /*AFLA*/ 	if (__builtin_types_compatible_p(typeof(*x), struct static_key_true))	\
// /*AFLA*/ 		branch = !arch_static_branch(&(x)->key, true);			\
// /*AFLA*/ 	else if (__builtin_types_compatible_p(typeof(*x), struct static_key_false)) \
// /*AFLA*/ 		branch = !arch_static_branch_jump(&(x)->key, true);		\
// /*AFLA*/ 	else									\
// /*AFLA*/ 		branch = ____wrong_branch_error();				\
// /*AFLA*/ 	branch;									\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define static_branch_unlikely(x)						\
// /*AFLA*/ ({										\
// /*AFLA*/ 	bool branch;								\
// /*AFLA*/ 	if (__builtin_types_compatible_p(typeof(*x), struct static_key_true))	\
// /*AFLA*/ 		branch = arch_static_branch_jump(&(x)->key, false);		\
// /*AFLA*/ 	else if (__builtin_types_compatible_p(typeof(*x), struct static_key_false)) \
// /*AFLA*/ 		branch = arch_static_branch(&(x)->key, false);			\
// /*AFLA*/ 	else									\
// /*AFLA*/ 		branch = ____wrong_branch_error();				\
// /*AFLA*/ 	branch;									\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #else /* !HAVE_JUMP_LABEL */
// /*AFLA*/ 
// /*AFLA*/ #define static_branch_likely(x)		likely(static_key_enabled(&(x)->key))
// /*AFLA*/ #define static_branch_unlikely(x)	unlikely(static_key_enabled(&(x)->key))
// /*AFLA*/ 
// /*AFLA*/ #endif /* HAVE_JUMP_LABEL */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Advanced usage; refcount, branch is enabled when: count != 0
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define static_branch_inc(x)		static_key_slow_inc(&(x)->key)
// /*AFLA*/ #define static_branch_dec(x)		static_key_slow_dec(&(x)->key)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Normal usage; boolean enable/disable.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define static_branch_enable(x)		static_key_enable(&(x)->key)
// /*AFLA*/ #define static_branch_disable(x)	static_key_disable(&(x)->key)
// /*AFLA*/ 
// /*AFLA*/ #endif	/* _LINUX_JUMP_LABEL_H */
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASSEMBLY__ */
