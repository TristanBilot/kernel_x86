#ifndef ISR_H_
#define ISR_H_


#define IRQ0	32
#define IRQ1	33
#define IRQ2	34
#define IRQ3	35
#define IRQ4	36
#define IRQ5	37
#define IRQ6	38
#define IRQ7	39
#define IRQ8	40
#define IRQ9	41
#define IRQ10	42
#define IRQ11   43
#define IRQ12	44
#define IRQ13	45
#define IRQ14	46
#define IRQ15	47

struct interrupt_context {

	u32 ds;
	u32 edi;
	u32 esi;
	u32 ebp;
	u32 useless;
	u32 ebx;
	u32 edx;
	u32 ecx;
	u32 eax;

	u32 nb;
	u32 err_code;

	u32 eip;
	u32 cs;
	u32 eflags;
	u32 esp;
	u32 ss;
};

/*
 * Enables registration of callbacks for interrupts or IRQs.
 */
typedef	void (*isr_h)(struct interrupt_context *c);

void register_interrupt_handler(u32 n, isr_h handler);


#endif /* ISR_H */