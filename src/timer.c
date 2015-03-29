// Timer (ARM Side)

#include <stdint.h>

#include "timer.h"
#include "mmio.h"
#include "interrupts.h"


void timer_init()
{
    // Enable the interrupt on the timer. Our interrupt_enable/_disable()
    // functions will mask it in INT_ENABLE_BASIC (on the BCM2935)
    uint32_t control = mmio_read(TIMER_CONTROL);
    control |= 0x1 << 5;
    mmio_write(TIMER_CONTROL, control);
}


void timer_enable()
{
    uint32_t control = mmio_read(TIMER_CONTROL);
    control |= 0x1 << 7;
    mmio_write(TIMER_CONTROL, control);
}


void timer_disable()
{
    uint32_t control = mmio_read(TIMER_CONTROL);
    control &= 0xFF7F; // clear bit 7
    mmio_write(TIMER_CONTROL, control);
}


void timer_enable_interrupt()
{
    // Enable the interrupt in the BCM2835. We enable it in TIMER_CONTROL in
    // timer_init() and then leave it enabled.
    mmio_write(INT_ENABLE_BASIC, 1);
}

void timer_disable_interrupt()
{
    // Mask it in the BCM2835, but leave it enabled in TIMER_CONTROL.
    mmio_write(INT_DISABLE_BASIC, 1);
}


void timer_set_value(uint16_t value)
{
    mmio_write(TIMER_LOAD, value);
}


uint16_t timer_get_value()
{
    return mmio_read(TIMER_VALUE);
}


uint8_t timer_interrupt_pending()
{
    // Ignore the interrupt pending register of the timer and instead check
    // the flag in the BCM2835.
    return mmio_read(INT_PENDING_BASIC) & 1;
}


void timer_interrupt_clear()
{
    mmio_write(TIMER_IRQ_ACK, 0xFFFF);
}
