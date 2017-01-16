// /*AFLA*/ #ifndef TYPECHECK_H_INCLUDED
// /*AFLA*/ #define TYPECHECK_H_INCLUDED
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Check at compile time that something is of a particular type.
// /*AFLA*/  * Always evaluates to 1 so you may use it easily in comparisons.
// /*AFLA*/  */
// /*AFLA*/ #define typecheck(type,x) \
// /*AFLA*/ ({	type __dummy; \
// /*AFLA*/ 	typeof(x) __dummy2; \
// /*AFLA*/ 	(void)(&__dummy == &__dummy2); \
// /*AFLA*/ 	1; \
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Check at compile time that 'function' is a certain type, or is a pointer
// /*AFLA*/  * to that type (needs to use typedef for the function type.)
// /*AFLA*/  */
// /*AFLA*/ #define typecheck_fn(type,function) \
// /*AFLA*/ ({	typeof(type) __tmp = function; \
// /*AFLA*/ 	(void)__tmp; \
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #endif		/* TYPECHECK_H_INCLUDED */
