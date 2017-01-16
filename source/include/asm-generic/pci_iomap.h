// /*AFLA*/ /* Generic I/O port emulation, based on MN10300 code
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
// /*AFLA*/  * Written by David Howells (dhowells@redhat.com)
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or
// /*AFLA*/  * modify it under the terms of the GNU General Public Licence
// /*AFLA*/  * as published by the Free Software Foundation; either version
// /*AFLA*/  * 2 of the Licence, or (at your option) any later version.
// /*AFLA*/  */
// /*AFLA*/ #ifndef __ASM_GENERIC_PCI_IOMAP_H
// /*AFLA*/ #define __ASM_GENERIC_PCI_IOMAP_H
// /*AFLA*/ 
// /*AFLA*/ struct pci_dev;
// /*AFLA*/ #ifdef CONFIG_PCI
// /*AFLA*/ /* Create a virtual mapping cookie for a PCI BAR (memory or IO) */
// /*AFLA*/ extern void __iomem *pci_iomap(struct pci_dev *dev, int bar, unsigned long max);
// /*AFLA*/ extern void __iomem *pci_iomap_wc(struct pci_dev *dev, int bar, unsigned long max);
// /*AFLA*/ extern void __iomem *pci_iomap_range(struct pci_dev *dev, int bar,
// /*AFLA*/ 				     unsigned long offset,
// /*AFLA*/ 				     unsigned long maxlen);
// /*AFLA*/ extern void __iomem *pci_iomap_wc_range(struct pci_dev *dev, int bar,
// /*AFLA*/ 					unsigned long offset,
// /*AFLA*/ 					unsigned long maxlen);
// /*AFLA*/ /* Create a virtual mapping cookie for a port on a given PCI device.
// /*AFLA*/  * Do not call this directly, it exists to make it easier for architectures
// /*AFLA*/  * to override */
// /*AFLA*/ #ifdef CONFIG_NO_GENERIC_PCI_IOPORT_MAP
// /*AFLA*/ extern void __iomem *__pci_ioport_map(struct pci_dev *dev, unsigned long port,
// /*AFLA*/ 				      unsigned int nr);
// /*AFLA*/ #else
// /*AFLA*/ #define __pci_ioport_map(dev, port, nr) ioport_map((port), (nr))
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #elif defined(CONFIG_GENERIC_PCI_IOMAP)
// /*AFLA*/ static inline void __iomem *pci_iomap(struct pci_dev *dev, int bar, unsigned long max)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __iomem *pci_iomap_wc(struct pci_dev *dev, int bar, unsigned long max)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ static inline void __iomem *pci_iomap_range(struct pci_dev *dev, int bar,
// /*AFLA*/ 					    unsigned long offset,
// /*AFLA*/ 					    unsigned long maxlen)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ static inline void __iomem *pci_iomap_wc_range(struct pci_dev *dev, int bar,
// /*AFLA*/ 					       unsigned long offset,
// /*AFLA*/ 					       unsigned long maxlen)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #endif /* __ASM_GENERIC_IO_H */
