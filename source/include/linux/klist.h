// /*AFLA*/ /*
// /*AFLA*/  *	klist.h - Some generic list helpers, extending struct list_head a bit.
// /*AFLA*/  *
// /*AFLA*/  *	Implementations are found in lib/klist.c
// /*AFLA*/  *
// /*AFLA*/  *
// /*AFLA*/  *	Copyright (C) 2005 Patrick Mochel
// /*AFLA*/  *
// /*AFLA*/  *	This file is rleased under the GPL v2.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_KLIST_H
// /*AFLA*/ #define _LINUX_KLIST_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/spinlock.h>
// /*AFLA*/ #include <linux/kref.h>
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ 
// /*AFLA*/ struct klist_node;
// /*AFLA*/ struct klist {
// /*AFLA*/ 	spinlock_t		k_lock;
// /*AFLA*/ 	struct list_head	k_list;
// /*AFLA*/ 	void			(*get)(struct klist_node *);
// /*AFLA*/ 	void			(*put)(struct klist_node *);
// /*AFLA*/ } __attribute__ ((aligned (sizeof(void *))));
// /*AFLA*/ 
// /*AFLA*/ #define KLIST_INIT(_name, _get, _put)					\
// /*AFLA*/ 	{ .k_lock	= __SPIN_LOCK_UNLOCKED(_name.k_lock),		\
// /*AFLA*/ 	  .k_list	= LIST_HEAD_INIT(_name.k_list),			\
// /*AFLA*/ 	  .get		= _get,						\
// /*AFLA*/ 	  .put		= _put, }
// /*AFLA*/ 
// /*AFLA*/ #define DEFINE_KLIST(_name, _get, _put)					\
// /*AFLA*/ 	struct klist _name = KLIST_INIT(_name, _get, _put)
// /*AFLA*/ 
// /*AFLA*/ extern void klist_init(struct klist *k, void (*get)(struct klist_node *),
// /*AFLA*/ 		       void (*put)(struct klist_node *));
// /*AFLA*/ 
// /*AFLA*/ struct klist_node {
// /*AFLA*/ 	void			*n_klist;	/* never access directly */
// /*AFLA*/ 	struct list_head	n_node;
// /*AFLA*/ 	struct kref		n_ref;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern void klist_add_tail(struct klist_node *n, struct klist *k);
// /*AFLA*/ extern void klist_add_head(struct klist_node *n, struct klist *k);
// /*AFLA*/ extern void klist_add_behind(struct klist_node *n, struct klist_node *pos);
// /*AFLA*/ extern void klist_add_before(struct klist_node *n, struct klist_node *pos);
// /*AFLA*/ 
// /*AFLA*/ extern void klist_del(struct klist_node *n);
// /*AFLA*/ extern void klist_remove(struct klist_node *n);
// /*AFLA*/ 
// /*AFLA*/ extern int klist_node_attached(struct klist_node *n);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ struct klist_iter {
// /*AFLA*/ 	struct klist		*i_klist;
// /*AFLA*/ 	struct klist_node	*i_cur;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ extern void klist_iter_init(struct klist *k, struct klist_iter *i);
// /*AFLA*/ extern void klist_iter_init_node(struct klist *k, struct klist_iter *i,
// /*AFLA*/ 				 struct klist_node *n);
// /*AFLA*/ extern void klist_iter_exit(struct klist_iter *i);
// /*AFLA*/ extern struct klist_node *klist_prev(struct klist_iter *i);
// /*AFLA*/ extern struct klist_node *klist_next(struct klist_iter *i);
// /*AFLA*/ 
// /*AFLA*/ #endif
