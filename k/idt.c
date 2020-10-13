// #include <system.h>
// #include <idt.h>
#include <k/types.h>
#include <string.h>

#include "idt.h"

#define MAX_IDT_ENTRIES 256

/* Defines an IDT entry */
struct __attribute__((packed)) idt_entry
{
	u16 base_lo;
	u16 sel;	/* Our kernel segment goes here! */
	u8 always0;	/* This will ALWAYS be set to 0! */
	u8 flags;	/* Set using the above table! */
	u16 base_hi;
};

struct __attribute__((packed)) idt_ptr
{
	u16 limit;
	u32 base;
};

/* Declare an IDT of 256 entries.  Although we will only use the
 * first 32 entries in this tutorial, the rest exists as a bit
 * of a trap.  If any undefined IDT entry is hit, it normally
 * will caused an "Unhandled Interrupt" exception.  Any descriptor
 * for which the 'presence' bit is cleared (0) will generate an
 * "Unhandled Interrupt" exception */
struct idt_entry idt[MAX_IDT_ENTRIES];
struct idt_ptr _idtp;

extern void _idt_load();

/* Use this function to set an entry in the IDT.  A lot simpler
 * than twiddling with the GDT ;) */
void add_idt_gate(u8 num, u32 base, u16 sel, u8 flags)
{
	/* Assign the base values */
	idt[num].base_lo = (base & 0xFFFF);
	idt[num].base_hi = (base >> 16) & 0xFFFF;

	/* Set the other fields */
	idt[num].sel = sel;
	idt[num].always0 = 0;
	idt[num].flags = flags;
}

/* Installs the IDT */
void init_idt()
{
	/* Sets the special IDT pointer up, just like in 'gdt.c' */
	_idtp.limit = (sizeof (struct idt_entry) * MAX_IDT_ENTRIES) - 1;
	_idtp.base = (u32)&idt;

	/* Before, clear the IDT space memory */
	memset((void*)&idt, 0, sizeof(struct idt_entry) * MAX_IDT_ENTRIES);

	/* Add the IDT interrupt handlers */
    // add_idt_gate( 0, (u32)isr0 , 0x08, 0x8E);
    // add_idt_gate( 1, (u32)isr1 , 0x08, 0x8E);
    // add_idt_gate(31, (u32)isr32, 0x08, 0x8E);

	/* The assembly function which will load the IDT */
    _idt_load();
}