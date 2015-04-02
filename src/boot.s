.section .init
.globl _start
_start:
    ;@ Set up a temporary stack in low-memory. We'll switch to a more permanent
    ;@ stack before calling the kernel.
    mov sp, #0x8000

    bl map_kernel_to_higher_memory

    @ SP for IRQ mode
    @ 0xD2 = (PSR_IRQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0, #0xD2
    msr cpsr_c, r0
    ldr sp, =0xC0007000

    @ SP for FIQ mode
    @ 0xD1 = (PSR_FIQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0, #0xD1
    msr cpsr_c, r0
    ldr sp, =0xC0006000

    @ SP for ABT mode
    @ 0xD7 = (PSR_ABT_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0, #0xD7
    msr cpsr_c, r0
    ldr sp, =0xC0007000

    @ SP for SVC mode
    @ 0xD3 = (PSR_SVC_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0, #0xD3
    msr cpsr_c, r0
    ldr sp, =0xC0008000

    bl install_interrupt_vector

    bl zero_bss

    bl kernel_main


map_kernel_to_higher_memory:
    push {lr}
    ;@ Creates a 16KB page table at 0xC000 (the next 16KB aligned address).
    ;@ The page table maps the physical 1MB section at 0x0 to 0x0 and 0xC0000000
    ;@ in the virtual address space. (We'll continue to execute at 0x0 and then
    ;@ jump to ~0xC0000000 when we branch to kernel_main(). mmu_init() will
    ;@ setup up a proper page table later).
    ldr r0, =0xC000  ;@ Base of the page table
    ldr r1, =0x3000  ;@ Index for page table entry of 0xC0000000
    ldr r2, =0xC02   ;@ Section page table entry with AP(3) flags and domain 0
    str r2, [r0]     ;@ Map 0x0
    str r2, [r0, r1] ;@ Map 0xC0000000

    ;@ Set TTBR0 to point to the page table
    ldr r0, =0xC000
    mcr p15, 0, r0, c2, c0, 0
    ;@ Set Domain 0 to 'Manager'
    mov r0, #0x3
    mcr p15, 0, r0, c3, c0, 0
    ;@ Turn on the MMU
    mrc p15, 0, r0, c1, c0, 0
    orr r0, r0, #0x1
    mcr p15, 0, r0, c1, c0, 0

    pop {pc}


zero_bss:
    ldr r0, =_bss_start
    ldr r1, =_bss_end
    mov r2, #0
    mov r3, #0
    mov r4, #0
    mov r5, #0
_zero_bss_loop:
    stmia r0!, {r2, r3, r4, r5}
    cmp r0, r1
    blo _zero_bss_loop
    mov pc, lr


;@ The ARM1176JZF-S has Secure Extensions, which means that we can specify a
;@ an interrupt vector, rather than placing it at 0x0 or the high vector addr.
;@ The address of the vector must be 32 byte aligned.
;@ See the TRM section 3.2.43
install_interrupt_vector:
    ldr r0, =interrupt_vector
    MCR p15, 0, r0, c12, c0, 0
    mov pc, lr
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
