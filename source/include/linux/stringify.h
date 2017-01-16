// /*AFLA*/ #ifndef __LINUX_STRINGIFY_H
// /*AFLA*/ #define __LINUX_STRINGIFY_H
// /*AFLA*/ 
// /*AFLA*/ /* Indirect stringification.  Doing two levels allows the parameter to be a
// /*AFLA*/  * macro itself.  For example, compile with -DFOO=bar, __stringify(FOO)
// /*AFLA*/  * converts to "bar".
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define __stringify_1(x...)	#x
// /*AFLA*/ #define __stringify(x...)	__stringify_1(x)
// /*AFLA*/ 
// /*AFLA*/ #endif	/* !__LINUX_STRINGIFY_H */
