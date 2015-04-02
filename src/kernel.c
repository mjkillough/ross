#include <stdint.h>

#include "io.h"
#include "gpio.h"
#include "uart.h"
#include "interrupts.h"
#include "mmu.h"
#include "timer.h"


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
    page_table_t page_table = mmu_init();
    uart_init();

    kprintf("Ross says Hello!\n");

    // Mask all BCM2835 interrupts
    mmio_write(INT_DISABLE_GPU1, 0xFF);
    mmio_write(INT_DISABLE_GPU2, 0xFF);
    mmio_write(INT_DISABLE_BASIC, 0xFF);
    // Enable UARTINT, GPU IRQ 57
    mmio_write(INT_ENABLE_GPU2, 1 << 25);

    // Enable IRQ/FIQ on ARM
    interrupts_enable();

    timer_init();
    timer_set_value(0xFFFF);
    timer_enable_interrupt();
    timer_enable();

    while(1) {
        sleep(); sleep(); sleep();
    }
}
