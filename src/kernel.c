/*
*  kernel.c
*/

#include "../lib/kernel.h"
#include "../lib/stdio.h"

void kmain(void)
{
	const char *_msg1 = "Booting up: CandleKernel!";

	kstd_write(_msg1);

	return;
}
