// /*AFLA*/ #ifndef _LINUX_STRING_H_
// /*AFLA*/ #define _LINUX_STRING_H_
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ #include <linux/compiler.h>	/* for inline */
// /*AFLA*/ #include <linux/types.h>	/* for size_t */
// /*AFLA*/ #include <linux/stddef.h>	/* for NULL */
// /*AFLA*/ #include <stdarg.h>
// /*AFLA*/ #include <uapi/linux/string.h>
// /*AFLA*/ 
// /*AFLA*/ extern char *strndup_user(const char __user *, long);
// /*AFLA*/ extern void *memdup_user(const void __user *, size_t);
// /*AFLA*/ extern void *memdup_user_nul(const void __user *, size_t);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Include machine specific inline routines
// /*AFLA*/  */
// /*AFLA*/ #include <asm/string.h>
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_STRCPY
// /*AFLA*/ extern char * strcpy(char *,const char *);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_STRNCPY
// /*AFLA*/ extern char * strncpy(char *,const char *, __kernel_size_t);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_STRLCPY
// /*AFLA*/ size_t strlcpy(char *, const char *, size_t);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_STRSCPY
// /*AFLA*/ ssize_t __must_check strscpy(char *, const char *, size_t);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_STRCAT
// /*AFLA*/ extern char * strcat(char *, const char *);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_STRNCAT
// /*AFLA*/ extern char * strncat(char *, const char *, __kernel_size_t);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_STRLCAT
// /*AFLA*/ extern size_t strlcat(char *, const char *, __kernel_size_t);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_STRCMP
// /*AFLA*/ extern int strcmp(const char *,const char *);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_STRNCMP
// /*AFLA*/ extern int strncmp(const char *,const char *,__kernel_size_t);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_STRCASECMP
// /*AFLA*/ extern int strcasecmp(const char *s1, const char *s2);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_STRNCASECMP
// /*AFLA*/ extern int strncasecmp(const char *s1, const char *s2, size_t n);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_STRCHR
// /*AFLA*/ extern char * strchr(const char *,int);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_STRCHRNUL
// /*AFLA*/ extern char * strchrnul(const char *,int);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_STRNCHR
// /*AFLA*/ extern char * strnchr(const char *, size_t, int);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_STRRCHR
// /*AFLA*/ extern char * strrchr(const char *,int);
// /*AFLA*/ #endif
// /*AFLA*/ extern char * __must_check skip_spaces(const char *);
// /*AFLA*/ 
// /*AFLA*/ extern char *strim(char *);
// /*AFLA*/ 
// /*AFLA*/ static inline __must_check char *strstrip(char *str)
// /*AFLA*/ {
// /*AFLA*/ 	return strim(str);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_STRSTR
// /*AFLA*/ extern char * strstr(const char *, const char *);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_STRNSTR
// /*AFLA*/ extern char * strnstr(const char *, const char *, size_t);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_STRLEN
// /*AFLA*/ extern __kernel_size_t strlen(const char *);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_STRNLEN
// /*AFLA*/ extern __kernel_size_t strnlen(const char *,__kernel_size_t);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_STRPBRK
// /*AFLA*/ extern char * strpbrk(const char *,const char *);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_STRSEP
// /*AFLA*/ extern char * strsep(char **,const char *);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_STRSPN
// /*AFLA*/ extern __kernel_size_t strspn(const char *,const char *);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_STRCSPN
// /*AFLA*/ extern __kernel_size_t strcspn(const char *,const char *);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifndef __HAVE_ARCH_MEMSET
// /*AFLA*/ extern void * memset(void *,int,__kernel_size_t);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_MEMCPY
// /*AFLA*/ extern void * memcpy(void *,const void *,__kernel_size_t);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_MEMMOVE
// /*AFLA*/ extern void * memmove(void *,const void *,__kernel_size_t);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_MEMSCAN
// /*AFLA*/ extern void * memscan(void *,int,__kernel_size_t);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_MEMCMP
// /*AFLA*/ extern int memcmp(const void *,const void *,__kernel_size_t);
// /*AFLA*/ #endif
// /*AFLA*/ #ifndef __HAVE_ARCH_MEMCHR
// /*AFLA*/ extern void * memchr(const void *,int,__kernel_size_t);
// /*AFLA*/ #endif
// /*AFLA*/ void *memchr_inv(const void *s, int c, size_t n);
// /*AFLA*/ char *strreplace(char *s, char old, char new);
// /*AFLA*/ 
// /*AFLA*/ extern void kfree_const(const void *x);
// /*AFLA*/ 
// /*AFLA*/ extern char *kstrdup(const char *s, gfp_t gfp) __malloc;
// /*AFLA*/ extern const char *kstrdup_const(const char *s, gfp_t gfp);
// /*AFLA*/ extern char *kstrndup(const char *s, size_t len, gfp_t gfp);
// /*AFLA*/ extern void *kmemdup(const void *src, size_t len, gfp_t gfp);
// /*AFLA*/ 
// /*AFLA*/ extern char **argv_split(gfp_t gfp, const char *str, int *argcp);
// /*AFLA*/ extern void argv_free(char **argv);
// /*AFLA*/ 
// /*AFLA*/ extern bool sysfs_streq(const char *s1, const char *s2);
// /*AFLA*/ extern int kstrtobool(const char *s, bool *res);
// /*AFLA*/ static inline int strtobool(const char *s, bool *res)
// /*AFLA*/ {
// /*AFLA*/ 	return kstrtobool(s, res);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ int match_string(const char * const *array, size_t n, const char *string);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_BINARY_PRINTF
// /*AFLA*/ int vbin_printf(u32 *bin_buf, size_t size, const char *fmt, va_list args);
// /*AFLA*/ int bstr_printf(char *buf, size_t size, const char *fmt, const u32 *bin_buf);
// /*AFLA*/ int bprintf(u32 *bin_buf, size_t size, const char *fmt, ...) __printf(3, 4);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ extern ssize_t memory_read_from_buffer(void *to, size_t count, loff_t *ppos,
// /*AFLA*/ 				       const void *from, size_t available);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * strstarts - does @str start with @prefix?
// /*AFLA*/  * @str: string to examine
// /*AFLA*/  * @prefix: prefix to look for.
// /*AFLA*/  */
// /*AFLA*/ static inline bool strstarts(const char *str, const char *prefix)
// /*AFLA*/ {
// /*AFLA*/ 	return strncmp(str, prefix, strlen(prefix)) == 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ size_t memweight(const void *ptr, size_t bytes);
// /*AFLA*/ void memzero_explicit(void *s, size_t count);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * kbasename - return the last part of a pathname.
// /*AFLA*/  *
// /*AFLA*/  * @path: path to extract the filename from.
// /*AFLA*/  */
// /*AFLA*/ static inline const char *kbasename(const char *path)
// /*AFLA*/ {
// /*AFLA*/ 	const char *tail = strrchr(path, '/');
// /*AFLA*/ 	return tail ? tail + 1 : path;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_STRING_H_ */
