#include "keyboard.h"
#include "io.h"
#include "isr.h"

void init_keyboard(void)
{
    register_interrupt_handler(IRQ1, &getkey);
}

int getkey()
{
    outb(0x20, 0x20);
    if (inb(KEYBOARD_STATUS_PORT) & 0x01) {
        int keycode = inb(KEYBOARD_DATA_PORT);
        char c = key_dictionnary[keycode];
        if (keycode < 0)
            return -1;
        printf("%c", c);
        return c;
    }
    return -1;
}
