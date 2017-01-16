// /*AFLA*/ #ifndef _DELAYED_CALL_H
// /*AFLA*/ #define _DELAYED_CALL_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Poor man's closures; I wish we could've done them sanely polymorphic,
// /*AFLA*/  * but...
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ struct delayed_call {
// /*AFLA*/ 	void (*fn)(void *);
// /*AFLA*/ 	void *arg;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_DELAYED_CALL(name) struct delayed_call name = {NULL, NULL}
// /*AFLA*/ 
// /*AFLA*/ /* I really wish we had closures with sane typechecking... */
// /*AFLA*/ static inline void set_delayed_call(struct delayed_call *call,
// /*AFLA*/ 		void (*fn)(void *), void *arg)
// /*AFLA*/ {
// /*AFLA*/ 	call->fn = fn;
// /*AFLA*/ 	call->arg = arg;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void do_delayed_call(struct delayed_call *call)
// /*AFLA*/ {
// /*AFLA*/ 	if (call->fn)
// /*AFLA*/ 		call->fn(call->arg);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void clear_delayed_call(struct delayed_call *call)
// /*AFLA*/ {
// /*AFLA*/ 	call->fn = NULL;
// /*AFLA*/ }
// /*AFLA*/ #endif
