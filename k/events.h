
#include <k/types.h>
#include <string.h>
#include "isr.h"


#ifndef EVENTS_H_
#define EVENTS_H_


#define GATE_INT (0b0110)
#define GAT_IRQ_ENABLE (1 << 0)
#define GATE_32 (1 << 3)


enum gate_perms{
	KERNEL = 0,
	USER = 3
};
int add_interrupt_handler(void *f, size_t n, int perms, int flags);

struct __attribute__((packed)) interrupt_context{
	u32 nb;

	u32 esi;
	u32 edi;
	u32 edx;
	u32 ecx;
	
	u16 ds;
	u16 gs;
	u16 fs;
	u16 es;

	u32 ebx;
	u32 eax;

    u32 err_code;
	u32 eip;
	u16 cs;
	u16 pad0;
	u32 eflags;
	u32 esp;
	u16 ss;
	u16 pad1;
	
};


void dump_interrupt_context(struct interrupt_context *c, int verbose);



extern struct {
	u8 bytes[ISR_ALIGN];
} isr_vector[ISR_COUNT];


#endif /* EVENTS_H_ */