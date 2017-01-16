// /*AFLA*/ /*
// /*AFLA*/   Red Black Trees
// /*AFLA*/   (C) 1999  Andrea Arcangeli <andrea@suse.de>
// /*AFLA*/   
// /*AFLA*/   This program is free software; you can redistribute it and/or modify
// /*AFLA*/   it under the terms of the GNU General Public License as published by
// /*AFLA*/   the Free Software Foundation; either version 2 of the License, or
// /*AFLA*/   (at your option) any later version.
// /*AFLA*/ 
// /*AFLA*/   This program is distributed in the hope that it will be useful,
// /*AFLA*/   but WITHOUT ANY WARRANTY; without even the implied warranty of
// /*AFLA*/   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// /*AFLA*/   GNU General Public License for more details.
// /*AFLA*/ 
// /*AFLA*/   You should have received a copy of the GNU General Public License
// /*AFLA*/   along with this program; if not, write to the Free Software
// /*AFLA*/   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// /*AFLA*/ 
// /*AFLA*/   linux/include/linux/rbtree.h
// /*AFLA*/ 
// /*AFLA*/   To use rbtrees you'll have to implement your own insert and search cores.
// /*AFLA*/   This will avoid us to use callbacks and to drop drammatically performances.
// /*AFLA*/   I know it's not the cleaner way,  but in C (not in C++) to get
// /*AFLA*/   performances and genericity...
// /*AFLA*/ 
// /*AFLA*/   See Documentation/rbtree.txt for documentation and samples.
// /*AFLA*/ */
// /*AFLA*/ 
// /*AFLA*/ #ifndef	_LINUX_RBTREE_H
// /*AFLA*/ #define	_LINUX_RBTREE_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/kernel.h>
// /*AFLA*/ #include <linux/stddef.h>
// /*AFLA*/ #include <linux/rcupdate.h>
// /*AFLA*/ 
// /*AFLA*/ struct rb_node {
// /*AFLA*/ 	unsigned long  __rb_parent_color;
// /*AFLA*/ 	struct rb_node *rb_right;
// /*AFLA*/ 	struct rb_node *rb_left;
// /*AFLA*/ } __attribute__((aligned(sizeof(long))));
// /*AFLA*/     /* The alignment might seem pointless, but allegedly CRIS needs it */
// /*AFLA*/ 
// /*AFLA*/ struct rb_root {
// /*AFLA*/ 	struct rb_node *rb_node;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #define rb_parent(r)   ((struct rb_node *)((r)->__rb_parent_color & ~3))
// /*AFLA*/ 
// /*AFLA*/ #define RB_ROOT	(struct rb_root) { NULL, }
// /*AFLA*/ #define	rb_entry(ptr, type, member) container_of(ptr, type, member)
// /*AFLA*/ 
// /*AFLA*/ #define RB_EMPTY_ROOT(root)  (READ_ONCE((root)->rb_node) == NULL)
// /*AFLA*/ 
// /*AFLA*/ /* 'empty' nodes are nodes that are known not to be inserted in an rbtree */
// /*AFLA*/ #define RB_EMPTY_NODE(node)  \
// /*AFLA*/ 	((node)->__rb_parent_color == (unsigned long)(node))
// /*AFLA*/ #define RB_CLEAR_NODE(node)  \
// /*AFLA*/ 	((node)->__rb_parent_color = (unsigned long)(node))
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ extern void rb_insert_color(struct rb_node *, struct rb_root *);
// /*AFLA*/ extern void rb_erase(struct rb_node *, struct rb_root *);
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ /* Find logical next and previous nodes in a tree */
// /*AFLA*/ extern struct rb_node *rb_next(const struct rb_node *);
// /*AFLA*/ extern struct rb_node *rb_prev(const struct rb_node *);
// /*AFLA*/ extern struct rb_node *rb_first(const struct rb_root *);
// /*AFLA*/ extern struct rb_node *rb_last(const struct rb_root *);
// /*AFLA*/ 
// /*AFLA*/ /* Postorder iteration - always visit the parent after its children */
// /*AFLA*/ extern struct rb_node *rb_first_postorder(const struct rb_root *);
// /*AFLA*/ extern struct rb_node *rb_next_postorder(const struct rb_node *);
// /*AFLA*/ 
// /*AFLA*/ /* Fast replacement of a single node without remove/rebalance/add/rebalance */
// /*AFLA*/ extern void rb_replace_node(struct rb_node *victim, struct rb_node *new,
// /*AFLA*/ 			    struct rb_root *root);
// /*AFLA*/ extern void rb_replace_node_rcu(struct rb_node *victim, struct rb_node *new,
// /*AFLA*/ 				struct rb_root *root);
// /*AFLA*/ 
// /*AFLA*/ static inline void rb_link_node(struct rb_node *node, struct rb_node *parent,
// /*AFLA*/ 				struct rb_node **rb_link)
// /*AFLA*/ {
// /*AFLA*/ 	node->__rb_parent_color = (unsigned long)parent;
// /*AFLA*/ 	node->rb_left = node->rb_right = NULL;
// /*AFLA*/ 
// /*AFLA*/ 	*rb_link = node;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void rb_link_node_rcu(struct rb_node *node, struct rb_node *parent,
// /*AFLA*/ 				    struct rb_node **rb_link)
// /*AFLA*/ {
// /*AFLA*/ 	node->__rb_parent_color = (unsigned long)parent;
// /*AFLA*/ 	node->rb_left = node->rb_right = NULL;
// /*AFLA*/ 
// /*AFLA*/ 	rcu_assign_pointer(*rb_link, node);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define rb_entry_safe(ptr, type, member) \
// /*AFLA*/ 	({ typeof(ptr) ____ptr = (ptr); \
// /*AFLA*/ 	   ____ptr ? rb_entry(____ptr, type, member) : NULL; \
// /*AFLA*/ 	})
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * rbtree_postorder_for_each_entry_safe - iterate in post-order over rb_root of
// /*AFLA*/  * given type allowing the backing memory of @pos to be invalidated
// /*AFLA*/  *
// /*AFLA*/  * @pos:	the 'type *' to use as a loop cursor.
// /*AFLA*/  * @n:		another 'type *' to use as temporary storage
// /*AFLA*/  * @root:	'rb_root *' of the rbtree.
// /*AFLA*/  * @field:	the name of the rb_node field within 'type'.
// /*AFLA*/  *
// /*AFLA*/  * rbtree_postorder_for_each_entry_safe() provides a similar guarantee as
// /*AFLA*/  * list_for_each_entry_safe() and allows the iteration to continue independent
// /*AFLA*/  * of changes to @pos by the body of the loop.
// /*AFLA*/  *
// /*AFLA*/  * Note, however, that it cannot handle other modifications that re-order the
// /*AFLA*/  * rbtree it is iterating over. This includes calling rb_erase() on @pos, as
// /*AFLA*/  * rb_erase() may rebalance the tree, causing us to miss some nodes.
// /*AFLA*/  */
// /*AFLA*/ #define rbtree_postorder_for_each_entry_safe(pos, n, root, field) \
// /*AFLA*/ 	for (pos = rb_entry_safe(rb_first_postorder(root), typeof(*pos), field); \
// /*AFLA*/ 	     pos && ({ n = rb_entry_safe(rb_next_postorder(&pos->field), \
// /*AFLA*/ 			typeof(*pos), field); 1; }); \
// /*AFLA*/ 	     pos = n)
// /*AFLA*/ 
// /*AFLA*/ #endif	/* _LINUX_RBTREE_H */
