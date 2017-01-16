// /*AFLA*/ #ifndef _LINUX_TIMERQUEUE_H
// /*AFLA*/ #define _LINUX_TIMERQUEUE_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/rbtree.h>
// /*AFLA*/ #include <linux/ktime.h>
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ struct timerqueue_node {
// /*AFLA*/ 	struct rb_node node;
// /*AFLA*/ 	ktime_t expires;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct timerqueue_head {
// /*AFLA*/ 	struct rb_root head;
// /*AFLA*/ 	struct timerqueue_node *next;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ extern bool timerqueue_add(struct timerqueue_head *head,
// /*AFLA*/ 			   struct timerqueue_node *node);
// /*AFLA*/ extern bool timerqueue_del(struct timerqueue_head *head,
// /*AFLA*/ 			   struct timerqueue_node *node);
// /*AFLA*/ extern struct timerqueue_node *timerqueue_iterate_next(
// /*AFLA*/ 						struct timerqueue_node *node);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * timerqueue_getnext - Returns the timer with the earliest expiration time
// /*AFLA*/  *
// /*AFLA*/  * @head: head of timerqueue
// /*AFLA*/  *
// /*AFLA*/  * Returns a pointer to the timer node that has the
// /*AFLA*/  * earliest expiration time.
// /*AFLA*/  */
// /*AFLA*/ static inline
// /*AFLA*/ struct timerqueue_node *timerqueue_getnext(struct timerqueue_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	return head->next;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void timerqueue_init(struct timerqueue_node *node)
// /*AFLA*/ {
// /*AFLA*/ 	RB_CLEAR_NODE(&node->node);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void timerqueue_init_head(struct timerqueue_head *head)
// /*AFLA*/ {
// /*AFLA*/ 	head->head = RB_ROOT;
// /*AFLA*/ 	head->next = NULL;
// /*AFLA*/ }
// /*AFLA*/ #endif /* _LINUX_TIMERQUEUE_H */
