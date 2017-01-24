#ifndef _LINUX_STDDEF_H
#define _LINUX_STDDEF_H

#include <uapi/linux/stddef.h>

#undef NULL
#define NULL ((void *)0)

enum {
	false	= 0,
	true	= 1
};
// /*AFLA*/ 
// /*AFLA*/ #undef offsetof
// /*AFLA*/ #ifdef __compiler_offsetof
// /*AFLA*/ #define offsetof(TYPE, MEMBER)	__compiler_offsetof(TYPE, MEMBER)
// /*AFLA*/ #else
// /*AFLA*/ #define offsetof(TYPE, MEMBER)	((size_t)&((TYPE *)0)->MEMBER)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * offsetofend(TYPE, MEMBER)
// /*AFLA*/  *
// /*AFLA*/  * @TYPE: The type of the structure
// /*AFLA*/  * @MEMBER: The member within the structure to get the end offset of
// /*AFLA*/  */
// /*AFLA*/ #define offsetofend(TYPE, MEMBER) \
// /*AFLA*/ 	(offsetof(TYPE, MEMBER)	+ sizeof(((TYPE *)0)->MEMBER))
// /*AFLA*/ 
#endif
