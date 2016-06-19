#include "interrupts.h"
#include "io.h"
#include "mmio.h"
#include "uart.h"
#include "timer.h"
#include "panic.h"


void interrupts_enable(void)
{
    uint32_t cpsr_c;
    asm("mrs %0, cpsr" : "=r" (cpsr_c));
    // Clear the A (imprecise data aborts), I (IRQ), F(IRQ) bits - [8:6]
    cpsr_c &= 0xE3F;
    asm("msr cpsr_c, %0" : : "r" (cpsr_c));
}


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


// We use a custom wrapper (below) for the _prefetch_abort so that we have
// can return to the same instruction (or HALT). GCC's abort wrapper returns to
// the *next* instruction, which is rarely what we want.
void _interrupts_isr_prefetch_abort(uint32_t faulting_addr)
{
    uint32_t fault_status = 0;
    asm volatile ("mrc p15, 0, %0, c5, c0, 1" : "=r" (fault_status));

    kprintf("interrupts_isr_prefetch_abort: fault_status=%p faulting_addr=%p\n",
        fault_status, faulting_addr
    );

    panic("unhandled prefetch abort");
}
void __attribute__((naked)) interrupts_isr_prefetch_abort(void)
{
    asm volatile ("sub lr, lr, #4");
    asm volatile ("stmfd sp!, {r0-r12, lr}");
    uint32_t faulting_addr;
    asm volatile ("mov %0, lr" : "=r" (faulting_addr));
    _interrupts_isr_prefetch_abort(faulting_addr);
    asm volatile ("ldmfd sp!, {r0-r12, pc}^");
}

// We use a customer wrapper for _data_abort() for the same reasons as
// _prefetch_abort().
void _interrupts_isr_data_abort(uint32_t faulting_addr)
{
    uint32_t fault_status = 0;
    asm volatile ("mrc p15, 0, %0, c5, c0, 0" : "=r" (fault_status));
    uint32_t fault_addr = 0;
    asm volatile ("mrc p15, 0, %0, c6, c0, 0" : "=r" (fault_addr));

    kprintf("interrupts_isr_data_abort: fault_status=%p faulting_addr=%p fault_addr=%p\n",
        fault_status, faulting_addr, fault_addr
    );


    panic("unhandled data abort");
}
void __attribute__((naked)) interrupts_isr_data_abort(void)
{
    asm volatile ("sub lr, lr, #8");
    asm volatile ("stmfd sp!, {r0-r12, lr}");
    uint32_t faulting_addr;
    asm volatile ("mov %0, lr" : "=r" (faulting_addr));
    _interrupts_isr_data_abort(faulting_addr);
    asm volatile ("ldmfd sp!, {r0-r12, pc}^");
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
