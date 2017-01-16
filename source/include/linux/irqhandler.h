// /*AFLA*/ #ifndef _LINUX_IRQHANDLER_H
// /*AFLA*/ #define _LINUX_IRQHANDLER_H
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Interrupt flow handler typedefs are defined here to avoid circular
// /*AFLA*/  * include dependencies.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ struct irq_desc;
// /*AFLA*/ struct irq_data;
// /*AFLA*/ typedef	void (*irq_flow_handler_t)(struct irq_desc *desc);
// /*AFLA*/ typedef	void (*irq_preflow_handler_t)(struct irq_data *data);
// /*AFLA*/ 
// /*AFLA*/ #endif
