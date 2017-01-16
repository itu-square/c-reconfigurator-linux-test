// /*AFLA*/ #ifndef _ASM_GENERIC_RESOURCE_H
// /*AFLA*/ #define _ASM_GENERIC_RESOURCE_H
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/asm-generic/resource.h>
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * boot-time rlimit defaults for the init task:
// /*AFLA*/  */
// /*AFLA*/ #define INIT_RLIMITS							\
// /*AFLA*/ {									\
// /*AFLA*/ 	[RLIMIT_CPU]		= {  RLIM_INFINITY,  RLIM_INFINITY },	\
// /*AFLA*/ 	[RLIMIT_FSIZE]		= {  RLIM_INFINITY,  RLIM_INFINITY },	\
// /*AFLA*/ 	[RLIMIT_DATA]		= {  RLIM_INFINITY,  RLIM_INFINITY },	\
// /*AFLA*/ 	[RLIMIT_STACK]		= {       _STK_LIM,  RLIM_INFINITY },	\
// /*AFLA*/ 	[RLIMIT_CORE]		= {              0,  RLIM_INFINITY },	\
// /*AFLA*/ 	[RLIMIT_RSS]		= {  RLIM_INFINITY,  RLIM_INFINITY },	\
// /*AFLA*/ 	[RLIMIT_NPROC]		= {              0,              0 },	\
// /*AFLA*/ 	[RLIMIT_NOFILE]		= {   INR_OPEN_CUR,   INR_OPEN_MAX },	\
// /*AFLA*/ 	[RLIMIT_MEMLOCK]	= {    MLOCK_LIMIT,    MLOCK_LIMIT },	\
// /*AFLA*/ 	[RLIMIT_AS]		= {  RLIM_INFINITY,  RLIM_INFINITY },	\
// /*AFLA*/ 	[RLIMIT_LOCKS]		= {  RLIM_INFINITY,  RLIM_INFINITY },	\
// /*AFLA*/ 	[RLIMIT_SIGPENDING]	= { 		0,	       0 },	\
// /*AFLA*/ 	[RLIMIT_MSGQUEUE]	= {   MQ_BYTES_MAX,   MQ_BYTES_MAX },	\
// /*AFLA*/ 	[RLIMIT_NICE]		= { 0, 0 },				\
// /*AFLA*/ 	[RLIMIT_RTPRIO]		= { 0, 0 },				\
// /*AFLA*/ 	[RLIMIT_RTTIME]		= {  RLIM_INFINITY,  RLIM_INFINITY },	\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif
