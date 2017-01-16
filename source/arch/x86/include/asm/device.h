// /*AFLA*/ #ifndef _ASM_X86_DEVICE_H
// /*AFLA*/ #define _ASM_X86_DEVICE_H
// /*AFLA*/ 
// /*AFLA*/ struct dev_archdata {
// /*AFLA*/ #ifdef CONFIG_X86_DEV_DMA_OPS
// /*AFLA*/ 	struct dma_map_ops *dma_ops;
// /*AFLA*/ #endif
// /*AFLA*/ #if defined(CONFIG_INTEL_IOMMU) || defined(CONFIG_AMD_IOMMU)
// /*AFLA*/ 	void *iommu; /* hook for IOMMU specific extension */
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #if defined(CONFIG_X86_DEV_DMA_OPS) && defined(CONFIG_PCI_DOMAINS)
// /*AFLA*/ struct dma_domain {
// /*AFLA*/ 	struct list_head node;
// /*AFLA*/ 	struct dma_map_ops *dma_ops;
// /*AFLA*/ 	int domain_nr;
// /*AFLA*/ };
// /*AFLA*/ void add_dma_domain(struct dma_domain *domain);
// /*AFLA*/ void del_dma_domain(struct dma_domain *domain);
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ struct pdev_archdata {
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif /* _ASM_X86_DEVICE_H */
