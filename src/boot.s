.section .init
.globl _start
_start:

    @ SP for IRQ mode
    @ 0xD2 = (PSR_IRQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0, #0xD2
    msr cpsr_c, r0
    mov sp, #0x80000

    @ SP for FIQ mode
    @ 0xD1 = (PSR_FIQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0, #0xD1
    msr cpsr_c, r0
    mov sp, #0x4000

    @ SP for SVC mode
    @ 0xD3 = (PSR_SVC_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0, #0xD3
    msr cpsr_c, r0
    mov sp, #0x8000

    bl install_interrupt_vector

    @ Mask all BCM2835 specific interrupts
    mov r1, #0xFF
    ldr r0, =0x2000B000
    str r1, [r0, #0x21C] ;@ INT_DISABLE_GPU1
    str r1, [r0, #0x220] ;@ INT_DISABLE_GPU2
    str r1, [r0, #0x224] ;@ INT_DISABLE_BASIC

    @ Enable IRQ
    @ 0x53 = (PSR_SVC_MODE|PSR_FIQ_EN|PSR_IRQ_DIS)
    @; mov r0, #0x53
    @; msr cpsr_c, r0

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
