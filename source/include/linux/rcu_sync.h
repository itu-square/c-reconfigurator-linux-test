// /*AFLA*/ /*
// /*AFLA*/  * RCU-based infrastructure for lightweight reader-writer locking
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or modify
// /*AFLA*/  * it under the terms of the GNU General Public License as published by
// /*AFLA*/  * the Free Software Foundation; either version 2 of the License, or
// /*AFLA*/  * (at your option) any later version.
// /*AFLA*/  *
// /*AFLA*/  * This program is distributed in the hope that it will be useful,
// /*AFLA*/  * but WITHOUT ANY WARRANTY; without even the implied warranty of
// /*AFLA*/  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// /*AFLA*/  * GNU General Public License for more details.
// /*AFLA*/  *
// /*AFLA*/  * You should have received a copy of the GNU General Public License
// /*AFLA*/  * along with this program; if not, you can access it online at
// /*AFLA*/  * http://www.gnu.org/licenses/gpl-2.0.html.
// /*AFLA*/  *
// /*AFLA*/  * Copyright (c) 2015, Red Hat, Inc.
// /*AFLA*/  *
// /*AFLA*/  * Author: Oleg Nesterov <oleg@redhat.com>
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_RCU_SYNC_H_
// /*AFLA*/ #define _LINUX_RCU_SYNC_H_
// /*AFLA*/ 
// /*AFLA*/ #include <linux/wait.h>
// /*AFLA*/ #include <linux/rcupdate.h>
// /*AFLA*/ 
// /*AFLA*/ enum rcu_sync_type { RCU_SYNC, RCU_SCHED_SYNC, RCU_BH_SYNC };
// /*AFLA*/ 
// /*AFLA*/ /* Structure to mediate between updaters and fastpath-using readers.  */
// /*AFLA*/ struct rcu_sync {
// /*AFLA*/ 	int			gp_state;
// /*AFLA*/ 	int			gp_count;
// /*AFLA*/ 	wait_queue_head_t	gp_wait;
// /*AFLA*/ 
// /*AFLA*/ 	int			cb_state;
// /*AFLA*/ 	struct rcu_head		cb_head;
// /*AFLA*/ 
// /*AFLA*/ 	enum rcu_sync_type	gp_type;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern void rcu_sync_lockdep_assert(struct rcu_sync *);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rcu_sync_is_idle() - Are readers permitted to use their fastpaths?
// /*AFLA*/  * @rsp: Pointer to rcu_sync structure to use for synchronization
// /*AFLA*/  *
// /*AFLA*/  * Returns true if readers are permitted to use their fastpaths.
// /*AFLA*/  * Must be invoked within an RCU read-side critical section whose
// /*AFLA*/  * flavor matches that of the rcu_sync struture.
// /*AFLA*/  */
// /*AFLA*/ static inline bool rcu_sync_is_idle(struct rcu_sync *rsp)
// /*AFLA*/ {
// /*AFLA*/ #ifdef CONFIG_PROVE_RCU
// /*AFLA*/ 	rcu_sync_lockdep_assert(rsp);
// /*AFLA*/ #endif
// /*AFLA*/ 	return !rsp->gp_state; /* GP_IDLE */
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void rcu_sync_init(struct rcu_sync *, enum rcu_sync_type);
// /*AFLA*/ extern void rcu_sync_enter_start(struct rcu_sync *);
// /*AFLA*/ extern void rcu_sync_enter(struct rcu_sync *);
// /*AFLA*/ extern void rcu_sync_exit(struct rcu_sync *);
// /*AFLA*/ extern void rcu_sync_dtor(struct rcu_sync *);
// /*AFLA*/ 
// /*AFLA*/ #define __RCU_SYNC_INITIALIZER(name, type) {				\
// /*AFLA*/ 		.gp_state = 0,						\
// /*AFLA*/ 		.gp_count = 0,						\
// /*AFLA*/ 		.gp_wait = __WAIT_QUEUE_HEAD_INITIALIZER(name.gp_wait),	\
// /*AFLA*/ 		.cb_state = 0,						\
// /*AFLA*/ 		.gp_type = type,					\
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ #define	__DEFINE_RCU_SYNC(name, type)	\
// /*AFLA*/ 	struct rcu_sync_struct name = __RCU_SYNC_INITIALIZER(name, type)
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_RCU_SYNC(name)		\
// /*AFLA*/ 	__DEFINE_RCU_SYNC(name, RCU_SYNC)
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_RCU_SCHED_SYNC(name)	\
// /*AFLA*/ 	__DEFINE_RCU_SYNC(name, RCU_SCHED_SYNC)
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_RCU_BH_SYNC(name)	\
// /*AFLA*/ 	__DEFINE_RCU_SYNC(name, RCU_BH_SYNC)
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_RCU_SYNC_H_ */
