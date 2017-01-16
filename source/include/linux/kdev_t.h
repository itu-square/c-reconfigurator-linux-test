// /*AFLA*/ #ifndef _LINUX_KDEV_T_H
// /*AFLA*/ #define _LINUX_KDEV_T_H
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/linux/kdev_t.h>
// /*AFLA*/ 
// /*AFLA*/ #define MINORBITS	20
// /*AFLA*/ #define MINORMASK	((1U << MINORBITS) - 1)
// /*AFLA*/ 
// /*AFLA*/ #define MAJOR(dev)	((unsigned int) ((dev) >> MINORBITS))
// /*AFLA*/ #define MINOR(dev)	((unsigned int) ((dev) & MINORMASK))
// /*AFLA*/ #define MKDEV(ma,mi)	(((ma) << MINORBITS) | (mi))
// /*AFLA*/ 
// /*AFLA*/ #define print_dev_t(buffer, dev)					\
// /*AFLA*/ 	sprintf((buffer), "%u:%u\n", MAJOR(dev), MINOR(dev))
// /*AFLA*/ 
// /*AFLA*/ #define format_dev_t(buffer, dev)					\
// /*AFLA*/ 	({								\
// /*AFLA*/ 		sprintf(buffer, "%u:%u", MAJOR(dev), MINOR(dev));	\
// /*AFLA*/ 		buffer;							\
// /*AFLA*/ 	})
// /*AFLA*/ 
// /*AFLA*/ /* acceptable for old filesystems */
// /*AFLA*/ static inline bool old_valid_dev(dev_t dev)
// /*AFLA*/ {
// /*AFLA*/ 	return MAJOR(dev) < 256 && MINOR(dev) < 256;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u16 old_encode_dev(dev_t dev)
// /*AFLA*/ {
// /*AFLA*/ 	return (MAJOR(dev) << 8) | MINOR(dev);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline dev_t old_decode_dev(u16 val)
// /*AFLA*/ {
// /*AFLA*/ 	return MKDEV((val >> 8) & 255, val & 255);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u32 new_encode_dev(dev_t dev)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned major = MAJOR(dev);
// /*AFLA*/ 	unsigned minor = MINOR(dev);
// /*AFLA*/ 	return (minor & 0xff) | (major << 8) | ((minor & ~0xff) << 12);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline dev_t new_decode_dev(u32 dev)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned major = (dev & 0xfff00) >> 8;
// /*AFLA*/ 	unsigned minor = (dev & 0xff) | ((dev >> 12) & 0xfff00);
// /*AFLA*/ 	return MKDEV(major, minor);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u64 huge_encode_dev(dev_t dev)
// /*AFLA*/ {
// /*AFLA*/ 	return new_encode_dev(dev);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline dev_t huge_decode_dev(u64 dev)
// /*AFLA*/ {
// /*AFLA*/ 	return new_decode_dev(dev);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int sysv_valid_dev(dev_t dev)
// /*AFLA*/ {
// /*AFLA*/ 	return MAJOR(dev) < (1<<14) && MINOR(dev) < (1<<18);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline u32 sysv_encode_dev(dev_t dev)
// /*AFLA*/ {
// /*AFLA*/ 	return MINOR(dev) | (MAJOR(dev) << 18);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned sysv_major(u32 dev)
// /*AFLA*/ {
// /*AFLA*/ 	return (dev >> 18) & 0x3fff;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned sysv_minor(u32 dev)
// /*AFLA*/ {
// /*AFLA*/ 	return dev & 0x3ffff;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif
