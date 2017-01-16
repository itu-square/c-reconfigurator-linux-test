// /*AFLA*/ /*****************************************************************************
// /*AFLA*/  *                                                                           *
// /*AFLA*/  * Copyright (c) David L. Mills 1993                                         *
// /*AFLA*/  *                                                                           *
// /*AFLA*/  * Permission to use, copy, modify, and distribute this software and its     *
// /*AFLA*/  * documentation for any purpose and without fee is hereby granted, provided *
// /*AFLA*/  * that the above copyright notice appears in all copies and that both the   *
// /*AFLA*/  * copyright notice and this permission notice appear in supporting          *
// /*AFLA*/  * documentation, and that the name University of Delaware not be used in    *
// /*AFLA*/  * advertising or publicity pertaining to distribution of the software       *
// /*AFLA*/  * without specific, written prior permission.  The University of Delaware   *
// /*AFLA*/  * makes no representations about the suitability this software for any      *
// /*AFLA*/  * purpose.  It is provided "as is" without express or implied warranty.     *
// /*AFLA*/  *                                                                           *
// /*AFLA*/  *****************************************************************************/
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Modification history timex.h
// /*AFLA*/  *
// /*AFLA*/  * 29 Dec 97	Russell King
// /*AFLA*/  *	Moved CLOCK_TICK_RATE, CLOCK_TICK_FACTOR and FINETUNE to asm/timex.h
// /*AFLA*/  *	for ARM machines
// /*AFLA*/  *
// /*AFLA*/  *  9 Jan 97    Adrian Sun
// /*AFLA*/  *      Shifted LATCH define to allow access to alpha machines.
// /*AFLA*/  *
// /*AFLA*/  * 26 Sep 94	David L. Mills
// /*AFLA*/  *	Added defines for hybrid phase/frequency-lock loop.
// /*AFLA*/  *
// /*AFLA*/  * 19 Mar 94	David L. Mills
// /*AFLA*/  *	Moved defines from kernel routines to header file and added new
// /*AFLA*/  *	defines for PPS phase-lock loop.
// /*AFLA*/  *
// /*AFLA*/  * 20 Feb 94	David L. Mills
// /*AFLA*/  *	Revised status codes and structures for external clock and PPS
// /*AFLA*/  *	signal discipline.
// /*AFLA*/  *
// /*AFLA*/  * 28 Nov 93	David L. Mills
// /*AFLA*/  *	Adjusted parameters to improve stability and increase poll
// /*AFLA*/  *	interval.
// /*AFLA*/  *
// /*AFLA*/  * 17 Sep 93    David L. Mills
// /*AFLA*/  *      Created file $NTP/include/sys/timex.h
// /*AFLA*/  * 07 Oct 93    Torsten Duwe
// /*AFLA*/  *      Derived linux/timex.h
// /*AFLA*/  * 1995-08-13    Torsten Duwe
// /*AFLA*/  *      kernel PLL updated to 1994-12-13 specs (rfc-1589)
// /*AFLA*/  * 1997-08-30    Ulrich Windl
// /*AFLA*/  *      Added new constant NTP_PHASE_LIMIT
// /*AFLA*/  * 2004-08-12    Christoph Lameter
// /*AFLA*/  *      Reworked time interpolation logic
// /*AFLA*/  */
// /*AFLA*/ #ifndef _UAPI_LINUX_TIMEX_H
// /*AFLA*/ #define _UAPI_LINUX_TIMEX_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/time.h>
// /*AFLA*/ 
// /*AFLA*/ #define NTP_API		4	/* NTP API version */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * syscall interface - used (mainly by NTP daemon)
// /*AFLA*/  * to discipline kernel clock oscillator
// /*AFLA*/  */
// /*AFLA*/ struct timex {
// /*AFLA*/ 	unsigned int modes;	/* mode selector */
// /*AFLA*/ 	__kernel_long_t offset;	/* time offset (usec) */
// /*AFLA*/ 	__kernel_long_t freq;	/* frequency offset (scaled ppm) */
// /*AFLA*/ 	__kernel_long_t maxerror;/* maximum error (usec) */
// /*AFLA*/ 	__kernel_long_t esterror;/* estimated error (usec) */
// /*AFLA*/ 	int status;		/* clock command/status */
// /*AFLA*/ 	__kernel_long_t constant;/* pll time constant */
// /*AFLA*/ 	__kernel_long_t precision;/* clock precision (usec) (read only) */
// /*AFLA*/ 	__kernel_long_t tolerance;/* clock frequency tolerance (ppm)
// /*AFLA*/ 				   * (read only)
// /*AFLA*/ 				   */
// /*AFLA*/ 	struct timeval time;	/* (read only, except for ADJ_SETOFFSET) */
// /*AFLA*/ 	__kernel_long_t tick;	/* (modified) usecs between clock ticks */
// /*AFLA*/ 
// /*AFLA*/ 	__kernel_long_t ppsfreq;/* pps frequency (scaled ppm) (ro) */
// /*AFLA*/ 	__kernel_long_t jitter; /* pps jitter (us) (ro) */
// /*AFLA*/ 	int shift;              /* interval duration (s) (shift) (ro) */
// /*AFLA*/ 	__kernel_long_t stabil;            /* pps stability (scaled ppm) (ro) */
// /*AFLA*/ 	__kernel_long_t jitcnt; /* jitter limit exceeded (ro) */
// /*AFLA*/ 	__kernel_long_t calcnt; /* calibration intervals (ro) */
// /*AFLA*/ 	__kernel_long_t errcnt; /* calibration errors (ro) */
// /*AFLA*/ 	__kernel_long_t stbcnt; /* stability limit exceeded (ro) */
// /*AFLA*/ 
// /*AFLA*/ 	int tai;		/* TAI offset (ro) */
// /*AFLA*/ 
// /*AFLA*/ 	int  :32; int  :32; int  :32; int  :32;
// /*AFLA*/ 	int  :32; int  :32; int  :32; int  :32;
// /*AFLA*/ 	int  :32; int  :32; int  :32;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Mode codes (timex.mode)
// /*AFLA*/  */
// /*AFLA*/ #define ADJ_OFFSET		0x0001	/* time offset */
// /*AFLA*/ #define ADJ_FREQUENCY		0x0002	/* frequency offset */
// /*AFLA*/ #define ADJ_MAXERROR		0x0004	/* maximum time error */
// /*AFLA*/ #define ADJ_ESTERROR		0x0008	/* estimated time error */
// /*AFLA*/ #define ADJ_STATUS		0x0010	/* clock status */
// /*AFLA*/ #define ADJ_TIMECONST		0x0020	/* pll time constant */
// /*AFLA*/ #define ADJ_TAI			0x0080	/* set TAI offset */
// /*AFLA*/ #define ADJ_SETOFFSET		0x0100  /* add 'time' to current time */
// /*AFLA*/ #define ADJ_MICRO		0x1000	/* select microsecond resolution */
// /*AFLA*/ #define ADJ_NANO		0x2000	/* select nanosecond resolution */
// /*AFLA*/ #define ADJ_TICK		0x4000	/* tick value */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __KERNEL__
// /*AFLA*/ #define ADJ_OFFSET_SINGLESHOT	0x8001	/* old-fashioned adjtime */
// /*AFLA*/ #define ADJ_OFFSET_SS_READ	0xa001	/* read-only adjtime */
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* NTP userland likes the MOD_ prefix better */
// /*AFLA*/ #define MOD_OFFSET	ADJ_OFFSET
// /*AFLA*/ #define MOD_FREQUENCY	ADJ_FREQUENCY
// /*AFLA*/ #define MOD_MAXERROR	ADJ_MAXERROR
// /*AFLA*/ #define MOD_ESTERROR	ADJ_ESTERROR
// /*AFLA*/ #define MOD_STATUS	ADJ_STATUS
// /*AFLA*/ #define MOD_TIMECONST	ADJ_TIMECONST
// /*AFLA*/ #define MOD_TAI	ADJ_TAI
// /*AFLA*/ #define MOD_MICRO	ADJ_MICRO
// /*AFLA*/ #define MOD_NANO	ADJ_NANO
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Status codes (timex.status)
// /*AFLA*/  */
// /*AFLA*/ #define STA_PLL		0x0001	/* enable PLL updates (rw) */
// /*AFLA*/ #define STA_PPSFREQ	0x0002	/* enable PPS freq discipline (rw) */
// /*AFLA*/ #define STA_PPSTIME	0x0004	/* enable PPS time discipline (rw) */
// /*AFLA*/ #define STA_FLL		0x0008	/* select frequency-lock mode (rw) */
// /*AFLA*/ 
// /*AFLA*/ #define STA_INS		0x0010	/* insert leap (rw) */
// /*AFLA*/ #define STA_DEL		0x0020	/* delete leap (rw) */
// /*AFLA*/ #define STA_UNSYNC	0x0040	/* clock unsynchronized (rw) */
// /*AFLA*/ #define STA_FREQHOLD	0x0080	/* hold frequency (rw) */
// /*AFLA*/ 
// /*AFLA*/ #define STA_PPSSIGNAL	0x0100	/* PPS signal present (ro) */
// /*AFLA*/ #define STA_PPSJITTER	0x0200	/* PPS signal jitter exceeded (ro) */
// /*AFLA*/ #define STA_PPSWANDER	0x0400	/* PPS signal wander exceeded (ro) */
// /*AFLA*/ #define STA_PPSERROR	0x0800	/* PPS signal calibration error (ro) */
// /*AFLA*/ 
// /*AFLA*/ #define STA_CLOCKERR	0x1000	/* clock hardware fault (ro) */
// /*AFLA*/ #define STA_NANO	0x2000	/* resolution (0 = us, 1 = ns) (ro) */
// /*AFLA*/ #define STA_MODE	0x4000	/* mode (0 = PLL, 1 = FLL) (ro) */
// /*AFLA*/ #define STA_CLK		0x8000	/* clock source (0 = A, 1 = B) (ro) */
// /*AFLA*/ 
// /*AFLA*/ /* read-only bits */
// /*AFLA*/ #define STA_RONLY (STA_PPSSIGNAL | STA_PPSJITTER | STA_PPSWANDER | \
// /*AFLA*/ 	STA_PPSERROR | STA_CLOCKERR | STA_NANO | STA_MODE | STA_CLK)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Clock states (time_state)
// /*AFLA*/  */
// /*AFLA*/ #define TIME_OK		0	/* clock synchronized, no leap second */
// /*AFLA*/ #define TIME_INS	1	/* insert leap second */
// /*AFLA*/ #define TIME_DEL	2	/* delete leap second */
// /*AFLA*/ #define TIME_OOP	3	/* leap second in progress */
// /*AFLA*/ #define TIME_WAIT	4	/* leap second has occurred */
// /*AFLA*/ #define TIME_ERROR	5	/* clock not synchronized */
// /*AFLA*/ #define TIME_BAD	TIME_ERROR /* bw compat */
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* _UAPI_LINUX_TIMEX_H */
