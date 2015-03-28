/******************************************************************************
*   kernel.ld
*    by Alex Chadwick
*
*   A linker script for generation of raspberry pi kernel images, with C
*   code.
******************************************************************************/

SECTIONS {
    /*
     * First and formost we need the .init section, containing the code to
     * be run first. We allow room for the ATAGs and stack and conform to
     * the bootloader's expectation by putting this code at 0x8000.
     */
    .init 0x8000 : {
        *(.init)
    }

    /*
    * Next we put the data.
    */
    .data : {
        *(.data)
        *.c.o(*)
    }

    /*
     * Next we put the rest of the code.
     */
    .text : {
        *.c.o(.text)
        *(.text)
    }

    .rodata : {
        *(.rodata)
    }

    __bss_start = .;
    .bss :
    {
        bss = .;
        *(.bss)
    }
    . = ALIGN(4096); /* align to page size */
    __bss_end = .;

    /* Then everything else, including debug symbols. */
}
