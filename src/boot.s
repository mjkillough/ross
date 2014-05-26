.section .init
.globl _start
_start:
    mov sp, #0x8000
    bl install_interrupt_vector
    bl kernel_main

halt:
    wfe
    b halt

install_interrupt_vector:
    ldr r0, =interrupt_vector
    mov r1, #0x0000
    ldmia r0!, {r2, r3, r4, r5, r6, r7, r8}
    stmia r1!, {r2, r3, r4, r5, r6, r7, r8}
    mov pc, lr

interrupt_vector:
    b interrupts_isr_reset
    b interrupts_isr_undefined
    b interrupts_isr_swi
    b interrupts_isr_prefetch_abort
    b interrupts_isr_data_abort
    b interrupts_isr_irq
    b interrupts_isr_fiq
