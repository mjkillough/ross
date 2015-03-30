// GPIO Manipulation

#ifndef __GPIO_H__
#define __GPIO_H__

#include <stdint.h>

#include "mmio.h"


enum {
    GPIO_BASE = 0x7E200000,

    GPFSEL0   = (GPIO_BASE + 0x00),
    GPFSEL1   = (GPIO_BASE + 0x04),
    GPFSEL2   = (GPIO_BASE + 0x08),
    GPFSEL3   = (GPIO_BASE + 0x16),
    GPFSEL4   = (GPIO_BASE + 0x24),

    GPSET0    = (GPIO_BASE + 0x1C),
    GPSET1    = (GPIO_BASE + 0x20),

    GPCLR0    = (GPIO_BASE + 0x28),
    GPCLR1    = (GPIO_BASE + 0x2C),

    // ... skip boring GPIO pins ...

    GPPUD     = (GPIO_BASE + 0x94),
    GPPUDCLK0 = (GPIO_BASE + 0x98),
    GPPUDCLK1 = (GPIO_BASE + 0x9C)
};


static inline void gpio_function_select(char pin, char func)
{
    // yuck, mul
    mmio_write(GPIO_BASE + (pin/10), func << ((pin % 10)*3));
}


static inline void gpio_turn_off(char pin)
{
    uint32_t addr = pin < 32 ? GPCLR0 : GPCLR1;
    mmio_write(addr, 1 << (pin % 32));
}


static inline void gpio_turn_on(char pin)
{
    uint32_t addr = pin < 32 ? GPSET0 : GPSET1;
    mmio_write(addr, 1 << (pin % 32));
}


#endif // __GPIO_H__
