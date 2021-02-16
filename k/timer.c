#include <k/types.h>
#include "isr.h"
#include "io.h"


u32 tick = 0;		//global variable which will be increment
void timer_handler(struct interrupt_context c)
{
	++tick;
}

void init_timer()
{
	u32 div;
	u8 lo, hi;

	/* register our timer callback. */
	register_interrupt_handler(IRQ0, &timer_handler);

	/*
	 * The value we sent to the PIT is the value to divide it's input clock
	 * (1193180 Hz) by, to get our required frequency which is 100.
	 */
	div = 1193180 / 100;

	/* Send the command byte. */
	outb(0x43, 0x36);

	/* Divisor has to be sent byte-wise, so split here into upper/lower
	   bytes. */
	lo = (u8)(div & 0xFF);
	hi = (u8)((div >> 8) & 0xFF);

	// Send the frequency divisor.
	outb(0x40, lo);
	outb(0x40, hi);
	outb(0x20, 0x20);			/* EOI */
}

long gettick()
{
	return tick;
}

