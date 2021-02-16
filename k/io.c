#include "io.h"
#include "init_tables.h"


// int write(const char *buf, size_t count)
// {
// 	char *serial = SERIAL_PORT;
// 	int b_count = 0;
// 	//outb(serial, 38400);
// 	for (size_t i = 0; i < count; i++)
// 		outb(serial, buf[i]);
// 	return b_count == count ? count : -1;
// }
