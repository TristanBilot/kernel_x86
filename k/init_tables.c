#include <k/types.h>
#include "io.h"
#include "init_tables.h"
#include "timer.h"

#define COM1 0x3f8

extern void	gdt_flush(u32 gdt_ptr);
extern void	idt_flush(u32 idt_p);



/* 1. SERIAL PORT */
void init_uart(void)                //Initialize uart 16550 for 38400 bps
{
    u8 ier = inb(SERIAL_PORT + 1);
    u8 fcr = inb(SERIAL_PORT + 2);
    u8 lcr = inb(SERIAL_PORT + 3);
    u16 rate = 38400;

    outb(SERIAL_PORT + 3, lcr | 1 << 7);
    outb(SERIAL_PORT + 0, (115200 / rate) & 255); // = 0x3
    outb(SERIAL_PORT + 1, (115200 / rate) >> 8);  // = 0x0
    outb(SERIAL_PORT + 3, lcr & ~(1 << 7));

    outb(SERIAL_PORT + 2, fcr | 1 << 1);
    outb(SERIAL_PORT + 2, fcr | 1 << 2);
    outb(SERIAL_PORT + 2, fcr | 1 << 6);
    outb(SERIAL_PORT + 2, fcr | 1 << 7);
	outb(SERIAL_PORT + 1, ier | 1 << 1);
}
/* 2. MEMORY MANAGER */

static struct gdt_entry gdt[5];
static struct gdt_r gdt_r;

static struct idt_entry idt_e[256];
static struct idt_r idt_r;

static void set_gdt_entry(unsigned index, u32 base, u32 limit, u16 access, u16 flags)/* Function to initialize an index of the gtd_entry structure */
{
    if (index >= SIZE_ARRAY(gdt))
        printf("ARRAY SIZE INCORRECT\n");


    gdt[index].base_15_00 = (base & 0xFFFF);   // set segment limit of 00-15
	gdt[index].base_23_16 = (base >> 16) & 0xFF; //set up base 16-23 bit
	gdt[index].base_31_24 = (base >> 24) & 0xFF; //set up base 24-31 bit*/

	gdt[index].seg_lim_15_00    = (limit & 0xFFFF); //set up base 00-15 bit
	gdt[index].granularity = (limit >> 16) & 0x0F;   //set segment of 16-19 

	gdt[index].granularity |= flags & 0xF0;  // granularity, operand size, zero and avl  -> flags
	gdt[index].access_byte = access; //set up type, s, dpl, p 

}


static void init_gdt(void)
{
	gdt_r.limit = sizeof(gdt) - 1;
	gdt_r.base  = (u32)&gdt;

	set_gdt_entry(0, 0, 0, 0, 0);                /* Null segment */
	set_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); /* Code segment 0xCF = 1100 1111 and 0x9A = 1001 1010 */  
	set_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); /* Data segment 0xCF = 1100 1111 and 0x92 = 1001 0010*/
	//set_gdt_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); /* User mode code segment */ 
	//set_gdt_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); /* User mode data segment */

	gdt_flush((u32)&gdt_r);
}


static int idt_set_gate(u32 n, u32 base, u16 sel, u8 flags)
{
	if (n >= SIZE_ARRAY(idt_e))
        return -1;
	idt_e[n].base_15_00 = base & 0xFFFF;
	idt_e[n].base_16_31 = (base >> 16) & 0xFFFF;

	idt_e[n].segment_sel= sel;
	idt_e[n].zero = 0;
	/* We must uncomment the OR below when we get to using user-mode. It
	  sets the interrupt gate's privilege level to 3. */
	idt_e[n].flags   = flags /* | 0x60 */;
    return 0;
}




void init_idt()
{
    idt_r.limit = sizeof(idt_e) - 1;
	idt_r.base  = (u32)&idt_e;

    /* Send ICW1 to PIC Master and to PIC Slave */
	outb(0x20, 0x11);
	outb(0xA0, 0x11);

	/* Send ICW2 : remap PICM to 0x20 and PICS to 0x28 */
	outb(0x21, 0x20);
	outb(0xA1, 0x28);			/* REMAP IRQ TABLE */

	/* Send ICW3 : IRQ2 -> connection to slave - setup cacading */
	outb(0x21, 0x04);			/* Master - command: 0x20, données: 0x21 */
	outb(0xA1, 0x02);			/* Slave - command: 0xA0, données: 0xA1 */
	/* Send ICW4 to PICM and to PICS - Environment Info */
	outb(0x21, 0x01);
	outb(0xA1, 0x01);


	/* Mask all interrupts */
	outb(0x21 , 0xff);
	outb(0xA1 , 0xff);

	/* Unmask all interrupts */
	// outb(0x21, 0x0);
	// outb(0xA1, 0x0);

	outb(0x21 , 0xfc);			// keyboard init	on active le IRQ 0 et  IRQ 1           1111 100
  	outb(0xA1 , 0xff);																	  //1111 1111

	outb(0x20, 0x20);
    outb(0xA1, 0x20);

	__asm__("sti\n");


    idt_set_gate( 0,  (u32)isr0, 0x08, 0x8E);
	idt_set_gate( 1,  (u32)isr1, 0x08, 0x8E);
	idt_set_gate( 2,  (u32)isr2, 0x08, 0x8E);
	idt_set_gate( 3,  (u32)isr3, 0x08, 0x8E);
	idt_set_gate( 4,  (u32)isr4, 0x08, 0x8E);
	idt_set_gate( 5,  (u32)isr5, 0x08, 0x8E);
	idt_set_gate( 6,  (u32)isr6, 0x08, 0x8E);
	idt_set_gate( 7,  (u32)isr7, 0x08, 0x8E);
	idt_set_gate( 8,  (u32)isr8, 0x08, 0x8E);
	idt_set_gate( 9,  (u32)isr9, 0x08, 0x8E);
	idt_set_gate(10, (u32)isr10, 0x08, 0x8E);
	idt_set_gate(11, (u32)isr11, 0x08, 0x8E);
	idt_set_gate(12, (u32)isr12, 0x08, 0x8E);
	idt_set_gate(13, (u32)isr13, 0x08, 0x8E);
	idt_set_gate(14, (u32)isr14, 0x08, 0x8E);
	idt_set_gate(15, (u32)isr15, 0x08, 0x8E);
	idt_set_gate(16, (u32)isr16, 0x08, 0x8E);
	idt_set_gate(17, (u32)isr17, 0x08, 0x8E);
	idt_set_gate(18, (u32)isr18, 0x08, 0x8E);
	idt_set_gate(19, (u32)isr19, 0x08, 0x8E);
	idt_set_gate(20, (u32)isr20, 0x08, 0x8E);
	idt_set_gate(21, (u32)isr21, 0x08, 0x8E);   /*		SET ISR  	*/
	idt_set_gate(22, (u32)isr22, 0x08, 0x8E);
	idt_set_gate(23, (u32)isr23, 0x08, 0x8E);
	idt_set_gate(24, (u32)isr24, 0x08, 0x8E);
	idt_set_gate(25, (u32)isr25, 0x08, 0x8E);
	idt_set_gate(26, (u32)isr26, 0x08, 0x8E);
	idt_set_gate(27, (u32)isr27, 0x08, 0x8E);
	idt_set_gate(28, (u32)isr28, 0x08, 0x8E);
	idt_set_gate(29, (u32)isr29, 0x08, 0x8E);
	idt_set_gate(30, (u32)isr30, 0x08, 0x8E);
	idt_set_gate(31, (u32)isr31, 0x08, 0x8E);

	idt_set_gate(32, (u32)irq0, 0x08, 0x8E);
	idt_set_gate(33, (u32)irq1, 0x08, 0x8E);
	idt_set_gate(34, (u32)irq2, 0x08, 0x8E);
	idt_set_gate(35, (u32)irq3, 0x08, 0x8E);
	idt_set_gate(36, (u32)irq4, 0x08, 0x8E);
	idt_set_gate(37, (u32)irq5, 0x08, 0x8E);
	idt_set_gate(38, (u32)irq6, 0x08, 0x8E);
	idt_set_gate(39, (u32)irq7, 0x08, 0x8E);
	idt_set_gate(40, (u32)irq8, 0x08, 0x8E);
	idt_set_gate(41, (u32)irq9, 0x08, 0x8E);
	idt_set_gate(42, (u32)irq10, 0x08, 0x8E);
	idt_set_gate(43, (u32)irq11, 0x08, 0x8E);
	idt_set_gate(44, (u32)irq12, 0x08, 0x8E);
	idt_set_gate(45, (u32)irq13, 0x08, 0x8E);
	idt_set_gate(46, (u32)irq14, 0x08, 0x8E);
	idt_set_gate(47, (u32)irq15, 0x08, 0x8E);



    idt_flush((u32)&idt_r);
}




void init_kernel(void)
{
    init_uart();
    init_gdt();
    init_idt();
	init_timer();
	init_keyboard();
}