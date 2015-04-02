.section .init
.globl _start
_start:
    # Set up a temporary stack in low-memory. We'll switch to a more permanent
    # stack before calling the kernel.
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

    @ Mask all BCM2835 specific interrupts
    mov r1, #0xFF
    ldr r0, =0x2000B000
    str r1, [r0, #0x21C] ;@ INT_DISABLE_GPU1
    str r1, [r0, #0x220] ;@ INT_DISABLE_GPU2
    str r1, [r0, #0x224] ;@ INT_DISABLE_BASIC

    @ Enable IRQ
    @ 0x53 = (PSR_SVC_MODE|PSR_FIQ_DIS|PSR_IRQ_EN)
    @ TODO: Move this to kernel_main/C
    mov r0, #0x53
    msr cpsr_c, r0

    bl zero_bss

    bl kernel_main


map_kernel_to_higher_memory:
    push {lr}
    # Creates a 16KB page table at 0xC000 (the next 16KB aligned address)
    # The page table identity maps upto 0xC0000000 and mirrors 0x00000000 to
    # 0x40000000 above 0xC0000000.
    ldr r0, =0xC000
    # Identity map upto 0xC0000000
    mov r1, #0x0    ;@ Loop index
    ldr r2, =0x3000 ;@ 0x3000 entries (3/4 of memory)
    ldr r3, =0xC02  ;@ Our flags for each of our page table entries.
    bl _fill_page_table
    # Map above 0xC0000000 to the bottom of memory
    ldr r0, =0xF000 ;@ Base addr in page table (3/4 through - 0xC000+0x3000)
    mov r1, #0x0    ;@ Reset loop index
    ldr r2, =0x1000 ;@ 0x1000 entries (last 1/4 of memory)
    bl _fill_page_table

    # Set TTBR0 to point to the page table
    ldr r0, =0xC000
    mcr p15, 0, r0, c2, c0, 0
    # Set Domain 0 to 'Manager'
    mov r0, #0x3
    mcr p15, 0, r0, c3, c0, 0
    # Turn on the MMU
    mrc p15, 0, r0, c1, c0, 0
    orr r0, r0, #0x1
    mcr p15, 0, r0, c1, c0, 0

    pop {pc}
_fill_page_table:
    lsl r5, r1, #20 ;@ Move index to base_addr location in PTE
    orr r5, r5, r3  ;@ Apply the flags to PTE
    str r5, [r0, r1, LSL #2] ;@ Store the PTE in the PT
    add r1, r1, #1
    cmp r1, r2
    blo _fill_page_table
    mov pc, lr


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
