// Timer (ARM Side) - refer to BCM2835 doc, Chapter 14.

#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>


enum {
    TIMER_BASE    = 0x7E00B000,

    TIMER_LOAD    = (TIMER_BASE + 0x400),
    TIMER_VALUE   = (TIMER_BASE + 0x404),
    TIMER_CONTROL = (TIMER_BASE + 0x408),
    TIMER_IRQ_ACK = (TIMER_BASE + 0x40C),
    TIMER_IRQ_RAW = (TIMER_BASE + 0x410),
    TIMER_IRQ_MSK = (TIMER_BASE + 0x414),
    TIMER_RELOAD  = (TIMER_BASE + 0x418),
    TIMER_DIVIDER = (TIMER_BASE + 0x41C),
    TIMER_COUNTER = (TIMER_BASE + 0x420),
};


void timer_init();
void timer_enable();
void timer_disable();
void timer_enable_interrupt();
void timer_disable_interrupt();
void timer_set_value(uint16_t value);
uint16_t timer_get_value();
uint8_t timer_interrupt_pending();
void timer_interrupt_clear();


#endif // __TIMER_H__
