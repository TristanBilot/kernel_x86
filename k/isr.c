
#include <k/types.h>
#include "isr.h"
#include "timer.h"
#include "init_tables.h"
#include "io.h"



static const char *exceptions[32] = {
	[0] = "Divide Error",
	[1] = "Debug Exception",
	[2] = "NMI Interrupt",
	[3] = "Breakpoint",
	[4] = "Overflow",
	[5] = "Bound Range Exceeded",
	[6] = "Invalid Opcode",
	[7] = "Device Not Available",
	[8] = "Double Fault",
	[9] = "Coprocessor Segment Overrum",
	[10] = "Invalid TSS",
	[11] = "Segment Not Present",
	[12] = "Stack Segment Fault",
	[13] = "General Protection Fault",
	[14] = "Page Fault",
	[15] = "Unknown interrupt exception",
	[16] = "x87 FPU Fault",
	[17] = "Alignment Check",
	[18] = "Machine Check",
	[19] = "SIMD Floating Point Exception",
	[20] = "Virtualization Exception",
};


static const char *irq_message[16] = {					/*C'était pour les test de print */
	[0] = "System timer : PIT ",
	[1] = "Keyboard controller",
	[2] = "Cascaded signals from IRQ 8 - 15",
	[3] = "Serial port controller for SP 2",   /* SP = SERIAL PORT*/
	[4] = "Serial port controller for SP 1",
	[5] = "Parallel port 2 and 3 or Sound Card",
	[6] = "Floppy disk controller",									/* IRQ 0 - 7 -> MASTER */
	[7] = "Parallel port 1",										/* IRQ 8 - 15 -> SLAVE */
	[8] = "Real-time clock",
	[9] = "ACPI",
	[10] = "The Interrupt is left open for the use of peripherals ",											/* Advanced Configuration and Power Interface ( */
	[11] = "The Interrupt is left open for the use of peripherals ",
	[12] = "Mouse on PS/2 connector",
	[13] = "CPU co-processor  or  integrated floating point unit  or  inter-processor interrupt",
	[14] = "Primary ATA channel",
	[15] = "Secondary ATA channel",
};

isr_h interrupt_handlers[256];


void register_interrupt_handler(u32 n, isr_h handler)
{
	if (n >= SIZE_ARRAY(interrupt_handlers))
		printf("Bad interrupt handler index.");
	interrupt_handlers[n] = handler;
}



void generic_handler(struct interrupt_context *c)
{
    if (c->nb < 32)				/* ISR 0 - 31 */
	{
		if (exceptions[c->nb] != NULL)
			printf("Handled interrupt %d : %s\n", c->nb,exceptions[c->nb]);
		else
			printf("Handled interrupt : Reserved\n");
	}
    else			/* IRQ start to 32 to 47 */
	{
		if (interrupt_handlers[c->nb])
		{
			interrupt_handlers[c->nb](c);		/* On regarde si on a pas register une fonction déja dans le custom handler si cest bon on la lance */
		}
		else
			printf("Unhandled IRQ %d: %s\n", c->nb - 32 , irq_message[c->nb - 32]);		//test

		if (c->nb >= 40)			/* >= 59 -> IRQ > 7 donc il faut envoyer un EOI au maitre et à l'esclave ca concerne les deux  */
			outb(0xA0, 0x20);		/* Envoi un signal de reset à l'esclave car >=59 -> irq esclave*/
		
		outb(0x20, 0x20);				/* Envoi un signal de reset au maitre.  La value de outb 0x20 = EOI */
	}

}
