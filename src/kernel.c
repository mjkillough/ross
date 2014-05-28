#include <stdint.h>

#include "io.h"
#include "gpio.h"
#include "uart.h"


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
    flash_act_led();
}
