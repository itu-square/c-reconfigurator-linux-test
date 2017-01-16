// /*AFLA*/ #ifndef _LINUX_DEBUGOBJECTS_H
// /*AFLA*/ #define _LINUX_DEBUGOBJECTS_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/spinlock.h>
// /*AFLA*/ 
// /*AFLA*/ enum debug_obj_state {
// /*AFLA*/ 	ODEBUG_STATE_NONE,
// /*AFLA*/ 	ODEBUG_STATE_INIT,
// /*AFLA*/ 	ODEBUG_STATE_INACTIVE,
// /*AFLA*/ 	ODEBUG_STATE_ACTIVE,
// /*AFLA*/ 	ODEBUG_STATE_DESTROYED,
// /*AFLA*/ 	ODEBUG_STATE_NOTAVAILABLE,
// /*AFLA*/ 	ODEBUG_STATE_MAX,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct debug_obj_descr;
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct debug_obj - representaion of an tracked object
// /*AFLA*/  * @node:	hlist node to link the object into the tracker list
// /*AFLA*/  * @state:	tracked object state
// /*AFLA*/  * @astate:	current active state
// /*AFLA*/  * @object:	pointer to the real object
// /*AFLA*/  * @descr:	pointer to an object type specific debug description structure
// /*AFLA*/  */
// /*AFLA*/ struct debug_obj {
// /*AFLA*/ 	struct hlist_node	node;
// /*AFLA*/ 	enum debug_obj_state	state;
// /*AFLA*/ 	unsigned int		astate;
// /*AFLA*/ 	void			*object;
// /*AFLA*/ 	struct debug_obj_descr	*descr;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct debug_obj_descr - object type specific debug description structure
// /*AFLA*/  *
// /*AFLA*/  * @name:		name of the object typee
// /*AFLA*/  * @debug_hint:		function returning address, which have associated
// /*AFLA*/  *			kernel symbol, to allow identify the object
// /*AFLA*/  * @is_static_object:	return true if the obj is static, otherwise return false
// /*AFLA*/  * @fixup_init:		fixup function, which is called when the init check
// /*AFLA*/  *			fails. All fixup functions must return true if fixup
// /*AFLA*/  *			was successful, otherwise return false
// /*AFLA*/  * @fixup_activate:	fixup function, which is called when the activate check
// /*AFLA*/  *			fails
// /*AFLA*/  * @fixup_destroy:	fixup function, which is called when the destroy check
// /*AFLA*/  *			fails
// /*AFLA*/  * @fixup_free:		fixup function, which is called when the free check
// /*AFLA*/  *			fails
// /*AFLA*/  * @fixup_assert_init:  fixup function, which is called when the assert_init
// /*AFLA*/  *			check fails
// /*AFLA*/  */
// /*AFLA*/ struct debug_obj_descr {
// /*AFLA*/ 	const char		*name;
// /*AFLA*/ 	void *(*debug_hint)(void *addr);
// /*AFLA*/ 	bool (*is_static_object)(void *addr);
// /*AFLA*/ 	bool (*fixup_init)(void *addr, enum debug_obj_state state);
// /*AFLA*/ 	bool (*fixup_activate)(void *addr, enum debug_obj_state state);
// /*AFLA*/ 	bool (*fixup_destroy)(void *addr, enum debug_obj_state state);
// /*AFLA*/ 	bool (*fixup_free)(void *addr, enum debug_obj_state state);
// /*AFLA*/ 	bool (*fixup_assert_init)(void *addr, enum debug_obj_state state);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_OBJECTS
// /*AFLA*/ extern void debug_object_init      (void *addr, struct debug_obj_descr *descr);
// /*AFLA*/ extern void
// /*AFLA*/ debug_object_init_on_stack(void *addr, struct debug_obj_descr *descr);
// /*AFLA*/ extern int debug_object_activate  (void *addr, struct debug_obj_descr *descr);
// /*AFLA*/ extern void debug_object_deactivate(void *addr, struct debug_obj_descr *descr);
// /*AFLA*/ extern void debug_object_destroy   (void *addr, struct debug_obj_descr *descr);
// /*AFLA*/ extern void debug_object_free      (void *addr, struct debug_obj_descr *descr);
// /*AFLA*/ extern void debug_object_assert_init(void *addr, struct debug_obj_descr *descr);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Active state:
// /*AFLA*/  * - Set at 0 upon initialization.
// /*AFLA*/  * - Must return to 0 before deactivation.
// /*AFLA*/  */
// /*AFLA*/ extern void
// /*AFLA*/ debug_object_active_state(void *addr, struct debug_obj_descr *descr,
// /*AFLA*/ 			  unsigned int expect, unsigned int next);
// /*AFLA*/ 
// /*AFLA*/ extern void debug_objects_early_init(void);
// /*AFLA*/ extern void debug_objects_mem_init(void);
// /*AFLA*/ #else
// /*AFLA*/ static inline void
// /*AFLA*/ debug_object_init      (void *addr, struct debug_obj_descr *descr) { }
// /*AFLA*/ static inline void
// /*AFLA*/ debug_object_init_on_stack(void *addr, struct debug_obj_descr *descr) { }
// /*AFLA*/ static inline int
// /*AFLA*/ debug_object_activate  (void *addr, struct debug_obj_descr *descr) { return 0; }
// /*AFLA*/ static inline void
// /*AFLA*/ debug_object_deactivate(void *addr, struct debug_obj_descr *descr) { }
// /*AFLA*/ static inline void
// /*AFLA*/ debug_object_destroy   (void *addr, struct debug_obj_descr *descr) { }
// /*AFLA*/ static inline void
// /*AFLA*/ debug_object_free      (void *addr, struct debug_obj_descr *descr) { }
// /*AFLA*/ static inline void
// /*AFLA*/ debug_object_assert_init(void *addr, struct debug_obj_descr *descr) { }
// /*AFLA*/ 
// /*AFLA*/ static inline void debug_objects_early_init(void) { }
// /*AFLA*/ static inline void debug_objects_mem_init(void) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_DEBUG_OBJECTS_FREE
// /*AFLA*/ extern void debug_check_no_obj_freed(const void *address, unsigned long size);
// /*AFLA*/ #else
// /*AFLA*/ static inline void
// /*AFLA*/ debug_check_no_obj_freed(const void *address, unsigned long size) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif
