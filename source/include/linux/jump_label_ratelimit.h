// /*AFLA*/ #ifndef _LINUX_JUMP_LABEL_RATELIMIT_H
// /*AFLA*/ #define _LINUX_JUMP_LABEL_RATELIMIT_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/jump_label.h>
// /*AFLA*/ #include <linux/workqueue.h>
// /*AFLA*/ 
// /*AFLA*/ #if defined(CC_HAVE_ASM_GOTO) && defined(CONFIG_JUMP_LABEL)
// /*AFLA*/ struct static_key_deferred {
// /*AFLA*/ 	struct static_key key;
// /*AFLA*/ 	unsigned long timeout;
// /*AFLA*/ 	struct delayed_work work;
// /*AFLA*/ };
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef HAVE_JUMP_LABEL
// /*AFLA*/ extern void static_key_slow_dec_deferred(struct static_key_deferred *key);
// /*AFLA*/ extern void
// /*AFLA*/ jump_label_rate_limit(struct static_key_deferred *key, unsigned long rl);
// /*AFLA*/ 
// /*AFLA*/ #else	/* !HAVE_JUMP_LABEL */
// /*AFLA*/ struct static_key_deferred {
// /*AFLA*/ 	struct static_key  key;
// /*AFLA*/ };
// /*AFLA*/ static inline void static_key_slow_dec_deferred(struct static_key_deferred *key)
// /*AFLA*/ {
// /*AFLA*/ 	STATIC_KEY_CHECK_USE();
// /*AFLA*/ 	static_key_slow_dec(&key->key);
// /*AFLA*/ }
// /*AFLA*/ static inline void
// /*AFLA*/ jump_label_rate_limit(struct static_key_deferred *key,
// /*AFLA*/ 		unsigned long rl)
// /*AFLA*/ {
// /*AFLA*/ 	STATIC_KEY_CHECK_USE();
// /*AFLA*/ }
// /*AFLA*/ #endif	/* HAVE_JUMP_LABEL */
// /*AFLA*/ #endif	/* _LINUX_JUMP_LABEL_RATELIMIT_H */
