#include <k/types.h>
#include "io.h"
#include "init.h"

void init_kernel(void)
{
    init_uart();
    // ...
}

void init_uart(void)
{
    u8 lcr = inb(SERIAL_PORT + 3);
    u16 rate = 38400;
    outb(SERIAL_PORT + 3, lcr | 1 << 7);
    outb(SERIAL_PORT + 0, (115200 / rate) & 0xFF);
    outb(SERIAL_PORT + 1, (115200 / rate) >> 8);
    outb(SERIAL_PORT + 3, lcr & ~(1 << 7));
}


