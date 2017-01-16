// /*AFLA*/ #ifndef _LINUX_CONTEXT_TRACKING_STATE_H
// /*AFLA*/ #define _LINUX_CONTEXT_TRACKING_STATE_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/percpu.h>
// /*AFLA*/ #include <linux/static_key.h>
// /*AFLA*/ 
// /*AFLA*/ struct context_tracking {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * When active is false, probes are unset in order
// /*AFLA*/ 	 * to minimize overhead: TIF flags are cleared
// /*AFLA*/ 	 * and calls to user_enter/exit are ignored. This
// /*AFLA*/ 	 * may be further optimized using static keys.
// /*AFLA*/ 	 */
// /*AFLA*/ 	bool active;
// /*AFLA*/ 	int recursion;
// /*AFLA*/ 	enum ctx_state {
// /*AFLA*/ 		CONTEXT_DISABLED = -1,	/* returned by ct_state() if unknown */
// /*AFLA*/ 		CONTEXT_KERNEL = 0,
// /*AFLA*/ 		CONTEXT_USER,
// /*AFLA*/ 		CONTEXT_GUEST,
// /*AFLA*/ 	} state;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_CONTEXT_TRACKING
// /*AFLA*/ extern struct static_key_false context_tracking_enabled;
// /*AFLA*/ DECLARE_PER_CPU(struct context_tracking, context_tracking);
// /*AFLA*/ 
// /*AFLA*/ static inline bool context_tracking_is_enabled(void)
// /*AFLA*/ {
// /*AFLA*/ 	return static_branch_unlikely(&context_tracking_enabled);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool context_tracking_cpu_is_enabled(void)
// /*AFLA*/ {
// /*AFLA*/ 	return __this_cpu_read(context_tracking.active);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool context_tracking_in_user(void)
// /*AFLA*/ {
// /*AFLA*/ 	return __this_cpu_read(context_tracking.state) == CONTEXT_USER;
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static inline bool context_tracking_in_user(void) { return false; }
// /*AFLA*/ static inline bool context_tracking_active(void) { return false; }
// /*AFLA*/ static inline bool context_tracking_is_enabled(void) { return false; }
// /*AFLA*/ static inline bool context_tracking_cpu_is_enabled(void) { return false; }
// /*AFLA*/ #endif /* CONFIG_CONTEXT_TRACKING */
// /*AFLA*/ 
// /*AFLA*/ #endif
