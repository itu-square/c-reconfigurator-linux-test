// /*AFLA*/ #ifndef _LINUX_KASAN_CHECKS_H
// /*AFLA*/ #define _LINUX_KASAN_CHECKS_H
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_KASAN
// /*AFLA*/ void kasan_check_read(const void *p, unsigned int size);
// /*AFLA*/ void kasan_check_write(const void *p, unsigned int size);
// /*AFLA*/ #else
// /*AFLA*/ static inline void kasan_check_read(const void *p, unsigned int size) { }
// /*AFLA*/ static inline void kasan_check_write(const void *p, unsigned int size) { }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif
