// UART I/O
// Originally taken from http://wiki.osdev.org/ARM_RaspberryPi_Tutorial_C

#include <stdint.h>

#include "uart.h"
#include "mmio.h"
#include "gpio.h"


// delays for at least the given number of cycles
void delay(uint32_t cycles)
{
    asm volatile(
        "__delay_%=: subs %[cycles], %[cycles], #1; bne __delay_%=\n"
        : : [cycles]"r"(cycles) : "cc"
    );
}


void uart_init(void)
{
    // Disable PUD for GPIO 14/15 (see 6.1 of BCM2835)
    mmio_write(GPPUD, 0);
    delay(150);
    mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
    delay(150);
    mmio_write(GPPUD, 0);

    // Clear interrupts
    mmio_write(UART0_ICR, 0x7FF);

    // Set integer & fractional part of baud rate.
    // Divider = UART_CLOCK/(16 * Baud)
    // Fraction part register = (Fractional part * 64) + 0.5
    // UART_CLOCK = 3000000; Baud = 115200.
    // Divider = 3000000/(16 * 115200) = 1.627 = ~1.
    // Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
    mmio_write(UART0_IBRD, 1);
    mmio_write(UART0_FBRD, 40);

    // Enable FIFO & 8 bit data transmissio (1 stop bit, no parity).
    mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

    // Mask all interrupts.
    mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) |
                           (1 << 6) | (1 << 7) | (1 << 8) |
                           (1 << 9) | (1 << 10));

    // Enable UART0, receive & transfer part of UART.
    mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}


void uart_putc(uint8_t byte)
{
    // wait for UART to become ready to transmit
    while (1) {
        if (!(mmio_read(UART0_FR) & (1 << 5))) {
            break;
        }
    }
    mmio_write(UART0_DR, byte);
}


void uart_puts(const char *str)
{
    while (*str) {
        uart_putc(*str++);
    }
}


uint8_t uart_getc()
{
    // wait for UART to have recieved something
    while(1) {
        if (!(mmio_read(UART0_FR) & (1 << 4))) {
            break;
        }
    }
    return mmio_read(UART0_DR);
}
