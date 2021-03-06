#ifndef IO_H_
#define IO_H_

#include <k/types.h>

#define SERIAL_PORT (void *)0x3F8

int write(const char *buf, size_t count);

static inline void outb(u16 port, u8 val)
{
	//asm volatile ("outb %0, %1" : /* No output */ : "a"(val), "d"(port));
	asm volatile ("outb %1, %0" : : "dN" (port), "a" (val));
}

static inline u8 inb(u16 port)
{
	u8 res;

	asm volatile ("inb %1, %0" : "=&a"(res) : "d"(port));

	return res;
}

static inline void outw(u16 port, u16 val)
{
	asm volatile ("outw %0, %1" : /* No output */ : "a"(val), "d"(port));
}

static inline u16 inw(u16 port)
{
	u16 res;

	asm volatile ("inw %1, %0" : "=&a"(res) : "d"(port));

	return res;
}

#endif				/* !IO_H_ */