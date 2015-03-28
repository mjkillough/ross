#include <stdint.h>

#include "io.h"
#include "gpio.h"
#include "uart.h"
#include "interrupts.h"


void sleep()
{
    for (uint32_t i = 0x3F0000; i > 0; )
        // Doing this as part of the loop body makes gdb happier when stepping.
        i--;
}


void flash_act_led(void)
{
    gpio_function_select(16, 1);
    char flip = 1;
    while (1) {
        sleep();

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

    int i = 0;
    while (1) {
        kprintf("%p\n", i++);
        sleep();
    }
}
