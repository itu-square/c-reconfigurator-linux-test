// /*AFLA*/ #ifndef _LINUX_PERSONALITY_H
// /*AFLA*/ #define _LINUX_PERSONALITY_H
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/linux/personality.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Return the base personality without flags.
// /*AFLA*/  */
// /*AFLA*/ #define personality(pers)	(pers & PER_MASK)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Change personality of the currently running process.
// /*AFLA*/  */
// /*AFLA*/ #define set_personality(pers)	(current->personality = (pers))
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_PERSONALITY_H */
