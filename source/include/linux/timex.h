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
// /*AFLA*/ #ifndef _LINUX_TIMEX_H
// /*AFLA*/ #define _LINUX_TIMEX_H
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/linux/timex.h>
// /*AFLA*/ 
// /*AFLA*/ #define ADJ_ADJTIME		0x8000	/* switch between adjtime/adjtimex modes */
// /*AFLA*/ #define ADJ_OFFSET_SINGLESHOT	0x0001	/* old-fashioned adjtime */
// /*AFLA*/ #define ADJ_OFFSET_READONLY	0x2000	/* read-only adjtime */
// /*AFLA*/ #include <linux/compiler.h>
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ #include <linux/param.h>
// /*AFLA*/ 
// /*AFLA*/ #include <asm/timex.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef random_get_entropy
// /*AFLA*/ /*
// /*AFLA*/  * The random_get_entropy() function is used by the /dev/random driver
// /*AFLA*/  * in order to extract entropy via the relative unpredictability of
// /*AFLA*/  * when an interrupt takes places versus a high speed, fine-grained
// /*AFLA*/  * timing source or cycle counter.  Since it will be occurred on every
// /*AFLA*/  * single interrupt, it must have a very low cost/overhead.
// /*AFLA*/  *
// /*AFLA*/  * By default we use get_cycles() for this purpose, but individual
// /*AFLA*/  * architectures may override this in their asm/timex.h header file.
// /*AFLA*/  */
// /*AFLA*/ #define random_get_entropy()	get_cycles()
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * SHIFT_PLL is used as a dampening factor to define how much we
// /*AFLA*/  * adjust the frequency correction for a given offset in PLL mode.
// /*AFLA*/  * It also used in dampening the offset correction, to define how
// /*AFLA*/  * much of the current value in time_offset we correct for each
// /*AFLA*/  * second. Changing this value changes the stiffness of the ntp
// /*AFLA*/  * adjustment code. A lower value makes it more flexible, reducing
// /*AFLA*/  * NTP convergence time. A higher value makes it stiffer, increasing
// /*AFLA*/  * convergence time, but making the clock more stable.
// /*AFLA*/  *
// /*AFLA*/  * In David Mills' nanokernel reference implementation SHIFT_PLL is 4.
// /*AFLA*/  * However this seems to increase convergence time much too long.
// /*AFLA*/  *
// /*AFLA*/  * https://lists.ntp.org/pipermail/hackers/2008-January/003487.html
// /*AFLA*/  *
// /*AFLA*/  * In the above mailing list discussion, it seems the value of 4
// /*AFLA*/  * was appropriate for other Unix systems with HZ=100, and that
// /*AFLA*/  * SHIFT_PLL should be decreased as HZ increases. However, Linux's
// /*AFLA*/  * clock steering implementation is HZ independent.
// /*AFLA*/  *
// /*AFLA*/  * Through experimentation, a SHIFT_PLL value of 2 was found to allow
// /*AFLA*/  * for fast convergence (very similar to the NTPv3 code used prior to
// /*AFLA*/  * v2.6.19), with good clock stability.
// /*AFLA*/  *
// /*AFLA*/  *
// /*AFLA*/  * SHIFT_FLL is used as a dampening factor to define how much we
// /*AFLA*/  * adjust the frequency correction for a given offset in FLL mode.
// /*AFLA*/  * In David Mills' nanokernel reference implementation SHIFT_FLL is 2.
// /*AFLA*/  *
// /*AFLA*/  * MAXTC establishes the maximum time constant of the PLL.
// /*AFLA*/  */
// /*AFLA*/ #define SHIFT_PLL	2	/* PLL frequency factor (shift) */
// /*AFLA*/ #define SHIFT_FLL	2	/* FLL frequency factor (shift) */
// /*AFLA*/ #define MAXTC		10	/* maximum time constant (shift) */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * SHIFT_USEC defines the scaling (shift) of the time_freq and
// /*AFLA*/  * time_tolerance variables, which represent the current frequency
// /*AFLA*/  * offset and maximum frequency tolerance.
// /*AFLA*/  */
// /*AFLA*/ #define SHIFT_USEC 16		/* frequency offset scale (shift) */
// /*AFLA*/ #define PPM_SCALE ((s64)NSEC_PER_USEC << (NTP_SCALE_SHIFT - SHIFT_USEC))
// /*AFLA*/ #define PPM_SCALE_INV_SHIFT 19
// /*AFLA*/ #define PPM_SCALE_INV ((1LL << (PPM_SCALE_INV_SHIFT + NTP_SCALE_SHIFT)) / \
// /*AFLA*/ 		       PPM_SCALE + 1)
// /*AFLA*/ 
// /*AFLA*/ #define MAXPHASE 500000000L	/* max phase error (ns) */
// /*AFLA*/ #define MAXFREQ 500000		/* max frequency error (ns/s) */
// /*AFLA*/ #define MAXFREQ_SCALED ((s64)MAXFREQ << NTP_SCALE_SHIFT)
// /*AFLA*/ #define MINSEC 256		/* min interval between updates (s) */
// /*AFLA*/ #define MAXSEC 2048		/* max interval between updates (s) */
// /*AFLA*/ #define NTP_PHASE_LIMIT ((MAXPHASE / NSEC_PER_USEC) << 5) /* beyond max. dispersion */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * kernel variables
// /*AFLA*/  * Note: maximum error = NTP synch distance = dispersion + delay / 2;
// /*AFLA*/  * estimated error = NTP dispersion.
// /*AFLA*/  */
// /*AFLA*/ extern unsigned long tick_usec;		/* USER_HZ period (usec) */
// /*AFLA*/ extern unsigned long tick_nsec;		/* SHIFTED_HZ period (nsec) */
// /*AFLA*/ 
// /*AFLA*/ /* Required to safely shift negative values */
// /*AFLA*/ #define shift_right(x, s) ({	\
// /*AFLA*/ 	__typeof__(x) __x = (x);	\
// /*AFLA*/ 	__typeof__(s) __s = (s);	\
// /*AFLA*/ 	__x < 0 ? -(-__x >> __s) : __x >> __s;	\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define NTP_SCALE_SHIFT		32
// /*AFLA*/ 
// /*AFLA*/ #define NTP_INTERVAL_FREQ  (HZ)
// /*AFLA*/ #define NTP_INTERVAL_LENGTH (NSEC_PER_SEC/NTP_INTERVAL_FREQ)
// /*AFLA*/ 
// /*AFLA*/ extern int do_adjtimex(struct timex *);
// /*AFLA*/ extern void hardpps(const struct timespec64 *, const struct timespec64 *);
// /*AFLA*/ 
// /*AFLA*/ int read_current_timer(unsigned long *timer_val);
// /*AFLA*/ void ntp_notify_cmos_timer(void);
// /*AFLA*/ 
// /*AFLA*/ /* The clock frequency of the i8253/i8254 PIT */
// /*AFLA*/ #define PIT_TICK_RATE 1193182ul
// /*AFLA*/ 
// /*AFLA*/ #endif /* LINUX_TIMEX_H */
