/*
 * Copyright (c) LSE
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY LSE AS IS AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL LSE BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <k/kstd.h>
#include "multiboot.h"
#include "io.h"
#include "init_tables.h"
#include "kmalloc.h"

int write(const char *buf, size_t count) // write() sends up to count bytes from the buffer to the serial port COM1
{
	char *serial = SERIAL_PORT;
	int b_count = 0;
	for (size_t i = 0; i < count; i++)
	{
		outb(serial, buf[i]);
		b_count++;
	}
	return b_count == count ? count : -1;
}

void k_main(unsigned long magic, multiboot_info_t *info)
{
	(void)magic;
	(void)info;
	init_kernel();
	//init_test_memory(info);
	init_atapi(info);

	asm volatile("int $0");
	asm volatile("int $20");
	asm volatile("int $21");

	printf("APRES INTERRUPTION\n");

	char star[4] = "|/-\\";
	char *fb = (void *)0xb8000;

	for (unsigned i = 0; ; ) {
		*fb = star[i++ % 4];
	}
	for (;;)
		asm volatile ("hlt");
}