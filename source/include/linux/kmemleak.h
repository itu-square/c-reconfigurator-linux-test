// /*AFLA*/ /*
// /*AFLA*/  * include/linux/kmemleak.h
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) 2008 ARM Limited
// /*AFLA*/  * Written by Catalin Marinas <catalin.marinas@arm.com>
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or modify
// /*AFLA*/  * it under the terms of the GNU General Public License version 2 as
// /*AFLA*/  * published by the Free Software Foundation.
// /*AFLA*/  *
// /*AFLA*/  * This program is distributed in the hope that it will be useful,
// /*AFLA*/  * but WITHOUT ANY WARRANTY; without even the implied warranty of
// /*AFLA*/  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// /*AFLA*/  * GNU General Public License for more details.
// /*AFLA*/  *
// /*AFLA*/  * You should have received a copy of the GNU General Public License
// /*AFLA*/  * along with this program; if not, write to the Free Software
// /*AFLA*/  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __KMEMLEAK_H
// /*AFLA*/ #define __KMEMLEAK_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/slab.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_KMEMLEAK
// /*AFLA*/ 
// /*AFLA*/ extern void kmemleak_init(void) __init;
// /*AFLA*/ extern void kmemleak_alloc(const void *ptr, size_t size, int min_count,
// /*AFLA*/ 			   gfp_t gfp) __ref;
// /*AFLA*/ extern void kmemleak_alloc_percpu(const void __percpu *ptr, size_t size,
// /*AFLA*/ 				  gfp_t gfp) __ref;
// /*AFLA*/ extern void kmemleak_free(const void *ptr) __ref;
// /*AFLA*/ extern void kmemleak_free_part(const void *ptr, size_t size) __ref;
// /*AFLA*/ extern void kmemleak_free_percpu(const void __percpu *ptr) __ref;
// /*AFLA*/ extern void kmemleak_update_trace(const void *ptr) __ref;
// /*AFLA*/ extern void kmemleak_not_leak(const void *ptr) __ref;
// /*AFLA*/ extern void kmemleak_ignore(const void *ptr) __ref;
// /*AFLA*/ extern void kmemleak_scan_area(const void *ptr, size_t size, gfp_t gfp) __ref;
// /*AFLA*/ extern void kmemleak_no_scan(const void *ptr) __ref;
// /*AFLA*/ extern void kmemleak_alloc_phys(phys_addr_t phys, size_t size, int min_count,
// /*AFLA*/ 				gfp_t gfp) __ref;
// /*AFLA*/ extern void kmemleak_free_part_phys(phys_addr_t phys, size_t size) __ref;
// /*AFLA*/ extern void kmemleak_not_leak_phys(phys_addr_t phys) __ref;
// /*AFLA*/ extern void kmemleak_ignore_phys(phys_addr_t phys) __ref;
// /*AFLA*/ 
// /*AFLA*/ static inline void kmemleak_alloc_recursive(const void *ptr, size_t size,
// /*AFLA*/ 					    int min_count, unsigned long flags,
// /*AFLA*/ 					    gfp_t gfp)
// /*AFLA*/ {
// /*AFLA*/ 	if (!(flags & SLAB_NOLEAKTRACE))
// /*AFLA*/ 		kmemleak_alloc(ptr, size, min_count, gfp);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void kmemleak_free_recursive(const void *ptr, unsigned long flags)
// /*AFLA*/ {
// /*AFLA*/ 	if (!(flags & SLAB_NOLEAKTRACE))
// /*AFLA*/ 		kmemleak_free(ptr);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void kmemleak_erase(void **ptr)
// /*AFLA*/ {
// /*AFLA*/ 	*ptr = NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ static inline void kmemleak_init(void)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void kmemleak_alloc(const void *ptr, size_t size, int min_count,
// /*AFLA*/ 				  gfp_t gfp)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void kmemleak_alloc_recursive(const void *ptr, size_t size,
// /*AFLA*/ 					    int min_count, unsigned long flags,
// /*AFLA*/ 					    gfp_t gfp)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void kmemleak_alloc_percpu(const void __percpu *ptr, size_t size,
// /*AFLA*/ 					 gfp_t gfp)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void kmemleak_free(const void *ptr)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void kmemleak_free_part(const void *ptr, size_t size)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void kmemleak_free_recursive(const void *ptr, unsigned long flags)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void kmemleak_free_percpu(const void __percpu *ptr)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void kmemleak_update_trace(const void *ptr)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void kmemleak_not_leak(const void *ptr)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void kmemleak_ignore(const void *ptr)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void kmemleak_scan_area(const void *ptr, size_t size, gfp_t gfp)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void kmemleak_erase(void **ptr)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void kmemleak_no_scan(const void *ptr)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void kmemleak_alloc_phys(phys_addr_t phys, size_t size,
// /*AFLA*/ 				       int min_count, gfp_t gfp)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void kmemleak_free_part_phys(phys_addr_t phys, size_t size)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void kmemleak_not_leak_phys(phys_addr_t phys)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void kmemleak_ignore_phys(phys_addr_t phys)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif	/* CONFIG_DEBUG_KMEMLEAK */
// /*AFLA*/ 
// /*AFLA*/ #endif	/* __KMEMLEAK_H */
