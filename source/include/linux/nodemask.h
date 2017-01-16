// /*AFLA*/ #ifndef __LINUX_NODEMASK_H
// /*AFLA*/ #define __LINUX_NODEMASK_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Nodemasks provide a bitmap suitable for representing the
// /*AFLA*/  * set of Node's in a system, one bit position per Node number.
// /*AFLA*/  *
// /*AFLA*/  * See detailed comments in the file linux/bitmap.h describing the
// /*AFLA*/  * data type on which these nodemasks are based.
// /*AFLA*/  *
// /*AFLA*/  * For details of nodemask_parse_user(), see bitmap_parse_user() in
// /*AFLA*/  * lib/bitmap.c.  For details of nodelist_parse(), see bitmap_parselist(),
// /*AFLA*/  * also in bitmap.c.  For details of node_remap(), see bitmap_bitremap in
// /*AFLA*/  * lib/bitmap.c.  For details of nodes_remap(), see bitmap_remap in
// /*AFLA*/  * lib/bitmap.c.  For details of nodes_onto(), see bitmap_onto in
// /*AFLA*/  * lib/bitmap.c.  For details of nodes_fold(), see bitmap_fold in
// /*AFLA*/  * lib/bitmap.c.
// /*AFLA*/  *
// /*AFLA*/  * The available nodemask operations are:
// /*AFLA*/  *
// /*AFLA*/  * void node_set(node, mask)		turn on bit 'node' in mask
// /*AFLA*/  * void node_clear(node, mask)		turn off bit 'node' in mask
// /*AFLA*/  * void nodes_setall(mask)		set all bits
// /*AFLA*/  * void nodes_clear(mask)		clear all bits
// /*AFLA*/  * int node_isset(node, mask)		true iff bit 'node' set in mask
// /*AFLA*/  * int node_test_and_set(node, mask)	test and set bit 'node' in mask
// /*AFLA*/  *
// /*AFLA*/  * void nodes_and(dst, src1, src2)	dst = src1 & src2  [intersection]
// /*AFLA*/  * void nodes_or(dst, src1, src2)	dst = src1 | src2  [union]
// /*AFLA*/  * void nodes_xor(dst, src1, src2)	dst = src1 ^ src2
// /*AFLA*/  * void nodes_andnot(dst, src1, src2)	dst = src1 & ~src2
// /*AFLA*/  * void nodes_complement(dst, src)	dst = ~src
// /*AFLA*/  *
// /*AFLA*/  * int nodes_equal(mask1, mask2)	Does mask1 == mask2?
// /*AFLA*/  * int nodes_intersects(mask1, mask2)	Do mask1 and mask2 intersect?
// /*AFLA*/  * int nodes_subset(mask1, mask2)	Is mask1 a subset of mask2?
// /*AFLA*/  * int nodes_empty(mask)		Is mask empty (no bits sets)?
// /*AFLA*/  * int nodes_full(mask)			Is mask full (all bits sets)?
// /*AFLA*/  * int nodes_weight(mask)		Hamming weight - number of set bits
// /*AFLA*/  *
// /*AFLA*/  * void nodes_shift_right(dst, src, n)	Shift right
// /*AFLA*/  * void nodes_shift_left(dst, src, n)	Shift left
// /*AFLA*/  *
// /*AFLA*/  * int first_node(mask)			Number lowest set bit, or MAX_NUMNODES
// /*AFLA*/  * int next_node(node, mask)		Next node past 'node', or MAX_NUMNODES
// /*AFLA*/  * int next_node_in(node, mask)		Next node past 'node', or wrap to first,
// /*AFLA*/  *					or MAX_NUMNODES
// /*AFLA*/  * int first_unset_node(mask)		First node not set in mask, or 
// /*AFLA*/  *					MAX_NUMNODES
// /*AFLA*/  *
// /*AFLA*/  * nodemask_t nodemask_of_node(node)	Return nodemask with bit 'node' set
// /*AFLA*/  * NODE_MASK_ALL			Initializer - all bits set
// /*AFLA*/  * NODE_MASK_NONE			Initializer - no bits set
// /*AFLA*/  * unsigned long *nodes_addr(mask)	Array of unsigned long's in mask
// /*AFLA*/  *
// /*AFLA*/  * int nodemask_parse_user(ubuf, ulen, mask)	Parse ascii string as nodemask
// /*AFLA*/  * int nodelist_parse(buf, map)		Parse ascii string as nodelist
// /*AFLA*/  * int node_remap(oldbit, old, new)	newbit = map(old, new)(oldbit)
// /*AFLA*/  * void nodes_remap(dst, src, old, new)	*dst = map(old, new)(src)
// /*AFLA*/  * void nodes_onto(dst, orig, relmap)	*dst = orig relative to relmap
// /*AFLA*/  * void nodes_fold(dst, orig, sz)	dst bits = orig bits mod sz
// /*AFLA*/  *
// /*AFLA*/  * for_each_node_mask(node, mask)	for-loop node over mask
// /*AFLA*/  *
// /*AFLA*/  * int num_online_nodes()		Number of online Nodes
// /*AFLA*/  * int num_possible_nodes()		Number of all possible Nodes
// /*AFLA*/  *
// /*AFLA*/  * int node_random(mask)		Random node with set bit in mask
// /*AFLA*/  *
// /*AFLA*/  * int node_online(node)		Is some node online?
// /*AFLA*/  * int node_possible(node)		Is some node possible?
// /*AFLA*/  *
// /*AFLA*/  * node_set_online(node)		set bit 'node' in node_online_map
// /*AFLA*/  * node_set_offline(node)		clear bit 'node' in node_online_map
// /*AFLA*/  *
// /*AFLA*/  * for_each_node(node)			for-loop node over node_possible_map
// /*AFLA*/  * for_each_online_node(node)		for-loop node over node_online_map
// /*AFLA*/  *
// /*AFLA*/  * Subtlety:
// /*AFLA*/  * 1) The 'type-checked' form of node_isset() causes gcc (3.3.2, anyway)
// /*AFLA*/  *    to generate slightly worse code.  So use a simple one-line #define
// /*AFLA*/  *    for node_isset(), instead of wrapping an inline inside a macro, the
// /*AFLA*/  *    way we do the other calls.
// /*AFLA*/  *
// /*AFLA*/  * NODEMASK_SCRATCH
// /*AFLA*/  * When doing above logical AND, OR, XOR, Remap operations the callers tend to
// /*AFLA*/  * need temporary nodemask_t's on the stack. But if NODES_SHIFT is large,
// /*AFLA*/  * nodemask_t's consume too much stack space.  NODEMASK_SCRATCH is a helper
// /*AFLA*/  * for such situations. See below and CPUMASK_ALLOC also.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ #include <linux/threads.h>
// /*AFLA*/ #include <linux/bitmap.h>
// /*AFLA*/ #include <linux/numa.h>
// /*AFLA*/ 
// /*AFLA*/ typedef struct { DECLARE_BITMAP(bits, MAX_NUMNODES); } nodemask_t;
// /*AFLA*/ extern nodemask_t _unused_nodemask_arg_;
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * nodemask_pr_args - printf args to output a nodemask
// /*AFLA*/  * @maskp: nodemask to be printed
// /*AFLA*/  *
// /*AFLA*/  * Can be used to provide arguments for '%*pb[l]' when printing a nodemask.
// /*AFLA*/  */
// /*AFLA*/ #define nodemask_pr_args(maskp)		MAX_NUMNODES, (maskp)->bits
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The inline keyword gives the compiler room to decide to inline, or
// /*AFLA*/  * not inline a function as it sees best.  However, as these functions
// /*AFLA*/  * are called in both __init and non-__init functions, if they are not
// /*AFLA*/  * inlined we will end up with a section mis-match error (of the type of
// /*AFLA*/  * freeable items not being freed).  So we must use __always_inline here
// /*AFLA*/  * to fix the problem.  If other functions in the future also end up in
// /*AFLA*/  * this situation they will also need to be annotated as __always_inline
// /*AFLA*/  */
// /*AFLA*/ #define node_set(node, dst) __node_set((node), &(dst))
// /*AFLA*/ static __always_inline void __node_set(int node, volatile nodemask_t *dstp)
// /*AFLA*/ {
// /*AFLA*/ 	set_bit(node, dstp->bits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define node_clear(node, dst) __node_clear((node), &(dst))
// /*AFLA*/ static inline void __node_clear(int node, volatile nodemask_t *dstp)
// /*AFLA*/ {
// /*AFLA*/ 	clear_bit(node, dstp->bits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define nodes_setall(dst) __nodes_setall(&(dst), MAX_NUMNODES)
// /*AFLA*/ static inline void __nodes_setall(nodemask_t *dstp, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	bitmap_fill(dstp->bits, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define nodes_clear(dst) __nodes_clear(&(dst), MAX_NUMNODES)
// /*AFLA*/ static inline void __nodes_clear(nodemask_t *dstp, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	bitmap_zero(dstp->bits, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* No static inline type checking - see Subtlety (1) above. */
// /*AFLA*/ #define node_isset(node, nodemask) test_bit((node), (nodemask).bits)
// /*AFLA*/ 
// /*AFLA*/ #define node_test_and_set(node, nodemask) \
// /*AFLA*/ 			__node_test_and_set((node), &(nodemask))
// /*AFLA*/ static inline int __node_test_and_set(int node, nodemask_t *addr)
// /*AFLA*/ {
// /*AFLA*/ 	return test_and_set_bit(node, addr->bits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define nodes_and(dst, src1, src2) \
// /*AFLA*/ 			__nodes_and(&(dst), &(src1), &(src2), MAX_NUMNODES)
// /*AFLA*/ static inline void __nodes_and(nodemask_t *dstp, const nodemask_t *src1p,
// /*AFLA*/ 					const nodemask_t *src2p, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	bitmap_and(dstp->bits, src1p->bits, src2p->bits, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define nodes_or(dst, src1, src2) \
// /*AFLA*/ 			__nodes_or(&(dst), &(src1), &(src2), MAX_NUMNODES)
// /*AFLA*/ static inline void __nodes_or(nodemask_t *dstp, const nodemask_t *src1p,
// /*AFLA*/ 					const nodemask_t *src2p, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	bitmap_or(dstp->bits, src1p->bits, src2p->bits, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define nodes_xor(dst, src1, src2) \
// /*AFLA*/ 			__nodes_xor(&(dst), &(src1), &(src2), MAX_NUMNODES)
// /*AFLA*/ static inline void __nodes_xor(nodemask_t *dstp, const nodemask_t *src1p,
// /*AFLA*/ 					const nodemask_t *src2p, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	bitmap_xor(dstp->bits, src1p->bits, src2p->bits, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define nodes_andnot(dst, src1, src2) \
// /*AFLA*/ 			__nodes_andnot(&(dst), &(src1), &(src2), MAX_NUMNODES)
// /*AFLA*/ static inline void __nodes_andnot(nodemask_t *dstp, const nodemask_t *src1p,
// /*AFLA*/ 					const nodemask_t *src2p, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	bitmap_andnot(dstp->bits, src1p->bits, src2p->bits, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define nodes_complement(dst, src) \
// /*AFLA*/ 			__nodes_complement(&(dst), &(src), MAX_NUMNODES)
// /*AFLA*/ static inline void __nodes_complement(nodemask_t *dstp,
// /*AFLA*/ 					const nodemask_t *srcp, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	bitmap_complement(dstp->bits, srcp->bits, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define nodes_equal(src1, src2) \
// /*AFLA*/ 			__nodes_equal(&(src1), &(src2), MAX_NUMNODES)
// /*AFLA*/ static inline int __nodes_equal(const nodemask_t *src1p,
// /*AFLA*/ 					const nodemask_t *src2p, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	return bitmap_equal(src1p->bits, src2p->bits, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define nodes_intersects(src1, src2) \
// /*AFLA*/ 			__nodes_intersects(&(src1), &(src2), MAX_NUMNODES)
// /*AFLA*/ static inline int __nodes_intersects(const nodemask_t *src1p,
// /*AFLA*/ 					const nodemask_t *src2p, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	return bitmap_intersects(src1p->bits, src2p->bits, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define nodes_subset(src1, src2) \
// /*AFLA*/ 			__nodes_subset(&(src1), &(src2), MAX_NUMNODES)
// /*AFLA*/ static inline int __nodes_subset(const nodemask_t *src1p,
// /*AFLA*/ 					const nodemask_t *src2p, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	return bitmap_subset(src1p->bits, src2p->bits, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define nodes_empty(src) __nodes_empty(&(src), MAX_NUMNODES)
// /*AFLA*/ static inline int __nodes_empty(const nodemask_t *srcp, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	return bitmap_empty(srcp->bits, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define nodes_full(nodemask) __nodes_full(&(nodemask), MAX_NUMNODES)
// /*AFLA*/ static inline int __nodes_full(const nodemask_t *srcp, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	return bitmap_full(srcp->bits, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define nodes_weight(nodemask) __nodes_weight(&(nodemask), MAX_NUMNODES)
// /*AFLA*/ static inline int __nodes_weight(const nodemask_t *srcp, unsigned int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	return bitmap_weight(srcp->bits, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define nodes_shift_right(dst, src, n) \
// /*AFLA*/ 			__nodes_shift_right(&(dst), &(src), (n), MAX_NUMNODES)
// /*AFLA*/ static inline void __nodes_shift_right(nodemask_t *dstp,
// /*AFLA*/ 					const nodemask_t *srcp, int n, int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	bitmap_shift_right(dstp->bits, srcp->bits, n, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define nodes_shift_left(dst, src, n) \
// /*AFLA*/ 			__nodes_shift_left(&(dst), &(src), (n), MAX_NUMNODES)
// /*AFLA*/ static inline void __nodes_shift_left(nodemask_t *dstp,
// /*AFLA*/ 					const nodemask_t *srcp, int n, int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	bitmap_shift_left(dstp->bits, srcp->bits, n, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* FIXME: better would be to fix all architectures to never return
// /*AFLA*/           > MAX_NUMNODES, then the silly min_ts could be dropped. */
// /*AFLA*/ 
// /*AFLA*/ #define first_node(src) __first_node(&(src))
// /*AFLA*/ static inline int __first_node(const nodemask_t *srcp)
// /*AFLA*/ {
// /*AFLA*/ 	return min_t(int, MAX_NUMNODES, find_first_bit(srcp->bits, MAX_NUMNODES));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define next_node(n, src) __next_node((n), &(src))
// /*AFLA*/ static inline int __next_node(int n, const nodemask_t *srcp)
// /*AFLA*/ {
// /*AFLA*/ 	return min_t(int,MAX_NUMNODES,find_next_bit(srcp->bits, MAX_NUMNODES, n+1));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Find the next present node in src, starting after node n, wrapping around to
// /*AFLA*/  * the first node in src if needed.  Returns MAX_NUMNODES if src is empty.
// /*AFLA*/  */
// /*AFLA*/ #define next_node_in(n, src) __next_node_in((n), &(src))
// /*AFLA*/ int __next_node_in(int node, const nodemask_t *srcp);
// /*AFLA*/ 
// /*AFLA*/ static inline void init_nodemask_of_node(nodemask_t *mask, int node)
// /*AFLA*/ {
// /*AFLA*/ 	nodes_clear(*mask);
// /*AFLA*/ 	node_set(node, *mask);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define nodemask_of_node(node)						\
// /*AFLA*/ ({									\
// /*AFLA*/ 	typeof(_unused_nodemask_arg_) m;				\
// /*AFLA*/ 	if (sizeof(m) == sizeof(unsigned long)) {			\
// /*AFLA*/ 		m.bits[0] = 1UL << (node);				\
// /*AFLA*/ 	} else {							\
// /*AFLA*/ 		init_nodemask_of_node(&m, (node));			\
// /*AFLA*/ 	}								\
// /*AFLA*/ 	m;								\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ #define first_unset_node(mask) __first_unset_node(&(mask))
// /*AFLA*/ static inline int __first_unset_node(const nodemask_t *maskp)
// /*AFLA*/ {
// /*AFLA*/ 	return min_t(int,MAX_NUMNODES,
// /*AFLA*/ 			find_first_zero_bit(maskp->bits, MAX_NUMNODES));
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define NODE_MASK_LAST_WORD BITMAP_LAST_WORD_MASK(MAX_NUMNODES)
// /*AFLA*/ 
// /*AFLA*/ #if MAX_NUMNODES <= BITS_PER_LONG
// /*AFLA*/ 
// /*AFLA*/ #define NODE_MASK_ALL							\
// /*AFLA*/ ((nodemask_t) { {							\
// /*AFLA*/ 	[BITS_TO_LONGS(MAX_NUMNODES)-1] = NODE_MASK_LAST_WORD		\
// /*AFLA*/ } })
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ #define NODE_MASK_ALL							\
// /*AFLA*/ ((nodemask_t) { {							\
// /*AFLA*/ 	[0 ... BITS_TO_LONGS(MAX_NUMNODES)-2] = ~0UL,			\
// /*AFLA*/ 	[BITS_TO_LONGS(MAX_NUMNODES)-1] = NODE_MASK_LAST_WORD		\
// /*AFLA*/ } })
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define NODE_MASK_NONE							\
// /*AFLA*/ ((nodemask_t) { {							\
// /*AFLA*/ 	[0 ... BITS_TO_LONGS(MAX_NUMNODES)-1] =  0UL			\
// /*AFLA*/ } })
// /*AFLA*/ 
// /*AFLA*/ #define nodes_addr(src) ((src).bits)
// /*AFLA*/ 
// /*AFLA*/ #define nodemask_parse_user(ubuf, ulen, dst) \
// /*AFLA*/ 		__nodemask_parse_user((ubuf), (ulen), &(dst), MAX_NUMNODES)
// /*AFLA*/ static inline int __nodemask_parse_user(const char __user *buf, int len,
// /*AFLA*/ 					nodemask_t *dstp, int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	return bitmap_parse_user(buf, len, dstp->bits, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define nodelist_parse(buf, dst) __nodelist_parse((buf), &(dst), MAX_NUMNODES)
// /*AFLA*/ static inline int __nodelist_parse(const char *buf, nodemask_t *dstp, int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	return bitmap_parselist(buf, dstp->bits, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define node_remap(oldbit, old, new) \
// /*AFLA*/ 		__node_remap((oldbit), &(old), &(new), MAX_NUMNODES)
// /*AFLA*/ static inline int __node_remap(int oldbit,
// /*AFLA*/ 		const nodemask_t *oldp, const nodemask_t *newp, int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	return bitmap_bitremap(oldbit, oldp->bits, newp->bits, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define nodes_remap(dst, src, old, new) \
// /*AFLA*/ 		__nodes_remap(&(dst), &(src), &(old), &(new), MAX_NUMNODES)
// /*AFLA*/ static inline void __nodes_remap(nodemask_t *dstp, const nodemask_t *srcp,
// /*AFLA*/ 		const nodemask_t *oldp, const nodemask_t *newp, int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	bitmap_remap(dstp->bits, srcp->bits, oldp->bits, newp->bits, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define nodes_onto(dst, orig, relmap) \
// /*AFLA*/ 		__nodes_onto(&(dst), &(orig), &(relmap), MAX_NUMNODES)
// /*AFLA*/ static inline void __nodes_onto(nodemask_t *dstp, const nodemask_t *origp,
// /*AFLA*/ 		const nodemask_t *relmapp, int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	bitmap_onto(dstp->bits, origp->bits, relmapp->bits, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define nodes_fold(dst, orig, sz) \
// /*AFLA*/ 		__nodes_fold(&(dst), &(orig), sz, MAX_NUMNODES)
// /*AFLA*/ static inline void __nodes_fold(nodemask_t *dstp, const nodemask_t *origp,
// /*AFLA*/ 		int sz, int nbits)
// /*AFLA*/ {
// /*AFLA*/ 	bitmap_fold(dstp->bits, origp->bits, sz, nbits);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #if MAX_NUMNODES > 1
// /*AFLA*/ #define for_each_node_mask(node, mask)			\
// /*AFLA*/ 	for ((node) = first_node(mask);			\
// /*AFLA*/ 		(node) < MAX_NUMNODES;			\
// /*AFLA*/ 		(node) = next_node((node), (mask)))
// /*AFLA*/ #else /* MAX_NUMNODES == 1 */
// /*AFLA*/ #define for_each_node_mask(node, mask)			\
// /*AFLA*/ 	if (!nodes_empty(mask))				\
// /*AFLA*/ 		for ((node) = 0; (node) < 1; (node)++)
// /*AFLA*/ #endif /* MAX_NUMNODES */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Bitmasks that are kept for all the nodes.
// /*AFLA*/  */
// /*AFLA*/ enum node_states {
// /*AFLA*/ 	N_POSSIBLE,		/* The node could become online at some point */
// /*AFLA*/ 	N_ONLINE,		/* The node is online */
// /*AFLA*/ 	N_NORMAL_MEMORY,	/* The node has regular memory */
// /*AFLA*/ #ifdef CONFIG_HIGHMEM
// /*AFLA*/ 	N_HIGH_MEMORY,		/* The node has regular or high memory */
// /*AFLA*/ #else
// /*AFLA*/ 	N_HIGH_MEMORY = N_NORMAL_MEMORY,
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_MOVABLE_NODE
// /*AFLA*/ 	N_MEMORY,		/* The node has memory(regular, high, movable) */
// /*AFLA*/ #else
// /*AFLA*/ 	N_MEMORY = N_HIGH_MEMORY,
// /*AFLA*/ #endif
// /*AFLA*/ 	N_CPU,		/* The node has one or more cpus */
// /*AFLA*/ 	NR_NODE_STATES
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The following particular system nodemasks and operations
// /*AFLA*/  * on them manage all possible and online nodes.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ extern nodemask_t node_states[NR_NODE_STATES];
// /*AFLA*/ 
// /*AFLA*/ #if MAX_NUMNODES > 1
// /*AFLA*/ static inline int node_state(int node, enum node_states state)
// /*AFLA*/ {
// /*AFLA*/ 	return node_isset(node, node_states[state]);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void node_set_state(int node, enum node_states state)
// /*AFLA*/ {
// /*AFLA*/ 	__node_set(node, &node_states[state]);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void node_clear_state(int node, enum node_states state)
// /*AFLA*/ {
// /*AFLA*/ 	__node_clear(node, &node_states[state]);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int num_node_state(enum node_states state)
// /*AFLA*/ {
// /*AFLA*/ 	return nodes_weight(node_states[state]);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define for_each_node_state(__node, __state) \
// /*AFLA*/ 	for_each_node_mask((__node), node_states[__state])
// /*AFLA*/ 
// /*AFLA*/ #define first_online_node	first_node(node_states[N_ONLINE])
// /*AFLA*/ #define first_memory_node	first_node(node_states[N_MEMORY])
// /*AFLA*/ static inline int next_online_node(int nid)
// /*AFLA*/ {
// /*AFLA*/ 	return next_node(nid, node_states[N_ONLINE]);
// /*AFLA*/ }
// /*AFLA*/ static inline int next_memory_node(int nid)
// /*AFLA*/ {
// /*AFLA*/ 	return next_node(nid, node_states[N_MEMORY]);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int nr_node_ids;
// /*AFLA*/ extern int nr_online_nodes;
// /*AFLA*/ 
// /*AFLA*/ static inline void node_set_online(int nid)
// /*AFLA*/ {
// /*AFLA*/ 	node_set_state(nid, N_ONLINE);
// /*AFLA*/ 	nr_online_nodes = num_node_state(N_ONLINE);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void node_set_offline(int nid)
// /*AFLA*/ {
// /*AFLA*/ 	node_clear_state(nid, N_ONLINE);
// /*AFLA*/ 	nr_online_nodes = num_node_state(N_ONLINE);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ static inline int node_state(int node, enum node_states state)
// /*AFLA*/ {
// /*AFLA*/ 	return node == 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void node_set_state(int node, enum node_states state)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void node_clear_state(int node, enum node_states state)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int num_node_state(enum node_states state)
// /*AFLA*/ {
// /*AFLA*/ 	return 1;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define for_each_node_state(node, __state) \
// /*AFLA*/ 	for ( (node) = 0; (node) == 0; (node) = 1)
// /*AFLA*/ 
// /*AFLA*/ #define first_online_node	0
// /*AFLA*/ #define first_memory_node	0
// /*AFLA*/ #define next_online_node(nid)	(MAX_NUMNODES)
// /*AFLA*/ #define nr_node_ids		1
// /*AFLA*/ #define nr_online_nodes		1
// /*AFLA*/ 
// /*AFLA*/ #define node_set_online(node)	   node_set_state((node), N_ONLINE)
// /*AFLA*/ #define node_set_offline(node)	   node_clear_state((node), N_ONLINE)
// /*AFLA*/ 
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_NUMA) && (MAX_NUMNODES > 1)
// /*AFLA*/ extern int node_random(const nodemask_t *maskp);
// /*AFLA*/ #else
// /*AFLA*/ static inline int node_random(const nodemask_t *mask)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define node_online_map 	node_states[N_ONLINE]
// /*AFLA*/ #define node_possible_map 	node_states[N_POSSIBLE]
// /*AFLA*/ 
// /*AFLA*/ #define num_online_nodes()	num_node_state(N_ONLINE)
// /*AFLA*/ #define num_possible_nodes()	num_node_state(N_POSSIBLE)
// /*AFLA*/ #define node_online(node)	node_state((node), N_ONLINE)
// /*AFLA*/ #define node_possible(node)	node_state((node), N_POSSIBLE)
// /*AFLA*/ 
// /*AFLA*/ #define for_each_node(node)	   for_each_node_state(node, N_POSSIBLE)
// /*AFLA*/ #define for_each_online_node(node) for_each_node_state(node, N_ONLINE)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * For nodemask scrach area.
// /*AFLA*/  * NODEMASK_ALLOC(type, name) allocates an object with a specified type and
// /*AFLA*/  * name.
// /*AFLA*/  */
// /*AFLA*/ #if NODES_SHIFT > 8 /* nodemask_t > 256 bytes */
// /*AFLA*/ #define NODEMASK_ALLOC(type, name, gfp_flags)	\
// /*AFLA*/ 			type *name = kmalloc(sizeof(*name), gfp_flags)
// /*AFLA*/ #define NODEMASK_FREE(m)			kfree(m)
// /*AFLA*/ #else
// /*AFLA*/ #define NODEMASK_ALLOC(type, name, gfp_flags)	type _##name, *name = &_##name
// /*AFLA*/ #define NODEMASK_FREE(m)			do {} while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /* A example struture for using NODEMASK_ALLOC, used in mempolicy. */
// /*AFLA*/ struct nodemask_scratch {
// /*AFLA*/ 	nodemask_t	mask1;
// /*AFLA*/ 	nodemask_t	mask2;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #define NODEMASK_SCRATCH(x)						\
// /*AFLA*/ 			NODEMASK_ALLOC(struct nodemask_scratch, x,	\
// /*AFLA*/ 					GFP_KERNEL | __GFP_NORETRY)
// /*AFLA*/ #define NODEMASK_SCRATCH_FREE(x)	NODEMASK_FREE(x)
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_NODEMASK_H */
