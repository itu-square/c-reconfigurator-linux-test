// /*AFLA*/ #ifndef _LINUX_BYTEORDER_GENERIC_H
// /*AFLA*/ #define _LINUX_BYTEORDER_GENERIC_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * linux/byteorder/generic.h
// /*AFLA*/  * Generic Byte-reordering support
// /*AFLA*/  *
// /*AFLA*/  * The "... p" macros, like le64_to_cpup, can be used with pointers
// /*AFLA*/  * to unaligned data, but there will be a performance penalty on 
// /*AFLA*/  * some architectures.  Use get_unaligned for unaligned data.
// /*AFLA*/  *
// /*AFLA*/  * Francois-Rene Rideau <fare@tunes.org> 19970707
// /*AFLA*/  *    gathered all the good ideas from all asm-foo/byteorder.h into one file,
// /*AFLA*/  *    cleaned them up.
// /*AFLA*/  *    I hope it is compliant with non-GCC compilers.
// /*AFLA*/  *    I decided to put __BYTEORDER_HAS_U64__ in byteorder.h,
// /*AFLA*/  *    because I wasn't sure it would be ok to put it in types.h
// /*AFLA*/  *    Upgraded it to 2.1.43
// /*AFLA*/  * Francois-Rene Rideau <fare@tunes.org> 19971012
// /*AFLA*/  *    Upgraded it to 2.1.57
// /*AFLA*/  *    to please Linus T., replaced huge #ifdef's between little/big endian
// /*AFLA*/  *    by nestedly #include'd files.
// /*AFLA*/  * Francois-Rene Rideau <fare@tunes.org> 19971205
// /*AFLA*/  *    Made it to 2.1.71; now a facelift:
// /*AFLA*/  *    Put files under include/linux/byteorder/
// /*AFLA*/  *    Split swab from generic support.
// /*AFLA*/  *
// /*AFLA*/  * TODO:
// /*AFLA*/  *   = Regular kernel maintainers could also replace all these manual
// /*AFLA*/  *    byteswap macros that remain, disseminated among drivers,
// /*AFLA*/  *    after some grep or the sources...
// /*AFLA*/  *   = Linus might want to rename all these macros and files to fit his taste,
// /*AFLA*/  *    to fit his personal naming scheme.
// /*AFLA*/  *   = it seems that a few drivers would also appreciate
// /*AFLA*/  *    nybble swapping support...
// /*AFLA*/  *   = every architecture could add their byteswap macro in asm/byteorder.h
// /*AFLA*/  *    see how some architectures already do (i386, alpha, ppc, etc)
// /*AFLA*/  *   = cpu_to_beXX and beXX_to_cpu might some day need to be well
// /*AFLA*/  *    distinguished throughout the kernel. This is not the case currently,
// /*AFLA*/  *    since little endian, big endian, and pdp endian machines needn't it.
// /*AFLA*/  *    But this might be the case for, say, a port of Linux to 20/21 bit
// /*AFLA*/  *    architectures (and F21 Linux addict around?).
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The following macros are to be defined by <asm/byteorder.h>:
// /*AFLA*/  *
// /*AFLA*/  * Conversion of long and short int between network and host format
// /*AFLA*/  *	ntohl(__u32 x)
// /*AFLA*/  *	ntohs(__u16 x)
// /*AFLA*/  *	htonl(__u32 x)
// /*AFLA*/  *	htons(__u16 x)
// /*AFLA*/  * It seems that some programs (which? where? or perhaps a standard? POSIX?)
// /*AFLA*/  * might like the above to be functions, not macros (why?).
// /*AFLA*/  * if that's true, then detect them, and take measures.
// /*AFLA*/  * Anyway, the measure is: define only ___ntohl as a macro instead,
// /*AFLA*/  * and in a separate file, have
// /*AFLA*/  * unsigned long inline ntohl(x){return ___ntohl(x);}
// /*AFLA*/  *
// /*AFLA*/  * The same for constant arguments
// /*AFLA*/  *	__constant_ntohl(__u32 x)
// /*AFLA*/  *	__constant_ntohs(__u16 x)
// /*AFLA*/  *	__constant_htonl(__u32 x)
// /*AFLA*/  *	__constant_htons(__u16 x)
// /*AFLA*/  *
// /*AFLA*/  * Conversion of XX-bit integers (16- 32- or 64-)
// /*AFLA*/  * between native CPU format and little/big endian format
// /*AFLA*/  * 64-bit stuff only defined for proper architectures
// /*AFLA*/  *	cpu_to_[bl]eXX(__uXX x)
// /*AFLA*/  *	[bl]eXX_to_cpu(__uXX x)
// /*AFLA*/  *
// /*AFLA*/  * The same, but takes a pointer to the value to convert
// /*AFLA*/  *	cpu_to_[bl]eXXp(__uXX x)
// /*AFLA*/  *	[bl]eXX_to_cpup(__uXX x)
// /*AFLA*/  *
// /*AFLA*/  * The same, but change in situ
// /*AFLA*/  *	cpu_to_[bl]eXXs(__uXX x)
// /*AFLA*/  *	[bl]eXX_to_cpus(__uXX x)
// /*AFLA*/  *
// /*AFLA*/  * See asm-foo/byteorder.h for examples of how to provide
// /*AFLA*/  * architecture-optimized versions
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define cpu_to_le64 __cpu_to_le64
// /*AFLA*/ #define le64_to_cpu __le64_to_cpu
// /*AFLA*/ #define cpu_to_le32 __cpu_to_le32
// /*AFLA*/ #define le32_to_cpu __le32_to_cpu
// /*AFLA*/ #define cpu_to_le16 __cpu_to_le16
// /*AFLA*/ #define le16_to_cpu __le16_to_cpu
// /*AFLA*/ #define cpu_to_be64 __cpu_to_be64
// /*AFLA*/ #define be64_to_cpu __be64_to_cpu
// /*AFLA*/ #define cpu_to_be32 __cpu_to_be32
// /*AFLA*/ #define be32_to_cpu __be32_to_cpu
// /*AFLA*/ #define cpu_to_be16 __cpu_to_be16
// /*AFLA*/ #define be16_to_cpu __be16_to_cpu
// /*AFLA*/ #define cpu_to_le64p __cpu_to_le64p
// /*AFLA*/ #define le64_to_cpup __le64_to_cpup
// /*AFLA*/ #define cpu_to_le32p __cpu_to_le32p
// /*AFLA*/ #define le32_to_cpup __le32_to_cpup
// /*AFLA*/ #define cpu_to_le16p __cpu_to_le16p
// /*AFLA*/ #define le16_to_cpup __le16_to_cpup
// /*AFLA*/ #define cpu_to_be64p __cpu_to_be64p
// /*AFLA*/ #define be64_to_cpup __be64_to_cpup
// /*AFLA*/ #define cpu_to_be32p __cpu_to_be32p
// /*AFLA*/ #define be32_to_cpup __be32_to_cpup
// /*AFLA*/ #define cpu_to_be16p __cpu_to_be16p
// /*AFLA*/ #define be16_to_cpup __be16_to_cpup
// /*AFLA*/ #define cpu_to_le64s __cpu_to_le64s
// /*AFLA*/ #define le64_to_cpus __le64_to_cpus
// /*AFLA*/ #define cpu_to_le32s __cpu_to_le32s
// /*AFLA*/ #define le32_to_cpus __le32_to_cpus
// /*AFLA*/ #define cpu_to_le16s __cpu_to_le16s
// /*AFLA*/ #define le16_to_cpus __le16_to_cpus
// /*AFLA*/ #define cpu_to_be64s __cpu_to_be64s
// /*AFLA*/ #define be64_to_cpus __be64_to_cpus
// /*AFLA*/ #define cpu_to_be32s __cpu_to_be32s
// /*AFLA*/ #define be32_to_cpus __be32_to_cpus
// /*AFLA*/ #define cpu_to_be16s __cpu_to_be16s
// /*AFLA*/ #define be16_to_cpus __be16_to_cpus
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * They have to be macros in order to do the constant folding
// /*AFLA*/  * correctly - if the argument passed into a inline function
// /*AFLA*/  * it is no longer constant according to gcc..
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #undef ntohl
// /*AFLA*/ #undef ntohs
// /*AFLA*/ #undef htonl
// /*AFLA*/ #undef htons
// /*AFLA*/ 
// /*AFLA*/ #define ___htonl(x) __cpu_to_be32(x)
// /*AFLA*/ #define ___htons(x) __cpu_to_be16(x)
// /*AFLA*/ #define ___ntohl(x) __be32_to_cpu(x)
// /*AFLA*/ #define ___ntohs(x) __be16_to_cpu(x)
// /*AFLA*/ 
// /*AFLA*/ #define htonl(x) ___htonl(x)
// /*AFLA*/ #define ntohl(x) ___ntohl(x)
// /*AFLA*/ #define htons(x) ___htons(x)
// /*AFLA*/ #define ntohs(x) ___ntohs(x)
// /*AFLA*/ 
// /*AFLA*/ static inline void le16_add_cpu(__le16 *var, u16 val)
// /*AFLA*/ {
// /*AFLA*/ 	*var = cpu_to_le16(le16_to_cpu(*var) + val);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void le32_add_cpu(__le32 *var, u32 val)
// /*AFLA*/ {
// /*AFLA*/ 	*var = cpu_to_le32(le32_to_cpu(*var) + val);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void le64_add_cpu(__le64 *var, u64 val)
// /*AFLA*/ {
// /*AFLA*/ 	*var = cpu_to_le64(le64_to_cpu(*var) + val);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void be16_add_cpu(__be16 *var, u16 val)
// /*AFLA*/ {
// /*AFLA*/ 	*var = cpu_to_be16(be16_to_cpu(*var) + val);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void be32_add_cpu(__be32 *var, u32 val)
// /*AFLA*/ {
// /*AFLA*/ 	*var = cpu_to_be32(be32_to_cpu(*var) + val);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void be64_add_cpu(__be64 *var, u64 val)
// /*AFLA*/ {
// /*AFLA*/ 	*var = cpu_to_be64(be64_to_cpu(*var) + val);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_BYTEORDER_GENERIC_H */
