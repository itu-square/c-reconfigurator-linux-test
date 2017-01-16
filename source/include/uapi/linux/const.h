// /*AFLA*/ /* const.h: Macros for dealing with constants.  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_CONST_H
// /*AFLA*/ #define _LINUX_CONST_H
// /*AFLA*/ 
// /*AFLA*/ /* Some constant macros are used in both assembler and
// /*AFLA*/  * C code.  Therefore we cannot annotate them always with
// /*AFLA*/  * 'UL' and other type specifiers unilaterally.  We
// /*AFLA*/  * use the following macros to deal with this.
// /*AFLA*/  *
// /*AFLA*/  * Similarly, _AT() will cast an expression with a type in C, but
// /*AFLA*/  * leave it unchanged in asm.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifdef __ASSEMBLY__
// /*AFLA*/ #define _AC(X,Y)	X
// /*AFLA*/ #define _AT(T,X)	X
// /*AFLA*/ #else
// /*AFLA*/ #define __AC(X,Y)	(X##Y)
// /*AFLA*/ #define _AC(X,Y)	__AC(X,Y)
// /*AFLA*/ #define _AT(T,X)	((T)(X))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define _BITUL(x)	(_AC(1,UL) << (x))
// /*AFLA*/ #define _BITULL(x)	(_AC(1,ULL) << (x))
// /*AFLA*/ 
// /*AFLA*/ #endif /* !(_LINUX_CONST_H) */
