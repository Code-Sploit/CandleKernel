/*
*  kernel.c
*/

#include "kernel.h"

#include "../lib/stdio.h"

void kmain(void)
{
	const char *str = "my first kernel";

	kstd_write(str);

	return;
}
