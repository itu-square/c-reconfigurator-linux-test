// /*AFLA*/ /* Generic associative array implementation.
// /*AFLA*/  *
// /*AFLA*/  * See Documentation/assoc_array.txt for information.
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) 2013 Red Hat, Inc. All Rights Reserved.
// /*AFLA*/  * Written by David Howells (dhowells@redhat.com)
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or
// /*AFLA*/  * modify it under the terms of the GNU General Public Licence
// /*AFLA*/  * as published by the Free Software Foundation; either version
// /*AFLA*/  * 2 of the Licence, or (at your option) any later version.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_ASSOC_ARRAY_H
// /*AFLA*/ #define _LINUX_ASSOC_ARRAY_H
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_ASSOCIATIVE_ARRAY
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ #define ASSOC_ARRAY_KEY_CHUNK_SIZE BITS_PER_LONG /* Key data retrieved in chunks of this size */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Generic associative array.
// /*AFLA*/  */
// /*AFLA*/ struct assoc_array {
// /*AFLA*/ 	struct assoc_array_ptr	*root;		/* The node at the root of the tree */
// /*AFLA*/ 	unsigned long		nr_leaves_on_tree;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Operations on objects and index keys for use by array manipulation routines.
// /*AFLA*/  */
// /*AFLA*/ struct assoc_array_ops {
// /*AFLA*/ 	/* Method to get a chunk of an index key from caller-supplied data */
// /*AFLA*/ 	unsigned long (*get_key_chunk)(const void *index_key, int level);
// /*AFLA*/ 
// /*AFLA*/ 	/* Method to get a piece of an object's index key */
// /*AFLA*/ 	unsigned long (*get_object_key_chunk)(const void *object, int level);
// /*AFLA*/ 
// /*AFLA*/ 	/* Is this the object we're looking for? */
// /*AFLA*/ 	bool (*compare_object)(const void *object, const void *index_key);
// /*AFLA*/ 
// /*AFLA*/ 	/* How different is an object from an index key, to a bit position in
// /*AFLA*/ 	 * their keys? (or -1 if they're the same)
// /*AFLA*/ 	 */
// /*AFLA*/ 	int (*diff_objects)(const void *object, const void *index_key);
// /*AFLA*/ 
// /*AFLA*/ 	/* Method to free an object. */
// /*AFLA*/ 	void (*free_object)(void *object);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Access and manipulation functions.
// /*AFLA*/  */
// /*AFLA*/ struct assoc_array_edit;
// /*AFLA*/ 
// /*AFLA*/ static inline void assoc_array_init(struct assoc_array *array)
// /*AFLA*/ {
// /*AFLA*/ 	array->root = NULL;
// /*AFLA*/ 	array->nr_leaves_on_tree = 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern int assoc_array_iterate(const struct assoc_array *array,
// /*AFLA*/ 			       int (*iterator)(const void *object,
// /*AFLA*/ 					       void *iterator_data),
// /*AFLA*/ 			       void *iterator_data);
// /*AFLA*/ extern void *assoc_array_find(const struct assoc_array *array,
// /*AFLA*/ 			      const struct assoc_array_ops *ops,
// /*AFLA*/ 			      const void *index_key);
// /*AFLA*/ extern void assoc_array_destroy(struct assoc_array *array,
// /*AFLA*/ 				const struct assoc_array_ops *ops);
// /*AFLA*/ extern struct assoc_array_edit *assoc_array_insert(struct assoc_array *array,
// /*AFLA*/ 						   const struct assoc_array_ops *ops,
// /*AFLA*/ 						   const void *index_key,
// /*AFLA*/ 						   void *object);
// /*AFLA*/ extern void assoc_array_insert_set_object(struct assoc_array_edit *edit,
// /*AFLA*/ 					  void *object);
// /*AFLA*/ extern struct assoc_array_edit *assoc_array_delete(struct assoc_array *array,
// /*AFLA*/ 						   const struct assoc_array_ops *ops,
// /*AFLA*/ 						   const void *index_key);
// /*AFLA*/ extern struct assoc_array_edit *assoc_array_clear(struct assoc_array *array,
// /*AFLA*/ 						  const struct assoc_array_ops *ops);
// /*AFLA*/ extern void assoc_array_apply_edit(struct assoc_array_edit *edit);
// /*AFLA*/ extern void assoc_array_cancel_edit(struct assoc_array_edit *edit);
// /*AFLA*/ extern int assoc_array_gc(struct assoc_array *array,
// /*AFLA*/ 			  const struct assoc_array_ops *ops,
// /*AFLA*/ 			  bool (*iterator)(void *object, void *iterator_data),
// /*AFLA*/ 			  void *iterator_data);
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_ASSOCIATIVE_ARRAY */
// /*AFLA*/ #endif /* _LINUX_ASSOC_ARRAY_H */
