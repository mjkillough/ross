#ifndef __PANIC_H__
#define __PANIC_H__

#define panic(msg)                          \
    do {                                    \
        kprintf("panic: " #msg "\n");       \
        while (1)                           \
            0;                              \
    } while (0);                            \

#endif
