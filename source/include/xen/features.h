// /*AFLA*/ /******************************************************************************
// /*AFLA*/  * features.h
// /*AFLA*/  *
// /*AFLA*/  * Query the features reported by Xen.
// /*AFLA*/  *
// /*AFLA*/  * Copyright (c) 2006, Ian Campbell
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef __XEN_FEATURES_H__
// /*AFLA*/ #define __XEN_FEATURES_H__
// /*AFLA*/ 
// /*AFLA*/ #include <xen/interface/features.h>
// /*AFLA*/ 
// /*AFLA*/ void xen_setup_features(void);
// /*AFLA*/ 
// /*AFLA*/ extern u8 xen_features[XENFEAT_NR_SUBMAPS * 32];
// /*AFLA*/ 
// /*AFLA*/ static inline int xen_feature(int flag)
// /*AFLA*/ {
// /*AFLA*/ 	return xen_features[flag];
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASM_XEN_FEATURES_H__ */
