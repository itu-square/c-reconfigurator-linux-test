// /*AFLA*/ #ifndef __LINUX_UACCESS_H__
// /*AFLA*/ #define __LINUX_UACCESS_H__
// /*AFLA*/ 
// /*AFLA*/ #include <linux/sched.h>
// /*AFLA*/ #include <asm/uaccess.h>
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void pagefault_disabled_inc(void)
// /*AFLA*/ {
// /*AFLA*/ 	current->pagefault_disabled++;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static __always_inline void pagefault_disabled_dec(void)
// /*AFLA*/ {
// /*AFLA*/ 	current->pagefault_disabled--;
// /*AFLA*/ 	WARN_ON(current->pagefault_disabled < 0);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * These routines enable/disable the pagefault handler. If disabled, it will
// /*AFLA*/  * not take any locks and go straight to the fixup table.
// /*AFLA*/  *
// /*AFLA*/  * User access methods will not sleep when called from a pagefault_disabled()
// /*AFLA*/  * environment.
// /*AFLA*/  */
// /*AFLA*/ static inline void pagefault_disable(void)
// /*AFLA*/ {
// /*AFLA*/ 	pagefault_disabled_inc();
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * make sure to have issued the store before a pagefault
// /*AFLA*/ 	 * can hit.
// /*AFLA*/ 	 */
// /*AFLA*/ 	barrier();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void pagefault_enable(void)
// /*AFLA*/ {
// /*AFLA*/ 	/*
// /*AFLA*/ 	 * make sure to issue those last loads/stores before enabling
// /*AFLA*/ 	 * the pagefault handler again.
// /*AFLA*/ 	 */
// /*AFLA*/ 	barrier();
// /*AFLA*/ 	pagefault_disabled_dec();
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Is the pagefault handler disabled? If so, user access methods will not sleep.
// /*AFLA*/  */
// /*AFLA*/ #define pagefault_disabled() (current->pagefault_disabled != 0)
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The pagefault handler is in general disabled by pagefault_disable() or
// /*AFLA*/  * when in irq context (via in_atomic()).
// /*AFLA*/  *
// /*AFLA*/  * This function should only be used by the fault handlers. Other users should
// /*AFLA*/  * stick to pagefault_disabled().
// /*AFLA*/  * Please NEVER use preempt_disable() to disable the fault handler. With
// /*AFLA*/  * !CONFIG_PREEMPT_COUNT, this is like a NOP. So the handler won't be disabled.
// /*AFLA*/  * in_atomic() will report different values based on !CONFIG_PREEMPT_COUNT.
// /*AFLA*/  */
// /*AFLA*/ #define faulthandler_disabled() (pagefault_disabled() || in_atomic())
// /*AFLA*/ 
// /*AFLA*/ #ifndef ARCH_HAS_NOCACHE_UACCESS
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long __copy_from_user_inatomic_nocache(void *to,
// /*AFLA*/ 				const void __user *from, unsigned long n)
// /*AFLA*/ {
// /*AFLA*/ 	return __copy_from_user_inatomic(to, from, n);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned long __copy_from_user_nocache(void *to,
// /*AFLA*/ 				const void __user *from, unsigned long n)
// /*AFLA*/ {
// /*AFLA*/ 	return __copy_from_user(to, from, n);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif		/* ARCH_HAS_NOCACHE_UACCESS */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * probe_kernel_read(): safely attempt to read from a location
// /*AFLA*/  * @dst: pointer to the buffer that shall take the data
// /*AFLA*/  * @src: address to read from
// /*AFLA*/  * @size: size of the data chunk
// /*AFLA*/  *
// /*AFLA*/  * Safely read from address @src to the buffer at @dst.  If a kernel fault
// /*AFLA*/  * happens, handle that and return -EFAULT.
// /*AFLA*/  */
// /*AFLA*/ extern long probe_kernel_read(void *dst, const void *src, size_t size);
// /*AFLA*/ extern long __probe_kernel_read(void *dst, const void *src, size_t size);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * probe_kernel_write(): safely attempt to write to a location
// /*AFLA*/  * @dst: address to write to
// /*AFLA*/  * @src: pointer to the data that shall be written
// /*AFLA*/  * @size: size of the data chunk
// /*AFLA*/  *
// /*AFLA*/  * Safely write to address @dst from the buffer at @src.  If a kernel fault
// /*AFLA*/  * happens, handle that and return -EFAULT.
// /*AFLA*/  */
// /*AFLA*/ extern long notrace probe_kernel_write(void *dst, const void *src, size_t size);
// /*AFLA*/ extern long notrace __probe_kernel_write(void *dst, const void *src, size_t size);
// /*AFLA*/ 
// /*AFLA*/ extern long strncpy_from_unsafe(char *dst, const void *unsafe_addr, long count);
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * probe_kernel_address(): safely attempt to read from a location
// /*AFLA*/  * @addr: address to read from
// /*AFLA*/  * @retval: read into this variable
// /*AFLA*/  *
// /*AFLA*/  * Returns 0 on success, or -EFAULT.
// /*AFLA*/  */
// /*AFLA*/ #define probe_kernel_address(addr, retval)		\
// /*AFLA*/ 	probe_kernel_read(&retval, addr, sizeof(retval))
// /*AFLA*/ 
// /*AFLA*/ #ifndef user_access_begin
// /*AFLA*/ #define user_access_begin() do { } while (0)
// /*AFLA*/ #define user_access_end() do { } while (0)
// /*AFLA*/ #define unsafe_get_user(x, ptr, err) do { if (unlikely(__get_user(x, ptr))) goto err; } while (0)
// /*AFLA*/ #define unsafe_put_user(x, ptr, err) do { if (unlikely(__put_user(x, ptr))) goto err; } while (0)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif		/* __LINUX_UACCESS_H__ */
