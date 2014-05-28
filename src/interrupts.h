#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__


enum {
    INT_BASE = 0x2000B000,

    INT_PENDING_BASIC = (INT_BASE + 0x200),
    INT_PENDING_GPU1  = (INT_BASE + 0x204),
    INT_PENDING_GPU2  = (INT_BASE + 0x208),
    INT_FIQ_CONTROL   = (INT_BASE + 0x20C),
    INT_ENABLE_GPU1   = (INT_BASE + 0x210),
    INT_ENABLE_GPU2   = (INT_BASE + 0x214),
    INT_ENABLE_BASIC  = (INT_BASE + 0x218),
    INT_DISABLE_GPU1  = (INT_BASE + 0x21C),
    INT_DISABLE_GPU2  = (INT_BASE + 0x220),
    INT_DISABLE_BASIC = (INT_BASE + 0x224),
};


void interrupts_enable(void);
// void interrupts_disable(void);


void __attribute__ ((interrupt)) interrupts_isr_reset(void);
void __attribute__ ((interrupt ("UNDEF"))) interrupts_isr_undefined(void);
void __attribute__ ((interrupt ("SWI"))) interrupts_isr_swi(void);
void __attribute__ ((interrupt ("ABORT"))) interrupts_isr_prefetch_abort(void);
void __attribute__ ((interrupt ("ABORT"))) interrupts_isr_data_abort(void);
void __attribute__ ((interrupt)) interrupts_isr_unused(void);
void __attribute__ ((interrupt ("FIQ"))) interrupts_isr_irq(void);
void __attribute__ ((interrupt ("IRQ"))) interrupts_isr_fiq(void);

#endif // __INTERRUPTS_H__
