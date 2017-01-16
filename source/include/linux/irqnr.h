// /*AFLA*/ #ifndef _LINUX_IRQNR_H
// /*AFLA*/ #define _LINUX_IRQNR_H
// /*AFLA*/ 
// /*AFLA*/ #include <uapi/linux/irqnr.h>
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ extern int nr_irqs;
// /*AFLA*/ extern struct irq_desc *irq_to_desc(unsigned int irq);
// /*AFLA*/ unsigned int irq_get_next_irq(unsigned int offset);
// /*AFLA*/ 
// /*AFLA*/ # define for_each_irq_desc(irq, desc)					\
// /*AFLA*/ 	for (irq = 0, desc = irq_to_desc(irq); irq < nr_irqs;		\
// /*AFLA*/ 	     irq++, desc = irq_to_desc(irq))				\
// /*AFLA*/ 		if (!desc)						\
// /*AFLA*/ 			;						\
// /*AFLA*/ 		else
// /*AFLA*/ 
// /*AFLA*/ 
// /*AFLA*/ # define for_each_irq_desc_reverse(irq, desc)				\
// /*AFLA*/ 	for (irq = nr_irqs - 1, desc = irq_to_desc(irq); irq >= 0;	\
// /*AFLA*/ 	     irq--, desc = irq_to_desc(irq))				\
// /*AFLA*/ 		if (!desc)						\
// /*AFLA*/ 			;						\
// /*AFLA*/ 		else
// /*AFLA*/ 
// /*AFLA*/ # define for_each_active_irq(irq)			\
// /*AFLA*/ 	for (irq = irq_get_next_irq(0); irq < nr_irqs;	\
// /*AFLA*/ 	     irq = irq_get_next_irq(irq + 1))
// /*AFLA*/ 
// /*AFLA*/ #define for_each_irq_nr(irq)                   \
// /*AFLA*/        for (irq = 0; irq < nr_irqs; irq++)
// /*AFLA*/ 
// /*AFLA*/ #endif
