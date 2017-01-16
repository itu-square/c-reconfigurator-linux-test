// /*AFLA*/ #ifndef _LINUX_IRQRETURN_H
// /*AFLA*/ #define _LINUX_IRQRETURN_H
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * enum irqreturn
// /*AFLA*/  * @IRQ_NONE		interrupt was not from this device or was not handled
// /*AFLA*/  * @IRQ_HANDLED		interrupt was handled by this device
// /*AFLA*/  * @IRQ_WAKE_THREAD	handler requests to wake the handler thread
// /*AFLA*/  */
// /*AFLA*/ enum irqreturn {
// /*AFLA*/ 	IRQ_NONE		= (0 << 0),
// /*AFLA*/ 	IRQ_HANDLED		= (1 << 0),
// /*AFLA*/ 	IRQ_WAKE_THREAD		= (1 << 1),
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ typedef enum irqreturn irqreturn_t;
// /*AFLA*/ #define IRQ_RETVAL(x)	((x) ? IRQ_HANDLED : IRQ_NONE)
// /*AFLA*/ 
// /*AFLA*/ #endif
