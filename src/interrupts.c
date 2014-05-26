#include "interrupts.h"
#include "uart.h"


void __attribute__ ((interrupt)) interrupts_isr_reset(void)
{
    // We should never actually see this. When the RPi ARM is RESET, the rest
    // of the RPi is reset as well, meaning our interrupt vector is lost.
    uart_puts("interrupts_isr_reset");
}


void __attribute__ ((interrupt ("UNDEF"))) interrupts_isr_undefined(void)
{
    uart_puts("interrupts_isr_undefined");
}


void __attribute__ ((interrupt ("SWI"))) interrupts_isr_swi(void)
{
    uart_puts("interrupts_isr_swi");
}


void __attribute__ ((interrupt ("ABORT"))) interrupts_isr_prefetch_abort(void)
{
    uart_puts("interrupts_isr_prefetch_abort");
}


void __attribute__ ((interrupt ("ABORT"))) interrupts_isr_data_abort(void)
{
    uart_puts("interrupts_isr_data_abort");
}


void __attribute__ ((interrupt ("FIQ"))) interrupts_isr_irq(void)
{
    uart_puts("interrupts_isr_irq");
}


void __attribute__ ((interrupt ("IRQ"))) interrupts_isr_fiq(void)
{
    uart_puts("interrupts_isr_fiq");
}
