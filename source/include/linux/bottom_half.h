// /*AFLA*/ #ifndef _LINUX_BH_H
// /*AFLA*/ #define _LINUX_BH_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/preempt.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_TRACE_IRQFLAGS
// /*AFLA*/ extern void __local_bh_disable_ip(unsigned long ip, unsigned int cnt);
// /*AFLA*/ #else
// /*AFLA*/ static __always_inline void __local_bh_disable_ip(unsigned long ip, unsigned int cnt)
// /*AFLA*/ {
// /*AFLA*/ 	preempt_count_add(cnt);
// /*AFLA*/ 	barrier();
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ static inline void local_bh_disable(void)
// /*AFLA*/ {
// /*AFLA*/ 	__local_bh_disable_ip(_THIS_IP_, SOFTIRQ_DISABLE_OFFSET);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ extern void _local_bh_enable(void);
// /*AFLA*/ extern void __local_bh_enable_ip(unsigned long ip, unsigned int cnt);
// /*AFLA*/ 
// /*AFLA*/ static inline void local_bh_enable_ip(unsigned long ip)
// /*AFLA*/ {
// /*AFLA*/ 	__local_bh_enable_ip(ip, SOFTIRQ_DISABLE_OFFSET);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void local_bh_enable(void)
// /*AFLA*/ {
// /*AFLA*/ 	__local_bh_enable_ip(_THIS_IP_, SOFTIRQ_DISABLE_OFFSET);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_BH_H */
