// /*AFLA*/ #ifndef __KERN_LEVELS_H__
// /*AFLA*/ #define __KERN_LEVELS_H__
// /*AFLA*/ 
// /*AFLA*/ #define KERN_SOH	"\001"		/* ASCII Start Of Header */
// /*AFLA*/ #define KERN_SOH_ASCII	'\001'
// /*AFLA*/ 
// /*AFLA*/ #define KERN_EMERG	KERN_SOH "0"	/* system is unusable */
// /*AFLA*/ #define KERN_ALERT	KERN_SOH "1"	/* action must be taken immediately */
// /*AFLA*/ #define KERN_CRIT	KERN_SOH "2"	/* critical conditions */
// /*AFLA*/ #define KERN_ERR	KERN_SOH "3"	/* error conditions */
// /*AFLA*/ #define KERN_WARNING	KERN_SOH "4"	/* warning conditions */
// /*AFLA*/ #define KERN_NOTICE	KERN_SOH "5"	/* normal but significant condition */
// /*AFLA*/ #define KERN_INFO	KERN_SOH "6"	/* informational */
// /*AFLA*/ #define KERN_DEBUG	KERN_SOH "7"	/* debug-level messages */
// /*AFLA*/ 
// /*AFLA*/ #define KERN_DEFAULT	KERN_SOH "d"	/* the default kernel loglevel */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Annotation for a "continued" line of log printout (only done after a
// /*AFLA*/  * line that had no enclosing \n). Only to be used by core/arch code
// /*AFLA*/  * during early bootup (a continued line is not SMP-safe otherwise).
// /*AFLA*/  */
// /*AFLA*/ #define KERN_CONT	KERN_SOH "c"
// /*AFLA*/ 
// /*AFLA*/ /* integer equivalents of KERN_<LEVEL> */
// /*AFLA*/ #define LOGLEVEL_SCHED		-2	/* Deferred messages from sched code
// /*AFLA*/ 					 * are set to this special level */
// /*AFLA*/ #define LOGLEVEL_DEFAULT	-1	/* default (or last) loglevel */
// /*AFLA*/ #define LOGLEVEL_EMERG		0	/* system is unusable */
// /*AFLA*/ #define LOGLEVEL_ALERT		1	/* action must be taken immediately */
// /*AFLA*/ #define LOGLEVEL_CRIT		2	/* critical conditions */
// /*AFLA*/ #define LOGLEVEL_ERR		3	/* error conditions */
// /*AFLA*/ #define LOGLEVEL_WARNING	4	/* warning conditions */
// /*AFLA*/ #define LOGLEVEL_NOTICE		5	/* normal but significant condition */
// /*AFLA*/ #define LOGLEVEL_INFO		6	/* informational */
// /*AFLA*/ #define LOGLEVEL_DEBUG		7	/* debug-level messages */
// /*AFLA*/ 
// /*AFLA*/ #endif
