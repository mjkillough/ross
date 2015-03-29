SECTIONS {
    /* boot.s goes first. At the very start of this (at 0x8000) there is .init,
     * which is our entry point. We make sure that all of boot.s' sections go
     * next, as these must all be located in the lower section of memory - the
     * rest of the kernel is located high in memory and can only be accessed
     * once we've enabled the MMU.
     */
    .init 0x8000 : {
        *(.init)
    }
    .data.boot : {
        */boot.o(.data)
    }
    .text.boot : {
        */boot.o(.text)
    }
    .rodata.boot : {
        */boot.o(.rodata)
    }
    _boot_end = .;

    /* The rest of the kernel is going to be placed high in virtual memory and
     * can only be accessed after we've enabled paging and mapped it.
     */
    . = 0xc0000000;
    _kernel_start = .;
    .data 0xc0000000 + _boot_end : AT (_boot_end) {
        *(.data)
        *.c.o(*)
    }
    .text : {
        *.c.o(.text)
        *(.text)
    }
    .rodata : {
        *(.rodata)
    }

    /* We remember the location of the BSS region, so that boot.s can zero it
     * after it's enabled the MMU but before it calls into kernel_main().
     */
    .bss : ALIGN(4096) {
        _bss_start = .;
        *(.bss)
        _bss_end = .;
    }

    /* Then everything else, including debug symbols. */
}
