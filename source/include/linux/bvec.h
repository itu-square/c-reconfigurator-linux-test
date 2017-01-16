// /*AFLA*/ /*
// /*AFLA*/  * bvec iterator
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) 2001 Ming Lei <ming.lei@canonical.com>
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or modify
// /*AFLA*/  * it under the terms of the GNU General Public License version 2 as
// /*AFLA*/  * published by the Free Software Foundation.
// /*AFLA*/  *
// /*AFLA*/  * This program is distributed in the hope that it will be useful,
// /*AFLA*/  * but WITHOUT ANY WARRANTY; without even the implied warranty of
// /*AFLA*/  *
// /*AFLA*/  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// /*AFLA*/  * GNU General Public License for more details.
// /*AFLA*/  *
// /*AFLA*/  * You should have received a copy of the GNU General Public Licens
// /*AFLA*/  * along with this program; if not, write to the Free Software
// /*AFLA*/  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-
// /*AFLA*/  */
// /*AFLA*/ #ifndef __LINUX_BVEC_ITER_H
// /*AFLA*/ #define __LINUX_BVEC_ITER_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ #include <linux/bug.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * was unsigned short, but we might as well be ready for > 64kB I/O pages
// /*AFLA*/  */
// /*AFLA*/ struct bio_vec {
// /*AFLA*/ 	struct page	*bv_page;
// /*AFLA*/ 	unsigned int	bv_len;
// /*AFLA*/ 	unsigned int	bv_offset;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct bvec_iter {
// /*AFLA*/ 	sector_t		bi_sector;	/* device address in 512 byte
// /*AFLA*/ 						   sectors */
// /*AFLA*/ 	unsigned int		bi_size;	/* residual I/O count */
// /*AFLA*/ 
// /*AFLA*/ 	unsigned int		bi_idx;		/* current index into bvl_vec */
// /*AFLA*/ 
// /*AFLA*/ 	unsigned int            bi_bvec_done;	/* number of bytes completed in
// /*AFLA*/ 						   current bvec */
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * various member access, note that bio_data should of course not be used
// /*AFLA*/  * on highmem page vectors
// /*AFLA*/  */
// /*AFLA*/ #define __bvec_iter_bvec(bvec, iter)	(&(bvec)[(iter).bi_idx])
// /*AFLA*/ 
// /*AFLA*/ #define bvec_iter_page(bvec, iter)				\
// /*AFLA*/ 	(__bvec_iter_bvec((bvec), (iter))->bv_page)
// /*AFLA*/ 
// /*AFLA*/ #define bvec_iter_len(bvec, iter)				\
// /*AFLA*/ 	min((iter).bi_size,					\
// /*AFLA*/ 	    __bvec_iter_bvec((bvec), (iter))->bv_len - (iter).bi_bvec_done)
// /*AFLA*/ 
// /*AFLA*/ #define bvec_iter_offset(bvec, iter)				\
// /*AFLA*/ 	(__bvec_iter_bvec((bvec), (iter))->bv_offset + (iter).bi_bvec_done)
// /*AFLA*/ 
// /*AFLA*/ #define bvec_iter_bvec(bvec, iter)				\
// /*AFLA*/ ((struct bio_vec) {						\
// /*AFLA*/ 	.bv_page	= bvec_iter_page((bvec), (iter)),	\
// /*AFLA*/ 	.bv_len		= bvec_iter_len((bvec), (iter)),	\
// /*AFLA*/ 	.bv_offset	= bvec_iter_offset((bvec), (iter)),	\
// /*AFLA*/ })
// /*AFLA*/ 
// /*AFLA*/ static inline void bvec_iter_advance(const struct bio_vec *bv,
// /*AFLA*/ 				     struct bvec_iter *iter,
// /*AFLA*/ 				     unsigned bytes)
// /*AFLA*/ {
// /*AFLA*/ 	WARN_ONCE(bytes > iter->bi_size,
// /*AFLA*/ 		  "Attempted to advance past end of bvec iter\n");
// /*AFLA*/ 
// /*AFLA*/ 	while (bytes) {
// /*AFLA*/ 		unsigned iter_len = bvec_iter_len(bv, *iter);
// /*AFLA*/ 		unsigned len = min(bytes, iter_len);
// /*AFLA*/ 
// /*AFLA*/ 		bytes -= len;
// /*AFLA*/ 		iter->bi_size -= len;
// /*AFLA*/ 		iter->bi_bvec_done += len;
// /*AFLA*/ 
// /*AFLA*/ 		if (iter->bi_bvec_done == __bvec_iter_bvec(bv, *iter)->bv_len) {
// /*AFLA*/ 			iter->bi_bvec_done = 0;
// /*AFLA*/ 			iter->bi_idx++;
// /*AFLA*/ 		}
// /*AFLA*/ 	}
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define for_each_bvec(bvl, bio_vec, iter, start)			\
// /*AFLA*/ 	for (iter = (start);						\
// /*AFLA*/ 	     (iter).bi_size &&						\
// /*AFLA*/ 		((bvl = bvec_iter_bvec((bio_vec), (iter))), 1);	\
// /*AFLA*/ 	     bvec_iter_advance((bio_vec), &(iter), (bvl).bv_len))
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_BVEC_ITER_H */
