
#ifndef INIT_TABLES_H_
#define INIT_TABLES_H_

#define MAX_IDT_ENTRIES 256
#define ISR_ALIGN 16
#define ISR_COUNT 256
#define SIZE_ARRAY(x)	(sizeof((x)) / sizeof((x)[0]))



/*************************************    GDT    *************************************/

static struct gdt_entry {
    u16 seg_lim_15_00 : 16;   /* Segment limit from 0 to 15th bit --> segment of 16 bits        */
    u16 base_15_00 : 16;      /* Segment base address from 0 to 15th bit --> segment of 16 bits */
    u8 base_23_16 : 8;       /* Segment base address from 16 to 23th bit --> segment of 8 bits */
    u8 access_byte : 8;      /* Segment with type, s, dpl, p                                   */
    u8 granularity: 8;       /* segment with granularity, operand size, l and avl           */
    u16 base_31_24 : 8;       /* Segment base address from 24 to 31th bit --> segment of 8 bits */
}__attribute__((packed));


static struct gdt_r   /* structure to load our GDT , called : gtd_r */
{
    u16 limit;   /* should be 16 bytes */
    u32 base;  /* should be 32 bytes */
}__attribute__((packed));

static void set_gdt_entry(unsigned index, u32 base_adress, u32 limit, u16 access, u16 flags);
static void init_gdt();
static void gdt_load();


/*************************************    FIN GDT    *************************************/



/*************************************    IDT    *************************************/

/* this structure describe an interrupt gate. */
struct idt_entry
{
	u16	base_15_00 : 16; /* The lower 16 bits of the address to jump to when this interrupt fires. */
	u16	segment_sel : 16;     /* Kernel segment selector. */
	u8 zero : 8; /* This must always be zero. */
	u8 flags : 8;   /* flags : p, dpl, others flags.. */
	u16 base_16_31 : 16; /* The upper 16 bits of the address to jump to. */
} __attribute__((packed));



/* a struct who pointed on idt_entry . */
struct idt_r
{
	u16 limit;
	u32 base;  /* The address of the first element in our idt_entry_t array. */
} __attribute__((packed));



void init_idt();
static int idt_set_gate(u32 index, u32 base, u16 segment, u8 flags);




/* These extern directives let us access the addresses of our ASM ISR
   handlers. */
extern void  isr0(void);
extern void  isr1(void);
extern void  isr2(void);
extern void  isr3(void);
extern void  isr4(void);
extern void  isr5(void);
extern void  isr6(void);
extern void  isr7(void);
extern void  isr8(void);
extern void  isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);


extern void  irq0(void);
extern void  irq1(void);
extern void  irq2(void);
extern void  irq3(void);
extern void  irq4(void);
extern void  irq5(void);
extern void  irq6(void);
extern void  irq7(void);
extern void  irq8(void);
extern void  irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);


/*************************************    FIN IDT    *************************************/





void init_UART();
void init_kernel();
static void init_table();



#endif