#include "interrupts.h"
#include "io.h"
#include "mmio.h"
#include "uart.h"
#include "timer.h"


void __attribute__ ((interrupt)) interrupts_isr_reset(void)
{
    // We should never actually see this. When the RPi ARM is RESET, the rest
    // of the RPi is reset as well, meaning our interrupt vector is lost.
    kprintf("interrupts_isr_reset\n");
}


void __attribute__ ((interrupt ("UNDEF"))) interrupts_isr_undefined(void)
{
    kprintf("interrupts_isr_undefined\n");
}


void __attribute__ ((interrupt ("SWI"))) interrupts_isr_swi(void)
{
    kprintf("interrupts_isr_swi\n");
}


void __attribute__ ((interrupt ("ABORT"))) interrupts_isr_prefetch_abort(void)
{
    kprintf("interrupts_isr_prefetch_abort\n");
}


void __attribute__ ((interrupt ("ABORT"))) interrupts_isr_data_abort(void)
{
    kprintf("interrupts_isr_data_abort\n");
}


void __attribute__ ((interrupt)) interrupts_isr_unused(void)
{
    // This should never be encoutnered.
    kprintf("interrupts_isr_unused: unreachable code reached\n");
}


void __attribute__ ((interrupt ("IRQ"))) interrupts_isr_irq(void)
{
    // Timer?
    if (timer_interrupt_pending()) {
        timer_interrupt_clear();
        kprintf("interrupts_isr_irq: timer\n");
        return;
    }

    // UARTINT?
    if (mmio_read(INT_PENDING_GPU2) & (1 << 25)) {
        uint8_t c;
        if (mmio_read(UART0_MIS) & (1 << 4)) {
            c = uart_getc(); // read to clear the interrupt
        }
        kprintf("interrupts_isr_irq: uart received %p\n", (uint32_t)c);
        return;
    }
}


void __attribute__ ((interrupt ("FIQ"))) interrupts_isr_fiq(void)
{
    kprintf("interrupts_isr_fiq\n");
}
