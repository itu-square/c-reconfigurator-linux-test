// /*AFLA*/ #ifndef _ASM_X86_HWEIGHT_H
// /*AFLA*/ #define _ASM_X86_HWEIGHT_H
// /*AFLA*/ 
// /*AFLA*/ #include <asm/cpufeatures.h>
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_64BIT
// /*AFLA*/ /* popcnt %edi, %eax */
// /*AFLA*/ #define POPCNT32 ".byte 0xf3,0x0f,0xb8,0xc7"
// /*AFLA*/ /* popcnt %rdi, %rax */
// /*AFLA*/ #define POPCNT64 ".byte 0xf3,0x48,0x0f,0xb8,0xc7"
// /*AFLA*/ #define REG_IN "D"
// /*AFLA*/ #define REG_OUT "a"
// /*AFLA*/ #else
// /*AFLA*/ /* popcnt %eax, %eax */
// /*AFLA*/ #define POPCNT32 ".byte 0xf3,0x0f,0xb8,0xc0"
// /*AFLA*/ #define REG_IN "a"
// /*AFLA*/ #define REG_OUT "a"
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #define __HAVE_ARCH_SW_HWEIGHT
// /*AFLA*/ 
// /*AFLA*/ static __always_inline unsigned int __arch_hweight32(unsigned int w)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned int res;
// /*AFLA*/ 
// /*AFLA*/ 	asm (ALTERNATIVE("call __sw_hweight32", POPCNT32, X86_FEATURE_POPCNT)
// /*AFLA*/ 			 : "="REG_OUT (res)
// /*AFLA*/ 			 : REG_IN (w));
// /*AFLA*/ 
// /*AFLA*/ 	return res;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned int __arch_hweight16(unsigned int w)
// /*AFLA*/ {
// /*AFLA*/ 	return __arch_hweight32(w & 0xffff);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline unsigned int __arch_hweight8(unsigned int w)
// /*AFLA*/ {
// /*AFLA*/ 	return __arch_hweight32(w & 0xff);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_X86_32
// /*AFLA*/ static inline unsigned long __arch_hweight64(__u64 w)
// /*AFLA*/ {
// /*AFLA*/ 	return  __arch_hweight32((u32)w) +
// /*AFLA*/ 		__arch_hweight32((u32)(w >> 32));
// /*AFLA*/ }
// /*AFLA*/ #else
// /*AFLA*/ static __always_inline unsigned long __arch_hweight64(__u64 w)
// /*AFLA*/ {
// /*AFLA*/ 	unsigned long res;
// /*AFLA*/ 
// /*AFLA*/ 	asm (ALTERNATIVE("call __sw_hweight64", POPCNT64, X86_FEATURE_POPCNT)
// /*AFLA*/ 			 : "="REG_OUT (res)
// /*AFLA*/ 			 : REG_IN (w));
// /*AFLA*/ 
// /*AFLA*/ 	return res;
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_X86_32 */
// /*AFLA*/ 
// /*AFLA*/ #endif
