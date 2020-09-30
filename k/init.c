#include <k/types.h>
#include "io.h"
#include "init.h"

void init_kernel(void)
{
    init_uart();
    init_gdt();
}

/* 1. SERIAL PORT */
void init_uart(void)
{
    u8 ier = inb(SERIAL_PORT + 1);
    u8 fcr = inb(SERIAL_PORT + 2); //
    u8 lcr = inb(SERIAL_PORT + 3); //
    u16 rate = 38400;

    outb(SERIAL_PORT + 3, lcr | 1 << 7);
    outb(SERIAL_PORT + 0, (115200 / rate) & 255); // = 0x3
    outb(SERIAL_PORT + 1, (115200 / rate) >> 8);  // = 0x0
    outb(SERIAL_PORT + 3, lcr & ~(1 << 7));

    outb(SERIAL_PORT + 2, fcr | 1 << 1); //
    outb(SERIAL_PORT + 2, fcr | 1 << 2); //
    outb(SERIAL_PORT + 2, fcr | 1 << 6); //
    outb(SERIAL_PORT + 2, fcr | 1 << 7); //
	outb(SERIAL_PORT + 1, ier | 1 << 1); //
}

/* 2. MEMORY MANAGER */

static struct __attribute__((packed)){
    u16 seg_lim_15_00 : 16;
    u16 base_15_00 : 16;
    u16 base_23_16 : 8;
    u16 type : 4;
    u16 s : 1;
    u16 dpl : 2; /* 0 pour le kernel, 3 pour le user */
    u16 p : 1;
    u16 seg_lim_19_16 : 4;
    u16 avl : 1;
    u16 l : 1;
    u16 d_b : 1;
    u16 g : 1;
    u16 base_31_24 : 8;
} gdt[6];

enum descriptor_type {
    SYS,
    CODE_OR_DATA
};

void set_dgt_entry(unsigned index, u32 base_adress, u32 limit, u16 type, u16 dpl)
{
    gdt[index] = {
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
}

void init_gdt() {
    const u32 base_adress = 0;
    const u32 limit = 0xFFFFFFFF;

    gdt[0] = { 0 };                              /* null segment */
    set_dgt_entry(1, base_adress, limit, 10, 0); /* code segment */
    set_dgt_entry(2, base_adress, limit, 10, 0); /* data segment */
}

static struct __attribute__((packed))
{
    u16 limit;   /* should be 16 bytes */
    void *base;  /* should be 32 bytes */
} gdtr =
{
    .limit = sizeof(gdt),
    .base = gdt
};

static inline void
switch_to_protected(void)
{
    __asm__ volatile ("lgdt %[gdtr]\n"
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
                      : [gdtr]"m"(gdtr), [cs]"i"(0x8), [ds]"i"(0x10)
                      : "%eax");
}
