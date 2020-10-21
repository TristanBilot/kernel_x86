#include <k/types.h>
#include "io.h"
#include "init.h"
#include "idt.h"
#include "events.h"
void init_kernel(void)
{
    init_uart();
    init_gdt();
    load_idt();
    //init_idt();
}

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

static struct __attribute__((packed)) gdt_entry {
    u16 seg_lim_15_00 : 16;   /* Segment limit from 0 to 15th bit --> segment of 16 bits        */
    u16 base_15_00 : 16;      /* Segment base address from 0 to 15th bit --> segment of 16 bits */
    u16 base_23_16 : 8;       /* Segment base address from 16 to 23th bit --> segment of 8 bits */
    u16 type : 4;             /* Segment type                                                   */
    u16 s : 1;                /* Descriptor type (0 = system; 1 = code or data)                 */
    u16 dpl : 2;              /* 0 pour le kernel, 3 pour le user                               */
    u16 p : 1;                /* Segment present                                                */
    u16 seg_lim_19_16 : 4;    /* Segment limit from 16 to 19th bit --> segment of 4 bits        */
    u16 avl : 1;              /* Available for use by system software                           */
    u16 l : 1;                /* 64-bit code segment (IA-32e mode only)                         */
    u16 d_b : 1;              /* Default operation size (0 = 16-bit segment; 1 = 32-bit segment)*/
    u16 g : 1;                /* Granularity                                                    */
    u16 base_31_24 : 8;       /* Segment base address from 24 to 31th bit --> segment of 8 bits */
};

static struct __attribute__((packed)){ /* Structure gtd for our 6 sections : Null segment, Kernel Code segment, Kernel Data segment, Userland Code segment, Userland Data segment, Task State Segment */
    struct gdt_entry entry;
} gdt[6];

enum descriptor_type {
    SYS,
    CODE_OR_DATA
};
void set_gdt_entry(unsigned index, u32 base_adress, u32 limit, u16 type, u16 dpl) /* Function to initialize an index of the gtd_entry structure */
{
    struct gdt_entry entry = {
        .l = 0,
        .avl = 0,
        .d_b = 1,
        .dpl = dpl,
        .g = 1,
        .p = 1,
        .s = CODE_OR_DATA,
        .type = type,
        .seg_lim_15_00 = limit & 0xFFFF,
        .seg_lim_19_16 = limit & 0xF0000,
        .base_15_00 = base_adress & 0xFFFF,
        .base_23_16 = base_adress & 0xFF0000,
        .base_31_24 = base_adress & 0xFF000000
    };
    gdt[index].entry = entry;
}


static struct __attribute__((packed))   /* structure to load our GDT , called : gtd_r */
{
    u16 limit;   /* should be 16 bytes */
    void *base;  /* should be 32 bytes */
} gdt_r;



static inline void
switch_to_protected(void)        /* Function to load our gtd and reload segment selectors*/
{
    __asm__ volatile ("lgdt %[gdt_r]\n"
                      "mov  %%cr0, %%eax\n"
                      "or      $1, %%eax\n"
                      "mov  %%eax, %%cr0\n"
                      "mov  %[ds], %%ax\n"
                      "mov   %%ax, %%ds\n"
                      "mov   %%ax, %%es\n"
                      "mov   %%ax, %%fs\n"
                      "mov   %%ax, %%gs\n"
                      "ljmp %[cs], $set_cs\n"
                      "set_cs:\n"
                      :
                      : [gdt_r]"m"(gdt_r), [cs]"i"(0x8), [ds]"i"(0x10)
                      : "%eax");
}


void init_gdt() {                           /* Function to init gtd */
    const u32 base_adress = 0;
    const u32 limit = 0xFFFFFFFF;

    struct gdt_entry null_entry = {0};
    gdt[0].entry = null_entry;                   /* null segment */
    set_gdt_entry(1, base_adress, limit, 10, 0); /* code segment */
    set_gdt_entry(2, base_adress, limit, 10, 0); /* data segment */

    gdt_r.base = gdt; /* gdt base address */
    gdt_r.limit = sizeof(gdt) - 1; /* gdt size - 1 */
    switch_to_protected();
}