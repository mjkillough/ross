;@ Must be 32 byte aligned.
.globl interrupt_vector
.p2align 5
interrupt_vector:
    ldr pc, =interrupts_isr_reset
    ldr pc, =interrupts_isr_undefined
    ldr pc, =interrupts_isr_swi
    ldr pc, =interrupts_isr_prefetch_abort
    ldr pc, =interrupts_isr_data_abort
    ldr pc, =interrupts_isr_unused
    ldr pc, =interrupts_isr_irq
    ldr pc, =interrupts_isr_fiq
