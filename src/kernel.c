#include <stdint.h>

#include "io.h"
#include "gpio.h"
#include "uart.h"
#include "interrupts.h"


void flash_act_led(void)
{
    gpio_function_select(16, 1);
    char flip = 1;
    while (1) {
        // poor man's sleep
        for (uint32_t i = 0x3F0000; i > 0; i--) ;

        if (flip) {
            gpio_turn_off(16);
        } else {
            gpio_turn_on(16);
        }

        flip = flip ^ 1;
    }
}


void kernel_main(void)
{
    uart_init();

    kprintf("Ross says Hello!\n");
    kprintf("%%p = %p\n", 0xDEADB33F);

    // Enable UARTINT, GPU IRQ 57
    mmio_write(INT_ENABLE_GPU2, 1 << 25);

    for (uint32_t i = 0; i < 2*8*4; i += 4) {
        kprintf("%p = %p\n", i, mmio_read(i));
    }

    while (1) {
        for (uint32_t i = 0x3F0000; i > 0; i--) ;

        kprintf("UART0_IMSC = %p\n", mmio_read(UART0_IMSC));
        kprintf("UART0_RIS = %p\n", mmio_read(UART0_RIS));
        kprintf("UART0_MIS = %p\n", mmio_read(UART0_MIS));
        kprintf("INT_PENDING_BASIC = %p\n", mmio_read(INT_PENDING_BASIC));
        kprintf("INT_PENDING_GPU1 = %p\n", mmio_read(INT_PENDING_GPU1));
        kprintf("INT_PENDING_GPU2 = %p\n", mmio_read(INT_PENDING_GPU2));
    }
}
