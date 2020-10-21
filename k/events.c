
#include "events.h"

#define MAX_IDT_ENTRIES 256
#define ARRAY_SIZE(x)  (sizeof(x) / sizeof((x)[0]))

struct __attribute__ ((packed)) idt_e  //ce que le prof a fait
{
	u16 offset_low;
	u16 segment;
	u8 zero;
	u8 flags : 5;
	u8 dpl : 2;
	u8 p : 1;
	u16 offset_high;
};


struct __attribute__ ((packed))idtr {
	u16 limit;
	u32 base;
};

static struct idt_e idt[MAX_IDT_ENTRIES];

static int interrupt_add_gate(void *f, size_t n, int dpl, int flags)
{
	if (n >= ARRAY_SIZE(idt))
		return -1;
	
	idt[n].offset_low = (u32) f & -0xffff0000;
    idt[n].offset_high = ((u32) f & 0xffff0000) >> 16;
	idt[n].segment = 0x08;
	idt[n].zero = 0;
	idt[n].flags = flags;
	idt[n].dpl = dpl;
	idt[n].p = 1;

	return 0;
}

void *interrupt_handler[256];

int add_interrupt_handler(void *f, size_t n, int perms, int flags)
{
	if (n>= ARRAY_SIZE(interrupt_handler))
	    return 1;
	
	interrupt_handler[n] = f;

	return interrupt_add_gate(&isr_vector[n], n, perms, flags);
}

static const char *exceptions[31] = {
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


static void default_handler(struct interrupt_context *c)
{
	if (c->nb < 32)
	{
		if (exceptions[c->nb])
			printf("[!] Exception: %s(%u)\n", exceptions[c->nb], c->nb);
		else
			printf("[!] Exception: %u\n", c->nb);
	}

	else
		printf("[!] Unhandled interrupt: %u\n", c->nb);

    panic("Unhandled interrupt");
}

void dump_interrupt_context(struct interrupt_context *c, int verbose)
{
	printf("[!] error code=%d\n", c->err_code);
	printf("[!] eip=0x%x, esp=0x%x, eflag=0x%x\n",  c->eip, c->esp, c->eflags);
	printf("[!] cs=0x%x, ss=0x%x\n", c->cs, c->ss);

	if (verbose)
	{
		printf("[!] ds=0x%x, gs=0x%x, fs=0x%x, es=0x%x\n", c->ds, c->gs, c->fs, c->es);
		printf("[!] eax=0x%x, ebx=0x%x, ecx=0x%x, edc=0x%x, esi=0x%x, edi=0x%x\n", c->eax, c->ebx, c->ecx, c->edx, c->esi, c->edi);
	}
}



void load_idt(void)
{
	struct idtr idt_r;

	/* Sets the special IDT pointer up, just like in 'gdt.c' */
	idt_r.base = idt; /* idt base address */
	idt_r.limit = sizeof(idt) - 1; /* idt size - 1 */
	add_interrupt_handler((u32)isr_vector, 0, 0x08, 0x8E);
	struct interrupt_context *c;
	default_handler(c);
	/* Before, clear the IDT space memory */
	memset((void*)&idt, 0, sizeof(struct idt_e) * MAX_IDT_ENTRIES);
	__asm__ volatile("lidt %0\n"
	: /* no output */
	: "m" (idt_r)
	: "memory");
}
void init_interrupt(void)
{

}