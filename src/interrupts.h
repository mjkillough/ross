#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

void __attribute__ ((interrupt)) interrupts_isr_reset  (void);
void __attribute__ ((interrupt ("UNDEF"))) interrupts_isr_undefined  (void);
void __attribute__ ((interrupt ("SWI"))) interrupts_isr_swi(void);
void __attribute__ ((interrupt ("ABORT"))) interrupts_isr_prefetch_abort (void);
void __attribute__ ((interrupt ("ABORT"))) interrupts_isr_data_abort (void);
void __attribute__ ((interrupt ("FIQ"))) interrupts_isr_irq(void);
void __attribute__ ((interrupt ("IRQ"))) interrupts_isr_fiq(void);

#endif // __INTERRUPTS_H__
