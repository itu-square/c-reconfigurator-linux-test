// /*AFLA*/ #ifndef _LINUX_PATH_H
// /*AFLA*/ #define _LINUX_PATH_H
// /*AFLA*/ 
// /*AFLA*/ struct dentry;
// /*AFLA*/ struct vfsmount;
// /*AFLA*/ 
// /*AFLA*/ struct path {
// /*AFLA*/ 	struct vfsmount *mnt;
// /*AFLA*/ 	struct dentry *dentry;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern void path_get(const struct path *);
// /*AFLA*/ extern void path_put(const struct path *);
// /*AFLA*/ 
// /*AFLA*/ static inline int path_equal(const struct path *path1, const struct path *path2)
// /*AFLA*/ {
// /*AFLA*/ 	return path1->mnt == path2->mnt && path1->dentry == path2->dentry;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif  /* _LINUX_PATH_H */
