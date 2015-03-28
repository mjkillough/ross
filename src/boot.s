.section .init
.globl _start
_start:

    @ SP for IRQ mode
    @ 0xD2 = (PSR_IRQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0, #0xD2
    msr cpsr_c, r0
    mov sp, #0x7000

    @ SP for FIQ mode
    @ 0xD1 = (PSR_FIQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0, #0xD1
    msr cpsr_c, r0
    mov sp, #0x6000

    @ SP for ABT mode
    @ 0xD1 = (PSR_ABT_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0, #0xD7
    msr cpsr_c, r0
    mov sp, #0x7000

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
    @ 0x53 = (PSR_SVC_MODE|PSR_FIQ_DIS|PSR_IRQ_EN)
    mov r0, #0x53
    msr cpsr_c, r0

    bl zero_bss

    bl kernel_main


zero_bss:
    ldr r0, =__bss_start
    ldr r1, =__bss_end
    mov r2, #0
    mov r3, #0
    mov r4, #0
    mov r5, #0
_zero_bss_loop:
    stmia r0!, {r2, r3, r4, r5}
    cmp r0, r1
    blo _zero_bss_loop
    mov pc, lr


halt:
    wfe
    b halt


install_interrupt_vector:
    ldr r0, =interrupt_vector
    mov r1, #0x0000
    ldmia r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
    stmia r1!, {r2, r3, r4, r5, r6, r7, r8, r9}
    ldmia r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
    stmia r1!, {r2, r3, r4, r5, r6, r7, r8, r9}
    mov pc, lr

interrupt_vector:
    ldr pc, _addr_interrupts_isr_reset
    ldr pc, _addr_interrupts_isr_undefined
    ldr pc, _addr_interrupts_isr_swi
    ldr pc, _addr_interrupts_isr_prefetch_abort
    ldr pc, _addr_interrupts_isr_data_abort
    ldr pc, _addr_interrupts_isr_unused
    ldr pc, _addr_interrupts_isr_irq
    ldr pc, _addr_interrupts_isr_fiq
;@ Yuck. We need these stubs, so that the addr of the LDRs immediately follow
;@ the LDRs in the code. (We need this as we relocate the interrupt_vector to
;@ 0x00000000 at runtime).
_addr_interrupts_isr_reset:          .word interrupts_isr_reset
_addr_interrupts_isr_undefined:      .word interrupts_isr_undefined
_addr_interrupts_isr_swi:            .word interrupts_isr_swi
_addr_interrupts_isr_prefetch_abort: .word interrupts_isr_prefetch_abort
_addr_interrupts_isr_data_abort:     .word interrupts_isr_data_abort
_addr_interrupts_isr_unused:         .word interrupts_isr_unused
_addr_interrupts_isr_irq:            .word interrupts_isr_irq
_addr_interrupts_isr_fiq:            .word interrupts_isr_fiq
