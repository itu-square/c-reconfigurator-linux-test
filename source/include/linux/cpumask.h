// /*AFLA*/ #ifndef __LINUX_CPUMASK_H
// /*AFLA*/ #define __LINUX_CPUMASK_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Cpumasks provide a bitmap suitable for representing the
// /*AFLA*/  * set of CPU's in a system, one bit position per CPU number.  In general,
// /*AFLA*/  * only nr_cpu_ids (<= NR_CPUS) bits are valid.
// /*AFLA*/  */
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ #include <linux/threads.h>
// /*AFLA*/ #include <linux/bitmap.h>
// /*AFLA*/ #include <linux/bug.h>
// /*AFLA*/ 
// /*AFLA*/ /* Don't assign or return these: may not be this big! */
// /*AFLA*/ typedef struct cpumask { DECLARE_BITMAP(bits, NR_CPUS); } cpumask_t;
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_bits - get the bits in a cpumask
// /*AFLA*/  * @maskp: the struct cpumask *
// /*AFLA*/  *
// /*AFLA*/  * You should only assume nr_cpu_ids bits of this mask are valid.  This is
// /*AFLA*/  * a macro so it's const-correct.
// /*AFLA*/  */
// /*AFLA*/ #define cpumask_bits(maskp) ((maskp)->bits)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_pr_args - printf args to output a cpumask
// /*AFLA*/  * @maskp: cpumask to be printed
// /*AFLA*/  *
// /*AFLA*/  * Can be used to provide arguments for '%*pb[l]' when printing a cpumask.
// /*AFLA*/  */
// /*AFLA*/ #define cpumask_pr_args(maskp)		nr_cpu_ids, cpumask_bits(maskp)
// /*AFLA*/ 
// /*AFLA*/ #if NR_CPUS == 1
// /*AFLA*/ #define nr_cpu_ids		1
// /*AFLA*/ #else
// /*AFLA*/ extern int nr_cpu_ids;
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_CPUMASK_OFFSTACK
// /*AFLA*/ /* Assuming NR_CPUS is huge, a runtime limit is more efficient.  Also,
// /*AFLA*/  * not all bits may be allocated. */
// /*AFLA*/ #define nr_cpumask_bits	nr_cpu_ids
// /*AFLA*/ #else
// /*AFLA*/ #define nr_cpumask_bits	NR_CPUS
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The following particular system cpumasks and operations manage
// /*AFLA*/  * possible, present, active and online cpus.
// /*AFLA*/  *
// /*AFLA*/  *     cpu_possible_mask- has bit 'cpu' set iff cpu is populatable
// /*AFLA*/  *     cpu_present_mask - has bit 'cpu' set iff cpu is populated
// /*AFLA*/  *     cpu_online_mask  - has bit 'cpu' set iff cpu available to scheduler
// /*AFLA*/  *     cpu_active_mask  - has bit 'cpu' set iff cpu available to migration
// /*AFLA*/  *
// /*AFLA*/  *  If !CONFIG_HOTPLUG_CPU, present == possible, and active == online.
// /*AFLA*/  *
// /*AFLA*/  *  The cpu_possible_mask is fixed at boot time, as the set of CPU id's
// /*AFLA*/  *  that it is possible might ever be plugged in at anytime during the
// /*AFLA*/  *  life of that system boot.  The cpu_present_mask is dynamic(*),
// /*AFLA*/  *  representing which CPUs are currently plugged in.  And
// /*AFLA*/  *  cpu_online_mask is the dynamic subset of cpu_present_mask,
// /*AFLA*/  *  indicating those CPUs available for scheduling.
// /*AFLA*/  *
// /*AFLA*/  *  If HOTPLUG is enabled, then cpu_possible_mask is forced to have
// /*AFLA*/  *  all NR_CPUS bits set, otherwise it is just the set of CPUs that
// /*AFLA*/  *  ACPI reports present at boot.
// /*AFLA*/  *
// /*AFLA*/  *  If HOTPLUG is enabled, then cpu_present_mask varies dynamically,
// /*AFLA*/  *  depending on what ACPI reports as currently plugged in, otherwise
// /*AFLA*/  *  cpu_present_mask is just a copy of cpu_possible_mask.
// /*AFLA*/  *
// /*AFLA*/  *  (*) Well, cpu_present_mask is dynamic in the hotplug case.  If not
// /*AFLA*/  *      hotplug, it's a copy of cpu_possible_mask, hence fixed at boot.
// /*AFLA*/  *
// /*AFLA*/  * Subtleties:
// /*AFLA*/  * 1) UP arch's (NR_CPUS == 1, CONFIG_SMP not defined) hardcode
// /*AFLA*/  *    assumption that their single CPU is online.  The UP
// /*AFLA*/  *    cpu_{online,possible,present}_masks are placebos.  Changing them
// /*AFLA*/  *    will have no useful affect on the following num_*_cpus()
// /*AFLA*/  *    and cpu_*() macros in the UP case.  This ugliness is a UP
// /*AFLA*/  *    optimization - don't waste any instructions or memory references
// /*AFLA*/  *    asking if you're online or how many CPUs there are if there is
// /*AFLA*/  *    only one CPU.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ extern struct cpumask __cpu_possible_mask;
// /*AFLA*/ extern struct cpumask __cpu_online_mask;
// /*AFLA*/ extern struct cpumask __cpu_present_mask;
// /*AFLA*/ extern struct cpumask __cpu_active_mask;
// /*AFLA*/ #define cpu_possible_mask ((const struct cpumask *)&__cpu_possible_mask)
// /*AFLA*/ #define cpu_online_mask   ((const struct cpumask *)&__cpu_online_mask)
// /*AFLA*/ #define cpu_present_mask  ((const struct cpumask *)&__cpu_present_mask)
// /*AFLA*/ #define cpu_active_mask   ((const struct cpumask *)&__cpu_active_mask)
// /*AFLA*/ 
// /*AFLA*/ #if NR_CPUS > 1
// /*AFLA*/ #define num_online_cpus()	cpumask_weight(cpu_online_mask)
// /*AFLA*/ #define num_possible_cpus()	cpumask_weight(cpu_possible_mask)
// /*AFLA*/ #define num_present_cpus()	cpumask_weight(cpu_present_mask)
// /*AFLA*/ #define num_active_cpus()	cpumask_weight(cpu_active_mask)
// /*AFLA*/ #define cpu_online(cpu)		cpumask_test_cpu((cpu), cpu_online_mask)
// /*AFLA*/ #define cpu_possible(cpu)	cpumask_test_cpu((cpu), cpu_possible_mask)
// /*AFLA*/ #define cpu_present(cpu)	cpumask_test_cpu((cpu), cpu_present_mask)
// /*AFLA*/ #define cpu_active(cpu)		cpumask_test_cpu((cpu), cpu_active_mask)
// /*AFLA*/ #else
// /*AFLA*/ #define num_online_cpus()	1U
// /*AFLA*/ #define num_possible_cpus()	1U
// /*AFLA*/ #define num_present_cpus()	1U
// /*AFLA*/ #define num_active_cpus()	1U
// /*AFLA*/ #define cpu_online(cpu)		((cpu) == 0)
// /*AFLA*/ #define cpu_possible(cpu)	((cpu) == 0)
// /*AFLA*/ #define cpu_present(cpu)	((cpu) == 0)
// /*AFLA*/ #define cpu_active(cpu)		((cpu) == 0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* verify cpu argument to cpumask_* operators */
// /*AFLA*/ static inline unsigned int cpumask_check(unsigned int cpu)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_DEBUG_PER_CPU_MAPS
// /*AFLA*/ 	WARN_ON_ONCE(cpu >= nr_cpumask_bits);
// /*AFLA*/ #endif /* CONFIG_DEBUG_PER_CPU_MAPS */
// /*AFLA*/ 	return cpu;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #if NR_CPUS == 1
// /*AFLA*/ /* Uniprocessor.  Assume all masks are "1". */
// /*AFLA*/ static inline unsigned int cpumask_first(const struct cpumask *srcp)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* Valid inputs for n are -1 and 0. */
// /*AFLA*/ static inline unsigned int cpumask_next(int n, const struct cpumask *srcp)
// /*AFLA*/ {
// /*AFLA*/ 	return n+1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned int cpumask_next_zero(int n, const struct cpumask *srcp)
// /*AFLA*/ {
// /*AFLA*/ 	return n+1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned int cpumask_next_and(int n,
// /*AFLA*/ 					    const struct cpumask *srcp,
// /*AFLA*/ 					    const struct cpumask *andp)
// /*AFLA*/ {
// /*AFLA*/ 	return n+1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* cpu must be a valid cpu, ie 0, so there's no other choice. */
// /*AFLA*/ static inline unsigned int cpumask_any_but(const struct cpumask *mask,
// /*AFLA*/ 					   unsigned int cpu)
// /*AFLA*/ {
// /*AFLA*/ 	return 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned int cpumask_local_spread(unsigned int i, int node)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define for_each_cpu(cpu, mask)			\
// /*AFLA*/ 	for ((cpu) = 0; (cpu) < 1; (cpu)++, (void)mask)
// /*AFLA*/ #define for_each_cpu_not(cpu, mask)		\
// /*AFLA*/ 	for ((cpu) = 0; (cpu) < 1; (cpu)++, (void)mask)
// /*AFLA*/ #define for_each_cpu_and(cpu, mask, and)	\
// /*AFLA*/ 	for ((cpu) = 0; (cpu) < 1; (cpu)++, (void)mask, (void)and)
// /*AFLA*/ #else
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_first - get the first cpu in a cpumask
// /*AFLA*/  * @srcp: the cpumask pointer
// /*AFLA*/  *
// /*AFLA*/  * Returns >= nr_cpu_ids if no cpus set.
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned int cpumask_first(const struct cpumask *srcp)
// /*AFLA*/ {
// /*AFLA*/ 	return find_first_bit(cpumask_bits(srcp), nr_cpumask_bits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_next - get the next cpu in a cpumask
// /*AFLA*/  * @n: the cpu prior to the place to search (ie. return will be > @n)
// /*AFLA*/  * @srcp: the cpumask pointer
// /*AFLA*/  *
// /*AFLA*/  * Returns >= nr_cpu_ids if no further cpus set.
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned int cpumask_next(int n, const struct cpumask *srcp)
// /*AFLA*/ {
// /*AFLA*/ 	/* -1 is a legal arg here. */
// /*AFLA*/ 	if (n != -1)
// /*AFLA*/ 		cpumask_check(n);
// /*AFLA*/ 	return find_next_bit(cpumask_bits(srcp), nr_cpumask_bits, n+1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_next_zero - get the next unset cpu in a cpumask
// /*AFLA*/  * @n: the cpu prior to the place to search (ie. return will be > @n)
// /*AFLA*/  * @srcp: the cpumask pointer
// /*AFLA*/  *
// /*AFLA*/  * Returns >= nr_cpu_ids if no further cpus unset.
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned int cpumask_next_zero(int n, const struct cpumask *srcp)
// /*AFLA*/ {
// /*AFLA*/ 	/* -1 is a legal arg here. */
// /*AFLA*/ 	if (n != -1)
// /*AFLA*/ 		cpumask_check(n);
// /*AFLA*/ 	return find_next_zero_bit(cpumask_bits(srcp), nr_cpumask_bits, n+1);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ int cpumask_next_and(int n, const struct cpumask *, const struct cpumask *);
// /*AFLA*/ int cpumask_any_but(const struct cpumask *mask, unsigned int cpu);
// /*AFLA*/ unsigned int cpumask_local_spread(unsigned int i, int node);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * for_each_cpu - iterate over every cpu in a mask
// /*AFLA*/  * @cpu: the (optionally unsigned) integer iterator
// /*AFLA*/  * @mask: the cpumask pointer
// /*AFLA*/  *
// /*AFLA*/  * After the loop, cpu is >= nr_cpu_ids.
// /*AFLA*/  */
// /*AFLA*/ #define for_each_cpu(cpu, mask)				\
// /*AFLA*/ 	for ((cpu) = -1;				\
// /*AFLA*/ 		(cpu) = cpumask_next((cpu), (mask)),	\
// /*AFLA*/ 		(cpu) < nr_cpu_ids;)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * for_each_cpu_not - iterate over every cpu in a complemented mask
// /*AFLA*/  * @cpu: the (optionally unsigned) integer iterator
// /*AFLA*/  * @mask: the cpumask pointer
// /*AFLA*/  *
// /*AFLA*/  * After the loop, cpu is >= nr_cpu_ids.
// /*AFLA*/  */
// /*AFLA*/ #define for_each_cpu_not(cpu, mask)				\
// /*AFLA*/ 	for ((cpu) = -1;					\
// /*AFLA*/ 		(cpu) = cpumask_next_zero((cpu), (mask)),	\
// /*AFLA*/ 		(cpu) < nr_cpu_ids;)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * for_each_cpu_and - iterate over every cpu in both masks
// /*AFLA*/  * @cpu: the (optionally unsigned) integer iterator
// /*AFLA*/  * @mask: the first cpumask pointer
// /*AFLA*/  * @and: the second cpumask pointer
// /*AFLA*/  *
// /*AFLA*/  * This saves a temporary CPU mask in many places.  It is equivalent to:
// /*AFLA*/  *	struct cpumask tmp;
// /*AFLA*/  *	cpumask_and(&tmp, &mask, &and);
// /*AFLA*/  *	for_each_cpu(cpu, &tmp)
// /*AFLA*/  *		...
// /*AFLA*/  *
// /*AFLA*/  * After the loop, cpu is >= nr_cpu_ids.
// /*AFLA*/  */
// /*AFLA*/ #define for_each_cpu_and(cpu, mask, and)				\
// /*AFLA*/ 	for ((cpu) = -1;						\
// /*AFLA*/ 		(cpu) = cpumask_next_and((cpu), (mask), (and)),		\
// /*AFLA*/ 		(cpu) < nr_cpu_ids;)
// /*AFLA*/ #endif /* SMP */
// /*AFLA*/ 
// /*AFLA*/ #define CPU_BITS_NONE						\
// /*AFLA*/ {								\
// /*AFLA*/ 	[0 ... BITS_TO_LONGS(NR_CPUS)-1] = 0UL			\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define CPU_BITS_CPU0						\
// /*AFLA*/ {								\
// /*AFLA*/ 	[0] =  1UL						\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_set_cpu - set a cpu in a cpumask
// /*AFLA*/  * @cpu: cpu number (< nr_cpu_ids)
// /*AFLA*/  * @dstp: the cpumask pointer
// /*AFLA*/  */
// /*AFLA*/ static inline void cpumask_set_cpu(unsigned int cpu, struct cpumask *dstp)
// /*AFLA*/ {
// /*AFLA*/ 	set_bit(cpumask_check(cpu), cpumask_bits(dstp));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_clear_cpu - clear a cpu in a cpumask
// /*AFLA*/  * @cpu: cpu number (< nr_cpu_ids)
// /*AFLA*/  * @dstp: the cpumask pointer
// /*AFLA*/  */
// /*AFLA*/ static inline void cpumask_clear_cpu(int cpu, struct cpumask *dstp)
// /*AFLA*/ {
// /*AFLA*/ 	clear_bit(cpumask_check(cpu), cpumask_bits(dstp));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_test_cpu - test for a cpu in a cpumask
// /*AFLA*/  * @cpu: cpu number (< nr_cpu_ids)
// /*AFLA*/  * @cpumask: the cpumask pointer
// /*AFLA*/  *
// /*AFLA*/  * Returns 1 if @cpu is set in @cpumask, else returns 0
// /*AFLA*/  */
// /*AFLA*/ static inline int cpumask_test_cpu(int cpu, const struct cpumask *cpumask)
// /*AFLA*/ {
// /*AFLA*/ 	return test_bit(cpumask_check(cpu), cpumask_bits((cpumask)));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_test_and_set_cpu - atomically test and set a cpu in a cpumask
// /*AFLA*/  * @cpu: cpu number (< nr_cpu_ids)
// /*AFLA*/  * @cpumask: the cpumask pointer
// /*AFLA*/  *
// /*AFLA*/  * Returns 1 if @cpu is set in old bitmap of @cpumask, else returns 0
// /*AFLA*/  *
// /*AFLA*/  * test_and_set_bit wrapper for cpumasks.
// /*AFLA*/  */
// /*AFLA*/ static inline int cpumask_test_and_set_cpu(int cpu, struct cpumask *cpumask)
// /*AFLA*/ {
// /*AFLA*/ 	return test_and_set_bit(cpumask_check(cpu), cpumask_bits(cpumask));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_test_and_clear_cpu - atomically test and clear a cpu in a cpumask
// /*AFLA*/  * @cpu: cpu number (< nr_cpu_ids)
// /*AFLA*/  * @cpumask: the cpumask pointer
// /*AFLA*/  *
// /*AFLA*/  * Returns 1 if @cpu is set in old bitmap of @cpumask, else returns 0
// /*AFLA*/  *
// /*AFLA*/  * test_and_clear_bit wrapper for cpumasks.
// /*AFLA*/  */
// /*AFLA*/ static inline int cpumask_test_and_clear_cpu(int cpu, struct cpumask *cpumask)
// /*AFLA*/ {
// /*AFLA*/ 	return test_and_clear_bit(cpumask_check(cpu), cpumask_bits(cpumask));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_setall - set all cpus (< nr_cpu_ids) in a cpumask
// /*AFLA*/  * @dstp: the cpumask pointer
// /*AFLA*/  */
// /*AFLA*/ static inline void cpumask_setall(struct cpumask *dstp)
// /*AFLA*/ {
// /*AFLA*/ 	bitmap_fill(cpumask_bits(dstp), nr_cpumask_bits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_clear - clear all cpus (< nr_cpu_ids) in a cpumask
// /*AFLA*/  * @dstp: the cpumask pointer
// /*AFLA*/  */
// /*AFLA*/ static inline void cpumask_clear(struct cpumask *dstp)
// /*AFLA*/ {
// /*AFLA*/ 	bitmap_zero(cpumask_bits(dstp), nr_cpumask_bits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_and - *dstp = *src1p & *src2p
// /*AFLA*/  * @dstp: the cpumask result
// /*AFLA*/  * @src1p: the first input
// /*AFLA*/  * @src2p: the second input
// /*AFLA*/  *
// /*AFLA*/  * If *@dstp is empty, returns 0, else returns 1
// /*AFLA*/  */
// /*AFLA*/ static inline int cpumask_and(struct cpumask *dstp,
// /*AFLA*/ 			       const struct cpumask *src1p,
// /*AFLA*/ 			       const struct cpumask *src2p)
// /*AFLA*/ {
// /*AFLA*/ 	return bitmap_and(cpumask_bits(dstp), cpumask_bits(src1p),
// /*AFLA*/ 				       cpumask_bits(src2p), nr_cpumask_bits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_or - *dstp = *src1p | *src2p
// /*AFLA*/  * @dstp: the cpumask result
// /*AFLA*/  * @src1p: the first input
// /*AFLA*/  * @src2p: the second input
// /*AFLA*/  */
// /*AFLA*/ static inline void cpumask_or(struct cpumask *dstp, const struct cpumask *src1p,
// /*AFLA*/ 			      const struct cpumask *src2p)
// /*AFLA*/ {
// /*AFLA*/ 	bitmap_or(cpumask_bits(dstp), cpumask_bits(src1p),
// /*AFLA*/ 				      cpumask_bits(src2p), nr_cpumask_bits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_xor - *dstp = *src1p ^ *src2p
// /*AFLA*/  * @dstp: the cpumask result
// /*AFLA*/  * @src1p: the first input
// /*AFLA*/  * @src2p: the second input
// /*AFLA*/  */
// /*AFLA*/ static inline void cpumask_xor(struct cpumask *dstp,
// /*AFLA*/ 			       const struct cpumask *src1p,
// /*AFLA*/ 			       const struct cpumask *src2p)
// /*AFLA*/ {
// /*AFLA*/ 	bitmap_xor(cpumask_bits(dstp), cpumask_bits(src1p),
// /*AFLA*/ 				       cpumask_bits(src2p), nr_cpumask_bits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_andnot - *dstp = *src1p & ~*src2p
// /*AFLA*/  * @dstp: the cpumask result
// /*AFLA*/  * @src1p: the first input
// /*AFLA*/  * @src2p: the second input
// /*AFLA*/  *
// /*AFLA*/  * If *@dstp is empty, returns 0, else returns 1
// /*AFLA*/  */
// /*AFLA*/ static inline int cpumask_andnot(struct cpumask *dstp,
// /*AFLA*/ 				  const struct cpumask *src1p,
// /*AFLA*/ 				  const struct cpumask *src2p)
// /*AFLA*/ {
// /*AFLA*/ 	return bitmap_andnot(cpumask_bits(dstp), cpumask_bits(src1p),
// /*AFLA*/ 					  cpumask_bits(src2p), nr_cpumask_bits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_complement - *dstp = ~*srcp
// /*AFLA*/  * @dstp: the cpumask result
// /*AFLA*/  * @srcp: the input to invert
// /*AFLA*/  */
// /*AFLA*/ static inline void cpumask_complement(struct cpumask *dstp,
// /*AFLA*/ 				      const struct cpumask *srcp)
// /*AFLA*/ {
// /*AFLA*/ 	bitmap_complement(cpumask_bits(dstp), cpumask_bits(srcp),
// /*AFLA*/ 					      nr_cpumask_bits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_equal - *src1p == *src2p
// /*AFLA*/  * @src1p: the first input
// /*AFLA*/  * @src2p: the second input
// /*AFLA*/  */
// /*AFLA*/ static inline bool cpumask_equal(const struct cpumask *src1p,
// /*AFLA*/ 				const struct cpumask *src2p)
// /*AFLA*/ {
// /*AFLA*/ 	return bitmap_equal(cpumask_bits(src1p), cpumask_bits(src2p),
// /*AFLA*/ 						 nr_cpumask_bits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_intersects - (*src1p & *src2p) != 0
// /*AFLA*/  * @src1p: the first input
// /*AFLA*/  * @src2p: the second input
// /*AFLA*/  */
// /*AFLA*/ static inline bool cpumask_intersects(const struct cpumask *src1p,
// /*AFLA*/ 				     const struct cpumask *src2p)
// /*AFLA*/ {
// /*AFLA*/ 	return bitmap_intersects(cpumask_bits(src1p), cpumask_bits(src2p),
// /*AFLA*/ 						      nr_cpumask_bits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_subset - (*src1p & ~*src2p) == 0
// /*AFLA*/  * @src1p: the first input
// /*AFLA*/  * @src2p: the second input
// /*AFLA*/  *
// /*AFLA*/  * Returns 1 if *@src1p is a subset of *@src2p, else returns 0
// /*AFLA*/  */
// /*AFLA*/ static inline int cpumask_subset(const struct cpumask *src1p,
// /*AFLA*/ 				 const struct cpumask *src2p)
// /*AFLA*/ {
// /*AFLA*/ 	return bitmap_subset(cpumask_bits(src1p), cpumask_bits(src2p),
// /*AFLA*/ 						  nr_cpumask_bits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_empty - *srcp == 0
// /*AFLA*/  * @srcp: the cpumask to that all cpus < nr_cpu_ids are clear.
// /*AFLA*/  */
// /*AFLA*/ static inline bool cpumask_empty(const struct cpumask *srcp)
// /*AFLA*/ {
// /*AFLA*/ 	return bitmap_empty(cpumask_bits(srcp), nr_cpumask_bits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_full - *srcp == 0xFFFFFFFF...
// /*AFLA*/  * @srcp: the cpumask to that all cpus < nr_cpu_ids are set.
// /*AFLA*/  */
// /*AFLA*/ static inline bool cpumask_full(const struct cpumask *srcp)
// /*AFLA*/ {
// /*AFLA*/ 	return bitmap_full(cpumask_bits(srcp), nr_cpumask_bits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_weight - Count of bits in *srcp
// /*AFLA*/  * @srcp: the cpumask to count bits (< nr_cpu_ids) in.
// /*AFLA*/  */
// /*AFLA*/ static inline unsigned int cpumask_weight(const struct cpumask *srcp)
// /*AFLA*/ {
// /*AFLA*/ 	return bitmap_weight(cpumask_bits(srcp), nr_cpumask_bits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_shift_right - *dstp = *srcp >> n
// /*AFLA*/  * @dstp: the cpumask result
// /*AFLA*/  * @srcp: the input to shift
// /*AFLA*/  * @n: the number of bits to shift by
// /*AFLA*/  */
// /*AFLA*/ static inline void cpumask_shift_right(struct cpumask *dstp,
// /*AFLA*/ 				       const struct cpumask *srcp, int n)
// /*AFLA*/ {
// /*AFLA*/ 	bitmap_shift_right(cpumask_bits(dstp), cpumask_bits(srcp), n,
// /*AFLA*/ 					       nr_cpumask_bits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_shift_left - *dstp = *srcp << n
// /*AFLA*/  * @dstp: the cpumask result
// /*AFLA*/  * @srcp: the input to shift
// /*AFLA*/  * @n: the number of bits to shift by
// /*AFLA*/  */
// /*AFLA*/ static inline void cpumask_shift_left(struct cpumask *dstp,
// /*AFLA*/ 				      const struct cpumask *srcp, int n)
// /*AFLA*/ {
// /*AFLA*/ 	bitmap_shift_left(cpumask_bits(dstp), cpumask_bits(srcp), n,
// /*AFLA*/ 					      nr_cpumask_bits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_copy - *dstp = *srcp
// /*AFLA*/  * @dstp: the result
// /*AFLA*/  * @srcp: the input cpumask
// /*AFLA*/  */
// /*AFLA*/ static inline void cpumask_copy(struct cpumask *dstp,
// /*AFLA*/ 				const struct cpumask *srcp)
// /*AFLA*/ {
// /*AFLA*/ 	bitmap_copy(cpumask_bits(dstp), cpumask_bits(srcp), nr_cpumask_bits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_any - pick a "random" cpu from *srcp
// /*AFLA*/  * @srcp: the input cpumask
// /*AFLA*/  *
// /*AFLA*/  * Returns >= nr_cpu_ids if no cpus set.
// /*AFLA*/  */
// /*AFLA*/ #define cpumask_any(srcp) cpumask_first(srcp)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_first_and - return the first cpu from *srcp1 & *srcp2
// /*AFLA*/  * @src1p: the first input
// /*AFLA*/  * @src2p: the second input
// /*AFLA*/  *
// /*AFLA*/  * Returns >= nr_cpu_ids if no cpus set in both.  See also cpumask_next_and().
// /*AFLA*/  */
// /*AFLA*/ #define cpumask_first_and(src1p, src2p) cpumask_next_and(-1, (src1p), (src2p))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_any_and - pick a "random" cpu from *mask1 & *mask2
// /*AFLA*/  * @mask1: the first input cpumask
// /*AFLA*/  * @mask2: the second input cpumask
// /*AFLA*/  *
// /*AFLA*/  * Returns >= nr_cpu_ids if no cpus set.
// /*AFLA*/  */
// /*AFLA*/ #define cpumask_any_and(mask1, mask2) cpumask_first_and((mask1), (mask2))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_of - the cpumask containing just a given cpu
// /*AFLA*/  * @cpu: the cpu (<= nr_cpu_ids)
// /*AFLA*/  */
// /*AFLA*/ #define cpumask_of(cpu) (get_cpu_mask(cpu))
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_parse_user - extract a cpumask from a user string
// /*AFLA*/  * @buf: the buffer to extract from
// /*AFLA*/  * @len: the length of the buffer
// /*AFLA*/  * @dstp: the cpumask to set.
// /*AFLA*/  *
// /*AFLA*/  * Returns -errno, or 0 for success.
// /*AFLA*/  */
// /*AFLA*/ static inline int cpumask_parse_user(const char __user *buf, int len,
// /*AFLA*/ 				     struct cpumask *dstp)
// /*AFLA*/ {
// /*AFLA*/ 	return bitmap_parse_user(buf, len, cpumask_bits(dstp), nr_cpu_ids);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_parselist_user - extract a cpumask from a user string
// /*AFLA*/  * @buf: the buffer to extract from
// /*AFLA*/  * @len: the length of the buffer
// /*AFLA*/  * @dstp: the cpumask to set.
// /*AFLA*/  *
// /*AFLA*/  * Returns -errno, or 0 for success.
// /*AFLA*/  */
// /*AFLA*/ static inline int cpumask_parselist_user(const char __user *buf, int len,
// /*AFLA*/ 				     struct cpumask *dstp)
// /*AFLA*/ {
// /*AFLA*/ 	return bitmap_parselist_user(buf, len, cpumask_bits(dstp),
// /*AFLA*/ 				     nr_cpu_ids);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_parse - extract a cpumask from a string
// /*AFLA*/  * @buf: the buffer to extract from
// /*AFLA*/  * @dstp: the cpumask to set.
// /*AFLA*/  *
// /*AFLA*/  * Returns -errno, or 0 for success.
// /*AFLA*/  */
// /*AFLA*/ static inline int cpumask_parse(const char *buf, struct cpumask *dstp)
// /*AFLA*/ {
// /*AFLA*/ 	char *nl = strchr(buf, '\n');
// /*AFLA*/ 	unsigned int len = nl ? (unsigned int)(nl - buf) : strlen(buf);
// /*AFLA*/ 
// /*AFLA*/ 	return bitmap_parse(buf, len, cpumask_bits(dstp), nr_cpu_ids);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpulist_parse - extract a cpumask from a user string of ranges
// /*AFLA*/  * @buf: the buffer to extract from
// /*AFLA*/  * @dstp: the cpumask to set.
// /*AFLA*/  *
// /*AFLA*/  * Returns -errno, or 0 for success.
// /*AFLA*/  */
// /*AFLA*/ static inline int cpulist_parse(const char *buf, struct cpumask *dstp)
// /*AFLA*/ {
// /*AFLA*/ 	return bitmap_parselist(buf, cpumask_bits(dstp), nr_cpu_ids);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumask_size - size to allocate for a 'struct cpumask' in bytes
// /*AFLA*/  */
// /*AFLA*/ static inline size_t cpumask_size(void)
// /*AFLA*/ {
// /*AFLA*/ 	return BITS_TO_LONGS(nr_cpumask_bits) * sizeof(long);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * cpumask_var_t: struct cpumask for stack usage.
// /*AFLA*/  *
// /*AFLA*/  * Oh, the wicked games we play!  In order to make kernel coding a
// /*AFLA*/  * little more difficult, we typedef cpumask_var_t to an array or a
// /*AFLA*/  * pointer: doing &mask on an array is a noop, so it still works.
// /*AFLA*/  *
// /*AFLA*/  * ie.
// /*AFLA*/  *	cpumask_var_t tmpmask;
// /*AFLA*/  *	if (!alloc_cpumask_var(&tmpmask, GFP_KERNEL))
// /*AFLA*/  *		return -ENOMEM;
// /*AFLA*/  *
// /*AFLA*/  *	  ... use 'tmpmask' like a normal struct cpumask * ...
// /*AFLA*/  *
// /*AFLA*/  *	free_cpumask_var(tmpmask);
// /*AFLA*/  *
// /*AFLA*/  *
// /*AFLA*/  * However, one notable exception is there. alloc_cpumask_var() allocates
// /*AFLA*/  * only nr_cpumask_bits bits (in the other hand, real cpumask_t always has
// /*AFLA*/  * NR_CPUS bits). Therefore you don't have to dereference cpumask_var_t.
// /*AFLA*/  *
// /*AFLA*/  *	cpumask_var_t tmpmask;
// /*AFLA*/  *	if (!alloc_cpumask_var(&tmpmask, GFP_KERNEL))
// /*AFLA*/  *		return -ENOMEM;
// /*AFLA*/  *
// /*AFLA*/  *	var = *tmpmask;
// /*AFLA*/  *
// /*AFLA*/  * This code makes NR_CPUS length memcopy and brings to a memory corruption.
// /*AFLA*/  * cpumask_copy() provide safe copy functionality.
// /*AFLA*/  *
// /*AFLA*/  * Note that there is another evil here: If you define a cpumask_var_t
// /*AFLA*/  * as a percpu variable then the way to obtain the address of the cpumask
// /*AFLA*/  * structure differently influences what this_cpu_* operation needs to be
// /*AFLA*/  * used. Please use this_cpu_cpumask_var_t in those cases. The direct use
// /*AFLA*/  * of this_cpu_ptr() or this_cpu_read() will lead to failures when the
// /*AFLA*/  * other type of cpumask_var_t implementation is configured.
// /*AFLA*/  */
// /*AFLA*/ #ifdef CONFIG_CPUMASK_OFFSTACK
// /*AFLA*/ typedef struct cpumask *cpumask_var_t;
// /*AFLA*/ 
// /*AFLA*/ #define this_cpu_cpumask_var_ptr(x) this_cpu_read(x)
// /*AFLA*/ 
// /*AFLA*/ bool alloc_cpumask_var_node(cpumask_var_t *mask, gfp_t flags, int node);
// /*AFLA*/ bool alloc_cpumask_var(cpumask_var_t *mask, gfp_t flags);
// /*AFLA*/ bool zalloc_cpumask_var_node(cpumask_var_t *mask, gfp_t flags, int node);
// /*AFLA*/ bool zalloc_cpumask_var(cpumask_var_t *mask, gfp_t flags);
// /*AFLA*/ void alloc_bootmem_cpumask_var(cpumask_var_t *mask);
// /*AFLA*/ void free_cpumask_var(cpumask_var_t mask);
// /*AFLA*/ void free_bootmem_cpumask_var(cpumask_var_t mask);
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ typedef struct cpumask cpumask_var_t[1];
// /*AFLA*/ 
// /*AFLA*/ #define this_cpu_cpumask_var_ptr(x) this_cpu_ptr(x)
// /*AFLA*/ 
// /*AFLA*/ static inline bool alloc_cpumask_var(cpumask_var_t *mask, gfp_t flags)
// /*AFLA*/ {
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool alloc_cpumask_var_node(cpumask_var_t *mask, gfp_t flags,
// /*AFLA*/ 					  int node)
// /*AFLA*/ {
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool zalloc_cpumask_var(cpumask_var_t *mask, gfp_t flags)
// /*AFLA*/ {
// /*AFLA*/ 	cpumask_clear(*mask);
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool zalloc_cpumask_var_node(cpumask_var_t *mask, gfp_t flags,
// /*AFLA*/ 					  int node)
// /*AFLA*/ {
// /*AFLA*/ 	cpumask_clear(*mask);
// /*AFLA*/ 	return true;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void alloc_bootmem_cpumask_var(cpumask_var_t *mask)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void free_cpumask_var(cpumask_var_t mask)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void free_bootmem_cpumask_var(cpumask_var_t mask)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_CPUMASK_OFFSTACK */
// /*AFLA*/ 
// /*AFLA*/ /* It's common to want to use cpu_all_mask in struct member initializers,
// /*AFLA*/  * so it has to refer to an address rather than a pointer. */
// /*AFLA*/ extern const DECLARE_BITMAP(cpu_all_bits, NR_CPUS);
// /*AFLA*/ #define cpu_all_mask to_cpumask(cpu_all_bits)
// /*AFLA*/ 
// /*AFLA*/ /* First bits of cpu_bit_bitmap are in fact unset. */
// /*AFLA*/ #define cpu_none_mask to_cpumask(cpu_bit_bitmap[0])
// /*AFLA*/ 
// /*AFLA*/ #define for_each_possible_cpu(cpu) for_each_cpu((cpu), cpu_possible_mask)
// /*AFLA*/ #define for_each_online_cpu(cpu)   for_each_cpu((cpu), cpu_online_mask)
// /*AFLA*/ #define for_each_present_cpu(cpu)  for_each_cpu((cpu), cpu_present_mask)
// /*AFLA*/ 
// /*AFLA*/ /* Wrappers for arch boot code to manipulate normally-constant masks */
// /*AFLA*/ void init_cpu_present(const struct cpumask *src);
// /*AFLA*/ void init_cpu_possible(const struct cpumask *src);
// /*AFLA*/ void init_cpu_online(const struct cpumask *src);
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ set_cpu_possible(unsigned int cpu, bool possible)
// /*AFLA*/ {
// /*AFLA*/ 	if (possible)
// /*AFLA*/ 		cpumask_set_cpu(cpu, &__cpu_possible_mask);
// /*AFLA*/ 	else
// /*AFLA*/ 		cpumask_clear_cpu(cpu, &__cpu_possible_mask);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ set_cpu_present(unsigned int cpu, bool present)
// /*AFLA*/ {
// /*AFLA*/ 	if (present)
// /*AFLA*/ 		cpumask_set_cpu(cpu, &__cpu_present_mask);
// /*AFLA*/ 	else
// /*AFLA*/ 		cpumask_clear_cpu(cpu, &__cpu_present_mask);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ set_cpu_online(unsigned int cpu, bool online)
// /*AFLA*/ {
// /*AFLA*/ 	if (online)
// /*AFLA*/ 		cpumask_set_cpu(cpu, &__cpu_online_mask);
// /*AFLA*/ 	else
// /*AFLA*/ 		cpumask_clear_cpu(cpu, &__cpu_online_mask);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void
// /*AFLA*/ set_cpu_active(unsigned int cpu, bool active)
// /*AFLA*/ {
// /*AFLA*/ 	if (active)
// /*AFLA*/ 		cpumask_set_cpu(cpu, &__cpu_active_mask);
// /*AFLA*/ 	else
// /*AFLA*/ 		cpumask_clear_cpu(cpu, &__cpu_active_mask);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * to_cpumask - convert an NR_CPUS bitmap to a struct cpumask *
// /*AFLA*/  * @bitmap: the bitmap
// /*AFLA*/  *
// /*AFLA*/  * There are a few places where cpumask_var_t isn't appropriate and
// /*AFLA*/  * static cpumasks must be used (eg. very early boot), yet we don't
// /*AFLA*/  * expose the definition of 'struct cpumask'.
// /*AFLA*/  *
// /*AFLA*/  * This does the conversion, and can be used as a constant initializer.
// /*AFLA*/  */
// /*AFLA*/ #define to_cpumask(bitmap)						\
// /*AFLA*/ 	((struct cpumask *)(1 ? (bitmap)				\
// /*AFLA*/ 			    : (void *)sizeof(__check_is_bitmap(bitmap))))
// /*AFLA*/ 
// /*AFLA*/ static inline int __check_is_bitmap(const unsigned long *bitmap)
// /*AFLA*/ {
// /*AFLA*/ 	return 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Special-case data structure for "single bit set only" constant CPU masks.
// /*AFLA*/  *
// /*AFLA*/  * We pre-generate all the 64 (or 32) possible bit positions, with enough
// /*AFLA*/  * padding to the left and the right, and return the constant pointer
// /*AFLA*/  * appropriately offset.
// /*AFLA*/  */
// /*AFLA*/ extern const unsigned long
// /*AFLA*/ 	cpu_bit_bitmap[BITS_PER_LONG+1][BITS_TO_LONGS(NR_CPUS)];
// /*AFLA*/ 
// /*AFLA*/ static inline const struct cpumask *get_cpu_mask(unsigned int cpu)
// /*AFLA*/ {
// /*AFLA*/ 	const unsigned long *p = cpu_bit_bitmap[1 + cpu % BITS_PER_LONG];
// /*AFLA*/ 	p -= cpu / BITS_PER_LONG;
// /*AFLA*/ 	return to_cpumask(p);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define cpu_is_offline(cpu)	unlikely(!cpu_online(cpu))
// /*AFLA*/ 
// /*AFLA*/ #if NR_CPUS <= BITS_PER_LONG
// /*AFLA*/ #define CPU_BITS_ALL						\
// /*AFLA*/ {								\
// /*AFLA*/ 	[BITS_TO_LONGS(NR_CPUS)-1] = BITMAP_LAST_WORD_MASK(NR_CPUS)	\
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else /* NR_CPUS > BITS_PER_LONG */
// /*AFLA*/ 
// /*AFLA*/ #define CPU_BITS_ALL						\
// /*AFLA*/ {								\
// /*AFLA*/ 	[0 ... BITS_TO_LONGS(NR_CPUS)-2] = ~0UL,		\
// /*AFLA*/ 	[BITS_TO_LONGS(NR_CPUS)-1] = BITMAP_LAST_WORD_MASK(NR_CPUS)	\
// /*AFLA*/ }
// /*AFLA*/ #endif /* NR_CPUS > BITS_PER_LONG */
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * cpumap_print_to_pagebuf  - copies the cpumask into the buffer either
// /*AFLA*/  *	as comma-separated list of cpus or hex values of cpumask
// /*AFLA*/  * @list: indicates whether the cpumap must be list
// /*AFLA*/  * @mask: the cpumask to copy
// /*AFLA*/  * @buf: the buffer to copy into
// /*AFLA*/  *
// /*AFLA*/  * Returns the length of the (null-terminated) @buf string, zero if
// /*AFLA*/  * nothing is copied.
// /*AFLA*/  */
// /*AFLA*/ static inline ssize_t
// /*AFLA*/ cpumap_print_to_pagebuf(bool list, char *buf, const struct cpumask *mask)
// /*AFLA*/ {
// /*AFLA*/ 	return bitmap_print_to_pagebuf(list, buf, cpumask_bits(mask),
// /*AFLA*/ 				      nr_cpu_ids);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #if NR_CPUS <= BITS_PER_LONG
// /*AFLA*/ #define CPU_MASK_ALL							\
// /*AFLA*/ (cpumask_t) { {								\
// /*AFLA*/ 	[BITS_TO_LONGS(NR_CPUS)-1] = BITMAP_LAST_WORD_MASK(NR_CPUS)	\
// /*AFLA*/ } }
// /*AFLA*/ #else
// /*AFLA*/ #define CPU_MASK_ALL							\
// /*AFLA*/ (cpumask_t) { {								\
// /*AFLA*/ 	[0 ... BITS_TO_LONGS(NR_CPUS)-2] = ~0UL,			\
// /*AFLA*/ 	[BITS_TO_LONGS(NR_CPUS)-1] = BITMAP_LAST_WORD_MASK(NR_CPUS)	\
// /*AFLA*/ } }
// /*AFLA*/ #endif /* NR_CPUS > BITS_PER_LONG */
// /*AFLA*/ 
// /*AFLA*/ #define CPU_MASK_NONE							\
// /*AFLA*/ (cpumask_t) { {								\
// /*AFLA*/ 	[0 ... BITS_TO_LONGS(NR_CPUS)-1] =  0UL				\
// /*AFLA*/ } }
// /*AFLA*/ 
// /*AFLA*/ #define CPU_MASK_CPU0							\
// /*AFLA*/ (cpumask_t) { {								\
// /*AFLA*/ 	[0] =  1UL							\
// /*AFLA*/ } }
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_CPUMASK_H */
